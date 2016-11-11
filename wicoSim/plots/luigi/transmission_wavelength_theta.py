#!/usr/bin/env python
import argparse
import pandas as pd
import numpy as np
import uncertainties as u
import uncertainties.unumpy as unp
import matplotlib.pyplot as plt

parser = argparse.ArgumentParser()
parser.add_argument("--path", default="./results/wavelength-theta-simulation/wico-wavelength-theta-simulation.pkl")
args = parser.parse_args()

df = pd.read_pickle(args.path)
# Bins for dz.
binsx = np.linspace(0, 36, 42)
binsy = np.linspace(200, 1000, 42)

digx = np.digitize(df.theta, binsx)
digy = np.digitize(df.wavelength, binsy)
xx, yy = np.meshgrid(binsx, binsy)

# Compute weighted mean for all bins by iterating all entries.
values = df.t.values
nom = np.zeros((len(binsx), len(binsy)))
denom = np.zeros((len(binsx), len(binsy)))
for i, dx, dy in zip(xrange(len(digx)), digx, digy):
    # Sum up 
    if values[i].std_dev > 0:
        nom[dx - 1, dy - 1] += values[i].nominal_value / values[i].std_dev ** 2
        denom[dx - 1, dy - 1] += 1.0 / values[i].std_dev ** 2
nominal_values = nom / denom
std_devs = np.sqrt(1.0 / denom)
# Clip zeros gracefully.
idx = denom == 0
nominal_values[idx] = 0
std_devs[idx] = 0

# Plot 2d distribution
f, ax = plt.subplots()
pc = ax.pcolormesh(xx, yy, nominal_values.T * 100.0, cmap="viridis")
ax.set_xlim(binsx[0], binsx[-1])
ax.set_ylim(binsy[0], binsy[-1])
ax.set_xlabel("angle of incidence $\\theta$ / deg")
ax.set_ylabel("wavelength $\lambda$ / nm")
f.colorbar(pc)

plt.show()
