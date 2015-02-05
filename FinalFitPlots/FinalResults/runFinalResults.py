#!/usr/bin/env python

# should have a working folder containing two datacards.txt files one for 2011 and one for 2012
# you can make this folder and then create a soft link to this script and to the two sub scripts:
#   ln -s ../testDoAllScript.py
#   ln -s ../subParametricToBatch.py
#   ln -s ../subBinnedToBatch.py
# the only other files should be the relevant workspaces pointed to by these datacards
# should be in a cmssw environment with HiggsAnalysis/CombinedLimit compiled

import os
import sys
import numpy

from optparse import OptionParser
parser = OptionParser()
parser.add_option("","--card2011",dest="card2011",type="string",help="Name of 2011 card")
parser.add_option("","--card2012",dest="card2012",type="string",help="Name of 2012 card")
parser.add_option("-p","--parametric",dest="parametric",default=False,action="store_true",help="Run parametric signal model (binned signal model is the default)")
parser.add_option("","--unblind",dest="unblind",default=False,action="store_true",help="Unblind")
parser.add_option("","--dryRun",dest="dryRun",default=False,action="store_true",help="Don't actually submit")
parser.add_option("","--mcMasses",dest="mcMasses",default=False,action="store_true",help="Only run MC masses")
parser.add_option("-L","--mHlow",dest="mHlow",type="int",default=110,help="Lowest MH")
parser.add_option("-H","--mHhigh",dest="mHhigh",type="int",default=150,help="Highest MH")
parser.add_option("-S","--mHstep",dest="mHstep",type="float",default=1,help="MH step")
parser.add_option("","--run2011",dest="run2011",default=False,action="store_true",help="Run only 2011")
parser.add_option("","--run2012",dest="run2012",default=False,action="store_true",help="Run only 2012")
parser.add_option("","--runBoth",dest="runBoth",default=False,action="store_true",help="Run only 2011+2012")
parser.add_option("","--makePlots",dest="makePlots",default=False,action="store_true",help="Don't submit jobs but make plots from already completed jobs")
parser.add_option("","--lumi2011",dest="lumi2011",type="float",default=5.3,help="2011 luminosity")
parser.add_option("","--lumi2012",dest="lumi2012",type="float",default=19.6,help="2012 luminosity")
(options,args)=parser.parse_args()

path = os.getcwd();

# if a specific year has not been set run everything
if not options.run2011 and not options.run2012 and not options.runBoth:
  options.run2011=True
  options.run2012=True
  options.runBoth=True
  
if options.parametric:
  combinedCard = 'datacard_parametric_7and8TeV.txt'
else:
  combinedCard = 'datacard_binned_7and8TeV.txt'

config=[]
if options.run2011:
  config.append([options.card2011,'7TeV','#sqrt{s}=7TeV L=%3.1ffb^{-1}'%(options.lumi2011)])
if options.run2012:
  config.append([options.card2012,'8TeV','#sqrt{s}=8TeV L=%3.1ffb^{-1}'%(options.lumi2012)])
if options.runBoth:
  config.append([combinedCard,'7and8TeV','#splitline{#sqrt{s}=7TeV L=%3.1ffb^{-1}}{#sqrt{s}=8TeV L=%3.1ffb^{-1}}'%(options.lumi2011,options.lumi2012)])
    
if not options.makePlots:

  # make combined datacard
  if options.runBoth:
    os.system("combineCards.py %s %s > %s"%(options.card2011,options.card2012,combinedCard)) 

  # unbinned signal model
  if options.parametric:
    for card, suffix, line in config:
      command = './subParametricToBatch.py -p %s -d %s -D %s -q 1nh -L %d -H %d -S %3.1f'%(path,card,suffix,options.mHlow,options.mHhigh,options.mHstep)
      if options.dryRun: command += " --dryRun"
      os.system(command+' -M ExpProfileLikelihood --expectSignal=1')
      #os.system(command+' -M ExpProfileLikelihood --expectSignal=1 --expectSignalMass=125')
      if options.unblind:
        os.system(command+' -M Asymptotic')
        os.system(command+' -M ProfileLikelihood')
        os.system(command+' -M MaxLikelihoodFit')
      else:
        os.system(command+' -M Asymptotic -e')
    
  # binned signal model
  else:
    for card, suffix, line in config:
      command = "./subBinnedToBatch.py -p %s -d %s -D %s -q 8nh -L %d -H %d -S %3.1f"%(path,card,suffix,options.mHlow,options.mHhigh,options.mHstep)
      if options.unblind: command += " --unblind"
      if options.mcMasses: command += " --mcMasses"
      if options.dryRun: command += " --dryRun"
      os.system(command) 

# make plots
else:
  # check jobs are complete
  job_dirs = ['Asymptotic','ExpProfileLikelihood']
  #if options.parametric: job_dirs.append('ExpProfileLikelihood_m125')
  if options.unblind: 
    job_dirs.append('ProfileLikelihood')
    job_dirs.append('MaxLikelihoodFit')

  failed_jobs=[]
  for card, suffix, line in config:
    if options.parametric:
      for dir in job_dirs:
        for mh in numpy.arange(options.mHlow,options.mHhigh+options.mHstep,options.mHstep):
          if not os.path.isfile('%s/%s/%s%5.1f.sh.done'%(suffix,dir,dir,mh)):
            failed_jobs.append('%s/%s/%s%5.1f.sh'%(suffix,dir,dir,mh))
    else:
      for dir in job_dirs:
        if not os.path.isfile('%s/%s/sub.sh.done'%(suffix,dir)):
          failed_jobs.append('%s/%s/sub.sh'%(suffix,dir))

  if len(failed_jobs)>0:
    print 'The following jobs have failed:'
    print '\t', failed_jobs
    a = raw_input('Do you want to resubmit them? [y]/[n]\n')
    if a=='y' or a=='Y' or a=='yes':
      for job in failed_jobs:
        os.system("rm -f %s.done"%job)
        os.system("rm -f %s.fail"%job)
        os.system("rm -f %s.run"%job)
        os.system("rm -f %s.log"%job)
        if options.parametric: os.system('bsub -q 1nh -o %s/%s.log %s/%s'%(os.getcwd(),job,os.getcwd(),job))
        else: os.system('bsub -q 8nh -o %s/%s.log %s/%s'%(os.getwcd(),job,os.getcwd(),job))
      sys.exit()
    else:
      b = raw_input('There are failed jobs that haven\'t been resubmited. Do you want to continue? [y]/[n]\n')
      if b=='n' or b=='N' or b=='no':
        sys.exit()

  result_dir = os.getcwd()
  macros_dir = os.getcwd().split('Macros')[0]+'Macros'
  os.chdir(macros_dir)
  os.system("mkdir -p %s/plots"%(result_dir))

  for card, suffix, line in config:
    print "-------------------------"
    print "------ Running", suffix, "-----"
    print "-------------------------"

    # pvalues
    print "Running exppval..."
    os.system("python limitPlotter.py -M ProfileLikelihood --pval -p %s/%s/ExpProfileLikelihood -o %s/%s/exppval.root"%(result_dir,suffix,result_dir,suffix))
    #if options.parametric: 
      #print "Running exppval_m125..."
      #os.system("python limitPlotter.py -M ProfileLikelihood --pval -p %s/%s/ExpProfileLikelihood_m125 -o %s/%s/exppval_m125.root"%(result_dir,suffix,result_dir,suffix))
    pval_command = "python limitPlotter.py -M ProfileLikelihood --nogrid --pval --addline=\"%s/%s/exppval.root:1:2:Expected 1xSM\" --addtxt=\"%s\""%(result_dir,suffix,line)
    #if options.parametric: pval_command += " --addline=\"%s/%s/exppval_m125.root:4:2:Expected 1xSM m_{H}=125\""%(result_dir,suffix)
    if options.unblind: pval_command += " -p %s/%s/ProfileLikelihood -o %s/%s/obspval.root"%(result_dir,suffix,result_dir,suffix)
    else: pval_command += " -p %s/%s/ExpProfileLikelihood -e"%(result_dir,suffix)
    print "Running pval plot..."
    os.system(pval_command)
    #print '\t', pval_command
    if options.unblind:
      os.system("mv pvaluesplot.pdf %s/plots/%s_obspval.pdf"%(result_dir,suffix))
      os.system("mv pvaluesplot.png %s/plots/%s_obspval.png"%(result_dir,suffix))
    else:
      os.system("mv pvaluesplot.pdf %s/plots/%s_exppval.pdf"%(result_dir,suffix))
      os.system("mv pvaluesplot.png %s/plots/%s_exppval.png"%(result_dir,suffix))
    
    # limits
    limit_command = "python limitPlotter.py -M Asymptotic -r -p %s/%s/Asymptotic --addtxt=\"%s\""%(result_dir,suffix,line)
    if not options.unblind: limit_command += " -e" 
    print "Running limit plot..."
    os.system(limit_command)
    if options.unblind:
      os.system("mv limit_Asymptotic_ratio.pdf %s/plots/%s_obslimit.pdf"%(result_dir,suffix))
      os.system("mv limit_Asymptotic_ratio.png %s/plots/%s_obslimit.png"%(result_dir,suffix))
    else:
      os.system("mv limit_Asymptotic_ratio.pdf %s/plots/%s_explimit.pdf"%(result_dir,suffix))
      os.system("mv limit_Asymptotic_ratio.png %s/plots/%s_explimit.png"%(result_dir,suffix))
    
    # best fit mu
    if options.unblind:
      print "Running maxlh plot..."
      os.system("python limitPlotter.py -M MaxLikelihoodFit -p %s/%s/MaxLikelihoodFit --addtxt=\"%s\""%(result_dir,suffix,line))
      os.system("mv maxlhplot.pdf %s/plots/%s_maxlh.pdf"%(result_dir,suffix)) 
      os.system("mv maxlhplot.png %s/plots/%s_maxlh.png"%(result_dir,suffix)) 
  
  if options.runBoth and options.run2011 and options.run2012:
    # make a special pvalue plot
    special_command = "python limitPlotter.py -M ProfileLikelihood --nogrid --pval -p %s/7and8TeV/ExpProfileLikelihood -e --addline=\"%s/7TeV/exppval.root:2:2:Expected 1xSM (7TeV)\" --addline=\"%s/8TeV/exppval.root:4:2:Expected 1xSM (8TeV)\" --addline=\"%s/7and8TeV/exppval.root:1:2:Expected 1xSM (7+8TeV)\" --addtxt=\"#splitline{#sqrt{s}=7TeV L=%3.1ffb^{-1}}{#sqrt{s}=8TeV L=%3.1ffb^{-1}}\""%(result_dir,result_dir,result_dir,result_dir,options.lumi2011,options.lumi2012)
    if options.unblind:
      special_command += " --addline=\"%s/7TeV/obspval.root:2:1:Observed (7TeV)\" --addline=\"%s/8TeV/obspval.root:4:1:Observed (8TeV)\" --addline=\"%s/7and8TeV/obspval.root:1:1:Observed (7+8TeV)\""%(result_dir,result_dir,result_dir)
    os.system(special_command)
    #print '\t', special_command
    if options.unblind:
      os.system("mv pvaluesplot.pdf %s/plots/obspval.pdf"%(result_dir))
      os.system("mv pvaluesplot.png %s/plots/obspval.png"%(result_dir))
    else:
      os.system("mv pvaluesplot.pdf %s/plots/exppval.pdf"%(result_dir))
      os.system("mv pvaluesplot.png %s/plots/exppval.png"%(result_dir))
