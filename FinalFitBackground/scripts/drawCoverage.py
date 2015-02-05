#!/usr/bin/env python

import sys
import os

from optparse import OptionParser
parser=OptionParser()
parser.add_option("-y","--year")
parser.add_option("-c","--cat",type="int",default=0)
parser.add_option("-v","--expVals",default=[],action="append")
parser.add_option("-f","--files",default=[],action="append")
parser.add_option("-I","--injectVal",type="float")
parser.add_option("-D","--outDir")
parser.add_option("-d","--datfile")
parser.add_option("-m","--runMasses",default=False,action="store_true",help="Default is to run mus")
(options,args)=parser.parse_args()

coverageValues=[0.1,0.5,1.,2.]

import ROOT as r
outf = r.TFile('CoverageFinalPlots.root','RECREATE')
def makePullPlot(h,ext,truth,usefit):
	r.gROOT.SetBatch(1)
	c=r.TCanvas("c","c",600,600)
	r.gStyle.SetOptFit(1111)
	r.gStyle.SetOptStat(1)
	h.SetMarkerStyle(20)
	h.Sumw2()
	h.SetMarkerSize(0.8)
	h.Draw("p")
	name = h.GetName()
	lLow = r.TLine(-0.14,0,-0.14,h.GetMaximum())
	lLow.SetLineColor(4)
	lLow.SetLineWidth(4)
	lHigh = r.TLine(0.14,0,0.14,h.GetMaximum())
	lHigh.SetLineColor(4)
	lHigh.SetLineWidth(4)
	lLow.Draw()
	lHigh.Draw()
	# Line of the median from Gaus
	if usefit: 
		med = h.GetFunction("gaus").GetParameter(1)
		up  = h.GetFunction("gaus").Eval(med)
		l = r.TLine(med,0,med,up)
		l.SetLineColor(2)
		l.SetLineStyle(2)
		l.SetLineWidth(2)
		l.Draw()
	# Line of the mean which is used
	med2 = h.GetMean()
	up2  = h.GetFunction("gaus").Eval(med2)
	l2 = r.TArrow(med2,up2,med2,0)
	l2.SetLineColor(1)
	l2.SetLineStyle(1)
	l2.SetLineWidth(2)
	l2.Draw()
	
        h.SetTitle("")
	h.GetXaxis().SetTitle("pull")
	h.GetYaxis().SetTitle("# Toys")
	h.GetYaxis().SetTitleOffset(1.2)
	# Finally also add the coverage of the 1sigma interval 
	lat = r.TLatex()
	lat.SetNDC()
	lat.SetTextFont(42)
	lat.SetTextSize(0.04)
	coverage=0.0;
	
	if usefit:
	  if h.GetFunction("gaus").GetProb() >=0.5:
	   tfunc= h.GetFunction("gaus")
	   coverage=float(tfunc.Integral(-1,1))/tfunc.Integral(h.GetBinLowEdge(1),h.GetBinLowEdge(h.GetNbinsX()+1))
	   lat.DrawLatex(0.12,0.8,"Cov(1#sigma)=%.1f %%"%(100.*coverage))

        c.Print('%s/%s_%.1f_%.1f_%s_pull.pdf'%(options.outDir,ext,truth,options.injectVal,h.GetName()))
    	c.Print('%s/%s_%.1f_%.1f_%s_pull.png'%(options.outDir,ext,truth,options.injectVal,h.GetName()))

def makeDecompositionPulls(hlist,ext,truth):
	if len(hlist)<1: return
	print hlist
	tnames = []
	gmax=0
	for i,h in enumerate(hlist):	
	  hname = h.GetName()
	  tnames.append(hname[hname.find("_bfit_"):hname.find("_only_")]) # truth is before the first underscore
	  h.SetLineColor(i+1)		
	  if h.GetMaximum() > gmax: 
		gmax=h.GetMaximum()

	hlist[0].SetMaximum(gmax*1.2)
	c=r.TCanvas("cp","cp",600,600)
	r.gStyle.SetOptStat(0)
	l = r.TLegend(0.6,0.7,0.89,0.89)
	l.SetFillColor(0)

	lines = []
	for i,h in enumerate(hlist):
	  if i==0: h.Draw()
	  else: h.Draw("same")	
	  l.AddEntry(h,tnames[i],"L")
	  line = r.TLine(h.GetMean(),0,h.GetMean(),gmax*1.2)
	  line.SetLineColor(h.GetLineColor())
	  lines.append(line)
	for li in lines : li.Draw()
	l.Draw()
	overname=hlist[0].GetName() 
	savename=overname[0:overname.find("_bfit")]
	hlist[0].SetTitle(savename)	
        c.Print('%s/%s_%.1f_%.1f_%s_decomp.pdf'%(options.outDir,ext,truth,options.injectVal,savename))
        c.Print('%s/%s_%.1f_%.1f_%s_decomp.png'%(options.outDir,ext,truth,options.injectVal,savename))

def makeTypePlot(h,ext,truth):
	r.gROOT.SetBatch(1)
	r.gStyle.SetOptStat(0)
	c=r.TCanvas("c","c",600,600)
	h.GetYaxis().SetTitle("# Occurances")
	h.GetYaxis().SetTitleOffset(1.2)
	h.GetXaxis().SetLabelSize(0.09)

        for i in range(1,h.GetNbinsX()+1):
          lab = h.GetXaxis().GetBinLabel(i)
          lab = lab[len("sb_paul_"):]
          lab = lab[0:lab.find("_cat")]
          h.GetXaxis().SetBinLabel(i,lab)

	h.Draw("hist")
	h.SetLineWidth(2)
	h.SetLineColor(1)
	h.SetFillColor(r.kBlue-3)
        c.Print('%s/%s_%.1f_%.1f_%s_type.pdf'%(options.outDir,ext,truth,options.injectVal,h.GetName()))
    	c.Print('%s/%s_%.1f_%.1f_%s_type.png'%(options.outDir,ext,truth,options.injectVal,h.GetName()))
	

def makePlot():
  
  if options.runMasses:
    valerr = 1.0 
    valTitle = "m_{H} (GeV)"
    ext = 'mass_mu%.1f'%options.injectVal
    xlow = 105
    xhigh = 155
  else:
    valerr = 0.25
    valTitle = "#mu_{gen}"
    ext = 'mu_mass%.1f'%options.injectVal
    xlow = 0.
    xhigh = 2.

  os.system('mkdir -p %s'%options.outDir)

  r.gROOT.SetBatch()

  dummyHist = r.TH1F('dummy','',len(options.expVals),xlow,xhigh)
  dummyHist.GetXaxis().SetTitle(valTitle)
  dummyHist.SetStats(0)

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

  #scanTypes=['Fab','Paul','Chi2','AIC']
  scanTypes = ['Fab','Chi2']
  #scanTypes = ['Chi2']
  graphCol=[r.kRed,r.kBlue-3,r.kGreen+1,r.kMagenta]
  graphFil=[1001,1001]
  #graphFil=[3144,3190,3002,3002]
  #label=['Hgg Nominal Pol','Envelope (no pen)','Envelope (1/dof pen)','Envelope (2/dof pen)']
  label=['Bern',"Env"]
  plotTypes=['Coverage','Bias','Pull']
  #plotTypes=['Pull']

  valToFileDict={}
  for i, expM in enumerate(options.expVals):
    finame = options.files[i]
    if "/eos/cms/" in finame: 
      #options.files[i]=options.files[i].strip("/eos/cms/")
      finame =  "root://eoscms//"+(finame.lstrip("/"))

      valToFileDict[float(expM)] = finame
      options.files[i]=finame
    else: valToFileDict[float(expM)] = finame

  truth_mods=[]
  dummyIndex = 0
  FileIsOk = False
  dummyFile=0

  while not FileIsOk:
    if dummyIndex >= len(options.files): break
    print "Trying ",options.files[dummyIndex]
    dummyFile = r.TFile.Open(options.files[dummyIndex])
    try:
      kkeys = dummyFile.GetListOfKeys()
      if (len(kkeys))>0:FileIsOk=True
      else:
	dummyIndex+=1
	FileIsOk=False
    except:	
	dummyIndex+=1
        FileIsOk=False
	
	
  if not FileIsOk: return

  for key in dummyFile.GetListOfKeys():
    name = key.GetName()
    truth = name.split('_mu')[0]
    if truth not in truth_mods: truth_mods.append(truth)
  dummyFile.Close()

  print truth_mods

  canv = r.TCanvas()
  canv.SetGridy(1)
  canv.SetGridx(1)

  siglines=[]
  sigboxes=[]
  for i, sig in enumerate(coverageValues):
    coverage = 1.-r.TMath.Prob(sig*sig,1)
    siglines.append(r.TLine(xlow,coverage,xhigh,coverage))
    siglines[i].SetLineWidth(2)
    siglines[i].SetLineStyle(2)
    sigboxes.append(r.TPaveText(1.02*xlow,coverage+0.1,1.07*xhigh,coverage))
    sigboxes[i].SetFillColor(0)
    sigboxes[i].SetLineColor(0)
    sigboxes[i].SetTextSize(0.04)
    sigboxes[i].AddText('%3.1f#sigma'%(sig))

  for truth in truth_mods:
    # coverage
    graphDict={}
    legCov = r.TLegend(0.3,0.75,0.59,0.89)
    legCov.SetFillColor(0)
    legCov.SetLineColor(0)
    legCov2 = r.TLegend(0.6,0.75,0.89,0.89)
    legCov2.SetFillColor(0)
    legCov2.SetLineColor(0)
    leg = r.TLegend(0.3,0.75,0.59,0.89)
    leg.SetFillColor(0)
    leg.SetLineColor(0)
    leg2 = r.TLegend(0.6,0.75,0.89,0.89)
    leg2.SetFillColor(0)
    leg2.SetLineColor(0)
    for ptype in plotTypes:
      graphDict[ptype] = {}
      if ptype=='Coverage':
        for c, stype in enumerate(scanTypes):
          graphDict[ptype][stype] = []
          for v, cov in enumerate(coverageValues):
            graphDict[ptype][stype].append(r.TGraphErrors())
            graphDict[ptype][stype][v].SetLineColor(graphCol[c])
            graphDict[ptype][stype][v].SetMarkerColor(graphCol[c])
            graphDict[ptype][stype][v].SetMarkerStyle(21)
            graphDict[ptype][stype][v].SetMarkerSize(0.8)
            graphDict[ptype][stype][v].SetFillColor(graphCol[c])
            graphDict[ptype][stype][v].SetFillStyle(graphFil[c])
          if c<len(scanTypes)/2:
            leg.AddEntry(graphDict[ptype][stype][0],label[c],"F")
            legCov.AddEntry(graphDict[ptype][stype][0],label[c],"L")
          else:
            leg2.AddEntry(graphDict[ptype][stype][0],label[c],"F")
            legCov2.AddEntry(graphDict[ptype][stype][0],label[c],"L")
      else:
        for c, stype in enumerate(scanTypes):
          graphDict[ptype][stype] = r.TGraphErrors()
          graphDict[ptype][stype].SetLineColor(graphCol[c])
          graphDict[ptype][stype].SetMarkerColor(graphCol[c])
          graphDict[ptype][stype].SetFillColor(graphCol[c])
          graphDict[ptype][stype].SetLineColor(1)
          graphDict[ptype][stype].SetFillStyle(graphFil[c])
    
    val_arr = valToFileDict.keys()
    val_arr.sort()
    for p, val in enumerate(val_arr):
      f = r.TFile.Open(valToFileDict[val])
      for c, stype in enumerate(scanTypes):
	
        muBiasBand.SetPoint(p,val-5,0)
        muBiasBand.SetPointError(p,valerr,0.2)
        muPullBand.SetPoint(p,val-5,0)
        muPullBand.SetPointError(p,valerr,0.14)

        hist = f.Get('%s_mu%s'%(truth,stype))
	if type(hist)!=type(r.TH1F()): 
		continue
        if options.runMasses:
	  if hist.GetRMS()==0: rms = 999 
	  else: rms=hist.GetRMS()
          graphDict['Bias'][stype].SetPoint(p,val,(hist.GetMean()-options.injectVal)/rms)
          graphDict['Bias'][stype].SetPointError(p,0,hist.GetMeanError()/rms)
        else:
          graphDict['Bias'][stype].SetPoint(p,val,(hist.GetMean()-val)/hist.rms)
          graphDict['Bias'][stype].SetPointError(p,0,hist.GetMeanError()/hist.rms)

        histPull = f.Get('%s_mu%sPull'%(truth,stype))
	#histPull.Rebin(2)
	histPull.Fit("gaus","","Q",histPull.GetMean()-4,histPull.GetMean()+4)
        #Iterative Fit

        if histPull.GetFunction("gaus") :
	  for fi in range(3):
		newmean=histPull.GetFunction("gaus").GetParameter(1)
		histPull.Fit("gaus","","Q",newmean-4.,newmean+4.)
	  makePullPlot(histPull,ext,val,1)
	else: 
	   makePullPlot(histPull,ext,val,0)

        if histPull.GetFunction("gaus") :
	  if histPull.GetFunction("gaus").GetProb()<0.05:
            graphDict['Pull'][stype].SetPoint(p,val,histPull.GetMean())
            graphDict['Pull'][stype].SetPointError(p,0,histPull.GetMeanError())
	  else:
            graphDict['Pull'][stype].SetPoint(p,val,histPull.GetFunction("gaus").GetParameter(1))
            graphDict['Pull'][stype].SetPointError(p,0,histPull.GetFunction("gaus").GetParError(1))
	else:
            graphDict['Pull'][stype].SetPoint(p,val,histPull.GetMean())
            graphDict['Pull'][stype].SetPointError(p,0,histPull.GetMeanError())

	if stype!="Fab": 
	  tronlyHists = []
	  for tr_o in truth_mods:
	    tronlyHists.append(f.Get('%s_mu%sPull_bfit_%s_only'%(truth,stype,tr_o)))
	    #print tronlyHists
	  makeDecompositionPulls(tronlyHists,ext,val)
	  tronlyHists = []

        for v, cov in enumerate(coverageValues):
          graph = f.Get('%s_mu%sCov%3.1f'%(truth,stype,cov))
          x = r.Double(0.)
          y = r.Double(0.)
          graph.GetPoint(0,x,y)
          yerr = graph.GetErrorY(0)
	  graphDict['Coverage'][stype][v].SetPoint(p,val,y)
          graphDict['Coverage'][stype][v].SetPointError(p,valerr,yerr)

        histType = f.Get('%s_mu%sType'%(truth,stype))
	makeTypePlot(histType,ext,val)
    muBiasBand.SetPoint(len(options.expVals),155,0)
    muBiasBand.SetPointError(len(options.expVals),valerr,0.2)
    muPullBand.SetPoint(len(options.expVals),155,0)
    muPullBand.SetPointError(len(options.expVals),valerr,0.14)

    title=truth
    #title = title.replace('0sigma','0.10sigma')
    #title = title.replace('1sigma','0.25sigma')
    #title = title.replace('2sigma','0.50sigma')
    #title = title.replace('3sigma','1.00sigma')

    if options.runMasses:
      dummyHist.SetTitle('Category=%d  Truth=%s  #mu_{gen}=%3.1f'%(options.cat,title,options.injectVal))
    else:
      dummyHist.SetTitle('Category=%d  Truth=%s  m_{H}=%3.0f'%(options.cat,title,options.injectVal))
    dummyHist.GetYaxis().SetRangeUser(0.,1.3)
    dummyHist.GetYaxis().SetTitle("Coverage");
    dummyHist.Draw()
    for sigline in siglines: sigline.Draw("same")
    for sigbox in sigboxes: sigbox.Draw("same")
    for c, stype in enumerate(scanTypes):
      for v, cov in enumerate(coverageValues):
        graphDict['Coverage'][stype][v].Draw("Psame")
        if options.runMasses: graphDict['Coverage'][stype][v].SetName('%s_cat%d_mass_%s_%s_cov%3.1f_coverage'%(options.year,options.cat,truth,stype,cov))
        else: graphDict['Coverage'][stype][v].SetName('%s_cat%d_mu_%s_%s_cov%3.1f_coverage'%(options.year,options.cat,truth,stype,cov))
        outf.cd()
        graphDict['Coverage'][stype][v].Write()
    if len(scanTypes)>1 :leg.Draw("same")
    if len(scanTypes)>1: leg2.Draw("same")
    canv.Print('%s/%s_%s_coverage.pdf'%(options.outDir,ext,truth))
    canv.Print('%s/%s_%s_coverage.png'%(options.outDir,ext,truth))
    
    dummyHist.GetYaxis().SetRangeUser(-0.5,0.5)
    dummyHist.GetYaxis().SetTitle("#mu bias (syst/stat)")
    dummyHist.Draw()
    muBiasBand.Draw("E3same")
    dummyHist.Draw("AXISsame")
    for c, stype in enumerate(scanTypes):
      graphDict['Bias'][stype].Draw("E3same")
      graphDict['Bias'][stype].Draw("Lsame")
      if options.runMasses: graphDict['Bias'][stype].SetName('%s_cat%d_mass_%s_%s_bias'%(options.year,options.cat,truth,stype))
      else: graphDict['Bias'][stype].SetName('%s_cat%d_mu_%s_%s_bias'%(options.year,options.cat,truth,stype))
      outf.cd()
      graphDict['Bias'][stype].Write()
    if len(scanTypes)>1 : leg.Draw("same")
    if len(scanTypes)>1 : leg2.Draw("same")
    #canv.Print('%s/%s_%s_bias.pdf'%(options.outDir,ext,truth))
    #canv.Print('%s/%s_%s_bias.png'%(options.outDir,ext,truth))
  
    dummyHist.GetYaxis().SetTitle("#mu pull (fit-gen)/err")
    dummyHist.Draw()
    muPullBand.Draw("E3same")
    dummyHist.Draw("AXISsame")
    for c, stype in enumerate(scanTypes):
      graphDict['Pull'][stype].Draw("E3same")
      graphDict['Pull'][stype].Draw("Lsame")
      if options.runMasses: graphDict['Pull'][stype].SetName('%s_cat%d_mass_%s_%s_pull'%(options.year,options.cat,truth,stype))
      else: graphDict['Pull'][stype].SetName('%s_cat%d_mu_%s_%s_pull'%(options.year,options.cat,truth,stype))
      outf.cd()
      graphDict['Pull'][stype].Write()
    if len(scanTypes)>1:  leg.Draw("same")
    if len(scanTypes)>1:   leg2.Draw("same")
    canv.Print('%s/%s_%s_pull.pdf'%(options.outDir,ext,truth))
    canv.Print('%s/%s_%s_pull.png'%(options.outDir,ext,truth))
  
if not options.datfile:
  makePlot()
else:
  f = open(options.datfile)
  cats = []
  BASEDIR=""
  for linet in f.readlines():
    if linet.startswith('#') or linet.startswith('\n'): continue
    if linet.startswith('year'): 
      options.year = linet.split('=')[1].strip('\n')
      continue
    if "cats=" in linet:
      vlist = (linet.split("="))[1]
      cats = [int(v) for v in vlist.split(",")] 
      continue
    if "basedir=" in linet:
      BASEDIR=linet.split("=")[1]
      BASEDIR = BASEDIR.rstrip("\n")
      continue

    linestouse = []
    if "{cat}" in linet :
	for c in cats: linestouse.append(linet.replace("{cat}","%d"%c))
    else: linestouse.append(linet)
    for line in linestouse:
     if "{basedir}" in line: 
	line=line.replace("{basedir}",BASEDIR)
     opts = line.split()
     if opts[0].split('=')[1]=='mu':
      options.runMasses=False
     elif opts[0].split('=')[1]=='mass':
      options.runMasses=True
     else:
      sys.exit('Invalid options')
     options.cat = int(opts[1].split('=')[1])
     options.outDir = opts[2].split('=')[1]
     options.injectVal = float(opts[3].split('=')[1])
     options.expVals=[]
     options.files=[]
     for val in opts[4].split('=')[1].split(','):
      options.expVals.append(float(val))
     for file in opts[5].split('=')[1].split(','):
      options.files.append(file)
     makePlot()


  

