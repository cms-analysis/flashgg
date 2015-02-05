import sys
import ROOT as r

r.gROOT.ProcessLine('.L $CMSSW_BASE/lib/$SCRAM_ARCH/libHiggsAnalysisCombinedLimit.so')
r.gROOT.ProcessLine('.L $CMSSW_BASE/src/HiggsAnalysis/HiggsTo2photons/h2gglobe/SimultaneousSignalFitting/scripts/effSigma.C+g')
from ROOT import effSigma

inf1 = r.TFile(sys.argv[1])
inf2 = r.TFile(sys.argv[2])

leg = r.TLegend(0.6,0.6,0.89,0.89)
leg.SetFillColor(0)
leg.SetLineColor(0)

for f in [inf1,inf2]:
	ws = f.Get('wsig_8TeV')
	mass = ws.var('CMS_hgg_mass')
	MH = ws.var('MH')
	smpdf = ws.pdf('sigpdfrel_sm_AllCats_allProcs')
	ggpdf = ws.pdf('sigpdfrel_gg_grav_AllCats_allProcs')
	qqpdf = ws.pdf('sigpdfrel_qq_grav_AllCats_allProcs')

	th_m125 = r.TH1F('m125','m125',3200,100,180)
	th_m126 = r.TH1F('m126','m126',3200,100,180)

	if f==inf1:
		plot = mass.frame(r.RooFit.Range(115,135))
	
	if f==inf1:
		MH.setVal(125)
		smpdf.plotOn(plot,r.RooFit.LineColor(r.kRed),r.RooFit.Normalization(r.RooAbsReal.RelativeExpected))
		smpdf.fillHistogram(th_m125,r.RooArgList(mass))
		print inf1.GetName(), 'm125', th_m125.GetMean(), th_m125.GetBinCenter(th_m125.GetMaximumBin())#effSigma(th_m125)
		MH.setVal(126)
		smpdf.plotOn(plot,r.RooFit.LineColor(r.kBlue),r.RooFit.LineStyle(2),r.RooFit.Normalization(r.RooAbsReal.RelativeExpected))
		smpdf.fillHistogram(th_m126,r.RooArgList(mass))
		print inf1.GetName(), 'm126', th_m126.GetMean(), th_m125.GetBinCenter(th_m125.GetMaximumBin()) #effSigma(th_m126)
	else: 
		MH.setVal(126)
		smpdf.plotOn(plot,r.RooFit.LineColor(r.kRed),r.RooFit.Normalization(r.RooAbsReal.RelativeExpected))
		smpdf.fillHistogram(th_m125,r.RooArgList(mass))
		print inf2.GetName(), 'm125', th_m125.GetMean(), th_m125.GetBinCenter(th_m125.GetMaximumBin()) #effSigma(th_m125)
		obj = plot.getObject(int(plot.numItems())-1)
		leg.AddEntry(obj,'Fitted','L')
		MH.setVal(125)
		smpdf.plotOn(plot,r.RooFit.LineColor(r.kBlue),r.RooFit.LineStyle(2),r.RooFit.Normalization(r.RooAbsReal.RelativeExpected))
		smpdf.fillHistogram(th_m126,r.RooArgList(mass))
		print inf2.GetName(), 'm126', th_m126.GetMean(), th_m125.GetBinCenter(th_m125.GetMaximumBin()) #effSigma(th_m126)
		obj = plot.getObject(int(plot.numItems())-1)
		leg.AddEntry(obj,'Extrapolated','L')

canv = r.TCanvas()
plot.Draw()
leg.Draw("same")
raw_input('Wait?')
canv.Print("extrap.pdf")
