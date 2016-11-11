#!/usr/bin/env python
# -*- coding: utf-8 -*-

import matplotlib.pyplot as plt
import numpy as np
import sys, os, glob
import matplotlib.tri as tri
from matplotlib.collections import PatchCollection, Collection
from matplotlib.patches import Circle, Polygon

import numpy.ma as ma
from ROOT import TFile, TTree
import hex
import sig

deg = np.pi / 180.0

path = "./results/efficiency"
if len(sys.argv) > 1:
    path = sys.argv[1]
files = glob.glob(path + "/*.root")
files.sort()

focal_length = 0.0
thetas = np.zeros(len(files))
phis = np.zeros(len(files))
eff = np.zeros(len(files))
eff_sipm = np.zeros(len(files))

file = TFile(files[0])
famousModel = file.Get("famousModel")
famousModel.GetEntry(0)
curvatureOfField = famousModel.curvatureOfField
fresnelLensModel = file.Get("fresnelLensModel")
fresnelLensModel.GetEntry(0)
dz = fresnelLensModel.dz
focal_length = fresnelLensModel.f + fresnelLensModel.dz
wicoModel = file.Get("winstonConeModel")
wicoModel.GetEntry(0)
wicoThickness = wicoModel.thickness
r1 = (wicoModel.r1 + wicoModel.thickness) * 2.0 / np.sqrt(3.0)
file.Close()
    
def get(filename):
    # Open root file and retreive tree
    file = TFile(filename)
    n = 0
    try:
        n = file.Get("wicoBackHits").GetEntriesFast()
    except:
        pass
    n_sipm = 0
    for i in xrange(64):
        try:
            hits = file.Get("g4sipmHits-%d" % i)
            if hits != None:
                n_sipm += hits.GetEntriesFast()
        except:
            pass
    # Determine nTot
    gps = file.Get("generalParticleSourceMessenger")
    gps.GetEntry(0)
    nParticles = gps.nParticles
    angleThetaMax = gps.angleThetaMax
    anglePhiMax = gps.anglePhiMax
    #
    file.Close()
    return n, n_sipm, nParticles, angleThetaMax, anglePhiMax

def read():
    global thetas
    global phis
    global eff
    global eff_sipm
    cache_path = path + "/cache.npy"
    cached = os.path.exists(cache_path)
    if cached:
        cache = open(cache_path, "rb")
        files_cached = np.load(cache)
        if np.array_equal(files_cached, files):
            print "Reading cache " + cache_path
            thetas = np.load(cache)
            phis = np.load(cache)
            eff = np.load(cache)
            eff_sipm = np.load(cache)
        else:
            cached = False
        cache.close()
    if not cached:
        for i, file in enumerate(files):
            n_hit, n_sipm, n_tot, thetas[i], phis[i] = get(file)
            eff[i] = n_hit / float(n_tot)
            eff_sipm[i] = n_sipm / float(n_tot)
        cache = open(cache_path, "wb")
        np.save(cache, files)
        np.save(cache, thetas)
        np.save(cache, phis)
        np.save(cache, eff)
        np.save(cache, eff_sipm)
        cache.close()
        
def camera():
    polygons = hex.polygons(hex.grid(64), r1, wicoThickness * 2.0 / np.sqrt(3))
    # Transform coordinates in angle coordinates.
    for i, p in enumerate(polygons):
        p = p.transpose()
        theta = np.arctan(np.sqrt(p[0] ** 2 + p[1] ** 2) / (focal_length - curvatureOfField * (p[0] ** 2 + p[1] ** 2)))
        phi = np.arctan2(p[1], p[0])
        polygons[i] = np.asarray([theta / deg * np.cos(phi), theta / deg * np.sin(phi)]).transpose()
    #
    return PatchCollection([Polygon(p) for p in polygons], color="None", edgecolor="k")
    
def integrate(x, y, eff, collection):
    # Number of pixels.
    size = len(collection.get_paths())
    # Efficiencies for each pixel.
    eff_sum = [[] for i in xrange(size)]
    n = [0] * size
    for xi, yi, effi in zip(x, y, eff):
        for i, p in enumerate(collection.get_paths()):
            if p.contains_point((xi, yi)):
                # eff_sum[i].append(effi)
                eff_sum[i].append(effi)
                n[i] += 1
                break
    # Compute mean and standard variation for each pixel.
    eff_mean, std_eff = np.zeros(size), np.zeros(size)
    for i in xrange(size):
        # print len(eff_sum[i])
        eff_mean[i] = np.mean(eff_sum[i])
        std_eff[i] = np.std(eff_sum[i])
    return eff_mean, std_eff
    
def plot(thetas, phis, eff, vmin=None, vmax=None, eff_min=0.03):
    # Total solid angle of the simulation.
    omega = 2.0 * np.pi * (1.0 - np.cos(np.max(thetas)))
    domega = omega / len(eff)
    # Wheight efficiency with active area.
    # eff = ma.masked_where(eff <= eff_min, eff)
    # frac = ma.count(eff) / float(len(eff))
    #
    # eff *= domega
    # Create a new figure.
    fig = plt.figure()
    # Create polar axes.
    rect = [0.1, 0.1, 0.8, 0.85]
    ax_polar1 = fig.add_axes(rect, polar=True, frameon=True)
    ax_polar1.grid(False)
    ax_polar1.get_xaxis().set_ticks([])
    # Create normal axes to draw the tripcolor.
    ax = fig.add_axes(rect, frameon=False)
    x = thetas / deg * np.cos(phis)
    y = thetas / deg * np.sin(phis)
    # Create triangulation
    triang = tri.Triangulation(x, y)
    # Create mask to mask unwanted triangles.
    eff_mean = eff[triang.triangles].mean(axis=1)
    # triang.set_mask(np.where(eff_mean <= eff_min, 1, 0))
    # Plot
    cax = ax.tripcolor(triang, eff * 100.0, vmin=vmin, vmax=vmax, cmap=plt.get_cmap("rainbow"))
    cax.set_rasterized(True)
    collection = camera()
    ax.add_collection(collection)
    eff_int, eff_std_int = integrate(x, y, eff, collection)
    print eff_int
    print eff_std_int
    #
    # Remove ticks.
    ax.get_xaxis().set_ticks([])
    ax.get_yaxis().set_ticks([])
    ax.set_aspect('equal')
    # Set the axes range.
    r_max = np.max(x)
    ax.set_xlim([-r_max, r_max])
    ax.set_ylim([-r_max, r_max])
    # Set axes range for the polar axes in the background.
    ax_polar1.set_rmax(ax.get_xlim()[1])
    # Create new polar axes on top of the tripcolor.
    ax_polar2 = fig.add_axes(rect, polar=True, frameon=False)
    ax_polar2.grid(True)
    ax_polar2.set_xlabel(r"angles of incidence $\theta$, $\phi$ / deg")
    # Set axes range.
    ax_polar2.set_rmax(ax.get_xlim()[1])
    # Finally, creat the colorbar.
    cb = fig.colorbar(cax, ax=[ax_polar1, ax, ax_polar2], shrink=1.0, pad=.2, aspect=20)
    eff = ma.masked_where(eff <= eff_min, eff)
    # mean_eff = np.sum(eff * domega) * 100 / (domega * ma.count(eff))
    # mean_eff = np.sum(eff * domega) * 1e6
    mean_eff = np.std(eff * 100)
    # cb.set_label("efficiency $\epsilon$ / %\n" + r"mean efficiency $\sum_{i}\epsilon_{i}\Omega_{i}$ / $\sum_{i}\Omega_{i} = %.1f$ %%" % mean_eff)
    cb.set_label("efficiency / %\n" + r"Mean = $%s$ %%" % sig.format_sig(np.mean(eff_int * 100.0), np.mean(eff_std_int * 100.0), sig=2))
    #
    fig.text(0.02, 0.98, "$f(x)=a\,x^2+b$", ha="left", va="top")
    fig.text(0.02, 0.94, "a = $%s$ mm$^{-1}$\nb = $%s$ mm" % (sig.format_sig2(curvatureOfField, sig=3), sig.format_sig2(dz, sig=2)), ha="left", va="top")

if __name__ == "__main__":
    read()
    plot(thetas, phis, eff)
    plt.savefig("efficiency2d.pdf")
    # plot(thetas, phis, eff_sipm)
    # plt.savefig("efficiency2d_sipm.pdf")
    plt.show()
