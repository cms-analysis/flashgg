import FWCore.ParameterSet.Config as cms

from flashgg.Taggers.globalVariables_cff import globalVariables

HHWWggCandidateDumpConfig = cms.PSet(
    className  = cms.untracked.string("CutBasedHHWWggCandidateDumper"),
    src = cms.InputTag("FlashggHHWWggCandidate"),
    generatorInfo = cms.InputTag("generator"),
    processId = cms.string(""),
    maxCandPerEvent = cms.int32(-1), # -1 turns off the maxCandPerEvent check
    lumiWeight = cms.double(1.0),
    classifierCfg = cms.PSet(categories=cms.VPSet()),
    categories = cms.VPSet(),

    workspaceName = cms.untracked.string("cms_HHWWgg_$SQRTS"),
    # workspaceName = cms.untracked.string("tagsDumper/cms_HHWWgg_$SQRTS"), # Don't know how to put workspace in "tagsDumper"
    nameTemplate = cms.untracked.string("$PROCESS_$SQRTS_$LABEL_$SUBCAT"), # For anything besides Signal 
    # nameTemplate = cms.untracked.string("ggF_125_$SQRTS_$LABEL_$SUBCAT"), # hardcoding SL name for now because don't know how to set $ variables 

    dumpHistos = cms.untracked.bool(False),
    dumpWorkspace = cms.untracked.bool(False),
    dumpTrees = cms.untracked.bool(False),

    quietRooFit = cms.untracked.bool(False),
    dumpGlobalVariables = cms.untracked.bool(True),
    globalVariables=globalVariables
)
