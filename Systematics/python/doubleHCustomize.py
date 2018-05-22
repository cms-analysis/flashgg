import FWCore.ParameterSet.Config as cms

def variablesToDump(customize):
    return [ "leadingJet_bDis := leadJet().bDiscriminator('pfCombinedInclusiveSecondaryVertexV2BJetTags')",#FIXME make the btag type configurable?
             "subleadingJet_bDis := subleadJet().bDiscriminator('pfCombinedInclusiveSecondaryVertexV2BJetTags')",
             "absCosThetaStar_CS := abs(getCosThetaStar_CS(6500))",#FIXME get energy from somewhere?
             "absCosTheta_bb := abs(CosThetaAngles()[1])",
             "absCosTheta_gg := abs(CosThetaAngles()[0])",
             "diphotonCandidatePtOverdiHiggsM := diphotonPtOverM()",
             "dijetCandidatePtOverdiHiggsM := dijetPtOverM()",
             "customLeadingPhotonIDMVA := diPhoton.leadingView.phoIdMvaWrtChosenVtx",
             "customSubLeadingPhotonIDMVA := diPhoton.subLeadingView.phoIdMvaWrtChosenVtx",
             "leadingPhotonSigOverE := diPhoton.leadingPhoton.sigEOverE",
             "subleadingPhotonSigOverE := diPhoton.subLeadingPhoton.sigEOverE",
             "sigmaMOverMDecorr := sqrt(0.5*(diPhoton.leadingPhoton.sigEOverE*diPhoton.leadingPhoton.sigEOverE + diPhoton.subLeadingPhoton.sigEOverE*diPhoton.subLeadingPhoton.sigEOverE))",
             "PhoJetMinDr := getPhoJetMinDr()",
             "HHbbggMVA := MVA()",
             "HHbbggMX := MX()",
             "category := categoryNumber()"
]

def tagList(customize,process):
    return [ ["DoubleHTag",12] ]#12 is the number of categories?


def customizeTagSequence(customize,process):
    process.load("flashgg.Taggers.flashggDoubleHTag_cff")

    ## customize here (regression, kin-fit, MVA...)
    ## process.flashggTagSequence += process.flashggDoubleHTagSequence
#    pos = process.flashggTagSequence.index(process.flashggTagSorter) - 1 
#    pos = process.flashggTagSequence.index(process.flashggTagSorter) -1  
#    process.flashggTagSequence.insert( pos, process.flashggDoubleHTagSequence )
    ## for step in process.flashggDoubleHTagSequence:
    ##     process.flashggTagSequence.append(step)

    if customize.doubleHTagsOnly:
        process.flashggTagSequence.remove(process.flashggVBFTag)
        process.flashggTagSequence.remove(process.flashggTTHLeptonicTag)
        process.flashggTagSequence.remove(process.flashggTTHHadronicTag)
        process.flashggTagSequence.remove(process.flashggVHEtTag)
        process.flashggTagSequence.remove(process.flashggVHLooseTag)
        process.flashggTagSequence.remove(process.flashggVHTightTag)
        process.flashggTagSequence.remove(process.flashggVHMetTag)
        process.flashggTagSequence.remove(process.flashggWHLeptonicTag)
        process.flashggTagSequence.remove(process.flashggZHLeptonicTag)
        process.flashggTagSequence.remove(process.flashggVHLeptonicLooseTag)
        process.flashggTagSequence.remove(process.flashggVHHadronicTag)
        process.flashggTagSequence.remove(process.flashggVBFMVA)
        process.flashggTagSequence.remove(process.flashggVBFDiPhoDiJetMVA)


        process.flashggTagSequence.replace(process.flashggUntagged, process.flashggDoubleHTagSequence)   



