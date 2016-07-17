import FWCore.ParameterSet.Config as cms

binInfo = cms.PSet(
    variables = cms.vstring("eta","pt"),
    bins = cms.VPSet(
        # CutBasedId - LooseWP scale factors (80X)
        # https://twiki.cern.ch/twiki/bin/viewauth/CMS/EgammaIDRecipesRun2#Efficiencies_and_scale_factors : combined : period 1 run < 273726 (0.5/fb) no systematics and period 2: run >= 273726 (5/fb) | for a total of : 9.2 /fb.
        
        cms.PSet( lowBounds = cms.vdouble( -5.0000,  0.0000 ) , upBounds = cms.vdouble( -2.0000, 10.0000 ) , values = cms.vdouble( 1.0000 ) , uncertainties = cms.vdouble( 0.0214 ) ),
        cms.PSet( lowBounds = cms.vdouble( -5.0000, 10.0000 ) , upBounds = cms.vdouble( -2.0000, 20.0000 ) , values = cms.vdouble( 1.0330 ) , uncertainties = cms.vdouble( 0.0214 ) ),
        cms.PSet( lowBounds = cms.vdouble( -5.0000, 20.0000 ) , upBounds = cms.vdouble( -2.0000, 30.0000 ) , values = cms.vdouble( 0.9936 ) , uncertainties = cms.vdouble( 0.0124 ) ),
        cms.PSet( lowBounds = cms.vdouble( -5.0000, 30.0000 ) , upBounds = cms.vdouble( -2.0000, 40.0000 ) , values = cms.vdouble( 1.0093 ) , uncertainties = cms.vdouble( 0.0106 ) ),
        cms.PSet( lowBounds = cms.vdouble( -5.0000, 40.0000 ) , upBounds = cms.vdouble( -2.0000, 50.0000 ) , values = cms.vdouble( 1.0076 ) , uncertainties = cms.vdouble( 0.0068 ) ),
        cms.PSet( lowBounds = cms.vdouble( -5.0000, 50.0000 ) , upBounds = cms.vdouble( -2.0000, 1000.0000 ) , values = cms.vdouble( 1.0080 ) , uncertainties = cms.vdouble( 0.0115 ) ),
        
        cms.PSet( lowBounds = cms.vdouble( -2.0000,  0.0000 ) , upBounds = cms.vdouble( -1.5660, 10.0000 ) , values = cms.vdouble( 1.0000 ) , uncertainties = cms.vdouble( 0.0500 ) ),
        cms.PSet( lowBounds = cms.vdouble( -2.0000, 10.0000 ) , upBounds = cms.vdouble( -1.5660, 20.0000 ) , values = cms.vdouble( 0.8634 ) , uncertainties = cms.vdouble( 0.0500 ) ),
        cms.PSet( lowBounds = cms.vdouble( -2.0000, 20.0000 ) , upBounds = cms.vdouble( -1.5660, 30.0000 ) , values = cms.vdouble( 0.9154 ) , uncertainties = cms.vdouble( 0.0295 ) ),
        cms.PSet( lowBounds = cms.vdouble( -2.0000, 30.0000 ) , upBounds = cms.vdouble( -1.5660, 40.0000 ) , values = cms.vdouble( 0.9699 ) , uncertainties = cms.vdouble( 0.0101 ) ),
        cms.PSet( lowBounds = cms.vdouble( -2.0000, 40.0000 ) , upBounds = cms.vdouble( -1.5660, 50.0000 ) , values = cms.vdouble( 0.9819 ) , uncertainties = cms.vdouble( 0.0032 ) ),
        cms.PSet( lowBounds = cms.vdouble( -2.0000, 50.0000 ) , upBounds = cms.vdouble( -1.5660, 1000.0000 ) , values = cms.vdouble( 0.9921 ) , uncertainties = cms.vdouble( 0.0043 ) ),
        
        cms.PSet( lowBounds = cms.vdouble( -1.5660,  0.0000 ) , upBounds = cms.vdouble( -1.4440, 10.0000 ) , values = cms.vdouble( 1.0000 ) , uncertainties = cms.vdouble( 0.0976 ) ),
        cms.PSet( lowBounds = cms.vdouble( -1.5660, 10.0000 ) , upBounds = cms.vdouble( -1.4440, 20.0000 ) , values = cms.vdouble( 0.9950 ) , uncertainties = cms.vdouble( 0.0976 ) ),
        cms.PSet( lowBounds = cms.vdouble( -1.5660, 20.0000 ) , upBounds = cms.vdouble( -1.4440, 30.0000 ) , values = cms.vdouble( 0.9836 ) , uncertainties = cms.vdouble( 0.1099 ) ),
        cms.PSet( lowBounds = cms.vdouble( -1.5660, 30.0000 ) , upBounds = cms.vdouble( -1.4440, 40.0000 ) , values = cms.vdouble( 0.9661 ) , uncertainties = cms.vdouble( 0.0164 ) ),
        cms.PSet( lowBounds = cms.vdouble( -1.5660, 40.0000 ) , upBounds = cms.vdouble( -1.4440, 50.0000 ) , values = cms.vdouble( 0.9767 ) , uncertainties = cms.vdouble( 0.0057 ) ),
        cms.PSet( lowBounds = cms.vdouble( -1.5660, 50.0000 ) , upBounds = cms.vdouble( -1.4440, 1000.0000 ) , values = cms.vdouble( 0.9909 ) , uncertainties = cms.vdouble( 0.0123 ) ),
        
        cms.PSet( lowBounds = cms.vdouble( -1.4440,  0.0000 ) , upBounds = cms.vdouble( -0.8000, 10.0000 ) , values = cms.vdouble( 1.0000 ) , uncertainties = cms.vdouble( 0.0228 ) ),
        cms.PSet( lowBounds = cms.vdouble( -1.4440, 10.0000 ) , upBounds = cms.vdouble( -0.8000, 20.0000 ) , values = cms.vdouble( 0.9908 ) , uncertainties = cms.vdouble( 0.0228 ) ),
        cms.PSet( lowBounds = cms.vdouble( -1.4440, 20.0000 ) , upBounds = cms.vdouble( -0.8000, 30.0000 ) , values = cms.vdouble( 0.9646 ) , uncertainties = cms.vdouble( 0.0262 ) ),
        cms.PSet( lowBounds = cms.vdouble( -1.4440, 30.0000 ) , upBounds = cms.vdouble( -0.8000, 40.0000 ) , values = cms.vdouble( 0.9782 ) , uncertainties = cms.vdouble( 0.0081 ) ),
        cms.PSet( lowBounds = cms.vdouble( -1.4440, 40.0000 ) , upBounds = cms.vdouble( -0.8000, 50.0000 ) , values = cms.vdouble( 0.9817 ) , uncertainties = cms.vdouble( 0.0035 ) ),
        cms.PSet( lowBounds = cms.vdouble( -1.4440, 50.0000 ) , upBounds = cms.vdouble( -0.8000, 1000.0000 ) , values = cms.vdouble( 0.9842 ) , uncertainties = cms.vdouble( 0.0021 ) ),
        
        cms.PSet( lowBounds = cms.vdouble( -0.8000,  0.0000 ) , upBounds = cms.vdouble( 0.0000, 10.0000 ) , values = cms.vdouble( 1.0000 ) , uncertainties = cms.vdouble( 0.0307 ) ),
        cms.PSet( lowBounds = cms.vdouble( -0.8000, 10.0000 ) , upBounds = cms.vdouble( 0.0000, 20.0000 ) , values = cms.vdouble( 0.9777 ) , uncertainties = cms.vdouble( 0.0307 ) ),
        cms.PSet( lowBounds = cms.vdouble( -0.8000, 20.0000 ) , upBounds = cms.vdouble( 0.0000, 30.0000 ) , values = cms.vdouble( 0.9699 ) , uncertainties = cms.vdouble( 0.0238 ) ),
        cms.PSet( lowBounds = cms.vdouble( -0.8000, 30.0000 ) , upBounds = cms.vdouble( 0.0000, 40.0000 ) , values = cms.vdouble( 0.9723 ) , uncertainties = cms.vdouble( 0.0049 ) ),
        cms.PSet( lowBounds = cms.vdouble( -0.8000, 40.0000 ) , upBounds = cms.vdouble( 0.0000, 50.0000 ) , values = cms.vdouble( 0.9777 ) , uncertainties = cms.vdouble( 0.0033 ) ),
        cms.PSet( lowBounds = cms.vdouble( -0.8000, 50.0000 ) , upBounds = cms.vdouble( 0.0000, 1000.0000 ) , values = cms.vdouble( 0.9764 ) , uncertainties = cms.vdouble( 0.0023 ) ),
        
        cms.PSet( lowBounds = cms.vdouble( 0.0000,  0.0000 ) , upBounds = cms.vdouble( 0.8000, 10.0000 ) , values = cms.vdouble( 1.0000 ) , uncertainties = cms.vdouble( 0.0307 ) ),
        cms.PSet( lowBounds = cms.vdouble( 0.0000, 10.0000 ) , upBounds = cms.vdouble( 0.8000, 20.0000 ) , values = cms.vdouble( 0.9383 ) , uncertainties = cms.vdouble( 0.0307 ) ),
        cms.PSet( lowBounds = cms.vdouble( 0.0000, 20.0000 ) , upBounds = cms.vdouble( 0.8000, 30.0000 ) , values = cms.vdouble( 0.9798 ) , uncertainties = cms.vdouble( 0.0238 ) ),
        cms.PSet( lowBounds = cms.vdouble( 0.0000, 30.0000 ) , upBounds = cms.vdouble( 0.8000, 40.0000 ) , values = cms.vdouble( 0.9798 ) , uncertainties = cms.vdouble( 0.0049 ) ),
        cms.PSet( lowBounds = cms.vdouble( 0.0000, 40.0000 ) , upBounds = cms.vdouble( 0.8000, 50.0000 ) , values = cms.vdouble( 0.9859 ) , uncertainties = cms.vdouble( 0.0033 ) ),
        cms.PSet( lowBounds = cms.vdouble( 0.0000, 50.0000 ) , upBounds = cms.vdouble( 0.8000, 1000.0000 ) , values = cms.vdouble( 0.9875 ) , uncertainties = cms.vdouble( 0.0023 ) ),
        
        cms.PSet( lowBounds = cms.vdouble( 0.8000,  0.0000 ) , upBounds = cms.vdouble( 1.4440, 10.0000 ) , values = cms.vdouble( 1.0000 ) , uncertainties = cms.vdouble( 0.0228 ) ),
        cms.PSet( lowBounds = cms.vdouble( 0.8000, 10.0000 ) , upBounds = cms.vdouble( 1.4440, 20.0000 ) , values = cms.vdouble( 0.9595 ) , uncertainties = cms.vdouble( 0.0228 ) ),
        cms.PSet( lowBounds = cms.vdouble( 0.8000, 20.0000 ) , upBounds = cms.vdouble( 1.4440, 30.0000 ) , values = cms.vdouble( 0.9695 ) , uncertainties = cms.vdouble( 0.0262 ) ),
        cms.PSet( lowBounds = cms.vdouble( 0.8000, 30.0000 ) , upBounds = cms.vdouble( 1.4440, 40.0000 ) , values = cms.vdouble( 0.9761 ) , uncertainties = cms.vdouble( 0.0081 ) ),
        cms.PSet( lowBounds = cms.vdouble( 0.8000, 40.0000 ) , upBounds = cms.vdouble( 1.4440, 50.0000 ) , values = cms.vdouble( 0.9817 ) , uncertainties = cms.vdouble( 0.0035 ) ),
        cms.PSet( lowBounds = cms.vdouble( 0.8000, 50.0000 ) , upBounds = cms.vdouble( 1.4440, 1000.0000 ) , values = cms.vdouble( 0.9832 ) , uncertainties = cms.vdouble( 0.0021 ) ),
        
        cms.PSet( lowBounds = cms.vdouble( 1.4440,  0.0000 ) , upBounds = cms.vdouble( 1.5660, 10.0000 ) , values = cms.vdouble( 1.0000 ) , uncertainties = cms.vdouble( 0.0976 ) ),
        cms.PSet( lowBounds = cms.vdouble( 1.4440, 10.0000 ) , upBounds = cms.vdouble( 1.5660, 20.0000 ) , values = cms.vdouble( 0.9796 ) , uncertainties = cms.vdouble( 0.0976 ) ),
        cms.PSet( lowBounds = cms.vdouble( 1.4440, 20.0000 ) , upBounds = cms.vdouble( 1.5660, 30.0000 ) , values = cms.vdouble( 0.9836 ) , uncertainties = cms.vdouble( 0.1099 ) ),
        cms.PSet( lowBounds = cms.vdouble( 1.4440, 30.0000 ) , upBounds = cms.vdouble( 1.5660, 40.0000 ) , values = cms.vdouble( 0.9593 ) , uncertainties = cms.vdouble( 0.0164 ) ),
        cms.PSet( lowBounds = cms.vdouble( 1.4440, 40.0000 ) , upBounds = cms.vdouble( 1.5660, 50.0000 ) , values = cms.vdouble( 0.9640 ) , uncertainties = cms.vdouble( 0.0057 ) ),
        cms.PSet( lowBounds = cms.vdouble( 1.4440, 50.0000 ) , upBounds = cms.vdouble( 1.5660, 1000.0000 ) , values = cms.vdouble( 0.9728 ) , uncertainties = cms.vdouble( 0.0123 ) ),
        
        cms.PSet( lowBounds = cms.vdouble( 1.5660,  0.0000 ) , upBounds = cms.vdouble( 2.0000, 10.0000 ) , values = cms.vdouble( 1.0000 ) , uncertainties = cms.vdouble( 0.0500 ) ),
        cms.PSet( lowBounds = cms.vdouble( 1.5660, 10.0000 ) , upBounds = cms.vdouble( 2.0000, 20.0000 ) , values = cms.vdouble( 0.9031 ) , uncertainties = cms.vdouble( 0.0500 ) ),
        cms.PSet( lowBounds = cms.vdouble( 1.5660, 20.0000 ) , upBounds = cms.vdouble( 2.0000, 30.0000 ) , values = cms.vdouble( 0.9345 ) , uncertainties = cms.vdouble( 0.0295 ) ),
        cms.PSet( lowBounds = cms.vdouble( 1.5660, 30.0000 ) , upBounds = cms.vdouble( 2.0000, 40.0000 ) , values = cms.vdouble( 0.9666 ) , uncertainties = cms.vdouble( 0.0101 ) ),
        cms.PSet( lowBounds = cms.vdouble( 1.5660, 40.0000 ) , upBounds = cms.vdouble( 2.0000, 50.0000 ) , values = cms.vdouble( 0.9840 ) , uncertainties = cms.vdouble( 0.0032 ) ),
        cms.PSet( lowBounds = cms.vdouble( 1.5660, 50.0000 ) , upBounds = cms.vdouble( 2.0000, 1000.0000 ) , values = cms.vdouble( 1.0046 ) , uncertainties = cms.vdouble( 0.0043 ) ),
        
        cms.PSet( lowBounds = cms.vdouble( 2.0000,  0.0000 ) , upBounds = cms.vdouble( 5.0000, 10.0000 ) , values = cms.vdouble( 1.0000 ) , uncertainties = cms.vdouble( 0.0214 ) ),
        cms.PSet( lowBounds = cms.vdouble( 2.0000, 10.0000 ) , upBounds = cms.vdouble( 5.0000, 20.0000 ) , values = cms.vdouble( 0.9637 ) , uncertainties = cms.vdouble( 0.0214 ) ),
        cms.PSet( lowBounds = cms.vdouble( 2.0000, 20.0000 ) , upBounds = cms.vdouble( 5.0000, 30.0000 ) , values = cms.vdouble( 0.9723 ) , uncertainties = cms.vdouble( 0.0124 ) ),
        cms.PSet( lowBounds = cms.vdouble( 2.0000, 30.0000 ) , upBounds = cms.vdouble( 5.0000, 40.0000 ) , values = cms.vdouble( 0.9853 ) , uncertainties = cms.vdouble( 0.0106 ) ),
        cms.PSet( lowBounds = cms.vdouble( 2.0000, 40.0000 ) , upBounds = cms.vdouble( 5.0000, 50.0000 ) , values = cms.vdouble( 0.9977 ) , uncertainties = cms.vdouble( 0.0068 ) ),
        cms.PSet( lowBounds = cms.vdouble( 2.0000, 50.0000 ) , upBounds = cms.vdouble( 5.0000, 1000.0000 ) , values = cms.vdouble( 0.9983 ) , uncertainties = cms.vdouble( 0.0115 ) )
        
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
