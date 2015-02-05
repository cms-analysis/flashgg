import ROOT
import sys

files = sys.argv[2:]
#files = []
#for f in range(115,130):
#for f in range(115,150):
#if f <150: 
#	files.append("higgsCombineTest.ProfileLikelihood.mH%d.root"%(int(f)))
#	files.append("higgsCombineTest.ProfileLikelihood.mH%d.5.root"%(int(f)))
#else:
#	files.append("higgsCombineTest.ProfileLikelihood.mH%d.root"%(int(f)))

ROOT.gROOT.ProcessLine( \
   "struct Entry{	\
    double r;		\
    double mh;		\
   };"
)
from ROOT import Entry

def getMass(file):
  try:
   tree = file.Get("limit")
   if ROOT.AddressOf(tree) == 0: return -1
  except:
   return -1
  br = tree.GetBranch("mh")
  c = Entry()
  br.SetAddress(ROOT.AddressOf(c,'mh'))
  tree.GetEntry(0)
  return c.mh


def getOBSERVED(file,entry=0):
  try:
   tree = file.Get("limit")
   if ROOT.AddressOf(tree) == 0: return -1,-1
  except:
   return -1,-1
  br = tree.GetBranch("limit")
  m = tree.GetBranch("mh")
  c = Entry()
  br.SetAddress(ROOT.AddressOf(c,'r'))
  m.SetAddress(ROOT.AddressOf(c,'mh'))
  tree.GetEntry(entry)	
  return c.r,c.mh

files.sort()

gr = ROOT.TGraphAsymmErrors()

F = [ROOT.TFile(f) for f in files]
pts = {}
for i,f in enumerate(F):
	#U,M= getOBSERVED(f,1)
	#D,M= getOBSERVED(f,0)
	#P,M= getOBSERVED(f,2)
	P,M= getOBSERVED(f,0)
        #	M = getMass(f)
        if M != -1:
          pts [M] = P
          ## gr.SetPoint(gr.GetN(),M,P)
	#gr.SetPointError(i,0,0,abs(D-P),abs(U-P))
	
	print i,M,P

keys = pts.keys()
keys.sort()

for k in keys:
  gr.SetPoint(gr.GetN(),k,pts[k])

out = ROOT.TFile(sys.argv[1],"RECREATE")
out.cd()
gr.SetName("observed")
gr.Write()


	
