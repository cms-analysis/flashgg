print "Loading Root..."

#import pdb; pdb.set_trace()
from ROOT import *
import os
import sys
import array

gStyle.SetOptStat(000000)
gStyle.SetCanvasBorderMode(0);
gStyle.SetCanvasColor(kWhite);
gROOT.SetBatch(1)
#ROOT.gROOT.ProcessLine(".x ../tdrstyle.cc")

print "Setting Initial Parameters."
can = TCanvas("Plots","Plots")
can.SetLogy(True)
can.SetGrid(True)
leg = TLegend(0.65, 0.74, 0.89, 0.89)
leg.SetFillColor(0)
leg.SetBorderSize(1)
mytext = TLatex()
mytext.SetTextSize(0.04)
#mytext.SetFillColor(0)
mytext.SetNDC()
#Minimum=0.00000001
#Maximum=100
#Sigmas=5
#intlumi=[12.3]
#Energy=[8]
#intlumi=[5.1,12.3]
#Energy=[7,8]
#legands=[]
#legands=["Observed","Expected 1xSM","Expected 1.6xSM","Expected 1xSM m=125","Expected 1.6xSM m=125"]
#legands=["Expected 1xSM","Expected 1.6xSM","Expected 1xSM m=125","Expected 1.6xSM m=125"]
Minimum=0.00001
Maximum=10
Sigmas=4
#intlumi=[5.1,19.6]
#Energy=[7,8]
intlumi=[19.6]
Energy=[8]
#legands=["7 and 8TeV Observed","7 and 8TeV Expected","8TeV Observed","8TeV Expected","7TeV Observed","7TeV Expected"]
#legands=["Combined Observed", "8TeV Inclusive Categories", "8TeV DiJet Tag", "8TeV Muon Tag", "8TeV Electron Tag", "8TeV MET Tag", "7TeV Inclusive Categories", "7TeV DiJet Tag"]
legands=["Unique MVA Events - MVA Cats -Exp","Unique MVA Events - CiC Cats - Exp"
FrequantistPValues=[]
#FrequantistPValues=["higgsCombinePValue.HybridNew.mH125.0.root"]
#FrequantistPValues=["higgsCombinePValue.HybridNew.mH123.5.root","higgsCombinePValue.HybridNew.mH124.0.root","higgsCombinePValue.HybridNew.mH124.5.root","higgsCombinePValue.HybridNew.mH125.0.root","higgsCombinePValue.HybridNew.mH125.5.root","higgsCombinePValue.HybridNew.mH126.0.root"]
def getPValue(mass,tree):
  for i in range(tree.GetEntries()):
    tree.GetEntry(i)
    if tree.mh==mass:
      return tree.limit,tree.limitErr
  print "Mass "+str(mass)+" not found exiting"
  exit(1)

Masses=array.array("f",[x * 0.1 for x in range(1100,1501,10)])
files = sys.argv[1:-1]

if len(files)==2 and len(legands)==0: legands=["Observed","Expected"]
elif len(legands)==0:
  for i in range(len(files)+1):
    if i==0:legands.append("Observed")
    else: legands.append("Category "+str(i))

#colors = [ kBlack, kRed, kOrange, kGreen, kCyan, kBlue, kMagenta, 28]
colors = [ kBlack, kRed, kGreen, kCyan, kBlue, kBlack, kRed, kGreen, kCyan, kBlue]
styles = [1, 1, 1, 1, 1, 7, 7, 7, 7, 7]
#styles = [1, 1, 1, 1, 1, 1, 1, 1]
#colors = [ kRed, kBlue, kRed, kBlue]
#colors = [ kBlack, kBlack, kRed, kRed, kBlue, kBlue]
#styles = [ 1, 7, 1, 7, 1, 7]


multigraph = TMultiGraph()
multigraph.SetTitle(";M_{H} (GeV/c^{2});Local P-Value")
multigraph.SetMinimum(Minimum)
multigraph.SetMaximum(Maximum)

for file,legand,color,style in zip(files,legands,colors,styles):
  print file
  PValues=[]
  PValuesError=[]
  tree = TFile.Open(file).Get("limit")
  
  for Mass in Masses:
    pval,perr=getPValue(Mass,tree)
    PValues.append(pval)
    PValuesError.append(perr)

  graph=TGraphErrors(len(PValues),Masses,array.array("f",PValues),array.array("f",[0]*len(Masses)),array.array("f",PValuesError))
  graph.SetLineColor(color)
  graph.SetLineStyle(style)
  graph.SetLineWidth(2)
  #if legand.find("Expected")!=-1: graph.SetLineStyle(7)
  graph.SetFillColor(kWhite)
  if (legand!=""): leg.AddEntry(graph,legand,'L')
  multigraph.Add(graph)

if len(FrequantistPValues)>0:
  FrequentistGraph = TGraphErrors(len(FrequantistPValues))
  for i in range(len(FrequantistPValues)):
    FrequentistTree = TFile.Open(FrequantistPValues[i]).Get("limit")
    FrequentistTree.GetEntry(0)
    FrequentistGraph.SetPoint(i,FrequentistTree.mh,FrequentistTree.limit)
    FrequentistGraph.SetPointError(i,0,FrequentistTree.limitErr)
  FrequentistGraph.SetLineColor(kRed)
  FrequentistGraph.SetLineWidth(1)
  FrequentistGraph.SetMarkerStyle(8)
  FrequentistGraph.SetMarkerColor(kBlue)
  FrequentistGraph.SetFillColor(kWhite)
  #multigraph.Add(FrequentistGraph)
  leg.AddEntry(FrequentistGraph,"Assembly of Toys",'lep')

multigraph.Draw("AC")
multigraph.GetXaxis().SetRangeUser(110,150)
if len(FrequantistPValues)>0: FrequentistGraph.Draw("EP SAME")
leg.Draw("")

line=[]
label=[]
for i in range(Sigmas):
  y = RooStats.SignificanceToPValue(i+1)
  line.append(TLine(110, y, 150, y))
  line[i].SetLineWidth(2)
  line[i].SetLineStyle(2)
  line[i].SetLineColor(kRed)
  line[i].Draw()
  label.append(TLatex(110 + 2, y * 1.1, "%d #sigma" % (i+1)))
  label[i].SetTextAlign(11);
  label[i].Draw()

if len(intlumi)==2: mytext.DrawLatex(0.18,0.82,"#splitline{CMS preliminary}{#splitline{#sqrt{s} = %i TeV L = %.1f fb^{-1}}{#sqrt{s} = %i TeV L = %.1f fb^{-1}}}" %(int(Energy[0]),float(intlumi[0]),int(Energy[1]),float(intlumi[1])))
else: mytext.DrawLatex(0.18,0.8,"#splitline{CMS preliminary}{#sqrt{s} = %i TeV L = %.1f fb^{-1}}" %(int(Energy[0]),float(intlumi[0])))

can.SaveAs('%s.pdf'%sys.argv[-1])
can.SaveAs('%s.png'%sys.argv[-1])
print "Done!"
