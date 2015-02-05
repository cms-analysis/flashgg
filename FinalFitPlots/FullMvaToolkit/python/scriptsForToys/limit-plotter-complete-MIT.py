# Original Authors - Nicholas Wardle, Nancy Marinelli, Doug Berry

# Run this with limit-plotter-complete.py METHOD model
# 	model  = sm (standard model) or ff (fermiophobic)
#	METHOD = ProfileLikelihood, Bayesian, Frequentist

# Standard Imports and calculators
import ROOT
import array,sys,numpy
ROOT.gROOT.ProcessLine(".L medianCalc.C++")
from ROOT import medianCalc
from ROOT import FrequentistLimits
from optparse import OptionParser

ROOT.gROOT.SetStyle("Plain")
ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptStat(0)

#-------------------------------------------------------------------------
# Configuration for the Plotter
intlumi = str(5.09)
#EXPmasses = [110,115,120,125,130,135,140,150]       # Only used in Bayesian and PL method
#'OBSmasses = [110,115,120,125,130,135,140,150]       # Only used in Bayesian and PL method
OBSmasses = numpy.arange(110,150.5,1.)
EXPmasses = numpy.arange(110,150.5,1.)
#OBSmasses = [110,115,120,125,130,135,140,150]
theorySMScales = [5,10]  			# A list of the C x sigma to draw

OFFSETLOW=2
OFFSETHIGH=4

FILLSTYLE=1001
SMFILLSTYLE=3244
FILLCOLOR_95=ROOT.kGreen
FILLCOLOR_68=ROOT.kYellow
FILLCOLOR_T=ROOT.kAzure+7			# Theory lines color
RANGEYABS=[0.0,0.6]
RANGEYRAT=[0.0,4]
#-------------------------------------------------------------------------
# UserInput
parser=OptionParser()
parser.add_option("-r","--doRatio",action="store_true")
parser.add_option("-s","--doSmooth",action="store_true")
parser.add_option("-b","--bayes",dest="bayes")
parser.add_option("-o","--outputLimits",dest="outputLimits")
parser.add_option("-e","--expectedOnly",action="store_true")
parser.add_option("-p","--path",dest="path",default="")
parser.add_option("","--pval",action="store_true")
(options,args)=parser.parse_args()
# ------------------------------------------------------------------------

if options.pval: EXPmasses=[]

"""
BASELINE = ROOT.TFile("limitICVBF_synch_e2.root")
baseline = BASELINE.Get("median")
baselineObs = BASELINE.Get("observed")
baseline.SetLineColor(1)
baselineObs.SetLineColor(1)
"""
# Overlay the Baysian observed Limit 
if options.bayes:
  BayesianFile =ROOT.TFile(options.bayes) 
  bayesObs = BayesianFile.Get("observed")

print "doRatio: ", options.doRatio
print "doSmooth: ", options.doSmooth
 
Method = args[0]
EXPName = Method+"/higgsCombineEXPECTED."+Method
if Method == "Asymptotic":  EXPName = Method+"/higgsCombineTest."+Method  # everyhting contained here
if Method == "ProfileLikelihood" or Method=="Asymptotic":
  OBSName = Method+"/higgsCombineTest."+Method
if Method == "Bayesian":
  OBSName = Method+"/higgsCombineOBSERVED.MarkovChainMC"
if Method == "Frequentist":
  OBSName = Method+"/higgsCombineOBSERVED.Frequentist"

EXPName=options.path+EXPName
OBSName=options.path+OBSName

if Method == "Frequentist" or Method == "Asymptotic": EXPmasses = OBSmasses[:]

if args[1] == "sm":
 ROOT.gROOT.ProcessLine(".L Normalization.C++")
 from ROOT import GetBR
 from ROOT import GetXsection
 from theory_sm import *
 extraString = "SM"
elif args[1] == "ff":
 ROOT.gROOT.ProcessLine(".L Normalization_ff.C++")
 from ROOT import GetBR
 from ROOT import GetXsection
 from theory_ff import *
 extraString = "FP"
else: sys.exit("choose either sm or ff model")

# get rid of the theory bands outside of the observed masses:
sub_allMasses		=[]
sub_xSec     		=[]
sub_xSecErrPlusPercent 	=[]
sub_xSecErrMinusPercent =[]
sub_br			=[]
for k in range(len(allMasses)):
  if allMasses[k] >= min(OBSmasses):
   sub_allMasses.append(allMasses[k])
   sub_xSec.append(xSec[k])    		
   sub_xSecErrPlusPercent.append(xSecErrPlusPercent[k])   	
   sub_xSecErrMinusPercent.append(xSecErrMinusPercent[k]) 
   sub_br.append(br[k])	
	
allMasses=sub_allMasses[:]	    
xSec=			sub_xSec[:]
xSecErrPlusPercent=	sub_xSecErrPlusPercent[:]
xSecErrMinusPercent=	sub_xSecErrMinusPercent[:]
br=			sub_br[:]


###### Nick's python for limits plus additional stuff to plot SM lines
ROOT.gROOT.ProcessLine( \
   "struct Entry{	\
    double r;		\
   };"
)
from ROOT import Entry
def getOBSERVED(file,entry=0):
  try:
   tree = file.Get("limit")
  except:
   return -1
  br = tree.GetBranch("limit")
  c = Entry()
  br.SetAddress(ROOT.AddressOf(c,'r'))
  tree.GetEntry(entry)
  return c.r

if Method=="Frequentist":
  EXPfiles=[]
  EXPmasses = OBSmasses[:]
  for m in EXPmasses:
    #if int(m)==m:
    #  EXPfiles.append(ROOT.TFile(EXPName+".mH%d.quant0.500.root"%m))
    #else:
      EXPfiles.append(ROOT.TFile(EXPName+".mH%.1f.quant0.500.root"%m))
  
elif Method=="Asymptotic":
  EXPfiles=[]
  EXPmasses = OBSmasses[:]
  for m in EXPmasses:
    #if int(m)==m:
    #  EXPfiles.append(ROOT.TFile(EXPName+".mH%d.root"%m))
    #else:
      EXPfiles.append(ROOT.TFile(EXPName+".mH%.1f.root"%m))

else:
  EXPfiles = [ROOT.TFile(EXPName+".mH%d.root"%m) for m in EXPmasses]

# Get the observed limits - Currently only does up to 1 decimal mass points
OBSfiles = []
if not options.expectedOnly:
  for m in OBSmasses:
    #if int(m)==m:
    #  OBSfiles.append(ROOT.TFile(OBSName+".mH%d.root"%m))
    #else:
      OBSfiles.append(ROOT.TFile(OBSName+".mH%.1f.root"%m))
  if Method == "Asymptotic":  obs = [getOBSERVED(O,5) for O in OBSfiles] # observed is last entry in these files
  else: obs = [getOBSERVED(O) for O in OBSfiles]
else:
  obs = [0 for O in OBSmasses]
  OBSfiles = obs[:]
#-------------------------------------------------------------------------

# Set-up the GRAPHS
leg=ROOT.TLegend(0.16,0.75,0.49,0.89)
leg.SetFillColor(0)
leg.SetBorderSize(0)

graph68  = ROOT.TGraphAsymmErrors()
graph95  = ROOT.TGraphAsymmErrors()
graphMed = ROOT.TGraphAsymmErrors()
graphObs = ROOT.TGraphAsymmErrors()
graphOne = ROOT.TGraphAsymmErrors()
dummyGraph= ROOT.TGraphAsymmErrors()

graph68up = ROOT.TGraphErrors()
graph68dn = ROOT.TGraphErrors()
graph95up = ROOT.TGraphErrors()
graph95dn = ROOT.TGraphErrors()
graphmede = ROOT.TGraphErrors()

LegendEntry = ""
if Method == "ProfileLikelihood": LegendEntry = "PL"
if Method == "Bayesian": LegendEntry = "Bayesian"
if Method == "Frequentist": LegendEntry = "CLs"
if Method == "Asymptotic": LegendEntry = "CLs - Asymptotic"

#if not options.expectedOnly: leg.AddEntry(baselineObs,"Observed %s (BDT Count) "%LegendEntry,"L")
if not options.expectedOnly: leg.AddEntry(graphObs,"Observed %s"%LegendEntry,"L")
if options.bayes and not options.expectedOnly: leg.AddEntry(bayesObs,"Observed Bayesian Limit","L")
#leg.AddEntry(baseline,"Expected (BDT Count)","L")
leg.AddEntry(graphMed,"Expected","L")
leg.AddEntry(graph68,"#pm 1#sigma","F")
leg.AddEntry(graph95,"#pm 2#sigma","F")

MG = ROOT.TMultiGraph()


#EXPECTED
for i,mass,f in zip(range(len(EXPfiles)),EXPmasses,EXPfiles):
  if options.pval: continue
  sm = 1.
 
  median = array.array('d',[0])
  up68   = array.array('d',[0])
  dn68   = array.array('d',[0])
  up95   = array.array('d',[0])
  dn95   = array.array('d',[0])

  if not options.doRatio: sm = GetBR(mass)*GetXsection(mass) 
  if Method == "Asymptotic":   
      median[0] = getOBSERVED(f,2)
      up95[0]   = getOBSERVED(f,4)
      dn95[0]   = getOBSERVED(f,0)
      up68[0]   = getOBSERVED(f,3)
      dn68[0]   = getOBSERVED(f,1)
      print "Mass - ",mass 
      print "Median   :",median[0] 
      print "Up 95%   :",up95[0]
      print "Down 95% :",dn95[0]
      print "Up 68%   :",up68[0]
      print "Down 68% :",dn68[0]

  else:
    tree = f.Get("limit")
    medianCalc("r_mH"+str(mass),tree,median,up68,dn68,up95,dn95)

  graph68.SetPoint(i,float(mass),median[0]*sm)
  graph95.SetPoint(i,float(mass),median[0]*sm)
  graphMed.SetPoint(i,float(mass),median[0]*sm)
  graphOne.SetPoint(i,float(mass),1.*sm)
  
  if Method == "Frequentist":

      up95[0]   = FrequentistLimits(f.GetName().replace("0.500.root","0.975.root"))
      dn95[0]   = FrequentistLimits(f.GetName().replace("0.500.root","0.027.root"))
      up68[0]   = FrequentistLimits(f.GetName().replace("0.500.root","0.840.root"))
      dn68[0]   = FrequentistLimits(f.GetName().replace("0.500.root","0.160.root"))

  
  diff95_up = abs(median[0] - up95[0])*sm
  diff95_dn = abs(median[0] - dn95[0])*sm
  diff68_up = abs(median[0] - up68[0])*sm
  diff68_dn = abs(median[0] - dn68[0])*sm
  
  graph68.SetPointError(i,0,0,diff68_dn,diff68_up)
  graph95.SetPointError(i,0,0,diff95_dn,diff95_up)
  graphMed.SetPointError(i,0,0,0,0)
  graphOne.SetPointError(i,0,0,0,0)

  if options.doSmooth:  # Always fit the absolute not the ratio
    sm=1.   
 #   for j,mm in enumerate(allMasses): 
#	if mm==mass: sm = xSec[j]*br[j]

    graphmede.SetPoint(i,float(mass),median[0]*sm)
    graph68up.SetPoint(i,float(mass),up68[0]*sm)
    graph68dn.SetPoint(i,float(mass),dn68[0]*sm)
    graph95up.SetPoint(i,float(mass),up95[0]*sm)
    graph95dn.SetPoint(i,float(mass),dn95[0]*sm)


# Smooth the Bands set -doSmooth
# Since i always fitted to the Absolute, need to see if i want the Ratio instead
if options.doSmooth:
 fitstring = "[0] + [1]*x*x + [2]*x*x*x +[3]*x*x*x*x + [4]*x"
 medfunc = ROOT.TF1("medfunc",fitstring,109.,150.);
 up68func = ROOT.TF1("up68func",fitstring,109.,150.);
 dn68func = ROOT.TF1("dn68func",fitstring,109.,150.);
 up95func = ROOT.TF1("up95func",fitstring,109.,150.);
 dn95func = ROOT.TF1("dn95func",fitstring,109.,150.);

 graphmede.Fit(medfunc,"R,M,EX0","Q")
 graph68up.Fit(up68func,"R,M,EX0","Q")
 graph68dn.Fit(dn68func,"R,M,EX0","Q")
 graph95up.Fit(up95func,"R,M,EX0","Q")
 graph95dn.Fit(dn95func,"R,M,EX0","Q")
 
# newMG = ROOT.TMultiGraph() 
 newCanvas = ROOT.TCanvas()
# newMG.Add(graphmede)
# newMG.Add(graphMed)
 #graphmede.SetLineColor(4)
 ROOT.gStyle.SetOptFit(1111)
 graphmede.SetMarkerSize(0.8)
 graphmede.SetMarkerStyle(21)
 graphmede.Draw("AP")
 newCanvas.SaveAs("smoothTest.pdf")

 for i,mass in zip(range(len(EXPmasses)),EXPmasses):

  sm=1.0
  if not options.doRatio:
  # for j,mm in enumerate(allMasses):
  #   if mm==mass: sm = (xSec[j]*br[j])  # since i fitted to absolute, if i want ratio, have to rescale
    sm = GetBR(mass)*GetXsection(mass)

  mediansmooth = medfunc.Eval(mass)

  graphMed.SetPoint(i,mass,mediansmooth*sm)
  graph68.SetPoint(i,mass,mediansmooth*sm)
  graph95.SetPoint(i,mass,mediansmooth*sm)

  diff95_up = abs(mediansmooth - up95func.Eval(mass))*sm
  diff95_dn = abs(mediansmooth - dn95func.Eval(mass))*sm
  diff68_up = abs(mediansmooth - up68func.Eval(mass))*sm
  diff68_dn = abs(mediansmooth - dn68func.Eval(mass))*sm

  graph68.SetPointError(i,0,0,diff68_dn,diff68_up)
  graph95.SetPointError(i,0,0,diff95_dn,diff95_up)

#OBSERVED
for i,mass in zip(range(len(OBSfiles)),OBSmasses):

    sm = 1.;
    if obs[i] ==-1: continue
    if not options.doRatio: sm = GetBR(M)*GetXsection(M)
    graphObs.SetPoint(i,float(mass),obs[i]*sm)
    graphObs.SetPointError(i,0,0,0,0)
    print "mass - ", mass    
    print "		Obs - ", obs[i]
		
		


#-------------------------------------------------------------------------
# Construct the theory bands

theoryArrays = []
theoryPlusArrays = []
theoryMinusArrays = []

dPlusArrays = []
dMinusArrays = []

xSecErrPlus = array.array('d',[0.0]*len(allMasses))
xSecErrMinus = array.array('d',[0.0]*len(allMasses))
dPlus = array.array('d',[0.0]*len(allMasses))

for th in theorySMScales:
  theoryArrays.append(array.array('d',[0.0]*len(allMasses)))
  theoryPlusArrays.append(array.array('d',[0.0]*len(allMasses)))
  theoryMinusArrays.append(array.array('d',[0.0]*len(allMasses)))
  dPlusArrays.append(array.array('d',[0.0]*len(allMasses)))
  dMinusArrays.append(array.array('d',[0.0]*len(allMasses)))


for i in range(len(allMasses)):
    xSecErrPlus[i] =  xSec[i] +  xSec[i]*xSecErrPlusPercent[i]/100.;
    xSecErrMinus[i] = xSec[i] -  xSec[i]*xSecErrMinusPercent[i]/100.;  

    if options.doRatio: xSec[i]=1.;
    else: xSec[i]=xSec[i]*br[i];

    xSecErrPlus[i] = xSecErrPlus[i]*br[i];
    xSecErrMinus[i]= xSecErrMinus[i]*br[i];

    for j,th in enumerate(theorySMScales):
      theoryArrays[j][i]=th*xSec[i];
      theoryPlusArrays[j][i]=th*xSecErrPlus[i];
      theoryMinusArrays[j][i]=th*xSecErrMinus[i];
      dPlusArrays[j][i]=abs(th*xSecErrPlus[i]-th*xSec[i])
      dMinusArrays[j][i]=abs(th*xSecErrMinus[i]-th*xSec[i])
    
  
myGraphXSecSM   = ROOT.TGraphAsymmErrors()
myGraphXSecSMScales = []
for th in theorySMScales:
  myGraphXSecSMScales.append(ROOT.TGraphAsymmErrors()) 
  
for i in range(len(allMasses)):
    myGraphXSecSM.SetPoint(i,allMasses[i],xSec[i])
    for j in range(len(theorySMScales)):
      myGraphXSecSMScales[j].SetPoint(i,allMasses[i],theoryArrays[j][i])

#---------------------------------------------------------------------------
#graph95.Smooth(len(EXPmasses))
#graph68.Smooth(len(EXPmasses))
#graphMed.Smooth(len(EXPmasses))
# ---------------------------------
graph95.SetFillColor(FILLCOLOR_95)
graph95.SetFillStyle(FILLSTYLE)
graph68.SetFillColor(FILLCOLOR_68)
graph68.SetFillStyle(FILLSTYLE)
graphMed.SetLineStyle(2)
graphMed.SetLineColor(2)
graphMed.SetMarkerColor(2)
graphMed.SetLineWidth(3)
graphObs.SetLineWidth(3)
if options.bayes:
 bayesObs.SetLineWidth(3)
 bayesObs.SetLineColor(4)
 bayesObs.SetMarkerColor(4)
 bayesObs.SetLineStyle(7)
graphOne.SetLineWidth(3)

graphObs.SetMarkerStyle(20)
graphObs.SetMarkerSize(2.0)
graphObs.SetLineColor(1)


myGraphXSecSM.SetLineStyle(1)
myGraphXSecSM.SetLineColor(FILLCOLOR_T)
myGraphXSecSM.SetLineWidth(2)
myGraphXSecSM.SetFillColor(FILLCOLOR_T)
myGraphXSecSM.SetMarkerColor(FILLCOLOR_T)
myGraphXSecSM.SetFillStyle(SMFILLSTYLE)

MG.Add(graph95)
MG.Add(graph68)
MG.Add(graphMed)
#MG.Add(baseline)
MG.Add(myGraphXSecSM)

if not options.doRatio:
 for j in range(len(theorySMScales)):
  myGraphXSecSMScales[j].SetLineStyle(1)
  myGraphXSecSMScales[j].SetLineColor(FILLCOLOR_T)
  myGraphXSecSMScales[j].SetMarkerColor(FILLCOLOR_T)
  myGraphXSecSMScales[j].SetLineWidth(2)
  myGraphXSecSMScales[j].SetFillColor(FILLCOLOR_T)
  myGraphXSecSMScales[j].SetFillStyle(SMFILLSTYLE)
  MG.Add(myGraphXSecSMScales[j])

# extend the dummy graph so that the limit plot has extra pieces.
#dummyGraph.SetPoint(1,min(OBSmasses)-OFFSETLOW,0)
#dummyGraph.SetPoint(1,max(OBSmasses)+OFFSETHIGH,0)

#MG.Add(dummyGraph)

if not options.expectedOnly:
  MG.Add(graphObs)
#  MG.Add(baselineObs)
  if options.bayes:
   MG.Add(bayesObs)

# -------------------------------------
C = ROOT.TCanvas("#int L = %s"%intlumi,"#int L = %s"%intlumi,1600,1100)
#C.SetLogy()
C.SetGrid(True)

dummyHist = ROOT.TH1D("dummy","",1,min(OBSmasses)-OFFSETLOW,max(OBSmasses)+OFFSETHIGH)
dummyHist.Draw("AXIS")
MG.Draw("L3")
dummyHist.Draw("AXIGSAME")

#MG.GetXaxis().SetTitle("m_{H}(GeV/c^{2})")
#MG.GetXaxis().SetRangeUser(min(OBSmasses)-OFFSETLOW,max(OBSmasses)+OFFSETHIGH)
dummyHist.GetXaxis().SetTitle("m_{H}(GeV)")
dummyHist.GetXaxis().SetRangeUser(min(OBSmasses)-OFFSETLOW,max(OBSmasses)+OFFSETHIGH)
if options.doRatio:
 #MG.GetYaxis().SetRangeUser(0.0,14.8)
 #MG.GetYaxis().SetTitle("\sigma_{H}xBR(H#rightarrow #gamma #gamma) / %s - 95%% CL"%extraString)
 dummyHist.GetYaxis().SetRangeUser(RANGEYRAT[0],RANGEYRAT[1])
 dummyHist.GetYaxis().SetTitle("\sigma(H#rightarrow #gamma #gamma)_{95%%CL} / \sigma(H#rightarrow #gamma #gamma)_{%s}"%extraString)
else: 
 #MG.GetYaxis().SetRangeUser(0.0,0.6)
 #MG.GetYaxis().SetTitle("\sigma_{H}xBR(H#rightarrow #gamma #gamma) - 95% CL")
 dummyHist.GetYaxis().SetRangeUser(RANGEYABS[0],RANGEYABS[1])
 dummyHist.GetYaxis().SetTitle("\sigma #times BR(H#rightarrow #gamma #gamma)_{95%CL} (pb)")

MG.SetTitle("#int L = %s"%intlumi)
mytext = ROOT.TLatex()
mytext.SetTextSize(0.04)
SMEnd = myGraphXSecSM.Eval(max(OBSmasses))
if not options.doRatio:
 mytext.DrawLatex(max(OBSmasses)+.3,SMEnd,"#sigma_{%s}"%extraString)
 for th in theorySMScales:
  mytext.DrawLatex(max(OBSmasses)+0.3,th*SMEnd,"%d#times#sigma_{%s}"%(th,extraString))

mytext.SetNDC()
mytext.DrawLatex(0.54,0.8,"#splitline{CMS preliminary}{#sqrt{s} = 7 TeV L = %.2f fb^{-1}}"%float(intlumi))
leg.Draw()

#Make a bunch of extensions to the plots
if options.doRatio:
 C.SaveAs(options.path+"limit_%s_%s_ratio.pdf"%(args[1],Method))
 C.SaveAs(options.path+"limit_%s_%s_ratio.png"%(args[1],Method))
 C.SaveAs(options.path+"limit_%s_%s_ratio.gif"%(args[1],Method))
 C.SaveAs(options.path+"limit_%s_%s_ratio.eps"%(args[1],Method))
 C.SaveAs(options.path+"limit_%s_%s_ratio.ps"%(args[1],Method))
else:
 C.SaveAs(options.path+"limit_%s_%s.pdf"%(args[1],Method))
 C.SaveAs(options.path+"limit_%s_%s.png"%(args[1],Method))
 C.SaveAs(options.path+"limit_%s_%s.gif"%(args[1],Method))
 C.SaveAs(options.path+"limit_%s_%s.eps"%(args[1],Method))
 C.SaveAs(options.path+"limit_%s_%s.ps"%(args[1],Method))

if options.outputLimits:
  print "Writing Limits To ROOT file"
  OUTTgraphs = ROOT.TFile(options.outputLimits,"RECREATE")
  graphMed.SetName("median")
  graphMed.Write()
  graphObs.SetName("observed")
  graphObs.Write()
  OUTTgraphs.Write()


