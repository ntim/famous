#!/usr/bin/env python
import argparse
import pandas as pd
import numpy as np
import uncertainties as u
import uncertainties.unumpy as unp
import matplotlib.pyplot as plt

parser = argparse.ArgumentParser()
parser.add_argument("--path", default="./results/dz-wavelength-simulation/famous-dz-wavelength-simulation.pkl")
args = parser.parse_args()

df = pd.read_pickle(args.path)
# Bins for dz.
f = df.z.min() - df.dz.min()
binsx = np.linspace(f - 64, f + 64, 64)
binsy = np.linspace(270, 1020, 64)
digx = np.digitize(df.z, binsx)
digy = np.digitize(df.wavelength, binsy)
xx, yy = np.meshgrid(binsx, binsy)

def profile(values):
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
    idx = (denom == 0)
    nominal_values[idx] = 0
    std_devs[idx] = 0
    #
    return nominal_values, std_devs

def plot(values, title=None):
    f, ax = plt.subplots()
    f.suptitle(title)
    ax.set_xlabel("lens focal plane distance $z$ / mm")
    ax.set_ylabel("wavelength $\lambda$ / nm")
    ax.text(0.975, 0.95, "FAMOUS simulation", ha="right", va="top", transform=ax.transAxes)
    ax.text(0.975, 0.90, "%d entries" % df.z.size, ha="right", va="top", transform=ax.transAxes)
    zz, _ = profile(values)
    pc = ax.pcolormesh(xx, yy, zz.T * 100.0, cmap="viridis")
    ax.set_xlim(binsx[0], binsx[-1])
    ax.set_ylim(binsy[0], binsy[-1])
    cb = f.colorbar(pc)
    cb.set_label("transmission efficiency / %")
    
plot(df.t_focal_plane, "Focal plane")
plot(df.t_after_wico, "After Winston cones")
plot(df.t_sipm, "All SiPMs")
plot(df.t_sipm_0, "Central SiPM")

plt.show()
