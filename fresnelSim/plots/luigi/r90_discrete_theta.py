#!/usr/bin/env python
import argparse
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from natsort import natsorted
from ROOT import TFile, TTree
from cycler import cycler

parser = argparse.ArgumentParser()
parser.add_argument("--path", default="results/discrete-theta-simulation/fresnel-r90-discrete-theta.pkl")
args = parser.parse_args()

dataframe = pd.read_pickle(args.path)
thetas = np.unique(dataframe.index.values)[::2]

f, ax = plt.subplots()
ax.set_prop_cycle(cycler('color', [plt.cm.viridis(i) for i in np.linspace(0, 1, len(thetas))]))
for theta in thetas:
    ax.plot(-dataframe.loc[theta].z, dataframe.loc[theta].r90, '-', label="$%d\,^\circ$" % theta)

ax.set_xlabel("lens focal plane distance $z$ / mm")
ax.set_ylabel("aberration radius $ r_{90}$ / mm")
ax.legend(loc="lower left", ncol=2)
# Proper y axis range.
ylim = (dataframe.r90.min(), dataframe.r90.max())
yrange = ylim[1] - ylim[0]
ax.set_ylim(ylim[0] - 0.1 * yrange, ylim[1] + 0.1 * yrange)

plt.show()
