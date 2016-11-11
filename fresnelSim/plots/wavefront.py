#!/usr/bin/env python
# -*- coding: utf-8 -*-

import matplotlib.pyplot as plt
import numpy as np
import sys, os, glob
import argparse
from image_analysis import *
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

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
    p0 = np.zeros((hits.GetEntries(), 3))
    p = np.zeros((hits.GetEntries(), 3))
    m = np.zeros((hits.GetEntries(), 3))
    opd = np.zeros(hits.GetEntries())
    for i in xrange(hits.GetEntries()):
        hits.GetEntry(i)
        p0[i] = hits.startPosition
        p[i] = hits.position
        m[i] = hits.momentum
        opd[i] = hits.opticalPathLength
    file.Close()
    return p0, p, m, opd, wvl, theta

# Read data.
p0, p, m, opd, wvl, theta = read_w_opl(path)

# Filter rays which do not reach the spot.
r_max = get_rms_spot_size(None, p, m) * 2.0
pos_norm = np.linalg.norm(p - get_centroid(None, p, m), axis=1)
p0 = p0[pos_norm < r_max]
p = p[pos_norm < r_max]
opd = opd[pos_norm < r_max]
m = m[pos_norm < r_max]

# Pupil coordinates (not yet normalized).
# rho = np.sqrt(p0[:, 0] ** 2 + p0[:, 1] ** 2)
# Get chief ray index.
# chief_idx = np.argmin(rho)

# Calculate path terminuses.
p1 = np.zeros(p.shape)
for i in xrange(p.shape[0]):
    p1[i] = p[i] - opd[i] * m[i]
r_max = max(p1[:, 0].max(), p1[:, 1].max())
chief_idx = np.argmin(np.sqrt(p1[:, 0] ** 2 + p1[:, 1] ** 2))

# pos_norm = np.linalg.norm(p - get_centroid(None, p, m), axis=1)
# chief_idx = np.argmin(pos_norm)

# chief_idx = np.argmin(np.arccos(-m[:,2]) - theta)
# print np.rad2deg(np.arccos(-m[chief_idx][2]))

p_afoc = np.array([0, 0, p1[chief_idx][2]])
nz_afoc = np.array([-np.sin(theta) * np.cos(0), -np.sin(theta) * np.sin(0), -np.cos(theta)])

# Plot wavefront.
c = np.zeros(p.shape[0])
for i in xrange(p.shape[0]):
    # Reference plane
    c[i] = np.dot(p_afoc - p1[i], nz_afoc) / np.dot(nz_afoc, nz_afoc) - np.dot(p1[chief_idx] - p1[i], m[chief_idx]) / np.dot(m[i], m[chief_idx])
    
# fig, ax = plt.subplots()
# trip = ax.tripcolor(p1[:, 0], p1[:, 1], opd - opd[chief_idx], cmap="rainbow")
# cb = fig.colorbar(trip)
# cb.set_label("OPL afocal image space / mm")
# ax.set_aspect('equal')

# Histogram
range = ((-1, 1), (-1, 1))
H_num, xedges, yedges = np.histogram2d(p1[:, 0] / r_max, p1[:, 1] / r_max, bins=args.bins, range=range)
H_opd, _, _ = np.histogram2d(p1[:, 0] / r_max, p1[:, 1] / r_max, bins=args.bins, range=range, weights=c / wvl)
# Compute mean entries per bin.
H_num = np.ma.masked_array(H_num, mask=H_num == 0)
H = np.fliplr(np.ma.divide(H_opd, H_num).T)
# Save result to external file.
H.dump(os.path.splitext(args.path)[0] + ".opd.np")

# Plot wavefront error.
fig, ax = plt.subplots()
im = ax.imshow(H, interpolation="nearest", cmap="rainbow", extent=[-1, 1, -1, 1])
cb = fig.colorbar(im)
cb.set_label("wavefront error / waves")
ax.set_aspect('equal')

# Plot ray fans.
fig, ax = plt.subplots()
ax.plot(xedges[:-1], H[:, H.shape[1] / 2] , label="x fan")
ax.plot(yedges[:-1], H[H.shape[0] / 2, ] , label="y fan")
ax.set_xlabel("exit pupil x / y")
ax.set_ylabel("optical path length / waves")

plt.show()
