import ROOT
import sys
import numpy

from optparse import OptionParser
parser = OptionParser()
parser.add_option("-i","--input",dest="tFileName")
parser.add_option("-g","--grep",dest="grepString",default="_")
parser.add_option("-a","--antigrep",dest="antigrepString",default="!")
parser.add_option("-N","--norm",dest="norm",action="store_true",default=False)
parser.add_option("-n","--noSort",action="store_false",dest="Sort",default=True)
(options,arg)=parser.parse_args()

filename = options.tFileName
grepBit  = options.grepString
antiGrepBit = options.antigrepString
tfile = ROOT.TFile(filename)

if options.norm:
  ws = tfile.Get("cms_hgg_workspace")
  for m in numpy.arange(110,150.5,0.5):
    var = ws.var("NBkgInSignal_mH%3.1f"%m)
    print "%20s %4.2f"%(var.GetName(),var.getVal())

files=[]
for key in tfile.GetListOfKeys():
  if 'th1f' in key.GetName() and 'VBF' not in key.GetName() and grepBit in key.GetName() and antiGrepBit not in key.GetName():
    th1f = tfile.Get(key.GetName())
    files.append([th1f.GetName(),th1f.GetNbinsX(),th1f.GetEntries(),th1f.Integral()])

if options.Sort: files.sort(key=lambda x: x[0])

for f in files:
	print "%60s %10d %10d %4.4f"%(f[0],f[1],f[2],f[3])
