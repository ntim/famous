#!/usr/bin/env python
# -*- coding: utf-8 -*-

import numpy as np

def jackknife_indices(x, n):
    '''
    Get a list of indices arrays for an array x with the number of samples n.
    '''
    idx = np.arange(0, len(x))
    idx = np.array_split(idx, n)
    return [np.concatenate(idx[:i] + idx[i + 1:]) for i in xrange(n)]

def std(data, func, n=100):
    '''
    Calculate the standard deviation of a quantity using data.
    The data is divided into n subsets for which each the quantity is 
    calculated by leaving subsample i out of the data.
    The standard deviation shall equal the true standard deviation if the 
    distribution of the quantity follows a normal distribution and for n 
    equal to the length of the data set. 
    
    Parameters
    ----------
    data: tuple of the data.
    func: the function which is used to calculate the quantity by func(*(data)).
    n: the number of samples to compute. 
    
    '''
    tdata = tuple(np.array(x) for x in data)
    s = func(*tdata)
    sum = 0.0
    idx = jackknife_indices(tdata[0], n)
    for i in xrange(n):
        tdata_new = (x[idx[i]] for x in tdata)
        sum += (func(*tdata_new) - s) ** 2
    return np.sqrt(sum * (float(n) - 1.0) / float(n))
