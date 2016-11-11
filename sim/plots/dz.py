#!/usr/bin/env python
# -*- coding: utf-8 -*-

import matplotlib.pyplot as plt
import numpy as np
import sys, os, glob
import matplotlib.tri as tri
import numpy.ma as ma
from ROOT import TFile, TTree

deg = np.pi / 180.0

path = "./results/dz"
if len(sys.argv) > 1:
    path = sys.argv[1]
files = glob.glob(path + "/*.root")
files.sort()

dzs = np.zeros(len(files))
eff = np.zeros(len(files))
eff_sipm = np.zeros(len(files))

def get(filename):
    # Open root file and retreive tree
    file = TFile(filename)
    n = 0
    try:
        n = file.Get("wicoBackHits").GetEntriesFast()
    except:
        pass
    n_sipm = 0
    for i in xrange(64):
        try:
            hits = file.Get("g4sipmHits-%d" % i)
            if hits != None:
                n_sipm += hits.GetEntriesFast()
        except:
            pass
    # Determine nTot
    gps = file.Get("generalParticleSourceMessenger")
    gps.GetEntry(0)
    nParticles = gps.nParticles
    # Determin dz.
    model = file.Get("famousModel")
    model.GetEntry(0)
    dz = model.dz
    #
    file.Close()
    return n, n_sipm, nParticles, dz

for i, file in enumerate(files):
    n_hit, n_sipm, n_tot, dz = get(file)
    dzs[i] = dz
    eff[i] = n_hit / float(n_tot)
    eff_sipm[i] = n_sipm / float(n_tot)
    
idx = np.argsort(dzs)
eff = eff[idx]
eff_sipm = eff_sipm[idx]
    
plt.plot(dzs, eff * 100.0, label="Winston cone exit, optimum dz = %.3f mm" % dzs[np.argmax(eff)])
plt.plot(dzs, eff_sipm * 100.0, label="SiPM, optimum dz = %.3f mm" % dzs[np.argmax(eff_sipm)])
plt.xlabel("dz / mm")
plt.ylabel("efficiency / %")
plt.legend(loc="upper right")
plt.show()