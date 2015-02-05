#!/usr/bin/env python

from optparse import OptionParser
parser = OptionParser()
parser.add_option("-i","--inputfilename",help="Data workspace file from Globe")
parser.add_option("-o","--outDir",default="plots",help="Out directory for plots default: %default")
parser.add_option("-O","--Options",default="",help="Add options")
parser.add_option("-q","--queue",default="8nh",help="Batch queue")
parser.add_option("-d","--datfile",default="dat/fTest.dat",help="dat file for the output")
parser.add_option("-s","--saveMultiPdf",default="",help="Save a multipdf workspace as this name")
parser.add_option("-c","--cats",type="int",help="Number of categories to run")
parser.add_option("--unblind",default=False,action="store_true",help="Unblind the mass spectrum in the range [110,150]")
parser.add_option("--dryRun",default=False,action="store_true",help="Dont submit jobs")
parser.add_option("-v","--verbose",default=False,action="store_true",help="Print more output")
parser.add_option("-S","--sqrts",type='int',default=8,help="Sqrt(S) COM energy for finding strings etc")
(options,args) = parser.parse_args()

import os
options.catLabels=[]
for cat in range(options.cats):
	options.catLabels.append('Category %d'%cat)

os.system("mkdir -p %s"%options.outDir)
for cat in range(options.cats):
	
	f = open('%s/ftest-sub%d.sh'%(options.outDir,cat),'w')
	f.write('#!/bin/bash\n')
	f.write('cd %s\n'%os.getcwd())
	f.write('eval `scramv1 runtime -sh`\n')	
	execLine = '$CMSSW_BASE/src/h2gglobe/BackgroundProfileFitting/bin/fTest -i %s -D %s --singleCat %d '%(options.inputfilename,options.outDir,cat)
        execLine+=" %s "%options.Options
	if options.unblind:
		execLine += ' --unblind'
	if options.verbose:
		execLine += ' --verbose'
	if options.sqrts==7:
		execLine += ' --is2011 '
	if options.datfile:
	  datfile=options.datfile[0:options.datfile.find(".dat")]+"_cat%d"%cat+".dat"
	  execLine += " -d %s" %datfile
	if options.saveMultiPdf:
	  saveMultiPdf=options.saveMultiPdf[0:options.saveMultiPdf.find(".root")]+"_cat%d"%cat+".root"
	  execLine += " --saveMultiPdf %s" %saveMultiPdf
	f.write('%s\n'%execLine);
	f.close()
	
	os.system('chmod +x %s'%f.name)
	if options.dryRun:
		print 'bsub -q %s -o %s.log %s'%(options.queue,os.path.abspath(f.name),os.path.abspath(f.name))
	else:
		os.system('bsub -q %s -o %s.log %s'%(options.queue,os.path.abspath(f.name),os.path.abspath(f.name)))
	
	
