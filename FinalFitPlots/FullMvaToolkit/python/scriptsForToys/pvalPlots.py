import sys
import ROOT
from ROOT import TColor
ROOT.gROOT.SetBatch(1)
intlumi="5.09"
bin=0
def Setup(gr,col,style,leg,name,MG):
	gr.SetLineWidth(2)
	gr.SetLineColor(col)
	gr.SetLineStyle(style)
	gr.SetMarkerColor(col)
	gr.SetMarkerSize(0.8)
	gr.SetMarkerStyle(20)
	leg.AddEntry(gr,name,"L")
	MG.Add(gr)

def fitSetup(gr,col,style,MG):
	gr.SetLineWidth(2)
	gr.SetLineColor(col)
	gr.SetLineStyle(style)
	gr.SetMarkerColor(col)
	gr.SetMarkerSize(0.8)
	gr.SetMarkerStyle(20)
	gr.SetFillColor(9)
	MG.Add(gr)

MG=ROOT.TMultiGraph()
fitMG=ROOT.TMultiGraph()

if int(bin)>0 and int(bin)<10:
  files = [
  #"/vols/cms02/nw709/hgg/src_cvs/dec14/CMSSW_4_2_8/src/HiggsAnalysis/CombinedLimit/appmva/bdtcount/limit/pvals.root",
  #"/vols/cms02/nw709/hgg/src_cvs/dec14/CMSSW_4_2_8/src/HiggsAnalysis/CombinedLimit/appmva/bdtcount/limit/pvals_vbf.root",
  #"/vols/cms02/nw709/hgg/src_cvs/dec14/CMSSW_4_2_8/src/HiggsAnalysis/CombinedLimit/appmva/bdtcount/limit/pvals_novbf.root"
  ]
  fitfiles = [
  #"/vols/cms02/nw709/hgg/src_cvs/dec14/CMSSW_4_2_8/src/HiggsAnalysis/CombinedLimit/appmva/bdtcount/limit/bf.root"
  "mva-datacards-grad-bin"+bin+"/bf_noerr.root"
  ]
else:
  files = [
  #"/vols/cms02/nw709/hgg/src_cvs/dec14/CMSSW_4_2_8/src/HiggsAnalysis/CombinedLimit/appmva/bdtcount/limit/pvals.root",
  #"mva-datacards-grad-bin1/pvals.root",
  #"mva-datacards-grad-bin2/pvals.root",
  #"mva-datacards-grad-bin3/pvals.root",
  #"mva-datacards-grad-bin4/pvals.root",
  #"mva-datacards-grad-bin5/pvals.root",
 # "mva-datacards-grad-bin6/pvals.root"
  sys.argv[1]+"/pvals.root"
  ]
  fitfiles = [
  #"/vols/cms02/nw709/hgg/src_cvs/dec14/CMSSW_4_2_8/src/HiggsAnalysis/CombinedLimit/appmva/bdtcount/limit/bf.root",
  #"mva-datacards-grad-bin1/bf_noerr.root",
  #"mva-datacards-grad-bin2/bf_noerr.root",
  #"mva-datacards-grad-bin3/bf_noerr.root",
  #"mva-datacards-grad-bin4/bf_noerr.root",
  #"mva-datacards-grad-bin5/bf_noerr.root",
 # "mva-datacards-grad-bin6/bf_noerr.root"
  sys.argv[1]+"/bf.root"
  ]

medfiles = [
#"/vols/cms02/nw709/hgg/src_cvs/dec14/CMSSW_4_2_8/src/HiggsAnalysis/CombinedLimit/appmva/bdtcount/limit/pvals.root"
]

onepoint = ROOT.TGraphErrors()
onepoint.SetPoint(0,124.0,0.0012012)
onepoint.SetPointError(0,0,0.0003465)
onepoint.SetMarkerColor(38)
onepoint.SetMarkerStyle(20)
onepoint.SetMarkerSize(1.5)

FILES = []
MEDFILES = []
GR=[]
fitFILES = []
fitGR=[]

colors=[1,2,TColor.GetColorDark(3),4,6,7,8]
styles=[1,1,1,1,1,1,1]
colors_med=[38]
styles_med=[7]

fitcolors=[1,2,3,4,6,7,9]
fitstyles=[1,1,1,1,1,1,1,1]

if int(bin)==1: names=["Local P-value (Bin VBF)"]
else:
  names=[
  "Local P-value (Bin "+str(int(bin)-1)+")"

  #"#splitline{Local P-value}{(combined)}",
  #"#splitline{Local P-value}{(Bin VBF)}",
  #"#splitline{Local P-value}{(Bin 1)}",
  #"#splitline{Local P-value}{(Bin 2)}",
  #"#splitline{Local P-value}{(Bin 3)}",
  #"#splitline{Local P-value}{(Bin 4)}",
  #"#splitline{Local P-value}{(Bin 5)}",
  #"#splitline{Local P-value}{(no di-jet)}",
  #"#splitline{Local P-value}{(di-jet only)}"
  ]
names_med=["1xSM Higgs Median Expected"]

# significance lines
#Lines = [1.,2.,3.,4.]
Lines = [1.,2.,3.]#,4.,5.,6.]
Vals=[ROOT.RooStats.SignificanceToPValue(L) for L in Lines]
TLines=[ROOT.TLine(110,V,150,V) for V in Vals]

fitLines = [1.]
fitTLines=[ROOT.TLine(110,fl,150,fl) for fl in fitLines]

legend=ROOT.TLegend(0.11,0.135,0.35,0.4)
legend.SetFillColor(0)
legend.SetBorderSize(0)

for i in range(len(medfiles)):
	MEDFILES.append(ROOT.TFile(medfiles[i]))
	GR.append(MEDFILES[i].Get("median"))
	Setup(GR[-1],colors_med[i],styles_med[i],legend,names_med[i],MG)

for i in range(len(files)):
	FILES.append(ROOT.TFile(files[i]))
	ggr =FILES[i].Get("observed")
	print ggr.GetName() 
	print ROOT.RooStats.PValueToSignificance(ggr.Eval(124.0))
	print ggr.Eval(124.0)
	GR.append(FILES[i].Get("observed"))
	
	Setup(GR[-1],colors[i],styles[i],legend,names[i],MG)

#MG.Add(onepoint)
#legend.AddEntry(onepoint,"Observed p-value (Ensemble)","PEl")

for i in range(len(fitfiles)):
	fitFILES.append(ROOT.TFile(fitfiles[i]))
	fitGR.append(fitFILES[i].Get("bestfit"))
	fitSetup(fitGR[i],fitcolors[i],fitstyles[i],fitMG)

ROOT.gROOT.SetStyle("Plain")
c = ROOT.TCanvas("c","c",1600,1800)
# Make 2 pads for the signal strength fit.
up = ROOT.TPad("u","u",0.01,0.7,0.99,0.99);
dp = ROOT.TPad("d","d",0.01,0.01,0.99,0.7);
up.SetNumber(1);
dp.SetNumber(2);
up.Draw();
dp.Draw();

mytext= ROOT.TLatex()
mytext.SetTextSize(0.045)

c.cd(1)
fitMG.Draw("AL")#E4")
#fitMGLINE = ROOT.TGraph()
#for m in range(110,151): fitMGLINE.SetPoint(m-110,float(m),fitMG.Eval(float(m)))
fitMG.Draw("LP")
fitMG.GetXaxis().SetRangeUser(110,150)
fitMG.GetYaxis().SetRangeUser(0,3)
fitMG.GetXaxis().SetTitle("")
fitMG.GetYaxis().SetTitle("#mu")
fitMG.GetYaxis().SetTitleSize(0.15)
fitMG.GetYaxis().SetTitleOffset(0.12)
#fitMG.GetYaxis().SetLabelSize(0.1)
#fitMG.GetXaxis().SetLabelSize(0.1)
for j,TL in enumerate(fitTLines):
	TL.SetLineStyle(1)
	TL.SetLineColor(1)
	TL.SetLineWidth(2)
	TL.Draw("same")
#	text.DrawLatex(151,fitLines[j],"%dxSM"%Lines[j])
#

fitMG.GetYaxis().SetNdivisions(3)
fitMG.GetYaxis().SetLabelSize(0.11);
fitMG.GetXaxis().SetLabelSize(0.11);
up.SetGrid(True)
#dp.SetLogy()
#
c.cd(2)
MG.Draw("AL")#LP")
MG.GetXaxis().SetRangeUser(110,150)
MG.GetYaxis().SetRangeUser(1.e-4,2.5)

MG.GetXaxis().SetTitle("m_{H} (GeV)")
MG.GetYaxis().SetTitle("p-value")
MG.GetYaxis().SetTitleOffset(0.6)
MG.GetYaxis().SetTitleSize(0.05)
MG.GetXaxis().SetTitleSize(0.05)
MG.GetYaxis().SetLabelSize(0.05)
MG.GetXaxis().SetLabelSize(0.05)
#onepoint.Draw("PE2L")
text = ROOT.TLatex()

#text.SetNDC()
for j,TL in enumerate(TLines):
	TL.SetLineStyle(7)
	TL.SetLineColor(2)
	TL.SetLineWidth(1)
	TL.Draw("same")
	text.DrawLatex(150.5,Vals[j]*0.88,"%d #sigma"%Lines[j])
#mytext.SetFillColor(8)	
mytext.SetNDC()
#mytext.SetFillColor(0)
#mytext.SetLineColor(0)
legend.Draw()

Box = ROOT.TBox(137,0.0015,147,0.0055)
Box.SetFillColor(10)
Box.SetFillStyle(1001)
#Box.Draw()
#mytext
mytext.DrawLatex(0.55,0.22,"#splitline{CMS preliminary}{#sqrt{s} = 7 TeV L = %.2f fb^{-1}}"%float(intlumi))
dp.SetGrid(True)
dp.SetLogy()


#raw_input() 
#dp.SaveAs("pvals_bin"+bin+".pdf")
if int(bin)==1: c.SaveAs("pvalues_binVBF.pdf")
if int(bin)>1 and int(bin)<10: c.SaveAs("pvalues_bin"+str(int(bin)-1)+".pdf")
else: 
  c.SaveAs(sys.argv[1]+"/pvalues.pdf")
  dp.SaveAs(sys.argv[1]+"/pvalues_nobf.pdf")
  c.SaveAs(sys.argv[1]+"/pvalues.png")
  dp.SaveAs(sys.argv[1]+"/pvalues_nobf.png")

