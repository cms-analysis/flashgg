#!/usr/bin/env python

import os
import sys
from optparse import OptionParser
parser=OptionParser()
parser.add_option("-D","--dir")
parser.add_option("-d","--datfile")
parser.add_option("--eosWalk",type="int")
parser.add_option("-q","--queue",default="8nm")
parser.add_option("--dryRun",default=False,action="store_true")
(options,args)=parser.parse_args()

os.system('mkdir -p %s'%options.dir)

def writeSpec(dat,i):
  f = open('%s/sub%d.sh'%(os.path.abspath(options.dir),i),'w')
  f.write('#!/bin/bash\n')
  f.write('rm -f %s.done\n'%(f.name))
  f.write('rm -f %s.fail\n'%(f.name))
  f.write('rm -f %s.run\n'%(f.name))
  f.write('rm -f %s.log\n'%(f.name))
  f.write('cd %s\n'%os.getcwd())
  f.write('eval `scramv1 runtime -sh`\n')
  f.write('cd -\n')
  f.write('cp %s/scripts/make_hists_from_raw_files.py .\n'%os.getcwd())
  f.write('cp %s .\n'%os.path.abspath(dat))
  f.write('mkdir lib\n')
  f.write('cp %s/lib/libBackgroundProfileFitting.so lib/\n'%os.getcwd())
  f.write('touch %s.run\n'%(f.name))
  subline = './make_hists_from_raw_files.py -d %s --runSpecificFiles=%d'%(os.path.basename(dat),i)
  if options.eosWalk:
    subline += ' --eosWalk=%d'%options.eosWalk
  f.write('if ( %s ) then \n'%subline)
  f.write('\ttouch %s.done\n'%(f.name))
  f.write('\trm -f %s.run\n'%(f.name))
  f.write('else\n')
  f.write('\ttouch %s.fail\n'%(f.name))
  f.write('fi\n')
  f.close()
  os.system('chmod +x %s'%f.name)
  if not options.dryRun: os.system('bsub -q %s -o %s.log %s'%(options.queue,f.name,f.name))
  else: print 'bsub -q %s -o %s.log %s'%(options.queue,f.name,f.name)


def writeTmpDat(dat):
 fi = open(dat)
 nam = options.dir+"/"+(dat.split("/")[-1])+"_tempdat.dat"
 tmp = open(nam,"w")
 cats = []
 for line in fi.readlines():
  if line.startswith('#') or line=='' or line.startswith('\n') or len(line)<=1: 
	continue
  if "cats=" in line:
    vlist = (line.split("="))[1]
    cats = [int(v) for v in vlist.split(",")] 
    continue
  if "{cat}" in line :
	for c in cats:
	  tmp.write(line.replace("{cat}","%d"%c))
  else: 
    if not ( line.startswith('#') or line=='	' or line.startswith('\n') or len(line.strip())<=1): 
	tmp.write(line)
 return nam

tmpdatfile = writeTmpDat(options.datfile)
df = open(tmpdatfile)
i=0
for line in df.readlines():
  if line.startswith('#') or line=='' or line =='\n' or len(line)==0: continue
  writeSpec(tmpdatfile,i)
  i+=1
