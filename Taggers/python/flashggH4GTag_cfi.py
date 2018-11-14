
import FWCore.ParameterSet.Config as cms
import flashgg.Taggers.dumperConfigTools as cfgTools
from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag
import flashgg.Taggers.PUJID_wps as pujid
flashggH4GTag = cms.EDProducer('FlashggH4GTagProducer', DiPhotonTag=cms.InputTag('flashggDiPhotons'), MinLeadPhoPt=cms.double(1.0 / 3.0), MinSubleadPhoPt=cms.double(0.25))

