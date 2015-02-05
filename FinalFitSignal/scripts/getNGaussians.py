#!/usr/bin/env python

import sys
import numpy
import os

def getNGaus(proc,cat):
  f = open('dat/in/initFit_%s_cat%d.dat'%(proc,cat))
  lines=[]
  for line in f.readlines():
    gLine = line.split()[0].split('_g')[-1]
    if gLine not in lines: lines.append(gLine)

  print 'CMS-HGG_%s_cat%d.root'%(proc,cat), proc, cat, len(lines), '1 1 1'

from optparse import OptionParser
parser=OptionParser()
parser.add_option("--runAll",dest="runAll",action="store_true",default=False,help="Do all procs all cats")
parser.add_option("-p","--proc",dest="proc",type="str",help="Process")
parser.add_option("-c","--cat",dest="cat",type="int",help="Category")
(options,args)=parser.parse_args()

if options.runAll:
  for proc in ['ggh','vbf','wzh','tth']:
    for cat in range(0,9):
      getNGaus(proc,cat,)
else:
  getNGaus(options.proc,options.cat)


