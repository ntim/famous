#!/usr/bin/env python
# -*- coding: utf-8 -*-

import numpy as np

def write(filename, data):
    f = open(filename, "wb")
    for d in data:
        np.save(f, d)
    f.close()
    return data

def read(filename, data):
    f = open(filename, "rb")
    load = []
    for i in xrange(len(data)):
        load.append(np.load(f))
    f.close()
    return load        
