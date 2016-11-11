#!/usr/bin/env python
import argparse
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

parser = argparse.ArgumentParser()
parser.add_argument("--path", default="./results/grooves-simulation/fresnel-grooves-simulation.pkl")
args = parser.parse_args()

df = pd.read_pickle(args.path)
# Bin entries by groove number.
bins = np.arange(0.5, 12.5, 1.0)
groups = df.groupby(np.digitize(df.grooves, bins))

# Plot minimum r_90 as function of the groove number.
f, ax = plt.subplots()
f.set_figwidth(f.get_figwidth() * 0.5, forward=True)
f.subplots_adjust(left=0.22)
ax.errorbar(bins, groups.mean().r90.values, yerr=groups.std().r90.values, fmt=".")
ax.set_xlabel("grooves / mm$^{-1}$")
ax.set_ylabel("aberration radius $ r_{90,min}$ / mm")
ax.text(0.975, 0.95, "FAMOUS simulation", ha="right", va="top", transform=ax.transAxes)
ax.text(0.975, 0.90, "%d entries" % df.grooves.size, ha="right", va="top", transform=ax.transAxes)
f.savefig("r90_grooves.pdf")

# Plot minimum lens-focal-plane distance as function of the groove number.
f, ax = plt.subplots()
f.set_figwidth(f.get_figwidth() * 0.5, forward=True)
f.subplots_adjust(left=0.22)
ax.errorbar(bins, -groups.mean().z_r90.values, yerr=groups.std().z_r90.values, fmt=".")
ax.set_xlabel("grooves / mm$^{-1}$")
ax.set_ylabel(r"lens focal plane distance of $ r_{90,min}$ / mm")
ax.text(0.975, 0.95, "FAMOUS simulation", ha="right", va="top", transform=ax.transAxes)
ax.text(0.975, 0.90, "%d entries" % df.grooves.size, ha="right", va="top", transform=ax.transAxes)
f.savefig("r90_grooves_z.pdf")

plt.show()
