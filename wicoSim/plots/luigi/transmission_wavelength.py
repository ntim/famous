#!/usr/bin/env python
import argparse
import pandas as pd
import numpy as np
import uncertainties as u
import uncertainties.unumpy as unp
import matplotlib.pyplot as plt

parser = argparse.ArgumentParser()
parser.add_argument("--path", default="./results/wavelength-simulation/wico-wavelength-simulation.pkl")
args = parser.parse_args()

df = pd.read_pickle(args.path)
f, ax = plt.subplots()
ax.set_xlabel("wavelength $\\lambda$ / nm")
ax.set_ylabel("transmission efficiency / \\%")
ax.text(0.975, 0.95, "FAMOUS simulation", ha="right", va="top", transform=ax.transAxes)

ax.errorbar(df.wavelength, unp.nominal_values(df.t) * 100.0, yerr=unp.std_devs(df.t) * 100.0, fmt=".")

plt.show()
