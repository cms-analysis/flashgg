import FWCore.ParameterSet.Config as cms

flashggPDFWeightObject = cms.EDProducer('FlashggPDFWeightProducer',
		tag = cms.untracked.string("initrwgt"),
		nPdfEigWeights = cms.uint32(60),
		delimiter_1 = cms.untracked.string("id=\""),
		delimiter_2 = cms.untracked.string("\">"),
		LHEEventTag = cms.InputTag('externalLHEProducer'),
                GenTag      = cms.InputTag('generator'),
		mc2hessianCSV = cms.FileInPath('PhysicsTools/HepMCCandAlgos/data/NNPDF30_lo_as_0130_hessian_60.csv')
	)
