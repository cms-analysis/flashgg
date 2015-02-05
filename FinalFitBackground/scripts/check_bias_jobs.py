#!/usr/bin/env python

import os
import sys
import fnmatch

from optparse import OptionParser
parser=OptionParser()

parser.add_option("-r","--resubFailed",default=False,action="store_true",help="Resubmit failed jobs")
#parser.add_option("","--rerun",default=False,action="store_true",help="Resubmit jobs in run status")
parser.add_option("-q","--queue",dest="queue",type="str",default="8nh",help="Which queue to run job in")
(options,args)=parser.parse_args()
dir = args[0]

# check completed jobs
comp={}
fail={}
failedscripts=[]
#run={}
#runedscripts=[]
for root,dirs,files in os.walk(dir):
  for file in fnmatch.filter(files,'*.sh'):
    cat = int(file.split('cat')[-1].split('_')[0])
    job = int(file.split('job')[-1].split('.sh')[0])
    if os.path.isfile('%s/%s.done'%(root,file)):
      if cat not in comp:
        comp[cat] = [job]
      else:
        comp[cat].append(job)
    if os.path.isfile('%s/%s.fail'%(root,file)):
      if cat not in fail:
        fail[cat] = [job]
      else:
        fail[cat].append(job)
      failedscripts.append('%s/%s'%(root,file))
 #   if os.path.isfile('%s/%s.run'%(root,file)):
 #     if cat not in run:
 #       run[cat] = [job]
 #     else:
 #       run[cat].append(job)
 #     runedscripts.append('%s/%s'%(root,file))

print 'Completed jobs:'
for key,item in comp.items():
  print '\t cat', key, ' - ', len(item)

print 'Failed jobs:'
for key,item in fail.items():
  print '\t cat', key, ' - ', len(item)

#print 'Running jobs:'
#for key,item in run.items():
#  print '\t cat', key, ' - ', len(item)

if options.resubFailed: 
  for fi in failedscripts:
    os.system('bsub -q %s -o %s.log < ./%s'%(options.queue,fi,fi))
    #print 'bsub -q %s -o %s.log < ./%s'%(options.queue,fi,fi)
  print "done subbing failures"

#if options.rerun: 
#  for fi in runedscripts:
#    os.system('bsub -q %s -o %s.log < ./%s'%(options.queue,fi,fi))
#  print "done re-subbing running jobs"
  

if options.resubFailed: sys.exit("Re-run without --resubFailed to merge jobs")
#if options.rerun : sys.exit("Re-run without --rerun to merge jobs")

answer = raw_input('Continue to merge jobs? (y/yes)')
if not answer=="y" or answer=="yes": sys.exit("done")

import ROOT as r
# merge jobs
for cat, jobs in comp.items():
  
  ofile = r.TFile('%s/BiasStudyOut_cat%d.root'%(dir,cat),'RECREATE')
  chain = r.TChain("muTree_cat%d"%cat)
  for job in jobs:
    chain.Add('%s/BiasStudyOut_cat%d_job%d.root/muTree'%(dir,cat,job))

  chain.Write()
  ofile.Close()

