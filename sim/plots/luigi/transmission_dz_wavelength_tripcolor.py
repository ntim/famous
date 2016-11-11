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
parser.add_argument("--path", default="./results/dz-wavelength-simulation/famous-dz-wavelength-simulation.pkl")
args = parser.parse_args()

df = pd.read_pickle(args.path)
# Create triangulation
triang = tri.Triangulation(df.z.values, df.wavelength.values)

def plot(values, title):
    f, ax = plt.subplots()
    f.suptitle(title)
    ax.set_xlabel("lens focal plane distance $z$ / mm")
    ax.set_ylabel("wavelength $\lambda$ / nm")
    ax.text(0.975, 0.95, "FAMOUS simulation", ha="right", va="top", transform=ax.transAxes)
    ax.text(0.975, 0.90, "%d entries" % df.z.size, ha="right", va="top", transform=ax.transAxes)
    pc = ax.tripcolor(triang, unp.nominal_values(values) * 100.0, cmap="viridis", vmin=0)
    cb = f.colorbar(pc)
    ax.set_xlim(df.z.min(), df.z.max())
    ax.set_ylim(df.wavelength.min(), df.wavelength.max())
    ax.set_axis_bgcolor(cm.viridis(0))
    cb.set_label("transmission efficiency / %")

plot(df.t_focal_plane, "Focal plane")
plot(df.t_after_wico, "After Winston cones")
plot(df.t_sipm, "All SiPMs")
plot(df.t_sipm_0, "Central SiPM")

plt.show()
