import FWCore.ParameterSet.Config as cms

process = cms.Process("FLASHggMicroAOD")

#-------------------- Import the JEC services -----------------------

process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
# from CondCore.DBCommon.CondDBSetup_cfi import *
from CondCore.CondDB.CondDB_cfi import CondDBSetup
process.jec = cms.ESSource("PoolDBESSource",CondDBSetup,
		connect = cms.string("frontier://FrontierPrep/CMS_COND_PHYSICSTOOLS"),
		toGet =  cms.VPSet(
			cms.PSet(record = cms.string("JetCorrectionsRecord"),
				tag = cms.string("JetCorrectorParametersCollection_PHYS14_V2_MC_AK4PFchs"),
				label=cms.untracked.string("AK4PFchs")),
			cms.PSet(record = cms.string("JetCorrectionsRecord"),
				tag = cms.string("JetCorrectorParametersCollection_PHYS14_V2_MC_AK4PF"),
				label=cms.untracked.string("AK4PF")),
			)
		)
process.es_prefer_jec = cms.ESPrefer("PoolDBESSource","jec")
