#-----------J. Tao from IHEP-Beijing--------------

import FWCore.ParameterSet.Config as cms

flashggMuMuGamma = cms.EDProducer('FlashggMuMuGammaProducer',
                                  DiMuonTag=cms.InputTag('flashggDiMuons'),
                                  PhotonTag=cms.InputTag('selectedFlashggPhotons'),
                                  VertexTag=cms.InputTag('offlineSlimmedPrimaryVertices'),
                                  ##Parameters                                                
                                  minPhotonPT=cms.untracked.double(10.)
                                  )
