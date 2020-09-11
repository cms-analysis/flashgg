import os
import FWCore.ParameterSet.Config as cms
import flashgg.Systematics.settings as settings

#https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideBTagMCTools#Hadron_parton_based_jet_flavour
#B Tag MC efficiencies

####Setting JEC and JER
# JetCorrectorParametersCollection_version = "JetCorrectorParametersCollection_Summer16_23Sep2016V4_MC_AK4PFchs"    
# JR_PtResolution_version = "JR_Summer16_23Sep2016V4_MC_PtResolution_AK4PFchs"
# JR_SF_version = "JR_Summer16_23Sep2016V4_MC_SF_AK4PFchs"
# year = settings.year
# process_type = settings.process_type
# if year == "2016":
#    JetCorrectorParametersCollection_version = "JetCorrectorParametersCollection_Summer16_23Sep2016V4_MC_AK4PFchs"
#    if process_type == 'Data' : JetCorrectorParametersCollection_version = "JetCorrectorParametersCollection_Summer16_23Sep2016AllV4_DATA_AK4PFchs"
#    JR_PtResolution_version = "JR_Summer16_23Sep2016V4_MC_PtResolution_AK4PFchs"
#    JR_SF_version = "JR_Summer16_23Sep2016V4_MC_SF_AK4PFchs"
# elif year == "2017":
#    JetCorrectorParametersCollection_version = "JetCorrectorParametersCollection_Fall17_17Nov2017_V6_MC_AK4PFchs"
#    if process_type == 'Data' : JetCorrectorParametersCollection_version = "JetCorrectorParametersCollection_Fall17_17Nov2017BCDEF_V6_DATA_AK4PFchs"
#    JR_PtResolution_version = "JR_Fall17_17Nov2017_V6_MC_PtResolution_AK4PFchs"
#    JR_SF_version = "JR_Fall17_17Nov2017_V6_MC_SF_AK4PFchs"

class jetSystematicsCustomize:
   def __init__(self, process, options):
      self.process = process
      self.options = options
      self.metaConditions = options.metaConditions
   
      self.bTagEffBins2016 = cms.PSet(
         variables = cms.vstring("hadronFlavour","abs(eta)","pt"),
         bins = cms.VPSet(

               #values for 0 to 50 GeV are actually for 25GeV to 50 GeV that is used in ttH analysis, kept the lower bound to 0 to avoid any errors
                     
                              #udsg jets
            cms.PSet(lowBounds = cms.vdouble(-0.5,0.0,0.0), upBounds = cms.vdouble(3.5,2.4,50.0), values = cms.vdouble(0.0125268), uncertainties = cms.vdouble(0.0)),
            cms.PSet(lowBounds = cms.vdouble(-0.5,0.0,50.0), upBounds = cms.vdouble(3.5,2.4,100.0), values = cms.vdouble(0.00932831), uncertainties = cms.vdouble(0.0)),
            cms.PSet(lowBounds = cms.vdouble(-0.5,0.0,100.0), upBounds = cms.vdouble(3.5,2.4,200.0), values = cms.vdouble(0.00976553), uncertainties = cms.vdouble(0.0)),
            cms.PSet(lowBounds = cms.vdouble(-0.5,0.0,200.0), upBounds = cms.vdouble(3.5,2.4,999999.0), values = cms.vdouble(0.0141429), uncertainties = cms.vdouble(0.0)),
                             #
            cms.PSet(lowBounds = cms.vdouble(-0.5,2.4,0.0), upBounds = cms.vdouble(3.5,10.0,50.0), values = cms.vdouble(0.0), uncertainties = cms.vdouble(0.0)),
            cms.PSet(lowBounds = cms.vdouble(-0.5,2.4,50.0), upBounds = cms.vdouble(3.5,10.0,100.0), values = cms.vdouble(0.0), uncertainties = cms.vdouble(0.0)),
            cms.PSet(lowBounds = cms.vdouble(-0.5,2.4,100.0), upBounds = cms.vdouble(3.5,10.0,200.0), values = cms.vdouble(0.0), uncertainties = cms.vdouble(0.0)),
            cms.PSet(lowBounds = cms.vdouble(-0.5,2.4,200.0), upBounds = cms.vdouble(3.5,10.0,999999.0), values = cms.vdouble(0.0), uncertainties = cms.vdouble(0.0)),
            #c jets
            cms.PSet(lowBounds = cms.vdouble(3.5,0.0,0.0), upBounds = cms.vdouble(4.5,2.4,50.0), values = cms.vdouble(0.122918), uncertainties = cms.vdouble(0.0)),
            cms.PSet(lowBounds = cms.vdouble(3.5,0.0,50.0), upBounds = cms.vdouble(4.5,2.4,100.0), values = cms.vdouble(0.124872), uncertainties = cms.vdouble(0.0)),
            cms.PSet(lowBounds = cms.vdouble(3.5,0.0,100.0), upBounds = cms.vdouble(4.5,2.4,200.0), values = cms.vdouble(0.135278), uncertainties = cms.vdouble(0.0)),
            cms.PSet(lowBounds = cms.vdouble(3.5,0.0,200.0), upBounds = cms.vdouble(4.5,2.4,999999.0), values = cms.vdouble(0.135077), uncertainties = cms.vdouble(0.0)),
            #
                             cms.PSet(lowBounds = cms.vdouble(3.5,2.4,0.0), upBounds = cms.vdouble(4.5,10.0,50.0), values = cms.vdouble(0.0), uncertainties = cms.vdouble(0.0)),
            cms.PSet(lowBounds = cms.vdouble(3.5,2.4,50.0), upBounds = cms.vdouble(4.5,10.0,100.0), values = cms.vdouble(0.0), uncertainties = cms.vdouble(0.0)),
            cms.PSet(lowBounds = cms.vdouble(3.5,2.4,100.0), upBounds = cms.vdouble(4.5,10.0,200.0), values = cms.vdouble(0.0), uncertainties = cms.vdouble(0.0)),
            cms.PSet(lowBounds = cms.vdouble(3.5,2.4,200.0), upBounds = cms.vdouble(4.5,10.0,999999.0), values = cms.vdouble(0.0), uncertainties = cms.vdouble(0.0)),
            #b jets
            cms.PSet(lowBounds = cms.vdouble(4.5,0.0,0.0), upBounds = cms.vdouble(5.5,2.4,50.0), values = cms.vdouble(0.590803), uncertainties = cms.vdouble(0.0)),
            cms.PSet(lowBounds = cms.vdouble(4.5,0.0,50.0), upBounds = cms.vdouble(5.5,2.4,100.0), values = cms.vdouble(0.639897), uncertainties = cms.vdouble(0.0)),
            cms.PSet(lowBounds = cms.vdouble(4.5,0.0,100.0), upBounds = cms.vdouble(5.5,2.4,200.0), values = cms.vdouble(0.639406), uncertainties = cms.vdouble(0.0)),
            cms.PSet(lowBounds = cms.vdouble(4.5,0.0,200.0), upBounds = cms.vdouble(5.5,2.4,999999.0), values = cms.vdouble(0.584623), uncertainties = cms.vdouble(0.0)),
            #
                             cms.PSet(lowBounds = cms.vdouble(4.5,2.4,0.0), upBounds = cms.vdouble(5.5,10.0,50.0), values = cms.vdouble(1.0), uncertainties = cms.vdouble(0.0)),
            cms.PSet(lowBounds = cms.vdouble(4.5,2.4,50.0), upBounds = cms.vdouble(5.5,10.0,100.0), values = cms.vdouble(1.0), uncertainties = cms.vdouble(0.0)),
            cms.PSet(lowBounds = cms.vdouble(4.5,2.4,100.0), upBounds = cms.vdouble(5.5,10.0,200.0), values = cms.vdouble(1.0), uncertainties = cms.vdouble(0.0)),
            cms.PSet(lowBounds = cms.vdouble(4.5,2.4,200.0), upBounds = cms.vdouble(5.5,10.0,999999.0), values = cms.vdouble(1.0), uncertainties = cms.vdouble(0.0))
         ))

      self.bTagEffBins2017 = cms.PSet( 
         variables = cms.vstring("hadronFlavour","abs(eta)","pt"),
         bins = cms.VPSet(
            #udsg jets
            cms.PSet(lowBounds = cms.vdouble(-0.5,0.0,0.0), upBounds = cms.vdouble(3.5,2.4,50.0), values = cms.vdouble(0.0455062), uncertainties = cms.vdouble(0.0)),
            cms.PSet(lowBounds = cms.vdouble(-0.5,0.0,50.0), upBounds = cms.vdouble(3.5,2.4,100.0), values = cms.vdouble(0.0248722), uncertainties = cms.vdouble(0.0)),
            cms.PSet(lowBounds = cms.vdouble(-0.5,0.0,100.0), upBounds = cms.vdouble(3.5,2.4,200.0), values = cms.vdouble(0.0265933), uncertainties = cms.vdouble(0.0)),
            cms.PSet(lowBounds = cms.vdouble(-0.5,0.0,200.0), upBounds = cms.vdouble(3.5,2.4,999999.0), values = cms.vdouble(0.062829), uncertainties = cms.vdouble(0.0)),
            #
            cms.PSet(lowBounds = cms.vdouble(-0.5,2.4,0.0), upBounds = cms.vdouble(3.5,10.0,50.0), values = cms.vdouble(0.0), uncertainties = cms.vdouble(0.0)),
            cms.PSet(lowBounds = cms.vdouble(-0.5,2.4,50.0), upBounds = cms.vdouble(3.5,10.0,100.0), values = cms.vdouble(0.0), uncertainties = cms.vdouble(0.0)),
            cms.PSet(lowBounds = cms.vdouble(-0.5,2.4,100.0), upBounds = cms.vdouble(3.5,10.0,200.0), values = cms.vdouble(0.0), uncertainties = cms.vdouble(0.0)),
            cms.PSet(lowBounds = cms.vdouble(-0.5,2.4,200.0), upBounds = cms.vdouble(3.5,10.0,999999.0), values = cms.vdouble(0.0), uncertainties = cms.vdouble(0.0)),
            #c jets
            cms.PSet(lowBounds = cms.vdouble(3.5,0.0,0.0), upBounds = cms.vdouble(4.5,2.4,50.0), values = cms.vdouble(0.226323), uncertainties = cms.vdouble(0.0)),
            cms.PSet(lowBounds = cms.vdouble(3.5,0.0,50.0), upBounds = cms.vdouble(4.5,2.4,100.0), values = cms.vdouble(0.208522), uncertainties = cms.vdouble(0.0)),
            cms.PSet(lowBounds = cms.vdouble(3.5,0.0,100.0), upBounds = cms.vdouble(4.5,2.4,200.0), values = cms.vdouble(0.214934), uncertainties = cms.vdouble(0.0)),
            cms.PSet(lowBounds = cms.vdouble(3.5,0.0,200.0), upBounds = cms.vdouble(4.5,2.4,999999.0), values = cms.vdouble(0.239287),uncertainties = cms.vdouble(0.0)),
            #
            cms.PSet(lowBounds = cms.vdouble(3.5,2.4,0.0), upBounds = cms.vdouble(4.5,10.0,50.0), values = cms.vdouble(0.0), uncertainties = cms.vdouble(0.0)),
            cms.PSet(lowBounds = cms.vdouble(3.5,2.4,50.0), upBounds = cms.vdouble(4.5,10.0,100.0), values = cms.vdouble(0.0), uncertainties = cms.vdouble(0.0)),
            cms.PSet(lowBounds = cms.vdouble(3.5,2.4,100.0), upBounds = cms.vdouble(4.5,10.0,200.0), values = cms.vdouble(0.0), uncertainties = cms.vdouble(0.0)),
            cms.PSet(lowBounds = cms.vdouble(3.5,2.4,200.0), upBounds = cms.vdouble(4.5,10.0,999999.0), values = cms.vdouble(0.0), uncertainties = cms.vdouble(0.0)),
            #b jets
            cms.PSet(lowBounds = cms.vdouble(4.5,0.0,0.0), upBounds = cms.vdouble(5.5,2.4,50.0), values = cms.vdouble(0.702162), uncertainties = cms.vdouble(0.0)),
            cms.PSet(lowBounds = cms.vdouble(4.5,0.0,50.0), upBounds = cms.vdouble(5.5,2.4,100.0), values = cms.vdouble(0.770467), uncertainties = cms.vdouble(0.0)),
            cms.PSet(lowBounds = cms.vdouble(4.5,0.0,100.0), upBounds = cms.vdouble(5.5,2.4,200.0), values = cms.vdouble(0.771944), uncertainties = cms.vdouble(0.0)),
            cms.PSet(lowBounds = cms.vdouble(4.5,0.0,200.0), upBounds = cms.vdouble(5.5,2.4,999999.0), values = cms.vdouble(0.750926), uncertainties = cms.vdouble(0.0)),
            #
            cms.PSet(lowBounds = cms.vdouble(4.5,2.4,0.0), upBounds = cms.vdouble(5.5,10.0,50.0), values = cms.vdouble(1.0), uncertainties = cms.vdouble(0.0)),
            cms.PSet(lowBounds = cms.vdouble(4.5,2.4,50.0), upBounds = cms.vdouble(5.5,10.0,100.0), values = cms.vdouble(1.0), uncertainties = cms.vdouble(0.0)),
            cms.PSet(lowBounds = cms.vdouble(4.5,2.4,100.0), upBounds = cms.vdouble(5.5,10.0,200.0), values = cms.vdouble(1.0), uncertainties = cms.vdouble(0.0)),
            cms.PSet(lowBounds = cms.vdouble(4.5,2.4,200.0), upBounds = cms.vdouble(5.5,10.0,999999.0), values = cms.vdouble(1.0), uncertainties = cms.vdouble(0.0))
         ))

      self.RMSShiftBins = cms.PSet(
         variables = cms.vstring("abs(eta)","pt"),
         bins = cms.VPSet(
            cms.PSet( lowBounds = cms.vdouble(2.5,20.), upBounds = cms.vdouble(2.75,30.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.005 )),
                     cms.PSet( lowBounds = cms.vdouble(2.75,20.), upBounds = cms.vdouble(3.0,30.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.004 )),
            cms.PSet( lowBounds = cms.vdouble(3.0,20.), upBounds = cms.vdouble(4.7,30.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.004 )),
            cms.PSet( lowBounds = cms.vdouble(2.5,30.), upBounds = cms.vdouble(2.75,50.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.004 )),
            cms.PSet( lowBounds = cms.vdouble(2.75,30.), upBounds = cms.vdouble(3.0,50.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.003 )),
            cms.PSet( lowBounds = cms.vdouble(3.0,30.), upBounds = cms.vdouble(4.7,50.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.003 )),
            cms.PSet( lowBounds = cms.vdouble(2.5,50.), upBounds = cms.vdouble(2.75,99999999.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.003 )),
            cms.PSet( lowBounds = cms.vdouble(2.75,50.), upBounds = cms.vdouble(3.0,99999999.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.003 )),
            cms.PSet( lowBounds = cms.vdouble(3.0,50.), upBounds = cms.vdouble(4.7,99999999.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.003 )),
         )
    )

      self.PUJIDShiftBins = cms.PSet(
         variables = cms.vstring("abs(eta)","pt"),
         bins = cms.VPSet(
            # pt 20--30
            cms.PSet( lowBounds = cms.vdouble(0.0 ,20.), upBounds = cms.vdouble(2.5 ,30.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.002 )),
            cms.PSet( lowBounds = cms.vdouble(2.5 ,20.), upBounds = cms.vdouble(2.75,30.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.02 )),
            cms.PSet( lowBounds = cms.vdouble(2.75,20.), upBounds = cms.vdouble(3.0 ,30.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.01 )),
            cms.PSet( lowBounds = cms.vdouble(3.0 ,20.), upBounds = cms.vdouble(5.0 ,30.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.04 )),
            # pt 30--50
            cms.PSet( lowBounds = cms.vdouble(0.0 ,30.), upBounds = cms.vdouble(2.5 ,50.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.005 )),
            cms.PSet( lowBounds = cms.vdouble(2.5 ,30.), upBounds = cms.vdouble(2.75,50.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.01 )),
            cms.PSet( lowBounds = cms.vdouble(2.75,30.), upBounds = cms.vdouble(3.0 ,50.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.06 )),
            cms.PSet( lowBounds = cms.vdouble(3.0 ,30.), upBounds = cms.vdouble(5.0 ,50.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.025 )),
            # pt 50--100
            cms.PSet( lowBounds = cms.vdouble(0.0 ,50.), upBounds = cms.vdouble(2.5 ,100.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.004 )),
            cms.PSet( lowBounds = cms.vdouble(2.5 ,50.), upBounds = cms.vdouble(2.75,100.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.01 )),
            cms.PSet( lowBounds = cms.vdouble(2.75,50.), upBounds = cms.vdouble(3.0 ,100.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.01 )),
            cms.PSet( lowBounds = cms.vdouble(3.0 ,50.), upBounds = cms.vdouble(5.0 ,100.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.01 )),
            # pt 100--inf, not applied
            cms.PSet( lowBounds = cms.vdouble(0.0 ,100.), upBounds = cms.vdouble(2.5 ,9999999.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.00 )),
            cms.PSet( lowBounds = cms.vdouble(2.5 ,100.), upBounds = cms.vdouble(2.75,9999999.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.00 )),
            cms.PSet( lowBounds = cms.vdouble(2.75,100.), upBounds = cms.vdouble(3.0 ,9999999.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.00 )),
            cms.PSet( lowBounds = cms.vdouble(3.0 ,100.), upBounds = cms.vdouble(5.0 ,9999999.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.00 )),    
         )
      )

      self.UnmatchedPUBins = cms.PSet(
         variables = cms.vstring("abs(eta)","pt"),
         bins = cms.VPSet(
            cms.PSet( lowBounds = cms.vdouble(2.50,20.), upBounds = cms.vdouble(2.75,25.), values = cms.vdouble(1.039), uncertainties = cms.vdouble( -0.039 )),
            cms.PSet( lowBounds = cms.vdouble(2.75,20.), upBounds = cms.vdouble(3.00,25.), values = cms.vdouble(1.008), uncertainties = cms.vdouble( -0.008 )),
            cms.PSet( lowBounds = cms.vdouble(3.00,20.), upBounds = cms.vdouble(4.00,25.), values = cms.vdouble(0.954), uncertainties = cms.vdouble( 0.046 )),
            cms.PSet( lowBounds = cms.vdouble(4.00,20.), upBounds = cms.vdouble(4.70,25.), values = cms.vdouble(0.894), uncertainties = cms.vdouble( 0.106 )),
            cms.PSet( lowBounds = cms.vdouble(2.50,25.), upBounds = cms.vdouble(2.75,30.), values = cms.vdouble(0.974), uncertainties = cms.vdouble( 0.026 )),
            cms.PSet( lowBounds = cms.vdouble(2.75,25.), upBounds = cms.vdouble(3.00,30.), values = cms.vdouble(0.891), uncertainties = cms.vdouble( 0.109 )),
            cms.PSet( lowBounds = cms.vdouble(3.00,25.), upBounds = cms.vdouble(4.00,30.), values = cms.vdouble(0.941), uncertainties = cms.vdouble( 0.059 )),
            cms.PSet( lowBounds = cms.vdouble(4.00,25.), upBounds = cms.vdouble(4.70,30.), values = cms.vdouble(0.924), uncertainties = cms.vdouble( 0.076 )),
            cms.PSet( lowBounds = cms.vdouble(2.50,30.), upBounds = cms.vdouble(2.75,50.), values = cms.vdouble(1.068), uncertainties = cms.vdouble( -0.068 )),
            cms.PSet( lowBounds = cms.vdouble(2.75,30.), upBounds = cms.vdouble(3.00,50.), values = cms.vdouble(0.680), uncertainties = cms.vdouble( 0.320 )),
            cms.PSet( lowBounds = cms.vdouble(3.00,30.), upBounds = cms.vdouble(4.00,50.), values = cms.vdouble(0.893), uncertainties = cms.vdouble( 0.107 )),
            cms.PSet( lowBounds = cms.vdouble(4.00,30.), upBounds = cms.vdouble(4.70,50.), values = cms.vdouble(0.877), uncertainties = cms.vdouble( 0.123 ))
         )
    )

   def createJetSystematicsForTag(self, jetInputTag):
      num = jetInputTag.productInstanceLabel
      newName = 'flashggJetSystematics'+num
      bTagger = self.metaConditions['bTagSystematics']['bTagger']
      eta = self.metaConditions['bTagSystematics']['eta'] 
      allJetUncerts = cms.VPSet(cms.PSet( MethodName = cms.string("FlashggJetEnergyCorrector"),
                                        Label = cms.string("JEC"),
                                        NSigmas = cms.vint32(-1,1),
                                        OverallRange = cms.string("abs(eta)<5.0"),
                                        Debug = cms.untracked.bool(False),
                                        ApplyCentralValue = cms.bool(True),
                                        SetupUncertainties = cms.bool(True),
                                        UseTextFile= cms.bool(False),
                                        TextFileName = cms.FileInPath(str(self.metaConditions['flashggJetSystematics']['textFileName'])),
                                        SourceName = cms.string("notused"),
                                        JetCorrectorTag = cms.InputTag("ak4PFCHSL1FastL2L3Corrector")
                                     ),
                              cms.PSet( MethodName = cms.string("FlashggJetSmear"),
                                     Label = cms.string("JER"),
                                        NSigmas = cms.vint32(-1,1),
                                        OverallRange = cms.string("abs(eta)<5.0"),
                                        RandomLabel = cms.string("rnd_g_JER"), # for no-match case
                                        rho = cms.InputTag('fixedGridRhoAll'),
                                        Debug = cms.untracked.bool(False),
                                        ApplyCentralValue = cms.bool(True),
                                        UseTextFiles = cms.bool(False),
                                        TextFileResolution = cms.string("notused"),
                                        TextFileSF = cms.string("notused")
                                     ),
                              cms.PSet( MethodName = cms.string("FlashggJetBTagWeight"),
                                        Label = cms.string("JetBTagCutWeight"),
                                        NSigmas = cms.vint32(-1,1),
                                        OverallRange = cms.string("pt>25.0&&abs(eta)<" + str(eta)),
                                        BinList = getattr(self, self.metaConditions['bTagSystematics']['bTagEffBins']),
                                        #bTag = cms.string(flashggBTag),
                                        bTag = cms.string(str(bTagger)),
                                        bTagCalibrationFile = cms.FileInPath(str(self.metaConditions['bTagSystematics']['bTagCalibrationFile_WPCut_'+ str(bTagger)])),
                                        bDiscriminator = cms.double(self.metaConditions['bTagSystematics']['bDiscriminatorValue_'+ str(bTagger)]),
                                        Debug = cms.untracked.bool(False),
                                        ApplyCentralValue = cms.bool(True)
                                     ),
                              cms.PSet( MethodName = cms.string("FlashggJetBTagReshapeWeight"),
                                        Label = cms.string("JetBTagReshapeWeight"),
                                        NSigmas = cms.vint32(-1,1),
                                        OverallRange = cms.string("pt>25.0&&abs(eta)<" + str(eta)),
                                        BinList = getattr(self, self.metaConditions['bTagSystematics']['bTagEffBins']),
                                        #                                                          bTag = cms.string(flashggBTag),
                                        bTag = cms.string(str(bTagger)), 
                                        bTagCalibrationFile = cms.FileInPath(str(self.metaConditions['bTagSystematics']['bTagCalibrationFile_Reshape_'+ str(bTagger)])),
                                        bTagReshapeSystOption = cms.int32(1),#For changing the source of uncertainty
                                        Debug = cms.untracked.bool(False),
                                        ApplyCentralValue = cms.bool(True)
                                     ),
                              cms.PSet( MethodName = cms.string("FlashggJetPUJIDShift"),
                                     Label = cms.string("PUJIDShift"),
                                        NSigmas = cms.vint32(-1,1),
                                        OverallRange = cms.string("abs(eta)<5.0&&pt>20.0"),
                                        BinList = self.PUJIDShiftBins,
                                        ApplyCentralValue = cms.bool(False),
                                        Debug = cms.untracked.bool(False)
                                     ),
                              cms.PSet( MethodName = cms.string("FlashggJetWeight"),
                                        Label = cms.string("UnmatchedPUWeight"),
                                        NSigmas = cms.vint32(-1,1),
                                        OverallRange = cms.string("abs(eta)>2.5&&abs(eta)<4.7&&pt>20.&&pt<50.&&hasGenMatch==0"),
                                        BinList = self.UnmatchedPUBins,
                                        ApplyCentralValue = cms.bool(False),
                                        Debug = cms.untracked.bool(False),
                                     )
                               )

      ## option to add the granular sources for jet systematics - off by default
      if self.options.doSystematics and self.options.doGranularJEC :
          for sourceName in self.metaConditions['flashggJetSystematics']['listOfSources']:
              allJetUncerts += cms.VPSet( cms.PSet( MethodName = cms.string("FlashggJetEnergyCorrector"),
                                                    Label = cms.string("JEC%s"%str(sourceName)),
                                                    NSigmas = cms.vint32(-1,1),
                                                    OverallRange = cms.string("abs(eta)<5.0"),
                                                    Debug = cms.untracked.bool(False),
                                                    ApplyCentralValue = cms.bool(False), ## these are only systematic variations, not additional corrections
                                                    SetupUncertainties = cms.bool(False),
                                                    UseTextFile = cms.bool(True), ## these are only available in txt files, not in global tag
                                                    TextFileName = cms.FileInPath(str(self.metaConditions['flashggJetSystematics']['textFileName'])),
                                                    SourceName = cms.string(str(sourceName)),
                                                    JetCorrectorTag = cms.InputTag("ak4PFCHSL1FastL2L3Corrector")
                                                  ) 
                                        )

      if self.metaConditions['flashggJetSystematics']['doHEMuncertainty'] and self.options.doSystematics:
          allJetUncerts += cms.VPSet( cms.PSet( MethodName = cms.string("FlashggJetHEMCorrector"),
                                                Label = cms.string("JetHEM"),
                                                NSigmas = cms.vint32(-1,1),
                                                OverallRange = cms.string("abs(eta)<5.0"),
                                                Debug = cms.untracked.bool(False),
                                                ApplyCentralValue = cms.bool(False)
                                              ) 
                                    )
          
      setattr(self.process, newName,
              cms.EDProducer('FlashggJetSystematicProducer',
                             src = jetInputTag,
                             SystMethods2D = cms.VPSet(),
                             SystMethods = allJetUncerts
                            )
             )

      return (getattr(self.process,newName),cms.InputTag(newName))
  
   def createJetSystematics(self, replaceTagList):
      self.process.load("JetMETCorrections.Configuration.JetCorrectors_cff")
      self.process.jetCorrectorChain = cms.Sequence(self.process.ak4PFCHSL1FastL2L3CorrectorChain)
      self.process.jetSystematicsSequence = cms.Sequence(self.process.jetCorrectorChain)
      systematicsInputList = cms.VInputTag()
      for jetInputTag in replaceTagList:
         module,tag = self.createJetSystematicsForTag(jetInputTag)
         self.process.jetSystematicsSequence += module
         systematicsInputList.append(tag)
         self.createJECESource()  
         #  createJERESource(self.process)  
      return systematicsInputList

   def createJECESource(self):
      if 'JetCorrectorParametersCollection_version' not in self.metaConditions.keys():
         return
      
      # self.process.load("CondCore.DBCommon.CondDBCommon_cfi")
      # self.process.load("CondCore.DBCommon.CondDBSetup_cfi")
      self.process.load("CondCore.CondDB.CondDB_cfi")
      self.process.jec = cms.ESSource("PoolDBESSource",
                                      DBParameters = cms.PSet(
                                         messageLevel = cms.untracked.int32(0)
                                      ),
                                      timetype = cms.string('runnumber'),
                                      toGet = cms.VPSet(cms.PSet(
                                         record = cms.string('JetCorrectionsRecord'),
                                         tag    = cms.string(
                                            str(self.metaConditions['JetCorrectorParametersCollection_version']["data" if self.options.processType=="data" else "MC"])),   
                                         label  = cms.untracked.string("AK4PFchs")
                                      )),
                                      connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
                                   )                               
      self.process.es_prefer_jec = cms.ESPrefer('PoolDBESSource', 'jec')

   def createJERESource(self):
      if 'JR_PtResolution_version' not in self.metaConditions.keys() or 'JR_SF_version' not in self.metaConditions.keys():
         return
      
      self.process.load('Configuration.StandardSequences.Services_cff')
      self.process.load("JetMETCorrections.Modules.JetResolutionESProducer_cfi")
      # from CondCore.DBCommon.CondDBSetup_cfi import CondDBSetup
      from CondCore.CondDB.CondDB_cfi import CondDBSetup
         
      self.process.jer = cms.ESSource("PoolDBESSource",
                                      CondDBSetup,
                                      toGet = cms.VPSet(
                                         # Resolution
                                         cms.PSet(
                                            record = cms.string('JetResolutionRcd'),
                                            tag    = cms.string(str(self.metaConditions['JR_PtResolution_version'])),       
                                            label  = cms.untracked.string('AK4PFchs_pt')
                                         ),                                  
                                         # Scale factors
                                         cms.PSet(
                                            record = cms.string('JetResolutionScaleFactorRcd'),
                                            tag    = cms.string(str(self.metaConditions['JR_SF_version'])),       
                                            label  = cms.untracked.string('AK4PFchs')
                                         ),
                                      ),
                                      connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
      )
   
      self.process.es_prefer_jer = cms.ESPrefer('PoolDBESSource', 'jer')
      
