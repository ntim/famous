#!/usr/bin/env python
import argparse
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import uncertainties as u
import uncertainties.unumpy as unp

parser = argparse.ArgumentParser()
parser.add_argument("--path", default="./results/wavelength-simulation/famous-wavelength-simulation.pkl")
args = parser.parse_args()

df = pd.read_pickle(args.path)
# Bin entries by groove number.
bins = np.arange(270, 1020, 10.0)
groups = df.groupby(np.digitize(df.wavelength, bins))

means = []
for name, group in groups:
    means.append(group.t_sipm_0.values.mean())
means = np.array(means)

# Plot minimum r_90 as function of the groove number.
f, ax = plt.subplots()
ax.errorbar(bins + 5.0, unp.nominal_values(means) * 100.0, xerr=0.5, yerr=unp.std_devs(means) * 100.0, fmt=".")
# ax.errorbar(df.wavelength, unp.nominal_values(df.t_sipm_0) * 100.0, xerr=0.5, yerr=unp.std_devs(df.t_sipm_0) * 100.0, fmt=".")
ax.set_xlabel("angle of incidence $\\theta$ / $^\circ$")
ax.set_ylabel("transmission / \\%")
ax.text(0.975, 0.05, "FAMOUS simulation", ha="right", va="top", transform=ax.transAxes)
ax.text(0.975, 0.10, "%d entries" % df.wavelength.size, ha="right", va="top", transform=ax.transAxes)

plt.show()
