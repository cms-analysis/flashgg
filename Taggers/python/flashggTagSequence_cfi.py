import FWCore.ParameterSet.Config as cms
from flashgg.Taggers.flashggDiPhotonMVA_cfi import flashggDiPhotonMVA
from flashgg.Taggers.flashggVBFMVA_cff import flashggVBFMVA,flashggVBFMVANew,flashggVBFDiPhoDiJetMVA, flashggVBFDiPhoDiJetMVANew
from flashgg.Taggers.flashggTags_cff import *
from flashgg.Taggers.flashggTagSorter_cfi import flashggTagSorter

flashggTagSequence = cms.Sequence(flashggDiPhotonMVA
                                  * flashggVBFMVA
                                  * flashggVBFMVANew
                                  * flashggVBFDiPhoDiJetMVA
                                  * flashggVBFDiPhoDiJetMVANew
                                  * (flashggUntaggedCategory
                                     + flashggVBFTag
                                     + flashggTTHLeptonicTag
                                     + flashggVHEtTag
                                     + flashggTTHHadronicTag
                                     + flashggVHLooseTag
                                     + flashggVHTightTag
                                     + flashggVHHadronicTag)
                                  * flashggTagSorter
                                  )

