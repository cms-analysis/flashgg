import FWCore.ParameterSet.Config as cms

binInfo = cms.PSet(
		variables = cms.vstring("eta","pt"),
		bins = cms.VPSet(
                        # TightID+LoosePFRelative isolation + Track scale factors : SF = SF(ID | Track)*SF(iso|ID)*SF(Track)
                        # uncertainties are: stat (+) syst. with syst: 1% for ID, 0.5% for iso, 1% to account for different Z vs multijet topology and Track SF unc.
                        # Moriond2017
                        # Preliminary numbers merged for 2016BCDEF and GH taken from : https://twiki.cern.ch/twiki/bin/view/CMS/MuonWorkInProgressAndPagResults
			cms.PSet(lowBounds = cms.vdouble(-5.0,0.00), upBounds = cms.vdouble(-2.1,20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000,0.0000)),
			cms.PSet(lowBounds = cms.vdouble(-5.0,20.00), upBounds = cms.vdouble(-2.1,25.00), values = cms.vdouble(0.9632), uncertainties = cms.vdouble(0.0154,0.0154)),
			cms.PSet(lowBounds = cms.vdouble(-5.0,25.00), upBounds = cms.vdouble(-2.1,30.00), values = cms.vdouble(0.9664), uncertainties = cms.vdouble(0.0292,0.0292)),
			cms.PSet(lowBounds = cms.vdouble(-5.0,30.00), upBounds = cms.vdouble(-2.1,40.00), values = cms.vdouble(0.9611), uncertainties = cms.vdouble(0.0151,0.0151)),
			cms.PSet(lowBounds = cms.vdouble(-5.0,40.00), upBounds = cms.vdouble(-2.1,50.00), values = cms.vdouble(0.9641), uncertainties = cms.vdouble(0.0158,0.0158)),
			cms.PSet(lowBounds = cms.vdouble(-5.0,50.00), upBounds = cms.vdouble(-2.1,60.00), values = cms.vdouble(0.9583), uncertainties = cms.vdouble(0.0153,0.0153)),
			cms.PSet(lowBounds = cms.vdouble(-5.0,60.00), upBounds = cms.vdouble(-2.1,float('inf')), values = cms.vdouble(0.9549), uncertainties = cms.vdouble(0.0161,0.0161)),
			cms.PSet(lowBounds = cms.vdouble(-2.1,0.00), upBounds = cms.vdouble(-1.6,20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000,0.0000)),
			cms.PSet(lowBounds = cms.vdouble(-2.1,20.00), upBounds = cms.vdouble(-1.6,25.00), values = cms.vdouble(0.9808), uncertainties = cms.vdouble(0.0151,0.0151)),
			cms.PSet(lowBounds = cms.vdouble(-2.1,25.00), upBounds = cms.vdouble(-1.6,30.00), values = cms.vdouble(0.9824), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(-2.1,30.00), upBounds = cms.vdouble(-1.6,40.00), values = cms.vdouble(0.9837), uncertainties = cms.vdouble(0.0177,0.0177)),
			cms.PSet(lowBounds = cms.vdouble(-2.1,40.00), upBounds = cms.vdouble(-1.6,50.00), values = cms.vdouble(0.9851), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(-2.1,50.00), upBounds = cms.vdouble(-1.6,60.00), values = cms.vdouble(0.9800), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(-2.1,60.00), upBounds = cms.vdouble(-1.6,float('inf')), values = cms.vdouble(0.9832), uncertainties = cms.vdouble(0.0152,0.0152)),
			cms.PSet(lowBounds = cms.vdouble(-1.6,0.00), upBounds = cms.vdouble(-1.2,20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000,0.0000)),
			cms.PSet(lowBounds = cms.vdouble(-1.6,20.00), upBounds = cms.vdouble(-1.2,25.00), values = cms.vdouble(0.9823), uncertainties = cms.vdouble(0.0151,0.0151)),
			cms.PSet(lowBounds = cms.vdouble(-1.6,25.00), upBounds = cms.vdouble(-1.2,30.00), values = cms.vdouble(0.9839), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(-1.6,30.00), upBounds = cms.vdouble(-1.2,40.00), values = cms.vdouble(0.9852), uncertainties = cms.vdouble(0.0177,0.0177)),
			cms.PSet(lowBounds = cms.vdouble(-1.6,40.00), upBounds = cms.vdouble(-1.2,50.00), values = cms.vdouble(0.9866), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(-1.6,50.00), upBounds = cms.vdouble(-1.2,60.00), values = cms.vdouble(0.9816), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(-1.6,60.00), upBounds = cms.vdouble(-1.2,float('inf')), values = cms.vdouble(0.9848), uncertainties = cms.vdouble(0.0152,0.0152)),
			cms.PSet(lowBounds = cms.vdouble(-1.2,0.00), upBounds = cms.vdouble(-0.9,20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000,0.0000)),
			cms.PSet(lowBounds = cms.vdouble(-1.2,20.00), upBounds = cms.vdouble(-0.9,25.00), values = cms.vdouble(0.9670), uncertainties = cms.vdouble(0.0176,0.0176)),
			cms.PSet(lowBounds = cms.vdouble(-1.2,25.00), upBounds = cms.vdouble(-0.9,30.00), values = cms.vdouble(0.9688), uncertainties = cms.vdouble(0.0152,0.0152)),
			cms.PSet(lowBounds = cms.vdouble(-1.2,30.00), upBounds = cms.vdouble(-0.9,40.00), values = cms.vdouble(0.9713), uncertainties = cms.vdouble(0.0151,0.0151)),
			cms.PSet(lowBounds = cms.vdouble(-1.2,40.00), upBounds = cms.vdouble(-0.9,50.00), values = cms.vdouble(0.9715), uncertainties = cms.vdouble(0.0152,0.0152)),
			cms.PSet(lowBounds = cms.vdouble(-1.2,50.00), upBounds = cms.vdouble(-0.9,60.00), values = cms.vdouble(0.9717), uncertainties = cms.vdouble(0.0151,0.0151)),
			cms.PSet(lowBounds = cms.vdouble(-1.2,60.00), upBounds = cms.vdouble(-0.9,float('inf')), values = cms.vdouble(0.9722), uncertainties = cms.vdouble(0.0153,0.0153)),
			cms.PSet(lowBounds = cms.vdouble(-0.9,0.00), upBounds = cms.vdouble(-0.6,20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000,0.0000)),
			cms.PSet(lowBounds = cms.vdouble(-0.9,20.00), upBounds = cms.vdouble(-0.6,25.00), values = cms.vdouble(0.9771), uncertainties = cms.vdouble(0.0153,0.0153)),
			cms.PSet(lowBounds = cms.vdouble(-0.9,25.00), upBounds = cms.vdouble(-0.6,30.00), values = cms.vdouble(0.9790), uncertainties = cms.vdouble(0.0262,0.0262)),
			cms.PSet(lowBounds = cms.vdouble(-0.9,30.00), upBounds = cms.vdouble(-0.6,40.00), values = cms.vdouble(0.9808), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(-0.9,40.00), upBounds = cms.vdouble(-0.6,50.00), values = cms.vdouble(0.9825), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(-0.9,50.00), upBounds = cms.vdouble(-0.6,60.00), values = cms.vdouble(0.9791), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(-0.9,60.00), upBounds = cms.vdouble(-0.6,float('inf')), values = cms.vdouble(0.9896), uncertainties = cms.vdouble(0.0151,0.0151)),
			cms.PSet(lowBounds = cms.vdouble(-0.6,0.00), upBounds = cms.vdouble(-0.3,20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000,0.0000)),
			cms.PSet(lowBounds = cms.vdouble(-0.6,20.00), upBounds = cms.vdouble(-0.3,25.00), values = cms.vdouble(0.9771), uncertainties = cms.vdouble(0.0153,0.0153)),
			cms.PSet(lowBounds = cms.vdouble(-0.6,25.00), upBounds = cms.vdouble(-0.3,30.00), values = cms.vdouble(0.9790), uncertainties = cms.vdouble(0.0262,0.0262)),
			cms.PSet(lowBounds = cms.vdouble(-0.6,30.00), upBounds = cms.vdouble(-0.3,40.00), values = cms.vdouble(0.9809), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(-0.6,40.00), upBounds = cms.vdouble(-0.3,50.00), values = cms.vdouble(0.9826), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(-0.6,50.00), upBounds = cms.vdouble(-0.3,60.00), values = cms.vdouble(0.9792), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(-0.6,60.00), upBounds = cms.vdouble(-0.3,float('inf')), values = cms.vdouble(0.9896), uncertainties = cms.vdouble(0.0151,0.0151)),
			cms.PSet(lowBounds = cms.vdouble(-0.3,0.00), upBounds = cms.vdouble(-0.2,20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000,0.0000)),
			cms.PSet(lowBounds = cms.vdouble(-0.3,20.00), upBounds = cms.vdouble(-0.2,25.00), values = cms.vdouble(0.9763), uncertainties = cms.vdouble(0.0153,0.0153)),
			cms.PSet(lowBounds = cms.vdouble(-0.3,25.00), upBounds = cms.vdouble(-0.2,30.00), values = cms.vdouble(0.9782), uncertainties = cms.vdouble(0.0262,0.0262)),
			cms.PSet(lowBounds = cms.vdouble(-0.3,30.00), upBounds = cms.vdouble(-0.2,40.00), values = cms.vdouble(0.9801), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(-0.3,40.00), upBounds = cms.vdouble(-0.2,50.00), values = cms.vdouble(0.9818), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(-0.3,50.00), upBounds = cms.vdouble(-0.2,60.00), values = cms.vdouble(0.9784), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(-0.3,60.00), upBounds = cms.vdouble(-0.2,float('inf')), values = cms.vdouble(0.9888), uncertainties = cms.vdouble(0.0151,0.0151)),
			cms.PSet(lowBounds = cms.vdouble(-0.2,0.00), upBounds = cms.vdouble(0.2,20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000,0.0000)),
			cms.PSet(lowBounds = cms.vdouble(-0.2,20.00), upBounds = cms.vdouble(0.2,25.00), values = cms.vdouble(0.9766), uncertainties = cms.vdouble(0.0153,0.0153)),
			cms.PSet(lowBounds = cms.vdouble(-0.2,25.00), upBounds = cms.vdouble(0.2,30.00), values = cms.vdouble(0.9785), uncertainties = cms.vdouble(0.0262,0.0262)),
			cms.PSet(lowBounds = cms.vdouble(-0.2,30.00), upBounds = cms.vdouble(0.2,40.00), values = cms.vdouble(0.9803), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(-0.2,40.00), upBounds = cms.vdouble(0.2,50.00), values = cms.vdouble(0.9820), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(-0.2,50.00), upBounds = cms.vdouble(0.2,60.00), values = cms.vdouble(0.9786), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(-0.2,60.00), upBounds = cms.vdouble(0.2,float('inf')), values = cms.vdouble(0.9891), uncertainties = cms.vdouble(0.0151,0.0151)),
			cms.PSet(lowBounds = cms.vdouble(0.2,0.00), upBounds = cms.vdouble(0.3,20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000,0.0000)),
			cms.PSet(lowBounds = cms.vdouble(0.2,20.00), upBounds = cms.vdouble(0.3,25.00), values = cms.vdouble(0.9773), uncertainties = cms.vdouble(0.0153,0.0153)),
			cms.PSet(lowBounds = cms.vdouble(0.2,25.00), upBounds = cms.vdouble(0.3,30.00), values = cms.vdouble(0.9792), uncertainties = cms.vdouble(0.0262,0.0262)),
			cms.PSet(lowBounds = cms.vdouble(0.2,30.00), upBounds = cms.vdouble(0.3,40.00), values = cms.vdouble(0.9811), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(0.2,40.00), upBounds = cms.vdouble(0.3,50.00), values = cms.vdouble(0.9827), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(0.2,50.00), upBounds = cms.vdouble(0.3,60.00), values = cms.vdouble(0.9793), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(0.2,60.00), upBounds = cms.vdouble(0.3,float('inf')), values = cms.vdouble(0.9898), uncertainties = cms.vdouble(0.0151,0.0151)),
			cms.PSet(lowBounds = cms.vdouble(0.3,0.00), upBounds = cms.vdouble(0.6,20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000	,0.0000)),
			cms.PSet(lowBounds = cms.vdouble(0.3,20.00), upBounds = cms.vdouble(0.6,25.00), values = cms.vdouble(0.9782), uncertainties = cms.vdouble(0.0153,0.0153)),
			cms.PSet(lowBounds = cms.vdouble(0.3,25.00), upBounds = cms.vdouble(0.6,30.00), values = cms.vdouble(0.9800), uncertainties = cms.vdouble(0.0262,0.0262)),
			cms.PSet(lowBounds = cms.vdouble(0.3,30.00), upBounds = cms.vdouble(0.6,40.00), values = cms.vdouble(0.9819), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(0.3,40.00), upBounds = cms.vdouble(0.6,50.00), values = cms.vdouble(0.9836), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(0.3,50.00), upBounds = cms.vdouble(0.6,60.00), values = cms.vdouble(0.9802), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(0.3,60.00), upBounds = cms.vdouble(0.6,float('inf')), values = cms.vdouble(0.9907), uncertainties = cms.vdouble(0.0151,0.0151)),
			cms.PSet(lowBounds = cms.vdouble(0.6,0.00), upBounds = cms.vdouble(0.9,20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000,0.0000)),
			cms.PSet(lowBounds = cms.vdouble(0.6,20.00), upBounds = cms.vdouble(0.9,25.00), values = cms.vdouble(0.9779), uncertainties = cms.vdouble(0.0153,0.0153)),
			cms.PSet(lowBounds = cms.vdouble(0.6,25.00), upBounds = cms.vdouble(0.9,30.00), values = cms.vdouble(0.9798), uncertainties = cms.vdouble(0.0262,0.0262)),
			cms.PSet(lowBounds = cms.vdouble(0.6,30.00), upBounds = cms.vdouble(0.9,40.00), values = cms.vdouble(0.9816), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(0.6,40.00), upBounds = cms.vdouble(0.9,50.00), values = cms.vdouble(0.9833), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(0.6,50.00), upBounds = cms.vdouble(0.9,60.00), values = cms.vdouble(0.9799), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(0.6,60.00), upBounds = cms.vdouble(0.9,float('inf')), values = cms.vdouble(0.9904), uncertainties = cms.vdouble(0.0151,0.0151)),
			cms.PSet(lowBounds = cms.vdouble(0.9,0.00), upBounds = cms.vdouble(1.2,	20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000,0.0000)),
			cms.PSet(lowBounds = cms.vdouble(0.9,20.00), upBounds = cms.vdouble(1.2,25.00), values = cms.vdouble(0.9675), uncertainties = cms.vdouble(0.0176,0.0176)),
			cms.PSet(lowBounds = cms.vdouble(0.9,25.00), upBounds = cms.vdouble(1.2,30.00), values = cms.vdouble(0.9693), uncertainties = cms.vdouble(0.0152,0.0152)),
			cms.PSet(lowBounds = cms.vdouble(0.9,30.00), upBounds = cms.vdouble(1.2,40.00), values = cms.vdouble(0.9718), uncertainties = cms.vdouble(0.0151,0.0151)),
			cms.PSet(lowBounds = cms.vdouble(0.9,40.00), upBounds = cms.vdouble(1.2,50.00), values = cms.vdouble(0.9721), uncertainties = cms.vdouble(0.0152,0.0152)),
			cms.PSet(lowBounds = cms.vdouble(0.9,50.00), upBounds = cms.vdouble(1.2,60.00), values = cms.vdouble(0.9722), uncertainties = cms.vdouble(0.0151,0.0151)),
			cms.PSet(lowBounds = cms.vdouble(0.9,60.00), upBounds = cms.vdouble(1.2,float('inf')), values = cms.vdouble(0.9727), uncertainties = cms.vdouble(0.0153,0.0153)),
			cms.PSet(lowBounds = cms.vdouble(1.2,0.00), upBounds = cms.vdouble(1.6,20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000,0.0000)),
			cms.PSet(lowBounds = cms.vdouble(1.2,20.00), upBounds = cms.vdouble(1.6,25.00), values = cms.vdouble(0.9812), uncertainties = cms.vdouble(0.0151,0.0151)),
			cms.PSet(lowBounds = cms.vdouble(1.2,25.00), upBounds = cms.vdouble(1.6,30.00), values = cms.vdouble(0.9828), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(1.2,30.00), upBounds = cms.vdouble(1.6,40.00), values = cms.vdouble(0.9841), uncertainties = cms.vdouble(0.0178,0.0178)),
			cms.PSet(lowBounds = cms.vdouble(1.2,40.00), upBounds = cms.vdouble(1.6,50.00), values = cms.vdouble(0.9855), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(1.2,50.00), upBounds = cms.vdouble(1.6,60.00), values = cms.vdouble(0.9804), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(1.2,60.00), upBounds = cms.vdouble(1.6,float('inf')), values = cms.vdouble(0.9836), uncertainties = cms.vdouble(0.0152,0.0152)),
			cms.PSet(lowBounds = cms.vdouble(1.6,0.00), upBounds = cms.vdouble(2.1,20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000,0.0000)),
			cms.PSet(lowBounds = cms.vdouble(1.6,20.00), upBounds = cms.vdouble(2.1,25.00), values = cms.vdouble(0.9809), uncertainties = cms.vdouble(0.0151,0.0151)),
			cms.PSet(lowBounds = cms.vdouble(1.6,25.00), upBounds = cms.vdouble(2.1,30.00), values = cms.vdouble(0.9825), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(1.6,30.00), upBounds = cms.vdouble(2.1,40.00), values = cms.vdouble(0.9837), uncertainties = cms.vdouble(0.0177,0.0177)),
			cms.PSet(lowBounds = cms.vdouble(1.6,40.00), upBounds = cms.vdouble(2.1,50.00), values = cms.vdouble(0.9851), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(1.6,50.00), upBounds = cms.vdouble(2.1,60.00), values = cms.vdouble(0.9801), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(1.6,60.00), upBounds = cms.vdouble(2.1,float('inf')), values = cms.vdouble(0.9833), uncertainties = cms.vdouble(0.0152,0.0152)),
			cms.PSet(lowBounds = cms.vdouble(2.1,0.00), upBounds = cms.vdouble(2.4,20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000,0.0000)),
			cms.PSet(lowBounds = cms.vdouble(2.1,20.00), upBounds = cms.vdouble(2.4,25.00), values = cms.vdouble(0.9594), uncertainties = cms.vdouble(0.0154,0.0154)),
			cms.PSet(lowBounds = cms.vdouble(2.1,25.00), upBounds = cms.vdouble(2.4,30.00), values = cms.vdouble(0.9626), uncertainties = cms.vdouble(0.0292,0.0292)),
			cms.PSet(lowBounds = cms.vdouble(2.1,30.00), upBounds = cms.vdouble(2.4,40.00), values = cms.vdouble(0.9574), uncertainties = cms.vdouble(0.0151,0.0151)),
			cms.PSet(lowBounds = cms.vdouble(2.1,40.00), upBounds = cms.vdouble(2.4,50.00), values = cms.vdouble(0.9603), uncertainties = cms.vdouble(0.0158,0.0158)),
			cms.PSet(lowBounds = cms.vdouble(2.1,50.00), upBounds = cms.vdouble(2.4,60.00), values = cms.vdouble(0.9546), uncertainties = cms.vdouble(0.0153,0.0153)),
			cms.PSet(lowBounds = cms.vdouble(2.1,60.00), upBounds = cms.vdouble(2.4,float('inf')), values = cms.vdouble(0.9511), uncertainties = cms.vdouble(0.0161,0.0161))
                        )
		)	

binMiniIsoInfo = cms.PSet(
                variables = cms.vstring("eta","pt"),
                bins = cms.VPSet(
                        # TightID+Mini isolation + Track scale factors : SF = SF(ID | Track)*SF(iso|ID)*SF(Track)
                        # uncertainties are: stat (+) syst. with syst: 1% for ID, 0.5% for iso, 1% to account for different Z vs multijet topology and Track SF unc.
                        # Moriond2017
                        # Preliminary numbers merged for 2016BCDEF and GH taken from : https://twiki.cern.ch/twiki/bin/view/CMS/MuonWorkInProgressAndPagResults
			# MinIso SFs separately calculated
			cms.PSet(lowBounds = cms.vdouble(-5.0,0.00), upBounds = cms.vdouble(-2.1,20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000,0.0000)),
			cms.PSet(lowBounds = cms.vdouble(-5.0,20.00), upBounds = cms.vdouble(-2.1,25.00), values = cms.vdouble(0.9615), uncertainties = cms.vdouble(0.0154,0.0154)),
			cms.PSet(lowBounds = cms.vdouble(-5.0,25.00), upBounds = cms.vdouble(-2.1,30.00), values = cms.vdouble(0.9612), uncertainties = cms.vdouble(0.0292,0.0292)),
			cms.PSet(lowBounds = cms.vdouble(-5.0,30.00), upBounds = cms.vdouble(-2.1,40.00), values = cms.vdouble(0.9604), uncertainties = cms.vdouble(0.0151,0.0151)),
			cms.PSet(lowBounds = cms.vdouble(-5.0,40.00), upBounds = cms.vdouble(-2.1,50.00), values = cms.vdouble(0.9632), uncertainties = cms.vdouble(0.0158,0.0158)),
			cms.PSet(lowBounds = cms.vdouble(-5.0,50.00), upBounds = cms.vdouble(-2.1,60.00), values = cms.vdouble(0.9580), uncertainties = cms.vdouble(0.0153,0.0153)),
			cms.PSet(lowBounds = cms.vdouble(-5.0,60.00), upBounds = cms.vdouble(-2.1,float('inf')), values = cms.vdouble(0.9552), uncertainties = cms.vdouble(0.0161,0.0161)),
			cms.PSet(lowBounds = cms.vdouble(-2.1,0.00), upBounds = cms.vdouble(-1.6,20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000,0.0000)),
			cms.PSet(lowBounds = cms.vdouble(-2.1,20.00), upBounds = cms.vdouble(-1.6,25.00), values = cms.vdouble(0.9760), uncertainties = cms.vdouble(0.0152,0.0152)),
			cms.PSet(lowBounds = cms.vdouble(-2.1,25.00), upBounds = cms.vdouble(-1.6,30.00), values = cms.vdouble(0.9780), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(-2.1,30.00), upBounds = cms.vdouble(-1.6,40.00), values = cms.vdouble(0.9803), uncertainties = cms.vdouble(0.0177,0.0177)),
			cms.PSet(lowBounds = cms.vdouble(-2.1,40.00), upBounds = cms.vdouble(-1.6,50.00), values = cms.vdouble(0.9831), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(-2.1,50.00), upBounds = cms.vdouble(-1.6,60.00), values = cms.vdouble(0.9783), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(-2.1,60.00), upBounds = cms.vdouble(-1.6,float('inf')), values = cms.vdouble(0.9828), uncertainties = cms.vdouble(0.0152,0.0152)),
			cms.PSet(lowBounds = cms.vdouble(-1.6,0.00), upBounds = cms.vdouble(-1.2,20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000,0.0000)),
			cms.PSet(lowBounds = cms.vdouble(-1.6,20.00), upBounds = cms.vdouble(-1.2,25.00), values = cms.vdouble(0.9776), uncertainties = cms.vdouble(0.0152,0.0152)),
			cms.PSet(lowBounds = cms.vdouble(-1.6,25.00), upBounds = cms.vdouble(-1.2,30.00), values = cms.vdouble(0.9796), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(-1.6,30.00), upBounds = cms.vdouble(-1.2,40.00), values = cms.vdouble(0.9819), uncertainties = cms.vdouble(0.0177,0.0177)),
			cms.PSet(lowBounds = cms.vdouble(-1.6,40.00), upBounds = cms.vdouble(-1.2,50.00), values = cms.vdouble(0.9846), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(-1.6,50.00), upBounds = cms.vdouble(-1.2,60.00), values = cms.vdouble(0.9798), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(-1.6,60.00), upBounds = cms.vdouble(-1.2,float('inf')), values = cms.vdouble(0.9843), uncertainties = cms.vdouble(0.0152,0.0152)),
			cms.PSet(lowBounds = cms.vdouble(-1.2,0.00), upBounds = cms.vdouble(-0.9,20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000,0.0000)),
			cms.PSet(lowBounds = cms.vdouble(-1.2,20.00), upBounds = cms.vdouble(-0.9,25.00), values = cms.vdouble(0.9637), uncertainties = cms.vdouble(0.0176,0.0176)),
			cms.PSet(lowBounds = cms.vdouble(-1.2,25.00), upBounds = cms.vdouble(-0.9,30.00), values = cms.vdouble(0.9623), uncertainties = cms.vdouble(0.0152,0.0152)),
			cms.PSet(lowBounds = cms.vdouble(-1.2,30.00), upBounds = cms.vdouble(-0.9,40.00), values = cms.vdouble(0.9684), uncertainties = cms.vdouble(0.0151,0.0151)),
			cms.PSet(lowBounds = cms.vdouble(-1.2,40.00), upBounds = cms.vdouble(-0.9,50.00), values = cms.vdouble(0.9692), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(-1.2,50.00), upBounds = cms.vdouble(-0.9,60.00), values = cms.vdouble(0.9725), uncertainties = cms.vdouble(0.0151,0.0151)),
			cms.PSet(lowBounds = cms.vdouble(-1.2,60.00), upBounds = cms.vdouble(-0.9,float('inf')), values = cms.vdouble(0.9723), uncertainties = cms.vdouble(0.0153,0.0153)),
			cms.PSet(lowBounds = cms.vdouble(-0.9,0.00), upBounds = cms.vdouble(-0.6,20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000,0.0000)),
			cms.PSet(lowBounds = cms.vdouble(-0.9,20.00), upBounds = cms.vdouble(-0.6,25.00), values = cms.vdouble(0.9791), uncertainties = cms.vdouble(0.0153,0.0153)),
			cms.PSet(lowBounds = cms.vdouble(-0.9,25.00), upBounds = cms.vdouble(-0.6,30.00), values = cms.vdouble(0.9775), uncertainties = cms.vdouble(0.0262,0.0262)),
			cms.PSet(lowBounds = cms.vdouble(-0.9,30.00), upBounds = cms.vdouble(-0.6,40.00), values = cms.vdouble(0.9776), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(-0.9,40.00), upBounds = cms.vdouble(-0.6,50.00), values = cms.vdouble(0.9798), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(-0.9,50.00), upBounds = cms.vdouble(-0.6,60.00), values = cms.vdouble(0.9767), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(-0.9,60.00), upBounds = cms.vdouble(-0.6,float('inf')), values = cms.vdouble(0.9885), uncertainties = cms.vdouble(0.0151,0.0151)),
			cms.PSet(lowBounds = cms.vdouble(-0.6,0.00), upBounds = cms.vdouble(-0.3,20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000,0.0000)),
			cms.PSet(lowBounds = cms.vdouble(-0.6,20.00), upBounds = cms.vdouble(-0.3,25.00), values = cms.vdouble(0.9791), uncertainties = cms.vdouble(0.0153,0.0153)),
			cms.PSet(lowBounds = cms.vdouble(-0.6,25.00), upBounds = cms.vdouble(-0.3,30.00), values = cms.vdouble(0.9776), uncertainties = cms.vdouble(0.0262,0.0262)),
			cms.PSet(lowBounds = cms.vdouble(-0.6,30.00), upBounds = cms.vdouble(-0.3,40.00), values = cms.vdouble(0.9777), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(-0.6,40.00), upBounds = cms.vdouble(-0.3,50.00), values = cms.vdouble(0.9799), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(-0.6,50.00), upBounds = cms.vdouble(-0.3,60.00), values = cms.vdouble(0.9768), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(-0.6,60.00), upBounds = cms.vdouble(-0.3,float('inf')), values = cms.vdouble(0.9885), uncertainties = cms.vdouble(0.0151,0.0151)),
			cms.PSet(lowBounds = cms.vdouble(-0.3,0.00), upBounds = cms.vdouble(-0.2,20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000,0.0000)),
			cms.PSet(lowBounds = cms.vdouble(-0.3,20.00), upBounds = cms.vdouble(-0.2,25.00), values = cms.vdouble(0.9783), uncertainties = cms.vdouble(0.0153,0.0153)),
			cms.PSet(lowBounds = cms.vdouble(-0.3,25.00), upBounds = cms.vdouble(-0.2,30.00), values = cms.vdouble(0.9768), uncertainties = cms.vdouble(0.0262,0.0262)),
			cms.PSet(lowBounds = cms.vdouble(-0.3,30.00), upBounds = cms.vdouble(-0.2,40.00), values = cms.vdouble(0.9769), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(-0.3,40.00), upBounds = cms.vdouble(-0.2,50.00), values = cms.vdouble(0.9791), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(-0.3,50.00), upBounds = cms.vdouble(-0.2,60.00), values = cms.vdouble(0.9760), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(-0.3,60.00), upBounds = cms.vdouble(-0.2,float('inf')), values = cms.vdouble(0.9877), uncertainties = cms.vdouble(0.0151,0.0151)),
			cms.PSet(lowBounds = cms.vdouble(-0.2,0.00), upBounds = cms.vdouble(0.2,20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000,0.0000)),
			cms.PSet(lowBounds = cms.vdouble(-0.2,20.00), upBounds = cms.vdouble(0.2,25.00), values = cms.vdouble(0.9786), uncertainties = cms.vdouble(0.0153,0.0153)),
			cms.PSet(lowBounds = cms.vdouble(-0.2,25.00), upBounds = cms.vdouble(0.2,30.00), values = cms.vdouble(0.9770), uncertainties = cms.vdouble(0.0262,0.0262)),
			cms.PSet(lowBounds = cms.vdouble(-0.2,30.00), upBounds = cms.vdouble(0.2,40.00), values = cms.vdouble(0.9771), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(-0.2,40.00), upBounds = cms.vdouble(0.2,50.00), values = cms.vdouble(0.9793), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(-0.2,50.00), upBounds = cms.vdouble(0.2,60.00), values = cms.vdouble(0.9762), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(-0.2,60.00), upBounds = cms.vdouble(0.2,float('inf')), values = cms.vdouble(0.9880), uncertainties = cms.vdouble(0.0151,0.0151)),
			cms.PSet(lowBounds = cms.vdouble(0.2,0.00), upBounds = cms.vdouble(0.3,20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000,0.0000)),
			cms.PSet(lowBounds = cms.vdouble(0.2,20.00), upBounds = cms.vdouble(0.3,25.00), values = cms.vdouble(0.9793), uncertainties = cms.vdouble(0.0153,0.0153)),
			cms.PSet(lowBounds = cms.vdouble(0.2,25.00), upBounds = cms.vdouble(0.3,30.00), values = cms.vdouble(0.9777), uncertainties = cms.vdouble(0.0262,0.0262)),
			cms.PSet(lowBounds = cms.vdouble(0.2,30.00), upBounds = cms.vdouble(0.3,40.00), values = cms.vdouble(0.9778), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(0.2,40.00), upBounds = cms.vdouble(0.3,50.00), values = cms.vdouble(0.9800), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(0.2,50.00), upBounds = cms.vdouble(0.3,60.00), values = cms.vdouble(0.9769), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(0.2,60.00), upBounds = cms.vdouble(0.3,float('inf')), values = cms.vdouble(0.9887), uncertainties = cms.vdouble(0.0151,0.0151)),
			cms.PSet(lowBounds = cms.vdouble(0.3,0.00), upBounds = cms.vdouble(0.6,20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000,0.0000)),
			cms.PSet(lowBounds = cms.vdouble(0.3,20.00), upBounds = cms.vdouble(0.6,25.00), values = cms.vdouble(0.9802), uncertainties = cms.vdouble(0.0153,0.0153)),
			cms.PSet(lowBounds = cms.vdouble(0.3,25.00), upBounds = cms.vdouble(0.6,30.00), values = cms.vdouble(0.9786), uncertainties = cms.vdouble(0.0262,0.0262)),
			cms.PSet(lowBounds = cms.vdouble(0.3,30.00), upBounds = cms.vdouble(0.6,40.00), values = cms.vdouble(0.9787), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(0.3,40.00), upBounds = cms.vdouble(0.6,50.00), values = cms.vdouble(0.9809), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(0.3,50.00), upBounds = cms.vdouble(0.6,60.00), values = cms.vdouble(0.9778), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(0.3,60.00), upBounds = cms.vdouble(0.6,float('inf')), values = cms.vdouble(0.9896), uncertainties = cms.vdouble(0.0151,0.0151)),
			cms.PSet(lowBounds = cms.vdouble(0.6,0.00), upBounds = cms.vdouble(0.9,20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000,0.0000)),
			cms.PSet(lowBounds = cms.vdouble(0.6,20.00), upBounds = cms.vdouble(0.9,25.00), values = cms.vdouble(0.9799), uncertainties = cms.vdouble(0.0153,0.0153)),
			cms.PSet(lowBounds = cms.vdouble(0.6,25.00), upBounds = cms.vdouble(0.9,30.00), values = cms.vdouble(0.9783), uncertainties = cms.vdouble(0.0262,0.0262)),
			cms.PSet(lowBounds = cms.vdouble(0.6,30.00), upBounds = cms.vdouble(0.9,40.00), values = cms.vdouble(0.9784), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(0.6,40.00), upBounds = cms.vdouble(0.9,50.00), values = cms.vdouble(0.9806), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(0.6,50.00), upBounds = cms.vdouble(0.9,60.00), values = cms.vdouble(0.9775), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(0.6,60.00), upBounds = cms.vdouble(0.9,float('inf')), values = cms.vdouble(0.9893), uncertainties = cms.vdouble(0.0151,0.0151)),
			cms.PSet(lowBounds = cms.vdouble(0.9,0.00), upBounds = cms.vdouble(1.2,20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000,0.0000)),
			cms.PSet(lowBounds = cms.vdouble(0.9,20.00), upBounds = cms.vdouble(1.2,25.00), values = cms.vdouble(0.9642), uncertainties = cms.vdouble(0.0176,0.0176)),
			cms.PSet(lowBounds = cms.vdouble(0.9,25.00), upBounds = cms.vdouble(1.2,30.00), values = cms.vdouble(0.9628), uncertainties = cms.vdouble(0.0152,0.0152)),
			cms.PSet(lowBounds = cms.vdouble(0.9,30.00), upBounds = cms.vdouble(1.2,40.00), values = cms.vdouble(0.9689), uncertainties = cms.vdouble(0.0151,0.0151)),
			cms.PSet(lowBounds = cms.vdouble(0.9,40.00), upBounds = cms.vdouble(1.2,50.00), values = cms.vdouble(0.9697), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(0.9,50.00), upBounds = cms.vdouble(1.2,60.00), values = cms.vdouble(0.9730), uncertainties = cms.vdouble(0.0151,0.0151)),
			cms.PSet(lowBounds = cms.vdouble(0.9,60.00), upBounds = cms.vdouble(1.2,float('inf')), values = cms.vdouble(0.9728), uncertainties = cms.vdouble(0.0153,0.0153)),
			cms.PSet(lowBounds = cms.vdouble(1.2,0.00), upBounds = cms.vdouble(1.6,20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000,0.0000)),
			cms.PSet(lowBounds = cms.vdouble(1.2,20.00), upBounds = cms.vdouble(1.6,25.00), values = cms.vdouble(0.9764), uncertainties = cms.vdouble(0.0152,0.0152)),
			cms.PSet(lowBounds = cms.vdouble(1.2,25.00), upBounds = cms.vdouble(1.6,30.00), values = cms.vdouble(0.9784), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(1.2,30.00), upBounds = cms.vdouble(1.6,40.00), values = cms.vdouble(0.9807), uncertainties = cms.vdouble(0.0178,0.0178)),
			cms.PSet(lowBounds = cms.vdouble(1.2,40.00), upBounds = cms.vdouble(1.6,50.00), values = cms.vdouble(0.9835), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(1.2,50.00), upBounds = cms.vdouble(1.6,60.00), values = cms.vdouble(0.9787), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(1.2,60.00), upBounds = cms.vdouble(1.6,float('inf')), values = cms.vdouble(0.9832), uncertainties = cms.vdouble(0.0152,0.0152)),
			cms.PSet(lowBounds = cms.vdouble(1.6,0.00), upBounds = cms.vdouble(2.1,20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000,0.0000)),
			cms.PSet(lowBounds = cms.vdouble(1.6,20.00), upBounds = cms.vdouble(2.1,25.00), values = cms.vdouble(0.9761), uncertainties = cms.vdouble(0.0152,0.0152)),
			cms.PSet(lowBounds = cms.vdouble(1.6,25.00), upBounds = cms.vdouble(2.1,30.00), values = cms.vdouble(0.9781), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(1.6,30.00), upBounds = cms.vdouble(2.1,40.00), values = cms.vdouble(0.9804), uncertainties = cms.vdouble(0.0177,0.0177)),
			cms.PSet(lowBounds = cms.vdouble(1.6,40.00), upBounds = cms.vdouble(2.1,50.00), values = cms.vdouble(0.9831), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(1.6,50.00), upBounds = cms.vdouble(2.1,60.00), values = cms.vdouble(0.9784), uncertainties = cms.vdouble(0.0150,0.0150)),
			cms.PSet(lowBounds = cms.vdouble(1.6,60.00), upBounds = cms.vdouble(2.1,float('inf')), values = cms.vdouble(0.9829), uncertainties = cms.vdouble(0.0152,0.0152)),
			cms.PSet(lowBounds = cms.vdouble(2.1,0.00), upBounds = cms.vdouble(2.4,20.00), values = cms.vdouble(1.0000), uncertainties = cms.vdouble(0.0000,0.0000)),
			cms.PSet(lowBounds = cms.vdouble(2.1,20.00), upBounds = cms.vdouble(2.4,25.00), values = cms.vdouble(0.9578), uncertainties = cms.vdouble(0.0154,0.0154)),
			cms.PSet(lowBounds = cms.vdouble(2.1,25.00), upBounds = cms.vdouble(2.4,30.00), values = cms.vdouble(0.9574), uncertainties = cms.vdouble(0.0292,0.0292)),
			cms.PSet(lowBounds = cms.vdouble(2.1,30.00), upBounds = cms.vdouble(2.4,40.00), values = cms.vdouble(0.9566), uncertainties = cms.vdouble(0.0151,0.0151)),
			cms.PSet(lowBounds = cms.vdouble(2.1,40.00), upBounds = cms.vdouble(2.4,50.00), values = cms.vdouble(0.9594), uncertainties = cms.vdouble(0.0158,0.0158)),
			cms.PSet(lowBounds = cms.vdouble(2.1,50.00), upBounds = cms.vdouble(2.4,60.00), values = cms.vdouble(0.9542), uncertainties = cms.vdouble(0.0153,0.0153)),
			cms.PSet(lowBounds = cms.vdouble(2.1,60.00), upBounds = cms.vdouble(2.4,float('inf')), values = cms.vdouble(0.9514), uncertainties = cms.vdouble(0.0161,0.0161))
                        )
                )


PSetMuonWeight2016 =  cms.PSet( MethodName = cms.string("FlashggMuonWeight"),
          Label = cms.string("MuonWeight"),
          NSigmas = cms.vint32(-1,1),
          OverallRange = cms.string("abs(eta)<2.4"),
          BinList = binInfo,
          Debug = cms.untracked.bool(False),
          ApplyCentralValue = cms.bool(True)
          )
PSetMuonMiniIsoWeight2016 = cms.PSet( MethodName = cms.string("FlashggMuonWeight"),
          Label = cms.string("MuonMiniIsoWeight"),
          NSigmas = cms.vint32(-1,1),
          OverallRange = cms.string("abs(eta)<2.4"),
          BinList = binMiniIsoInfo,
          Debug = cms.untracked.bool(False),
          ApplyCentralValue = cms.bool(True)
          )