#!/usr/bin/env python
# -*- coding: utf-8 -*-

import argparse
import matplotlib.pyplot as plt
import numpy as np
import sys, os, glob
from image_analysis import *
import jackknife
import sig

parser = argparse.ArgumentParser()
parser.add_argument("--path")
args = parser.parse_args()

# Open the most current root file.
if args.path is None:
    files = glob.glob("./results/*.root")
    files.sort()
    args.path = files[-1]
    
p, m = read(args.path)

# Filter rays which are far away from the centroid
p, m = filter(None, p, m)

# Find minimum.
x_rms_min = find_minimum_rms(p, m)
x_r90_min = find_minimum_aberration_radius(p, m)

# Plot
z_test = np.linspace(-24 + x_rms_min, 24 + x_rms_min, 256)
rms = [get_rms_spot_size(zi, p, m) for zi in z_test]
r90 = [get_aberration_radius(zi, p, m) for zi in z_test]

plt.plot(z_test, r90, label="$ r_{90}$ aberration radius\nmin = $(%s)$ mm" % sig.format_sig(x_r90_min, jackknife.std((p, m), find_minimum_aberration_radius, 10)))
plt.plot(z_test, rms, label="RMS spot size\nmin = $(%s)$ mm" % sig.format_sig(x_rms_min, jackknife.std((p, m), find_minimum_rms, 10)))

ylim = plt.ylim()
xlim = plt.xlim()
plt.vlines(x_rms_min, ylim[0], ylim[1], "red")

# 
plt.xlabel("z / mm")
plt.ylabel("value / mm")
plt.legend(loc="upper left")
plt.xlim(xlim)
plt.ylim(ylim)
plt.savefig("rms_spot_size.pdf")
plt.show()
