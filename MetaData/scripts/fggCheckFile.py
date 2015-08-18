#!/usr/bin/env python

import sys
import json

def main(args):
    import ROOT
    
    fName = args[0]
    jsonName = args[1]
    if fName.startswith("/store"):
        prepend = ["root://eoscms//eos/cms","root://xrootd-cms.infn.it/"]
    else:
        prepend = [""]
        
    for pp in prepend:
        fin = ROOT.TFile.Open("%s/%s" % (pp, fName))
        if fin and not fin.IsZombie():
            break
    if not fin or fin.IsZombie():
        sys.exit(1)
    
    lumi = fin.Get("LuminosityBlocks")
    totWeights = ROOT.TH1D("totWeights","totWeights", 1,0,2)
    ne = lumi.Draw("1>>totEvents(1,0,2)","edmMergeableCounter_eventCount__FLASHggMicroAOD.obj.value","goff")
    ## ne = lumi.Draw("1>>totWeights","Sum$(floatedmMergeableHisto_weightsCount_obsPileup_FLASHggMicroAOD.obj.values[])","goff")
    ne = lumi.Draw("1>>totWeights","edmMergeableDouble_weightsCount_totalWeight_FLASHggMicroAOD.obj.value","goff")
    if  ne>0:
        totEvents = ROOT.gDirectory.Get("totEvents").Integral()
        totWeights = ROOT.gDirectory.Get("totWeights").Integral()
    else:
        events = fin.Get("Events")
        totEvents = events.GetEntries()
        ne = events.Draw("1>>totWeights","GenEventInfoProduct_generator__GEN.obj.weights_[0]","goff")
        if ne < 0:
            ne = events.Draw("1>>totWeights","GenEventInfoProduct_generator__SIM.obj.weights_[0]","goff")
        if ne > 0:
            totWeights = ROOT.gDirectory.Get("totWeights").Integral()
        else:
            totWeights = 0.

    ## FIXME: add PU integrationx
    

    output = { "nevents" : int(totEvents),
               "weights" : totWeights
               }
    
    jf = open(jsonName,"w+")
    jf.write(json.dumps(output))
    jf.close()
    sys.exit(0)

if __name__ == "__main__":
    main(sys.argv[1:])
