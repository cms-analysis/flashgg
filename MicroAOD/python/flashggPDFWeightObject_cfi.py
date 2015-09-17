import FWCore.ParameterSet.Config as cms

flashggPDFWeightObject = cms.EDProducer('FlashggPDFWeightProducer',
		tag = cms.untracked.string("initrwgt"),
		pdfset = cms.untracked.string("PDF_variation"),
		delimiter_1 = cms.untracked.string("id=\""),
		delimiter_2 = cms.untracked.string("\">"),
		delimiter_3 = cms.untracked.string("</weightgroup>"),
		LHEEventTag = cms.untracked.InputTag('externalLHEProducer'),
		)
