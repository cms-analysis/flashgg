import FWCore.ParameterSet.Config as cms

flashggMuons = cms.EDProducer('FlashggMuonProducer',
		muonTag = cms.InputTag('slimmedMuons'),
		pfCandidatesTag = cms.InputTag("packedPFCandidates"),

		muminiso_r_min = cms.double(0.05),
		muminiso_r_max = cms.double(0.2),
		muminiso_kt_scale = cms.double(10.0),
		muminiso_deadcone_ch = cms.double(0.0001),
		muminiso_deadcone_pu = cms.double(0.01),
		muminiso_deadcone_ph = cms.double(0.01),
		muminiso_deadcone_nh = cms.double(0.01),
		muminiso_ptThresh = cms.double(0.5),
		muminiso_ptThresh_phot = cms.double(1.0)
		)

