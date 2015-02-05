print "Loading Root..."

#import pdb; pdb.set_trace()
from ROOT import *
import os
import sys
import array
import math

gStyle.SetOptStat(000000)
gStyle.SetCanvasBorderMode(0);
gStyle.SetCanvasColor(kWhite);
gROOT.SetBatch(1)
gStyle.SetPadLeftMargin(0.18);
gStyle.SetPadRightMargin(0.05);

print "Setting Initial Parameters."
can = TCanvas("Plots","Plots",800,800)
leg = TLegend(0.70, 0.65, 0.945, 0.78)
leg.SetFillColor(0)
leg.SetBorderSize(1)
mytext = TLatex()
mytext.SetTextSize(0.025)
mytext.SetNDC()
rMin=-7
rMax=12
Label1=TText()
Label1.SetNDC()
Label1.SetText(0.3,0.65,"7TeV")
Label1.SetTextFont(62)
Label1.SetTextSize(.09)
Label1.SetTextAngle(90)
Label2=TText()
Label2.SetNDC()
Label2.SetText(0.3,0.3,"8TeV")
Label2.SetTextFont(62)
Label2.SetTextSize(.09)
Label2.SetTextAngle(90)
#splitline=5
splitline=9
myline = TLine(rMin,splitline,rMax,splitline)
myline.SetLineColor(kBlack)
myline.SetLineWidth(3)
myline.SetLineStyle(7)
dumbline = TLine()
dumbline.SetLineColor(kBlack)
dumbline.SetLineWidth(1)
BinLabel = ["Untagged 0","Untagged 1","Untagged 2","Untagged 3","Di-jet tight","Di-jet loose","Muon","Electron","MET","Untagged 0","Untagged 1","Untagged 2","Untagged 3","Di-jet"]
intlumi=[5.1,19.6]
Energy=[7,8]
#intlumi=[12.3]
#Energy=[8]
Masses=[125.0,125.5,126.0]
directory = sys.argv[1]

for Mass in Masses:
  file = TFile.Open(directory+"/higgsCombineSignalStrength.ChannelCompatibilityCheck.mH"+str(Mass).replace(".0","")+".root")
  normal=file.Get("fit_nominal")
  rFit=normal.floatParsFinal().find("r")
  CombinedBestFitObserved=rFit.getVal()
  CombinedBestFitErrorUp=rFit.getAsymErrorHi()
  CombinedBestFitErrorDown=rFit.getAsymErrorLo()
  if math.fabs(CombinedBestFitErrorDown)<0.000001: CombinedBestFitErrorDown=-rFit.getAsymErrorHi()
  altername=file.Get("fit_alternate")
  Channels=[]
  BestFitObserved=[]
  BestFitErrorUp=[]
  BestFitErrorDown=[]
  for i in range(altername.floatParsFinal().getSize()):
    #print i,altername.floatParsFinal().at(i).GetName(),altername.floatParsFinal().at(i).getVal(),altername.floatParsFinal().at(i).getAsymErrorHi(),altername.floatParsFinal().at(i).getAsymErrorLo()
    chrFit=altername.floatParsFinal().at(i)
    if chrFit.GetName().find("ChannelCompatibilityCheck")!=-1:
      Channels.append(chrFit.GetName())
      BestFitObserved.append(chrFit.getVal())
      BestFitErrorUp.append(chrFit.getAsymErrorHi())
      if math.fabs(chrFit.getAsymErrorLo())<0.000001: BestFitErrorDown.append(math.fabs(chrFit.getAsymErrorHi()))
      else: BestFitErrorDown.append(math.fabs(chrFit.getAsymErrorLo()))
  dummyHist = TH2F("dummy",";Best Fit #sigma/#sigma_{SM};",1,rMin,rMax,len(Channels),0,len(Channels))
  ChannelNum=array.array("f",[x + 0.5 for x in range(len(Channels))])
  graph=TGraphAsymmErrors(len(Channels),array.array("f",BestFitObserved),ChannelNum,array.array("f",BestFitErrorDown),array.array("f",BestFitErrorUp),array.array("f",[0]*len(Channels)),array.array("f",[0]*len(Channels)))
  graph.SetLineColor(kRed)
  graph.SetLineWidth(1)
  graph.SetMarkerStyle(21)
  graph.SetMarkerSize(2)
  graph.SetMarkerColor(kBlack)
  graph.SetFillColor(kWhite)
  #leg.AddEntry(graph,"BestFit Category")
  for i in range(len(Channels)):
    #print Channels[i],BinLabel[i]
    dummyHist.GetYaxis().SetBinLabel(i+1, BinLabel[i]);
  dummyHist.SetTitleSize(0.045,"X")
  dummyHist.SetTitleOffset(0.95,"X")
  dummyHist.SetLabelSize(0.045,"X")
  dummyHist.SetLabelFont(62,"Y")
  dummyHist.SetLabelSize(0.045,"Y")
  dummyHist.SetTitleOffset(1.1,"Y")
  dummyHist.SetLineColor(kBlack)
  dummyHist.SetFillColor(kGreen)
  #leg.AddEntry(dummyHist,"68% Combined")
  leg.AddEntry(graph,"Event Class")
  leg.AddEntry(dummyHist,"Combined","f")
  leg.AddEntry(NULL,"m_{H} = %.1f GeV" %Mass,"")
  leg.AddEntry(NULL,"#sigma/#sigma_{SM} = %.2f#pm%.2f" %(CombinedBestFitObserved,CombinedBestFitErrorUp),"")
  dummyHist.Draw()
  BestFitBand=TBox(CombinedBestFitObserved+CombinedBestFitErrorDown,0,CombinedBestFitObserved+CombinedBestFitErrorUp,len(Channels))
  #BestFitBand.SetFillStyle(3013)
  BestFitBand.SetFillColor(kGreen)
  BestFitBand.SetLineStyle(0)
  BestFitBand.Draw()
  BestFitLine=TLine(CombinedBestFitObserved,0,CombinedBestFitObserved,len(Channels))
  BestFitLine.SetLineWidth(1)
  BestFitLine.SetLineColor(kBlack)
  BestFitLine.Draw()
  graph.Draw("P SAME")
  leg.Draw()
  if len(intlumi)==2: mytext.DrawLatex(0.70,0.85,"#splitline{CMS preliminary}{#splitline{#sqrt{s} = %i TeV, L = %.1f fb^{-1}}{#sqrt{s} = %i TeV, L = %.1f fb^{-1}}}" %(int(Energy[0]),float(intlumi[0]),int(Energy[1]),float(intlumi[1])))
  else: mytext.DrawLatex(0.70,0.85,"#splitline{CMS preliminary}{#sqrt{s} = %i TeV, L = %.1f fb^{-1}}" %(int(Energy[0]),float(intlumi[0])))
  can.RedrawAxis()
  Label1.Draw()
  Label2.Draw()
  myline.Draw()
  dumbline.DrawLineNDC(0.7+0.0305,0.78-0.06,0.7+0.0305,0.78-0.037)
  can.SaveAs("BestFit"+str(Mass)+"GeV_7and8TeV.pdf")
  graph.Clear()
  dummyHist.Clear()
  BestFitBand.Clear()
  BestFitLine.Clear()
  leg.Clear()
  gDirectory.Delete("*")


print "Done!"
