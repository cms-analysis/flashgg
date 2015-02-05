#!/bin/env python

import sys
import ROOT

ROOT.gSystem.Load("$CMSSW_BASE/lib/$SCRAM_ARCH/libHiggsAnalysisCombinedLimit.so")

fin = ROOT.TFile(sys.argv[1])
fout = ROOT.TFile("CMS-HGG_decorrnuis.root","RECREATE")
ws = fin.Get('wsig_8TeV')
ws.allVars().Print()

for cat in range(0,4):
  scale = ws.var("CMS_hgg_nuissancedeltamcat%d"%cat)
  print scale.GetName(), ' --> ',
  scale.SetName("CMS_hgg_nuissancedeltamcat%d_8TeV"%cat)
  print scale.GetName()
  smear = ws.var("CMS_hgg_nuissancedeltasmearcat%d"%cat)
  print smear.GetName(), ' --> ',
  smear.SetName("CMS_hgg_nuissancedeltasmearcat%d_8TeV"%cat)
  print smear.GetName()

fout.cd()
ws.Write()
print 'New workspace written to ', fout.GetName()
print 'REMEMBER TO CHANGE THE NAMES OF NUISANCES IN THE DATACARD!!!!!'
fout.Close()
fin.Close()
