import FWCore.ParameterSet.Config as cms

from globalVariables_cff import globalVariables

tagCandidateDumpConfig = cms.PSet(
    className  = cms.untracked.string(""),
    src = cms.InputTag(""),    
    generatorInfo = cms.InputTag("generator"),
    processId = cms.string(""),
    processIndex = cms.int32(50),
    maxCandPerEvent = cms.int32(-1),
    lumiWeight = cms.double(1.0),# Over-written by metaData if using fggRunJobs
    intLumi = cms.untracked.double(1000.), # in /pb. to be over-written by metaData if using fggRunJobs
    classifierCfg = cms.PSet(categories = cms.VPSet()),
    categories = cms.VPSet(),
    
    workspaceName = cms.untracked.string("cms_hgg_$SQRTS"),
    nameTemplate = cms.untracked.string("$PROCESS_$SQRTS_$CLASSNAME_$SUBCAT_$LABEL"),
    
    dumpHistos = cms.untracked.bool(False),
    dumpWorkspace = cms.untracked.bool(False),
    dumpTrees = cms.untracked.bool(True),
    
    quietRooFit = cms.untracked.bool(False),
    dumpGlobalVariables = cms.untracked.bool(True),
    globalVariables = globalVariables
)
