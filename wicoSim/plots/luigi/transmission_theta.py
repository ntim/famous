#!/usr/bin/env python
import argparse
import pandas as pd
import numpy as np
import uncertainties as u
import uncertainties.unumpy as unp
import matplotlib.pyplot as plt

parser = argparse.ArgumentParser()
parser.add_argument("--path", default="./results/theta-simulation/wico-theta-simulation.pkl")
args = parser.parse_args()

df = pd.read_pickle(args.path)
# Bin entries by groove number.
bins = np.arange(0, 36, 1)
groups = df.groupby(np.digitize(df.theta, bins))

means = []
for name, group in groups:
    means.append(group.t.values.mean())

# Plot minimum r_90 as function of the groove number.
f, ax = plt.subplots()
ax.errorbar(bins, unp.nominal_values(means) * 100.0, yerr=unp.std_devs(means) * 100.0, fmt=".")
ax.set_xlabel("angle of incidence $\\theta$ / $^\circ$")
ax.set_ylabel("transmission / \\%")
ax.text(0.975, 0.95, "FAMOUS simulation", ha="right", va="top", transform=ax.transAxes)
ax.text(0.975, 0.90, "%d entries" % df.t.size, ha="right", va="top", transform=ax.transAxes)

# Mark theta_max
r1 = 6.71
r2 = 3.0
theta_max = np.rad2deg(np.arcsin(r2 / r1))
ax.vlines(theta_max, 0, 110, linestyle=":")
ax.set_ylim(0, 110)

plt.show()