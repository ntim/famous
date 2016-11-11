#!/usr/bin/env python
import luigi
import luigi.contrib.external_program
import numpy as np
import scipy
import scipy.constants
import image_analysis
from ROOT import TFile, TTree
from image_analysis import *
import pandas as pd

class Run(luigi.contrib.external_program.ExternalProgramTask):
    id = luigi.IntParameter()
    name = luigi.Parameter()
    theta = luigi.Parameter()  # degrees
    
    nice_level = luigi.IntParameter(5)
    n_particles = luigi.IntParameter(10000)
    wavelength = luigi.FloatParameter(550)  # nm
    
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
        # Execute simulation.
        output = self.output()[1]
        return [#"ulimit", "-s", "32768", "&&", 
                "nice", "-n", self.nice_level,
                "./fresnelSim", "--mac", mac.fn, "--output", output.fn]
                   
    def output(self):
        return [luigi.LocalTarget("./results/%s/fresnel-%d.mac" % (self.name, self.id)), 
                luigi.LocalTarget("./results/%s/fresnel-%d.root" % (self.name, self.id)) ]


class DiscreteThetaSimulation(luigi.Task):
    name = luigi.Parameter("discrete-theta-simulation")
    
    def requires(self):
        if hasattr(self, "_cached_deps"):
            return self._cached_deps
        deps = []
        i = 0
        for theta in xrange(0, 13):
            deps.append(Run(id=i, name=self.name, theta=theta))
            i += 1
        self._cached_deps = deps
        return deps
    
    def run(self):
        index = np.zeros(512 * len(self.input()))
        z = np.zeros(512 * len(self.input()))
        r90 = np.zeros(512 * len(self.input()))
        for i, input in enumerate(self.input()):
            # Read theta.
            f = TFile(input[1].fn)
            gps = f.Get("generalParticleSourceMessenger")
            gps.GetEntry(0)
            theta = np.rad2deg(gps.angleThetaMin)
            f.Close()
            # Read rays.
            p, m = read(input[1].fn)
            # Filter rays which are far away from the centroid
            p, m = filter(None, p, m)
            # Find minimum r90.
            x_r90_min = find_minimum_aberration_radius(p, m)
            # Calculate r90 along z around r90 minimum.
            index[i * 512:(i + 1) * 512] = theta
            z_test = np.linspace(-24 + x_r90_min, 24 + x_r90_min, 512)
            z[i * 512:(i + 1) * 512] = z_test
            r90[i * 512:(i + 1) * 512] = np.asarray([get_aberration_radius(zi, p, m) for zi in z_test])
            #
        dataframe = pd.DataFrame({"z": z, "r90":r90}, index=index)
        dataframe.to_pickle(self.output().fn)
    
    def output(self):
        return luigi.LocalTarget("./results/%s/fresnel-r90-discrete-theta.pkl" % (self.name))
    

if __name__ == "__main__":
    luigi.run(main_task_cls=DiscreteThetaSimulation)
