#!/usr/bin/env python
# -*- coding: utf-8 -*-

import matplotlib.pyplot as plt
import numpy as np
from scipy.optimize import minimize
from numpy import nan

def read(filename):
    from ROOT import TFile, TTree
    file = TFile(filename)
    hits = file.Get("hits")
    #
    p = np.zeros((hits.GetEntries(), 3))
    m = np.zeros((hits.GetEntries(), 3))
    for i in xrange(hits.GetEntries()):
        hits.GetEntry(i)
        p[i] = hits.position
        m[i] = hits.momentum
    file.Close()
    return p, m

def get_p(z, p, m):
    '''
    Calculates the point p_i in the plane z given the position p and momentum m.    
    '''
    if z is None:
        return p
    return p + (z - p[...,2:3]) / m[...,2:3] * m
    
def get_centroid(z, p, m):
    '''
    Calculates the mean position in the plane z.
    '''
    return np.mean(get_p(z, p, m), axis=0).reshape((1, 3))

def get_rms_spot_size(z, p, m):
    '''
    Calculates the standard deviation of the radial positions in the plane z after subtracting the mean position.
    '''
    pos = get_p(z, p, m) - get_centroid(z, p, m)
    return np.sqrt((np.sum(np.linalg.norm(pos, axis=1) ** 2)) / len(pos))

def get_aberration_radius(z, p, m, epsilon=0.9):
    '''
    Calculates the radius of the disk encircling \epsilon of all points.
    '''
    pos = get_p(z, p, m) - get_centroid(z, p, m)
    r = np.linalg.norm(pos, axis=1)
    r.sort()
    return r[int(np.floor(epsilon * len(r)))]

def filter(z, p, m, r_max=100):
    '''
    Filters far off axis rays which are likely to come from stray light. 
    '''
    pos = get_p(z, p, m) - get_centroid(z, p, m)
    pos_norm = np.linalg.norm(pos, axis=1)
    p_new = p[np.where(pos_norm < r_max)]
    m_new = m[np.where(pos_norm < r_max)]
    return p_new, m_new

def find_minimum(p, m, func=get_rms_spot_size, x0=0):
    res = minimize(func, x0, args=(p, m), method='Nelder-Mead')
    if res.success:
        return res.x[0]
    return np.nan

find_minimum_rms = lambda p, m: find_minimum(p, m, func=get_rms_spot_size)
find_minimum_aberration_radius = lambda p, m: find_minimum(p, m, func=get_aberration_radius)
