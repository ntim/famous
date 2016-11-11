#!/usr/bin/env python
# -*- coding: utf-8 -*-

import matplotlib.pyplot as plt
import numpy as np
import sys, os, glob
from image_analysis import *
from scipy.optimize import leastsq, curve_fit
import jackknife
import sig

deg = np.pi / 180.0
f = 502.1

# Open the most current root file.
path = "./results/theta/*.root"
if len(sys.argv) > 1:
    path = sys.argv[1]
files = glob.glob(path)
files.sort()

mode = "rms"
if len(sys.argv) > 2:
    mode = sys.argv[2]
if mode == "r_90":
    label = "$ r_{90}$"
elif mode == "rms":
    label = "$\sigma_{r}$"
else:
    raise "mode not supported"

z = np.linspace(-32, 32, 128)
theta = np.zeros(len(files))
rms = np.zeros((len(theta), len(z)))
rms_min = np.zeros(len(files))
sigma_rms_min = np.zeros(len(files))
for i, filename in enumerate(files):
    # Read hits from file.
    p, m = read(filename)
    # Filter rays which are far away from the centroid
    p, m = filter(None, p, m)
    # Read theta
    file = TFile(filename)
    gps = file.Get("generalParticleSourceMessenger")
    gps.GetEntry(0)
    #
    theta[i] = gps.angleThetaMax / deg
    if mode == "r_90":
        rms[i] = [get_aberration_radius(zi, p, m) for zi in z]
        rms_min[i] = find_minimum(p, m, func=get_aberration_radius, x0=np.min(rms[i]))
        sigma_rms_min[i] = jackknife.std((p, m), find_minimum_aberration_radius, 10)
    elif mode == "rms":
        rms[i] = [get_rms_spot_size(zi, p, m) for zi in z]
        rms_min[i] = find_minimum(p, m, func=get_rms_spot_size)
        sigma_rms_min[i] = jackknife.std((p, m), find_minimum_rms, 10)
    # Close the file.
    file.Close()

# Sort results
idx = np.argsort(theta)
theta = theta[idx]
rms = rms[idx]

# Plot
x, y = np.meshgrid(z, theta)
pc = plt.pcolormesh(x, y, rms, shading='gouraud', cmap=plt.get_cmap("rainbow"))
pc.set_rasterized(True)

plt.plot(rms_min, theta, '-', linewidth=2, color="white")

cb = plt.colorbar()
cb.set_label("spot size %s / mm" % label)
plt.xlabel("z / mm")
plt.ylabel("theta / deg")
plt.xlim(np.min(z), np.max(z))
plt.ylim(np.min(theta), np.max(theta))

plt.savefig("theta_z_%s.pdf" % mode)

plt.figure()

x = f * np.tan(theta * deg)
plt.errorbar(x, rms_min, yerr=sigma_rms_min, fmt="o", markersize=3)

# Filter values with NaN
x_new, y_new, sigma_y_new = [], [], []
for i in xrange(len(x)):
    if not np.isnan(rms_min[i]):
        x_new.append(x[i])
        y_new.append(rms_min[i])
        sigma_y_new.append(sigma_rms_min[i])
x_new = np.asarray(x_new)
y_new = np.asarray(y_new)
sigma_y_new = np.asarray(sigma_y_new)

fitfunc = lambda x, p0, p1: p1 * x ** 2 + p0
# errfunc = lambda p, x, y: y - fitfunc(p, x)
p0 = [0, 1. / (2.0 * f)]
p1, pcov = curve_fit(fitfunc, x_new, y_new, p0, sigma_y_new)
sigma_p1 = [np.absolute(pcov[i][i]) ** 0.5 for i in xrange(len(p1))]
chi2, ndf = np.sum((fitfunc(x_new, *p1) - y_new) ** 2 / sigma_y_new ** 2), len(x_new) - len(p1) - 1

plt.plot(x, fitfunc(x, p1[0], p1[1]), '-')

text = "$f(x) = a x^2 + b$\n"
par_names = ["b", "a"]
par_units = ["mm", "mm^{-1}"]
for par, sigma_par, par_name, par_unit in zip(p1, sigma_p1, par_names, par_units):
    text += "$" + par_name + " = " + sig.format_sig(par, sigma_par) + "\," + par_unit + "$\n"
text += "$\chi2 / ndf = %d / %d$" % (chi2, ndf)
plt.text(0.5, 0.95, text, ha="center", va="top", transform=plt.axes().transAxes)

plt.xlabel("x / mm")
plt.ylabel("z of minimal spot size / mm")

plt.savefig("x_%s_fit.pdf" % mode)

plt.show()
