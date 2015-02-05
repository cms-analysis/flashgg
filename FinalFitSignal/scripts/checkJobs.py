#!/usr/bin/env python

import sys
import os
import fnmatch

dirs = ['initialFit','finalFit','mhFit','simFit']
procs = ['ggh','vbf','wzh','tth']
ncats = 9

def organise():
  for dir in dirs:
    for proc in procs:
      for cat in range(ncats):
        os.system('mkdir -p plots/%s/%s_cat%d'%(dir,proc,cat))
        os.system('mv plots/%s/*%s_cat%d*.p* plots/%s/%s_cat%d/'%(dir,proc,cat,dir,proc,cat))

if len(sys.argv)!=2:
  sys.exit('usage checkJobs.py <dir_with_jobs>')

jobdir=sys.argv[1]

if not os.path.isfile('%s/filestocombine.dat'%jobdir):
  sys.exit('no filestocombine.dat file found')

combinefile = '%s/filestocombine.dat'%jobdir

for root, dirs, files in os.walk(jobdir):
  running_jobs = fnmatch.filter(files,'*.sh.run')
  finished_jobs = fnmatch.filter(files,'*.sh.done')
  failed_jobs = fnmatch.filter(files,'*.sh.fail')
  all_jobs = fnmatch.filter(files,'*.sh')

print 'Jobs complete:'
print '\t', finished_jobs
print 'Jobs running:'
print '\t', running_jobs
print 'Jobs failed:'
print '\t', failed_jobs

queued_jobs=[]
for job in all_jobs:
  if os.path.isfile('%s%s.done'%(jobdir,job)): continue
  elif os.path.isfile('%s%s.run'%(jobdir,job)): continue
  elif os.path.isfile('%s%s.fail'%(jobdir,job)): continue
  else: queued_jobs.append(job)

print 'Jobs queued:'
print '\t', queued_jobs

if len(all_jobs)!=len(finished_jobs):
  sys.exit('Some jobs are still queued or have not been submitted. Please check')
  

if len(running_jobs)==0 and len(failed_jobs)==0 and len(finished_jobs)>0:
  print 'All jobs complete'
  print 'Will run script to package output.'
  #filRes = raw_input('Do you want to organise plots? (y/n)\n')
  #if filRes=='y' or filRes=='Y':
  #  organise()
  webRes = raw_input('Do you want to publish plots to web? (y/n)\n')
  if webRes=='n' or webRes=='N':
    os.system('./bin/PackageOutput -i %s -o %s/CMS-HGG_sigfit.root'%(combinefile,jobdir))
  elif webRes=='y' or webRes=='Y':
    webdir = raw_input('Enter web directory\n')
    os.system('./bin/PackageOutput -i %s -o %s/CMS-HGG_sigfit.root -w %s'%(combinefile,webdir,jobdir))
  else:
    sys.exit('Result not recognised')

