#-----------J. Tao from IHEP-Beijing--------------

import FWCore.ParameterSet.Config as cms

flashggMuMuGamma = cms.EDProducer('FlashggMuMuGammaProducer',
                                  DiMuonTag=cms.untracked.InputTag('flashggDiMuons'),
                                  PhotonTag=cms.untracked.InputTag('flashggPhotons'),
                                  VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
                                  ##Parameters                                                
                                  minPhotonPT=cms.untracked.double(10.)
                                  )
