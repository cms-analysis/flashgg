import FWCore.ParameterSet.Config as cms
from flashgg.Taggers.flashggDiPhotonMVA_cfi import flashggDiPhotonMVA
from flashgg.Taggers.flashggVBFMVA_cff import flashggVBFMVA,flashggVBFDiPhoDiJetMVA
from flashgg.Taggers.flashggTags_cff import *
from flashgg.Taggers.flashggPreselectedDiPhotons_cfi import flashggPreselectedDiPhotons
from flashgg.Taggers.flashggTagSorter_cfi import flashggTagSorter

flashggTagSequence = cms.Sequence(flashggPreselectedDiPhotons
				  * flashggDiPhotonMVA
                                  * flashggUnpackedJets
                                  * flashggVBFMVA
                                  * flashggVBFDiPhoDiJetMVA
                                  * ( flashggUntagged
                                     + flashggVBFTag
                                     + flashggTTHLeptonicTag
                                     + flashggTTHHadronicTag                                      
#                                     + flashggVHEtTag
#                                     + flashggVHLooseTag
#                                     + flashggVHTightTag
#                                     + flashggVHHadronicTag
					)
                                 * flashggTagSorter
                                  )

