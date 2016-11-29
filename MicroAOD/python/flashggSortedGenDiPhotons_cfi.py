import FWCore.ParameterSet.Config as cms

flashggSortedGenDiPhotons = cms.EDFilter('GenDiPhotonSorter',
                                           src=cms.InputTag('flashggSelectedGenDiPhotons'),
                                           maxNumber=cms.uint32(100),
                                           )
