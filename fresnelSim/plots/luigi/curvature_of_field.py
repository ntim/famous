#!/usr/bin/env python
import argparse
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import uncertainties as u
import uncertainties.unumpy as unp

parser = argparse.ArgumentParser()
parser.add_argument("--path", default="./results/theta-simulation/fresnel-theta-simulation.pkl")
args = parser.parse_args()

df = pd.read_pickle(args.path)
# Bin entries by groove number.
bins = np.arange(0.0, 12.5, 1.0)
groups = df.groupby(np.digitize(df.theta, bins))
x = -groups.mean().centroid_x.values
z_r90 = unp.uarray(-groups.mean().z_r90.values, groups.std().z_r90.values)
# z_r90 = unp.uarray(-groups.mean().centroid_z.values, groups.std().centroid_z.values)

f, ax = plt.subplots()
ax.errorbar(x, unp.nominal_values(z_r90), yerr=unp.std_devs(z_r90), fmt='.', label="Simulated $ r_{90}$")

def fit(x, y):
    from ROOT import TF1, TGraphAsymmErrors
    g = TGraphAsymmErrors(len(x))
    for i in xrange(len(x)):
        g.SetPoint(i, x[i], y[i].nominal_value)
        g.SetPointError(i, 0, 0, y[i].std_dev, y[i].std_dev)
    fit = TF1("fit", "-[1]*x*x+[0]", -40, 120)
    fit.SetParNames("z0", "a")
    g.Fit(fit)
    return fit

# Plot fit result for R90
fit = fit(x, z_r90)
xf = np.linspace(-10, 120)
yf = [fit.Eval(xi) for xi in xf]
ax.plot(xf, yf, label="Fit to $ r_{90}$, $\chi^2/\,ndof = %.1f\, /\, %d$" % (fit.GetChisquare(), fit.GetNDF()))

# Display fit of measurement.
y_meas = 5.132e+02 - 3.45e-03 * xf**2
ax.plot(xf, y_meas, ":", color="gray", label="Measurement")

# Annotate points
for theta, xi in zip(bins, x):
    ax.text(xi, fit.Eval(xi) - 4, "$%d^\circ$" % (theta), fontsize="medium", va="bottom", ha="center")

# Style plot
ax.set_xlabel("centroid position on focal plane $\\rho$ / mm")
ax.set_ylabel("lens focal plane distance $z$ / mm")
# Mirror y axis.
ax.set_ylim(515, 470)
ax.set_xlim(-10, 120)
ax.legend(loc="upper left")

# Annotate
ax.text(0.975, 0.05, "FAMOUS simulation", ha="right", va="bottom", transform=ax.transAxes)

plt.show()
