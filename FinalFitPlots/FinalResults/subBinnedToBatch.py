#!/usr/bin/env python

import os
import sys
import numpy
from optparse import OptionParser

parser = OptionParser()
parser.add_option("-p","--path",dest="path")
parser.add_option("-d","--datacard",dest="datacard")
parser.add_option("-D","--newDir",dest="newDir",default="./")
parser.add_option("-q","--queue",dest="queue",default="1nh")
parser.add_option("-L","--mHlow",dest="mHlow",type="int",default=110)
parser.add_option("-H","--mHhigh",dest="mHhigh",type="int",default=150)
parser.add_option("-S","--mHstep",dest="mHstep",type="float",default=1)
parser.add_option("","--dryRun",dest="dryRun",default=False,action="store_true")
parser.add_option("","--unblind",dest="unblind",default=False,action="store_true")
parser.add_option("","--mcMasses",dest="mcMasses",default=False,action="store_true")
(options,args)=parser.parse_args()

path = options.path 
dir = options.newDir
datacard = options.datacard

if options.mcMasses:
  masses="{110,115,120,125,130,135,140,145,150}"
else:
  masses="$(seq %d %3.1f %d)"%(options.mHlow,options.mHstep,options.mHhigh)

if not os.path.isdir('%s/%s'%(path,dir)):
  os.makedirs('%s/%s'%(path,dir))

if options.unblind:
  nDirs = ['Asymptotic','ProfileLikelihood','MaxLikelihoodFit','ExpProfileLikelihood']
else:
  nDirs = ['Asymptotic','ExpProfileLikelihood']

for n in nDirs:
  if not os.path.isdir('%s/%s/%s'%(path,dir,n)):
    os.makedirs('%s/%s/%s'%(path,dir,n))

for type in nDirs:
  f = open('%s/%s/%s/sub.sh'%(path,dir,type),'w')
  f.write('#!/bin/bash\n')
  f.write('cd %s/%s/%s\n'%(path,dir,type))
  f.write('eval `scramv1 runtime -sh`\n')
  f.write('touch %s.run\n'%f.name)
  f.write('if ( \n')
  f.write('\tfor m in %s; do\n'%masses)
  if type=='Asymptotic': 
    if options.unblind: f.write('\t\t combine %s/%s -M Asymptotic -m $m\n'%(path,datacard))
    else: f.write('\t\t combine %s/%s -M Asymptotic -m $m --run=expected\n'%(path,datacard))
  if type=='ProfileLikelihood':
    if options.unblind: f.write('\t\t combine %s/%s -M ProfileLikelihood --signif --pvalue -m $m\n'%(path,datacard))
    else: print 'Blinded: not running ProfileLikelihood'
  if type=='MaxLikelihoodFit':
    if options.unblind: f.write('\t\t combine %s/%s -M MaxLikelihoodFit --rMin=-3. --rMax=3. -m $m\n'%(path,datacard))
    else: print 'Blinded: not running MaxLikelihoodFit'
  if type=='ExpProfileLikelihood': 
    f.write('\t\t combine %s/%s -M ProfileLikelihood --expectSignal=1 -t -1 --signif --pvalue -m $m\n'%(path,datacard))
  f.write('\tdone\n')
  f.write(') then\n')
  f.write('\t touch %s.done\n'%f.name)
  f.write('\t hadd %s.root higgsCombine*\n'%type)
  f.write('else\n')
  f.write('\ttouch %s.fail\n'%f.name)
  f.write('fi')
  f.close()
  os.system('chmod +x %s'%f.name)

for type in nDirs:
  if not options.dryRun: os.system('bsub -q %s -o %s/%s/%s/sub.sh.log %s/%s/%s/sub.sh'%(options.queue,path,dir,type,path,dir,type))
