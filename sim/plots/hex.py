#!/usr/bin/env python
# -*- coding: utf-8 -*-

import numpy as np
from matplotlib.collections import PatchCollection, Collection
from matplotlib.patches import Circle, Polygon

class hex:
    def __init__(self, x, z):
        self.q = x
        self.r = z

    def to_axial(self):
        return self.q, self.r

    def to_cube(self):
        return self.q, -self.q - self.r, self.r

    def to_cartesian(self, size=1.0, offset=None):
        y = size * 3.0 / 2.0 * self.q
        x = size * np.sqrt(3.0) * (self.r + self.q / 2.0)
        if offset != None:
            x -= offset[0]
            y -= offset[1]
        return x, y

    def distance(self, h2):
        x1, y1, z1 = self.to_cube()
        x2, y2, z2 = h2.to_cube()
        return (np.abs(x1 - x2) + np.abs(y1 - y2) + np.abs(z1 - z2)) / 2.0

    def cartesian_distance(self, h2, size, offset=None):
        x1, y1 = self.to_cartesian(size, offset)
        x2, y2 = h2.to_cartesian(size, offset)
        return np.sqrt((x1 - x2) ** 2 + (y1 - y2) ** 2)

    def polygon(self, size=1.0, border=0.0, offset=None):
        c = self.to_cartesian(size)
        x = np.cos([2.0 * np.pi / 6.0 * (i + 0.5) for i in xrange(0, 7)]) * (size - border) + c[0]
        y = np.sin([2.0 * np.pi / 6.0 * (i + 0.5) for i in xrange(0, 7)]) * (size - border) + c[1]
        if offset != None:
            x -= offset[0]
            y -= offset[1]
        return np.asarray([x, y]).transpose()

    def circle(self, size=1.0, radius=1.0, offset=None, resolution=64):
        c = self.to_cartesian(size)
        x = np.cos([2.0 * np.pi / resolution * (i + 0.5) for i in xrange(0, resolution + 1)]) * radius + c[0]
        y = np.sin([2.0 * np.pi / resolution * (i + 0.5) for i in xrange(0, resolution + 1)]) * radius + c[1]
        if offset != None:
            x -= offset[0]
            y -= offset[1]
        return np.asarray([x, y]).transpose()

def grid(n_pix, center_hex = hex(0, 0.5)):
    #
    max = int(np.sqrt(n_pix) / 2)
    hexagons = []
    # Create hexagons.
    for x in xrange(-max, max + 1):
        for y in xrange(-max - 1, max + 1):
            for z in xrange(-max, max + 2):
                if x + y + z == 0:
                    h = hex(x, z)
                    hexagons.append(h)
    # Sort distance ascending by radial distance.
    hexagons.sort(key=lambda h: h.cartesian_distance(center_hex, 1.0))
    # Select the first n_pix pixels.
    return hexagons[0:n_pix]

def polygons(hexagons, size, border=0.0):
    # Center pixel (note offset in cube coordinates).
    center_hex = hex(0, 0.5)
    # Offset of half a hex in x direction to get a symmetric camera.
    offset = center_hex.to_cartesian(size) 
    return [h.polygon(size, border, offset) for h in hexagons]

def circles(hexagons, size, border=0.0):
    # Center pixel (note offset in cube coordinates).
    center_hex = hex(0, 0.5)
    # Offset of half a hex in x direction to get a symmetric camera.
    offset = center_hex.to_cartesian(size) 
    return [h.circle(size, border, offset) for h in hexagons]

def centers(hexagons, size):
    # Center pixel (note offset in cube coordinates).
    center_hex = hex(0, 0.5)
    # Offset of half a hex in x direction to get a symmetric camera.
    offset = center_hex.to_cartesian(size) 
    return [h.to_cartesian(size, offset) for h in hexagons]    

def patches(hexagons, size):
    #
    return PatchCollection([Polygon(poly) for poly in polygons(hexagons, size)], color="white", edgecolor="black")

def hex_round(x, y, z):
    rx = np.round(x)
    ry = np.round(y)
    rz = np.round(z)
    x_diff = np.abs(rx - x)
    y_diff = np.abs(ry - y)
    z_diff = np.abs(rz - z)
    if x_diff > y_diff and x_diff > z_diff:
        rx = -ry - rz
    elif y_diff > z_diff:
        ry = -rx - rz
    else:
        rz = -rx - ry
    return hex(rx, rz)

def nearest(c, size):
    q = 2.0 / 3.0 * c[0] / size
    r = (-1.0 / 3.0 * c[0] + 1.0 / 3.0 * np.sqrt(3.0) * c[1]) / size
    return hex_round(q, -q - r, r)
