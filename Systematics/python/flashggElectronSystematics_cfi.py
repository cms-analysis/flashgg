import FWCore.ParameterSet.Config as cms

binInfo = cms.PSet(
		variables = cms.vstring("eta","pt"),
		bins = cms.VPSet(
                        # CutBasedId - LooseWP scale factors
                        # preliminary numbers from M. Sani: /afs/cern.ch/work/a/arun/public/forEGM/CutBased_textFiles_withSyst/CutBasedID_LooseWP_fromTemplates_withSyst_v1.txt

                        cms.PSet( lowBounds = cms.vdouble( -5.000, 0.000 ) , upBounds = cms.vdouble(  5.000, 20.000  ) , values = cms.vdouble( 1.000 ) , uncertainties = cms.vdouble( 0.030 ) ),

                        cms.PSet( lowBounds = cms.vdouble( -5.000, 20.000 ) , upBounds = cms.vdouble( -2.000, 30.000  ) , values = cms.vdouble( 1.0000 ) , uncertainties = cms.vdouble( 0.0300 ) ),
                        cms.PSet( lowBounds = cms.vdouble( -2.000, 20.000 ) , upBounds = cms.vdouble( -1.566, 30.000  ) , values = cms.vdouble( 0.9905 ) , uncertainties = cms.vdouble( 0.0440 ) ),
                        cms.PSet( lowBounds = cms.vdouble( -1.566, 20.000 ) , upBounds = cms.vdouble( -1.444, 30.000  ) , values = cms.vdouble( 1.0083 ) , uncertainties = cms.vdouble( 0.0302 ) ),
                        cms.PSet( lowBounds = cms.vdouble( -1.444, 20.000 ) , upBounds = cms.vdouble( -0.800, 30.000  ) , values = cms.vdouble( 1.0199 ) , uncertainties = cms.vdouble( 0.0628 ) ),
                        cms.PSet( lowBounds = cms.vdouble( -0.800, 20.000 ) , upBounds = cms.vdouble( 0.000, 30.000  ) , values = cms.vdouble( 0.9871 ) , uncertainties = cms.vdouble( 0.0284 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 0.000, 20.000 ) , upBounds = cms.vdouble( 0.800, 30.000  ) , values = cms.vdouble( 1.0000 ) , uncertainties = cms.vdouble( 0.0300 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 0.800, 20.000 ) , upBounds = cms.vdouble( 1.444, 30.000  ) , values = cms.vdouble( 1.0146 ) , uncertainties = cms.vdouble( 0.0304 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 1.444, 20.000 ) , upBounds = cms.vdouble( 1.566, 30.000  ) , values = cms.vdouble( 1.0393 ) , uncertainties = cms.vdouble( 0.0312 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 1.566, 20.000 ) , upBounds = cms.vdouble( 2.000, 30.000  ) , values = cms.vdouble( 1.0233 ) , uncertainties = cms.vdouble( 0.0307 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 2.000, 20.000 ) , upBounds = cms.vdouble( 5.000, 30.000  ) , values = cms.vdouble( 1.0306 ) , uncertainties = cms.vdouble( 0.0542 ) ),

                        cms.PSet( lowBounds = cms.vdouble( -5.000, 30.000 ) , upBounds = cms.vdouble( -2.000, 40.000  ) , values = cms.vdouble( 1.0271 ) , uncertainties = cms.vdouble( 0.0216 ) ),
                        cms.PSet( lowBounds = cms.vdouble( -2.000, 30.000 ) , upBounds = cms.vdouble( -1.566, 40.000  ) , values = cms.vdouble( 1.0024 ) , uncertainties = cms.vdouble( 0.0436 ) ),
                        cms.PSet( lowBounds = cms.vdouble( -1.566, 30.000 ) , upBounds = cms.vdouble( -1.444, 40.000  ) , values = cms.vdouble( 1.0133 ) , uncertainties = cms.vdouble( 0.0405 ) ),
                        cms.PSet( lowBounds = cms.vdouble( -1.444, 30.000 ) , upBounds = cms.vdouble( -0.800, 40.000  ) , values = cms.vdouble( 0.9919 ) , uncertainties = cms.vdouble( 0.0157 ) ),
                        cms.PSet( lowBounds = cms.vdouble( -0.800, 30.000 ) , upBounds = cms.vdouble( 0.000, 40.000  ) , values = cms.vdouble( 0.9752 ) , uncertainties = cms.vdouble( 0.0125 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 0.000, 30.000 ) , upBounds = cms.vdouble( 0.800, 40.000  ) , values = cms.vdouble( 0.9774 ) , uncertainties = cms.vdouble( 0.0172 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 0.800, 30.000 ) , upBounds = cms.vdouble( 1.444, 40.000  ) , values = cms.vdouble( 0.9988 ) , uncertainties = cms.vdouble( 0.0190 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 1.444, 30.000 ) , upBounds = cms.vdouble( 1.566, 40.000  ) , values = cms.vdouble( 0.9985 ) , uncertainties = cms.vdouble( 0.0437 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 1.566, 30.000 ) , upBounds = cms.vdouble( 2.000, 40.000  ) , values = cms.vdouble( 0.9917 ) , uncertainties = cms.vdouble( 0.0148 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 2.000, 30.000 ) , upBounds = cms.vdouble( 5.000, 40.000  ) , values = cms.vdouble( 1.0246 ) , uncertainties = cms.vdouble( 0.0189 ) ),

                        cms.PSet( lowBounds = cms.vdouble( -5.000, 40.000 ) , upBounds = cms.vdouble( -2.000, 50.000  ) , values = cms.vdouble( 1.0186 ) , uncertainties = cms.vdouble( 0.0055 ) ),
                        cms.PSet( lowBounds = cms.vdouble( -2.000, 40.000 ) , upBounds = cms.vdouble( -1.566, 50.000  ) , values = cms.vdouble( 1.0034 ) , uncertainties = cms.vdouble( 0.0078 ) ),
                        cms.PSet( lowBounds = cms.vdouble( -1.566, 40.000 ) , upBounds = cms.vdouble( -1.444, 50.000  ) , values = cms.vdouble( 0.9938 ) , uncertainties = cms.vdouble( 0.0098 ) ),
                        cms.PSet( lowBounds = cms.vdouble( -1.444, 40.000 ) , upBounds = cms.vdouble( -0.800, 50.000  ) , values = cms.vdouble( 0.9881 ) , uncertainties = cms.vdouble( 0.0099 ) ),
                        cms.PSet( lowBounds = cms.vdouble( -0.800, 40.000 ) , upBounds = cms.vdouble( 0.000, 50.000  ) , values = cms.vdouble( 0.9775 ) , uncertainties = cms.vdouble( 0.0097 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 0.000, 40.000 ) , upBounds = cms.vdouble( 0.800, 50.000  ) , values = cms.vdouble( 0.9786 ) , uncertainties = cms.vdouble( 0.0133 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 0.800, 40.000 ) , upBounds = cms.vdouble( 1.444, 50.000  ) , values = cms.vdouble( 0.9881 ) , uncertainties = cms.vdouble( 0.0028 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 1.444, 40.000 ) , upBounds = cms.vdouble( 1.566, 50.000  ) , values = cms.vdouble( 0.9739 ) , uncertainties = cms.vdouble( 0.0128 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 1.566, 40.000 ) , upBounds = cms.vdouble( 2.000, 50.000  ) , values = cms.vdouble( 1.0034 ) , uncertainties = cms.vdouble( 0.0070 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 2.000, 40.000 ) , upBounds = cms.vdouble( 5.000, 50.000  ) , values = cms.vdouble( 1.0140 ) , uncertainties = cms.vdouble( 0.0185 ) ),

                        cms.PSet( lowBounds = cms.vdouble( -5.000, 50.000 ) , upBounds = cms.vdouble( -2.000, 10000.000  ) , values = cms.vdouble( 1.0114 ) , uncertainties = cms.vdouble( 0.0119 ) ),
                        cms.PSet( lowBounds = cms.vdouble( -2.000, 50.000 ) , upBounds = cms.vdouble( -1.566, 10000.000  ) , values = cms.vdouble( 1.0044 ) , uncertainties = cms.vdouble( 0.0168 ) ),
                        cms.PSet( lowBounds = cms.vdouble( -1.566, 50.000 ) , upBounds = cms.vdouble( -1.444, 10000.000  ) , values = cms.vdouble( 0.9805 ) , uncertainties = cms.vdouble( 0.0729 ) ),
                        cms.PSet( lowBounds = cms.vdouble( -1.444, 50.000 ) , upBounds = cms.vdouble( -0.800, 10000.000  ) , values = cms.vdouble( 0.9798 ) , uncertainties = cms.vdouble( 0.0188 ) ),
                        cms.PSet( lowBounds = cms.vdouble( -0.800, 50.000 ) , upBounds = cms.vdouble( 0.000, 10000.000  ) , values = cms.vdouble( 0.9778 ) , uncertainties = cms.vdouble( 0.0137 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 0.000, 50.000 ) , upBounds = cms.vdouble( 0.800, 10000.000  ) , values = cms.vdouble( 0.9789 ) , uncertainties = cms.vdouble( 0.0144 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 0.800, 50.000 ) , upBounds = cms.vdouble( 1.444, 10000.000  ) , values = cms.vdouble( 0.9861 ) , uncertainties = cms.vdouble( 0.0071 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 1.444, 50.000 ) , upBounds = cms.vdouble( 1.566, 10000.000  ) , values = cms.vdouble( 0.9854 ) , uncertainties = cms.vdouble( 0.0296 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 1.566, 50.000 ) , upBounds = cms.vdouble( 2.000, 10000.000  ) , values = cms.vdouble( 1.0077 ) , uncertainties = cms.vdouble( 0.0122 ) ),
                        cms.PSet( lowBounds = cms.vdouble( 2.000, 50.000 ) , upBounds = cms.vdouble( 5.000, 10000.000  ) , values = cms.vdouble( 1.0149 ) , uncertainties = cms.vdouble( 0.0304 ) )
                        
                        
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
