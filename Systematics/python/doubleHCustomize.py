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
             "sigmaMOverM := sqrt(0.5*(diPhoton.leadingPhoton.sigEOverE*diPhoton.leadingPhoton.sigEOverE + diPhoton.subLeadingPhoton.sigEOverE*diPhoton.subLeadingPhoton.sigEOverE))",
             "sigmaMOverMDecorr := getSigmaMDecorr()",
             "sigmaMJets := getSigmaMOverMJets()",
             "PhoJetMinDr := getPhoJetMinDr()",#up to here input variables to MVA
             "HHbbggMVA := MVA()",
             "HHbbggMX := MX()",
             "category := categoryNumber()",

             "leadingPhoton_pt := diPhoton.leadingPhoton.pt",
             "leadingPhoton_eta := diPhoton.leadingPhoton.eta",
             "leadingPhoton_phi := diPhoton.leadingPhoton.phi",
             "subleadingPhoton_pt := diPhoton.subLeadingPhoton.pt",
             "subleadingPhoton_eta := diPhoton.subLeadingPhoton.eta",
             "subleadingPhoton_phi := diPhoton.subLeadingPhoton.phi",

             "leadingJet_pt := leadJet().pt",
             "leadingJet_eta := leadJet().eta",
             "leadingJet_phi := leadJet().phi",
             "leadingJet_bRegNNCorr := leadJet().userFloat('bRegNNCorr')",
             "leadingJet_bRegNNResolution := leadJet().userFloat('bRegNNResolution')",

             "subleadingJet_pt := subleadJet().pt",
             "subleadingJet_eta := subleadJet().eta",
             "subleadingJet_phi := subleadJet().phi",
             "subleadingJet_bRegNNCorr := subleadJet().userFloat('bRegNNCorr')",
             "subleadingJet_bRegNNResolution := subleadJet().userFloat('bRegNNResolution')",

             "ttHMVA_MET := 0",# these variables are needed for ttH killer MVA, which has to be implemented in the producer with another mvaComputer
             "ttHMVA_njets := 0",
             "ttHMVA_Xtt0 :=0 ",
             "ttHMVA_Xtt1 :=0 ",
             "ttHMVA_MjjW0 :=0 ",
             "ttHMVA_MjjW1 :=0 ",
             "ttHMVA_Mjjbt0 :=0 ",
             "ttHMVA_Mjjbt1 :=0 ",
             "ttHMVA_leadingMuon :=0 ",
             "ttHMVA_subleadingMuon :=0 ",
             "ttHMVA_nmus :=0 ",
             "ttHMVA_leadingElectron :=0 ",
             "ttHMVA_subleadingElectron :=0 ",
             "ttHMVA_nelecs :=0 ",
             "ttHHHbggMVA := 0"
             
]

def tagList(customize,process):
    return [ ["DoubleHTag",12] ]#12 is the number of categories?


def customizeTagSequence(customize,process):
    process.load("flashgg.Taggers.flashggDoubleHTag_cff")
    from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag
    process.flashggDoubleHTag.JetTags = cms.VInputTag( ["bRegProducer%d" % icoll for icoll,coll in enumerate(UnpackedJetCollectionVInputTag) ] )
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



