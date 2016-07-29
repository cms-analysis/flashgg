import FWCore.ParameterSet.Config as cms

binInfo = cms.PSet(
		variables = cms.vstring("eta","pt"),
		bins = cms.VPSet(
                        # TightID+LoosePFRelative isolation + Track scale factors : SF = SF(ID | Track)*SF(iso|ID)*SF(Track)
                        # uncertainties are: stat (+) syst. with syst: 1% for ID, 0.5% for iso, 1% to account for different Z vs multijet topology and Track SF unc.
                        # 80X
                        # Preliminary numbers (7.5/fb) taken from : https://twiki.cern.ch/twiki/bin/view/CMS/MuonWorkInProgressAndPagResults
			cms.PSet(lowBounds = cms.vdouble(-5.0,0.00), upBounds = cms.vdouble(-2.1,20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000,0.0000)),	# This bin should never be selected, just here to avoid crashes   
			cms.PSet(lowBounds = cms.vdouble(-5.0,20.00), upBounds = cms.vdouble(-2.1,25.00), values = cms.vdouble(0.9619), uncertainties = cms.vdouble(0.0158,0.0159)),	
			cms.PSet(lowBounds = cms.vdouble(-5.0,25.00), upBounds = cms.vdouble(-2.1,30.00), values = cms.vdouble(0.9586), uncertainties = cms.vdouble(0.0154,0.0155)),	
			cms.PSet(lowBounds = cms.vdouble(-5.0,30.00), upBounds = cms.vdouble(-2.1,40.00), values = cms.vdouble(0.9563), uncertainties = cms.vdouble(0.0152,0.0153)),	
			cms.PSet(lowBounds = cms.vdouble(-5.0,40.00), upBounds = cms.vdouble(-2.1,50.00), values = cms.vdouble(0.9530), uncertainties = cms.vdouble(0.0151,0.0152)),	
			cms.PSet(lowBounds = cms.vdouble(-5.0,50.00), upBounds = cms.vdouble(-2.1,60.00), values = cms.vdouble(0.9634), uncertainties = cms.vdouble(0.0156,0.0158)),	
			cms.PSet(lowBounds = cms.vdouble(-5.0,60.00), upBounds = cms.vdouble(-2.1,100.00), values = cms.vdouble(0.9582), uncertainties = cms.vdouble(0.0169,0.0170)),	
			cms.PSet(lowBounds = cms.vdouble(-5.0,100.00), upBounds = cms.vdouble(-2.1,99999999.00), values = cms.vdouble(0.9046), uncertainties = cms.vdouble(0.0799,0.0799)),	
			cms.PSet(lowBounds = cms.vdouble(-2.1,0.00), upBounds = cms.vdouble(-1.6,20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000,0.0000)),	# This bin should never be selected, just here to avoid crashes   
			cms.PSet(lowBounds = cms.vdouble(-2.1,20.00), upBounds = cms.vdouble(-1.6,25.00), values = cms.vdouble(0.9806), uncertainties = cms.vdouble(0.0157,0.0157)),	
			cms.PSet(lowBounds = cms.vdouble(-2.1,25.00), upBounds = cms.vdouble(-1.6,30.00), values = cms.vdouble(0.9805), uncertainties = cms.vdouble(0.0156,0.0156)),	
			cms.PSet(lowBounds = cms.vdouble(-2.1,30.00), upBounds = cms.vdouble(-1.6,40.00), values = cms.vdouble(0.9829), uncertainties = cms.vdouble(0.0156,0.0156)),	
			cms.PSet(lowBounds = cms.vdouble(-2.1,40.00), upBounds = cms.vdouble(-1.6,50.00), values = cms.vdouble(0.9823), uncertainties = cms.vdouble(0.0155,0.0155)),	
			cms.PSet(lowBounds = cms.vdouble(-2.1,50.00), upBounds = cms.vdouble(-1.6,60.00), values = cms.vdouble(0.9835), uncertainties = cms.vdouble(0.0156,0.0156)),	
			cms.PSet(lowBounds = cms.vdouble(-2.1,60.00), upBounds = cms.vdouble(-1.6,100.00), values = cms.vdouble(0.9813), uncertainties = cms.vdouble(0.0157,0.0157)),	
			cms.PSet(lowBounds = cms.vdouble(-2.1,100.00), upBounds = cms.vdouble(-1.6,99999999.00), values = cms.vdouble(1.0057), uncertainties = cms.vdouble(0.0185,0.0185)),	
			cms.PSet(lowBounds = cms.vdouble(-1.6,0.00), upBounds = cms.vdouble(-1.2,20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000,0.0000)),	# This bin should never be selected, just here to avoid crashes   
			cms.PSet(lowBounds = cms.vdouble(-1.6,20.00), upBounds = cms.vdouble(-1.2,25.00), values = cms.vdouble(0.9847), uncertainties = cms.vdouble(0.0158,0.0158)),	
			cms.PSet(lowBounds = cms.vdouble(-1.6,25.00), upBounds = cms.vdouble(-1.2,30.00), values = cms.vdouble(0.9847), uncertainties = cms.vdouble(0.0156,0.0156)),	
			cms.PSet(lowBounds = cms.vdouble(-1.6,30.00), upBounds = cms.vdouble(-1.2,40.00), values = cms.vdouble(0.9871), uncertainties = cms.vdouble(0.0156,0.0156)),	
			cms.PSet(lowBounds = cms.vdouble(-1.6,40.00), upBounds = cms.vdouble(-1.2,50.00), values = cms.vdouble(0.9865), uncertainties = cms.vdouble(0.0156,0.0156)),	
			cms.PSet(lowBounds = cms.vdouble(-1.6,50.00), upBounds = cms.vdouble(-1.2,60.00), values = cms.vdouble(0.9877), uncertainties = cms.vdouble(0.0157,0.0157)),	
			cms.PSet(lowBounds = cms.vdouble(-1.6,60.00), upBounds = cms.vdouble(-1.2,100.00), values = cms.vdouble(0.9855), uncertainties = cms.vdouble(0.0158,0.0158)),	
			cms.PSet(lowBounds = cms.vdouble(-1.6,100.00), upBounds = cms.vdouble(-1.2,99999999.00), values = cms.vdouble(1.0099), uncertainties = cms.vdouble(0.0186,0.0186)),	
			cms.PSet(lowBounds = cms.vdouble(-1.2,0.00), upBounds = cms.vdouble(-1.1,20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000,0.0000)),	# This bin should never be selected, just here to avoid crashes   
			cms.PSet(lowBounds = cms.vdouble(-1.2,20.00), upBounds = cms.vdouble(-1.1,25.00), values = cms.vdouble(0.9643), uncertainties = cms.vdouble(0.0160,0.0160)),	
			cms.PSet(lowBounds = cms.vdouble(-1.2,25.00), upBounds = cms.vdouble(-1.1,30.00), values = cms.vdouble(0.9618), uncertainties = cms.vdouble(0.0155,0.0155)),	
			cms.PSet(lowBounds = cms.vdouble(-1.2,30.00), upBounds = cms.vdouble(-1.1,40.00), values = cms.vdouble(0.9681), uncertainties = cms.vdouble(0.0153,0.0153)),	
			cms.PSet(lowBounds = cms.vdouble(-1.2,40.00), upBounds = cms.vdouble(-1.1,50.00), values = cms.vdouble(0.9666), uncertainties = cms.vdouble(0.0153,0.0153)),	
			cms.PSet(lowBounds = cms.vdouble(-1.2,50.00), upBounds = cms.vdouble(-1.1,60.00), values = cms.vdouble(0.9675), uncertainties = cms.vdouble(0.0154,0.0154)),	
			cms.PSet(lowBounds = cms.vdouble(-1.2,60.00), upBounds = cms.vdouble(-1.1,100.00), values = cms.vdouble(0.9649), uncertainties = cms.vdouble(0.0157,0.0157)),	
			cms.PSet(lowBounds = cms.vdouble(-1.2,100.00), upBounds = cms.vdouble(-1.1,99999999.00), values = cms.vdouble(1.0212), uncertainties = cms.vdouble(0.0279,0.0279)),	
			cms.PSet(lowBounds = cms.vdouble(-1.1,0.00), upBounds = cms.vdouble(-0.9,20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000,0.0000)),	# This bin should never be selected, just here to avoid crashes   
			cms.PSet(lowBounds = cms.vdouble(-1.1,20.00), upBounds = cms.vdouble(-0.9,25.00), values = cms.vdouble(0.9619), uncertainties = cms.vdouble(0.0159,0.0159)),	
			cms.PSet(lowBounds = cms.vdouble(-1.1,25.00), upBounds = cms.vdouble(-0.9,30.00), values = cms.vdouble(0.9594), uncertainties = cms.vdouble(0.0154,0.0154)),	
			cms.PSet(lowBounds = cms.vdouble(-1.1,30.00), upBounds = cms.vdouble(-0.9,40.00), values = cms.vdouble(0.9656), uncertainties = cms.vdouble(0.0153,0.0153)),	
			cms.PSet(lowBounds = cms.vdouble(-1.1,40.00), upBounds = cms.vdouble(-0.9,50.00), values = cms.vdouble(0.9641), uncertainties = cms.vdouble(0.0153,0.0153)),	
			cms.PSet(lowBounds = cms.vdouble(-1.1,50.00), upBounds = cms.vdouble(-0.9,60.00), values = cms.vdouble(0.9651), uncertainties = cms.vdouble(0.0154,0.0154)),	
			cms.PSet(lowBounds = cms.vdouble(-1.1,60.00), upBounds = cms.vdouble(-0.9,100.00), values = cms.vdouble(0.9625), uncertainties = cms.vdouble(0.0156,0.0156)),	
			cms.PSet(lowBounds = cms.vdouble(-1.1,100.00), upBounds = cms.vdouble(-0.9,99999999.00), values = cms.vdouble(1.0186), uncertainties = cms.vdouble(0.0278,0.0278)),	
			cms.PSet(lowBounds = cms.vdouble(-0.9,0.00), upBounds = cms.vdouble(-0.6,20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000,0.0000)),	# This bin should never be selected, just here to avoid crashes   
			cms.PSet(lowBounds = cms.vdouble(-0.9,20.00), upBounds = cms.vdouble(-0.6,25.00), values = cms.vdouble(0.9456), uncertainties = cms.vdouble(0.0153,0.0153)),	
			cms.PSet(lowBounds = cms.vdouble(-0.9,25.00), upBounds = cms.vdouble(-0.6,30.00), values = cms.vdouble(0.9612), uncertainties = cms.vdouble(0.0153,0.0153)),	
			cms.PSet(lowBounds = cms.vdouble(-0.9,30.00), upBounds = cms.vdouble(-0.6,40.00), values = cms.vdouble(0.9684), uncertainties = cms.vdouble(0.0153,0.0153)),	
			cms.PSet(lowBounds = cms.vdouble(-0.9,40.00), upBounds = cms.vdouble(-0.6,50.00), values = cms.vdouble(0.9687), uncertainties = cms.vdouble(0.0153,0.0153)),	
			cms.PSet(lowBounds = cms.vdouble(-0.9,50.00), upBounds = cms.vdouble(-0.6,60.00), values = cms.vdouble(0.9658), uncertainties = cms.vdouble(0.0153,0.0153)),	
			cms.PSet(lowBounds = cms.vdouble(-0.9,60.00), upBounds = cms.vdouble(-0.6,100.00), values = cms.vdouble(0.9668), uncertainties = cms.vdouble(0.0154,0.0154)),	
			cms.PSet(lowBounds = cms.vdouble(-0.9,100.00), upBounds = cms.vdouble(-0.6,99999999.00), values = cms.vdouble(0.9821), uncertainties = cms.vdouble(0.0223,0.0223)),	
			cms.PSet(lowBounds = cms.vdouble(-0.6,0.00), upBounds = cms.vdouble(0,20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000,0.0000)),	# This bin should never be selected, just here to avoid crashes   
			cms.PSet(lowBounds = cms.vdouble(-0.6,20.00), upBounds = cms.vdouble(0,25.00), values = cms.vdouble(0.9437), uncertainties = cms.vdouble(0.0153,0.0153)),	
			cms.PSet(lowBounds = cms.vdouble(-0.6,25.00), upBounds = cms.vdouble(0,30.00), values = cms.vdouble(0.9593), uncertainties = cms.vdouble(0.0153,0.0153)),	
			cms.PSet(lowBounds = cms.vdouble(-0.6,30.00), upBounds = cms.vdouble(0,40.00), values = cms.vdouble(0.9665), uncertainties = cms.vdouble(0.0153,0.0153)),	
			cms.PSet(lowBounds = cms.vdouble(-0.6,40.00), upBounds = cms.vdouble(0,50.00), values = cms.vdouble(0.9668), uncertainties = cms.vdouble(0.0153,0.0153)),	
			cms.PSet(lowBounds = cms.vdouble(-0.6,50.00), upBounds = cms.vdouble(0,60.00), values = cms.vdouble(0.9639), uncertainties = cms.vdouble(0.0153,0.0153)),	
			cms.PSet(lowBounds = cms.vdouble(-0.6,60.00), upBounds = cms.vdouble(0,100.00), values = cms.vdouble(0.9649), uncertainties = cms.vdouble(0.0154,0.0154)),	
			cms.PSet(lowBounds = cms.vdouble(-0.6,100.00), upBounds = cms.vdouble(0,99999999.00), values = cms.vdouble(0.9801), uncertainties = cms.vdouble(0.0223,0.0223)),	
			cms.PSet(lowBounds = cms.vdouble(0.0,0.00), upBounds = cms.vdouble(0.6,20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000,0.0000)),	# This bin should never be selected, just here to avoid crashes   
			cms.PSet(lowBounds = cms.vdouble(0.0,20.00), upBounds = cms.vdouble(0.6,25.00), values = cms.vdouble(0.9468), uncertainties = cms.vdouble(0.0153,0.0153)),	
			cms.PSet(lowBounds = cms.vdouble(0.0,25.00), upBounds = cms.vdouble(0.6,30.00), values = cms.vdouble(0.9624), uncertainties = cms.vdouble(0.0153,0.0153)),	
			cms.PSet(lowBounds = cms.vdouble(0.0,30.00), upBounds = cms.vdouble(0.6,40.00), values = cms.vdouble(0.9696), uncertainties = cms.vdouble(0.0153,0.0153)),	
			cms.PSet(lowBounds = cms.vdouble(0.0,40.00), upBounds = cms.vdouble(0.6,50.00), values = cms.vdouble(0.9699), uncertainties = cms.vdouble(0.0153,0.0153)),	
			cms.PSet(lowBounds = cms.vdouble(0.0,50.00), upBounds = cms.vdouble(0.6,60.00), values = cms.vdouble(0.9671), uncertainties = cms.vdouble(0.0153,0.0153)),	
			cms.PSet(lowBounds = cms.vdouble(0.0,60.00), upBounds = cms.vdouble(0.6,100.00), values = cms.vdouble(0.9680), uncertainties = cms.vdouble(0.0154,0.0154)),	
			cms.PSet(lowBounds = cms.vdouble(0.0,100.00), upBounds = cms.vdouble(0.6,99999999.00), values = cms.vdouble(0.9833), uncertainties = cms.vdouble(0.0224,0.0224)),	
			cms.PSet(lowBounds = cms.vdouble(0.6,0.00), upBounds = cms.vdouble(0.9,20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000,0.0000)),	# This bin should never be selected, just here to avoid crashes   
			cms.PSet(lowBounds = cms.vdouble(0.6,20.00), upBounds = cms.vdouble(0.9,25.00), values = cms.vdouble(0.9487), uncertainties = cms.vdouble(0.0153,0.0153)),	
			cms.PSet(lowBounds = cms.vdouble(0.6,25.00), upBounds = cms.vdouble(0.9,30.00), values = cms.vdouble(0.9643), uncertainties = cms.vdouble(0.0153,0.0153)),	
			cms.PSet(lowBounds = cms.vdouble(0.6,30.00), upBounds = cms.vdouble(0.9,40.00), values = cms.vdouble(0.9715), uncertainties = cms.vdouble(0.0154,0.0154)),	
			cms.PSet(lowBounds = cms.vdouble(0.6,40.00), upBounds = cms.vdouble(0.9,50.00), values = cms.vdouble(0.9718), uncertainties = cms.vdouble(0.0154,0.0154)),	
			cms.PSet(lowBounds = cms.vdouble(0.6,50.00), upBounds = cms.vdouble(0.9,60.00), values = cms.vdouble(0.9690), uncertainties = cms.vdouble(0.0154,0.0154)),	
			cms.PSet(lowBounds = cms.vdouble(0.6,60.00), upBounds = cms.vdouble(0.9,100.00), values = cms.vdouble(0.9699), uncertainties = cms.vdouble(0.0155,0.0155)),	
			cms.PSet(lowBounds = cms.vdouble(0.6,100.00), upBounds = cms.vdouble(0.9,99999999.00), values = cms.vdouble(0.9853), uncertainties = cms.vdouble(0.0224,0.0224)),	
			cms.PSet(lowBounds = cms.vdouble(0.9,0.00), upBounds = cms.vdouble(1.1,20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000,0.0000)),	# This bin should never be selected, just here to avoid crashes   
			cms.PSet(lowBounds = cms.vdouble(0.9,20.00), upBounds = cms.vdouble(1.1,25.00), values = cms.vdouble(0.9650), uncertainties = cms.vdouble(0.0160,0.0160)),	
			cms.PSet(lowBounds = cms.vdouble(0.9,25.00), upBounds = cms.vdouble(1.1,30.00), values = cms.vdouble(0.9625), uncertainties = cms.vdouble(0.0155,0.0155)),	
			cms.PSet(lowBounds = cms.vdouble(0.9,30.00), upBounds = cms.vdouble(1.1,40.00), values = cms.vdouble(0.9688), uncertainties = cms.vdouble(0.0154,0.0154)),	
			cms.PSet(lowBounds = cms.vdouble(0.9,40.00), upBounds = cms.vdouble(1.1,50.00), values = cms.vdouble(0.9673), uncertainties = cms.vdouble(0.0153,0.0153)),	
			cms.PSet(lowBounds = cms.vdouble(0.9,50.00), upBounds = cms.vdouble(1.1,60.00), values = cms.vdouble(0.9682), uncertainties = cms.vdouble(0.0154,0.0154)),	
			cms.PSet(lowBounds = cms.vdouble(0.9,60.00), upBounds = cms.vdouble(1.1,100.00), values = cms.vdouble(0.9656), uncertainties = cms.vdouble(0.0157,0.0157)),	
			cms.PSet(lowBounds = cms.vdouble(0.9,100.00), upBounds = cms.vdouble(1.1,99999999.00), values = cms.vdouble(1.0219), uncertainties = cms.vdouble(0.0279,0.0279)),	
			cms.PSet(lowBounds = cms.vdouble(1.1,0.00), upBounds = cms.vdouble(1.2,20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000,0.0000)),	# This bin should never be selected, just here to avoid crashes   
			cms.PSet(lowBounds = cms.vdouble(1.1,20.00), upBounds = cms.vdouble(1.2,25.00), values = cms.vdouble(0.9633), uncertainties = cms.vdouble(0.0160,0.0160)),	
			cms.PSet(lowBounds = cms.vdouble(1.1,25.00), upBounds = cms.vdouble(1.2,30.00), values = cms.vdouble(0.9609), uncertainties = cms.vdouble(0.0154,0.0154)),	
			cms.PSet(lowBounds = cms.vdouble(1.1,30.00), upBounds = cms.vdouble(1.2,40.00), values = cms.vdouble(0.9671), uncertainties = cms.vdouble(0.0153,0.0153)),	
			cms.PSet(lowBounds = cms.vdouble(1.1,40.00), upBounds = cms.vdouble(1.2,50.00), values = cms.vdouble(0.9656), uncertainties = cms.vdouble(0.0153,0.0153)),	
			cms.PSet(lowBounds = cms.vdouble(1.1,50.00), upBounds = cms.vdouble(1.2,60.00), values = cms.vdouble(0.9665), uncertainties = cms.vdouble(0.0154,0.0154	)),	
			cms.PSet(lowBounds = cms.vdouble(1.1,60.00), upBounds = cms.vdouble(1.2,100.00), values = cms.vdouble(0.9640), uncertainties = cms.vdouble(0.0156,0.0156)),	
			cms.PSet(lowBounds = cms.vdouble(1.1,100.00), upBounds = cms.vdouble(1.2,99999999.00), values = cms.vdouble(1.0201), uncertainties = cms.vdouble(0.0278,0.0278)),	
			cms.PSet(lowBounds = cms.vdouble(1.2,0.00), upBounds = cms.vdouble(1.6,20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000,0.0000)),	# This bin should never be selected, just here to avoid crashes   
			cms.PSet(lowBounds = cms.vdouble(1.2,20.00), upBounds = cms.vdouble(1.6,25.00), values = cms.vdouble(0.9837), uncertainties = cms.vdouble(0.9838,0.0157)),	
			cms.PSet(lowBounds = cms.vdouble(1.2,25.00), upBounds = cms.vdouble(1.6,30.00), values = cms.vdouble(0.9837), uncertainties = cms.vdouble(0.9838,0.0156)),	
			cms.PSet(lowBounds = cms.vdouble(1.2,30.00), upBounds = cms.vdouble(1.6,40.00), values = cms.vdouble(0.9861), uncertainties = cms.vdouble(0.9862,0.0156)),	
			cms.PSet(lowBounds = cms.vdouble(1.2,40.00), upBounds = cms.vdouble(1.6,50.00), values = cms.vdouble(0.9855), uncertainties = cms.vdouble(0.9856,0.0156)),	
			cms.PSet(lowBounds = cms.vdouble(1.2,50.00), upBounds = cms.vdouble(1.6,60.00), values = cms.vdouble(0.9867), uncertainties = cms.vdouble(0.9868,0.0157)),	
			cms.PSet(lowBounds = cms.vdouble(1.2,60.00), upBounds = cms.vdouble(1.6,100.00), values = cms.vdouble(0.9845), uncertainties = cms.vdouble(0.9846,0.0158)),	
			cms.PSet(lowBounds = cms.vdouble(1.2,100.00), upBounds = cms.vdouble(1.6,99999999.00), values = cms.vdouble(1.0089), uncertainties = cms.vdouble(1.0091,0.0186)),	
			cms.PSet(lowBounds = cms.vdouble(1.6,0.00), upBounds = cms.vdouble(2.1,20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000,0.0000)),	# This bin should never be selected, just here to avoid crashes   
			cms.PSet(lowBounds = cms.vdouble(1.6,20.00), upBounds = cms.vdouble(2.1,25.00), values = cms.vdouble(0.9800), uncertainties = cms.vdouble(0.0157,0.0157)),	
			cms.PSet(lowBounds = cms.vdouble(1.6,25.00), upBounds = cms.vdouble(2.1,30.00), values = cms.vdouble(0.9800), uncertainties = cms.vdouble(0.0156,0.0156)),	
			cms.PSet(lowBounds = cms.vdouble(1.6,30.00), upBounds = cms.vdouble(2.1,40.00), values = cms.vdouble(0.9824), uncertainties = cms.vdouble(0.0155,0.0155)),	
			cms.PSet(lowBounds = cms.vdouble(1.6,40.00), upBounds = cms.vdouble(2.1,50.00), values = cms.vdouble(0.9818), uncertainties = cms.vdouble(0.0155,0.0155)),	
			cms.PSet(lowBounds = cms.vdouble(1.6,50.00), upBounds = cms.vdouble(2.1,60.00), values = cms.vdouble(0.9830), uncertainties = cms.vdouble(0.0156,0.0156)),	
			cms.PSet(lowBounds = cms.vdouble(1.6,60.00), upBounds = cms.vdouble(2.1,100.00), values = cms.vdouble(0.9808), uncertainties = cms.vdouble(0.0157,0.0157)),	
			cms.PSet(lowBounds = cms.vdouble(1.6,100.00), upBounds = cms.vdouble(2.1,99999999.00), values = cms.vdouble(1.0051), uncertainties = cms.vdouble(0.0185,0.0185)),	
			cms.PSet(lowBounds = cms.vdouble(2.1,0.00), upBounds = cms.vdouble(5.0,20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000,0.0000)),	# This bin should never be selected, just here to avoid crashes   
			cms.PSet(lowBounds = cms.vdouble(2.1,20.00), upBounds = cms.vdouble(5.0,25.00), values = cms.vdouble(0.9565), uncertainties = cms.vdouble(0.0158,0.0158)),	
			cms.PSet(lowBounds = cms.vdouble(2.1,25.00), upBounds = cms.vdouble(5.0,30.00), values = cms.vdouble(0.9531), uncertainties = cms.vdouble(0.0154,0.0154)),	
			cms.PSet(lowBounds = cms.vdouble(2.1,30.00), upBounds = cms.vdouble(5.0,40.00), values = cms.vdouble(0.9509), uncertainties = cms.vdouble(0.0152,0.0152)),	
			cms.PSet(lowBounds = cms.vdouble(2.1,40.00), upBounds = cms.vdouble(5.0,50.00), values = cms.vdouble(0.9476), uncertainties = cms.vdouble(0.0151,0.0151)),	
			cms.PSet(lowBounds = cms.vdouble(2.1,50.00), upBounds = cms.vdouble(5.0,60.00), values = cms.vdouble(0.9579), uncertainties = cms.vdouble(0.0156,0.0156)),	
			cms.PSet(lowBounds = cms.vdouble(2.1,60.00), upBounds = cms.vdouble(5.0,100.00), values = cms.vdouble(0.9528), uncertainties = cms.vdouble(0.0169,0.0169)),	
			cms.PSet(lowBounds = cms.vdouble(2.1,100.00), upBounds = cms.vdouble(5.0,99999999.00), values = cms.vdouble(0.8994), uncertainties = cms.vdouble(0.0795,0.0795))	
	                # TightID+LoosePFRelative isolation scale factors : SF = SF(ID)*SF(iso|ID)
			# uncertainties are: stat (+) syst. with syst: 1% for ID, 0.5% for iso, 1% to account for different Z vs multijet topology
                        # 80X
			# Preliminary numbers (2.6/fb) taken from : https://twiki.cern.ch/twiki/bin/view/CMS/MuonWorkInProgressAndPagResults
			#cms.PSet(lowBounds = cms.vdouble(0.000000), upBounds = cms.vdouble(20.000000), values = cms.vdouble(1.000000), uncertainties = cms.vdouble(0.014870,0.014870)),	# This bin should never be selected, just here to avoid crashes   
			#cms.PSet(lowBounds = cms.vdouble(20.000000), upBounds = cms.vdouble(25.000000), values = cms.vdouble(0.982074), uncertainties = cms.vdouble(0.014870,0.014870)),
			#cms.PSet(lowBounds = cms.vdouble(25.000000), upBounds = cms.vdouble(30.000000), values = cms.vdouble(0.982037), uncertainties = cms.vdouble(0.014778,0.014778)),
			#cms.PSet(lowBounds = cms.vdouble(30.000000), upBounds = cms.vdouble(40.000000), values = cms.vdouble(0.982530), uncertainties = cms.vdouble(0.014746,0.014746)),
			#cms.PSet(lowBounds = cms.vdouble(40.000000), upBounds = cms.vdouble(50.000000), values = cms.vdouble(0.981415), uncertainties = cms.vdouble(0.014723,0.014723)),
			#cms.PSet(lowBounds = cms.vdouble(50.000000), upBounds = cms.vdouble(60.000000), values = cms.vdouble(0.977608), uncertainties = cms.vdouble(0.014689,0.014689)),
			#cms.PSet(lowBounds = cms.vdouble(60.000000), upBounds = cms.vdouble(80.000000), values = cms.vdouble(0.984926), uncertainties = cms.vdouble(0.014906,0.014906)),
			#cms.PSet(lowBounds = cms.vdouble(80.000000), upBounds = cms.vdouble(120.000000), values = cms.vdouble(0.968123), uncertainties = cms.vdouble(0.015627,0.015627)),
			#cms.PSet(lowBounds = cms.vdouble(120.000000), upBounds = cms.vdouble(9999999999.000000), values = cms.vdouble(1.026026), uncertainties = cms.vdouble(0.025844,0.025844))
			# taken from : https://twiki.cern.ch/twiki/bin/view/CMS/MuonReferenceEffsRun2
                        #76X
                        #cms.PSet(lowBounds = cms.vdouble(0.), upBounds = cms.vdouble(20.), values = cms.vdouble(1.), uncertainties = cms.vdouble(0.,0.)), # This bin should never be selected, just here to avoid crashes   
                        #cms.PSet(lowBounds = cms.vdouble(20.000000), upBounds = cms.vdouble(25.000000), values = cms.vdouble(0.989725), uncertainties = cms.vdouble(0.015148,0.015148)),
                        #cms.PSet(lowBounds = cms.vdouble(25.000000), upBounds = cms.vdouble(30.000000), values = cms.vdouble(0.984663), uncertainties = cms.vdouble(0.015053,0.015053)),
                        #cms.PSet(lowBounds = cms.vdouble(30.000000), upBounds = cms.vdouble(40.000000), values = cms.vdouble(0.988170), uncertainties = cms.vdouble(0.015008,0.015008)),
                        #cms.PSet(lowBounds = cms.vdouble(40.000000), upBounds = cms.vdouble(50.000000), values = cms.vdouble(0.986433), uncertainties = cms.vdouble(0.015003,0.015003)),
                        #cms.PSet(lowBounds = cms.vdouble(50.000000), upBounds = cms.vdouble(60.000000), values = cms.vdouble(0.983007), uncertainties = cms.vdouble(0.015022,0.015022)),
                        #cms.PSet(lowBounds = cms.vdouble(60.000000), upBounds = cms.vdouble(80.000000), values = cms.vdouble(0.986717), uncertainties = cms.vdouble(0.015089,0.015089)),
                        #cms.PSet(lowBounds = cms.vdouble(80.000000), upBounds = cms.vdouble(120.000000), values = cms.vdouble(0.977206), uncertainties = cms.vdouble(0.015616,0.015616)),
                        #cms.PSet(lowBounds = cms.vdouble(120.000000), upBounds = cms.vdouble(9999999999.000000), values = cms.vdouble(0.975379), uncertainties = cms.vdouble(0.025048,0.025048))
                        #74X
                        #cms.PSet(lowBounds = cms.vdouble(0.), upBounds = cms.vdouble(20.), values = cms.vdouble(1.), uncertainties = cms.vdouble(0.,0.)), # This bin should never be selected, just here to avoid crashes
                        #cms.PSet(lowBounds = cms.vdouble(20.000000), upBounds = cms.vdouble(25.000000), values = cms.vdouble(0.992659), uncertainties = cms.vdouble(0.014311,0.014311)),
                        #cms.PSet(lowBounds = cms.vdouble(25.000000), upBounds = cms.vdouble(30.000000), values = cms.vdouble(0.988595), uncertainties = cms.vdouble(0.014204,0.014204)),
                        #cms.PSet(lowBounds = cms.vdouble(30.000000), upBounds = cms.vdouble(40.000000), values = cms.vdouble(0.988147), uncertainties = cms.vdouble(0.014151,0.014151)),
                        #cms.PSet(lowBounds = cms.vdouble(40.000000), upBounds = cms.vdouble(50.000000), values = cms.vdouble(0.987304), uncertainties = cms.vdouble(0.014859,0.014859)),
                        #cms.PSet(lowBounds = cms.vdouble(50.000000), upBounds = cms.vdouble(60.000000), values = cms.vdouble(0.983446), uncertainties = cms.vdouble(0.014165,0.014165)),
                        #cms.PSet(lowBounds = cms.vdouble(60.000000), upBounds = cms.vdouble(80.000000), values = cms.vdouble(0.987193), uncertainties = cms.vdouble(0.014235,0.014235)),
                        #cms.PSet(lowBounds = cms.vdouble(80.000000), upBounds = cms.vdouble(120.000000), values = cms.vdouble(0.983453), uncertainties = cms.vdouble(0.014742,0.014742)),
                        #cms.PSet(lowBounds = cms.vdouble(120.000000), upBounds = cms.vdouble(9999999.000000), values = cms.vdouble(1.003003), uncertainties = cms.vdouble(0.027881,0.027881))
                        )
		)	

flashggMuonSystematics = cms.EDProducer('FlashggMuonEffSystematicProducer',
					src = cms.InputTag("flashggSelectedMuons"),
					SystMethods2D = cms.VPSet(),
					SystMethods = cms.VPSet(cms.PSet( MethodName = cms.string("FlashggMuonWeight"),
									  Label = cms.string("MuonWeight"),
									  NSigmas = cms.vint32(-1,1),
									  OverallRange = cms.string("abs(eta)<2.4"),
									  BinList = binInfo,
									  Debug = cms.untracked.bool(False),
                                                                          ApplyCentralValue = cms.bool(True)
                                                                          )
								)
                                        )

