#!/usr/bin/env python
# -*- coding: utf-8 -*-

import matplotlib.pyplot as plt
import numpy as np
import sys, os, glob
import argparse
from image_analysis import *

parser = argparse.ArgumentParser()
parser.add_argument("--path", help="the path to a ROOT output file")
parser.add_argument("--bins", help="the bin width", default=128, type=int)
args = parser.parse_args()

# Include root after parsing the arguments.
from ROOT import TFile, TTree

# Open the most current root file.
path = args.path
if args.path is None:
    path = glob.glob("./results/*.root")[-1]
    
def wavelength(energy):
    '''
    @param energy - the energy in eV.
    @return wavelenght in millimeter.
    '''
    return 4.13566e-12 * 299.792458 / (energy * 1e-6) * 1e-6

def read_w_opl(filename):
    file = TFile(filename)
    # Get general run information.
    gps = file.Get("generalParticleSourceMessenger")
    gps.GetEntry(0)
    wvl = wavelength(gps.energyEMin)
    theta = gps.angleThetaMax
    # Get hits and optical path length.
    hits = file.Get("hits")
    p = np.zeros((hits.GetEntries(), 3))
    m = np.zeros((hits.GetEntries(), 3))
    opd = np.zeros(hits.GetEntries())
    for i in xrange(hits.GetEntries()):
        hits.GetEntry(i)
        p[i] = hits.position
        m[i] = hits.momentum
        opd[i] = hits.opticalPathLength
    file.Close()
    return p, m, opd, wvl, theta

# Read data.
p, m, opd, wvl, theta = read_w_opl(path)

# Filter rays which do not reach the spot.
r_max = get_rms_spot_size(None, p, m) * 1.
pos_norm = np.linalg.norm(p - get_centroid(None, p, m), axis=1)
p = p[pos_norm < r_max]
opd = opd[pos_norm < r_max]
m = m[pos_norm < r_max]

# Calculate path terminuses.
p0 = np.zeros(m.shape)
for i in xrange(m.shape[0]):
    p0[i] = p[i] - opd[i] * m[i]

# Normalize pupil coordinates.
r_max = max(p0[:, 0].max(), p0[:, 1].max())
rho = np.sqrt(p0[:, 0] ** 2 + p0[:, 1] ** 2)
p0 = p0 / r_max

# Normalize optical path length.
chief_idx = np.argmin(rho)
r = opd[chief_idx]
ref_sphere = np.zeros(m.shape[0])
# Calculate distance difference to reference sphere
for i in xrange(m.shape[0]):
    cprime = p[chief_idx] - p[i] 
    ref_sphere[i] = np.dot(cprime, m[i]) + np.sqrt(np.linalg.norm(np.dot(cprime, m[i])) ** 2 - np.linalg.norm(cprime) ** 2 + r ** 2)
    
opd = opd - ref_sphere

# Histogram
range = ((-1, 1), (-1, 1))
H_num, xedges, yedges = np.histogram2d(p0[:, 0], p0[:, 1], bins=args.bins, range=range)
H_opd, _, _ = np.histogram2d(p0[:, 0], p0[:, 1], bins=args.bins, range=range, weights=opd)
# Compute mean entries per bin.
H_num = np.ma.masked_array(H_num, mask=H_num == 0)
H = np.ma.divide(H_opd, H_num)
# Save result to external file.
H.dump(os.path.splitext(args.path)[0] + ".opd.np")

# Plot fan diagram.
fig, ax = plt.subplots()
ax.plot(xedges[:-1], H[:, H.shape[1] / 2] , label="x fan")
ax.plot(yedges[:-1], H[H.shape[0] / 2, ] , label="y fan")
ax.set_xlabel("exit pupil x / y")
ax.set_ylabel("optical path length / waves")

# Plot
fig, ax = plt.subplots()
im = ax.imshow(H, interpolation="nearest", cmap="rainbow", extent=[-1, 1, -1, 1])
ax.set_aspect('equal')

cb = fig.colorbar(im, format='%.1e')
cb.set_label(r"optical path length / waves")

ax.set_xlabel("exit pupil x")
ax.set_ylabel("exit pupil y")
fig.tight_layout()

plt.show()
