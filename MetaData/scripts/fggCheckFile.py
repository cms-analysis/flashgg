#!/usr/bin/env python

import sys
import json

def main(args):
    import ROOT
    
    fName = args[0]
    jsonName = args[1]
    if fName.startswith("/store"):
        prepend = ["root://eoscms.cern.ch//eos/cms","root://xrootd-cms.infn.it/"]
    else:
        prepend = [""]
        
    for pp in prepend:
        fin = ROOT.TFile.Open("%s/%s" % (pp, fName))
        if fin and not fin.IsZombie():
            break
    if not fin or fin.IsZombie():
        sys.exit(1)

    events = fin.Get("Events")
    isData=False
    if events.GetEntriesFast() > 0:
        events.GetEntry(0)
        isData=events.EventAuxiliary.isRealData()
    elif "Run20" in fName: # try to guess from file name
        isData=True

    if not isData:
        lumi = fin.Get("LuminosityBlocks")
        ne = lumi.Draw("1>>totEvents(1,0,2)","edmMergeableCounter_eventCount__FLASHggMicroAOD.obj.value","goff")
        ne = lumi.Draw("1>>totWeights(1,0,2)","edmMergeableDouble_weightsCount_totalWeight_FLASHggMicroAOD.obj.value","goff")
        if  ne>0:
            totEvents = ROOT.gDirectory.Get("totEvents").Integral()
            totWeights = ROOT.gDirectory.Get("totWeights").Integral()
        else:
            totEvents = events.GetEntries()
            ne = events.Draw("1>>totWeights(1,0,2)","GenEventInfoProduct_generator__GEN.obj.weights_[0]","goff")
            if ne < 0:
                ne = events.Draw("1>>totWeights(1,0,2)","GenEventInfoProduct_generator__SIM.obj.weights_[0]","goff")
            if ne > 0:
                totWeights = ROOT.gDirectory.Get("totWeights").Integral()
            else:
                totWeights = 0.
        lumi.GetEntry(0)
        minpu = lumi.floatedmMergeableHisto_weightsCount_truePileup_FLASHggMicroAOD.min
        maxpu = lumi.floatedmMergeableHisto_weightsCount_truePileup_FLASHggMicroAOD.max
        npu   = lumi.floatedmMergeableHisto_weightsCount_truePileup_FLASHggMicroAOD.values.size()
        print minpu, maxpu, npu
        puhisto = [ 0. ] * npu
        for il in xrange(lumi.GetEntries()):
            for ibin in xrange(npu):
                puhisto[ibin]  += lumi.floatedmMergeableHisto_weightsCount_truePileup_FLASHggMicroAOD.values[ibin]
        print puhisto
        tot = sum( puhisto )
        print tot
        print map(lambda x: float(x)/float(tot), puhisto)
    else:
        totEvents = events.GetEntriesFast()
        totWeights = totEvents

    output = { "events"    : int(events.GetEntriesFast()),
               "totEvents" : int(totEvents),
               "weights" : totWeights,
               }

    ### get list of processed lumi sections in data
    if isData:
        from DataFormats.FWLite import Lumis
        runsLumisDict = {}
        print fin.GetName()
        lumis = Lumis ([fin.GetName()])
        delivered = recorded = 0
        for lum in lumis:
            runList = runsLumisDict.setdefault (lum.aux().run(), [])
            runList.append( lum.aux().id().luminosityBlock() )
        output["lumis"] = runsLumisDict
    
    jf = open(jsonName,"w+")
    jf.write(json.dumps(output))
    jf.close()
    sys.exit(0)

if __name__ == "__main__":
    main(sys.argv[1:])
