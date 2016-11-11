#!/usr/bin/env python
# -*- coding: utf-8 -*-

import matplotlib.pyplot as plt
import numpy as np
import sys, os, glob
import matplotlib.tri as tri
from matplotlib.collections import PatchCollection, Collection
from matplotlib.patches import Circle, Polygon, CirclePolygon
import numpy.ma as ma
from ROOT import TFile, TTree
import hex
import sig
import sim
import cache

deg = np.pi / 180.0

path = "./results/efficiency"
if len(sys.argv) > 1:
    path = sys.argv[1]
files = glob.glob(path + "/*.root")
files.sort()

# Load famous model
famousModel = sim.FamousModel(files[0])
pixels = famousModel.pixels_to_cart(polygons=False)
pixel_centers = famousModel.pixel_centers_to_cart()
pixels = [Polygon(p) for p in pixels]

def find(point, pixels):
    for i, p in enumerate(pixels):
        if p.contains_point(point):
            return i
    return None

def read(filename):
    # Open root file and retreive tree
    file = TFile(filename)
    hits = file.Get("wicoBackHits")
    n = hits.GetEntriesFast()
    pos = np.zeros((n, 3))
    for i in xrange(n):
        hits.GetEntry(i)
        pos[i] = hits.position
    # Determine nTot
    gps = file.Get("generalParticleSourceMessenger")
    gps.GetEntry(0)
    n = gps.nParticles
    theta = gps.angleThetaMax
    phi = gps.anglePhiMax
    #
    return n, theta, phi, pos

n = np.zeros(len(files))
theta = np.zeros(len(files))
phi = np.zeros(len(files))
n_pix = np.zeros(len(files))
n_tot = np.zeros(len(files))
direction = np.zeros((len(files), 2))
center_id = [None] * len(files)

# Cache
if os.path.exists(path + "/cache_light_loss.npy"):
    cached_files, n, theta, phi, n_pix, n_tot, direction, center_id = cache.read(path + "/cache_light_loss.npy", (files, n, theta, phi, n_pix, n_tot, direction, center_id))
    if np.array_equal(cached_files, files):
        print "Using cache ..."
        files = []

for i, filename in enumerate(files):
    print i
    n[i], theta[i], phi[i], points = read(filename)
    n_tot[i] = len(points)
    # Determine center pixel from angular coordinates
    direction[i] = famousModel.ang_to_cart(theta[i], phi[i])
    center_id[i] = find(direction[i], pixels)
    # If simulation fell into an pixel, continue
    if center_id[i] == None:
        n_tot[i] = np.nan
        continue
    # Count the number of photons which have hit the found pixel
    for point in points:
        if pixels[center_id[i]].contains_point(point):
            n_pix[i] += 1

# Cache
if len(files) > 0:
    print "Writing cache ..."
    cache.write(path + "/cache_light_loss.npy", (files, n, theta, phi, n_pix, n_tot, direction, center_id))
    
# Integrate efficiency per pixel.
pixel_effs = np.zeros(len(pixels))
pixel_effs_num = np.zeros(len(pixels))
for i, dir in enumerate(direction):
    if center_id[i] == None:
        continue
    pixel_effs[center_id[i]] += n_pix[i] / n[i]
    pixel_effs_num[center_id[i]] += 1
    
pixel_effs = 100.0 * pixel_effs / pixel_effs_num

# Compute efficiency of the central pixel.       
eff = ma.fix_invalid(n_pix / n, fill_value=0.0).data * 100.0

# Create a new figure.
fig = plt.figure()
# Create polar axes.
rect = [0.1, 0.1, 0.8, 0.85]
ax_polar1 = fig.add_axes(rect, polar=True, frameon=True)
ax_polar1.grid(False)
ax_polar1.get_xaxis().set_ticks([])

# Create normal axes to draw the tripcolor.
ax = fig.add_axes(rect, frameon=False)
ax.get_xaxis().set_ticks([])
ax.get_yaxis().set_ticks([])
ax.set_aspect('equal')

# Plot
collection = PatchCollection(pixels, edgecolor="black", color="None")
cax = None
trip = False
if trip:
    triang = tri.Triangulation(direction[:, 0], direction[:, 1])
    # eff_mean = eff[triang.triangles].mean(axis=1)
    # triang.set_mask(np.where(eff_mean <= 0, 1, 0))
    cax = ax.tripcolor(triang, eff, cmap=plt.get_cmap("rainbow"))
    cax.set_rasterized(True)
else:
    collection = PatchCollection(pixels, edgecolor="black", cmap=plt.get_cmap("rainbow"))
    collection.set_array(pixel_effs)
    cax = collection
ax.add_collection(collection)

#
ax.set_xlim(np.min(direction[:, 0]), np.max(direction[:, 0]))
ax.set_ylim(np.min(direction[:, 1]), np.max(direction[:, 1]))

# Create second polar axes for the ticks.
ax_polar2 = fig.add_axes(rect, polar=True, frameon=False)
ax_polar2.grid(True)
ax_polar2.set_xlabel(r"angles of incidence $\theta$, $\phi$ / deg")

# Properly set the axis ranges.
r_max = np.max(theta) / deg
ax_polar1.set_rmax(r_max)
ax_polar2.set_rmax(r_max)

cb = fig.colorbar(cax, ax=[ax_polar1, ax, ax_polar2], shrink=1.0, pad=.2, aspect=20)
cb.set_label("light in central pixel / %")

fig.text(0.02, 0.98, "$f(x)=a\,x^2+b$", ha="left", va="top")
fig.text(0.02, 0.94, "a = $%s$ mm$^{-1}$\nb = $%s$ mm" % (sig.format_sig2(famousModel.curvatureOfField, sig=3), sig.format_sig2(famousModel.fresnelLensModel.dz, sig=2)), ha="left", va="top")

plt.savefig("light_loss_%d_%d.pdf" % (famousModel.curvatureOfField * 1e5, famousModel.fresnelLensModel.dz))
plt.show()
