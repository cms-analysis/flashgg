import ROOT
import os,numpy,sys,math,array
from optparse import OptionParser
parser = OptionParser()
parser.add_option("-i","--input",dest="tfileName")
parser.add_option("-m","--mass",dest="mass",type="float")
parser.add_option("-L","--lumistr",dest="lumistr",type="str",default="19.6")
parser.add_option("-S","--sqrts",dest="sqrts",type="int",default=8)
parser.add_option("","--noVbfTag",dest="includeVBF",default=True,action="store_false")

(options,args)=parser.parse_args()
options.splitSignal=False

ROOT.gROOT.SetBatch(1)
ROOT.gROOT.SetStyle("Plain")
ROOT.gStyle.SetOptStat(0)

# make defaults
# PLOT OPS ----------------
lumistring = options.lumistr+ " fb^{-1}"
sigscale   = 1.
plotOutDir="./"
mass=float(options.mass)
fNew =0
fNew2=0

is2011 = options.sqrts==7

if is2011:
 Bins_vbf  = [1,2]
 Bins_vh   = [3,4,5,6,7]
 nBins_vbf = len(Bins_vbf)
 nBins_vh  = len(Bins_vh)
else:
 Bins_vbf  = [1,2,3]
 Bins_vh   = [4,5,6,7,8,9]
 nBins_vbf = len(Bins_vbf)
 nBins_vh  = len(Bins_vh)

# Now the bins which are for the plots, assume 1 is the first non-incl bin 
if is2011:
 pl_VBF_bins = [1,2]
 pl_VH_bins  = [3,4,5,7]
 pl_ttH_bins = [6]
else:
 pl_VBF_bins = [1,2,3]
 pl_VH_bins  = [4,5,6,9]
 pl_ttH_bins = [7,8]

def plainBin(hist):
	nb = hist.GetNbinsX()
	h2 = ROOT.TH1F(hist.GetName()+"new","",nb,0,nb)
	numInclBins=h2.GetNbinsX()-nBins_vbf-nBins_vh
	for i in range (1,nb+1):
		h2.SetBinContent(i,hist.GetBinContent(i))
		h2.SetBinError(i,hist.GetBinError(i))
		if i<=numInclBins:
		  h2.GetXaxis().SetBinLabel(i,"Inc. Bin %d "%(i))
		elif i-numInclBins in pl_VBF_bins: 
		  h2.GetXaxis().SetBinLabel(i,"Di-jet ")
		elif i-numInclBins in pl_VH_bins: 
		  h2.GetXaxis().SetBinLabel(i,"VH tag ")
		elif i-numInclBins in pl_ttH_bins: 
		  h2.GetXaxis().SetBinLabel(i,"ttH tag ")

	h2.GetXaxis().SetNdivisions(nb)
	h2.GetYaxis().SetTitle("Events / BDT Bin")
	h2.GetXaxis().SetLabelSize(0.055)
	h2.GetYaxis().SetLabelSize(0.05)
	h2.GetYaxis().SetTitleSize(0.05)
	h2.GetYaxis().SetTitleOffset(1.)
	h2.GetXaxis().SetLabelFont(42)
	h2.GetYaxis().SetLabelFont(42)
	h2.GetYaxis().SetTitleFont(42)
	h2.GetXaxis().SetTitleFont(42)
	return h2

def plotDistributions(mass,data,signals,bkg,errors):

	if options.splitSignal: # last signal is separated off
	  for i in range(1,len(signals)-1):
		signals[0].Add(signals[i])
	else:
	  for i in range(1,len(signals)):
		signals[0].Add(signals[i])

	nbins = data.GetNbinsX()

	flatdata    = plainBin(data)
	flatsignal  = plainBin(signals[0])
	flatsignal1 = plainBin(signals[-1])

	flatbkg  = plainBin(bkg);flatbkg.SetLineColor(4);flatbkg.SetLineWidth(2)

	fNew  = flatbkg.Clone("sig1bkg")
	fNew2 = flatbkg.Clone("sig2bkg")

	flatdata.SetMarkerStyle(20);flatdata.SetMarkerSize(1.0)
		
	fNew.SetLineColor(1);fNew.SetLineWidth(2);fNew.SetFillStyle(1001);fNew.SetFillColor(3)
	fNew2.SetFillColor(5);fNew2.SetLineColor(1);fNew2.SetLineWidth(2);fNew2.SetFillStyle(1001)

	fNewT = fNew.Clone();fNew2T = fNew2.Clone();fNewT.SetFillStyle(1001);fNew2T.SetFillStyle(1001)

	flatsignal.SetLineWidth(2);flatsignal.SetLineColor(ROOT.kRed);flatsignal.Scale(sigscale)
	flatsignal1.SetLineWidth(2);flatsignal1.SetLineColor(ROOT.kGreen+4);flatsignal1.Scale(sigscale)
		
	leg = ROOT.TLegend(0.5,0.55,0.88,0.88);leg.SetFillColor(0);leg.SetBorderSize(0)
	leg.SetTextFont(42)

	for b in range(1,nbins+1):
		additional = errors[b-1]
  		fNew.SetBinError(b,((fNew.GetBinError(b)**2)+((fNew.GetBinContent(b)*additional)**2))**0.5)
  		fNew2.SetBinError(b,2*(((fNew2.GetBinError(b)**2)+((fNew2.GetBinContent(b)*additional)**2))**0.5))
	c = ROOT.TCanvas();c.SetLogy()
	if (not options.includeVBF): flatdata.GetXaxis().SetTitle("Category")
	flatdata.Draw("9");fNew2.Draw("9sameE2");fNew.Draw("9sameE2");flatbkg.Draw("9samehist")
	if options.splitSignal: 
	  sigst = ROOT.THStack();sigst.Add(flatsignal);sigst.Add(flatsignal1);sigst.Draw("9samehist")
	else:  flatsignal.Draw("9samehist")
	flatdata.Draw("9sameP");flatdata.SetMinimum(0.1);flatdata.SetMaximum(20*flatdata.Integral())

	leg.AddEntry(flatdata,"Data","PLE")
	if options.splitSignal:
	  leg.AddEntry(flatsignal,"Higgs (GG,WZ,TT), m_{H}=%3.1f GeV (x%d)"%(mass,int(sigscale)) ,"L")
	  leg.AddEntry(flatsignal1,"Higgs, m_{H}=%3.1f GeV (x%d)"%(mass,int(sigscale)) ,"L")

	else: leg.AddEntry(flatsignal,"Higgs, m_{H}=%3.1f GeV"%(mass) ,"L")
	leg.AddEntry(flatbkg,"Background","L");leg.AddEntry(fNewT,"\pm 1\sigma","F");leg.AddEntry(fNew2T,"\pm 2\sigma","F")
	leg.Draw()
	mytext = ROOT.TLatex();mytext.SetTextSize(0.05);mytext.SetNDC();mytext.SetTextFont(42);mytext.DrawLatex(0.15,0.82,"CMS preliminary");
	mytext.SetTextSize(0.05)
	mytext.DrawLatex(0.15,0.75,"#sqrt{s} = %d TeV L = %s"%(options.sqrts,lumistring))
	leg.Draw()
	c.RedrawAxis()
	c.SaveAs(plotOutDir+"/model_m%3.1f.pdf"%mass);c.SaveAs(plotOutDir+"/model_m%3.1f.png"%mass);c.SaveAs(plotOutDir+"/model_m%d.C"%mass)
	
	d = ROOT.TCanvas()
	leg2 = ROOT.TLegend(0.56,0.56,0.88,0.88)
	leg2.SetFillColor(0);leg2.SetBorderSize(0)
	if (not options.includeVBF): flatdata.GetXaxis().SetTitle("Category")
	flatdata.GetYaxis().SetTitle("Data - Background");
	datErrs = []
	for b in range(1,nbins+1): datErrs.append((flatdata.GetBinContent(b))**0.5);
	flatdata.Add(flatbkg,-1)
	for b in range(1,nbins+1): 
		flatdata.SetBinError(b,((datErrs[b-1]*datErrs[b-1]) +(fNew.GetBinError(b)*fNew.GetBinError(b)))**0.5 )
	flatbkg.Add(flatbkg,-1)

	flatdata.Draw("9");flatbkg.Draw("9samehist")
	if options.splitSignal: 
	  sigst = ROOT.THStack();sigst.Add(flatsignal);sigst.Add(flatsignal1)
	  sigst.Draw("9samehist")
	else:
	  flatsignal.Draw("9samehist")
	flatdata.Draw("9sameP");flatdata.SetMaximum(250);flatdata.SetMinimum(-100)

	leg2.AddEntry(flatdata,"Data","PLE")
	if options.splitSignal:
	  leg2.AddEntry(flatsignal,"Higgs (GG,WZ,TT), m_{H}=%3.0f GeV (x%d)"%(mass,int(sigscale)) ,"L")
	  leg2.AddEntry(flatsignal1,"Higgs, m_{H}=%3.0f GeV (x%d)"%(mass,int(sigscale)) ,"L")

	else: leg2.AddEntry(flatsignal,"Higgs, m_{H}=%3.0f GeV (x%d)"%(mass,int(sigscale)) ,"L")
	leg2.AddEntry(flatbkg,"Background","L")

	mytext.SetTextSize(0.04)

	leg2.Draw()
	mytext = ROOT.TLatex();mytext.SetTextSize(0.03);mytext.SetNDC();mytext.DrawLatex(0.1,0.92,"CMS preliminary,  #sqrt{s} = 7 TeV ");mytext.SetTextSize(0.04)
	mytext.DrawLatex(0.2,0.8,"#int L = %s"%(lumistring))
	d.SaveAs(plotOutDir+"/diff_model_m%3.1f.pdf"%mass);d.SaveAs(plotOutDir+"/diff_model_m%3.1f.png"%mass)
# First read the data-card
tfile = open(options.tfileName,"r")
dcardlines = tfile.readlines()
# we need observation, rate, bkg_norm and massBias lines
infoDict = {"data":[],"rate":[],"bkg_norm":[],"massBias":[],"ncat":int(1),"bkg":[],"ggh":[],"vbf":[],"wzh":[],"tth":[]}
for dl in dcardlines:
	info = dl.split()
	if len(info)<1: continue
	if info[0]=="observation":
		infoDict["data"]=info[1:]
		infoDict["ncat"]=len(info[1:])
	elif info[0]=="rate":
		infoDict["rate"]=info[1:]
	elif "bkg_norm" in info[0]:
		infoDict["bkg_norm"]=info[2:]
	elif "massBias" in info[0]:
		infoDict["massBias"].append(info[2:])

# make some bins
binEdges = [-1.+2*float(n)/infoDict["ncat"] for n in range(infoDict["ncat"]-2)]
binEdges.append(1.)
binEdges.append(1.04)
binEdges.append(1.08)
arrbinEdges = array.array("d",binEdges)
# Now we have everyting to constuct histograms:
ncat = infoDict["ncat"]
dataHist = ROOT.TH1F("dataHist","",ncat,arrbinEdges)
bkgHist  = ROOT.TH1F("bkgHist","",ncat,arrbinEdges)
gghHist  = ROOT.TH1F("gghHist","",ncat,arrbinEdges)
vbfHist  = ROOT.TH1F("vbfHist","",ncat,arrbinEdges)
wzhHist  = ROOT.TH1F("wzhHist","",ncat,arrbinEdges)
tthHist  = ROOT.TH1F("tthHist","",ncat,arrbinEdges)
		
# unfold the signals + bkg rates
for b in range(infoDict["ncat"]):
	infoDict["ggh"].append(infoDict["rate"][b*5+0])
	infoDict["vbf"].append(infoDict["rate"][b*5+1])
	infoDict["wzh"].append(infoDict["rate"][b*5+2])
	infoDict["tth"].append(infoDict["rate"][b*5+3])
	infoDict["bkg"].append(infoDict["rate"][b*5+4])

for b in range(infoDict["ncat"]):
	dataHist.SetBinContent(b+1,float(infoDict["data"][b]))	
	bkgHist.SetBinContent(b+1,float(infoDict["bkg"][b]))	
	gghHist.SetBinContent(b+1,float(infoDict["ggh"][b]))	
	vbfHist.SetBinContent(b+1,float(infoDict["vbf"][b]))	
	wzhHist.SetBinContent(b+1,float(infoDict["wzh"][b]))	
	tthHist.SetBinContent(b+1,float(infoDict["tth"][b]))

# final thing is to make the errors
binErrors=[0 for b in range(infoDict["ncat"])]
for mb in infoDict["massBias"]:
  
  for b in range(infoDict["ncat"]):
	errVal = mb[b*5+4]
	if errVal!="-":
		errsc = float((errVal.split("/"))[1])-1
		err = float(infoDict["bkg"][b])*errsc
		binErrors[b]+=(err*err)

for b in range(infoDict["ncat"]): 
	bkgHist.SetBinError(b+1,binErrors[b]**0.5)

#finall norm errors
normErrors = []
for b in range(infoDict["ncat"]): 
	nerror = ((infoDict["bkg_norm"][b*5+4]).split("/"))[1]
	normErrors.append(float(nerror)-1)
# Can finally make plots
plotDistributions(options.mass,dataHist.Clone("dataClone"),[gghHist.Clone("gghClone"),wzhHist.Clone("wzhClone"),tthHist.Clone("tthClone"),vbfHist.Clone("vbfClone")],bkgHist.Clone("bkgClone"),normErrors)
