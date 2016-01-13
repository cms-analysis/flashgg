import FWCore.ParameterSet.Config as cms

flashggPDFWeightObject = cms.EDProducer('FlashggPDFWeightProducer',
		tag = cms.untracked.string("initrwgt"),
		nPdfEigWeights = cms.uint32(60),
		pdfset_flag = cms.untracked.bool(False),
		flashgg_flag = cms.untracked.bool(True),
		alpha_s_flag = cms.untracked.bool(True),
		scale_flag = cms.untracked.bool(True),
		pdfset = cms.untracked.string("NNPDF30_lo_as_0130_nf_4.LHgrid"),
		delimiter_1 = cms.untracked.string("id=\""),
		delimiter_2 = cms.untracked.string("\">"),
		LHEEventTag = cms.InputTag('externalLHEProducer'),
                GenTag      = cms.InputTag('generator'),
		delimiter_3 = cms.untracked.string("</weightgroup>"),
		mc2hessianCSV = cms.FileInPath('PhysicsTools/HepMCCandAlgos/data/NNPDF30_lo_as_0130_hessian_60.csv')
	)
