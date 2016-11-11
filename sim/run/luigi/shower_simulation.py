#!/usr/bin/env python
import os
import glob
import luigi
import pandas as pd
import numpy as np
import uncertainties.unumpy as unp
import matplotlib.tri as tri
import scipy.interpolate
import scipy.sparse
import timeit
import collections
from ROOT import TFile, TTree


class WavelengthSimulation(luigi.ExternalTask):
    
    def output(self):
        return luigi.LocalTarget("results/wavelength-simulation/famous-wavelength-simulation.pkl")


class ParameterizationSimulation(luigi.ExternalTask):
    
    def output(self):
        return luigi.LocalTarget("results/parameterization-simulation/famous-parameterization-simulation.pkl")


class FamousPixelPositions(luigi.ExternalTask):
    
    def output(self):
        return luigi.LocalTarget("results/parameterization-simulation/famous-pixel-positions.pkl")


class FamousModel(luigi.WrapperTask):
    
    def requires(self):
        return [WavelengthSimulation(), ParameterizationSimulation(), FamousPixelPositions()]
    
    def wvl_interp(self):
        if hasattr(self, "_wvl_interp"):
            return self._wvl_interp
        print "Reading", self.input()[0].fn
        wvl_param = pd.read_pickle(self.input()[0].fn)
        wvl_bins = np.arange(270, 1020, 10.0)
        groups = wvl_param.groupby(np.digitize(wvl_param.wavelength, wvl_bins))
        means = np.array([group.t_sipm_0.values.mean().nominal_value for name, group in groups])
        wvl_interp = scipy.interpolate.interp1d(wvl_bins + 5.0, means)
        # Rescale interpolation by wavelength of parameterization simulation
        self._wvl_interp = scipy.interpolate.interp1d(wvl_bins + 5.0, means / wvl_interp(360.0))
        return self._wvl_interp
    
    def param(self):
        if hasattr(self, "_param"):
            return self._param
        print "Reading", self.input()[1].fn
        self._param = pd.read_pickle(self.input()[1].fn)
        return self._param
        
    
    def triang(self):
        if hasattr(self, "_triang"):
            return self._triang
        self._triang = tri.Triangulation(self.param().x, self.param().y)
        # self._triang = scipy.spatial.Delaunay(np.vstack((self.param().x, self.param().y)).T)
        return self._triang
    
    def sipm_pdf(self):
        if hasattr(self, "_sipm_pdf"):
            return self._sipm_pdf
        self._sipm_pdf = np.array([unp.nominal_values(getattr(self.param(), "t_sipm_%d" % i)) for i in xrange(len(self.pixels()))])
        return self._sipm_pdf
    
    def pixels(self):
        if hasattr(self, "_pixels"):
            return self._pixels
        print "Reading", self.input()[2].fn
        self._pixels = pd.read_pickle(self.input()[2].fn)
        return self._pixels


class ShowerSimulation(luigi.Task):
    input = luigi.Parameter("../../source/offline/resources/fl_E_18_0_0_rp_6.root")

    r = luigi.FloatParameter(502.1 / 2.0)  # mm
    theta_max = luigi.FloatParameter(np.deg2rad(9.1467816849))
    
    def transform_dir_to_sky(self, px, py, pz):
        # Convert to spherical coordinates
        r = np.sqrt(px ** 2 + py ** 2 + pz ** 2)
        theta = np.arccos(pz / r)
        phi = np.arctan2(py, px)
        # Transform angles into projections
        return np.sin(theta) * np.cos(phi), np.sin(theta) * np.sin(phi)
    
    def create_output_dict(self, entries, n_sipms):
        output_dict = {}
        output_dict["log_e"] = np.zeros(entries) 
        output_dict["azimuth"] = np.zeros(entries) 
        output_dict["zenith"] = np.zeros(entries) 
        output_dict["theta"] = np.zeros(entries) 
        output_dict["phi"] = np.zeros(entries) 
        output_dict["r"] = np.zeros(entries) 
        output_dict["x"] = np.zeros(entries) 
        output_dict["y"] = np.zeros(entries) 
        output_dict["z"] = np.zeros(entries) 
        output_dict["x_max"] = np.zeros(entries) 
        for sipm_id in xrange(n_sipms):
            output_dict["photons_%d" % sipm_id] = np.zeros(entries) 
            output_dict["times_%d" % sipm_id] = np.zeros(entries) 
        return output_dict
    
    def compute_event(self, event, famous, triang, trifinder, sipm_pdf, wvl_interp):
        r2_in_m = (self.r / 1000.0) ** 2
        x, y, times, weights, wavelengths = [], [], [], [], []
        for entry_id in xrange(event.GetEntries()):
            event.GetEntry(entry_id)
            # Filter by aperture radius.
            if (event.x ** 2 + event.y ** 2) < r2_in_m and np.arccos(-event.pz) < self.theta_max:
                xi, yi = self.transform_dir_to_sky(event.py, event.px, event.pz)
                x.append(xi)
                y.append(yi)
                times.append(event.time)
                weights.append(event.weight)
                wavelengths.append(event.wavelength * 1e9)
        # Convert everything to numpy arrays
        if len(x) == 0:
            return np.zeros(61), np.zeros(61)
        x = np.array(x)
        y = np.array(y)
        times = np.array(times)
        weights = np.array(weights)
        wavelengths = np.array(wavelengths)
        # Find triangles
        idx = trifinder(x, y)
        # Cut not found values
        selector = idx >= 0
        times = times[selector]
        weights = weights[selector]
        wavelengths = wavelengths[selector]
        idx = idx[selector]
        # Get triangles
        found_idx = triang.triangles[idx]
        # Loop over all SiPMs
        photons = sipm_pdf[:, found_idx].mean(axis=2) * weights * wvl_interp(wavelengths) / wvl_interp(360.0)
        # Compute number of photons for each pixel
        pixel_photons = photons.sum(axis=1)
        # Compute average pixel time
        pixel_times = [np.average(times, weights=photons[i]) if pixel_photons[i] > 0.0 else np.nan for i in xrange(len(pixel_photons))]
        return pixel_photons, pixel_times
    
    def run(self):
        famous = FamousModel()
        triang = famous.triang()
        trifinder = triang.get_trifinder()
        sipm_pdf = famous.sipm_pdf()
        wvl_interp = famous.wvl_interp()
        # Read events.
        input_file = TFile(self.input)
        header = input_file.Get("header")
        # Create output dictionary.
        output_dict = self.create_output_dict(header.GetEntries(), len(sipm_pdf))
        # Loop over all events.
        for event_id in xrange(header.GetEntries()):
            start = timeit.default_timer()
            header.GetEntry(event_id)
            event = input_file.Get("event%d" % event_id)
            # Create data structures
            photons, times = self.compute_event(event, famous, triang, trifinder, sipm_pdf, wvl_interp)
            # Create tree.
            output_dict["log_e"][event_id] = header.logE
            output_dict["azimuth"][event_id] = header.azimuth
            output_dict["zenith"][event_id] = header.zenith
            output_dict["theta"][event_id] = header.theta
            output_dict["phi"][event_id] = header.phi
            output_dict["r"][event_id] = header.r
            output_dict["x"][event_id] = header.x
            output_dict["y"][event_id] = header.y
            output_dict["z"][event_id] = header.z
            output_dict["x_max"][event_id] = header.xMax
            for sipm_id in xrange(len(photons)):
                # Calculate sum of detected photons.
                output_dict["photons_%d" % sipm_id][event_id] = photons[sipm_id]
                # Calculate peaking time.
                output_dict["times_%d" % sipm_id][event_id] = times[sipm_id]
            #
            print "Event", event_id, timeit.default_timer() - start, "sec"
            # Free memory.
            event.IsA().Destructor(event)
        # Close input file.
        input_file.Close()
        #
        df = pd.DataFrame(output_dict)
        df.to_pickle(self.output().fn)
        
    def output(self):
        return luigi.LocalTarget(os.path.splitext(self.input)[0] + ".pkl")
    

class All(luigi.Task):
    basedir = luigi.Parameter()
    
    def requires(self):
        inputs = glob.glob(os.path.join(self.basedir, "**", "photons.root"))
        return [ShowerSimulation(input=input) for input in inputs]
    
    def run(self):
        frames = []
        inputs = self.input()
        for input in inputs:
            frames.append(pd.read_pickle(input.fn))
        df = pd.concat(frames, ignore_index=True)
        df.to_pickle(self.output().fn)
        
    def output(self):
        return luigi.LocalTarget(os.path.join(self.basedir, "famous_info.pkl"))
    
if __name__ == '__main__':
    luigi.run()
