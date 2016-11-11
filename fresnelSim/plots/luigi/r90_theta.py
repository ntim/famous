#!/usr/bin/env python
import argparse
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

parser = argparse.ArgumentParser()
parser.add_argument("--path", default="./results/theta-simulation/fresnel-theta-simulation.pkl")
args = parser.parse_args()

df = pd.read_pickle(args.path)
# Bin entries by groove number.
bins = np.arange(0.0, 12.5, 1.0)
groups = df.groupby(np.digitize(df.theta, bins))

# Plot minimum r_90 as function of the groove number.
f, ax = plt.subplots()
ax.errorbar(bins, groups.mean().r90.values, xerr=0.5, yerr=groups.std().r90.values, fmt=".")
ax.set_xlabel("angle of incidence $\\theta$ / $^\circ$")
ax.set_ylabel("aberration radius $ r_{90,min}$ / mm")
ax.text(0.05, 0.95, "FAMOUS simulation", ha="left", va="top", transform=ax.transAxes)
ax.text(0.05, 0.90, "%d entries" % df.r90.size, ha="left", va="top", transform=ax.transAxes)
ax.set_xlim(-1, 13)

# Plot minimum lens-focal-plane distance as function of the groove number.
f, ax = plt.subplots()
ax.errorbar(bins, -groups.mean().z_r90.values, xerr=0.5, yerr=groups.std().z_r90.values, fmt=".")
ax.set_xlabel("angle of incidence $\\theta$ / $^\circ$")
ax.set_ylabel("lens focal plane distance of $ r_{90,min}$ / mm")
ax.text(0.05, 0.95, "FAMOUS simulation", ha="left", va="top", transform=ax.transAxes)
ax.text(0.05, 0.90, "%d entries" % df.r90.size, ha="left", va="top", transform=ax.transAxes)
ax.set_xlim(-1, 13)
ax.set_ylim(515, 475)

plt.show()
