import FWCore.ParameterSet.Config as cms
import os, json, re
from collections import OrderedDict

class EleSF_JSONReader :
    def __init__(self , file_name , sf_name ):
        self.Name = sf_name
        self.JSONFileName = os.path.expanduser( os.path.expandvars(str('$CMSSW_BASE/src/')+file_name) )
        with open( self.JSONFileName , 'r' ) as f :
            j = json.load( f ,object_pairs_hook=OrderedDict)
            self.Info = j[ sf_name ]
        self.binInfo = cms.PSet(
            variables = cms.vstring( "eta" , "pt" ),
            bins = cms.VPSet()
            )
        format_bins = r'^(?P<VarName>.*):\[(?P<From>-?\d*.\d*),(?P<To>-?\d*.\d*)\]'
        for pt_region in self.Info :
            pt_values = re.match( format_bins , pt_region , re.M|re.I )
            if pt_values and len( pt_values.groups() ) == 3 :
                pt_from = float( pt_values.group("From") )
                pt_to = float( pt_values.group("To") )

                for eta_region in self.Info[pt_region]:
                    eta_values = re.match( format_bins , eta_region , re.M|re.I )
                    sf_value = self.Info[pt_region][eta_region]["value"]
                    sf_error = self.Info[pt_region][eta_region]["error"]
                    eta_from = float( eta_values.group("From") )
                    eta_to = float( eta_values.group("To") )
                    
                    self.binInfo.bins.append(
                        cms.PSet(
                            lowBounds = cms.vdouble( eta_from , pt_from ) ,
                            upBounds = cms.vdouble( eta_to , pt_to ) ,
                            values = cms.vdouble( sf_value ) ,
                            uncertainties = cms.vdouble( sf_error )
                        )
                    )

                    if pt_to == 500.0:
                        #extend the extremes to prevent any crashes
                        self.binInfo.bins.append(
                            cms.PSet(
                                lowBounds = cms.vdouble( eta_from , pt_to ) ,
                                upBounds = cms.vdouble( eta_to , 9999 ) ,
                                values = cms.vdouble( sf_value ) ,
                                uncertainties = cms.vdouble( sf_error )
                            )
                        )

        
        

            else :
                print pt_region, "of sf" , sf_name , "from file", file_name , "can not be interpreted correctly"


        #SFs valid only from 10GeV, eta 2.5, bins extended to extremes to prevent any crashes 
        self.binInfo.bins.append(
            cms.PSet(
                lowBounds = cms.vdouble( -10.0000 , 0.00000 ) ,
                upBounds = cms.vdouble( -2.5 , 99999.0000 ) ,
                values = cms.vdouble( 1.0 ) ,
                uncertainties = cms.vdouble(0.0)
            )
        )
        
        self.binInfo.bins.append(
            cms.PSet( lowBounds = cms.vdouble( 2.5000, 0.0000 ) , 
                      upBounds = cms.vdouble( 10.0000, 999999999.0000 ) , 
                      values = cms.vdouble(  1.0 ) ,
                      uncertainties = cms.vdouble( 0.001 ) )
        )

        self.binInfo.bins.append(
            cms.PSet( lowBounds = cms.vdouble( -10, 0.0000 ) , 
                      upBounds = cms.vdouble( 10.0000, 10.0000 ) , 
                      values = cms.vdouble(  1.0 ) ,
                      uncertainties = cms.vdouble( 0.001 ) )
        )



    def getBinInfo(self):
        return self.binInfo
         


binInfo2016 = cms.PSet(
    variables = cms.vstring("eta","pt"),
    bins = cms.VPSet(
        # Medium MVA WP scale factors  for Fall17V2 ID 
        # Note : SFs valid only from 10GeV, eta 2.5, bins extended to extremes to prevent any crashes 
        
	cms.PSet( lowBounds = cms.vdouble( -6.0000, 0.0000 ) , upBounds = cms.vdouble( -2.5000, 9999999.0000 ) , values = cms.vdouble( 1. ) , uncertainties = cms.vdouble( 0.0001 ) ),
    
        cms.PSet( lowBounds = cms.vdouble( -2.5 , 0 ) , upBounds = cms.vdouble( -2, 10 ) , values = cms.vdouble(1.0)  , uncertainties = cms.vdouble(0.0001) ),
        cms.PSet( lowBounds = cms.vdouble( -2.5 , 10 ) , upBounds = cms.vdouble( -2, 20 ) , values = cms.vdouble(1.014) , uncertainties = cms.vdouble(0.021) ),
        cms.PSet( lowBounds = cms.vdouble( -2.5 , 20 ) , upBounds = cms.vdouble( -2, 35 ) , values = cms.vdouble(0.991) , uncertainties = cms.vdouble(0.009) ),
        cms.PSet( lowBounds = cms.vdouble( -2.5 , 35 ) , upBounds = cms.vdouble( -2, 50 ) , values = cms.vdouble(0.999) , uncertainties = cms.vdouble(0.006) ),
        cms.PSet( lowBounds = cms.vdouble( -2.5 , 50 ) , upBounds = cms.vdouble( -2, 100 ) , values = cms.vdouble(0.995) , uncertainties = cms.vdouble(0.008) ),
     cms.PSet( lowBounds = cms.vdouble( -2.5 , 100 ) , upBounds = cms.vdouble( -2, 200 ) , values = cms.vdouble( 1.014 ) , uncertainties = cms.vdouble(0.032  ) ),
     cms.PSet( lowBounds = cms.vdouble( -2.5 , 200 ) , upBounds = cms.vdouble( -2, 500 ) , values = cms.vdouble( 0.966 ) , uncertainties = cms.vdouble(0.089  ) ),
     cms.PSet( lowBounds = cms.vdouble( -2.5 , 500 ) , upBounds = cms.vdouble( -2, 999999999 ) , values = cms.vdouble( 0.966 ) , uncertainties = cms.vdouble(0.089  ) ),

        cms.PSet( lowBounds = cms.vdouble( -2 , 0 ) , upBounds = cms.vdouble( -1.566, 10 ) , values = cms.vdouble(1.0) , uncertainties = cms.vdouble(0.0001) ),
     cms.PSet( lowBounds = cms.vdouble( -2 , 10 ) , upBounds = cms.vdouble( -1.566, 20 ) , values = cms.vdouble( 0.962 ) , uncertainties = cms.vdouble(0.017 ) ),
     cms.PSet( lowBounds = cms.vdouble( -2 , 20 ) , upBounds = cms.vdouble( -1.566, 35 ) , values = cms.vdouble( 0.951 ) , uncertainties = cms.vdouble(0.012 ) ),
     cms.PSet( lowBounds = cms.vdouble( -2 , 35 ) , upBounds = cms.vdouble( -1.566, 50 ) , values = cms.vdouble( 0.968 ) , uncertainties = cms.vdouble(0.010  ) ),
     cms.PSet( lowBounds = cms.vdouble( -2 , 50 ) , upBounds = cms.vdouble( -1.566, 100 ) , values = cms.vdouble( 0.977 ) , uncertainties = cms.vdouble(0.002  ) ),
        cms.PSet( lowBounds = cms.vdouble( -2 , 100 ) , upBounds = cms.vdouble( -1.566, 200 ) , values = cms.vdouble( 0.983 ) , uncertainties = cms.vdouble(0.010) ),
     cms.PSet( lowBounds = cms.vdouble( -2 , 200 ) , upBounds = cms.vdouble( -1.566, 500 ) , values = cms.vdouble( 0.995 ) , uncertainties = cms.vdouble(0.043  ) ),
     cms.PSet( lowBounds = cms.vdouble( -2 , 500 ) , upBounds = cms.vdouble( -1.566, 99999999 ) , values = cms.vdouble( 0.995 ) , uncertainties = cms.vdouble(0.043  ) ),

     cms.PSet( lowBounds = cms.vdouble( -1.566 , 0 ) , upBounds = cms.vdouble( -1.444, 10 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.566 , 10 ) , upBounds = cms.vdouble( -1.444, 20 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.566 , 20 ) , upBounds = cms.vdouble( -1.444, 35 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.566 , 35 ) , upBounds = cms.vdouble( -1.444, 50 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.566 , 50 ) , upBounds = cms.vdouble( -1.444, 100 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.566 , 100 ) , upBounds = cms.vdouble( -1.444, 200 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.566 , 200 ) , upBounds = cms.vdouble( -1.444, 500 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.566 , 500 ) , upBounds = cms.vdouble( -1.444, 9999999 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),

     cms.PSet( lowBounds = cms.vdouble( -1.566 , 0 ) , upBounds = cms.vdouble( -1.444, 10 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.566 , 10 ) , upBounds = cms.vdouble( -1.444, 20 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.566 , 20 ) , upBounds = cms.vdouble( -1.444, 35 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.566 , 35 ) , upBounds = cms.vdouble( -1.444, 50 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.566 , 50 ) , upBounds = cms.vdouble( -1.444, 100 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.566 , 100 ) , upBounds = cms.vdouble( -1.444, 200 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.566 , 200 ) , upBounds = cms.vdouble( -1.444, 500 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.566 , 500 ) , upBounds = cms.vdouble( -1.444, 9999999 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
        

    cms.PSet( lowBounds = cms.vdouble( -0.8 , 0 ) , upBounds = cms.vdouble( 0, 10 ) , values = cms.vdouble( 1.0 ) , uncertainties = cms.vdouble(0.001  ) ),
    cms.PSet( lowBounds = cms.vdouble( -0.8 , 10 ) , upBounds = cms.vdouble( 0, 20 ) , values = cms.vdouble( 0.953 ) , uncertainties = cms.vdouble(0.015  ) ),
    cms.PSet( lowBounds = cms.vdouble( -0.8 , 20 ) , upBounds = cms.vdouble( 0, 35 ) , values = cms.vdouble( 0.952) , uncertainties = cms.vdouble(0.015  ) ),
    cms.PSet( lowBounds = cms.vdouble( -0.8 , 35 ) , upBounds = cms.vdouble( 0, 50 ) , values = cms.vdouble( 0.957 ) , uncertainties = cms.vdouble(0.002  ) ),
    cms.PSet( lowBounds = cms.vdouble( -0.8 , 50 ) , upBounds = cms.vdouble( 0, 100 ) , values = cms.vdouble( 0.963 ) , uncertainties = cms.vdouble(0.003  ) ),
    cms.PSet( lowBounds = cms.vdouble( -0.8 , 100 ) , upBounds = cms.vdouble( 0, 200 ) , values = cms.vdouble( 0.982 ) , uncertainties = cms.vdouble(0.009  ) ),
    cms.PSet( lowBounds = cms.vdouble( -0.8 , 200 ) , upBounds = cms.vdouble( 0, 500 ) , values = cms.vdouble( 0.979 ) , uncertainties = cms.vdouble(0.023  ) ),
    cms.PSet( lowBounds = cms.vdouble( -0.8 , 500 ) , upBounds = cms.vdouble( 0, 99999999 ) , values = cms.vdouble( 0.979 ) , uncertainties = cms.vdouble(0.023  ) ),

     cms.PSet( lowBounds = cms.vdouble( 0 , 0 ) , upBounds = cms.vdouble( 0.8, 10 ) , values = cms.vdouble( 1.0 ) , uncertainties = cms.vdouble(0.001  ) ),
     cms.PSet( lowBounds = cms.vdouble( 0 , 10 ) , upBounds = cms.vdouble( 0.8, 20 ) , values = cms.vdouble( 0.954 ) , uncertainties = cms.vdouble(0.015 ) ),
     cms.PSet( lowBounds = cms.vdouble( 0 , 20 ) , upBounds = cms.vdouble( 0.8, 35 ) , values = cms.vdouble( 0.966 ) , uncertainties = cms.vdouble(0.015 ) ),
     cms.PSet( lowBounds = cms.vdouble( 0 , 35 ) , upBounds = cms.vdouble( 0.8, 50 ) , values = cms.vdouble( 0.976 ) , uncertainties = cms.vdouble(0.002  ) ),
        cms.PSet( lowBounds = cms.vdouble( 0 , 50 ) , upBounds = cms.vdouble( 0.8, 100 ) , values = cms.vdouble(  0.979) , uncertainties = cms.vdouble(0.003  ) ),
        cms.PSet( lowBounds = cms.vdouble( 0 , 100 ) , upBounds = cms.vdouble( 0.8, 200 ) , values = cms.vdouble( 0.991) , uncertainties = cms.vdouble(0.009  ) ),
        cms.PSet( lowBounds = cms.vdouble( 0 , 200 ) , upBounds = cms.vdouble( 0.8, 500 ) , values = cms.vdouble( 0.976) , uncertainties = cms.vdouble(0.023  ) ),
     cms.PSet( lowBounds = cms.vdouble( 0 , 500 ) , upBounds = cms.vdouble( 0.8, 99999999 ) , values = cms.vdouble( 0.976 ) , uncertainties = cms.vdouble(0.023  ) ),

     cms.PSet( lowBounds = cms.vdouble( 0.8 , 0 ) , upBounds = cms.vdouble( 1.444, 10 ) , values = cms.vdouble( 1.0 ) , uncertainties = cms.vdouble(0.001  ) ),
     cms.PSet( lowBounds = cms.vdouble( 0.8 , 10 ) , upBounds = cms.vdouble( 1.444, 20 ) , values = cms.vdouble( 0.974 ) , uncertainties = cms.vdouble(0.11  ) ),
     cms.PSet( lowBounds = cms.vdouble( 0.8 , 20 ) , upBounds = cms.vdouble( 1.444, 35 ) , values = cms.vdouble( 0.947 ) , uncertainties = cms.vdouble(0.018  ) ),
     cms.PSet( lowBounds = cms.vdouble( 0.8 , 35 ) , upBounds = cms.vdouble( 1.444, 50 ) , values = cms.vdouble( 0.965 ) , uncertainties = cms.vdouble(0.004  ) ),
     cms.PSet( lowBounds = cms.vdouble( 0.8 , 50 ) , upBounds = cms.vdouble( 1.444, 100 ) , values = cms.vdouble( 0.970 ) , uncertainties = cms.vdouble(0.003  ) ),
        cms.PSet( lowBounds = cms.vdouble( 0.8 , 100 ) , upBounds = cms.vdouble( 1.444, 200 ) , values = cms.vdouble( 0.990) , uncertainties = cms.vdouble(0.011  ) ),
     cms.PSet( lowBounds = cms.vdouble( 0.8 , 200 ) , upBounds = cms.vdouble( 1.444, 500 ) , values = cms.vdouble( 0.984 ) , uncertainties = cms.vdouble(0.025  ) ),
     cms.PSet( lowBounds = cms.vdouble( 0.8 , 500 ) , upBounds = cms.vdouble( 1.444, 9999999 ) , values = cms.vdouble( 0.984 ) , uncertainties = cms.vdouble(0.025  ) ),

     cms.PSet( lowBounds = cms.vdouble( 1.444 , 0 ) , upBounds = cms.vdouble( 1.566, 10 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
    cms.PSet( lowBounds = cms.vdouble( 1.444 , 10 ) , upBounds = cms.vdouble( 1.566, 20 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.444 , 20 ) , upBounds = cms.vdouble( 1.566, 35 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.444 , 35 ) , upBounds = cms.vdouble( 1.566, 50 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.444 , 50 ) , upBounds = cms.vdouble( 1.566, 100 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.444 , 100 ) , upBounds = cms.vdouble( 1.566, 200 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.444 , 200 ) , upBounds = cms.vdouble( 1.566, 500 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.444 , 500 ) , upBounds = cms.vdouble( 1.566, 9999999 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),

     cms.PSet( lowBounds = cms.vdouble( 1.566 , 0 ) , upBounds = cms.vdouble( 2, 10 ) , values = cms.vdouble( 1.0 ) , uncertainties = cms.vdouble(0.001  ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.566 , 10 ) , upBounds = cms.vdouble( 2, 20 ) , values = cms.vdouble( 0.971 ) , uncertainties = cms.vdouble(0.017 ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.566 , 20 ) , upBounds = cms.vdouble( 2, 35 ) , values = cms.vdouble( 0.933 ) , uncertainties = cms.vdouble(0.012 ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.566 , 35 ) , upBounds = cms.vdouble( 2, 50 ) , values = cms.vdouble( 0.957 ) , uncertainties = cms.vdouble(0.010  ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.566 , 50 ) , upBounds = cms.vdouble( 2, 100 ) , values = cms.vdouble(  0.968 ) , uncertainties = cms.vdouble(0.002  ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.566 , 100 ) , upBounds = cms.vdouble( 2, 200 ) , values = cms.vdouble( 0.995 ) , uncertainties = cms.vdouble(0.011 ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.566 , 200 ) , upBounds = cms.vdouble( 2, 500 ) , values = cms.vdouble( 0.952 ) , uncertainties = cms.vdouble(0.043  ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.566 , 500 ) , upBounds = cms.vdouble( 2, 99999999 ) , values = cms.vdouble( 0.952 ) , uncertainties = cms.vdouble(0.043  ) ),

     cms.PSet( lowBounds = cms.vdouble( 2 , 0 ) , upBounds = cms.vdouble( 2.5, 10 ) , values = cms.vdouble( 1.0 ) , uncertainties = cms.vdouble(0.001  ) ),
     cms.PSet( lowBounds = cms.vdouble( 2 , 10 ) , upBounds = cms.vdouble( 2.5, 20 ) , values = cms.vdouble( 0.961 ) , uncertainties = cms.vdouble(0.021  ) ),
     cms.PSet( lowBounds = cms.vdouble( 2 , 20 ) , upBounds = cms.vdouble( 2.5, 35 ) , values = cms.vdouble( 0.953 ) , uncertainties = cms.vdouble(0.009  ) ),
     cms.PSet( lowBounds = cms.vdouble( 2 , 35 ) , upBounds = cms.vdouble( 2.5, 50 ) , values = cms.vdouble( 0.961 ) , uncertainties = cms.vdouble(0.006 ) ),
     cms.PSet( lowBounds = cms.vdouble( 2 , 50 ) , upBounds = cms.vdouble( 2.5, 100 ) , values = cms.vdouble(  0.969 ) , uncertainties = cms.vdouble(0.008  ) ),
     cms.PSet( lowBounds = cms.vdouble( 2 , 100 ) , upBounds = cms.vdouble( 2.5, 200 ) , values = cms.vdouble( 0.994 ) , uncertainties = cms.vdouble(0.032  ) ),
     cms.PSet( lowBounds = cms.vdouble( 2 , 200 ) , upBounds = cms.vdouble( 2.5, 500 ) , values = cms.vdouble( 1.018 ) , uncertainties = cms.vdouble(0.090  ) ),	
     cms.PSet( lowBounds = cms.vdouble( 2 , 500 ) , upBounds = cms.vdouble( 2.5, 999999999 ) , values = cms.vdouble( 1.018 ) , uncertainties = cms.vdouble(0.090  ) ),	

        	cms.PSet( lowBounds = cms.vdouble( 2.5000, 0.0000 ) , upBounds = cms.vdouble( 6.0000, 999999999.0000 ) , values = cms.vdouble(  1.0 ) , uncertainties = cms.vdouble( 0.001 ) )

        )
    )	



#setting up id scale factors
flashggElectronSystematics = cms.EDProducer('FlashggElectronEffSystematicProducer',
                                            src = cms.InputTag("flashggSelectedElectrons"),
                                            SystMethods2D = cms.VPSet(),
                                            SystMethods = cms.VPSet(cms.PSet( MethodName = cms.string("FlashggElectronWeight"),
                                                                              Label = cms.string("ElectronIDWeight"),
                                                                              NSigmas = cms.vint32(-1,1),
                                                                              OverallRange = cms.string("abs(eta)<2.5"),
                                                                              BinList = binInfo2016,
                                                                              Debug = cms.untracked.bool(False),
                                                                              ApplyCentralValue = cms.bool(True)
                                                                              )	
                                                                    )
                                            )

flashggElectronSystematics.SystMethods.append(cms.PSet( MethodName = cms.string("FlashggElectronWeight"),
                                                        Label = cms.string("ElectronRecoWeight"),
                                                        NSigmas = cms.vint32(-1,1),
                                                        OverallRange = cms.string("abs(eta)<2.5"),
                                                        BinList = binInfo2016,
                                                        Debug = cms.untracked.bool(False),
                                                        ApplyCentralValue = cms.bool(True)
                                                    )	
                                          )
