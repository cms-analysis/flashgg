#!/usr/bin/env python

import os
import sys
import numpy
from optparse import OptionParser

parser = OptionParser()
parser.add_option("-p","--path",dest="path")
parser.add_option("-d","--datacard",dest="datacard")
parser.add_option("-M","--method",dest="method")
parser.add_option("-D","--newDir",dest="newDir",default="./")
parser.add_option("-q","--queue",dest="queue")
parser.add_option("-e","--expectedOnly",dest="expectedOnly",default=False,action="store_true")
parser.add_option("-L","--mHlow",dest="mHlow",type="int",default=110)
parser.add_option("-H","--mHhigh",dest="mHhigh",type="int",default=150)
parser.add_option("-S","--mHstep",dest="mHstep",type="float",default=1)
parser.add_option("-A","--all",dest="all",default=False,action="store_true")
parser.add_option("","--dryRun",dest="dryRun",default=False,action="store_true")
parser.add_option("","--expectSignal",dest="expectSignal",type="float",default=1.)
parser.add_option("","--expectSignalMass",dest="expectSignalMass",type="float")

(options,args)=parser.parse_args()

path = options.path 
dir = options.newDir
datacard = options.datacard
method = options.method
folder = options.method
queue = options.queue
expectedOnly = options.expectedOnly
ml = options.mHlow
mh = options.mHhigh
step = options.mHstep

if method!='Asymptotic' and method!='AsymptoticNew' and method!='ProfileLikelihood' and method!='ExpProfileLikelihood' and method!='MaxLikelihoodFit':
  print method, 'is invalid'
  sys.exit()

if method=='ProfileLikelihood' and expectedOnly:
  folder='ExpProfileLikelihood'
  expectedOnly=False

if method=='ExpProfileLikelihood':
  folder='ExpProfileLikelihood'
  method='ProfileLikelihood'
  expectedOnly=False

if options.expectSignalMass:
  folder+='_m%d'%(options.expectSignalMass)

if not os.path.isdir('%s/%s/%s'%(path,dir,folder)):
  os.makedirs('%s/%s/%s'%(path,dir,folder))

files=[]

def makeScript(m):
  f = open("%s/%s/%s/%s%5.1f.sh"%(path,dir,folder,folder,m),"w")
  f.write('#!/bin/bash\n')
  f.write('cd %s/%s/%s\n'%(path,dir,folder))
  f.write('touch %s.run\n'%f.name)
  f.write('echo --------------------------------------------\n')
  f.write('echo   Running %s on %s at mass %5.1f \n'%(folder,datacard,m)) 
  f.write('echo --------------------------------------------\n')
  f.write('eval `scramv1 runtime -sh`\n')
  line = 'if ( combine %s/%s -M %s -m %5.1f'%(path,datacard,method,m)
  if folder=='MaxLikelihoodFit':
    line += ' --rMin=-3. --rMax=3.'
  if folder=='ProfileLikelihood':
    line += ' --signif --pvalue'
  if folder=='ExpProfileLikelihood':
    line += ' --signif --pvalue -t -1 --expectSignal=%3.1f'%options.expectSignal
  if options.expectSignalMass:
    line += ' --expectSignalMass=%5.1f'%options.expectSignalMass
  if expectedOnly:
    line += ' --run=expected'
  f.write(line+' ) then\n')
  f.write('\ttouch %s.done\n'%f.name)
  f.write('else\n')
  f.write('\t touch %s.fail\n'%f.name)
  f.write('fi\n')
  f.write('rm %s.run\n'%f.name)
  f.close()
  os.system('chmod +x %s'%f.name)
  files.append(f)
  
if not options.all:
  for m in numpy.arange(ml,mh+step,step):
    makeScript(m)

  for f in files:
    if os.path.exists('%s.log'%f.name):
      os.system('rm %s.log'%(f.name))
    if os.path.exists('%s.fail'%f.name):
      os.system('rm %s.fail'%(f.name))
    if os.path.exists('%s.done'%f.name):
      os.system('rm %s.done'%(f.name))
    if os.path.exists('%s.run'%f.name):
      os.system('rm %s.run'%(f.name))
    if not options.dryRun: os.system('bsub -q %s -o %s.log %s'%(queue,f.name,f.name))  
    #print 'bsub -q %s -o %s.log %s'%(queue,f.name,f.name)  

