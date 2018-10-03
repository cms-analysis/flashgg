import FWCore.ParameterSet.Config as cms

binInfo = cms.PSet(
    variables = cms.vstring("eta","pt"),
    bins = cms.VPSet(
        # CutBasedId - Medium MVA WP scale factors (94X)
        # https://twiki.cern.ch/twiki/bin/view/CMS/Egamma2017DataRecommendations#Electron_Reconstruction_Scale_Fa
        # Note : SFs valid only from 10GeV, eta 2.5, bins extended to extremes to prevent any crashes 
        
	cms.PSet( lowBounds = cms.vdouble( -6.0000, 0.0000 ) , upBounds = cms.vdouble( -2.5000, 9999999.0000 ) , values = cms.vdouble( 1. ) , uncertainties = cms.vdouble( 0.0001 ) ),
    
     cms.PSet( lowBounds = cms.vdouble( -2.5 , 0 ) , upBounds = cms.vdouble( -2, 10 ) , values = cms.vdouble( 0.907692 ) , uncertainties = cms.vdouble(0.0222214  ) ),
     cms.PSet( lowBounds = cms.vdouble( -2.5 , 10 ) , upBounds = cms.vdouble( -2, 20 ) , values = cms.vdouble( 0.907692 ) , uncertainties = cms.vdouble(0.0222214  ) ),
     cms.PSet( lowBounds = cms.vdouble( -2.5 , 20 ) , upBounds = cms.vdouble( -2, 35 ) , values = cms.vdouble( 0.891509 ) , uncertainties = cms.vdouble(0.0134782  ) ),
     cms.PSet( lowBounds = cms.vdouble( -2.5 , 35 ) , upBounds = cms.vdouble( -2, 50 ) , values = cms.vdouble( 0.914798 ) , uncertainties = cms.vdouble(0.00326116  ) ),
     cms.PSet( lowBounds = cms.vdouble( -2.5 , 50 ) , upBounds = cms.vdouble( -2, 100 ) , values = cms.vdouble( 0.921199 ) , uncertainties = cms.vdouble(0.0125809  ) ),
     cms.PSet( lowBounds = cms.vdouble( -2.5 , 100 ) , upBounds = cms.vdouble( -2, 200 ) , values = cms.vdouble( 0.965746 ) , uncertainties = cms.vdouble(0.0193935  ) ),
     cms.PSet( lowBounds = cms.vdouble( -2.5 , 200 ) , upBounds = cms.vdouble( -2, 500 ) , values = cms.vdouble( 0.986607 ) , uncertainties = cms.vdouble(0.0565223  ) ),
     cms.PSet( lowBounds = cms.vdouble( -2.5 , 500 ) , upBounds = cms.vdouble( -2, 999999999 ) , values = cms.vdouble( 0.986607 ) , uncertainties = cms.vdouble(0.0565223  ) ),

     cms.PSet( lowBounds = cms.vdouble( -2 , 0 ) , upBounds = cms.vdouble( -1.566, 10 ) , values = cms.vdouble( 0.952267 ) , uncertainties = cms.vdouble(0.0160396  ) ),
     cms.PSet( lowBounds = cms.vdouble( -2 , 10 ) , upBounds = cms.vdouble( -1.566, 20 ) , values = cms.vdouble( 0.952267 ) , uncertainties = cms.vdouble(0.0160396  ) ),
     cms.PSet( lowBounds = cms.vdouble( -2 , 20 ) , upBounds = cms.vdouble( -1.566, 35 ) , values = cms.vdouble( 0.915254 ) , uncertainties = cms.vdouble(0.0219403  ) ),
     cms.PSet( lowBounds = cms.vdouble( -2 , 35 ) , upBounds = cms.vdouble( -1.566, 50 ) , values = cms.vdouble( 0.938178 ) , uncertainties = cms.vdouble(0.00223596  ) ),
     cms.PSet( lowBounds = cms.vdouble( -2 , 50 ) , upBounds = cms.vdouble( -1.566, 100 ) , values = cms.vdouble( 0.946179 ) , uncertainties = cms.vdouble(0.00741477  ) ),
     cms.PSet( lowBounds = cms.vdouble( -2 , 100 ) , upBounds = cms.vdouble( -1.566, 200 ) , values = cms.vdouble( 0.973545 ) , uncertainties = cms.vdouble(0.0204246  ) ),
     cms.PSet( lowBounds = cms.vdouble( -2 , 200 ) , upBounds = cms.vdouble( -1.566, 500 ) , values = cms.vdouble( 0.994635 ) , uncertainties = cms.vdouble(0.0674024  ) ),
     cms.PSet( lowBounds = cms.vdouble( -2 , 500 ) , upBounds = cms.vdouble( -1.566, 99999999 ) , values = cms.vdouble( 0.994635 ) , uncertainties = cms.vdouble(0.0674024  ) ),

     cms.PSet( lowBounds = cms.vdouble( -1.566 , 0 ) , upBounds = cms.vdouble( -1.444, 10 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.566 , 10 ) , upBounds = cms.vdouble( -1.444, 20 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.566 , 20 ) , upBounds = cms.vdouble( -1.444, 35 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.566 , 35 ) , upBounds = cms.vdouble( -1.444, 50 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.566 , 50 ) , upBounds = cms.vdouble( -1.444, 100 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.566 , 100 ) , upBounds = cms.vdouble( -1.444, 200 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.566 , 200 ) , upBounds = cms.vdouble( -1.444, 500 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.566 , 500 ) , upBounds = cms.vdouble( -1.444, 9999999 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),

     cms.PSet( lowBounds = cms.vdouble( -1.444 , 0 ) , upBounds = cms.vdouble( -0.8, 10 ) , values = cms.vdouble( 0.969777 ) , uncertainties = cms.vdouble(0.0316699  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.444 , 10 ) , upBounds = cms.vdouble( -0.8, 20 ) , values = cms.vdouble( 0.969777 ) , uncertainties = cms.vdouble(0.0316699  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.444 , 20 ) , upBounds = cms.vdouble( -0.8, 35 ) , values = cms.vdouble( 0.938702 ) , uncertainties = cms.vdouble(0.0238821  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.444 , 35 ) , upBounds = cms.vdouble( -0.8, 50 ) , values = cms.vdouble( 0.947132 ) , uncertainties = cms.vdouble(0.0036573  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.444 , 50 ) , upBounds = cms.vdouble( -0.8, 100 ) , values = cms.vdouble( 0.949889 ) , uncertainties = cms.vdouble(0.00529399  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.444 , 100 ) , upBounds = cms.vdouble( -0.8, 200 ) , values = cms.vdouble( 0.976562 ) , uncertainties = cms.vdouble(0.011308  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.444 , 200 ) , upBounds = cms.vdouble( -0.8, 500 ) , values = cms.vdouble( 0.997688 ) , uncertainties = cms.vdouble(0.019687  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.444 , 500 ) , upBounds = cms.vdouble( -0.8, 99999999 ) , values = cms.vdouble( 0.997688 ) , uncertainties = cms.vdouble(0.019687  ) ),

     cms.PSet( lowBounds = cms.vdouble( -0.8 , 0 ) , upBounds = cms.vdouble( 0, 10 ) , values = cms.vdouble( 0.98103 ) , uncertainties = cms.vdouble(0.0131015  ) ),
    cms.PSet( lowBounds = cms.vdouble( -0.8 , 10 ) , upBounds = cms.vdouble( 0, 20 ) , values = cms.vdouble( 0.98103 ) , uncertainties = cms.vdouble(0.0131015  ) ),
     cms.PSet( lowBounds = cms.vdouble( -0.8 , 20 ) , upBounds = cms.vdouble( 0, 35 ) , values = cms.vdouble( 0.95933 ) , uncertainties = cms.vdouble(0.0173544  ) ),
     cms.PSet( lowBounds = cms.vdouble( -0.8 , 35 ) , upBounds = cms.vdouble( 0, 50 ) , values = cms.vdouble( 0.960407 ) , uncertainties = cms.vdouble(0.00304075  ) ),
     cms.PSet( lowBounds = cms.vdouble( -0.8 , 50 ) , upBounds = cms.vdouble( 0, 100 ) , values = cms.vdouble( 0.962754 ) , uncertainties = cms.vdouble(0.0125653  ) ),
     cms.PSet( lowBounds = cms.vdouble( -0.8 , 100 ) , upBounds = cms.vdouble( 0, 200 ) , values = cms.vdouble( 0.988636 ) , uncertainties = cms.vdouble(0.00547451  ) ),
     cms.PSet( lowBounds = cms.vdouble( -0.8 , 200 ) , upBounds = cms.vdouble( 0, 500 ) , values = cms.vdouble( 0.984216 ) , uncertainties = cms.vdouble(0.0404122  ) ),
     cms.PSet( lowBounds = cms.vdouble( -0.8 , 500 ) , upBounds = cms.vdouble( 0, 99999999 ) , values = cms.vdouble( 0.984216 ) , uncertainties = cms.vdouble(0.0404122  ) ),

     cms.PSet( lowBounds = cms.vdouble( 0 , 0 ) , upBounds = cms.vdouble( 0.8, 10 ) , values = cms.vdouble( 0.982527 ) , uncertainties = cms.vdouble(0.0131015  ) ),
     cms.PSet( lowBounds = cms.vdouble( 0 , 10 ) , upBounds = cms.vdouble( 0.8, 20 ) , values = cms.vdouble( 0.982527 ) , uncertainties = cms.vdouble(0.0131015  ) ),
     cms.PSet( lowBounds = cms.vdouble( 0 , 20 ) , upBounds = cms.vdouble( 0.8, 35 ) , values = cms.vdouble( 0.959572 ) , uncertainties = cms.vdouble(0.0173544  ) ),
     cms.PSet( lowBounds = cms.vdouble( 0 , 35 ) , upBounds = cms.vdouble( 0.8, 50 ) , values = cms.vdouble( 0.960541 ) , uncertainties = cms.vdouble(0.00304075  ) ),
     cms.PSet( lowBounds = cms.vdouble( 0 , 50 ) , upBounds = cms.vdouble( 0.8, 100 ) , values = cms.vdouble( 0.961798 ) , uncertainties = cms.vdouble(0.0125653  ) ),
     cms.PSet( lowBounds = cms.vdouble( 0 , 100 ) , upBounds = cms.vdouble( 0.8, 200 ) , values = cms.vdouble( 0.983165 ) , uncertainties = cms.vdouble(0.00547451  ) ),
     cms.PSet( lowBounds = cms.vdouble( 0 , 200 ) , upBounds = cms.vdouble( 0.8, 500 ) , values = cms.vdouble( 0.967778 ) , uncertainties = cms.vdouble(0.0404616  ) ),
     cms.PSet( lowBounds = cms.vdouble( 0 , 500 ) , upBounds = cms.vdouble( 0.8, 99999999 ) , values = cms.vdouble( 0.967778 ) , uncertainties = cms.vdouble(0.0404616  ) ),

     cms.PSet( lowBounds = cms.vdouble( 0.8 , 0 ) , upBounds = cms.vdouble( 1.444, 10 ) , values = cms.vdouble( 0.992021 ) , uncertainties = cms.vdouble(0.0316699  ) ),
     cms.PSet( lowBounds = cms.vdouble( 0.8 , 10 ) , upBounds = cms.vdouble( 1.444, 20 ) , values = cms.vdouble( 0.992021 ) , uncertainties = cms.vdouble(0.0316699  ) ),
     cms.PSet( lowBounds = cms.vdouble( 0.8 , 20 ) , upBounds = cms.vdouble( 1.444, 35 ) , values = cms.vdouble( 0.94303 ) , uncertainties = cms.vdouble(0.0238821  ) ),
     cms.PSet( lowBounds = cms.vdouble( 0.8 , 35 ) , upBounds = cms.vdouble( 1.444, 50 ) , values = cms.vdouble( 0.95017 ) , uncertainties = cms.vdouble(0.0036573  ) ),
     cms.PSet( lowBounds = cms.vdouble( 0.8 , 50 ) , upBounds = cms.vdouble( 1.444, 100 ) , values = cms.vdouble( 0.95302 ) , uncertainties = cms.vdouble(0.00529399  ) ),
     cms.PSet( lowBounds = cms.vdouble( 0.8 , 100 ) , upBounds = cms.vdouble( 1.444, 200 ) , values = cms.vdouble( 0.968854 ) , uncertainties = cms.vdouble(0.011308  ) ),
     cms.PSet( lowBounds = cms.vdouble( 0.8 , 200 ) , upBounds = cms.vdouble( 1.444, 500 ) , values = cms.vdouble( 0.953488 ) , uncertainties = cms.vdouble(0.0196504  ) ),
     cms.PSet( lowBounds = cms.vdouble( 0.8 , 500 ) , upBounds = cms.vdouble( 1.444, 9999999 ) , values = cms.vdouble( 0.953488 ) , uncertainties = cms.vdouble(0.0196504  ) ),

     cms.PSet( lowBounds = cms.vdouble( 1.444 , 0 ) , upBounds = cms.vdouble( 1.566, 10 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
    cms.PSet( lowBounds = cms.vdouble( 1.444 , 10 ) , upBounds = cms.vdouble( 1.566, 20 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.444 , 20 ) , upBounds = cms.vdouble( 1.566, 35 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.444 , 35 ) , upBounds = cms.vdouble( 1.566, 50 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.444 , 50 ) , upBounds = cms.vdouble( 1.566, 100 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.444 , 100 ) , upBounds = cms.vdouble( 1.566, 200 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.444 , 200 ) , upBounds = cms.vdouble( 1.566, 500 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.444 , 500 ) , upBounds = cms.vdouble( 1.566, 9999999 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),

     cms.PSet( lowBounds = cms.vdouble( 1.566 , 0 ) , upBounds = cms.vdouble( 2, 10 ) , values = cms.vdouble( 0.971463 ) , uncertainties = cms.vdouble(0.0161361  ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.566 , 10 ) , upBounds = cms.vdouble( 2, 20 ) , values = cms.vdouble( 0.971463 ) , uncertainties = cms.vdouble(0.0161361  ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.566 , 20 ) , upBounds = cms.vdouble( 2, 35 ) , values = cms.vdouble( 0.918367 ) , uncertainties = cms.vdouble(0.0219403  ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.566 , 35 ) , upBounds = cms.vdouble( 2, 50 ) , values = cms.vdouble( 0.940347 ) , uncertainties = cms.vdouble(0.00223596  ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.566 , 50 ) , upBounds = cms.vdouble( 2, 100 ) , values = cms.vdouble( 0.951613 ) , uncertainties = cms.vdouble(0.00741477  ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.566 , 100 ) , upBounds = cms.vdouble( 2, 200 ) , values = cms.vdouble( 0.990385 ) , uncertainties = cms.vdouble(0.0204246  ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.566 , 200 ) , upBounds = cms.vdouble( 2, 500 ) , values = cms.vdouble( 0.977895 ) , uncertainties = cms.vdouble(0.0672299  ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.566 , 500 ) , upBounds = cms.vdouble( 2, 99999999 ) , values = cms.vdouble( 0.977895 ) , uncertainties = cms.vdouble(0.0672299  ) ),

     cms.PSet( lowBounds = cms.vdouble( 2 , 0 ) , upBounds = cms.vdouble( 2.5, 10 ) , values = cms.vdouble( 0.901515 ) , uncertainties = cms.vdouble(0.0221553  ) ),
     cms.PSet( lowBounds = cms.vdouble( 2 , 10 ) , upBounds = cms.vdouble( 2.5, 20 ) , values = cms.vdouble( 0.901515 ) , uncertainties = cms.vdouble(0.0221553  ) ),
     cms.PSet( lowBounds = cms.vdouble( 2 , 20 ) , upBounds = cms.vdouble( 2.5, 35 ) , values = cms.vdouble( 0.889671 ) , uncertainties = cms.vdouble(0.0134782  ) ),
     cms.PSet( lowBounds = cms.vdouble( 2 , 35 ) , upBounds = cms.vdouble( 2.5, 50 ) , values = cms.vdouble( 0.910714 ) , uncertainties = cms.vdouble(0.00326116  ) ),
     cms.PSet( lowBounds = cms.vdouble( 2 , 50 ) , upBounds = cms.vdouble( 2.5, 100 ) , values = cms.vdouble( 0.918232 ) , uncertainties = cms.vdouble(0.0125413  ) ),
     cms.PSet( lowBounds = cms.vdouble( 2 , 100 ) , upBounds = cms.vdouble( 2.5, 200 ) , values = cms.vdouble( 0.976872 ) , uncertainties = cms.vdouble(0.0195088  ) ),
     cms.PSet( lowBounds = cms.vdouble( 2 , 200 ) , upBounds = cms.vdouble( 2.5, 500 ) , values = cms.vdouble( 0.898446 ) , uncertainties = cms.vdouble(0.0550825  ) ),	
     cms.PSet( lowBounds = cms.vdouble( 2 , 500 ) , upBounds = cms.vdouble( 2.5, 999999999 ) , values = cms.vdouble( 0.898446 ) , uncertainties = cms.vdouble(0.0550825  ) ),	

        	cms.PSet( lowBounds = cms.vdouble( 2.5000, 0.0000 ) , upBounds = cms.vdouble( 6.0000, 999999999.0000 ) , values = cms.vdouble(  1.0 ) , uncertainties = cms.vdouble( 0.001 ) )

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
