import FWCore.ParameterSet.Config as cms

flashggPDFWeightObject = cms.EDProducer('FlashggPDFWeightProducer',
		LHEEventTag = cms.InputTag('externalLHEProducer'),
                GenTag      = cms.InputTag('generator'),
		tag = cms.untracked.string("initrwgt"),
                doScaleWeights  = cms.untracked.bool(True),
		nPdfEigWeights = cms.uint32(60),
		mc2hessianCSV = cms.untracked.string(""),
                LHERunLabel = cms.string("externalLHEProducer"),
                Debug = cms.bool(False),
                PDFmap = cms.PSet(#see here https://lhapdf.hepforge.org/pdfsets.html to update the map if needed
                    NNPDF30_lo_as_0130_nf_4 = cms.untracked.uint32(263400),
                    NNPDF31_nnlo_as_0118_nf_4 = cms.untracked.uint32(320900)
                )
	)
