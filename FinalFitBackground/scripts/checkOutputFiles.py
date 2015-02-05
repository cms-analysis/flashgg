#!/usr/bin/env python

import os
import fnmatch
import sys
import time

from optparse import OptionParser
parser = OptionParser()
parser.add_option("-d","--dir")
parser.add_option("-g","--grep",default="*.root")
parser.add_option("-t","--timeLater")
(options,args) = parser.parse_args()

def time_filter(root, my_files, usertime):
  temp_files=[]
  for file in my_files:
    if os.path.getmtime(root+'/'+file)>usertime:
      temp_files.append(usertime)
  return temp_files

running_sum=0
for root, dirs, files in os.walk(options.dir):
  if len(dirs)==0:
    my_files = fnmatch.filter(files,options.grep)
    if options.timeLater: time_filter(root,my_files,options.timeLater)
    running_sum += len(my_files)
    print '%s/%15s -- %d'%(os.path.basename(os.path.abspath(os.path.join(root,os.pardir))),os.path.basename(root),len(my_files))

print 'Total matches =', running_sum
