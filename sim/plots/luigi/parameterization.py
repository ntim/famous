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

parser = argparse.ArgumentParser()
parser.add_argument("--path", default="./results/parameterization-simulation/famous-parameterization-simulation.pkl")
args = parser.parse_args()

# Read data.
df = pd.read_pickle(args.path)
famous = pd.read_pickle(os.path.join(os.path.dirname(args.path), "famous-pixel-positions.pkl"))
# Create triangulation
triang = tri.Triangulation(df.x, df.y)
pdf = unp.nominal_values(df.t_sipm) * 100.0

fig, ax = plt.subplots()
cax = ax.tripcolor(triang, pdf, vmin=1e-3, cmap="viridis")
ax.set_aspect("equal")
ax.set_xlabel(r"field of view $\sin(\theta) \cdot \cos(\phi)$")
ax.set_ylabel(r"field of view $\sin(\theta) \cdot \sin(\phi)$")
ax.set_axis_bgcolor(plt.get_cmap("viridis")(0))
cb = fig.colorbar(cax)
cb.set_label("efficiency / %")
cb.solids.set_rasterized(True)

# Plot pixel borders
dz = 9.55575
f = 498.33425
focal_length = f + dz
polygons = [camera.circle((xi, yi), camera.radius(famous)) for xi, yi in zip(famous.x, famous.y)]
# Transform coordinates in angle coordinates.
for i, p in enumerate(polygons):
    p = p.transpose()
    theta = np.arctan(np.sqrt(p[0] ** 2 + p[1] ** 2) / focal_length)
    phi = np.arctan2(p[1], p[0])
    polygons[i] = np.asarray([np.sin(theta) * np.cos(phi), np.sin(theta) * np.sin(phi)]).transpose()
collection = PatchCollection([Polygon(p) for p in polygons], edgecolor="k", facecolor="none", linewidth=1)
ax.add_collection(collection)

for i, xi, yi in zip(xrange(len(famous.x)), famous.x, famous.y):
    theta = np.arctan(np.sqrt(xi ** 2 + yi ** 2) / focal_length)
    phi = np.arctan2(yi, xi)
    xi, yi = np.sin(theta) * np.cos(phi), np.sin(theta) * np.sin(phi)
    ax.text(xi, yi, str(i), color="k", ha="center", va="center")

#
plt.show()
