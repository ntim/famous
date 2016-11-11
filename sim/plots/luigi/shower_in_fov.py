#!/usr/bin/env python
import os
import argparse
import numpy as np
import pandas as pd
import uncertainties.unumpy as unp
import matplotlib.pyplot as plt
import matplotlib.tri as tri
from matplotlib.collections import PatchCollection, Collection
from matplotlib.patches import Circle, Polygon
import camera
import timeit
import collections

parser = argparse.ArgumentParser()
parser.add_argument("--shower", default="../../source/offline/resources/photons_qII_proton_lgE18_n1_4km.root")
parser.add_argument("--path", default="./results/parameterization-simulation/famous-parameterization-simulation.pkl")
parser.add_argument("--event", default=0, type=int)
args = parser.parse_args()

# Read parameterization
param = pd.read_pickle(args.path)
# Read pixel coordinates
famous = pd.read_pickle(os.path.join(os.path.dirname(args.path), "famous-pixel-positions.pkl"))
# Create triangulation based on parameterization
triang = tri.Triangulation(param.x, param.y)
# Get triangle finder.
finder = triang.get_trifinder()
# Open shower.
from ROOT import TFile, TTree
file = TFile(args.shower)
# Get event.
event = file.Get("event%d" % args.event)
header = file.Get("header")
header.GetEntry(args.event)
# Get all photons in the field of view of the telescope.
photons = collections.defaultdict(list)
times = collections.defaultdict(list)
theta_max = np.deg2rad(10.0)
start = timeit.default_timer()
for i in xrange(event.GetEntriesFast()):
    event.GetEntry(i)
    # Filter by aperture radius.
    if (event.x ** 2 + event.y ** 2) < camera.r2_in_m:
        if np.arccos(-event.pz) < theta_max:
            xi, yi = camera.transform_dir_to_sky(event.py, event.px, event.pz)
            idx = finder(xi, yi)
            if idx >= 0:
                # Get point index
                p = triang.triangles[idx][0]
                # Save photon weight, ie. the number of photons in this bunch.
                photons[p].append(event.weight)
                times[p].append(event.time)

# Sum up photons per sky bin.
photon_sum = np.array([np.sum(photons[i]) for i in xrange(len(param))])
# Get all pixel pdf.
pdf = unp.nominal_values(param.t_sipm.values)
# Weight number of photons with the pdf.
folded = photon_sum * pdf
# Compute weighted average for timing.
times_avg = np.array([np.average(times[i], weights=photons[i]) if photon_sum[i] > 0 else 0 for i in xrange(len(param))])

print timeit.default_timer() - start, "sec"

fig, ax = plt.subplots()
ax.set_aspect("equal")
ax.set_xlabel(r"field of view $\sin(\theta) \cdot \cos(\phi)$")
ax.set_ylabel(r"field of view $\sin(\theta) \cdot \sin(\phi)$")
cax = ax.tripcolor(triang, photon_sum, vmin=0, cmap="viridis", rasterized=True)
ax.set_axis_bgcolor(plt.get_cmap("viridis")(0))
cb = fig.colorbar(cax)
cb.set_label("number of photons")
cb.solids.set_rasterized(True)
ax.text(0.025, 0.975, r"number of photons in FOV $N_\gamma = %d$" % np.sum(photon_sum), color="white", transform=ax.transAxes, ha="left", va="top")

# Plot shower core
x_core, y_core = camera.transform_dir_to_sky(header.y, header.x, header.z)
#plt.plot(-x_core, -y_core, "o", color="w")

# Compute shower direction
x_dir = np.sin(header.theta) * np.cos(header.phi)
y_dir = np.sin(header.theta) * np.sin(header.phi)
#plt.plot(y_dir, x_dir, "o", color="w")
ax.plot([-x_core, y_dir], [-y_core, x_dir], "-", color="w")
ax.set_xlim(param.x.min(), param.x.max())
ax.set_ylim(param.x.min(), param.x.max())


"""
fig, ax = plt.subplots()
ax.set_aspect("equal")
ax.set_xlabel(r"field of view $\sin(\theta) \cdot \cos(\phi)$")
ax.set_ylabel(r"field of view $\sin(\theta) \cdot \sin(\phi)$")
cax = ax.tripcolor(triang, folded, vmin=0, cmap="viridis", rasterized=True)
ax.set_axis_bgcolor(plt.get_cmap("viridis")(0))
cb = fig.colorbar(cax)
cb.set_label("number of detected photons")
cb.solids.set_rasterized(True)
ax.text(0.025, 0.975, r"number of detected photons $N_\gamma = %d$" % np.sum(folded), color="white", transform=ax.transAxes, ha="left", va="top")

fig, ax = plt.subplots()
ax.set_aspect("equal")
ax.set_xlabel(r"field of view $\sin(\theta) \cdot \cos(\phi)$")
ax.set_ylabel(r"field of view $\sin(\theta) \cdot \sin(\phi)$")
cax = ax.tripcolor(triang, times_avg, vmin=times_avg[times_avg > 0].min(), vmax=times_avg.max(), cmap="viridis", rasterized=True)
ax.set_axis_bgcolor(plt.get_cmap("viridis")(0))
cb = fig.colorbar(cax)
cb.set_label("average arrival time of photons / ns")
cb.solids.set_rasterized(True)
ax.text(0.025, 0.975, r"number of detected photons $N_\gamma = %d$" % np.sum(folded), color="white", transform=ax.transAxes, ha="left", va="top")

circles = [camera.circle((xi, yi), camera.radius(famous)) for xi, yi in zip(famous.x, famous.y)]
for i, p in enumerate(circles):
    circles[i] = np.array(camera.transform_camera_to_sky(p[:, 0], p[:, 1])).T
ax.add_collection(PatchCollection([Polygon(p) for p in circles], edgecolor="w", facecolor="none", linewidth=1, alpha=0.5))

# Annotate pixels with id.
for i, xi, yi in zip(xrange(len(famous)), famous.x, famous.y):
    xi, yi = camera.transform_camera_to_sky(xi, yi)
    ax.text(xi, yi, str(i), ha="center", va="center", color="k")
"""
plt.show()
