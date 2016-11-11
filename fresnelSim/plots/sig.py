#!/usr/bin/env python
# -*- coding: utf-8 -*-
from math import log10, floor

def sig_n(x):
	'''
	Get number of digits until first significant for float x
	'''
	return int(floor(log10(x)))

def round_sig(x, sig=1):
	'''
	Round float x to the significant digits sig.
	'''
	return round(x, sig-sig_n(x)-1)

def round_sig(x, sigma, sig=1):
	'''
	Round float x and uncertainty sigma to the significant digits sig of sigma.
	'''
	n = sig_n(sigma)
	return round(x, sig-n-1), round(sigma, sig-n-1)

def format_sig(x, sigma, sig=1, exp=3):
	'''
	Format float x and uncertainty sigma to the significant digits sig of sigma with LaTeX code. 
	Exp defines the threshold at which scientific notation will be used.
	'''
	n = sig - sig_n(sigma) - 1
	x, sigma = round_sig(x, sigma, sig)
	if abs(n) >= exp:
		x *= 10**n
		sigma *= 10**n
		return ("(%d \pm %.d) \cdot 10^{%d}") % (x, sigma, -n)
	if n >= 0:
		return ("%." + str(abs(n)) + "f \pm %." + str(abs(n)) + "f") % (x, sigma)
	return ("%d \pm %d") % (x, sigma)