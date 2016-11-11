#!/usr/bin/env python
import healpy as hp
import numpy as np


def _check_range(rng, allowed):
    if rng is None:
        rng = allowed
    else:
        if not hasattr(rng, '__len__'):
            raise ValueError("range object does not have len() method")

        if rng[0] < allowed[0] or rng[1] > allowed[1]:
            raise ValueError("lon_range should be within [%s,%s]" % allowed)
    return rng


def randsphere_ang(num, **kw):
    """
    Generate random angular theta,phi points on the sphere
    parameters
    ----------
    num: integer 
        The number of randoms to generate
    output
    ------
    theta,phi: tuple of arrays
        the random points
        theta in [0,pi]
        phi   in [0,2*pi]
    """
    import numpy as np
    from numpy import pi, cos, arccos
    theta_range = kw.get('theta_range', None)
    phi_range = kw.get('phi_range', None)
    theta_range = _check_range(theta_range, [0.0, pi])
    phi_range = _check_range(phi_range, [0, 2. * pi])
    phi = np.random.random(num)
    phi *= (phi_range[1] - phi_range[0])
    if phi_range[0] > 0:
        phi += phi_range[0]
    cos_theta_min = cos(theta_range[0])
    cos_theta_max = cos(theta_range[1])
    v = np.random.random(num)
    v *= (cos_theta_max - cos_theta_min)
    v += cos_theta_min
    v.clip(min=-1.0, max=1.0, out=v)
    # Now this generates on [0,pi)
    theta = arccos(v)
    return theta, phi


def randsphere(nside, ipix, num):
    res = hp.nside2resol(nside) * 24
    # Get the center
    theta_c, phi_c = hp.pix2ang(nside, ipix)
    # Dice coordinates
    theta, phi = [], []
    while len(theta) < num:
        t, p = randsphere_ang(num,
                              phi_range=[max(0, phi_c - res),
                                         min(2 * np.pi, phi_c + res)],
                              theta_range=[max(0, theta_c - res), min(np.pi, theta_c + res)])
        pix = hp.ang2pix(nside, t, p)
        t = t[pix == ipix]
        p = p[pix == ipix]
        if len(p) > 0:
            theta.extend(t)
            phi.extend(p)
    return np.array(theta[:num]), np.array(phi[:num])