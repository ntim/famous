#!/usr/bin/env python
# -*- coding: utf-8 -*-

import numpy as np
from ROOT import TFile, TTree
import hex

class WinstonConeModel:
    def __init__(self, filename):
        f = TFile(filename)
        t = f.Get("winstonConeModel")
        t.GetEntry(0)
        self.r1 = t.r1
        self.r2 = t.r2
        self.thickness = t.thickness
        self.surfaceSigmaAlpha = t.surfaceSigmaAlpha
        self.solid = t.solid
        self.length = t.length
        f.Close()

class FresnelLensModel:
    def __init__(self, filename):
        f = TFile(filename)
        t = f.Get("fresnelLensModel")
        t.GetEntry(0)
        self.r = t.r
        self.f = t.f
        self.k = t.k
        self.numberOfGrooves = t.numberOfGrooves
        self.thickness = t.thickness
        self.dz = t.dz
        f.Close()

class FamousModel:
    def __init__(self, filename):
        f = TFile(filename)
        t = f.Get("famousModel")
        t.GetEntry(0)
        self.numberOfPixels = t.numberOfPixels
        self.ug11FilterThickness = 1.0
        self.curvatureOfField = t.curvatureOfField
        f.Close()
        self.wicoModel = WinstonConeModel(filename)
        self.fresnelLensModel = FresnelLensModel(filename)
        
    def pixels(self, polygons=True):
        size = (self.wicoModel.r1 + self.wicoModel.thickness) * 2.0 / np.sqrt(3.0)
        if polygons:
            border = self.wicoModel.thickness * 2.0 / np.sqrt(3)
            return hex.polygons(hex.grid(self.numberOfPixels), size, border)
        return hex.circles(hex.grid(self.numberOfPixels), size, self.wicoModel.r1)
    
    def pixels_to_cart(self, polygons=True):
        all = self.pixels(polygons)
        for i, p in enumerate(all):
            all[i] = np.asarray(self.to_cart(p.transpose())).transpose()
        return all

    def pixel_centers(self):
        size = (self.wicoModel.r1 + self.wicoModel.thickness) * 2.0 / np.sqrt(3)
        return hex.centers(hex.grid(self.numberOfPixels), size)
    
    def pixel_centers_to_cart(self):
        all = self.pixel_centers()
        for i, c in enumerate(all):
            all[i] = self.to_cart(c)
        return all
    
    def f(self):
        return self.fresnelLensModel.f + self.fresnelLensModel.dz
    
    def shift(self, point):
        '''Determines the coordinate shift of a point on the focal plane according to the curvature of the model.'''
        r = np.sqrt(point[0] ** 2 + point[1] ** 2)
        alpha_r = np.arctan(self.curvatureOfField * r)
        alpha_x = np.arctan(self.curvatureOfField * point[0])
        alpha_y = np.arctan(self.curvatureOfField * point[1])
        dz = (self.wicoModel.length + self.ug11FilterThickness / 2.0) * np.cos(alpha_r)
        dx = dz * np.sin(alpha_x)
        dy = dz * np.sin(alpha_y)
        return dx, dy, dz
    
    def to_cart(self, point):
        '''Transforms the point on the image plane according to the curvature of the model.'''
        dx, dy, dz = self.shift(point)
        return (point[0] + dx, point[1] + dy)
    
    def to_ang(self, point):
        '''Trasnforms the point on the image plane according to the curvature of the model to sky coordinates.'''
        r = np.sqrt(point[0] ** 2 + point[1] ** 2)
        theta = np.arctan(r / (self.f() - self.curvatureOfField * r ** 2))
        phi = np.arctan2(point[1], point[0])
        return theta, phi
    
    def ang_to_cart(self, theta, phi):
        r = self.f() * np.tan(theta)
        x = -r * np.cos(phi)
        y = -r * np.sin(phi)
        return self.to_cart((x, y))
