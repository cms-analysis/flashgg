print "Loading Root..."

#import pdb; pdb.set_trace()
from ROOT import *
import os
import sys
import array

gROOT.Macro("$HOME/rootlogon.C")
gStyle.SetOptStat(000000)
gStyle.SetCanvasBorderMode(0);
gStyle.SetCanvasColor(kWhite);

print "Setting Initial Parameters."
can = TCanvas("Plots","Plots",850,600)
can.SetLogy(True)
can.SetGrid(True)
leg = TLegend(0.7, 0.77, 0.87, 0.87)
leg.SetFillColor(0)
leg.SetBorderSize(1)
mytext = TLatex()
mytext.SetTextSize(0.04)
mytext.SetNDC()
intlumi=4.76

pwd = "/data/ndpc2/c/HiggsGammaGamma/PhotonPlusJet/CMSSW_4_2_3/src/ND_Hto2Photons/TreeReaders/"

Masses=array.array("f",[x * 0.1 for x in range(1100,1505,5)])
MassError=array.array("f",[0]*81)
PValues = []
PValuesError = []
directory = sys.argv[1]
logfiles = os.popen("/bin/ls "+directory+"/SIGFrequentist.mH1??*.log").readlines()
for file in logfiles:
	filename = file.strip("\n")
	PValues.append(float(os.popen("grep p-value "+filename+" | awk '{print $3}'").readlines()[0].strip(")\n")))
	PValuesError.append(float(os.popen("grep p-value "+filename+" | awk '{print $5}'").readlines()[0].strip(")\n")))

#for i in range(len(PValues)):
	#if PValues[i]>0.5: PValues[i]=0.5

PValuesSignal = []
PValuesSignalError = []
directory += "_signal"
logfiles = os.popen("/bin/ls "+directory+"/SIGFrequentist.mH1??*.log").readlines()
for file in logfiles:
	filename = file.strip("\n")
	PValuesSignal.append(float(os.popen("grep p-value "+filename+" | awk '{print $3}'").readlines()[0].strip(")\n")))
	PValuesSignalError.append(float(os.popen("grep p-value "+filename+" | awk '{print $5}'").readlines()[0].strip(")\n")))

multigraph = TMultiGraph()
multigraph.SetTitle(";m_{Higgs} (GeV/c^{2});P-Value Frequentist")
#multigraph.SetMinimum(0.005)
multigraph.SetMinimum(0.0001)
multigraph.SetMaximum(10)

#import pdb; pdb.set_trace()
graphsignal=TGraphErrors(int(len(PValues)),Masses,array.array("f",PValuesSignal),MassError,array.array("f",PValuesSignalError))
graphsignal.SetLineColor(kBlue)
graphsignal.SetMarkerStyle(21)
graphsignal.SetMarkerColor(kBlue)
graphsignal.SetLineWidth(2)
graphsignal.SetFillColor(kWhite)
leg.AddEntry(graphsignal,"SM Higgs")
multigraph.Add(graphsignal)

graph=TGraphErrors(int(len(PValues)),Masses,array.array("f",PValues),MassError,array.array("f",PValuesError))
graph.SetLineColor(kBlack)
graph.SetMarkerStyle(21)
graph.SetLineWidth(2)
graph.SetFillColor(kWhite)
leg.AddEntry(graph,"Observed")
multigraph.Add(graph)

multigraph.Draw("AP")
multigraph.GetXaxis().SetRangeUser(110,150)
leg.Draw("")

mytext.DrawLatex(0.18,0.8,"#splitline{CMS preliminary}{#sqrt{s} = 7 TeV L = %.2f fb^{-1}}"%float(intlumi))
OneSigma = TLine(110, 0.158655254, 150, 0.158655254)
OneSigma.SetLineColor(kRed)
OneSigma.SetLineStyle(2)
OneSigma.Draw()
TwoSigma = TLine(110, 0.022750132, 150, 0.022750132)
TwoSigma.SetLineColor(kRed)
TwoSigma.SetLineStyle(2)
TwoSigma.Draw()
ThreeSigma = TLine(110, 0.001349898, 150, 0.001349898)
ThreeSigma.SetLineColor(kRed)
ThreeSigma.SetLineStyle(2)
ThreeSigma.Draw()
#mytext.DrawLatex(0.91,0.45,"1\sigma")
#mytext.DrawLatex(0.91,0.25,"2\sigma")
mytext.DrawLatex(0.91,0.6,"1\sigma")
mytext.DrawLatex(0.91,0.46,"2\sigma")
mytext.DrawLatex(0.91,0.27,"3\sigma")
can.SaveAs(sys.argv[2])
print "Done!"
