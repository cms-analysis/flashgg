#!/usr/bin/env python

from optparse import OptionParser
parser=OptionParser()
parser.add_option("-D","--readFromDat",dest="readFromDat",type="str",help="Read these run options from datfile")
parser.add_option("-s","--sigfilename",dest="sigfilename",type="str",help="Input signal workspace file")
parser.add_option("-b","--bkgfilename",dest="bkgfilename",type="str",help="Input background/data workspace file")
parser.add_option("-w","--sigwsname",dest="sigwsname",type="str",help="Input signal workspace name",default="cms_hgg_workspace")
parser.add_option("-B","--bkgwsname",dest="bkgwsname",type="str",help="Input background/data workspace name",default="cms_hgg_workspace")
parser.add_option("-d","--datfile",dest="datfile",type="str",help="Config datfile")
parser.add_option("-o","--outerDir",dest="outerDir",help="Dir name")
parser.add_option("-c","--cats",dest="cats",default=[],action="append")
parser.add_option("-m","--expectSignals",default=[],action="append")
parser.add_option("-M","--expectSignalMasses",default=[],action="append")
parser.add_option("-L","--mulow",dest="mulow",type="float",help="Value of mu to start scan")
parser.add_option("-H","--muhigh",dest="muhigh",type="float",help="Value of mu to end scan")
parser.add_option("-S","--mustep",dest="mustep",type="float",help="Value of mu step size")
parser.add_option("-t","--toysperjob",dest="toysperjob",type="int",help="Number of toys to run per job")
parser.add_option("-n","--njobs",dest="njobs",type="int",help="Number of jobs to run")
parser.add_option("-j","--jstart",dest="jstart",type="int",default=0,help="Start job number here")
parser.add_option("-q","--queue",dest="queue",type="str",default="8nh",help="Which queue to run job in")
parser.add_option("-e","--eosPath",dest="eosPath",help="Write output files to eos")
parser.add_option("--takeOtherFiles",type="str",help="Copy these files over to batch sandbox")
parser.add_option("","--skipPlots",default=False,action="store_true",help="Don\'t plot all the envelopes")
parser.add_option("","--dryRun",dest="dryRun",default=False,action="store_true",help="Don't submit")
(options,args)=parser.parse_args()

import os
if options.eosPath:
  if '/eos/cms' in options.eosPath:
    options.eosPath = options.eosPath.split('/eos/cms')[1]
  os.system('cmsMkdir %s'%options.eosPath)

def writeSubScript(cat,mlow,mhigh,mstep,outdir,muInject,massInject):
  cat = int(cat)
  muInject=float(muInject)
  massInject=float(massInject)
  subline = './BiasStudy -s %s -b %s --sigwsname %s --bkgwsname %s -d %s -c %d -L %3.1f -H %3.1f -S %5.3f -t %d -D %s --expectSignal=%3.1f --expectSignalMass=%3d'%(os.path.basename(options.sigfilename),os.path.basename(options.bkgfilename),options.sigwsname,options.bkgwsname,os.path.basename(options.datfile),cat,mlow,mhigh,mstep,options.toysperjob,os.path.abspath(outdir),muInject,massInject)

  if options.skipPlots: subline += ' --skipPlots'

  for j in range(options.jstart,options.njobs):
    f = open('%s/%s/sub_cat%d_job%d.sh'%(os.getcwd(),outdir,cat,j),'w')
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
    f.write('cp %s/bin/BiasStudy .\n'%(os.getcwd()))
    if options.takeOtherFiles:
     for sandbox_file in options.takeOtherFiles.split(','):
       f.write('cp %s . \n'%os.path.abspath(sandbox_file))
    f.write('touch %s.run\n'%(f.name))
    execline = subline + ' -j %d -o BiasStudyOut_cat%d_job%d.root'%(j,cat,j)
    f.write('if ( %s ) then \n'%execline)
    if options.eosPath:
      f.write('\tif ( cmsStage -f BiasStudyOut_cat%d_job%d.root %s/%s/BiasStudyOut_cat%d_job%d.root ) then \n'%(cat,j,options.eosPath,outdir,cat,j))
    else:
      f.write('\tif ( cp -f BiasStudyOut_cat%d_job%d.root %s/%s ) then\n'%(cat,j,os.getcwd(),outdir))
    f.write('\t\ttouch %s.done\n'%(f.name))
    f.write('\t\trm -f %s.run\n'%(f.name))
    f.write('\telse\n')
    f.write('\t\ttouch %s.fail\n'%(f.name))
    f.write('\t\trm -f %s.run\n'%(f.name))
    f.write('\tfi\n')
    f.write('else\n')
    f.write('\ttouch %s.fail\n'%(f.name))
    f.write('fi\n')
    f.close()
    os.system('chmod +x %s'%f.name)
    if not options.dryRun: os.system('bsub -q %s -o %s.log %s'%(options.queue,f.name,f.name))
    else: print 'bsub -q %s -o %s.log %s'%(options.queue,f.name,f.name)

if not options.readFromDat:
  for cat in options.cats:
    cat = int(cat)
    for i, mu in enumerate(options.expectSignals):
      mu=float(mu)
      storage_dir = '%s/cat%d_mu%3.1f'%(options.outerDir,cat,mu)
      if options.eosPath:
        os.system('cmsMkdir %s/%s'%(options.eosPath,storage_dir))
      os.system('mkdir -p %s'%storage_dir)
      writeSubScript(cat,options.mulow,options.muhigh,options.mustep,storage_dir,mu,options.expectSignalMasses[i])

else:
  f = open(options.readFromDat)
  configDict={}
  cats = []
  for linet in f.readlines():
    if linet.startswith('#') or linet.startswith(' ') or linet.startswith('\n'): continue
    if not linet.startswith('cat'):
      #configDict[linet.split('=')[0]] = linet.split('=')[1].strip('\n')
      setattr(options,linet.split('=')[0],linet.split('=')[1].strip('\n'))
    else:
     
      if "cats=" in linet:
        vlist = (linet.split("="))[1]
        cats = [int(v) for v in vlist.split(",")] 
        continue

      linestouse = []

      if "{cat}" in linet :
	for c in cats: linestouse.append(linet.replace("{cat}","%d"%c))
      
      else: linestouse.append(linet)

      for line in linestouse:
       lineConfig = line.split()
       cat = int(lineConfig[0].split('=')[1])
       injectmu = float(lineConfig[1].split('=')[1])
       mlow = float(lineConfig[2].split('=')[1])
       mhigh = float(lineConfig[3].split('=')[1])
       mstep = float(lineConfig[4].split('=')[1])
       injectmass = int(lineConfig[5].split('=')[1])
       storage_dir = '%s/cat%d_mu%3.1f_mass%d'%(options.outerDir,cat,injectmu,injectmass)
       if options.eosPath:
         os.system('cmsMkdir %s/%s'%(options.eosPath,storage_dir))
       os.system('mkdir -p %s'%storage_dir)
       writeSubScript(cat,mlow,mhigh,mstep,storage_dir,injectmu,injectmass)

