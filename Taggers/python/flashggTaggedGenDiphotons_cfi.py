import FWCore.ParameterSet.Config as cms

import flashgg.Taggers.flashggDoubleHReweight_cfi as reweight_settings
from flashgg.Taggers.flashggDoubleHReweight_cfi import flashggDoubleHReweight

flashggTaggedGenDiphotons = cms.EDProducer('FlashggTaggedGenDiPhotonProducer',
                                           src=cms.InputTag("flashggGenDiPhotons"),
                                           tags=cms.InputTag("flashggTagSorter"),
                                           GenParticleTag=cms.InputTag("flashggPrunedGenParticles"),
                                           HHbbgg_doReweight = flashggDoubleHReweight.doReweight,
                                           HHbbgg_reweight_producer = cms.string(reweight_settings.reweight_producer),
                                           HHbbgg_reweight_names = cms.vstring(reweight_settings.reweight_names),
                                           ForceGenDiphotonProduction = cms.bool(False)
                                  )
