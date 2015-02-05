import ROOT
import sys

files = sys.argv[2:]
#path = sys.argv[2]
#files = []
#for f in range(115,130):
#for f in range(110,151):
#  if f <150: 
#    files.append(path+"higgsCombineTest.MaxLikelihoodFit.mH%d.root"%(int(f)))
#    files.append(path+"higgsCombineTest.MaxLikelihoodFit.mH%d.5.root"%(int(f)))
#  else:
#    files.append(path+"higgsCombineTest.MaxLikelihoodFit.mH%d.root"%(int(f)))

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
  except:
   return -1
  br = tree.GetBranch("limit")
  m = tree.GetBranch("mh")
  c = Entry()
  br.SetAddress(ROOT.AddressOf(c,'r'))
  m.SetAddress(ROOT.AddressOf(c,'mh'))
  tree.GetEntry(entry)	
  return c.r,c.mh

files.sort()

gr = ROOT.TGraphAsymmErrors(len(files))

points=[]

F = [ROOT.TFile(f) for f in files]
for i,f in enumerate(F):
  U,M= getOBSERVED(f,2)
  D,M= getOBSERVED(f,1)
  P,M= getOBSERVED(f,0)
  #P,M= getOBSERVED(f,0)
  #	M = getMass(f)
  gr.SetPoint(i,M,P)
  gr.SetPointError(i,0,0,abs(D-P),abs(U-P))
    
  print i,M,P,D,U
  #print i,M,P

out = ROOT.TFile(sys.argv[1],"RECREATE")
out.cd()
gr.SetName("bestfit")
gr.Write()


	
