#!/usr/bin/env python

from optparse import OptionParser
parser=OptionParser()
parser.add_option("-s","--sigfilename",dest="sigfilename",type="str",help="Input signal workspace file")
parser.add_option("-b","--bkgfilename",dest="bkgfilename",type="str",help="Input background/data workspace file")
parser.add_option("-o","--outputdir",dest="outputdir",type="str",help="Output directory location")
parser.add_option("-d","--datfile",dest="datfile",type="str",help="Config datfile")
parser.add_option("-c","--cat",dest="cat",type="int",default=-1,help="Run specfic cat (default runs all)")
parser.add_option("-L","--mulow",dest="mulow",type="float",help="Value of mu to start scan")
parser.add_option("-H","--muhigh",dest="muhigh",type="float",help="Value of mu to end scan")
parser.add_option("-S","--mustep",dest="mustep",type="float",help="Value of mu step size")
parser.add_option("","--expectSignal",type="float",help="Inject signal in toys")
parser.add_option("","--skipPlots",default=False,action="store_true",help="Don\'t plot all the envelopes")
parser.add_option("-t","--toysperjob",dest="toysperjob",type="int",help="Number of toys to run per job")
parser.add_option("-n","--njobs",dest="njobs",type="int",help="Number of jobs to run")
parser.add_option("-j","--jstart",dest="jstart",type="int",default=0,help="Start job number here")
parser.add_option("-q","--queue",dest="queue",type="str",default="8nh",help="Which queue to run job in")
parser.add_option("","--dryRun",dest="dryRun",default=False,action="store_true",help="Don't submit")
(options,args)=parser.parse_args()

import os

os.system("mkdir -p %s"%options.outputdir)

def makejob(cat):

  subline = '%s/bin/BiasStudy -s %s -b %s -d %s -c %d -L %3.1f -H %3.1f -S %5.3f -t %d -D %s'%(os.getcwd(),os.path.basename(options.sigfilename),os.path.basename(options.bkgfilename),os.path.basename(options.datfile),options.cat,options.mulow,options.muhigh,options.mustep,options.toysperjob,os.path.abspath(options.outputdir))
  if options.expectSignal: subline += ' --expectSignal=%3.1f'%(options.expectSignal)
  if options.skipPlots: subline += ' --skipPlots'

  for j in range(options.jstart,options.njobs):
    f = open('%s/%s/sub_cat%d_job%d.sh'%(os.getcwd(),options.outputdir,cat,j),'w')
    f.write('#!/bin/bash\n')
    f.write('rm -f %s.done\n'%(f.name))
    f.write('rm -f %s.fail\n'%(f.name))
    f.write('rm -f %s.run\n'%(f.name))
    f.write('rm -f %s.log\n'%(f.name))
    f.write('cd %s\n'%os.getcwd())
    f.write('eval `scramv1 runtime -sh`\n')
    f.write('cd -\n')
    f.write('cp %s .\n'%os.path.abspath(options.sigfilename))
    f.write('cp %s .\n'%os.path.abspath(options.bkgfilename))
    f.write('cp %s .\n'%os.path.abspath(options.datfile))
    f.write('touch %s.run\n'%(f.name))
    execline = subline + ' -j %d -o BiasStudyOut_cat%d_job%d.root'%(j,cat,j)
    f.write('if ( %s ) then \n'%execline)
    f.write('\ttouch %s.done\n'%(f.name))
    f.write('\tcp BiasStudyOut_cat%d_job%d.root %s/%s\n'%(cat,j,os.getcwd(),options.outputdir))
    f.write('\trm -f %s.run\n'%(f.name))
    f.write('else\n')
    f.write('\ttouch %s.fail\n'%(f.name))
    f.write('fi\n')
    f.close()
    os.system('chmod +x %s'%f.name)
    if not options.dryRun: os.system('bsub -q %s -o %s.log %s'%(options.queue,f.name,f.name))
    else: print 'bsub -q %s -o %s.log %s'%(options.queue,f.name,f.name)

cats_to_run=[]
if options.cat==-1:
  cats_to_run=range(0,9)
else:
  cats_to_run.append(options.cat)

for cat in cats_to_run:
  makejob(cat)
