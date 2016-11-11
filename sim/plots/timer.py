#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Usage:
# timer = Timer()
# with timer:
#     do stuff ...
# print timer.duration()
#

import time
class Timer(object):
    def __enter__(self):
        self.__start = time.time()

    def __exit__(self, type, value, traceback):
        # Error handling here
        self.__finish = time.time()

    def duration(self):
        return self.__finish - self.__start