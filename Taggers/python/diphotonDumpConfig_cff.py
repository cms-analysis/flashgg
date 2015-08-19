import FWCore.ParameterSet.Config as cms

diphotonDumpConfig = cms.PSet(
    className  = cms.untracked.string("CutAndClassBasedDiPhotonDumper"),
    src = cms.InputTag("flashggDiPhotons"),
    generatorInfo = cms.InputTag("generator"),
    processId = cms.string(""),
    maxCandPerEvent = cms.int32(-1), # -1 turns off the maxCandPerEvent check
    lumiWeight = cms.double(1.0),
    classifierCfg = cms.PSet(categories=cms.VPSet()),
    categories = cms.VPSet(),

    workspaceName = cms.untracked.string("cms_hgg_$SQRTS"),
    nameTemplate = cms.untracked.string("$PROCESS_$SQRTS_$LABEL_$SUBCAT_$SYST"),
    systLabel = cms.untracked.string(""),
    
    dumpHistos = cms.untracked.bool(True),
    dumpWorkspace = cms.untracked.bool(False),
    dumpTrees = cms.untracked.bool(False),
    
    quietRooFit = cms.untracked.bool(False),
    dumpGlobalVariables = cms.untracked.bool(True),
    globalVariables = cms.PSet(
        rho =  cms.InputTag('fixedGridRhoAll'),
        vertexes = cms.InputTag("offlineSlimmedPrimaryVertices"),
        )

)
