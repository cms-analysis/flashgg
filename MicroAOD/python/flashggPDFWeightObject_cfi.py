import FWCore.ParameterSet.Config as cms

flashggPDFWeightObject = cms.EDProducer('FlashggPDFWeightProducer',
		tag = cms.untracked.string("initrwgt"),
		pdfid_1 = cms.untracked.string("292201"),
		pdfid_2 = cms.untracked.string("292302"),
		delimiter_1 = cms.untracked.string("id=\""),
		delimiter_2 = cms.untracked.string("\">"),
		LHEEventTag = cms.untracked.InputTag('externalLHEProducer'),
		)
