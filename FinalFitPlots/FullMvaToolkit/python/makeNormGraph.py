import ROOT
import sys
from optparse import OptionParser

parser = OptionParser()
parser.add_option("-i","--input",dest="tfileName",help="Input file")
parser.add_option("-o","--output",dest="ofileName",help="Output file",type='str',default="bkgnorms.root")
parser.add_option("-l","--lowerMH",dest="mHmin",help="Output file",type='float',default=110.)
parser.add_option("-u","--upperMH",dest="mHmax",help="Output file",type='float',default=150.)
parser.add_option("-s","--step",dest="step",help="Output file",type='float',default=0.5)

(options,args)=parser.parse_args()

fi  = ROOT.TFile(options.tfileName)
fo  = ROOT.TFile(options.ofileName,"RECREATE")

mHmin = options.mHmin#110.
mHmax = options.mHmax#150. 
step  = options.step#0.5
nVals = int((mHmax-mHmin)/step + 1)

nSmooth = 100

ws = fi.Get("cms_hgg_workspace")

nGraph = ROOT.TGraph()

hist = ROOT.TH1F("NormHist","NormHist",nVals,mHmin-step/2,mHmax+step/2)

for mh_i in range(nVals):
  mass = float(mHmin + mh_i*step)
  var = ws.var("NBkgInSignal_mH%3.1f"%mass)
  nGraph.SetPoint(mh_i,mass,var.getVal())
  hist.SetBinContent(mh_i+1,var.getVal())

hist_smooth = hist.Clone()

# Smooth part
for i in range(nSmooth): 
	hist_smooth.Smooth()

hist_smooth.Scale(hist.Integral()/hist_smooth.Integral())
nGraph_smooth = ROOT.TGraph()

for mh_i in range(nVals):
  mass = float(mHmin + mh_i*step)
  nGraph_smooth.SetPoint(mh_i,mass,hist_smooth.GetBinContent(mh_i+1))

ROOT.gROOT.SetBatch(1)

can = ROOT.TCanvas("c","c",600,600)
nGraph.SetLineColor(ROOT.kGreen+1)
nGraph_smooth.SetLineColor(2)
nGraph.SetLineWidth(2)
nGraph_smooth.SetLineWidth(2)
leg = ROOT.TLegend(0.7,.7,.89,.89)
leg.SetFillColor(10)
leg.AddEntry(nGraph,"Nominal","L")
leg.AddEntry(nGraph_smooth,"Smoothed x %d"%nSmooth,"L")
nGraph.Draw("AL")
nGraph_smooth.Draw("L")
leg.Draw()

nGraph.SetName("Nominal")
nGraph_smooth.SetName("NormGraph")

fo.cd()
nGraph.Write()
nGraph_smooth.Write()

can.Write()
fo.Close()

