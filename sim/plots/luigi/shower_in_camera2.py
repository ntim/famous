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
import scipy.interpolate
import camera
import timeit
import collections

parser = argparse.ArgumentParser()
parser.add_argument("--shower", default="../../source/offline/resources/photons_qII_proton_lgE18_n1_4km.root")
parser.add_argument("--path", default="./results/parameterization-simulation/famous-parameterization-simulation.pkl")
parser.add_argument("--event", default=0, type=int)
args = parser.parse_args()

# Read parameterization.
param = pd.read_pickle("./results/parameterization-simulation/famous-parameterization-simulation.pkl")
# Read pixel coordinates.
pixels = pd.read_pickle("./results/parameterization-simulation/famous-pixel-positions.pkl")
sipm_pdf = np.array([unp.nominal_values(getattr(param, "t_sipm_%d" % i)) for i in xrange(len(pixels))])
# Get wavelength simulation results.
wvl_param = pd.read_pickle("./results/wavelength-simulation/famous-wavelength-simulation.pkl")
# Create wavelength interpolation
wvl_bins = np.arange(270, 1020, 10.0)
groups = wvl_param.groupby(np.digitize(wvl_param.wavelength, wvl_bins))
means = np.array([group.t_sipm_0.values.mean().nominal_value for name, group in groups])
wvl_interp = scipy.interpolate.interp1d(wvl_bins + 5.0, means)
# Create triangulation based on parameterization
triang = tri.Triangulation(param.x, param.y)
# Get triangle finder.
trifinder = triang.get_trifinder()
# Open shower.
from ROOT import TFile, TTree
file = TFile(args.shower)
# Get event.
start = timeit.default_timer()
event = file.Get("event%d" % args.event)
# Get all photons in the field of view of the telescope.
x, y, times, weights, wavelengths = [], [], [], [], []
theta_max = np.deg2rad(9.1467816849)
for entry_id in xrange(event.GetEntries()):
    event.GetEntry(entry_id)
    # Filter by aperture radius.
    if (event.x ** 2 + event.y ** 2) < camera.r2_in_m and np.arccos(-event.pz) < theta_max:
        xi, yi = camera.transform_dir_to_sky(event.py, event.px, event.pz)
        x.append(xi)
        y.append(yi)
        times.append(event.time)
        weights.append(event.weight)
        wavelengths.append(event.wavelength * 1e9)
# Convert everything to numpy arrays
x = np.array(x)
y = np.array(y)
times = np.array(times)
weights = np.array(weights)
wavelengths = np.array(wavelengths)
# Find triangles
idx = trifinder(x, y)
# Cut not found values
selector = idx >= 0
times = times[selector]
weights = weights[selector]
wavelengths = wavelengths[selector]
idx = idx[selector]
# Get triangles
found_idx = triang.triangles[idx]
# Loop over all SiPMs
photons = sipm_pdf[:, found_idx].mean(axis=2) * weights * wvl_interp(wavelengths) / wvl_interp(360.0)
# Compute number of photons for each pixel
pixel_photons = photons.sum(axis=1)
# Compute average pixel time
pixel_times = [np.average(times, weights=photons[i]) if pixel_photons[i] >= 1.0 else np.nan for i in xrange(len(pixel_photons))]

# Find triangle.
#         
#         if idx >= 0:
#         # Get point index.
#             idx = triang.triangles[idx]
#             # Loop over all pixels.
#             pdf = sipm_pdf[:,idx].mean(axis=1)
#             prod = pdf * event.weight
#             for sipm_id in xrange(len(pdf)):
#                 # Compute weight.
#                 if prod[sipm_id] > 0.0:
#                     weights[sipm_id].append(prod[sipm_id])
#                     times[sipm_id].append(event.time)
#                     wvl[sipm_id].append(event.wavelength * 1e9)
# # Apply wavelength dependence afterwards for improved performance.
# photons = [np.array(weights[i]) * wvl_interp(wvl[i]) / wvl_interp(360.0) for i in xrange(len(weights))]
# pixel_photons = np.array([np.sum(photons[i]) for i in xrange(len(photons))])
# pixel_times = [np.average(times[i], weights=photons[i]) if pixel_photons[i] >= 1.0 else np.nan for i in xrange(len(times))]
# pixel_photons[pixel_photons < 1.0] = np.nan
# pixel_photons = np.ma.masked_invalid(pixel_photons)

print timeit.default_timer() - start, "sec"

# Plot camera
f, ax = plt.subplots()
ax.set_aspect("equal")
ax.set_xlabel("x / mm")
ax.set_ylabel("y / mm")
camera.set_lim(ax, pixels)
coll = camera.collection(pixels)
coll.set_array(np.ma.masked_invalid(pixel_photons))
ax.add_collection(coll)
cb = f.colorbar(coll)
cb.set_label("number of photons")
cb.solids.set_rasterized(True)
ax.text(0.025, 0.975, "FAMOUS simulation", ha="left", va="top", transform=ax.transAxes)
    
# Plot camera
f, ax = plt.subplots()
ax.set_aspect("equal")
ax.set_xlabel("x / mm")
ax.set_ylabel("y / mm")
camera.set_lim(ax, pixels)
coll = camera.collection(pixels)
coll.set_array(np.ma.masked_invalid(pixel_times))
ax.add_collection(coll)
cb = f.colorbar(coll)
cb.set_label("peak arrival time of photons / ns")
cb.solids.set_rasterized(True)
ax.text(0.025, 0.975, "FAMOUS simulation", ha="left", va="top", transform=ax.transAxes)
    
# Plot camera
f, ax = plt.subplots()
ax.set_aspect("equal")
ax.set_xlabel("x / mm")
ax.set_ylabel("y / mm")
camera.set_lim(ax, pixels)
def radius(i):
    r = camera.radius(pixels)
    v = r * pixel_photons[i] / np.ma.max(pixel_photons)
    if np.isnan(v) or np.ma.is_masked(v) or v < 1.0:
        return 1.0
    return v
circles = [Circle((xi, yi), radius(i)) for i, xi, yi in zip(xrange(len(pixels.x)), pixels.x.values, pixels.y.values)]
coll = PatchCollection(circles, cmap="viridis", edgecolor="k")
coll.set_array(np.ma.masked_invalid(pixel_times))
ax.add_collection(coll)
cb = f.colorbar(coll)
cb.set_label("peak arrival time of photons / ns")
cb.solids.set_rasterized(True)
ax.text(0.025, 0.975, "FAMOUS simulation", ha="left", va="top", transform=ax.transAxes)

"""    
for i, xi, yi in zip(xrange(len(pixels)), pixels.x, pixels.y):
    ax.text(xi, yi, str(i), ha="center", va="center", color="k")
"""

plt.show()
