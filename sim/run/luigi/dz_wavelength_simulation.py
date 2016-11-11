#!/usr/bin/env python
from ROOT import TFile, TTree
import luigi
import luigi.contrib.external_program
import numpy as np
import scipy.constants
import pandas as pd
import uncertainties as u
from range_integer import *


class Run(luigi.contrib.external_program.ExternalProgramTask):
    id = luigi.IntParameter()
    name = luigi.Parameter()
    wavelength = luigi.FloatParameter() # nm
    dz = luigi.FloatParameter() # mm
    
    nice_level = luigi.IntParameter(5, significant=False)
    n_particles = luigi.IntParameter(3000, significant=False)
    ug11_filter_thickness = luigi.FloatParameter(1, significant=False)
    
    def program_args(self):
        # Create macro file.
        mac = self.output()[0]
        if not mac.exists():
            with mac.open("w") as o:
                e = scipy.constants.value("Planck constant in eV s") * scipy.constants.c / (self.wavelength * 1e-9)
                print >> o, "/gps/energy/eMin %.18f eV" % e 
                print >> o, "/gps/energy/eMax %.18f eV" % e
                print >> o, "/gps/nParticles %d" % self.n_particles
                print >> o, "/gps/angle/thetaMin 0 deg"
                print >> o, "/gps/angle/thetaMax 0.75 deg"
                print >> o, "/gps/angle/phiMin 0 deg"
                print >> o, "/gps/angle/phiMax 360 deg"
                print >> o, "/g4sipm/digitize/hits 0"
                print >> o, "/run/beamOn 1"
        output = self.output()[1]
        return ["nice", "-n", self.nice_level,
                "./sim", "--mac", mac.fn, "--output", output.fn, "--dz", self.dz, "--ug11-filter-thickness", self.ug11_filter_thickness]
                   
    def output(self):
        return [luigi.LocalTarget("./results/%s/famous-%d.mac" % (self.name, self.id)),
                luigi.LocalTarget("./results/%s/famous-%d.root" % (self.name, self.id))]
        
        
class Transmission(luigi.Task):
    id = luigi.IntParameter()
    name = luigi.Parameter()
    wavelength = luigi.FloatParameter()
    dz = luigi.FloatParameter()
    n_sipms = luigi.IntParameter()
    
    def requires(self):
        return self.clone(Run)
    
    def run(self):
        df = {}
        # Open output file.
        f = TFile(self.input()[1].fn)
        # Update cached wavelength.
        gps = f.Get("generalParticleSourceMessenger")
        gps.GetEntry(0)
        self.wavelength = scipy.constants.value("Planck constant in eV s") * scipy.constants.c / (gps.energyEMin * 1e-3)
        df["wavelength"] = [self.wavelength]
        # Get number of particles.
        n = gps.nParticles
        df["n"] = [gps.nParticles]
        # Update cached dz.
        lensModel = f.Get("fresnelLensModel")
        lensModel.GetEntry(0)
        self.dz = lensModel.dz
        z = lensModel.dz + lensModel.f
        df["dz"] = [self.dz]
        df["z"] = [z]
        # Get transmission after lens.
        lens = f.Get("fresnelLensBackHits")
        df["t_after_lens"] = u.ufloat(lens.GetEntries(), np.sqrt(lens.GetEntries())) / n
        # Get transmission at focal plane.
        focal = f.Get("focalPlaneHits")
        df["t_focal_plane"] = u.ufloat(focal.GetEntries(), np.sqrt(focal.GetEntries())) / n
        # Get transmission after Winston cones.
        wico = f.Get("wicoBackHits")
        df["t_after_wico"] = u.ufloat(wico.GetEntries(), np.sqrt(wico.GetEntries())) / n
        # Get detected photons for each SiPM.
        n_sipm = 0.0
        for sipm_id in xrange(self.n_sipms):
            sipm = f.Get("g4sipmHits-%d" % sipm_id)
            if sipm != None:
                n_sipm += sipm.GetEntries()
                if sipm_id > 0:
                    continue
                df["t_sipm_%d" % sipm_id] = u.ufloat(sipm.GetEntries(), np.sqrt(sipm.GetEntries())) / n
        df["t_sipm"] = u.ufloat(n_sipm, np.sqrt(n_sipm)) / n
        f.Close()
        # Save to pandas dataframe
        df = pd.DataFrame(df)
        df.to_pickle(self.output().fn)
    
    def output(self):
        return luigi.LocalTarget("./results/%s/famous-%d.pkl" % (self.name, self.id))
        
        
class DzWavelengthSimulation(luigi.Task):
    name = luigi.Parameter("dz-wavelength-simulation")
    n_sipms = luigi.IntParameter(61)
    n_repititions = luigi.IntParameter(2 ** 13)
    
    def requires(self):
        # The hammersley creates points evenly distributed on a plane.
        task_kwargs = [dict(dz=dz, wavelength=wavelength, name=self.name, n_sipms=self.n_sipms) for wavelength, dz in hammersley.plane(self.n_repititions, (270, 1020), (-64, 64))]
        return RangeInteger(of=Transmission, task_kwargs=task_kwargs, stop=self.n_repititions + 1)
    
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
    luigi.run(main_task_cls=DzWavelengthSimulation)
    
