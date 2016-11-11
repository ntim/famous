#!/usr/bin/env python
# -*- coding: utf-8 -*-

from ROOT import TFile, TTree
import matplotlib.pyplot as plt
import numpy as np
import sys, os, glob

path = "./results/*.root"
if len(sys.argv) > 1:
    filename = sys.argv[1]
else:
    filename = glob.glob(path)[0]
    
file = TFile(filename)
hits = file.Get("g4sipmHits-0") 
if hits == None:
    hits = file.Get("hits")

x = []
for i in xrange(hits.GetEntries()):
    hits.GetEntry(i)
    m = hits.momentum
    x.append(np.arccos(np.abs(m[2] / np.sqrt(m[0]**2 + m[1]**2 + m[2]**2))) * 180.0 / np.pi)
    
x = np.asarray(x)
bins = np.arange(np.ceil(np.max(x)))
n, _ = np.histogram(x, bins=bins)
std = np.sqrt(n)
plt.errorbar((_[1:] + _[:-1]) / 2, n, yerr=std, fmt=".")

plt.xlabel(r"theta / deg")
plt.ylabel(r"entries")
plt.show()
