#!/usr/bin/env python
from ROOT import TFile, TTree
import luigi
import luigi.contrib.external_program
import numpy as np
import scipy.constants
import pandas as pd
import uncertainties as u
import hammersley
from range_integer import *
import healpy as hp
import healpy_utils as hpu


def randdisk(num, r_max):
    theta = np.random.uniform(0, 2.0 * np.pi , num)
    r2 = np.random.uniform(0, 1, num)
    x = r_max * np.sqrt(r2) * np.cos(theta)
    y = r_max * np.sqrt(r2) * np.sin(theta)
    return x, y


class CreateExternalInput(luigi.Task):
    id = luigi.IntParameter()
    name = luigi.Parameter()
    n_side = luigi.IntParameter()
    ipix = luigi.IntParameter()
    
    r_max = luigi.FloatParameter(502.1 / 2.0)  # mm
    wavelength = luigi.FloatParameter(360)  # nm
    n_particles = luigi.IntParameter(10000, significant=False)
    
    def run(self):
        theta, phi = hpu.randsphere(self.n_side, self.ipix, self.n_particles)
        px = np.sin(theta) * np.cos(phi)
        py = np.sin(theta) * np.sin(phi)
        pz = np.cos(theta)
        x, y = randdisk(self.n_particles, self.r_max)
        # Create ROOT file with tree.
        self.output().makedirs()
        file = TFile(self.output().fn, "RECREATE")
        tree = TTree("event0", "event0")
        xi = np.zeros(1, dtype=float)
        yi = np.zeros(1, dtype=float)
        pxi = np.zeros(1, dtype=float)
        pyi = np.zeros(1, dtype=float)
        pzi = np.zeros(1, dtype=float)
        ti = np.zeros(1, dtype=float)
        wvli = np.ones(1, dtype=float) * self.wavelength
        wi = np.ones(1, dtype=float)
        tree.Branch("x", xi, "x/D")
        tree.Branch("y", yi, "y/D")
        tree.Branch("px", pxi, "px/D")
        tree.Branch("py", pyi, "py/D")
        tree.Branch("pz", pzi, "pz/D")
        tree.Branch("time", ti, "time/D")
        tree.Branch("wavelength", wvli, "wavelength/D")
        tree.Branch("weight", wi, "weight/D")
        for i in xrange(self.n_particles):
            xi[0] = x[i]
            yi[0] = y[i]
            pxi[0] = px[i]
            pyi[0] = py[i]
            pzi[0] = pz[i]
            tree.Fill()
        file.Write()
        file.Close()
        
    def output(self):
        return luigi.LocalTarget("./results/%s/famous-event-%d.root" % (self.name, self.id))


class Run(luigi.contrib.external_program.ExternalProgramTask):
    id = luigi.IntParameter()
    name = luigi.Parameter()
    n_side = luigi.IntParameter()
    ipix = luigi.IntParameter()

    nice_level = luigi.IntParameter(5, significant=False)
    n_particles = luigi.IntParameter(10000, significant=False)
    
    def requires(self):
        return self.clone(CreateExternalInput)
    
    def program_args(self):
        # Create macro file.
        mac = luigi.LocalTarget("./results/%s/famous.mac" % self.name)
        if not mac.exists():
            with mac.open("w") as o:
                print >> o, "/g4sipm/digitize/hits 0"
                print >> o, "/run/beamOn 1"
        return ["nice", "-n", self.nice_level, "./sim", "--mac", mac.fn, "--output", self.output().fn, "--input", self.input().fn]
                   
    def output(self):
        return luigi.LocalTarget("./results/%s/famous-%d.root" % (self.name, self.id))


class Transmission(luigi.Task):
    id = luigi.IntParameter()
    name = luigi.Parameter()
    n_sipms = luigi.IntParameter()
    n_side = luigi.IntParameter()
    ipix = luigi.IntParameter()
    
    n_particles = luigi.IntParameter(10000, significant=False)
    
    def requires(self):
        return self.clone(Run)
    
    def run(self):
        df = {}
        # Open output file.
        f = TFile(self.input().fn)
        # Update cached wavelength.
        gps = f.Get("generalParticleSourceMessenger")
        gps.GetEntry(0)
        df["wavelength"] = [scipy.constants.value("Planck constant in eV s") * scipy.constants.c / (gps.energyEMin * 1e-3)]
        theta, phi = hp.pix2ang(self.n_side, self.ipix)
        df["theta"] = [theta]
        df["phi"] = [phi]
        df["x"] = [np.sin(theta) * np.cos(phi)]
        df["y"] = [np.sin(theta) * np.sin(phi)]
        # Get number of particles.
        n = 10000
        # Get detected photons for each SiPM.
        n_sipm = 0.0
        for sipm_id in xrange(self.n_sipms):
            sipm = f.Get("g4sipmHits-%d" % sipm_id)
            if sipm != None:
                n_sipm += sipm.GetEntries()
                df["t_sipm_%d" % sipm_id] = u.ufloat(sipm.GetEntries(), np.sqrt(sipm.GetEntries())) / self.n_particles
        df["t_sipm"] = u.ufloat(n_sipm, np.sqrt(n_sipm)) / self.n_particles
        # Save to pandas dataframe
        df = pd.DataFrame(df)
        df.to_pickle(self.output().fn)
        # Export pixel positions
        output = luigi.LocalTarget("./results/%s/famous-pixel-positions.pkl" % (self.name))
        if not output.exists():
            x, y = [], []
            t = f.Get("famousModelPixelCoordinates")
            for i in xrange(t.GetEntries()):
                t.GetEntry(i)
                x.append(t.x)
                y.append(t.y)
            df = pd.DataFrame({"x":x, "y":y})
            df.to_pickle(output.fn)
        f.Close()
    
    def output(self):
        return luigi.LocalTarget("./results/%s/famous-%d.pkl" % (self.name, self.id))


class ParameterizationSimulation(luigi.Task):
    name = luigi.Parameter("parameterization-simulation")
    n_sipms = luigi.IntParameter(61)
    n_side = luigi.IntParameter(256)
    theta_max = luigi.FloatParameter(10.0)  # deg
    
    def requires(self):
        # Get angular coordinates of all pixels.
        pix = np.arange(hp.nside2npix(self.n_side))
        theta, phi = hp.pix2ang(self.n_side, pix)
        pix = pix[theta < np.deg2rad(self.theta_max)]
        phi = phi[theta < np.deg2rad(self.theta_max)]
        theta = theta[theta < np.deg2rad(self.theta_max)]
        # Create task arguments.
        task_kwargs = [dict(name=self.name, n_sipms=self.n_sipms, n_side=self.n_side, ipix=ipix) for ipix in pix]
        self.n_repititions = len(task_kwargs)
        #
        return RangeInteger(of=Transmission, task_kwargs=task_kwargs, stop=len(task_kwargs) + 1)
    
    def run(self):
        frames = []
        for i in xrange(1, self.n_repititions + 1):
            input = "./results/%s/famous-%d.pkl" % (self.name, i)
            frames.append(pd.read_pickle(input))
        df = pd.concat(frames, ignore_index=True)
        df.to_pickle(self.output().fn)
        
    def output(self):
        return luigi.LocalTarget("./results/%s/famous-%s.pkl" % (self.name, self.name))
    

if __name__ == "__main__":
    luigi.run(main_task_cls=ParameterizationSimulation)
