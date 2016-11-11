#!/usr/bin/env python
# -*- coding: utf-8 -*-

from ROOT import TFile, TTree
import matplotlib.pyplot as plt
import numpy as np
import sys, os, glob

path = "./results/efficiency/*.root"
if len(sys.argv) > 1:
    path = sys.argv[1]
files = glob.glob(path)
    
def get(filename):
    # Open root file and retreive tree
    file = TFile(filename)
    n = 0
    try:
        n = file.Get("wicoBackHits").GetEntries()
    except:
        pass
    n_sipm = 0
    for i in xrange(64):
        try:
            hits = file.Get("g4sipmHits-%d" % i)
            if hits != None:
                n_sipm += hits.GetEntries()
        except:
            pass
    # Determine nTot
    gps = file.Get("generalParticleSourceMessenger")
    gps.GetEntry(0)
    nParticles = gps.nParticles
    angleThetaMax = gps.angleThetaMax
    anglePhiMax = gps.anglePhiMax
    #
    file.Close()
    return n * 1.0, n_sipm * 1.0, nParticles * 1.0, angleThetaMax, anglePhiMax
    
x = []
y = []
y_sipm = []
yerr = []
yerr_sipm = []
for file in files:
    n, n_sipm, n_tot, theta, phi = get(file)
    if phi != 0:
        continue
    x.append(theta * 180.0 / np.pi)
    y.append(n / n_tot)
    y_sipm.append(n_sipm / n_tot)
    yerr.append(np.sqrt(n) / n_tot)
    yerr_sipm.append(np.sqrt(n_sipm) / n_tot)

# Plot histogram.
plt.errorbar(x, y, yerr=yerr, fmt="o", label="Winston cone exit")
plt.errorbar(x, y_sipm, yerr=yerr_sipm, fmt="_", label="Detected by SiPM")
plt.xlabel(r"incidence angle $\theta$ / $^\circ$")
plt.ylabel(r"efficiency")
plt.legend()
plt.show()
