import FWCore.ParameterSet.Config as cms

from globalVariables_cff import globalVariables

DiPhotonMVATrainingDumpConfNew = cms.PSet(
		className = cms.untracked.string("CutBasedDiPhotonMVAResultDumper"),
		src = cms.InputTag("flashggDiPhotonMVANew"),
#src = cms.InputTag("flashggPhotons"),
		generatorInfo = cms.InputTag("generator"),
		processId = cms.string(""),
		maxCandPerEvent = cms.int32(1),
		lumiWeight = cms.double(1.0),
		classifierCfg = cms.PSet(categories=cms.VPSet()),
		categories = cms.VPSet(),
		workspaceName = cms.untracked.string("cms_hgg_$SQRTS"),
		nameTemplate = cms.untracked.string("$PROCESS_$SQRTS_$LABEL_$SUBCAT"),
		dumpHistos = cms.untracked.bool(True),
		dumpWorkspace = cms.untracked.bool(False),
		dumpTrees = cms.untracked.bool(False),
		quietRooFit = cms.untracked.bool(False),
		dumpGlobalVariables = cms.untracked.bool(True),
		globalVariables = globalVariables
		)
