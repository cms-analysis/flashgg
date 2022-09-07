import FWCore.ParameterSet.Config as cms

globalVariables = cms.PSet(
    rho =  cms.InputTag('fixedGridRhoAll'),
    vertexes = cms.InputTag("offlineSlimmedPrimaryVertices"),
    
    puReWeight=cms.bool(False),
    puBins=cms.vdouble(),
    dataPu=cms.vdouble(),
    mcPu=cms.vdouble(),
    puInfo=cms.InputTag("slimmedAddPileupInfo"),
    extraFloats=cms.PSet(),
    dumpLHEInfo=cms.bool(False),
    lheTable=cms.InputTag("lheInfoTable:LHE"),
    lhePartTable=cms.InputTag("lheInfoTable:LHEPart"),
    dumpMelaWeightsInfo=cms.bool(False),
    melaTables=cms.VInputTag(),
    )

