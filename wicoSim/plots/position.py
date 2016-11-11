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

x, y = [], []
for i in xrange(hits.GetEntries()):
    hits.GetEntry(i)
    x.append(hits.position[0])
    y.append(hits.position[1])
    
plt.hist2d(x, y, bins=100)

cb = plt.colorbar()
cb.set_label("hits")
plt.xlabel(r"x / mm")
plt.ylabel(r"y / mm")
plt.axes().set_aspect('equal')
plt.show()
