#!/usr/bin/env python

def unit_plane(n):
    """
    Taken from http://cgkit.sourceforge.net/doc2/hammersley.html
    
    Yields a sequence of n tuples (x, y) which represent a point on the unit square. 
    The sequence of points for a particular n is always the same. 
    When n changes an entirely new sequence will be generated.
    This function uses a base of 2.

    """
    for k in range(n):
        u = 0
        p = 0.5
        kk = k
        while kk > 0:
            if kk & 1:
                u += p
            p *= 0.5
            kk >>= 1
        v = (k + 0.5) / n
        yield (u, v)

def plane(n, xrange, yrange):
    dx = xrange[1] - xrange[0]
    dy = yrange[1] - yrange[0]
    for p in unit_plane(n):
        yield (p[0] * dx + xrange[0], p[1] * dy + yrange[0])
