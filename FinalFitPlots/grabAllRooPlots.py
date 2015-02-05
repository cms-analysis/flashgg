import ROOT
import sys, os
import numpy

grepmode=False
if len(sys.argv) < 2: 
	sys.exit("grabAllRooPlots.py usage:  python grabAllRooPlots.py FileName.root (optional -grep=expr , only plots containing expr in name are used)")

fileName = sys.argv[1]

if len(sys.argv) == 3:
  if "-grep=" in sys.argv[2]:
	grepmode = True
	grep	 = ((sys.argv[2]).split("="))[-1]

ROOT.gROOT.SetBatch(True)
ROOT.gROOT.SetStyle("Plain")
F = ROOT.TFile(fileName)
keys = F.GetListOfKeys()
plots = []

if not os.path.isdir("FitPlots"):
  os.makedirs("FitPlots")

if grepmode: print "grepping ", grep
for K in keys:
	obj = K.ReadObj()
	if grepmode:
	  if "plot" in obj.GetName() and grep in obj.GetName(): plots.append(obj.Clone())
	else:
	  if "plot" in obj.GetName(): plots.append(obj.Clone())

nPlots = len(plots)

if (nPlots >1):
  
  if int(nPlots**0.5)*int(nPlots**0.5) == nPlots: 
  	can = ROOT.TCanvas("c","Fit Plots",int(nPlots**0.5)*1200,int(nPlots**0.5)*900) # each plot needs to be pretty big
	can.Divide(int(nPlots**0.5),int(nPlots**0.5))

  else:
    sqrtnPlots = int(nPlots**0.5)+1
    can = ROOT.TCanvas("c","Fit Plots",sqrtnPlots*1200,sqrtnPlots*900) # each plot needs to be pretty big
    can.Divide(sqrtnPlots,sqrtnPlots)
else: 
  can = ROOT.TCanvas("c","Fit Plots",1200,900)
for i,P in enumerate(plots):
  can.cd(i+1)
  P.DrawClonePad()
  newcan = ROOT.TCanvas()
  P.DrawClonePad()
  mass = (P.GetName().split("model_")[1]).split("_cat")[0]
  newcan.SaveAs("FitPlots/normFit_m"+mass+".png")

## can.SaveAs("allThePlots_%s.eps"%fileName)
dir=os.path.dirname(fileName)
base=os.path.basename(fileName).replace(".root","")
can.SaveAs(os.path.join(dir,"allThePlots_%s.eps"%base) )

# also make norm vs mh plot
ws = F.Get('cms_hgg_workspace')
evalMasses = numpy.arange(110,150.5,0.5)
normG = ROOT.TGraph(len(evalMasses))
for i,m in enumerate(evalMasses):
  d = ws.var('NBkgInSignal_mH%3.1f'%m)
  normG.SetPoint(i,m,d.getVal())

normCan = ROOT.TCanvas()
normG.SetMarkerSize(2)
normG.SetLineWidth(2)
normG.SetTitle("Fit normalisation")
normG.GetYaxis().SetTitle("Background events in signal region")
normG.GetXaxis().SetTitle("m_{H} (GeV)")
normG.Draw("ALP")
normCan.SaveAs("FitPlots/normAll.png")

