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
    grooves = luigi.Parameter()  # grooves per mm
    
    nice_level = luigi.IntParameter(5)
    n_particles = luigi.IntParameter(10000)
    wavelength = luigi.FloatParameter(550)  # nm
    
    def program_args(self):
        # Create macro file.
        mac = self.output()[0]
        if not mac.exists():
            with mac.open("w") as o:
                e = scipy.constants.value("Planck constant in eV s") * scipy.constants.c / (self.wavelength * 1e-9)
                print >> o, "/gps/angle/thetaMin 0 deg"
                print >> o, "/gps/angle/thetaMax 0 deg"
                print >> o, "/gps/angle/phiMin 0 deg"
                print >> o, "/gps/angle/phiMax 0 deg"
                print >> o, "/gps/energy/eMin %.8f eV" % e 
                print >> o, "/gps/energy/eMax %.8f eV" % e
                print >> o, "/gps/nParticles %d" % self.n_particles
                print >> o, "/run/beamOn 1"
       # Execute simulation.
        output = self.output()[1]
        return [  # "ulimit", "-s", "32768", "&&", 
                "nice", "-n", self.nice_level,
                "./fresnelSim", "--mac", mac.fn, "--output", output.fn, "--grooves", self.grooves]
                   
    def output(self):
        return [luigi.LocalTarget("./results/%s/fresnel-%d.mac" % (self.name, self.id)), luigi.LocalTarget("./results/%s/fresnel-%d.root" % (self.name, self.id)) ]


class R90Min(luigi.Task):
    id = luigi.IntParameter()
    name = luigi.Parameter()
    grooves = luigi.Parameter()  # grooves per mm
    
    def requires(self):
        return self.clone(Run)
    
    def run(self):
        # Read
        p, m = image_analysis.read(self.input()[1].fn)
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
        # Update cached theta.
        f = TFile(self.input()[1].fn)
        gps = f.Get("generalParticleSourceMessenger")
        gps.GetEntry(0)
        # Calculate transmission
        trans = u.ufloat(len(p), np.sqrt(len(p)))
        trans = trans / gps.nParticles
        f.Close()
        # Write to pandas DataFrame for later merging.
        df = pd.DataFrame({"grooves": [self.grooves],
                           "centroid_x": [centroid[0][0]],
                           "centroid_y": [centroid[0][1]],
                           "centroid_z": [centroid[0][2]],
                           "z_r90": [z_r90_min], "r90": [r90_min],
                           "z_rms": [z_rms_min], "rms":[rms_min],
                           "t": trans})
        df.to_pickle(self.output().fn)
        
    def output(self):
        return luigi.LocalTarget("./results/%s/fresnel-%d.pkl" % (self.name, self.id))


class R90GroovesSimulation(luigi.Task):
    name = luigi.Parameter("grooves-simulation")
    
    def requires(self):
        if hasattr(self, "_cached_deps"):
            return self._cached_deps
        deps = []
        grooves = np.concatenate((np.arange(1, 13), [30]))
        grooves = [10, 20, 30]
        n_repititions = len(grooves) * 100
        for i in xrange(n_repititions):
            deps.append(R90Min(id=i, name=self.name, grooves=grooves[i % len(grooves)]))
        self._cached_deps = deps
        return deps
    
    def run(self):
        frames = []
        for input in self.input():
            frames.append(pd.read_pickle(input.fn))
        df = pd.concat(frames, ignore_index=True)
        df.to_pickle(self.output().fn)
        
    def output(self):
        return luigi.LocalTarget("./results/%s/fresnel-grooves-simulation.pkl" % self.name)


if __name__ == "__main__":
    luigi.run(main_task_cls=R90GroovesSimulation)
