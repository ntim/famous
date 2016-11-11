#!/usr/bin/env python
import argparse
import numpy as np
import pandas as pd
import uncertainties.unumpy as unp
import matplotlib.pyplot as plt
import matplotlib.tri as tri

parser = argparse.ArgumentParser()
parser.add_argument("--path", default="./results/parameterization-simulation/famous-parameterization-simulation.pkl")
args = parser.parse_args()

# Read data.
df = pd.read_pickle(args.path)
# Bin in theta.
theta_range = df.theta.max() - df.theta.min() 
bin_width = theta_range / 36.0
bins = np.arange(df.theta.min(), df.theta.max(), bin_width)
groups = df.groupby(np.digitize(df.theta, bins))
names = np.array([name for name, group in groups])
theta = bins[names - 1] + bin_width
means = np.array([group.t_sipm.values.mean() for name, group in groups])

# Plot
fig, ax = plt.subplots()
ax.errorbar(np.rad2deg(theta), unp.nominal_values(means) * 100.0, xerr=bin_width / 2.0, yerr=unp.std_devs(means) * 100.0, fmt=".")
ax.set_xlabel("angle of incidence $\\theta$ / $^\circ$")
ax.set_ylabel("transmission / \\%")
ax.text(0.975, 0.05, "FAMOUS simulation", ha="right", va="top", transform=ax.transAxes)
ax.text(0.975, 0.10, "%d entries" % df.wavelength.size, ha="right", va="top", transform=ax.transAxes)
#
plt.show()
