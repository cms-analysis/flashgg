import FWCore.ParameterSet.Config as cms

flashggDiElectrons = cms.EDProducer('FlashggDiElectronProducer',
                                  ElectronTag=cms.InputTag('flashggSelectedElectrons'),
                                  ##Parameters                                                
                                  minElectronPt=cms.double(7.),
                                  maxElectronEta=cms.double(2.4)
                                  )
