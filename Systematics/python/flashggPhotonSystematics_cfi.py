import FWCore.ParameterSet.Config as cms

flashggDiPhotonSystematics = cms.EDProducer('FlashggPhotonSystematicProducer',
                                            src = cms.InputTag("flashggRandomizedPhotons"),
                                            SystMethods2D = cms.VPSet(),
                                            # the number of syst methods matches the number of nuisance parameters
                                            # assumed for a given systematic uncertainty and is NOT required
                                            # to match 1-to-1 the number of bins above.
                                            SystMethods = cms.VPSet(),
                                            )
