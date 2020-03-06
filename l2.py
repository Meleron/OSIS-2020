#!/usr/bin/env python
# -*- coding: utf-8 -*-
import os
import sys

res = 0
for properFile in os.listdir("."):
    if os.path.getsize(properFile) > res:
        res = os.path.getsize(properFile)
        largest_file = properFile
print('Largest file:' + largest_file)
print('File size:' + str(res/1024) + 'kB')