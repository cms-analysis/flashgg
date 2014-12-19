import FWCore.ParameterSet.Config as cms
from flashgg.TagProducers.flashggDiPhotonMVA_cfi import flashggDiPhotonMVA
from flashgg.TagProducers.flashggVBFMVA_cff import flashggVBFMVA,flashggVBFDiPhoDiJetMVA
from flashgg.TagProducers.flashggTags_cff import *
from flashgg.TagProducers.flashggTagSorter_cfi import flashggTagSorter

flashggTagSequence = cms.Sequence(flashggDiPhotonMVA*
                                  flashggVBFMVA*
                                  flashggVBFDiPhoDiJetMVA*
                                  (flashggUntaggedCategory+flashggVBFTag+flashggTTHleptonicTag+flashggTTHhadronicTag+flashggVHlooseTag+flashggVHtightTag)*
                                  flashggTagSorter
                                  )

