#!/usr/bin/env python
# -*- coding: utf-8 -*-

import matplotlib.pyplot as plt
import numpy as np
import sys, os, glob
from ROOT import TFile, TTree
from image_analysis import *
import numpy.ma as ma

# Open the most current root file.
path = "./results/*.root"
if len(sys.argv) > 1:
    path = sys.argv[1]
files = glob.glob(path)
files.sort()

def wavelength(energy):
    '''
    @param energy - the energy in eV.
    @return wavelenght in millimeter.
    '''
    return 4.13566e-12 * 299.792458 / (energy * 1e-6) * 1e-6

def read_w_opl(filename):
    file = TFile(filename)
    hits = file.Get("hits")
    #
    p0 = np.zeros((hits.GetEntries(), 3))
    p1 = np.zeros((hits.GetEntries(), 3))
    m = np.zeros((hits.GetEntries(), 3))
    opl = np.zeros(hits.GetEntries())
    for i in xrange(hits.GetEntries()):
        hits.GetEntry(i)
        p0[i] = hits.startPosition
        p1[i] = hits.position
        m[i] = hits.momentum
        opl[i] = hits.opticalPathLength
    file.Close()
    return p0, p1, m, opl

def get_rmax(p, m):
    p, m = filter(None, p, m)
    return get_rms_spot_size(None, p, m)

# Histogram
p0, p1, m, opl = read_w_opl(files[-1])

# Filter rays which do not reach the spot.
r_max = get_rmax(p1, m) * 2.0
pos_norm = np.linalg.norm(p1 - get_centroid(None, p1, m), axis=1)
p0 = p0[np.where(pos_norm < r_max)]
opl = opl[np.where(pos_norm < r_max)]

p = p0.transpose()

# Find chief ray.
r = np.sqrt(p[0] ** 2 + p[1] ** 2)
chief_opl = opl[np.argmin(r)]

# Subtract the reference sphere.
f = chief_opl
dx = r / np.sin(np.arctan(r / f)) - f

# Subtract chief ray optical path difference from the rest.
opl -= chief_opl + dx

# Divide optical path length by the wavelength.
file = TFile(files[-1])
gps = file.Get("generalParticleSourceMessenger")
gps.GetEntry(0)
wvl = wavelength(gps.energyEMin)
theta = gps.angleThetaMax * 180.0 / np.pi
file.Close()

opl /= wvl

# Compute the wavefront Peak-to-Valley error
wPV = np.max(opl) - np.min(opl)
# Compute the RMS wavefront error
omega = np.std(opl)
# Compute the strehl ratio
# strehl = 1.0 / np.exp((2.0 * np.pi * omega) ** 2) 

bins = 96
r_max = np.max(r)
range = ((-r_max, r_max), (-r_max, r_max))
H_num, xedges, yedges = np.histogram2d(p[0], p[1], bins=bins, range=range)
H_opl, _, _ = np.histogram2d(p[0], p[1], bins=bins, range=range, weights=opl)

H = ma.masked_invalid(H_opl / H_num)

X, Y = np.meshgrid(xedges, yedges)
pc = plt.pcolormesh(X, Y, H, cmap=plt.get_cmap('RdBu'))
pc.set_rasterized(True)
plt.axes().set_aspect('equal')

cb = plt.colorbar(format='%.1e')
cb.set_label("wavefront error / number of waves")

text = "$\theta = %.2f deg$\n" % theta
text += "$ W_{P-V} = %.2e$\n" % wPV
text += "$\omega = %.2e$\n" % omega
# text += "$S = %.2e$\n" % strehl
plt.text(0.05, 0.95, text, ha="left", va="top", transform=plt.axes().transAxes)

plt.xlim(xedges[0], xedges[-1])
plt.ylim(yedges[0], yedges[-1])
plt.xlabel("x / mm")
plt.ylabel("y / mm")
plt.tight_layout()
plt.show()
