#!/usr/bin/env python

import os
import fnmatch
import sys

from optparse import OptionParser
parser = OptionParser()
parser.add_option("-d","--dir")
parser.add_option("-e","--eosdir")
parser.add_option("--dryRun",default=False,action="store_true")
(options,args) = parser.parse_args()

done_files=[]
out_files=[]

for root, dirs, files in os.walk(options.dir):
  for file in fnmatch.filter(files,'*.done'):
    job = file.split('.sh.done')[0].split('_job')[1]
    cat = file.split('_job')[0].split('_cat')[1]
    basedir = os.path.basename(root)
    subname = file.split('.done')[0]
    if not os.path.exists('%s/%s/%s/BiasStudyOut_cat%s_job%s.root'%(options.eosdir,options.dir,basedir,cat,job)):
      if not options.dryRun: os.system('bsub -q 8nh -o %s/%s.log %s/%s'%(os.path.abspath(root),subname,os.path.abspath(root),subname))
      else: print root, subname


"""
for root, dirs, files in os.walk(options.eosdir+'/'+options.dir):
  for file in fnmatch.filter(files,"*.root"):
    job = file.split('.sh.done')[0].split('_job')[1]
    cat = file.split('_job')[0].split('_cat')[1]
    print root, file, job, cat
"""
    

