#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Utility functions for easy invocation of the simulation from Python code.
#

import os, sys, subprocess

DEVNULL = open("/dev/null", "a")

def call(mac="run.mac", output="./results", args=None, out=False):
    '''
    Calls the simulation.
    '''
    # Create output directory if it does not exist.
    try:
        if not os.path.exists(output) :
            os.mkdir(output)
    except:
        print sys.exc_info()
    # Create exe call string.
    exe = "./sim  --mac=" + mac + " --output=" + output 
    if args != None:
        exe += " " + args
    #
    print "\t" + exe
    stdout = DEVNULL
    if out:
        stdout = open(str(prefix) + ".out", "w")
    return subprocess.Popen(exe, stdout=stdout, stderr=sys.stderr, shell=True)

def write_mac(content, mac="tmp.mac"):
    '''
    @param content - a sequence of strings.
    @param mac - the mac file name.
    '''
    f = open(mac, "w")
    content = [c + "\n" for c in content]
    f.writelines(content)
    f.close()
