#!/usr/bin/env python

import sys
import ROOT
fin=ROOT.TFile.Open(sys.argv[1])
pu = fin.Get("pileup")
## pileup = map( pu.GetBinContent, xrange(1,pu.GetNbinsX()+1) )
pileup = map( lambda x: pu.GetBinContent(x) / pu.Integral(), xrange(1,pu.GetNbinsX()+1) )
print ",".join(map(lambda x: "%1.4g"%x, pileup))

