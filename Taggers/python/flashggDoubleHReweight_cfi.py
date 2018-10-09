import FWCore.ParameterSet.Config as cms

flashggDoubleHReweight = cms.EDProducer("FlashggDoubleHReweighter",
                                       GenParticleTag = cms.InputTag( "flashggPrunedGenParticles" ), # to compute MC-truth info
                                       targetNode = cms.int32(-1), 
                                   	   fullGridWeightsFile=cms.untracked.FileInPath("flashgg/Taggers/data/NonResReWeight/weights_v1_1507_points.root"),
                                   	   benchmarksWeightsFile=cms.untracked.FileInPath("flashgg/Taggers/data/NonResReWeight/weights_v3_bench12_points.root") 
                                       )
