#!/usr/bin/env python
import luigi
import luigi.contrib.external_program
import numpy as np
import scipy
import scipy.constants
import image_analysis
import pandas as pd
import uncertainties as u
from ROOT import TFile, TTree


class Run(luigi.contrib.external_program.ExternalProgramTask):
    id = luigi.IntParameter()
    name = luigi.Parameter()
    wavelength = luigi.FloatParameter()
    
    nice_level = luigi.IntParameter(5)
    theta = luigi.Parameter(0)  # degrees
    n_particles = luigi.IntParameter(10000)
    
    def program_args(self):
        # Create macro file.
        mac = self.output()[0]
        if not mac.exists():
            with mac.open("w") as o:
                e = scipy.constants.value("Planck constant in eV s") * scipy.constants.c / (self.wavelength * 1e-9)
                print >> o, "/gps/angle/thetaMin %.18f deg" % self.theta
                print >> o, "/gps/angle/thetaMax %.18f deg" % self.theta
                print >> o, "/gps/angle/phiMin 0 deg"
                print >> o, "/gps/angle/phiMax 0 deg"
                print >> o, "/gps/energy/eMin %.18f eV" % e 
                print >> o, "/gps/energy/eMax %.18f eV" % e
                print >> o, "/gps/nParticles %d" % self.n_particles
                print >> o, "/run/beamOn 1"
        output = self.output()[1]
        return ["nice", "-n", self.nice_level,
                "./fresnelSim", "--mac", mac.fn, "--output", output.fn]
                   
    def output(self):
        return [luigi.LocalTarget("./results/%s/fresnel-%d.mac" % (self.name, self.id)),
                luigi.LocalTarget("./results/%s/fresnel-%d.root" % (self.name, self.id)) ]


class R90Min(luigi.Task):
    id = luigi.IntParameter()
    name = luigi.Parameter()
    wavelength = luigi.FloatParameter()
    
    def requires(self):
        return self.clone(Run)
    
    def run(self):
        # Update cached theta.
        f = TFile(self.input()[1].fn)
        gps = f.Get("generalParticleSourceMessenger")
        gps.GetEntry(0)
        # Update cached wavelength.
        self.wavelength = scipy.constants.value("Planck constant in eV s") * scipy.constants.c / (gps.energyEMin * 1e-3)
        # Read rays
        p, m = image_analysis.read(self.input()[1].fn)
        # Nothing came through.
        if len(p) == 0:
            df = pd.DataFrame({"wavelength": [self.wavelength],
                               "centroid_x": [0],
                               "centroid_y": [0],
                               "centroid_z": [0],
                               "z_r90": [np.nan], "r90": [np.nan],
                               "z_rms": [np.nan], "rms":[np.nan],
                               "t": u.ufloat(0, 0)})
            df.to_pickle(self.output().fn)
            return
        # Get centroid.
        centroid = image_analysis.get_centroid(None, p, m)
        # Filter rays wich are too far away from the centroid
        p, m = image_analysis.filter(None, p, m)
        # Determine minimum and its position of R90
        z_r90_min = image_analysis.find_minimum_aberration_radius(p, m)
        r90_min = image_analysis.get_aberration_radius(z_r90_min, p, m)
        # Determine minimum and its position of RMS
        z_rms_min = image_analysis.find_minimum_rms(p, m)
        rms_min = image_analysis.get_rms_spot_size(z_rms_min, p, m)
        # Calculate transmission
        trans = u.ufloat(len(p), np.sqrt(len(p)))
        trans = trans / gps.nParticles
        f.Close()
        # Write to pandas DataFrame for later merging.
        df = pd.DataFrame({"wavelength": [self.wavelength],
                            "centroid_x": [0],
                            "centroid_y": [0],
                            "centroid_z": [0],
                            "z_r90": [z_r90_min], "r90": [r90_min],
                            "z_rms": [z_rms_min], "rms":[rms_min],
                            "t": trans})
        df.to_pickle(self.output().fn)
        
    def output(self):
        return luigi.LocalTarget("./results/%s/fresnel-%d.pkl" % (self.name, self.id))

    
class R90WavelengthSimulation(luigi.Task):
    name = luigi.Parameter("wavelength-simulation")
    n_repititions = luigi.IntParameter(1000)
    
    def requires(self):
        if hasattr(self, "_cached_deps"):
            return self._cached_deps
        deps = []
        for i, wavelength in enumerate(np.random.uniform(200, 1000, self.n_repititions)):
            deps.append(R90Min(id=i, name=self.name, wavelength=wavelength))
        self._cached_deps = deps
        return deps
    
    def run(self):
        frames = []
        for input in self.input():
            frames.append(pd.read_pickle(input.fn))
        df = pd.concat(frames, ignore_index=True)
        df.to_pickle(self.output().fn)
        
    def output(self):
        return luigi.LocalTarget("./results/%s/fresnel-wavelength-simulation.pkl" % self.name)
    

if __name__ == "__main__":
    luigi.run(main_task_cls=R90WavelengthSimulation)
    
