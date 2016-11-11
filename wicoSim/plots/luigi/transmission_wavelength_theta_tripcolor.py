#!/usr/bin/env python
import argparse
import pandas as pd
import numpy as np
import uncertainties as u
import uncertainties.unumpy as unp
import matplotlib.pyplot as plt
import matplotlib.tri as tri
import matplotlib.cm as cm

parser = argparse.ArgumentParser()
parser.add_argument("--path", default="./results/wavelength-theta-simulation/wico-wavelength-theta-simulation.pkl")
args = parser.parse_args()

df = pd.read_pickle(args.path)
# Create triangulation
triang = tri.Triangulation(df.theta.values, df.wavelength.values)
# Create interpolation based on triangulation.
interp = tri.LinearTriInterpolator(triang, unp.nominal_values(df.t))
# Create rectangular grid.
nbins = 128
xx, yy = np.meshgrid(np.linspace(df.theta.min(), df.theta.max(), nbins), np.linspace(df.wavelength.min(), df.wavelength.max(), nbins))
zz = interp(xx[1:,1:], yy[1:,1:])

# Plot 2d distribution
f, ax = plt.subplots()
# pc = ax.tripcolor(triang, unp.nominal_values(df.t) * 100.0, cmap="viridis", vmin=0)
pc = ax.pcolormesh(xx[:-1,:-1], yy[:-1,:-1], zz * 100.0, cmap="viridis", vmin=0)
ax.set_xlim(xx.min(), xx.max())
ax.set_ylim(yy.min(), yy.max())
ax.set_xlabel("angle of incidence $\\theta$ / deg")
ax.set_ylabel("wavelength $\lambda$ / nm")
ax.set_axis_bgcolor(cm.viridis(0))
f.colorbar(pc)

plt.show()
