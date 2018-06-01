import FWCore.ParameterSet.Config as cms

flashggSelectedGenDiPhotons = cms.EDFilter('GenDiPhotonSelector',
                                             src=cms.InputTag('flashggGenDiPhotons'),
                                             cut=cms.string("1"),
                                             )
