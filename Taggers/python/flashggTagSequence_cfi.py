import FWCore.ParameterSet.Config as cms
from flashgg.Taggers.flashggDiPhotonMVA_cfi import flashggDiPhotonMVA
from flashgg.Taggers.flashggVBFMVA_cff import flashggVBFMVA,flashggVBFDiPhoDiJetMVA
from flashgg.Taggers.flashggTags_cff import *
from flashgg.Taggers.flashggTagSorter_cfi import flashggTagSorter

flashggTagSequence = cms.Sequence(flashggDiPhotonMVA
                                  * flashggVBFMVA
                                  * flashggVBFDiPhoDiJetMVA
                                  * (flashggUntaggedCategory
                                     + flashggVBFTag
                                     + flashggTTHLeptonicTag
                                     + flashggTTHHadronicTag
                                     + flashggVHLooseTag
                                     + flashggVHTightTag
                                     + flashggVHHadronicTag)
                                  * flashggTagSorter
                                  )

