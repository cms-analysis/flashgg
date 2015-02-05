#!/usr/bin/env python

# HYBRID:   aicEnvelope_job0_hybrid_exp3_pol3_truth_job0_toy99_cat3_toy99
# STANDARD: aicEnvelope_job0_sb_truth_pow1_cat0_truth_job0_toy99_cat0_toy99 

import os
import sys
import fnmatch
import math
from optparse import OptionParser
parser=OptionParser()
parser.add_option("-D","--dir")
parser.add_option("-o","--outfile")
parser.add_option("-d","--datfile")
parser.add_option("--expectSignal",type="float",default=0.)
parser.add_option("--runSpecificFiles",type="str")
parser.add_option("--eosWalk",type="int",help="Walk eos for files - provide njobs")
(options,args)=parser.parse_args()

import ROOT as r

r.gSystem.Load('lib/libBackgroundProfileFitting.so')

profiler = r.ProfileMultiplePdfs()

def makeHists(cat=0,meanB=50,meanL=-4.,meanH=4.,errB=50,errL=0.5,errH=1.5,pullB=50,pullL=-5.,pullH=5.):

  dir = options.dir
  list_of_files=[]

  if '/store/' in dir and options.eosWalk:
    dir = dir.strip('~')
    if '/eos/cms' not in dir:
      dir = '/eos/cms'+dir
    dir = 'root://eoscms/'+dir
  
  if '/store/' in options.outfile and options.eosWalk:
    options.outfile = options.outfile.strip('~')
    if '/eos/cms' not in options.outfile:
      options.outfile = '/eos/cms'+options.outfile
    options.outfile = 'root://eoscms/'+options.outfile
  
  if options.eosWalk:
    for job in range(options.eosWalk):
      list_of_files.append('BiasStudyOut_cat%d_job%d.root'%(cat,job))
  else:
    for root, dirs, files in os.walk(dir):
      for filename in fnmatch.filter(files,'*job*.root'):
        list_of_files.append(filename)

  outfile = r.TFile.Open('%s'%options.outfile,'RECREATE')

  truth_models = set()

  TestFileFound = False
  test_file=0
  TFi = 0
  print  dir+'/'+list_of_files[TFi]
  while not TestFileFound:
    test_file = r.TFile.Open(dir+'/'+list_of_files[TFi])
    try :
	test_file.GetName()
	TestFileFound=True
    except:
	TFi+=1
	TestFileFound=False
  
  for key in test_file.GetListOfKeys():
    if 'truth' not in key.GetName(): continue
    if 'hybrid' in key.GetName():
      truth = 'hybrid'+key.GetName().split('_truth')[0].split('hybrid')[1].split('_cat')[0]
    elif 'smooth' in key.GetName():
      truth = 'smooth'+key.GetName().split('_truth')[0].split('smooth')[1]
    elif 'kpdf' in key.GetName():
      truth = 'kpdf'+key.GetName().split('_truth')[0].split('kpdf')[1]
    else:
      truth= key.GetName().split('truth')[1].split('_cat')[0][1:]
    truth_models.add(truth)

  print truth_models

  types = set()
  types.add('Fab')
  types.add('Paul')
  types.add('Chi2')
  types.add('AIC')

  util={}
  util['fab'] = 'Fab'
  util['paul'] = 'Paul'
  util['chi2'] = 'Chi2'
  util['aic'] = 'AIC'

  coverageValues=[0.1,0.5,1.,2.]

  histMap={}
  histErrMap={}
  histPullMap={}
  histPullTruthMap={}
  histTypeMap={}
  graphCovMap={}
  counterCovMap={}

  for type in types:
    histMap[type] = {}
    histErrMap[type] = {}
    histPullMap[type] = {}
    histPullTruthMap[type] = {}
    histTypeMap[type] = {}
    graphCovMap[type] = {}
    counterCovMap[type] = {}
    for mod in truth_models:
      histMap[type][mod] = r.TH1F('%s_mu%s'%(mod,type),'%s_mu%s'%(mod,type),meanB,meanL,meanH)
      histErrMap[type][mod] = r.TH1F('%s_mu%sErr'%(mod,type),'%s_mu%sErr'%(mod,type),errB,errL,errH)
      bWidth = (pullH-pullL)/pullB # so we can center at 0
      histPullMap[type][mod] = r.TH1F('%s_mu%sPull'%(mod,type),'%s_mu%sPull'%(mod,type),pullB,pullL-bWidth/2,pullH-bWidth/2)
      histPullTruthMap[type][mod] = {}
      for mod1 in truth_models:
	histPullTruthMap[type][mod][mod1] = r.TH1F('%s_mu%sPull_bfit_%s_only'%(mod,type,mod1),'%s_mu%sPull_bfit_%s_only'%(mod,type,mod1),pullB,pullL-bWidth/2,pullH-bWidth/2)
      histTypeMap[type][mod] = {}
      graphCovMap[type][mod] = []
      counterCovMap[type][mod] = []
      for c, cov in enumerate(coverageValues):
        graphCovMap[type][mod].append(r.TGraphErrors())
        graphCovMap[type][mod][c].SetName('%s_mu%sCov%3.1f'%(mod,type,cov))
        counterCovMap[type][mod].append([0,0])

  print dir
  for i, f in enumerate(list_of_files):
    print '\tJob', i+1,'/',len(list_of_files), '\r',
    sys.stdout.flush()
    file = r.TFile.Open(dir+'/'+f)
    try :
	file.GetName()
    except:
	continue
    for key in file.GetListOfKeys():
      graph = key.ReadObj()
      if 'Envelope' not in graph.GetName(): continue
      if 'hybrid' in key.GetName():
        truth = 'hybrid'+key.GetName().split('_truth')[0].split('hybrid')[1].split('_cat')[0] 
      elif 'smooth' in key.GetName():
        truth = 'smooth'+key.GetName().split('_truth')[0].split('smooth')[1]
      elif 'kpdf' in key.GetName():
        truth = 'kpdf'+key.GetName().split('_truth')[0].split('kpdf')[1]
      else:
        truth = graph.GetName().split('truth')[1].split('_cat')[0][1:]
      type = graph.GetName().split('Envelope')[0]
      mytype = util[type]
     
        
      #muInfo = profiler.getMinAndErrorAsymmVec(graph,1.)
      muInfo = profiler.getMinAndErrorLinearAsymmVec(graph,1.)
      muVal = muInfo.at(0)
      err_low = muInfo.at(1)
      err_high  = muInfo.at(2)

      # symmetric error
      if abs(err_low)<99 and abs(err_high) <99 : sym_err = (err_low+err_high)/2.
      elif abs(err_low)<99 : sym_err = err_low
      elif abs(err_high)<99 : sym_err = err_high
      else: sym_err = 9999

      #if  sym_err <0.01 or ( abs(muVal-options.expectSignal) > 2*sym_err):
      pull = profiler.getPull(graph,options.expectSignal) ## need to fit best fit and truth for this
      if math.isnan(pull) : continue
      """
      if abs(pull)<0.25 :# doesnt work so well
      #else: taken from L.Lyons but "seems odd" since distribution of mu should follow assymmetry of LH curve
      if muVal<options.expectSignal : err = err_high 
      else : err =  err_low
      if err > 0: pull = (muVal-options.expectSignal)/err
      else: pull=0
      """
	
      if muVal>=99. or sym_err>=99.: continue# or abs(err_low)>99. or abs(err_high)>99.: continue

      # first find which pdf gave the best fit
      bfname = graph.GetTitle()
      family = ""
      for fam in truth_models: 
	if fam in bfname : family = fam

      if bfname in histTypeMap[mytype][truth].keys():
	histTypeMap[mytype][truth][bfname]+=1
      elif bfname:
	histTypeMap[mytype][truth][bfname]=1

      histMap[mytype][truth].Fill(muVal)
      histErrMap[mytype][truth].Fill(sym_err)
      histPullMap[mytype][truth].Fill(pull)
      if family!="": histPullTruthMap[mytype][truth][family].Fill(pull)
      
      for c, cov in enumerate(coverageValues):
        counterCovMap[mytype][truth][c][1] += 1
        if r.TMath.Abs(pull)<cov:
          counterCovMap[mytype][truth][c][0] += 1
  
  print '\n',
  outfile.cd()

  for type, item in histMap.items():
    for truth, hist in item.items():
      hist.Write()
  for type, item in histErrMap.items():
    for truth, hist in item.items():
      hist.Write()
  for type, item in histPullMap.items():
    for truth, hist in item.items():
      hist.Write()
  for type, item in histPullTruthMap.items():
    for truth, hists in item.items():
	for tr,hist in hists.items():
          hist.Write()
  for type, item in histTypeMap.items():
    for truth, hist in item.items():
      ntypes = len(hist.keys())
      htmp = r.TH1I('%s_mu%sType'%(truth,type),'%s_mu%sType'%(truth,type),ntypes,0,ntypes)
      pdfbin = 0
      for pdf in hist.keys(): 
	htmp.SetBinContent(pdfbin+1,hist[pdf])
	htmp.SetBinError(pdfbin+1,hist[pdf]**0.5)
	htmp.GetXaxis().SetBinLabel(pdfbin+1,pdf)
	pdfbin+=1
      htmp.Write()
     
  for type, item in counterCovMap.items():
    for truth, covArray in item.items():
      for c, covCounter in enumerate(covArray):
        nPass = float(covCounter[0])
        nTotal = float(covCounter[1])
	if nTotal!=0:        covVal = nPass/nTotal
	else: covVal = 0
        if nTotal!=0: covErr = r.TMath.Sqrt((covVal*(1.-covVal))/nTotal)
	else :covErr = 0
        graphCovMap[type][truth][c].SetPoint(0,options.expectSignal,covVal)
        graphCovMap[type][truth][c].SetPointError(0,0.5,covErr)
        graphCovMap[type][truth][c].Write()
  outfile.Close()

def countLines(filename):
  f = open(filename)
  count=0
  for line in f.readlines():
    if line.startswith('#') or line=='' or line =='\n': continue
    else: count+=1
  f.close()
  return count

# --main-- here
if not options.datfile:
  makeHists()
else:
  nfiles = countLines(options.datfile)
  f = open(options.datfile)
  sw = r.TStopwatch()
  list_of_specifics=[]
  if options.runSpecificFiles:
    for spec in options.runSpecificFiles.split(','):
      list_of_specifics.append(int(spec))
  i=0
  for line in f.readlines():
    if line.startswith('#') or line=='' or line =='\n': continue
    if options.runSpecificFiles and i not in list_of_specifics: 
      i+=1
      continue 
    info = line.split()
    cat = int(info[0])
    options.dir = info[1]
    options.outfile = info[2]
    options.expectSignal = float(info[3])
    sw.Reset()
    sw.Start()
    print 'Running file', i, 'of', nfiles
    makeHists(cat,int(info[4]),float(info[5]),float(info[6]),int(info[7]),float(info[8]),float(info[9]),int(info[10]),float(info[11]),float(info[12]))
    sw.Stop()
    print 'Took:', sw.Print()
    i+=1
  f.close()
    
