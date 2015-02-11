#!/usr/bin/env python

import sys
import json

def main(args):
    import ROOT
    
    fin = ROOT.TFile.Open(args[0])
    if not fin or fin.IsZombie():
        sys.exit(1)
    
    lumi = fin.Get("LuminosityBlocks")
    lumi.Draw("1>>totEvents(1,0,2)","edmMergeableCounter_eventCount__FLASHggMicroAOD.obj.value","goff")
    lumi.Draw("1>>totWeights(1,0,2)","edmMergeableDouble_weightsCount_totalWeight_FLASHggMicroAOD.obj.value","goff")

    ## FIXME: add PU integrationx
    
    totEvents = ROOT.gDirectory.Get("totEvents").Integral()
    totWeights = ROOT.gDirectory.Get("totWeights").Integral()

    output = { "nevents" : int(totEvents),
               "weights" : totWeights
               }
    
    print json.dumps(output)
    sys.exit(0)

if __name__ == "__main__":
    main(sys.argv[1:])
