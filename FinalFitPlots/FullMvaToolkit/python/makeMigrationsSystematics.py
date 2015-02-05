# Script for generating additional variation templates for other uncertainties
# Will append histograms to file
import array
from optparse import OptionParser
parser = OptionParser()
parser.add_option("-i","--input",dest="input",help="Input file")
parser.add_option("-o","--output",dest="ofileName",help="Output file (to be updated)")
parser.add_option("-m","--mapFile",dest="mapFile",help="Category map for the inclusive bins")
parser.add_option("-s","--sqrts",dest="sqrts",help="Set the COM energy", type="int", default=8)
(options,args)=parser.parse_args()

# Config 
mH = 125

import ROOT
ROOT.gROOT.SetStyle("Plain")
ROOT.gROOT.SetBatch(True)

# First, upen the file to be updated
ofile = ROOT.TFile(options.ofileName, "UPDATE")
ifile = ROOT.TFile(options.input)
mfile = ROOT.TFile(options.mapFile)

# set sqrts
ext = "%dTeV"%options.sqrts

# Treenames
base = "full_mva_trees"
#processes = ["ggh","vbf","wzh","tth"] # can include all for E_res/scale systematics
processes = ["ggh","vbf"]
systematics =["pdfWeight_QCDscale"]
for pdf_i in range(1,27):
 systematics.append("pdfWeight_pdfset%d"%pdf_i)

# Get the tree file 
cmap  = mfile.Get("Category_Map")
bmap  = mfile.Get("Bin_Edges")
nCats = bmap.GetNbinsX()
barr  = array.array("d",[bmap.GetBinLowEdge(be) for be in range(1,nCats+2)])
bdtmin=  cmap.GetXaxis().GetXmin()

# BEGIN 
print "Producing Migrations from .."
print " sys file = ", options.input
print " using cat-map ", options.mapFile
print " found %d inclusive cats in map."%(nCats)

def FillHist(hc,hmap,systematics,tree,map):
 
    n = tree.GetEntries()

    for evt in range(n):   
     tree.GetEntry(evt)
     if evt%100000 == 0 : print " ..event %d/%d"%(evt,n)

     mass    = getattr(tree,"mass")
     bdt     = getattr(tree,"bdtoutput")
     w	     = getattr(tree,"weight")
     dmom    = float(mass-mH)/mH

     if bdt < bdtmin: continue	
     if dmom > 0.02:  continue
     binmap  = map.FindBin(bdt,dmom)
     bin 	= map.GetBinContent(binmap)
     hc.Fill(bin,w)
          
     for hl,sys in zip(hmap,systematics):
      for typ in ["Up","Down"]:
	if   typ=="Up"  : h = hl[0]
	elif typ=="Down": h = hl[1]

	mass    = getattr(tree,"mass_%s_%s"%(sys,typ))
	bdt     = getattr(tree,"bdtoutput_%s_%s"%(sys,typ))
	w	= getattr(tree,"weight_%s_%s"%(sys,typ))
	dmom    = float(mass-mH)/mH

	if bdt < bdtmin: continue	
	if dmom > 0.02:  continue
	binmap  = map.FindBin(bdt,dmom)
	bin 	= map.GetBinContent(binmap)
	h.Fill(bin,w)

for proc in processes:
  tree = ifile.Get("%s/%s_m%d_%s"%(base,proc,mH,ext))
  hcentral = ROOT.TH1F("th1f_sig_grad_%s_%3.1f_central"%(proc,float(mH)),"central",nCats,barr)

  hmap = []
  for sys in systematics:
    hu = ROOT.TH1F("th1f_sig_grad_%s_%3.1f_%sUp01_sigma"%(proc,float(mH),sys),sys,nCats,barr)
    hd = ROOT.TH1F("th1f_sig_grad_%s_%3.1f_%sDown01_sigma"%(proc,float(mH),sys),sys,nCats,barr)
    hmap.append([hu.Clone(),hd.Clone()])

  FillHist(hcentral,hmap,systematics,tree,cmap)
  ofile.WriteTObject(hcentral)
  for h in hmap:
    ofile.WriteTObject(h[0])
    ofile.WriteTObject(h[1])
    
print ".. systematics saved in ", options.ofileName

# Done

