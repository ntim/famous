#!/usr/bin/env python
import os
import argparse
import numpy as np
import pandas as pd
import uncertainties.unumpy as unp
import matplotlib.pyplot as plt
import matplotlib.tri as tri
from matplotlib.backends.backend_pdf import PdfPages
from matplotlib.collections import PatchCollection, Collection
from matplotlib.patches import Circle, Polygon
import camera

parser = argparse.ArgumentParser()
parser.add_argument("--path", default="./results/parameterization-simulation/famous-parameterization-simulation.pkl")
args = parser.parse_args()
famous = pd.read_pickle(os.path.join(os.path.dirname(args.path), "famous-pixel-positions.pkl"))

# Read data.
df = pd.read_pickle(args.path)
# Create triangulation
triang = tri.Triangulation(df.x, df.y)
# Determine maximum transmission efficiency entry
n_sipms = 61
# Get all pixels
pixels = [unp.nominal_values(getattr(df, "t_sipm_%d" % i)) for i in xrange(n_sipms)]
vmax = np.max(pixels)

with PdfPages(os.path.join(os.path.dirname(args.path), 'famous-parameterization-all-pixels.pdf')) as pdf:
    for i in xrange(n_sipms):
        # Plot and append to PDF.
        fig, ax = plt.subplots()
        cax = ax.tripcolor(triang, pixels[i], vmin=1e-3, vmax=vmax, cmap="viridis", rasterized=True)
        ax.set_aspect("equal")
        ax.set_xlabel(r"field of view $\sin(\theta) \cdot \cos(\phi)$")
        ax.set_ylabel(r"field of view $\sin(\theta) \cdot \sin(\phi)$")
        ax.set_axis_bgcolor(plt.get_cmap("viridis")(0))
        cb = fig.colorbar(cax)
        cb.set_label("efficiency / %")
        cb.solids.set_rasterized(True)
        pdf.savefig(dpi=300)
        plt.close()
 