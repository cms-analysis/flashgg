import FWCore.ParameterSet.Config as cms

binInfo = cms.PSet(
		variables = cms.vstring("abs(eta)","pt"),
		bins = cms.VPSet(
                        # CutBasedId - LooseWP scale factors (76X)
                        # https://twiki.cern.ch/twiki/bin/view/CMS/EgammaIDRecipesRun2
                        cms.PSet( lowBounds = cms.vdouble( 0.000,  0.000 ) , upBounds = cms.vdouble( 0.800, 10.000  ) , values = cms.vdouble( 1.0000 ) , uncertainties = cms.vdouble( 0.0382 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 0.000, 10.000 ) , upBounds = cms.vdouble( 0.800, 20.000  ) , values = cms.vdouble( 1.0092 ) , uncertainties = cms.vdouble( 0.0382 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 0.000, 20.000 ) , upBounds = cms.vdouble( 0.800, 30.000  ) , values = cms.vdouble( 0.9898 ) , uncertainties = cms.vdouble( 0.0299 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 0.000, 30.000 ) , upBounds = cms.vdouble( 0.800, 40.000  ) , values = cms.vdouble( 0.9897 ) , uncertainties = cms.vdouble( 0.0192 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 0.000, 40.000 ) , upBounds = cms.vdouble( 0.800, 50.000  ) , values = cms.vdouble( 0.9902 ) , uncertainties = cms.vdouble( 0.0052 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 0.000, 50.000 ) , upBounds = cms.vdouble( 0.800, 10000.000  ) , values = cms.vdouble( 0.9893 ) , uncertainties = cms.vdouble( 0.0047 ) ),

                        cms.PSet( lowBounds = cms.vdouble( 0.800,  0.000 ) , upBounds = cms.vdouble( 1.444, 10.000  ) , values = cms.vdouble( 1.0000 ) , uncertainties = cms.vdouble( 0.0374 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 0.800, 10.000 ) , upBounds = cms.vdouble( 1.444, 20.000  ) , values = cms.vdouble( 1.0772 ) , uncertainties = cms.vdouble( 0.0374 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 0.800, 20.000 ) , upBounds = cms.vdouble( 1.444, 30.000  ) , values = cms.vdouble( 0.9909 ) , uncertainties = cms.vdouble( 0.0355 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 0.800, 30.000 ) , upBounds = cms.vdouble( 1.444, 40.000  ) , values = cms.vdouble( 0.9908 ) , uncertainties = cms.vdouble( 0.0238 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 0.800, 40.000 ) , upBounds = cms.vdouble( 1.444, 50.000  ) , values = cms.vdouble( 0.9902 ) , uncertainties = cms.vdouble( 0.0500 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 0.800, 50.000 ) , upBounds = cms.vdouble( 1.444, 10000.000  ) , values = cms.vdouble( 0.9893 ) , uncertainties = cms.vdouble( 0.0056 ) ),

                        cms.PSet( lowBounds = cms.vdouble( 1.444,  0.000 ) , upBounds = cms.vdouble( 1.566, 10.000  ) , values = cms.vdouble( 1.0000 ) , uncertainties = cms.vdouble( 0.0999 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 1.444, 10.000 ) , upBounds = cms.vdouble( 1.566, 20.000  ) , values = cms.vdouble( 1.0714 ) , uncertainties = cms.vdouble( 0.0999 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 1.444, 20.000 ) , upBounds = cms.vdouble( 1.566, 30.000  ) , values = cms.vdouble( 0.9767 ) , uncertainties = cms.vdouble( 0.3217 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 1.444, 30.000 ) , upBounds = cms.vdouble( 1.566, 40.000  ) , values = cms.vdouble( 0.9838 ) , uncertainties = cms.vdouble( 0.0744 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 1.444, 40.000 ) , upBounds = cms.vdouble( 1.566, 50.000  ) , values = cms.vdouble( 0.9815 ) , uncertainties = cms.vdouble( 0.0163 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 1.444, 50.000 ) , upBounds = cms.vdouble( 1.566, 10000.000  ) , values = cms.vdouble( 0.9841 ) , uncertainties = cms.vdouble( 0.0484 ) ),

                        cms.PSet( lowBounds = cms.vdouble( 1.566,  0.000 ) , upBounds = cms.vdouble( 2.000, 10.000  ) , values = cms.vdouble( 1.0000 ) , uncertainties = cms.vdouble( 0.0494 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 1.566, 10.000 ) , upBounds = cms.vdouble( 2.000, 20.000  ) , values = cms.vdouble( 1.0268 ) , uncertainties = cms.vdouble( 0.0494 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 1.566, 20.000 ) , upBounds = cms.vdouble( 2.000, 30.000  ) , values = cms.vdouble( 0.9786 ) , uncertainties = cms.vdouble( 0.0458 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 1.566, 30.000 ) , upBounds = cms.vdouble( 2.000, 40.000  ) , values = cms.vdouble( 0.9893 ) , uncertainties = cms.vdouble( 0.0131 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 1.566, 40.000 ) , upBounds = cms.vdouble( 2.000, 50.000  ) , values = cms.vdouble( 1.0056 ) , uncertainties = cms.vdouble( 0.0077 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 1.566, 50.000 ) , upBounds = cms.vdouble( 2.000, 10000.000  ) , values = cms.vdouble( 1.0144 ) , uncertainties = cms.vdouble( 0.0055 ) ),

                        cms.PSet( lowBounds = cms.vdouble( 2.000,  0.000 ) , upBounds = cms.vdouble( 5.000, 10.000  ) , values = cms.vdouble( 1.0000 ) , uncertainties = cms.vdouble( 0.0586 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 2.000, 10.000 ) , upBounds = cms.vdouble( 5.000, 20.000  ) , values = cms.vdouble( 1.0389 ) , uncertainties = cms.vdouble( 0.0586 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 2.000, 20.000 ) , upBounds = cms.vdouble( 5.000, 30.000  ) , values = cms.vdouble( 1.0053 ) , uncertainties = cms.vdouble( 0.0244 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 2.000, 30.000 ) , upBounds = cms.vdouble( 5.000, 40.000  ) , values = cms.vdouble( 1.0036 ) , uncertainties = cms.vdouble( 0.0452 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 2.000, 40.000 ) , upBounds = cms.vdouble( 5.000, 50.000  ) , values = cms.vdouble( 1.0139 ) , uncertainties = cms.vdouble( 0.0031 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 2.000, 50.000 ) , upBounds = cms.vdouble( 5.000, 10000.000  ) , values = cms.vdouble( 1.0160 ) , uncertainties = cms.vdouble( 0.0300 ) )

                        )
		)	


flashggElectronSystematics = cms.EDProducer('FlashggElectronEffSystematicProducer',
                                            src = cms.InputTag("flashggSelectedElectrons"),
                                            SystMethods2D = cms.VPSet(),
                                            SystMethods = cms.VPSet(cms.PSet( MethodName = cms.string("FlashggElectronWeight"),
                                                                              Label = cms.string("ElectronWeight"),
                                                                              NSigmas = cms.vint32(-1,1),
                                                                              OverallRange = cms.string("abs(eta)<2.5"),
                                                                              BinList = binInfo,
                                                                              Debug = cms.untracked.bool(False),
                                                                              ApplyCentralValue = cms.bool(True)
                                                                              )	
                                                                    )
                                            )
