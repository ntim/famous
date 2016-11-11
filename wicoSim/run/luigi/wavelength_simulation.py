#!/usr/bin/env python
from ROOT import TFile, TTree
import luigi
import luigi.contrib.external_program
import numpy as np
import scipy
import scipy.constants
import pandas as pd
import uncertainties as u


class Run(luigi.contrib.external_program.ExternalProgramTask):
    id = luigi.IntParameter()
    name = luigi.Parameter()
    wavelength = luigi.FloatParameter()  # nm
    
    nice_level = luigi.IntParameter(5)
    n_particles = luigi.IntParameter(10000)
    
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
                print >> o, "/gps/energy/eMin %.18f eV" % e 
                print >> o, "/gps/energy/eMax %.18f eV" % e
                print >> o, "/gps/nParticles %d" % self.n_particles
                print >> o, "/run/beamOn 1"
       # Execute simulation.
        output = self.output()[1]
        return [  # "ulimit", "-s", "32768", "&&", 
                "nice", "-n", self.nice_level,
                "./wicoSim", "--mac", mac.fn, "--output", output.fn, "--sipm", "null"]
                   
    def output(self):
        return [luigi.LocalTarget("./results/%s/wico-%d.mac" % (self.name, self.id)), luigi.LocalTarget("./results/%s/wico-%d.root" % (self.name, self.id)) ]


class Transmission(luigi.Task):
    id = luigi.IntParameter()
    name = luigi.Parameter()
    wavelength = luigi.Parameter()  # deg
    
    def requires(self):
        return self.clone(Run)
    
    def run(self):
        # Update cached theta.
        f = TFile(self.input()[1].fn)
        gps = f.Get("generalParticleSourceMessenger")
        gps.GetEntry(0)
        hits = f.Get("hits")
        # Calculate transmission
        trans = u.ufloat(hits.GetEntries(), np.sqrt(hits.GetEntries()))
        trans = trans / gps.nParticles
        f.Close()
        # Write to pandas DataFrame for later merging.
        df = pd.DataFrame({"wavelength": [self.wavelength], "t": [trans]})
        df.to_pickle(self.output().fn)
        
    def output(self):
        return luigi.LocalTarget("./results/%s/wico-%d.pkl" % (self.name, self.id))


class WavelengthSimulation(luigi.Task):
    name = luigi.Parameter("wavelength-simulation")
    
    def requires(self):
        if hasattr(self, "_cached_deps"):
            return self._cached_deps
        deps = []
        wavelength = np.arange(200, 1000, 4)
        for i in xrange(len(wavelength)):
            deps.append(Transmission(id=i, name=self.name, wavelength=wavelength[i]))
        self._cached_deps = deps
        return deps
    
    def run(self):
        frames = []
        for input in self.input():
            frames.append(pd.read_pickle(input.fn))
        df = pd.concat(frames, ignore_index=True)
        df.to_pickle(self.output().fn)
        
    def output(self):
        return luigi.LocalTarget("./results/%s/wico-wavelength-simulation.pkl" % self.name)


if __name__ == "__main__":
    luigi.run(main_task_cls=WavelengthSimulation)
