import ROOT
import sys, os 

ROOT.gROOT.ProcessLine( \
   "struct Entry{	\
    double r;		\
    double mh;		\
   };"
)
from ROOT import Entry
from rootglobestyle import *
setTDRStyle()
ROOT.gStyle.SetOptTitle(0)
ROOT.gStyle.SetOptFit(0)
ROOT.gStyle.SetOptStat(0)

import math

def findMaximum(points):
  
  max = -9999.
  maxMass = 0.
  for i in points:
    if math.isnan(i[0]):
      continue
    if (i[0] > max):
      max = i[0]
      maxMass = i[1]

  return maxMass, max
    
def getMass(file):
  try:
   tree = file.Get("limit")
  except:
   return -1
  br = tree.GetBranch("mh")
  c = Entry()
  br.SetAddress(ROOT.AddressOf(c,'mh'))
  tree.GetEntry(0)
  return c.mh


def getOBSERVED(file,entry=0,ispval=False):
  try:
   tree = file.Get("limit")
   br = tree.GetBranch("limit")
   m = tree.GetBranch("mh")
   c = Entry()
   br.SetAddress(ROOT.AddressOf(c,'r'))
   m.SetAddress(ROOT.AddressOf(c,'mh'))
   tree.GetEntry(entry)
  except:
   return -1,-1

  r = c.r
  if ispval:
    print r,
    if r > 0:
      r = ROOT.RooStats.PValueToSignificance(r)
    else:
      r = 0.
    print r,c.mh
  return r,c.mh

files = sys.argv[2:]
files.sort()
addSubSamples = False
addStatOnly = True

gr = ROOT.TGraphAsymmErrors()

F = [ROOT.TFile(f) for f in files]
p = []
for i,f in enumerate(F):
	P,M = getOBSERVED(f,0,ispval=True)
        p.append((P*P, M))
        ## p.append((P, M))

# sort by mass
p = sorted(p, key=lambda i:i[1])

print p

maxMass, max = findMaximum(p)
print max, maxMass
for y,x in p:
  print x,y
  if( x > 0. ):
    gr.SetPoint(gr.GetN(), x, max - y)


ROOT.gSystem.SetIncludePath("-I$ROOTSYS/include -I$ROOFITSYS/include")
ROOT.gROOT.LoadMacro("%s/GraphToTF1.C+" % os.path.dirname(sys.argv[0]) )

gr.Print()
sp = ROOT.GraphToTF1( "mygraph", gr )
func = ROOT.TF1("myfunc",sp,120,130,1,"GraphToTF1");
func.SetParameter(0,0.)
fit = func

### func = ROOT.TF1("myfunc", "pol3");
### func.SetRange(maxMass -1.5, maxMass +1.5)
### gr.Fit("myfunc", "r+")
### 
### ### a = gr.GetFunction("myfunc").GetParameter(2)
### ### b = gr.GetFunction("myfunc").GetParameter(1)
### ## vtx = -b/(2*a)
### fit = gr.GetFunction("myfunc")

## vtx =fit.GetMinimumX(123, 126)
vtx = 0.
valueAtVtx = 0.
vtx =fit.GetMinimumX(maxMass -1.2, maxMass +1.2)
valueAtVtx =fit.Eval(vtx)

dXP =        fit.GetX(valueAtVtx + 1., vtx, vtx+2) - vtx
dXM = vtx - fit.GetX(valueAtVtx + 1., vtx -2, vtx) 

dX2P =       fit.GetX(valueAtVtx + 4., vtx, vtx + 4) - vtx
dX2M = vtx - fit.GetX(valueAtVtx + 4., vtx - 4, vtx) 

print vtx, valueAtVtx, dXP, dXM, dX2P, dX2M

shift = valueAtVtx
for i in range(gr.GetN()):
  x = gr.GetX()[i]
  y = gr.GetY()[i] - shift 
  gr.SetPoint( i, x, y )
fit.SetParameter(0,fit.GetParameter(0)-shift)
  
print "Best Mass: %3.2f + %3.2f (%3.2f) - %3.2f (%3.2f) "%(vtx, dXP, dX2P, dXM, dX2M)

out = ROOT.TFile(sys.argv[1],"RECREATE")
out.cd()
gr.SetName("observed")
gr.GetXaxis().SetTitle("m_{H} (GeV)")
gr.GetYaxis().SetTitle("-2 #Delta ln L")
gr.GetYaxis().SetRangeUser(-0.5, 10)

gr.GetXaxis().SetRangeUser(122., 128.)
gr.SetMarkerSize(0.6)
gr.SetMarkerStyle(ROOT.kFullCircle)
gr.SetMarkerColor(ROOT.kBlack)

name = sys.argv[1].replace(".root","")
c = ROOT.TCanvas(name,name)
gr.SetLineWidth(2)
gr.Draw("apl")
## fit.Draw("SAME")

lines = [ ROOT.TLine(122., 0, 128., 0),
          ROOT.TLine(122., 1, 128., 1), ROOT.TLine(vtx-dXM,  0, vtx-dXM,  1), ROOT.TLine(vtx+dXP,  0, vtx+dXP,  1), 
          ROOT.TLine(122., 4, 128., 4), ROOT.TLine(vtx-dX2M, 0, vtx-dX2M, 4), ROOT.TLine(vtx+dX2P, 0, vtx+dX2P, 4) ]

for l in lines:
  l.SetLineColor(ROOT.kRed)
  #3 l.SetLineWidth(2)
  l.Draw("SAME")

if addSubSamples:
  f2011 = ROOT.TFile.Open("best_mass_mva_fine_2011.root")
  f2012 = ROOT.TFile.Open("best_mass_mva_fine_2012.root")
  chi2011 = f2011.Get("observed")
  chi2012 = f2012.Get("observed")
  
  chi2011.SetLineColor(ROOT.kGreen+2)
  chi2011.SetLineWidth(2)
  chi2011.SetLineStyle(ROOT.kDashed)
  chi2011.SetTitle("sqrt{s} = 7 TeV data")
  
  chi2012.SetLineColor(ROOT.kBlue+2)
  chi2012.SetLineWidth(2)
  chi2012.SetLineStyle(ROOT.kDashed)
  chi2012.SetTitle("sqrt{s} = 8 TeV data")
  
  chi2011.Draw("L")
  chi2012.Draw("L")

if addStatOnly:
  fstat = ROOT.TFile.Open("hig12028_massfac_mass_v3_1_stat.root")
  chistat = fstat.Get("observed")

  chistat.SetLineWidth(2)
  chistat.SetLineStyle(ROOT.kDashed)

  chistat.Draw("L")

  ### fcorr = ROOT.TFile.Open("test_fully_corr.root")
  ### chicorr = fcorr.Get("observed")
  ### 
  ### chicorr.SetLineColor(ROOT.kBlue+2)
  ### chicorr.SetLineWidth(2)
  ### chicorr.SetLineStyle(ROOT.kDashed)
  ### 
  ### chicorr.Draw("L")

gr.Draw("PL")


mytext = ROOT.TPaveText(0.1828859,0.7334495,0.5167785,0.9337979,"brNDC")
mytext.SetFillColor(0)
mytext.SetLineColor(0)
mytext.SetTextAlign(11)
mytext.SetTextFont(42)

mytext.AddText("CMS preliminary")
mytext.AddText("#sqrt{s} = 7 TeV L = 5.1 fb^{-1}")
mytext.AddText("#sqrt{s} = 8 TeV L = 5.3 fb^{-1}")
mytext.Draw("same")

if addSubSamples:
  leg = ROOT.TLegend(0.5620805,0.7560976,0.8624161,0.8902439)
  leg.SetShadowColor(ROOT.kWhite), leg.SetLineColor(ROOT.kWhite), leg.SetFillColor(ROOT.kWhite), leg.SetTextFont(42)
  leg.AddEntry(chi2011,"#sqrt{s} = 7TeV data","l")
  leg.AddEntry(chi2012,"#sqrt{s} = 8TeV data","l")
  leg.AddEntry(gr,"Combined","pl")
  leg.Draw("same")

#gr.SetGridx(1)
#gr.SetGridy(1)
out.cd()
gr.Write()
fit.Write("fit")

	
for fmt in "C", "png", "pdf":
  c.SaveAs( "%s.%s" % ( c.GetName(), fmt ) )
