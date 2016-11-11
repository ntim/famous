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


class Run(luigi.contrib.external_program.ExternalProgramTask):
    id = luigi.IntParameter()
    name = luigi.Parameter()
    n_side = luigi.IntParameter()
    ipix = luigi.IntParameter()

    nice_level = luigi.IntParameter(5, significant=False)
    wavelength = luigi.FloatParameter(360)  # nm
    n_particles = luigi.IntParameter(10000, significant=False)
    
    def program_args(self):
        # Create macro file.
        mac = luigi.LocalTarget("./results/%s/famous-%d.mac" % (self.name, self.id))
        if not mac.exists():
            theta, phi = hp.pix2ang(self.n_side, self.ipix)
            with mac.open("w") as o:
                e = scipy.constants.value("Planck constant in eV s") * scipy.constants.c / (self.wavelength * 1e-9)
                print >> o, "/g4sipm/digitize/hits 0"
                print >> o, "/gps/energy/eMin %.18f eV" % e 
                print >> o, "/gps/energy/eMax %.18f eV" % e
                print >> o, "/gps/nParticles %d" % self.n_particles
                print >> o, "/gps/angle/thetaMin %.18f rad" % theta
                print >> o, "/gps/angle/thetaMax %.18f rad" % theta
                print >> o, "/gps/angle/phiMin %.18f rad" % phi
                print >> o, "/gps/angle/phiMax %.18f rad" % phi
                print >> o, "/g4sipm/digitize/hits 0"
                print >> o, "/run/beamOn 1"
        return ["nice", "-n", self.nice_level, "./sim", "--mac", mac.fn, "--output", self.output().fn]
                   
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
