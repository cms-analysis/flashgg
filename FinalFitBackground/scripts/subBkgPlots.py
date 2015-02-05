#!/usr/bin/env python

from optparse import OptionParser
parser = OptionParser()
parser.add_option("-b","--bkgfilename",help="Data and background workspace file")
parser.add_option("-s","--sigfilename",help="Signal file (can be binned or parametric) or left blank")
parser.add_option("-d","--outDir",default="BkgPlots",help="Out directory for plots default: %default")
parser.add_option("-c","--cats",type="int",help="Number of categories to run")
parser.add_option("-l","--catLabels",default="mk_default",help="Category labels (comma separated) default will use Category %cat")
parser.add_option("-S","--sqrts",type='int',default=8,help="Sqrt(S) COM energy for finding strings etc")
parser.add_option("--isMultiPdf",default=False,action="store_true",help="Use for multipdf workspaces")
parser.add_option("--doBands",default=False,action="store_true",help="Use to draw bands")
parser.add_option("--useBinnedData",default=False,action="store_true",help="Use binned data")
parser.add_option("--makeCrossCheckProfPlots",default=False,action="store_true",help="Make some cross check plots - this is very slow!!")
parser.add_option("--massStep",type="float",default=0.5,help="Mass step for calculating bands. Use a large number like 5 for quick running")
parser.add_option("--nllTolerance",type="float",default=0.05,help="Tolerance for nll calc in %")
parser.add_option("--blind",default=False,action="store_true",help="Blind the mass spectrum in the range [110,150]")
parser.add_option("--runLocal",default=False,action="store_true",help="Run locally")
parser.add_option("--dryRun",default=False,action="store_true",help="Dont submit jobs")
parser.add_option("-q","--queue",default="8nh")
parser.add_option("-v","--verbose",default=False,action="store_true",help="Print more output")
(options,args) = parser.parse_args()

import os

os.system('mkdir -p %s'%options.outDir)

if options.catLabels=='mk_default':
	options.catLabels=[]
	for cat in range(options.cats):
		options.catLabels.append('Category %d'%cat)
else:
	options.catLabels = options.catLabels.split(',')

for cat in range(options.cats):
	
	f = open('%s/sub%d.sh'%(options.outDir,cat),'w')
	f.write('#!/bin/bash\n')
	f.write('cd %s\n'%os.getcwd())
	f.write('eval `scramv1 runtime -sh`\n')
	execLine = '$CMSSW_BASE/src/h2gglobe/BackgroundProfileFitting/bin/makeBkgPlots -b %s -s %s -o %s/BkgPlots_cat%d.root -d %s -c %d -l \"%s\"'%(options.bkgfilename,options.sigfilename,options.outDir,cat,options.outDir,cat,options.catLabels[cat])
	execLine += " --sqrts %d "%options.sqrts
	if options.doBands:
		execLine += ' --doBands --massStep %5.3f --nllTolerance %5.3f'%(options.massStep,options.nllTolerance)
	if options.blind:
		execLine += ' --blind'
	if options.isMultiPdf:
		execLine += ' --isMultiPdf'
	if options.useBinnedData:
		execLine += ' --useBinnedData'
	if options.makeCrossCheckProfPlots:
		execLine += ' --makeCrossCheckProfPlots'
	if options.verbose:
		execLine += ' --verbose'
	f.write('%s\n'%execLine);
	f.close()
	
	os.system('chmod +x %s'%f.name)
	if options.dryRun:
		print 'bsub -q %s -o %s.log %s'%(options.queue,os.path.abspath(f.name),os.path.abspath(f.name))
	elif options.runLocal:
		os.system('./%s'%f.name)
	else:
		os.system('bsub -q %s -o %s.log %s'%(options.queue,os.path.abspath(f.name),os.path.abspath(f.name)))
	
	
