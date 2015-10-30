import FWCore.ParameterSet.Config as cms

flashggPDFWeightObject = cms.EDProducer('FlashggPDFWeightProducer',
		tag = cms.untracked.string("initrwgt"),
		pdfset = cms.untracked.string("292201"),
		delimiter_1 = cms.untracked.string("id=\""),
		delimiter_2 = cms.untracked.string("\">"),
		delimiter_3 = cms.untracked.string("292207"),
		LHEEventTag = cms.untracked.InputTag('externalLHEProducer'),
		)
