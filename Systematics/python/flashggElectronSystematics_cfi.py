import FWCore.ParameterSet.Config as cms


binInfo2018 = cms.PSet(
    variables = cms.vstring("eta","pt"),
    bins = cms.VPSet(
        # Medium MVA WP scale factors  for Fall17V2 ID 
        #         # Note : SFs valid only from 10GeV, eta 2.5, bins extended to extremes to prevent any crashes 
        
	cms.PSet( lowBounds = cms.vdouble( -6.0000, 0.0000 ) , upBounds = cms.vdouble( -2.5000, 9999999.0000 ) , values = cms.vdouble( 1. ) , uncertainties = cms.vdouble( 0.0001 ) ),
    
     cms.PSet( lowBounds = cms.vdouble( -2.5 , 0 ) , upBounds = cms.vdouble( -2, 10 ) , values = cms.vdouble( 1.0 ) , uncertainties = cms.vdouble(0.001  ) ),
        cms.PSet( lowBounds = cms.vdouble( -2.5 , 10 ) , upBounds = cms.vdouble( -2, 20 ) ,  values = cms.vdouble(0.994 ) , uncertainties = cms.vdouble(0.0178 ) ),
        cms.PSet( lowBounds = cms.vdouble( -2.5 , 20 ) , upBounds = cms.vdouble( -2, 35 ) ,  values = cms.vdouble(0.979 ) , uncertainties = cms.vdouble(0.0187 ) ),
        cms.PSet( lowBounds = cms.vdouble( -2.5 , 35 ) , upBounds = cms.vdouble( -2, 50 ) ,  values = cms.vdouble(0.979) , uncertainties =  cms.vdouble(0.00577) ),
        cms.PSet( lowBounds = cms.vdouble( -2.5 , 50 ) , upBounds = cms.vdouble( -2, 100 ) , values = cms.vdouble(0.978) , uncertainties =  cms.vdouble(0.00603) ),
        cms.PSet( lowBounds = cms.vdouble( -2.5 , 100 ) , upBounds = cms.vdouble( -2, 200 ) ,values = cms.vdouble(0.988) , uncertainties =  cms.vdouble(0.0255 ) ),
     cms.PSet( lowBounds = cms.vdouble( -2.5 , 200 ) , upBounds = cms.vdouble( -2, 500 ) ,   values = cms.vdouble(0.971) , uncertainties =  cms.vdouble(0.0933 ) ),
        cms.PSet( lowBounds = cms.vdouble( -2.5 , 500 ) , upBounds = cms.vdouble( -2, 999999999 ) , values = cms.vdouble( 0.971) , uncertainties = cms.vdouble(0.0933) ),

        cms.PSet( lowBounds = cms.vdouble( -2 , 0 ) , upBounds = cms.vdouble( -1.566, 10 ) , values = cms.vdouble( 1.0) , uncertainties = cms.vdouble(0.001  ) ),
     cms.PSet( lowBounds = cms.vdouble( -2 , 10 ) , upBounds = cms.vdouble( -1.566, 20 ) , values = cms.vdouble(0.976) , uncertainties = cms.vdouble(0.0229 ) ),
     cms.PSet( lowBounds = cms.vdouble( -2 , 20 ) , upBounds = cms.vdouble( -1.566, 35 ) , values = cms.vdouble(0.959) , uncertainties = cms.vdouble(0.0202 ) ),
     cms.PSet( lowBounds = cms.vdouble( -2 , 35 ) , upBounds = cms.vdouble( -1.566, 50 ) , values = cms.vdouble(0.971) , uncertainties = cms.vdouble(0.00368 ) ),
     cms.PSet( lowBounds = cms.vdouble( -2 , 50 ) , upBounds = cms.vdouble( -1.566, 100 ) , values = cms.vdouble(0.98) , uncertainties = cms.vdouble(0.0029 ) ),
     cms.PSet( lowBounds = cms.vdouble( -2 , 100 ) , upBounds = cms.vdouble( -1.566, 200 ) , values = cms.vdouble(0.998) , uncertainties = cms.vdouble(0.0121) ),
     cms.PSet( lowBounds = cms.vdouble( -2 , 200 ) , upBounds = cms.vdouble( -1.566, 500 ) , values = cms.vdouble(0.99 ) , uncertainties = cms.vdouble(0.0439) ),
     cms.PSet( lowBounds = cms.vdouble( -2 , 500 ) , upBounds = cms.vdouble( -1.566, 99999999 ) , values = cms.vdouble(0.99  ) , uncertainties = cms.vdouble(0.0439  ) ),

     cms.PSet( lowBounds = cms.vdouble( -1.566 , 0 ) , upBounds = cms.vdouble( -1.444, 10 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.566 , 10 ) , upBounds = cms.vdouble( -1.444, 20 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.566 , 20 ) , upBounds = cms.vdouble( -1.444, 35 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.566 , 35 ) , upBounds = cms.vdouble( -1.444, 50 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.566 , 50 ) , upBounds = cms.vdouble( -1.444, 100 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.566 , 100 ) , upBounds = cms.vdouble( -1.444, 200 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.566 , 200 ) , upBounds = cms.vdouble( -1.444, 500 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.566 , 500 ) , upBounds = cms.vdouble( -1.444, 9999999 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),

        cms.PSet( lowBounds = cms.vdouble( -1.444 , 0 ) , upBounds = cms.vdouble( -0.8, 10 ) , values = cms.vdouble( 1.0) , uncertainties = cms.vdouble(0.001  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.444 , 10 ) , upBounds = cms.vdouble( -0.8, 20 ) , values =  cms.vdouble(1     ) , uncertainties = cms.vdouble(0.0151  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.444 , 20 ) , upBounds = cms.vdouble( -0.8, 35 ) , values =  cms.vdouble(0.967 ) , uncertainties = cms.vdouble(0.0271  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.444 , 35 ) , upBounds = cms.vdouble( -0.8, 50 ) , values =  cms.vdouble(0.972 ) , uncertainties = cms.vdouble(0.00354 ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.444 , 50 ) , upBounds = cms.vdouble( -0.8, 100 ) , values = cms.vdouble(0.973 ) , uncertainties = cms.vdouble(0.00568  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.444 , 100 ) , upBounds = cms.vdouble( -0.8, 200 ) , values =cms.vdouble(0.981 ) , uncertainties = cms.vdouble(0.0207 ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.444 , 200 ) , upBounds = cms.vdouble( -0.8, 500 ) , values =cms.vdouble(0.969 ) , uncertainties = cms.vdouble(0.0298 ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.444 , 500 ) , upBounds = cms.vdouble( -0.8, 99999999 ) , values = cms.vdouble( 0.969 ) , uncertainties = cms.vdouble(0.0298  ) ),

     cms.PSet( lowBounds = cms.vdouble( -0.8 , 0 ) , upBounds = cms.vdouble( 0, 10 ) , values =    cms.vdouble(1.0 ) , uncertainties =  cms.vdouble(0.001  ) ),
    cms.PSet( lowBounds = cms.vdouble( -0.8 , 10 ) , upBounds = cms.vdouble( 0, 20 ) , values =    cms.vdouble(0.958) , uncertainties = cms.vdouble( 0.0195  ) ),
     cms.PSet( lowBounds = cms.vdouble( -0.8 , 20 ) , upBounds = cms.vdouble( 0, 35 ) , values =   cms.vdouble(0.968) , uncertainties = cms.vdouble( 0.0179  ) ),
     cms.PSet( lowBounds = cms.vdouble( -0.8 , 35 ) , upBounds = cms.vdouble( 0, 50 ) , values =   cms.vdouble(0.976 ) , uncertainties = cms.vdouble(0.00296  ) ),
     cms.PSet( lowBounds = cms.vdouble( -0.8 , 50 ) , upBounds = cms.vdouble( 0, 100 ) , values =  cms.vdouble(0.975 ) , uncertainties = cms.vdouble(0.00429  ) ),
     cms.PSet( lowBounds = cms.vdouble( -0.8 , 100 ) , upBounds = cms.vdouble( 0, 200 ) , values = cms.vdouble(0.976 ) , uncertainties = cms.vdouble(0.0115   ) ),
     cms.PSet( lowBounds = cms.vdouble( -0.8 , 200 ) , upBounds = cms.vdouble( 0, 500 ) , values = cms.vdouble(0.991 ) , uncertainties = cms.vdouble(0.036    ) ),
        cms.PSet( lowBounds = cms.vdouble( -0.8 , 500 ) , upBounds = cms.vdouble( 0, 99999999 ) , values = cms.vdouble( 0.991) , uncertainties = cms.vdouble(0.036  ) ),

     cms.PSet( lowBounds = cms.vdouble( 0 , 0 ) , upBounds = cms.vdouble( 0.8, 10 ) , values =   cms.vdouble(1.0 ) , uncertainties = cms.vdouble(0.0131015  ) ),
     cms.PSet( lowBounds = cms.vdouble( 0 , 10 ) , upBounds = cms.vdouble( 0.8, 20 ) , values =  cms.vdouble( 0.99 ) , uncertainties = cms.vdouble(0.0195) ),
     cms.PSet( lowBounds = cms.vdouble( 0 , 20 ) , upBounds = cms.vdouble( 0.8, 35 ) , values =  cms.vdouble( 0.974) , uncertainties = cms.vdouble(0.0179) ),
     cms.PSet( lowBounds = cms.vdouble( 0 , 35 ) , upBounds = cms.vdouble( 0.8, 50 ) , values =  cms.vdouble( 0.977) , uncertainties = cms.vdouble(0.0029 ) ),
     cms.PSet( lowBounds = cms.vdouble( 0 , 50 ) , upBounds = cms.vdouble( 0.8, 100 ) , values = cms.vdouble( 0.978) , uncertainties = cms.vdouble(0.0042) ),
     cms.PSet( lowBounds = cms.vdouble( 0 , 100 ) , upBounds = cms.vdouble( 0.8, 200 ) , values =cms.vdouble( 0.983) , uncertainties = cms.vdouble(0.0116 ) ),
     cms.PSet( lowBounds = cms.vdouble( 0 , 200 ) , upBounds = cms.vdouble( 0.8, 500 ) , values =cms.vdouble( 0.949) , uncertainties = cms.vdouble(0.0354) ),
     cms.PSet( lowBounds = cms.vdouble( 0 , 500 ) , upBounds = cms.vdouble( 0.8, 99999999 ) , values = cms.vdouble(0.949 ) , uncertainties = cms.vdouble(0.0354  ) ),

     cms.PSet( lowBounds = cms.vdouble( 0.8 , 0 ) , upBounds = cms.vdouble( 1.444, 10 ) , values = cms.vdouble(1.0 ) , uncertainties = cms.vdouble(0.010  ) ),
     cms.PSet( lowBounds = cms.vdouble( 0.8 , 10 ) , upBounds = cms.vdouble( 1.444, 20 ) , values = cms.vdouble(1.02  ) , uncertainties = cms.vdouble(0.0151  ) ),
     cms.PSet( lowBounds = cms.vdouble( 0.8 , 20 ) , upBounds = cms.vdouble( 1.444, 35 ) , values = cms.vdouble(0.966) , uncertainties = cms.vdouble (0.0271  ) ),
     cms.PSet( lowBounds = cms.vdouble( 0.8 , 35 ) , upBounds = cms.vdouble( 1.444, 50 ) , values = cms.vdouble(0.972) , uncertainties = cms.vdouble (0.00354 ) ),
     cms.PSet( lowBounds = cms.vdouble( 0.8 , 50 ) , upBounds = cms.vdouble( 1.444, 100 ) ,values = cms.vdouble(0.975) , uncertainties = cms.vdouble (0.00568 ) ),
     cms.PSet( lowBounds = cms.vdouble( 0.8 , 100 ) , upBounds = cms.vdouble( 1.444, 200 ),values = cms.vdouble(0.987 ) , uncertainties = cms.vdouble(0.0208 ) ),
     cms.PSet( lowBounds = cms.vdouble( 0.8 , 200 ) , upBounds = cms.vdouble( 1.444, 500 ),values = cms.vdouble(0.96 ) , uncertainties = cms.vdouble (0.0285  ) ),
     cms.PSet( lowBounds = cms.vdouble( 0.8 , 500 ) , upBounds = cms.vdouble( 1.444, 9999999 ) , values = cms.vdouble(0.96 ) , uncertainties = cms.vdouble(0.0285  ) ),

     cms.PSet( lowBounds = cms.vdouble( 1.444 , 0 ) , upBounds = cms.vdouble( 1.566, 10 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
    cms.PSet( lowBounds = cms.vdouble( 1.444 , 10 ) , upBounds = cms.vdouble( 1.566, 20 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.444 , 20 ) , upBounds = cms.vdouble( 1.566, 35 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.444 , 35 ) , upBounds = cms.vdouble( 1.566, 50 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.444 , 50 ) , upBounds = cms.vdouble( 1.566, 100 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.444 , 100 ) , upBounds = cms.vdouble( 1.566, 200 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.444 , 200 ) , upBounds = cms.vdouble( 1.566, 500 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.444 , 500 ) , upBounds = cms.vdouble( 1.566, 9999999 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),

     cms.PSet( lowBounds = cms.vdouble( 1.566 , 0 ) , upBounds = cms.vdouble( 2, 10 ) , values = cms.vdouble( 1.0 ) , uncertainties = cms.vdouble(0.001  ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.566 , 10 ) , upBounds = cms.vdouble( 2, 20 ) , values =   cms.vdouble(0.99 ) , uncertainties = cms.vdouble(0.0229 ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.566 , 20 ) , upBounds = cms.vdouble( 2, 35 ) , values =   cms.vdouble(0.954) , uncertainties = cms.vdouble(0.0202 ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.566 , 35 ) , upBounds = cms.vdouble( 2, 50 ) , values =   cms.vdouble(0.969) , uncertainties = cms.vdouble(0.00368 ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.566 , 50 ) , upBounds = cms.vdouble( 2, 100 ) , values =  cms.vdouble(0.981) , uncertainties = cms.vdouble(0.0029  ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.566 , 100 ) , upBounds = cms.vdouble( 2, 200 ) , values = cms.vdouble(0.977) , uncertainties = cms.vdouble(0.0115 ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.566 , 200 ) , upBounds = cms.vdouble( 2, 500 ) , values = cms.vdouble(0.941) , uncertainties = cms.vdouble(0.0395 ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.566 , 500 ) , upBounds = cms.vdouble( 2, 99999999 ) , values = cms.vdouble( 0.941 ) , uncertainties = cms.vdouble(0.0395 ) ),

        cms.PSet( lowBounds = cms.vdouble( 2 , 0 ) , upBounds = cms.vdouble( 2.5, 10 ) , values = cms.vdouble( 1.0) , uncertainties = cms.vdouble(0.001 ) ),
     cms.PSet( lowBounds = cms.vdouble( 2 , 10 ) , upBounds = cms.vdouble( 2.5, 20 ) , values =   cms.vdouble(0.995)  , uncertainties = cms.vdouble(0.0178  ) ),
     cms.PSet( lowBounds = cms.vdouble( 2 , 20 ) , upBounds = cms.vdouble( 2.5, 35 ) , values =   cms.vdouble(0.968)  , uncertainties = cms.vdouble(0.0187  ) ),
     cms.PSet( lowBounds = cms.vdouble( 2 , 35 ) , upBounds = cms.vdouble( 2.5, 50 ) , values =   cms.vdouble(0.972)  , uncertainties = cms.vdouble(0.00577 ) ),
     cms.PSet( lowBounds = cms.vdouble( 2 , 50 ) , upBounds = cms.vdouble( 2.5, 100 ) , values =  cms.vdouble(0.955) , uncertainties =  cms.vdouble(0.00603) ),
     cms.PSet( lowBounds = cms.vdouble( 2 , 100 ) , upBounds = cms.vdouble( 2.5, 200 ) , values = cms.vdouble(0.965 ) , uncertainties = cms.vdouble(0.0252  ) ),
     cms.PSet( lowBounds = cms.vdouble( 2 , 200 ) , upBounds = cms.vdouble( 2.5, 500 ) , values = cms.vdouble(0.976 ) , uncertainties = cms.vdouble(0.0875  ) ),	
     cms.PSet( lowBounds = cms.vdouble( 2 , 500 ) , upBounds = cms.vdouble( 2.5, 999999999 ) , values = cms.vdouble( 0.976 ) , uncertainties = cms.vdouble(0.0875  ) ),	

        	cms.PSet( lowBounds = cms.vdouble( 2.5000, 0.0000 ) , upBounds = cms.vdouble( 6.0000, 999999999.0000 ) , values = cms.vdouble(  1.0 ) , uncertainties = cms.vdouble( 0.001 ) )

        )
    )	


binInfo2017 = cms.PSet(
    variables = cms.vstring("eta","pt"),
    bins = cms.VPSet(
        # Medium MVA WP scale factors  for Fall17V2 ID 
        #         # Note : SFs valid only from 10GeV, eta 2.5, bins extended to extremes to prevent any crashes 
        
	cms.PSet( lowBounds = cms.vdouble( -6.0000, 0.0000 ) , upBounds = cms.vdouble( -2.5000, 9999999.0000 ) , values = cms.vdouble( 1. ) , uncertainties = cms.vdouble( 0.0001 ) ),
    
     cms.PSet( lowBounds = cms.vdouble( -2.5 , 0 ) , upBounds = cms.vdouble( -2, 10 ) , values = cms.vdouble( 1.0 ) , uncertainties = cms.vdouble(0.001  ) ),
        cms.PSet( lowBounds = cms.vdouble( -2.5 , 10 ) , upBounds = cms.vdouble( -2, 20 ) , values = cms.vdouble( 0.95 ) , uncertainties = cms.vdouble(0.0113) ),
        cms.PSet( lowBounds = cms.vdouble( -2.5 , 20 ) , upBounds = cms.vdouble( -2, 35 ) , values = cms.vdouble(0.931 ) , uncertainties = cms.vdouble(0.0189) ),
        cms.PSet( lowBounds = cms.vdouble( -2.5 , 35 ) , upBounds = cms.vdouble( -2, 50 ) , values = cms.vdouble(0.942) , uncertainties =  cms.vdouble(0.011 ) ),
        cms.PSet( lowBounds = cms.vdouble( -2.5 , 50 ) , upBounds = cms.vdouble( -2, 100 ) , values = cms.vdouble(0.944) , uncertainties = cms.vdouble(0.0145) ),
        cms.PSet( lowBounds = cms.vdouble( -2.5 , 100 ) , upBounds = cms.vdouble( -2, 200 ) , values = cms.vdouble(0.974) , uncertainties =cms.vdouble(0.0159) ),
     cms.PSet( lowBounds = cms.vdouble( -2.5 , 200 ) , upBounds = cms.vdouble( -2, 500 ) , values = cms.vdouble( 0.867) , uncertainties =  cms.vdouble(0.0596) ),
        cms.PSet( lowBounds = cms.vdouble( -2.5 , 500 ) , upBounds = cms.vdouble( -2, 999999999 ) , values = cms.vdouble( 0.867) , uncertainties = cms.vdouble(0.0596) ),

        cms.PSet( lowBounds = cms.vdouble( -2 , 0 ) , upBounds = cms.vdouble( -1.566, 10 ) , values = cms.vdouble( 1.0) , uncertainties = cms.vdouble(0.001  ) ),
     cms.PSet( lowBounds = cms.vdouble( -2 , 10 ) , upBounds = cms.vdouble( -1.566, 20 ) , values = cms.vdouble(0.966 ) , uncertainties = cms.vdouble(0.0252 ) ),
     cms.PSet( lowBounds = cms.vdouble( -2 , 20 ) , upBounds = cms.vdouble( -1.566, 35 ) , values = cms.vdouble(0.941 ) , uncertainties = cms.vdouble(0.0225 ) ),
     cms.PSet( lowBounds = cms.vdouble( -2 , 35 ) , upBounds = cms.vdouble( -1.566, 50 ) , values = cms.vdouble(0.959 ) , uncertainties = cms.vdouble(0.0069  ) ),
     cms.PSet( lowBounds = cms.vdouble( -2 , 50 ) , upBounds = cms.vdouble( -1.566, 100 ) , values = cms.vdouble(0.966) , uncertainties = cms.vdouble(0.0053  ) ),
     cms.PSet( lowBounds = cms.vdouble( -2 , 100 ) , upBounds = cms.vdouble( -1.566, 200 ) , values = cms.vdoubl(0.973) , uncertainties = cms.vdouble(0.06   ) ),
     cms.PSet( lowBounds = cms.vdouble( -2 , 200 ) , upBounds = cms.vdouble( -1.566, 500 ) , values = cms.vdoubl(0.995) , uncertainties = cms.vdouble(0.0637 ) ),
     cms.PSet( lowBounds = cms.vdouble( -2 , 500 ) , upBounds = cms.vdouble( -1.566, 99999999 ) , values = cms.vdouble(0.995  ) , uncertainties = cms.vdouble(0.0637  ) ),

     cms.PSet( lowBounds = cms.vdouble( -1.566 , 0 ) , upBounds = cms.vdouble( -1.444, 10 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.566 , 10 ) , upBounds = cms.vdouble( -1.444, 20 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.566 , 20 ) , upBounds = cms.vdouble( -1.444, 35 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.566 , 35 ) , upBounds = cms.vdouble( -1.444, 50 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.566 , 50 ) , upBounds = cms.vdouble( -1.444, 100 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.566 , 100 ) , upBounds = cms.vdouble( -1.444, 200 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.566 , 200 ) , upBounds = cms.vdouble( -1.444, 500 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.566 , 500 ) , upBounds = cms.vdouble( -1.444, 9999999 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),

        cms.PSet( lowBounds = cms.vdouble( -1.444 , 0 ) , upBounds = cms.vdouble( -0.8, 10 ) , values = cms.vdouble( 1.0) , uncertainties = cms.vdouble(0.001  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.444 , 10 ) , upBounds = cms.vdouble( -0.8, 20 ) , values =  cms.vdouble(1     ) , uncertainties = cms.vdouble(0.0533 ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.444 , 20 ) , upBounds = cms.vdouble( -0.8, 35 ) , values =  cms.vdouble(0.955 ) , uncertainties = cms.vdouble(0.0306 ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.444 , 35 ) , upBounds = cms.vdouble( -0.8, 50 ) , values =  cms.vdouble(0.967 ) , uncertainties = cms.vdouble(0.0078 ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.444 , 50 ) , upBounds = cms.vdouble( -0.8, 100 ) , values = cms.vdouble(0.968 ) , uncertainties = cms.vdouble(0.0027  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.444 , 100 ) , upBounds = cms.vdouble( -0.8, 200 ) , values =cms.vdouble(0.97  ) , uncertainties = cms.vdouble(0.0352) ),
     cms.PSet( lowBounds = cms.vdouble( -1.444 , 200 ) , upBounds = cms.vdouble( -0.8, 500 ) , values =cms.vdouble(0.978 ) , uncertainties = cms.vdouble(0.0594) ),
     cms.PSet( lowBounds = cms.vdouble( -1.444 , 500 ) , upBounds = cms.vdouble( -0.8, 99999999 ) , values = cms.vdouble( 0.978 ) , uncertainties = cms.vdouble(0.0594  ) ),

     cms.PSet( lowBounds = cms.vdouble( -0.8 , 0 ) , upBounds = cms.vdouble( 0, 10 ) , values =    cms.vdouble(1.0 ) , uncertainties =  cms.vdouble(0.001  ) ),
    cms.PSet( lowBounds = cms.vdouble( -0.8 , 10 ) , upBounds = cms.vdouble( 0, 20 ) , values =    cms.vdouble(0.981) , uncertainties = cms.vdouble( 0.0185 ) ),
     cms.PSet( lowBounds = cms.vdouble( -0.8 , 20 ) , upBounds = cms.vdouble( 0, 35 ) , values =   cms.vdouble(0.988) , uncertainties = cms.vdouble( 0.0206 ) ),
     cms.PSet( lowBounds = cms.vdouble( -0.8 , 35 ) , upBounds = cms.vdouble( 0, 50 ) , values =   cms.vdouble(0.977 ) , uncertainties = cms.vdouble(0.0047  ) ),
     cms.PSet( lowBounds = cms.vdouble( -0.8 , 50 ) , upBounds = cms.vdouble( 0, 100 ) , values =  cms.vdouble(0.976 ) , uncertainties = cms.vdouble(0.0078  ) ),
     cms.PSet( lowBounds = cms.vdouble( -0.8 , 100 ) , upBounds = cms.vdouble( 0, 200 ) , values = cms.vdouble(0.983 ) , uncertainties = cms.vdouble(0.0222  ) ),
     cms.PSet( lowBounds = cms.vdouble( -0.8 , 200 ) , upBounds = cms.vdouble( 0, 500 ) , values = cms.vdouble(0.989 ) , uncertainties = cms.vdouble(0.0433  ) ),
        cms.PSet( lowBounds = cms.vdouble( -0.8 , 500 ) , upBounds = cms.vdouble( 0, 99999999 ) , values = cms.vdouble( 0.989) , uncertainties = cms.vdouble(0.0433  ) ),

     cms.PSet( lowBounds = cms.vdouble( 0 , 0 ) , upBounds = cms.vdouble( 0.8, 10 ) , values =   cms.vdouble(1.0 ) , uncertainties = cms.vdouble(0.0131015  ) ),
     cms.PSet( lowBounds = cms.vdouble( 0 , 10 ) , upBounds = cms.vdouble( 0.8, 20 ) , values =  cms.vdouble( 0.966) , uncertainties = cms.vdouble(0.0184  ) ),
     cms.PSet( lowBounds = cms.vdouble( 0 , 20 ) , upBounds = cms.vdouble( 0.8, 35 ) , values =  cms.vdouble( 0.987) , uncertainties = cms.vdouble(0.0206  ) ),
     cms.PSet( lowBounds = cms.vdouble( 0 , 35 ) , upBounds = cms.vdouble( 0.8, 50 ) , values =  cms.vdouble( 0.976) , uncertainties = cms.vdouble(0.00477  ) ),
     cms.PSet( lowBounds = cms.vdouble( 0 , 50 ) , upBounds = cms.vdouble( 0.8, 100 ) , values = cms.vdouble( 0.975) , uncertainties = cms.vdouble(0.00788 ) ),
     cms.PSet( lowBounds = cms.vdouble( 0 , 100 ) , upBounds = cms.vdouble( 0.8, 200 ) , values =cms.vdouble( 0.988) , uncertainties = cms.vdouble(0.0222   ) ),
     cms.PSet( lowBounds = cms.vdouble( 0 , 200 ) , upBounds = cms.vdouble( 0.8, 500 ) , values =cms.vdouble( 0.954) , uncertainties = cms.vdouble(0.043   ) ),
     cms.PSet( lowBounds = cms.vdouble( 0 , 500 ) , upBounds = cms.vdouble( 0.8, 99999999 ) , values = cms.vdouble(0.954 ) , uncertainties = cms.vdouble(0.043  ) ),

     cms.PSet( lowBounds = cms.vdouble( 0.8 , 0 ) , upBounds = cms.vdouble( 1.444, 10 ) , values = cms.vdouble(1.0 ) , uncertainties = cms.vdouble(0.010  ) ),
     cms.PSet( lowBounds = cms.vdouble( 0.8 , 10 ) , upBounds = cms.vdouble( 1.444, 20 ) , values = cms.vdouble(0.995 ) , uncertainties = cms.vdouble(0.0533   ) ),
     cms.PSet( lowBounds = cms.vdouble( 0.8 , 20 ) , upBounds = cms.vdouble( 1.444, 35 ) , values = cms.vdouble(0.977) , uncertainties = cms.vdouble (0.0306   ) ),
     cms.PSet( lowBounds = cms.vdouble( 0.8 , 35 ) , upBounds = cms.vdouble( 1.444, 50 ) , values = cms.vdouble(0.968) , uncertainties = cms.vdouble (0.00783  ) ),
     cms.PSet( lowBounds = cms.vdouble( 0.8 , 50 ) , upBounds = cms.vdouble( 1.444, 100 ) ,values = cms.vdouble(0.971) , uncertainties = cms.vdouble (0.0027   ) ),
     cms.PSet( lowBounds = cms.vdouble( 0.8 , 100 ) , upBounds = cms.vdouble( 1.444, 200 ),values = cms.vdouble(0.977 ) , uncertainties = cms.vdouble(0.0352  ) ),
     cms.PSet( lowBounds = cms.vdouble( 0.8 , 200 ) , upBounds = cms.vdouble( 1.444, 500 ),values = cms.vdouble(0.96 ) , uncertainties = cms.vdouble (0.0594   ) ),
     cms.PSet( lowBounds = cms.vdouble( 0.8 , 500 ) , upBounds = cms.vdouble( 1.444, 9999999 ) , values = cms.vdouble(0.96 ) , uncertainties = cms.vdouble(0.0594  ) ),

     cms.PSet( lowBounds = cms.vdouble( 1.444 , 0 ) , upBounds = cms.vdouble( 1.566, 10 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
    cms.PSet( lowBounds = cms.vdouble( 1.444 , 10 ) , upBounds = cms.vdouble( 1.566, 20 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.444 , 20 ) , upBounds = cms.vdouble( 1.566, 35 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.444 , 35 ) , upBounds = cms.vdouble( 1.566, 50 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.444 , 50 ) , upBounds = cms.vdouble( 1.566, 100 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.444 , 100 ) , upBounds = cms.vdouble( 1.566, 200 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.444 , 200 ) , upBounds = cms.vdouble( 1.566, 500 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.444 , 500 ) , upBounds = cms.vdouble( 1.566, 9999999 ) , values = cms.vdouble( 1 ) , uncertainties = cms.vdouble(1  ) ),

     cms.PSet( lowBounds = cms.vdouble( 1.566 , 0 ) , upBounds = cms.vdouble( 2, 10 ) , values = cms.vdouble( 1.0 ) , uncertainties = cms.vdouble(0.001  ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.566 , 10 ) , upBounds = cms.vdouble( 2, 20 ) , values =   cms.vdouble(0.978 ) , uncertainties = cms.vdouble(0.0252) ),
     cms.PSet( lowBounds = cms.vdouble( 1.566 , 20 ) , upBounds = cms.vdouble( 2, 35 ) , values =   cms.vdouble(0.952 ) , uncertainties = cms.vdouble(0.0225) ),
     cms.PSet( lowBounds = cms.vdouble( 1.566 , 35 ) , upBounds = cms.vdouble( 2, 50 ) , values =   cms.vdouble(0.96  ) , uncertainties = cms.vdouble(0.0069 ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.566 , 50 ) , upBounds = cms.vdouble( 2, 100 ) , values =  cms.vdouble(0.973 ) , uncertainties = cms.vdouble(0.0053 ) ),
     cms.PSet( lowBounds = cms.vdouble( 1.566 , 100 ) , upBounds = cms.vdouble( 2, 200 ) , values = cms.vdouble(0.996 ) , uncertainties = cms.vdouble(0.0601) ),
     cms.PSet( lowBounds = cms.vdouble( 1.566 , 200 ) , upBounds = cms.vdouble( 2, 500 ) , values = cms.vdouble(0.961 ) , uncertainties = cms.vdouble(0.0633) ),
     cms.PSet( lowBounds = cms.vdouble( 1.566 , 500 ) , upBounds = cms.vdouble( 2, 99999999 ) , values = cms.vdouble( 0.961 ) , uncertainties = cms.vdouble(0.0633 ) ),

        cms.PSet( lowBounds = cms.vdouble( 2 , 0 ) , upBounds = cms.vdouble( 2.5, 10 ) , values = cms.vdouble( 1.0) , uncertainties = cms.vdouble(0.001 ) ),
     cms.PSet( lowBounds = cms.vdouble( 2 , 10 ) , upBounds = cms.vdouble( 2.5, 20 ) , values =   cms.vdouble(0.941)  , uncertainties = cms.vdouble(0.0115  ) ),
     cms.PSet( lowBounds = cms.vdouble( 2 , 20 ) , upBounds = cms.vdouble( 2.5, 35 ) , values =   cms.vdouble(0.931)  , uncertainties = cms.vdouble(0.0189  ) ),
     cms.PSet( lowBounds = cms.vdouble( 2 , 35 ) , upBounds = cms.vdouble( 2.5, 50 ) , values =   cms.vdouble(0.934)  , uncertainties = cms.vdouble(0.011   ) ),
     cms.PSet( lowBounds = cms.vdouble( 2 , 50 ) , upBounds = cms.vdouble( 2.5, 100 ) , values =  cms.vdouble(0.933) , uncertainties =  cms.vdouble(0.0145 ) ),
     cms.PSet( lowBounds = cms.vdouble( 2 , 100 ) , upBounds = cms.vdouble( 2.5, 200 ) , values = cms.vdouble(0.953 ) , uncertainties = cms.vdouble(0.0161  ) ),
     cms.PSet( lowBounds = cms.vdouble( 2 , 200 ) , upBounds = cms.vdouble( 2.5, 500 ) , values = cms.vdouble(1.06  ) , uncertainties = cms.vdouble(0.0696  ) ),	
     cms.PSet( lowBounds = cms.vdouble( 2 , 500 ) , upBounds = cms.vdouble( 2.5, 999999999 ) , values = cms.vdouble( 1.06 ) , uncertainties = cms.vdouble(0.0696  ) ),	

        	cms.PSet( lowBounds = cms.vdouble( 2.5000, 0.0000 ) , upBounds = cms.vdouble( 6.0000, 999999999.0000 ) , values = cms.vdouble(  1.0 ) , uncertainties = cms.vdouble( 0.001 ) )

        )
    )	

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

     cms.PSet( lowBounds = cms.vdouble( -1.444 , 0 ) , upBounds = cms.vdouble( -0.8, 10 ) , values = cms.vdouble( 1.0 ) , uncertainties = cms.vdouble(0.001  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.444 , 10 ) , upBounds = cms.vdouble( -0.8, 20 ) , values = cms.vdouble( 0.965 ) , uncertainties = cms.vdouble(0.011  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.444 , 20 ) , upBounds = cms.vdouble( -0.8, 35 ) , values = cms.vdouble( 0.944 ) , uncertainties = cms.vdouble(0.018  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.444 , 35 ) , upBounds = cms.vdouble( -0.8, 50 ) , values = cms.vdouble( 0.960 ) , uncertainties = cms.vdouble(0.004  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.444 , 50 ) , upBounds = cms.vdouble( -0.8, 100 ) , values = cms.vdouble( 0.964 ) , uncertainties = cms.vdouble(0.003  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.444 , 100 ) , upBounds = cms.vdouble( -0.8, 200 ) , values = cms.vdouble( 0.977 ) , uncertainties = cms.vdouble(0.011 ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.444 , 200 ) , upBounds = cms.vdouble( -0.8, 500 ) , values = cms.vdouble( 0.981) , uncertainties = cms.vdouble(0.025  ) ),
     cms.PSet( lowBounds = cms.vdouble( -1.444 , 500 ) , upBounds = cms.vdouble( -0.8, 99999999 ) , values = cms.vdouble( 0.981) , uncertainties = cms.vdouble(0.025  ) ),

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



flashggElectronSystematics = cms.EDProducer('FlashggElectronEffSystematicProducer',
                                            src = cms.InputTag("flashggSelectedElectrons"),
                                            SystMethods2D = cms.VPSet(),
                                            SystMethods = cms.VPSet(cms.PSet( MethodName = cms.string("FlashggElectronWeight"),
                                                                              Label = cms.string("ElectronWeight"),
                                                                              NSigmas = cms.vint32(-1,1),
                                                                              OverallRange = cms.string("abs(eta)<2.5"),
                                                                              BinList = binInfo2016,
                                                                              Debug = cms.untracked.bool(False),
                                                                              ApplyCentralValue = cms.bool(True)
                                                                              )	
                                                                    )
                                            )
