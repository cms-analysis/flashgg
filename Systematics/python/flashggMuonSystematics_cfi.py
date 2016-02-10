import FWCore.ParameterSet.Config as cms

binInfo = cms.PSet(
		variables = cms.vstring("pt"),
		bins = cms.VPSet(
	                # TightID+LoosePFRelative isolation scale factors : SF = SF(ID)*SF(iso|ID)
                        # taken from : https://twiki.cern.ch/twiki/bin/view/CMS/MuonReferenceEffsRun2
                        cms.PSet(lowBounds = cms.vdouble(0.), upBounds = cms.vdouble(20.), values = cms.vdouble(1.), uncertainties = cms.vdouble(0.,0.)), # This bin should never be selected, just here to avoid crashes
                        cms.PSet(lowBounds = cms.vdouble(20.000000), upBounds = cms.vdouble(25.000000), values = cms.vdouble(0.992659), uncertainties = cms.vdouble(0.014311,0.014311)),
                        cms.PSet(lowBounds = cms.vdouble(25.000000), upBounds = cms.vdouble(30.000000), values = cms.vdouble(0.988595), uncertainties = cms.vdouble(0.014204,0.014204)),
                        cms.PSet(lowBounds = cms.vdouble(30.000000), upBounds = cms.vdouble(40.000000), values = cms.vdouble(0.988147), uncertainties = cms.vdouble(0.014151,0.014151)),
                        cms.PSet(lowBounds = cms.vdouble(40.000000), upBounds = cms.vdouble(50.000000), values = cms.vdouble(0.987304), uncertainties = cms.vdouble(0.014859,0.014859)),
                        cms.PSet(lowBounds = cms.vdouble(50.000000), upBounds = cms.vdouble(60.000000), values = cms.vdouble(0.983446), uncertainties = cms.vdouble(0.014165,0.014165)),
                        cms.PSet(lowBounds = cms.vdouble(60.000000), upBounds = cms.vdouble(80.000000), values = cms.vdouble(0.987193), uncertainties = cms.vdouble(0.014235,0.014235)),
                        cms.PSet(lowBounds = cms.vdouble(80.000000), upBounds = cms.vdouble(120.000000), values = cms.vdouble(0.983453), uncertainties = cms.vdouble(0.014742,0.014742)),
                        cms.PSet(lowBounds = cms.vdouble(120.000000), upBounds = cms.vdouble(9999999.000000), values = cms.vdouble(1.003003), uncertainties = cms.vdouble(0.027881,0.027881))
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

