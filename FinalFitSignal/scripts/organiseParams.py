#!/usr/bin/env python

import sys
import numpy
import os

def orgParams(proc,cat,mh):
  paramfile = open('dat/out/initFit_%s_cat%d.dat'%(proc,cat))
  newoutfile = open('dat/in/initFit_%s_cat%d.dat'%(proc,cat),'w')
  mylines=[]
  for line in paramfile.readlines():
    if 'mh%d'%mh in line: mylines.append(line)

  for m in numpy.arange(110,151,5):
    for line in mylines:
      newoutfile.write(line.replace('mh%d'%mh,'mh%d'%m))

  newoutfile.close()
  paramfile.close()

from optparse import OptionParser
parser=OptionParser()
parser.add_option("--runAll",dest="runAll",action="store_true",default=False,help="Do all procs all cats")
parser.add_option("-p","--proc",dest="proc",type="str",help="Process")
parser.add_option("-c","--cat",dest="cat",type="int",help="Category")
parser.add_option("-m","--mh",dest="mh",type="int",default=125,help="Change all params to match this mass")
parser.add_option("--splitVH",dest="splitVH",action="store_true",default=False,help="Split VH process into WH and ZH")
(options,args)=parser.parse_args()

if options.runAll:
  procs = ['ggh','vbf']
  if options.splitVH: procs += ['wh','zh','tth']
  else: procs += ['wzh','tth']
  for proc in procs:
    for cat in range(0,9):
      orgParams(proc,cat,options.mh)
else:
  orgParams(options.proc,options.cat,options.mh)



