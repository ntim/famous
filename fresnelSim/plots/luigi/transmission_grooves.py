#!/usr/bin/env python
import argparse
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import uncertainties as u
import uncertainties.unumpy as unp

parser = argparse.ArgumentParser()
parser.add_argument("--path", default="./results/grooves-simulation/fresnel-grooves-simulation.pkl")
args = parser.parse_args()

df = pd.read_pickle(args.path)
# Bin entries by groove number.
bins = np.arange(0.5, 12.5, 1.0)
groups = df.groupby(np.digitize(df.grooves, bins))

means = []
for name, group in groups:
    means.append(group.t.values.mean())

# Plot minimum r_90 as function of the groove number.
f, ax = plt.subplots()
ax.errorbar(bins, unp.nominal_values(means) * 100.0, xerr=0.5, yerr=unp.std_devs(means) * 100.0, fmt=".")
ax.set_xlabel("grooves / mm$^{-1}$")
ax.set_ylabel("transmission / \\%")
ax.text(0.975, 0.95, "FAMOUS simulation", ha="right", va="top", transform=ax.transAxes)
ax.text(0.975, 0.90, "%d entries" % df.t.size, ha="right", va="top", transform=ax.transAxes)
ax.set_xlim(-1, 13)

plt.show()
