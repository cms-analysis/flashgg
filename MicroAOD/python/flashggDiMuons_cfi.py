#-----------J. Tao from IHEP-Beijing--------------

import FWCore.ParameterSet.Config as cms

flashggDiMuons = cms.EDProducer('FlashggDiMuonProducer',
                                  MuonTag=cms.InputTag('flashggMuons'),
                                  VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'), 
                                  ##Parameters                                                
                                  minMuonPT=cms.untracked.double(5.),
                                  maxMuonEta=cms.untracked.double(2.4)
                                  )
