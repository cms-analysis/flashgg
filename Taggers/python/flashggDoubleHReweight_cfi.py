import FWCore.ParameterSet.Config as cms

flashggDoubleHReweight = cms.EDProducer("FlashggDoubleHRewighter",
                                       GenParticleTag = cms.InputTag( "flashggPrunedGenParticles" ), # to compute MC-truth info
                                       targetNode = cms.int32(-1), 
                                   	   fullGridWeightsFile=cms.untracked.FileInPath("flashgg/Taggers/data/HHTagger/weights_v1_1507_points.root"),
                                   	   benchmarksWeightsFile=cms.untracked.FileInPath("flashgg/Taggers/data/HHTagger/weights_v3_bench12_points.root") 
                                       )
