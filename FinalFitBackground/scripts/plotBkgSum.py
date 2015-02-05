import os

import ROOT as r

r.gROOT.ProcessLine('.L $CMSSW_BASE/lib/$SCRAM_ARCH/libHiggsAnalysisCombinedLimit.so')

def makePlot(datfile,bkgfile,sigfile,ncats,sqrtS):

	df = r.TFile(datfile)
	bf = r.TFile(bkgfile)
	sf = r.TFile(sigfile)

	dws = df.Get('cms_hgg_workspace')
	bws = bf.Get('multipdf')
	sws = sf.Get('wsig_%dTeV'%sqrtS)

	extPdfs = r.RooArgList()
	bkgPdfs = r.RooArgList()
	bkgNorms = r.RooArgList()

	for c in range(ncats):
		bkgMPdf = bws.pdf('CMS_hgg_cat%d_%dTeV_bkgshape'%(c,sqrtS))
		bkgNorm = bws.var('CMS_hgg_cat%d_%dTeV_bkgshape_norm'%(c,sqrtS))
		bkgPdf = bkgMPdf.getCurrentPdf()
		bkgPdfs.add(bkgPdf)
		bkgNorms.add(bkgNorm)
		extPdf = r.RooExtendPdf('ext_cat%d'%c,'',bkgPdf,bkgNorm)
		extPdfs.add(extPdf)
		if c==0:
			dataSum = dws.data('data_mass_cat%d'%c)
		else:
			data = dws.data('data_mass_cat%d'%c)
			dataSum.append(data)

	sigpdf = sws.pdf('sigpdfrelAllCats_allProcs')
	bkgpdf = r.RooAddPdf('bkg','bkg',bkgPdfs,bkgNorms)

	mass = bws.var('CMS_hgg_mass')
	mass.setRange('low',100,110)
	mass.setRange('high',150,180)

	mh = sws.var('MH')
	mh.setVal(125)
	mh.setConstant()

	canv = r.TCanvas()
	leg = r.TLegend(0.6,0.6,0.89,0.89)
	leg.SetLineColor(0)
	leg.SetFillColor(0)
	plot = mass.frame()
	dataSum.plotOn(plot,r.RooFit.Invisible(),r.RooFit.Binning(80))
	dataSum.plotOn(plot,r.RooFit.Binning(80))
	#dataSum.plotOn(plot,r.RooFit.CutRange('low'),r.RooFit.Binning(80))
	#dataSum.plotOn(plot,r.RooFit.CutRange('high'),r.RooFit.Binning(80))
	dataLeg = plot.getObject(int(plot.numItems()-1))
	leg.AddEntry(dataLeg,'Data','LEP')
	bkgpdf.plotOn(plot,r.RooFit.Normalization(1.0,r.RooAbsReal.RelativeExpected),r.RooFit.LineColor(r.kRed),r.RooFit.LineWidth(3))
	bkgLeg = plot.getObject(int(plot.numItems()-1))
	leg.AddEntry(bkgLeg,'Background Model','L')
	sigpdf.plotOn(plot,r.RooFit.Normalization(5.0,r.RooAbsReal.RelativeExpected),r.RooFit.LineColor(r.kBlue),r.RooFit.LineWidth(3))
	sigpdf.plotOn(plot,r.RooFit.Normalization(5.0,r.RooAbsReal.RelativeExpected),r.RooFit.LineColor(r.kBlue),r.RooFit.LineWidth(3),r.RooFit.FillColor(38),r.RooFit.FillStyle(3001),r.RooFit.DrawOption('F'))
	sigLeg = plot.getObject(int(plot.numItems()-1))
	leg.AddEntry(sigLeg,'Signal (5xSM)','LF')
	
	plot.SetMinimum(0.0001)
	plot.GetXaxis().SetTitle('m_{#gamma#gamma} (GeV)')
	plot.SetTitle('Combined mass distribution - %dTeV'%sqrtS)
	plot.Draw()
	leg.Draw()

	canv.Update()
	canv.Modified()
	canv.Print('comb_%dTeV.pdf'%sqrtS)
	canv.Print('comb_%dTeV.png'%sqrtS)

r.gROOT.SetBatch()
makePlot('~/latest_workspaces/legacy_workspaces/MVA/legacy_preapprov/CMS-HGG_massfac_7TeV_data.root','~/latest_workspaces/legacy_workspaces/MVA/legacy_preapprov/CMS-HGG_massfac_7TeV_multipdf.root','~/latest_workspaces/legacy_workspaces/MVA/legacy_preapprov/CMS-HGG_massfac_7TeV_sigfit.root',11,7)
makePlot('~/latest_workspaces/legacy_workspaces/MVA/legacy_preapprov/CMS-HGG_massfac_8TeV_data.root','~/latest_workspaces/legacy_workspaces/MVA/legacy_preapprov/CMS-HGG_massfac_8TeV_multipdf.root','~/latest_workspaces/legacy_workspaces/MVA/legacy_preapprov/CMS-HGG_massfac_8TeV_sigfit.root',14,8)
	
	
