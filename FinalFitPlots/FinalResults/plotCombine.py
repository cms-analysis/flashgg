#!/usr/bin/env python

# e.g. ./plotCombine.py --unblind --run2011 --lumi2011=5.1 --splitChannels2011="cat0_7TeVcat1_7TeVcat2_7TeVcat3_7TeV:Inclusive:4" --splitChannels2011="cat4_7TeV:Dijet Tag:2"
# e.g. /plotCombine.py --unblind --run2012 --lumi2012=19.6 --splitChannels2012="cat0cat1cat2cat3:Inclusive:4" --splitChannels2012="cat4cat5:Dijet Tag:2" --splitChannels2012="cat6cat7:Lepton Tag:3" --splitChannels2012="cat8:MET Tag:6"
# e.g. ./plotCombine.py --unblind --runBoth --lumi2011=5.1 --lumi2012=19.6 --splitChannelsBoth="ch1_cat0_7TeVch1_cat1_7TeVch1_cat2_7TeVch1_cat3_7TeVch2_cat0_8TeVch2_cat1_8TeVch2_cat2_8TeV ch2_cat3_8TeV:Inclusive:4" --splitChannelsBoth="ch1_cat4_7TeVch2_cat4_8TeVch2_cat5_8TeV:Dijet Tag:2" --splitChannelsBoth="ch2_cat6_8TeVch2_cat7_8TeVch2_cat8_8TeV:Lepton/MET Tag:6"

# one command to rule them all
# e.g. ./plotCombine.py --unblind --run2011 --lumi2011=5.1 --splitChannels2011="cat0_7TeVcat1_7TeVcat2_7TeVcat3_7TeV:Inclusive:4" --splitChannels2011="cat4_7TeV:Dijet Tag:2" --run2012 --lumi2012=19.6 --splitChannels2012="cat0cat1cat2cat3:Inclusive:4" --splitChannels2012="cat4cat5:Dijet Tag:2" --splitChannels2012="cat6cat7:Lepton Tag:3" --splitChannels2012="cat8:MET Tag:6" --runBoth --splitChannelsBoth="ch1_cat0_7TeVch1_cat1_7TeVch1_cat2_7TeVch1_cat3_7TeVch2_cat0_8TeVch2_cat1_8TeVch2_cat2_8TeV ch2_cat3_8TeV:Inclusive:4" --splitChannelsBoth="ch1_cat4_7TeVch2_cat4_8TeVch2_cat5_8TeV:Dijet Tag:2" --splitChannelsBoth="ch2_cat6_8TeVch2_cat7_8TeVch2_cat8_8TeV:Lepton/MET Tag:6"


import os
import numpy

from optparse import OptionParser

parser = OptionParser()
parser.add_option("","--unblind",dest="unblind",default=False,action="store_true",help="Unblind")
parser.add_option("","--parametric",dest="parametric",default=False,action="store_true",help="Run parametric model")
parser.add_option("","--run2011",dest="run2011",default=False,action="store_true",help="Run only 2011")
parser.add_option("","--run2012",dest="run2012",default=False,action="store_true",help="Run only 2012")
parser.add_option("","--runBoth",dest="runBoth",default=False,action="store_true",help="Run only 2011+2012")
parser.add_option("","--lumi2011",dest="lumi2011",type="float",default=5.1,help="2011 luminosity")
parser.add_option("","--lumi2012",dest="lumi2012",type="float",default=19.6,help="2012 luminosity")
parser.add_option("","--skipDatacard",dest="skipDatacard",default=False,action="store_true",help="Skip the original datacard (if running sub channels)")
parser.add_option("","--splitChannels2011",dest="splitChannels2011",default=[],action="append",help="Run these channels for 2011 (can pass multiple times) Split with a colon - dir:leg:col")
parser.add_option("","--splitChannels2012",dest="splitChannels2012",default=[],action="append",help="Run these channels for 2012 (can pass multiple times) Split with a colon - dir:leg:col")
parser.add_option("","--splitChannelsBoth",dest="splitChannelsBoth",default=[],action="append",help="Run these channels for Both (can pass multiple times) Split with a colon - dir:leg:col")
parser.add_option("","--skipHadd",dest="skipHadd",default=False,action="store_true",help="Skip hadding of combine output")
parser.add_option("-M","--methods",dest="methods",default=[],action="append",help="Combine methods - default does several (can pass multiple times)")
parser.add_option("-S","--doSpecials",dest="specials",default=[],action="append",help="Run specials. Split with colon - model:mh (e.g MuMH:125)")
(options,args)=parser.parse_args()

os.system('mkdir -p plots')

def hadd(folder,method,subfolder=''):
  if method=='ExpProfileLikelihood' or method=='ExpProfileLikelihoodm125':
    file_method = 'ProfileLikelihood'
  else:
    file_method = method
  os.system('hadd -f %s/%s/%s/%s.root %s/%s/%s/higgsCombine*%s*.root'%(folder,method,subfolder,method,folder,method,subfolder,file_method))

def plotAsymptotic(folder,lumistring):
  if 'Asymptotic' in options.methods:
    if options.unblind:
      os.system('./makeCombinePlots.py -f %s/Asymptotic/Asymptotic.root --text="%s" --limit'%(folder,lumistring))
      os.system('cp limit.pdf plots/%s_obslimit.pdf'%folder)
      os.system('cp limit.png plots/%s_obslimit.png'%folder)
      os.system('cp limit.C plots/%s_obslimit.C'%folder)
    else:
      os.system('./makeCombinePlots.py -f %s/Asymptotic/Asymptotic.root --text="%s" --limit -e'%(folder,lumistring))
      os.system('cp limit.pdf plots/%s_explimit.pdf'%folder)
      os.system('cp limit.png plots/%s_explimit.png'%folder)
      os.system('cp limit.C plots/%s_explimit.C'%folder)

def plotProfileLikelihood(folder,lumistring,splitChannels):
  line=""
  if 'ExpProfileLikelihood' in options.methods:
    for job in splitChannels:
      subfolder = job.split(':')[0].replace(' ','')
      color = int(job.split(':')[2])
      name = job.split(':')[1]
      if options.unblind:
        line += ' -f %s/ExpProfileLikelihood/%s/ExpProfileLikelihood.root -c %d -s 2 -w 2 -n "-1"'%(folder,subfolder,color)
      else:
        line += ' -f %s/ExpProfileLikelihood/%s/ExpProfileLikelihood.root -c %d -s 2 -w 2 -n "%s"'%(folder,subfolder,color,name)
    line += ' -f %s/ExpProfileLikelihood/ExpProfileLikelihood.root -c 1 -s 2 -w 2 -n "Expected"'%(folder)
  
  if 'ExpProfileLikelihoodm125' in options.methods:
    line += ' -f %s/ExpProfileLikelihoodm125/ExpProfileLikelihoodm125.root -c 1 -s 7 -w 2 -n "Expected m_{H}=125"'%(folder)
  
  if 'ProfileLikelihood' in options.methods:
    for job in splitChannels:
      subfolder = job.split(':')[0].replace(' ','')
      color = int(job.split(':')[2])
      name = job.split(':')[1]
      line += ' -f %s/ProfileLikelihood/%s/ProfileLikelihood.root -c %d -s 1 -w 2 -n "%s"'%(folder,subfolder,color,name)
    line += ' -f %s/ProfileLikelihood/ProfileLikelihood.root -c 1 -s 1 -w 2 -n "Observed"'%(folder)

  if 'ExpProfileLikelihood' in options.methods or 'ExpProfileLikelihoodm125' in options.methods or 'ProfileLikelihood' in options.methods:
    os.system('./makeCombinePlots.py %s --text="%s" --pval'%(line,lumistring))
    if options.unblind:
      os.system('cp pval.pdf plots/%s_obspval.pdf'%folder)
      os.system('cp pval.png plots/%s_obspval.png'%folder)
      os.system('cp pval.C plots/%s_obspval.C'%folder)
    else:
      os.system('cp pval.pdf plots/%s_exppval.pdf'%folder)
      os.system('cp pval.png plots/%s_exppval.png'%folder)
      os.system('cp pval.C plots/%s_exppval.C'%folder)

def plotMaxLikelihoodFit(folder,lumistring):
  if 'MaxLikelihoodFit' in options.methods:
    os.system('./makeCombinePlots.py -f %s/MaxLikelihoodFit/MaxLikelihoodFit.root --text="%s" --maxlh'%(folder,lumistring))
    os.system('cp maxlh.pdf plots/%s_maxlh.pdf'%folder)
    os.system('cp maxlh.png plots/%s_maxlh.png'%folder)
    os.system('cp maxlh.C plots/%s_maxlh.C'%folder)

def plotPvalAcrossYears(lumistring):
  line = ''
  if 'ExpProfileLikelihood' in options.methods:
    if options.unblind:
      line += ' -f 7TeV/ExpProfileLikelihood/ExpProfileLikelihood.root -c 2 -s 2 -w 2 -n -1 -f 8TeV/ExpProfileLikelihood/ExpProfileLikelihood.root -c 4 -s 2 -w 2 -n -1 -f 7and8TeV/ExpProfileLikelihood/ExpProfileLikelihood.root -c 1 -s 2 -w 2 -n "Expected"'
    else:
      line += ' -f 7TeV/ExpProfileLikelihood/ExpProfileLikelihood.root -c 2 -s 2 -w 2 -n "7TeV Expected" -f 8TeV/ExpProfileLikelihood/ExpProfileLikelihood.root -c 4 -s 2 -w 2 -n "8TeV Expected" -f 7and8TeV/ExpProfileLikelihood/ExpProfileLikelihood.root -c 1 -s 2 -w 2 -n "Combined Expected"'
  
  if 'ExpProfileLikelihoodm125' in options.methods:
    line += ' -f 7TeV/ExpProfileLikelihoodm125/ExpProfileLikelihoodm125.root -c 2 -s 7 -w 2 -n -1 -f 8TeV/ExpProfileLikelihoodm125/ExpProfileLikelihoodm125.root -c 4 -s 7 -w 2 -n -1 -f 7and8TeV/ExpProfileLikelihoodm125/ExpProfileLikelihoodm125.root -c 1 -s 7 -w 2 -n "Expected m_{H}=125"'
  
  if 'ProfileLikelihood' in options.methods:
    line += ' -f 7TeV/ProfileLikelihood/ProfileLikelihood.root -c 2 -s 1 -w 2 -n "7TeV Observed" -f 8TeV/ProfileLikelihood/ProfileLikelihood.root -c 4 -s 1 -w 2 -n "8TeV Observed" -f 7and8TeV/ProfileLikelihood/ProfileLikelihood.root -c 1 -s 1 -w 2 -n "Combined Observed"'

  os.system('./makeCombinePlots.py %s --text="%s" --pval'%(line,lumistring))
  if options.unblind:
    os.system('cp pval.pdf plots/obspval.pdf')
    os.system('cp pval.png plots/obspval.png')
    os.system('cp pval.C plots/obspval.C')
  else:
    os.system('cp pval.pdf plots/exppval.pdf')
    os.system('cp pval.png plots/exppval.png')
    os.system('cp pval.C plots/exppval.C')

# MAIN STARTS HERE
# if a specific year has not been set run everything
if not options.run2011 and not options.run2012 and not options.runBoth:
  options.run2011=True
  options.run2012=True
  options.runBoth=True

# figure out which methods to run
allowed_methods = ['Asymptotic','ProfileLikelihood','ExpProfileLikelihood','MaxLikelihoodFit']
if options.parametric:
  allowed_methods.append('ExpProfileLikelihoodm125')
# if no specific combine methods then do all
if len(options.methods)==0:
  options.methods = allowed_methods
else:
  for option in options.methods:
    if option not in allowed_methods:
      sys.exit('%s is not in allowed_methods: %s'%(option,allowed_methods))

# check specials
allowed_specials = ["ChanComp","Mass","Mu","MuMH","RvRf"]
for spec in options.specials:
  opts=spec.split(':')
  if opts[0] not in allowed_specials:
    sys.exit('%s is not an allowed special option. Must be one of %s'%(opts[0],allowed_specials))

config=[]
if options.run2011:
  config.append(['7TeV','#splitline{}{#sqrt{s}=7TeV L=%3.1ffb^{-1}}'%(options.lumi2011),options.splitChannels2011])
if options.run2012:
  config.append(['8TeV','#splitline{}{#sqrt{s}=8TeV L=%3.1ffb^{-1}}'%(options.lumi2012),options.splitChannels2012])
if options.runBoth:
  config.append(['7and8TeV','#splitline{#sqrt{s}=7TeV L=%3.1ffb^{-1}}{#sqrt{s}=8TeV L=%3.1ffb^{-1}}'%(options.lumi2011,options.lumi2012),options.splitChannelsBoth])

for folder, lumistring, splitChannels in config:
  if not options.skipHadd:
    if not options.skipDatacard:
      for meth in options.methods:
        hadd(folder,meth)
    for job in splitChannels:
      subfolder = job.split(':')[0].replace(' ','')
      hadd(folder,'ExpProfileLikelihood',subfolder)
      hadd(folder,'ProfileLikelihood',subfolder)
  
  if not options.skipDatacard:
    # limit plots
    plotAsymptotic(folder,lumistring)
    # pvalue plots
    plotProfileLikelihood(folder,lumistring,splitChannels)
    # maxlh plots
    plotMaxLikelihoodFit(folder,lumistring)

    # one special plot for all years
    if options.run2011 and options.run2012 and options.runBoth and folder=='7and8TeV':
      plotPvalAcrossYears(lumistring)

  for spec in options.specials:
    opts=spec.split(':')
    model=opts[0]
    mh=opts[1]
    if model=='Mass':
      os.system('./makeCombinePlots.py -f %s/Specials/higgsCombine%sMassSyst.MultiDimFit.mH%s.root -c 1 -s 1 -w 3 -n "Stat + Syst" -f %s/Specials/higgsCombine%sMassStat.MultiDimFit.mH%s.root -c 4 -s 2 -w 3 -n "Stat Only" --text="%s" --mh'%(folder,folder,mh,folder,folder,mh,lumistring))
      os.system('mv MH.pdf plots/%s_mhNLL.pdf'%(folder))
      os.system('mv MH.png plots/%s_mhNLL.png'%(folder))
      os.system('mv MH.C plots/%s_mhNLL.C'%(folder))
    if model=='Mu':
      os.system('./makeCombinePlots.py -f %s/Specials/higgsCombine7and8TeVMuSyst.MultiDimFit.mH%s.root -c 1 -s 1 -w 3 -n "Stat + Syst" -f %s/Specials/higgsCombine7and8TeVMuStat.MultiDimFit.mH%s.root -c 4 -s 2 -w 3 -n "Stat Only" --text="%s" --mu'%(folder,mh,folder,mh,lumistring))   
      os.system('mv r.pdf plots/%s_muNLL.pdf'%(folder))
      os.system('mv r.png plots/%s_muNLL.png'%(folder))
      os.system('mv r.C plots/%s_muNLL.C'%(folder))
    if model=="MuMH":
      if not options.skipHadd:
        os.system('hadd -f %s/Specials/higgsCombine%sMuMHScan.MultiDimFit.mH%s.root %s/Specials/higgsCombine%sMuMHScan_*.MultiDimFit.mH%s.root'%(folder,folder,mh,folder,folder,mh))
        os.system('./makeCombinePlots.py -f %s/Specials/higgsCombine%sMuMHScan.MultiDimFit.mH%s.root --mumh --text="%s"'%(folder,folder,mh,lumistring))
        os.system('mv MuMH_col.pdf plots/%s_MuMH_col.pdf'%(folder))
        os.system('mv MuMH_col.png plots/%s_MuMH_col.png'%(folder))
        os.system('mv MuMH_col.C plots/%s_MuMH_col.C'%(folder))
        os.system('mv MuMH.pdf plots/%s_MuMH.pdf'%(folder))
        os.system('mv MuMH.png plots/%s_MuMH.png'%(folder))
        os.system('mv MuMH.C plots/%s_MuMH.C'%(folder))
    if model=="RvRf":
      if not options.skipHadd:
        os.system('hadd -f %s/Specials/higgsCombine%sRvRfScan.MultiDimFit.mH%s.root %s/Specials/higgsCombine%sRvRfScan_*.MultiDimFit.mH%s.root'%(folder,folder,mh,folder,folder,mh))
        os.system('./makeCombinePlots.py -f %s/Specials/higgsCombine%sRvRfScan.MultiDimFit.mH%s.root --rvrf --text="%s"'%(folder,folder,mh,lumistring))
        os.system('mv RvRf_col.pdf plots/%s_RvRf_col.pdf'%(folder))
        os.system('mv RvRf_col.png plots/%s_RvRf_col.png'%(folder))
        os.system('mv RvRf_col.C plots/%s_RvRf_col.C'%(folder))
        os.system('mv RvRf.pdf plots/%s_RvRf.pdf'%(folder))
        os.system('mv RvRf.png plots/%s_RvRf.png'%(folder))
        os.system('mv RvRf.C plots/%s_RvRf.C'%(folder))

