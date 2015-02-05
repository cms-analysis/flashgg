#!/usr/bin/env python

from optparse import OptionParser
parser = OptionParser()
parser.add_option("-i","--infile")
parser.add_option("-n","--ncats")
parser.add_option("-p","--procs",default='ggh,vbf,wh,zh,tth')
parser.add_option("-w","--ws",default="wsig_8TeV")
(options,args) = parser.parse_args()

import ROOT as r
r.gSystem.Load("$CMSSW_BASE/lib/$SCRAM_ARCH/libHiggsAnalysisCombinedLimit.so")
r.gSystem.Load("../libLoopAll.so")

tf = r.TFile(options.infile)
ws = tf.Get(options.ws)
#ws.Print()

mass = ws.var('CMS_hgg_mass')
MH = ws.var('MH')

sumAllPdfs = ws.pdf('sigpdfrelAllCats_allProcs')
plot = mass.frame()
plot2 = mass.frame()

for m in range(110,151,5):
	MH.setVal(m)
	allData = ws.data('sig_mass_m%d_AllCats'%m)
	print m, allData.GetName()
	allData.plotOn(plot,r.RooFit.Binning(80))
	sumAllPdfs.plotOn(plot)

canv = r.TCanvas()
plot.GetXaxis().SetTitle("m_{#gamma#gamma} (GeV)")
plot.Draw()
canv.Print("plots/all_fits.pdf")
canv.Print("plots/all_fits.png")

for m in range(1,110,151):
	MH.setVal(m)
	sumAllPdfs.plotOn(plot2,r.Normalization(1.0,r.RooFit.RooAbsReal.RelativeExpected))

canv = r.TCanvas()
plot2.GetXaxis().SetTitle("m_{#gamma#gamma} (GeV)")
plot2.Draw()
canv.Print("plots/all_interp.pdf")
canv.Print("plots/all_interp.png")




