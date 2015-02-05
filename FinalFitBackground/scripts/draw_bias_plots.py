#!/usr/bin/env python

import sys
import os

from optparse import OptionParser
parser=OptionParser()
parser.add_option("-d","--datfile")
parser.add_option("-i","--infile")
parser.add_option("-o","--outdir")
parser.add_option("-c","--cat",type="int")
parser.add_option("--expectSignal",type="float",default=0.)
parser.add_option("--is2011",default=False,action="store_true")
parser.add_option("--skipANlines",default=False,action="store_true")
parser.add_option("--remakeHists",default=False,action="store_true")
parser.add_option("-b","--batch",default=False,action="store_true")
parser.add_option("-m","--meanHistOpts",default="30,-3.,3.")
parser.add_option("-e","--errHistOpts",default="50,0.5,1.")
parser.add_option("-y","--biasYvals",default="-0.25,0.25")
parser.add_option("-Y","--pullYvals",default="-0.25,0.25")
parser.add_option("-L","--legLeft",default=False,action="store_true")

(options,args)=parser.parse_args()

import ROOT as r

if options.remakeHists:
  hist_file = options.infile.replace('.root','Hists.root')
  r.gROOT.ProcessLine('.L scripts/make_bias_hists.C+g')
  from ROOT import make_bias_hists
  meanOpts = options.meanHistOpts.split(',')
  errOpts = options.errHistOpts.split(',')
  make_bias_hists(options.infile,hist_file,options.cat,options.expectSignal,int(meanOpts[0]),float(meanOpts[1]),float(meanOpts[2]),int(errOpts[0]),float(errOpts[1]),float(errOpts[2]))
  options.infile = hist_file

if options.batch: r.gROOT.SetBatch()
r.gStyle.SetOptStat(0)

fabDict = {}
if options.is2011:
  # cat 0
  fabDict['truth_exp1_cat0'] = 0.03
  fabDict['truth_pow1_cat0'] = 0.11
  fabDict['truth_pol3_cat0'] = 0.03
  fabDict['truth_lau1_cat0'] = 0.11
  # cat 1
  fabDict['truth_exp3_cat1'] = -0.04
  fabDict['truth_pow1_cat1'] = 0.09
  fabDict['truth_pol3_cat1'] = 0.04
  fabDict['truth_lau1_cat1'] = 0.09
  # cat 2
  fabDict['truth_exp3_cat2'] = 0.11
  fabDict['truth_pow1_cat2'] = 0.19
  fabDict['truth_pol3_cat2'] = 0.09
  fabDict['truth_lau1_cat2'] = 0.20
  # cat 3
  fabDict['truth_exp3_cat3'] = 0.42
  fabDict['truth_pow1_cat3'] = 0.29
  fabDict['truth_pol4_cat3'] = 0.17
  fabDict['truth_lau1_cat3'] = 0.30
else:
  # cat 0
  fabDict['truth_exp1_cat0'] = 0.03
  fabDict['truth_pow1_cat0'] = 0.11
  fabDict['truth_pol3_cat0'] = 0.03
  fabDict['truth_lau1_cat0'] = 0.11
  # cat 1
  fabDict['truth_exp3_cat1'] = -0.04
  fabDict['truth_pow1_cat1'] = 0.09
  fabDict['truth_pol3_cat1'] = 0.04
  fabDict['truth_lau1_cat1'] = 0.09
  # cat 2
  fabDict['truth_exp3_cat2'] = 0.11
  fabDict['truth_pow1_cat2'] = 0.19
  fabDict['truth_pol3_cat2'] = 0.09
  fabDict['truth_lau1_cat2'] = 0.20
  # cat 3
  fabDict['truth_exp3_cat3'] = 0.42
  fabDict['truth_pow1_cat3'] = 0.29
  fabDict['truth_pol4_cat3'] = 0.17
  fabDict['truth_lau1_cat3'] = 0.30

def drawHists():
  file = r.TFile.Open(options.infile)

  os.system('mkdir -p %s'%options.outdir)
  truth_mods=[]

  for key in file.GetListOfKeys():
    name = key.GetName()
    truth = name.split('_mu')[0]
    if truth not in truth_mods: truth_mods.append(truth)

  print truth_mods

  muHistDummy = r.TH1F('muHistDummy','',len(truth_mods),0,len(truth_mods))
  l = r.TF1('l','0.',0.,len(truth_mods))
  l.SetLineColor(r.kBlack)
  l.SetLineWidth(2)
  l.SetLineStyle(7)
  muBiasBand = r.TGraphErrors()
  muBiasBand.SetLineColor(r.kGray)
  muBiasBand.SetFillColor(r.kGray)
  muBiasBand.SetFillStyle(1001)
  muBiasBand.SetMarkerColor(r.kGray)
  muPullBand = r.TGraphErrors()
  muPullBand.SetLineColor(r.kGray)
  muPullBand.SetFillColor(r.kGray)
  muPullBand.SetFillStyle(1001)
  muPullBand.SetMarkerColor(r.kGray)
  muBiasFabNote = r.TGraph()

  muBiasFabNote.SetLineColor(r.kBlue)
  muBiasFabNote.SetLineWidth(2)
  muBiasFabNote.SetLineStyle(2)

  graphDict={}

  scanTypes=['Fab','Paul','Chi2','AIC']
  graphCol=[r.kBlue,r.kRed,r.kGreen+1,r.kMagenta]
  label=['Hgg Nominal Pol','Envelope (no pen)','Envelope (1/dof pen)','Envelope (2/dof pen)']
  plotTypes=['Mean','RMS','Bias','Pull','MeanProfile','PullProfile']

  for c, stype in enumerate(scanTypes):
    graphDict[stype] = {}
    for ptype in plotTypes:
      graphDict[stype][ptype] = r.TGraphErrors()
      graphDict[stype][ptype].SetLineColor(graphCol[c])
      graphDict[stype][ptype].SetMarkerColor(graphCol[c])
      graphDict[stype][ptype].SetLineWidth(2)
      graphDict[stype][ptype].SetMarkerStyle(r.kFullCircle)
      graphDict[stype][ptype].SetTitle('Category %d - %s'%(options.cat,ptype))

  canv = r.TCanvas()

  for type in ['','Err','Pull']:
    for i, truth in enumerate(truth_mods):
      name = truth.split('_cat')[0]
      max=0.
      leg = r.TLegend(0.6,0.7,0.89,0.89)
      if options.legLeft:
        leg.SetX1(0.11)
        leg.SetX2(0.4)
      leg.SetLineColor(0)
      leg.SetFillColor(0)
      for c, stype in enumerate(scanTypes):
        hist = file.Get('%s_mu%s%s'%(truth,stype,type))
        max = r.TMath.Max(hist.GetMaximum(),max)
      
      for c, stype in enumerate(scanTypes):
        hist = file.Get('%s_mu%s%s'%(truth,stype,type))
        if options.is2011:
          hist.SetTitle('2011 Category %d - Truth %s'%(options.cat,name))
        else:
          hist.SetTitle('2012 Category %d - Truth %s'%(options.cat,name))
        hist.SetLineColor(graphCol[c])
        hist.SetLineWidth(2)
        hist.GetYaxis().SetRangeUser(0.,max*1.1)
        leg.AddEntry(hist,label[c],'L')
        if c==0: hist.Draw()
        else: hist.Draw("same")
      leg.Draw("same")
      canv.Print('%s/%s%s.pdf'%(options.outdir,name,type))
      canv.Print('%s/%s%s.png'%(options.outdir,name,type))

      if type=='':
        muHistDummy.GetXaxis().SetBinLabel(i+1,name)
        muBiasBand.SetPoint(i,i,0.)
        muBiasBand.SetPointError(i,1.,0.2)
        muPullBand.SetPoint(i,i,0.)
        muPullBand.SetPointError(i,1.,0.1)
        if not options.skipANlines: muBiasFabNote.SetPoint(i,0.5+i,fabDict['truth_%s_cat%d'%(name,options.cat)])
        
        for c, stype in enumerate(scanTypes):
          hist = file.Get('%s_mu%s%s'%(truth,stype,type))
          graphDict[stype]['Mean'].SetPoint(i,0.5+i,hist.GetMean())
          graphDict[stype]['Mean'].SetPointError(i,0.,hist.GetMeanError())
          graphDict[stype]['RMS'].SetPoint(i,0.5+i,hist.GetRMS())
          graphDict[stype]['RMS'].SetPointError(i,0.,hist.GetRMSError())
          graphDict[stype]['MeanProfile'].SetPoint(i,0.5+i,hist.GetMean())
          graphDict[stype]['MeanProfile'].SetPointError(i,0.,hist.GetRMS())
          graphDict[stype]['Bias'].SetPoint(i,0.5+i,(hist.GetMean()-options.expectSignal)/hist.GetRMS())
          graphDict[stype]['Bias'].SetPointError(i,0.,hist.GetMeanError()/hist.GetRMS())

      if type=='Pull':
        for c, stype in enumerate(scanTypes):
          hist = file.Get('%s_mu%s%s'%(truth,stype,type))
          graphDict[stype]['Pull'].SetPoint(i,0.5+i,hist.GetMean())
          graphDict[stype]['Pull'].SetPointError(i,0.,hist.GetMeanError())
          graphDict[stype]['PullProfile'].SetPoint(i,0.5+i,hist.GetMean())
          graphDict[stype]['PullProfile'].SetPointError(i,0.,hist.GetRMS())


  if options.is2011:
    muHistDummy.SetTitle("2011 Category %d - #mu_{gen}=%3.1f"%(options.cat,options.expectSignal))
  else:
    muHistDummy.SetTitle("2012 Category %d - #mu_{gen}=%3.1f"%(options.cat,options.expectSignal))
  muHistDummy.GetXaxis().SetTitle("Truth Model")
  muHistDummy.GetXaxis().SetLabelSize(0.06)
  muBiasBand.SetPoint(len(truth_mods),len(truth_mods),0.)
  muBiasBand.SetPointError(len(truth_mods),1.,0.2)
  muPullBand.SetPoint(len(truth_mods),len(truth_mods),0.)
  muPullBand.SetPointError(len(truth_mods),1.,0.1)

  # mu mean
  muHistDummy.GetYaxis().SetRangeUser(-0.2,0.2)
  muHistDummy.GetYaxis().SetTitle("#mu mean from toys")
  muHistDummy.Draw("")
  l.Draw("same")
  for stype in scanTypes:
    graphDict[stype]['Mean'].Draw("EPsame")
  leg.Draw("same")
  canv.Print('%s/mean.pdf'%options.outdir)
  canv.Print('%s/mean.png'%options.outdir)

  # mu rms
  muHistDummy.GetYaxis().SetRangeUser(float(options.errHistOpts.split(',')[1]),float(options.errHistOpts.split(',')[2]))
  muHistDummy.GetYaxis().SetTitle("#mu rms from toys")
  muHistDummy.Draw("")
  for stype in scanTypes:
    graphDict[stype]['RMS'].Draw("EPsame")
  leg.Draw("same")
  canv.Print('%s/rms.pdf'%options.outdir)
  canv.Print('%s/rms.png'%options.outdir)

  # mu profile (mean w/ rms err)
  canv = r.TCanvas()
  muHistDummy.GetYaxis().SetRangeUser(-2.,2.)
  muHistDummy.GetYaxis().SetTitle("#mu profile (mean w/ rms err)")
  muHistDummy.Draw("")
  l.Draw("same")
  for stype in scanTypes:
    graphDict[stype]['MeanProfile'].Draw("EPsame")
  leg.Draw("same")
  canv.Print('%s/meanprof.pdf'%options.outdir)
  canv.Print('%s/meanprof.png'%options.outdir)

  legPull = leg.Clone()
  # mu bias 
  muHistDummy.GetYaxis().SetRangeUser(float(options.biasYvals.split(',')[0]),float(options.biasYvals.split(',')[1]))
  muHistDummy.GetYaxis().SetTitle("#mu bias (syst/stat)")
  if not options.skipANlines: leg.AddEntry(muBiasFabNote,"Fabian from AN","L")
  leg.AddEntry(muBiasBand,"20% syst/stat band","F")
  muHistDummy.Draw("")
  muBiasBand.Draw("E3same")
  muHistDummy.Draw("AXISsame")
  l.Draw("same")
  if not options.skipANlines: muBiasFabNote.Draw("Lsame")
  for stype in scanTypes:
    graphDict[stype]['Bias'].Draw("EPsame")
    #if stype=='Fab': graphDict[stype]['Bias'].Draw("EPsame")
  leg.Draw("same")
  canv.Print('%s/bias.pdf'%options.outdir)
  canv.Print('%s/bias.png'%options.outdir)

  # mu pull
  muHistDummy.GetYaxis().SetRangeUser(float(options.pullYvals.split(',')[0]),float(options.pullYvals.split(',')[1]))
  muHistDummy.GetYaxis().SetTitle("#mu pull (fit-gen)/err")
  legPull.AddEntry(muPullBand,"10% pull band","F")
  muHistDummy.Draw("")
  muPullBand.Draw("E3same")
  muHistDummy.Draw("AXISsame")
  l.Draw("same")
  for stype in scanTypes:
    graphDict[stype]['Pull'].Draw("EPsame")
    #if stype=='Fab': graphDict[stype]['Pull'].Draw("EPsame")
  legPull.Draw("same")
  canv.Print('%s/pull.pdf'%options.outdir)
  canv.Print('%s/pull.png'%options.outdir)

  # mu pull profile
  muHistDummy.GetYaxis().SetRangeUser(-2.,2.)
  muHistDummy.GetYaxis().SetTitle("#mu pull (fit-gen)/err")
  muHistDummy.Draw("")
  muPullBand.Draw("E3same")
  muHistDummy.Draw("AXISsame")
  l.Draw("same")
  for stype in scanTypes:
    graphDict[stype]['PullProfile'].Draw("EPsame")
  legPull.Draw("same")
  canv.Print('%s/pullprof.pdf'%options.outdir)
  canv.Print('%s/pullprof.png'%options.outdir)

if not options.datfile:
  drawHists()
else:
  f = open(options.datfile)
  for line in f.readlines():
    if line.startswith('#') or line=='' or line =='\n': continue
    info = line.split()
    options.cat = int(info[0])
    options.outdir = info[1]
    options.infile = info[2]
    options.expectSignal = float(info[3])
    options.biasYvals = info[4]
    options.pullYvals = info[5]
    if info[6]=='L': options.legLeft=True
    drawHists()
  
