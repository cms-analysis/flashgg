import FWCore.ParameterSet.Config as cms
from flashgg.Taggers.flashggDiPhotonMVA_cfi import flashggDiPhotonMVA
from flashgg.Taggers.flashggVBFMVA_cff import flashggVBFMVA,flashggVBFDiPhoDiJetMVA
from flashgg.Taggers.flashggPrefireWeight_cff import flashggPrefireWeight
from flashgg.Taggers.flashggTags_cff import *
from flashgg.Taggers.flashggPreselectedDiPhotons_cfi import flashggPreselectedDiPhotons
from flashgg.Taggers.flashggTagSorter_cfi import flashggTagSorter
from flashgg.Taggers.flashggDifferentialPhoIdInputsCorrection_cfi import flashggDifferentialPhoIdInputsCorrection, setup_flashggDifferentialPhoIdInputsCorrection

def flashggPrepareTagSequence(process, options):
    setup_flashggDifferentialPhoIdInputsCorrection(process, options)
    flashggPreselectedDiPhotons.src = "flashggDifferentialPhoIdInputsCorrection"

    flashggTagSequence = cms.Sequence(flashggDifferentialPhoIdInputsCorrection
                                      * flashggPreselectedDiPhotons
                                      * flashggDiPhotonMVA
                                      * flashggUnpackedJets
                                      * flashggVBFMVA
                                      * flashggVBFDiPhoDiJetMVA
                                      * flashggPrefireWeight
                                      * ( flashggUntagged
                                      #                                  *( flashggSigmaMoMpToMTag
                                          + flashggVBFTag
                                          + flashggTTHDiLeptonTag
                                          + flashggTTHLeptonicTag
                      + flashggTHQLeptonicTag
#                                     + flashggTTHHadronicTTag                                      
#                                     + flashggTTHHadronicLTag                                      
                                          + flashggTTHHadronicTag
                                      #############old VH tags##############
                                      #                  + flashggVHEtTag
                                      #                  + flashggVHLooseTag
                                      #                  + flashggVHTightTag
                                      ###########updated VH tags############
                                          + flashggVHMetTag
                                          + flashggWHLeptonicTag
                                          + flashggZHLeptonicTag
                                          + flashggVHLeptonicLooseTag
                                          + flashggVHHadronicTag
                                      )
                                      * flashggTagSorter
                                  )

    return flashggTagSequence
