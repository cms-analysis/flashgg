#!/usr/bin/env python
# standard is to run on histograms_CMS-HGG.root for data bkg sig separately
# run for mode in {data,bkg,ggh,vbf,wzh,tth}; do ./scripts/subFMTBatch.py -p $PWD -i files/histograms_CMS-HGG_

import os
import sys

from optparse import OptionParser

parser = OptionParser()
parser.add_option("-p","--path",dest="path")
parser.add_option("-i","--infile",dest="inputFile")
parser.add_option("-o","--outfile",dest="outputFile")
parser.add_option("-d","--datfile",dest="datFile")
parser.add_option("-w","--webfolder",dest="web")
parser.add_option("-m","--mode",dest="mode",default="all")
parser.add_option("","--dryRun",dest="dryRun",default=False,action="store_true")
(options,args)=parser.parse_args()

dir = options.path
infile = options.inputFile
outfile = options.outputFile
datfile = options.datFile

modes=[]
if options.mode=='all':
  modes = ['data','bkg','ggh','vbf','wzh','tth']
else:
  modes = options.mode.split(',')

print modes

for mode in modes:
  if mode!='data' and mode!='bkg' and mode!='ggh' and mode!='vbf' and mode!='wzh' and mode!='tth':
    print mode, 'is not a valid mode'
    continue
  file = open('%s/scripts/%s.sh'%(dir,mode),'w')
  file.write('#!/bin/bash\n')
  file.write('cd %s\n'%dir)
  file.write('rm %s.success\n'%file.name)
  file.write('rm %s.fail\n'%file.name)
  file.write('rm %s.log\n'%file.name)
  file.write('eval `scramv1 runtime -sh`\n')
  file.write('\n')
  file.write('if ( %s/bin/RunFullMvaAnalysis -i %s/%s -o %s/files/CMS-HGG_fmt_hists_%s.root --skipRebin --histosFromTrees --useDat %s/%s -v -D --histFromTreeMode %s ); \n'%(dir,dir,infile,dir,mode,dir,datfile,mode))
  file.write('\tthen touch %s.success\n'%file.name)
  file.write('else touch %s.fail\n'%file.name)
  file.write('fi\n')
  file.close()
  os.system('chmod +x %s'%file.name)
  if not options.dryRun: os.system('bsub -q 8nh -o %s.log %s'%(file.name,file.name))

# now do everything else

if options.mode=='all':
  f = open('%s/scripts/batchFMT.sh'%dir,'w')
  f.write('#!/bin/bash\n')
  f.write('cd %s\n'%dir)
  f.write('mkdir -p %s/files/backup\n'%dir)
  f.write('eval `scramv1 runtime -sh`\n')
  f.write('\n')
  #f.write('# Clear plot directory\n')
  #f.write('rm -rf %s/plots/*\n\n'%(dir))

  f.write('# now do rebinning and fitting\n')
  f.write('if ( %s/bin/RunFullMvaAnalysis -i %s/files/CMS-HGG_fmtHistos.root -o %s/%s --dumpDatFile %s/dat/mvaanalysis_fmtedges.dat --diagnose -v --useDat %s/%s ); \n'%(dir,dir,dir,outfile,dir,dir,datfile))
  f.write('\tthen cp %s/%s %s/files/backup/CMS-HGG_fmtRebinned.root\n'%(dir,outfile,dir))
  f.write('\techo \'rebinned file written to %s/%s and copied to %s/files/backup/CMS-HGG_fmtRebinned.root\'\n'%(dir,outfile,dir))
  f.write('else\n\ttouch %s/%s.fail \n\texit \nfi\n\n'%(dir,f.name))

  f.write('# now do background model\n')
  f.write('if ( %s/bin/RunFullMvaAnalysis -i %s/files/CMS-HGG_fmtHistos.root -o %s/%s --useDat %s/dat/mvaanalysis_fmtedges.dat --skipRebin --bkgModel --diagnose -v ); \n'%(dir,dir,dir,outfile,dir))
  f.write('\tthen cp %s/%s %s/files/backup/CMS-HGG_fmtLastSuceess.root\n'%(dir,outfile,dir))
  f.write('\techo \'corrected background file written to %s/%s and copied to %s/files/backup/CMS-HGG_fmtLastSuccess.root\'\n'%(dir,outfile,dir))
  f.write('else\n\ttouch %s/%s.fail \n\texit \nfi\n\n'%(dir,f.name))

  f.write('# now do interpolation\n')
  f.write('if ( %s/bin/RunFullMvaAnalysis -i %s/files/CMS-HGG_fmtHistos.root -o %s/%s --useDat %s/dat/mvaanalysis_fmtedges.dat --skipRebin --interp --diagnose -v ); \n'%(dir,dir,dir,outfile,dir))
  f.write('\tthen cp %s/%s %s/files/backup/CMS-HGG_fmtLastSuceess.root\n'%(dir,outfile,dir))
  f.write('\techo \'interpolated file written to %s/%s and copied to %s/files/backup/CMS-HGG_fmtLastSuccess.root\'\n'%(dir,outfile,dir))
  f.write('else\n\ttouch %s/%s.fail \n\texit \nfi\n\n'%(dir,f.name))

  f.write('# now do write datacards\n')
  f.write('if ( %s/bin/RunFullMvaAnalysis -i %s/files/CMS-HGG_fmtHistos.root -o %s/%s --useDat %s/dat/mvaanalysis_fmtedges.dat --skipRebin --datacards --diagnose -v ); \n'%(dir,dir,dir,outfile,dir))
  f.write('\tthen echo \'datacards written to %s/mva-datacards-grad\'\n'%(dir))
  f.write('else\n\ttouch %s/%s.fail \n\texit \nfi\n\n'%(dir,f.name))

  f.write('# now make plots\n')
  f.write('if ( %s/bin/RunFullMvaAnalysis -i %s/files/CMS-HGG_fmtHistos.root -o %s/%s --useDat %s/dat/mvaanalysis_fmtedges.dat --skipRebin --doPlot --diagnose -v --www %s ); \n'%(dir,dir,dir,outfile,dir,options.web))
  f.write('\tthen cp %s/%s %s/files/backup/CMS-HGG_fmtComplete.root\n'%(dir,outfile,dir))
  f.write('\techo \'complete file written to %s/%s and copied to %s/files/backup/CMS-HGG_fmtComplete.root\'\n'%(dir,outfile,dir))
  f.write('else\n\ttouch %s/%s.fail \n\texit \nfi\n\n'%(dir,f.name))

  f.write('# now run combine\n')
  f.write('if ( %s/bin/RunFullMvaAnalysis -i %s/files/CMS-HGG_fmtHistos.root -o %s/%s --useDat %s/dat/mvaanalysis_fmtedges.dat --skipRebin --runCombine --diagnose -v ); \n'%(dir,dir,dir,outfile,dir))
  f.write('\tthen echo \'combine successful out put written to %s\''%(dir))
  f.write('else\n\ttouch %s/%s.fail \n\texit \nfi\n\n'%(dir,f.name))

  f.write('touch %s/%s.success\n\n'%(dir,f.name))

  f.close()
  os.system('chmod +x %s'%f.name)
  #if not options.dryRun: os.system('bsub -q 8nh -o %s.log %s'%(f.name,f.name))


