#!/usr/bin/env python
import os
import argparse
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.collections import PatchCollection, Collection
from matplotlib.patches import Circle, Polygon

dz = 9.55575  # mm
f = 498.33425  # mm
focal_length = f + dz
curvature_of_field = 0
r = 502.1 / 2.0  # mm
r2_in_m = (r / 1000.0) ** 2  # m^2
    
def radius(df):
    return np.abs(df.x.values[0] - df.x.values[1]) / 2.0 - 1

def circle(center, radius, resolution=64):
    x = np.cos([2.0 * np.pi / resolution * (i + 0.5) for i in xrange(0, resolution + 1)]) * radius + center[0]
    y = np.sin([2.0 * np.pi / resolution * (i + 0.5) for i in xrange(0, resolution + 1)]) * radius + center[1]
    return np.array((x, y)).T

def collection(df):
    polygons = [circle((xi, yi), radius(df)) for xi, yi in zip(df.x.values, df.y.values)]
    c = PatchCollection([Polygon(p) for p in polygons], cmap="viridis", edgecolor="k")
    c.set_array(np.zeros(len(polygons)))
    return c
    
def set_lim(ax, df):
    ax.set_aspect("equal")
    ax.set_xlim(df.x.min() - 1.5 * radius(df), df.x.max() + 1.5 * radius(df))
    ax.set_ylim(ax.get_xlim())
    
def transform_camera_to_sky(x, y):
    """
    Transform from camera coordinates to sky coordinates.
    """
    # Compute angles.
    theta = np.arctan2(np.sqrt(x ** 2 + y ** 2), (focal_length - curvature_of_field * (x ** 2 + y ** 2)))
    phi = np.arctan2(y, x)
    # Transform angles into projections
    x = np.sin(theta) * np.cos(phi)
    y = np.sin(theta) * np.sin(phi)
    return x, y

def transform_dir_to_sky(px, py, pz):
    # Convert to spherical coordinates
    r = np.sqrt(px ** 2 + py ** 2 + pz ** 2)
    theta = np.arccos(-pz / r)
    phi = np.arctan2(py, px)
    # Transform angles into projections
    x = np.sin(theta) * np.cos(phi)
    y = np.sin(theta) * np.sin(phi)
    # Find corresponding healpix bin.
    return x, y

def transform_dir_to_ang(px, py, pz):
    # Convert to spherical coordinates
    r = np.sqrt(px ** 2 + py ** 2 + pz ** 2)
    theta = np.arccos(-pz / r)
    phi = np.arctan2(py, px)
    return theta, phi

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("--path", default="./results/parameterization-simulation/famous-parameterization-simulation.pkl")
    args = parser.parse_args()
    
    # Read pixel coordinates
    df = pd.read_pickle(os.path.join(os.path.dirname(args.path), "famous-pixel-positions.pkl"))
        
    # Plot camera
    f, ax = plt.subplots()
    ax.set_aspect("equal")
    ax.set_xlabel("x / mm")
    ax.set_ylabel("y / mm")
    set_lim(ax, df)
    coll = collection(df)
    ax.add_collection(coll)
    cb = f.colorbar(coll)
    cb.set_label("number of photons")
    cb.solids.set_rasterized(True)
    
    for i, xi, yi in zip(xrange(len(df)), df.x, df.y):
        ax.text(xi, yi, str(i), ha="center", va="center", color="k")

    plt.show()
