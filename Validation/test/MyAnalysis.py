#!/usr/bin/env python

from __future__ import print_function
import sys, os, shutil, re, subprocess
import ROOT
from DataFormats.FWLite import Events, Handle
import numpy as np

def main():

  from FWCore.ParameterSet.VarParsing import VarParsing
  
  options = VarParsing ('analysis')
  options.inputFiles = "/afs/cern.ch/work/d/devdatta/CMSREL/Flashgg/CMSSW_9_4_2/src/flashgg/myMicroAODOutputFile.root"
  options.parseArguments()

  events = Events(options.inputFiles)
  nevents = 0
  for event in events:

    h_jets = Handle("std::vector<std::vector<flashgg::Jet> >")
    event.getByLabel("flashggFinalJets", h_jets)

    print( "N(jets) = %i" % len(h_jets.product()) )
    jets = h_jets.product()[0]
    for jet in jets:
      print( "jet pt = %f DeepCSVBDisc = %f" % (jet.pt(), jet.bDiscriminator("pfDeepCSVJetTags:probb")+jet.bDiscriminator("pfDeepCSVJetTags:probbb")) )

    h_dipho = Handle("std::vector<flashgg::DiPhotonCandidate>")
    event.getByLabel("flashggDiPhotons", h_dipho)

    diphos = h_dipho.product()
    print( "N(diphotons) = %i" %len(diphos) )
    for dipho in diphos:
      print( "Diphoton pt = %f" % dipho.pt() )

if __name__ == "__main__":
    main()
