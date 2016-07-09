import FWCore.ParameterSet.Config as cms
from os import environ

# https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetResolution#JER_Scaling_factors_and_Uncertai
# Data/MC SFs
smearBins = cms.PSet(
    variables = cms.vstring("abs(eta)"),
    bins = cms.VPSet(
                     cms.PSet( lowBounds = cms.vdouble(0.0), upBounds = cms.vdouble(0.8),
                               values = cms.vdouble( 1.061 ), uncertainties = cms.vdouble( 0.023 ) ),
                     cms.PSet( lowBounds = cms.vdouble(0.8), upBounds = cms.vdouble(1.3),
                               values = cms.vdouble( 1.088 ), uncertainties = cms.vdouble( 0.029 ) ),
                     cms.PSet( lowBounds = cms.vdouble(1.3), upBounds = cms.vdouble(1.9),
                              values = cms.vdouble( 1.106 ), uncertainties = cms.vdouble( 0.030 ) ),
                     cms.PSet( lowBounds = cms.vdouble(1.9), upBounds = cms.vdouble(2.5),
                              values = cms.vdouble( 1.126 ), uncertainties = cms.vdouble( 0.094 ) ),
                     cms.PSet( lowBounds = cms.vdouble(2.5), upBounds = cms.vdouble(3.0),
                              values = cms.vdouble( 1.343 ), uncertainties = cms.vdouble( 0.123 ) ),
                     cms.PSet( lowBounds = cms.vdouble(3.0), upBounds = cms.vdouble(3.2),
                              values = cms.vdouble( 1.303 ), uncertainties = cms.vdouble( 0.111 ) ),
                     cms.PSet( lowBounds = cms.vdouble(3.2), upBounds = cms.vdouble(5.0),
                              values = cms.vdouble( 1.320 ), uncertainties = cms.vdouble( 0.286 ) )
                    ))

#https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideBTagMCTools#Hadron_parton_based_jet_flavour
#B Tag MC efficiencies
bTagEffBins = cms.PSet(
    variables = cms.vstring("hadronFlavour","abs(eta)","pt"),
    bins = cms.VPSet(
                     #udsg jets
                     cms.PSet(lowBounds = cms.vdouble(-0.5,0.0,0.0), upBounds = cms.vdouble(3.5,2.4,50.0), values = cms.vdouble(0.04360846), uncertainties = cms.vdouble(0.0)),
                     cms.PSet(lowBounds = cms.vdouble(-0.5,0.0,50.0), upBounds = cms.vdouble(3.5,2.4,100.0), values = cms.vdouble(0.02723417), uncertainties = cms.vdouble(0.0)),
                     cms.PSet(lowBounds = cms.vdouble(-0.5,0.0,100.0), upBounds = cms.vdouble(3.5,2.4,200.0), values = cms.vdouble(0.02553537), uncertainties = cms.vdouble(0.0)),
                     cms.PSet(lowBounds = cms.vdouble(-0.5,0.0,200.0), upBounds = cms.vdouble(3.5,2.4,999999.0), values = cms.vdouble(0.03050519), uncertainties = cms.vdouble(0.0)),
                     #
                     cms.PSet(lowBounds = cms.vdouble(-0.5,2.4,0.0), upBounds = cms.vdouble(3.5,10.0,50.0), values = cms.vdouble(0.0), uncertainties = cms.vdouble(0.0)),
                     cms.PSet(lowBounds = cms.vdouble(-0.5,2.4,50.0), upBounds = cms.vdouble(3.5,10.0,100.0), values = cms.vdouble(0.0), uncertainties = cms.vdouble(0.0)),
                     cms.PSet(lowBounds = cms.vdouble(-0.5,2.4,100.0), upBounds = cms.vdouble(3.5,10.0,200.0), values = cms.vdouble(0.0), uncertainties = cms.vdouble(0.0)),
                     cms.PSet(lowBounds = cms.vdouble(-0.5,2.4,200.0), upBounds = cms.vdouble(3.5,10.0,999999.0), values = cms.vdouble(0.0), uncertainties = cms.vdouble(0.0)),
                     #c jets
                     cms.PSet(lowBounds = cms.vdouble(3.5,0.0,0.0), upBounds = cms.vdouble(4.5,2.4,50.0), values = cms.vdouble(0.1269147), uncertainties = cms.vdouble(0.0)),
                     cms.PSet(lowBounds = cms.vdouble(3.5,0.0,50.0), upBounds = cms.vdouble(4.5,2.4,100.0), values = cms.vdouble(0.1301700), uncertainties = cms.vdouble(0.0)),
                     cms.PSet(lowBounds = cms.vdouble(3.5,0.0,100.0), upBounds = cms.vdouble(4.5,2.4,200.0), values = cms.vdouble(0.1364042), uncertainties = cms.vdouble(0.0)),
                     cms.PSet(lowBounds = cms.vdouble(3.5,0.0,200.0), upBounds = cms.vdouble(4.5,2.4,999999.0), values = cms.vdouble(0.1647941), uncertainties = cms.vdouble(0.0)),
                     #
                     cms.PSet(lowBounds = cms.vdouble(3.5,2.4,0.0), upBounds = cms.vdouble(4.5,10.0,50.0), values = cms.vdouble(0.0), uncertainties = cms.vdouble(0.0)),
                     cms.PSet(lowBounds = cms.vdouble(3.5,2.4,50.0), upBounds = cms.vdouble(4.5,10.0,100.0), values = cms.vdouble(0.0), uncertainties = cms.vdouble(0.0)),
                     cms.PSet(lowBounds = cms.vdouble(3.5,2.4,100.0), upBounds = cms.vdouble(4.5,10.0,200.0), values = cms.vdouble(0.0), uncertainties = cms.vdouble(0.0)),
                     cms.PSet(lowBounds = cms.vdouble(3.5,2.4,200.0), upBounds = cms.vdouble(4.5,10.0,999999.0), values = cms.vdouble(0.0), uncertainties = cms.vdouble(0.0)),
                     #b jets
                     cms.PSet(lowBounds = cms.vdouble(4.5,0.0,0.0), upBounds = cms.vdouble(5.5,2.4,50.0), values = cms.vdouble(0.5500933), uncertainties = cms.vdouble(0.0)),
                     cms.PSet(lowBounds = cms.vdouble(4.5,0.0,50.0), upBounds = cms.vdouble(5.5,2.4,100.0), values = cms.vdouble(0.6088235), uncertainties = cms.vdouble(0.0)),
                     cms.PSet(lowBounds = cms.vdouble(4.5,0.0,100.0), upBounds = cms.vdouble(5.5,2.4,200.0), values = cms.vdouble(0.6348762), uncertainties = cms.vdouble(0.0)),
                     cms.PSet(lowBounds = cms.vdouble(4.5,0.0,200.0), upBounds = cms.vdouble(5.5,2.4,999999.0), values = cms.vdouble(0.5543137), uncertainties = cms.vdouble(0.0)),
                     #
                     cms.PSet(lowBounds = cms.vdouble(4.5,2.4,0.0), upBounds = cms.vdouble(5.5,10.0,50.0), values = cms.vdouble(1.0), uncertainties = cms.vdouble(0.0)),
                     cms.PSet(lowBounds = cms.vdouble(4.5,2.4,50.0), upBounds = cms.vdouble(5.5,10.0,100.0), values = cms.vdouble(1.0), uncertainties = cms.vdouble(0.0)),
                     cms.PSet(lowBounds = cms.vdouble(4.5,2.4,100.0), upBounds = cms.vdouble(5.5,10.0,200.0), values = cms.vdouble(1.0), uncertainties = cms.vdouble(0.0)),
                     cms.PSet(lowBounds = cms.vdouble(4.5,2.4,200.0), upBounds = cms.vdouble(5.5,10.0,999999.0), values = cms.vdouble(1.0), uncertainties = cms.vdouble(0.0))
                  ))   

bDiscriminator74X = cms.double(0.890)
bDiscriminator76X = cms.double(0.800)
from flashgg.MicroAOD.flashggJets_cfi import flashggBTag

RMSShiftBins = cms.PSet(
    variables = cms.vstring("abs(eta)","pt"),
    bins = cms.VPSet(
                     cms.PSet( lowBounds = cms.vdouble(2.5,20.), upBounds = cms.vdouble(3.0,25.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.006 )),
                     cms.PSet( lowBounds = cms.vdouble(3.0,20.), upBounds = cms.vdouble(3.5,25.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.010 )),
                     cms.PSet( lowBounds = cms.vdouble(3.5,20.), upBounds = cms.vdouble(4.0,25.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.006 )),
                     cms.PSet( lowBounds = cms.vdouble(4.0,20.), upBounds = cms.vdouble(4.7,25.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.008 )),
                     cms.PSet( lowBounds = cms.vdouble(2.5,25.), upBounds = cms.vdouble(3.0,30.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.005 )),
                     cms.PSet( lowBounds = cms.vdouble(3.0,25.), upBounds = cms.vdouble(3.5,30.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.006 )),
                     cms.PSet( lowBounds = cms.vdouble(3.5,25.), upBounds = cms.vdouble(4.0,30.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.002 )),
                     cms.PSet( lowBounds = cms.vdouble(4.0,25.), upBounds = cms.vdouble(4.7,30.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.005 )),
                     cms.PSet( lowBounds = cms.vdouble(2.5,30.), upBounds = cms.vdouble(3.0,40.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.002 )),
                     cms.PSet( lowBounds = cms.vdouble(3.0,30.), upBounds = cms.vdouble(3.5,40.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.004 )),
                     cms.PSet( lowBounds = cms.vdouble(3.5,30.), upBounds = cms.vdouble(4.0,40.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.002 )),
                     cms.PSet( lowBounds = cms.vdouble(4.0,30.), upBounds = cms.vdouble(4.7,40.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.002 )),
                     cms.PSet( lowBounds = cms.vdouble(2.5,40.), upBounds = cms.vdouble(3.0,50.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.002 )),
                     cms.PSet( lowBounds = cms.vdouble(3.0,40.), upBounds = cms.vdouble(3.5,50.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.004 )),
                     cms.PSet( lowBounds = cms.vdouble(3.5,40.), upBounds = cms.vdouble(4.0,50.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.002 )),
                     cms.PSet( lowBounds = cms.vdouble(4.0,40.), upBounds = cms.vdouble(4.7,50.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.002 )),
                     cms.PSet( lowBounds = cms.vdouble(2.5,50.), upBounds = cms.vdouble(3.0,9999999.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.002 )),
                     cms.PSet( lowBounds = cms.vdouble(3.0,50.), upBounds = cms.vdouble(3.5,9999999.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.002 )),
                     cms.PSet( lowBounds = cms.vdouble(3.5,50.), upBounds = cms.vdouble(4.0,9999999.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.002 )),
                     cms.PSet( lowBounds = cms.vdouble(4.0,50.), upBounds = cms.vdouble(4.7,9999999.), values = cms.vdouble(0.0), uncertainties = cms.vdouble( 0.002 )),
                     )
    )

def createJetSystematicsForTag(process,jetInputTag):
  num = jetInputTag.productInstanceLabel
  newName = 'flashggJetSystematics'+num
  setattr(process,newName,
          cms.EDProducer('FlashggJetSystematicProducer',
                         src = jetInputTag,
                         SystMethods2D = cms.VPSet(),
                         SystMethods = cms.VPSet(cms.PSet( MethodName = cms.string("FlashggJetEnergyCorrector"),
                                                           Label = cms.string("JEC"),
                                                           NSigmas = cms.vint32(-1,1),
                                                           OverallRange = cms.string("abs(eta)<5.0"),
                                                           Debug = cms.untracked.bool(False),
                                                           ApplyCentralValue = cms.bool(True),
                                                           SetupUncertainties = cms.bool(True),
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
                                                           UseTextFiles = cms.bool(True),
                                                           TextFileResolution = cms.string("%s/src/flashgg/Systematics/data/JER/Spring16_25nsV6_MC_PtResolution_AK4PFchs.txt" % environ['CMSSW_BASE']),
                                                           TextFileSF = cms.string("%s/src/flashgg/Systematics/data/JER/Spring16_25nsV6_MC_SF_AK4PFchs.txt" % environ['CMSSW_BASE'])
                                                           ),
                                                 cms.PSet( MethodName = cms.string("FlashggJetBTagWeight"),
                                                           Label = cms.string("JetBTagWeight"),
                                                           NSigmas = cms.vint32(-1,1),
                                                           OverallRange = cms.string("pt>25.0&&abs(eta)<2.4"),
                                                           BinList = bTagEffBins,
						 	   bTag = cms.string(flashggBTag),
						 	   bDiscriminator = bDiscriminator76X, #Medium working point for CSV B tagger, for CMSSW74X use: bDiscriminator74X
							   Debug = cms.untracked.bool(False),
                                                           ApplyCentralValue = cms.bool(True)
                                                           ),
                                                 cms.PSet( MethodName = cms.string("FlashggJetRMSShift"),
                                                           Label = cms.string("RMSShift"),
                                                           NSigmas = cms.vint32(-1,1),
                                                           OverallRange = cms.string("abs(eta)>2.5&&abs(eta)<4.7&&pt>20."),
                                                           BinList  = RMSShiftBins,
                                                           ApplyCentralValue = cms.bool(False),
                                                           Debug = cms.untracked.bool(False)
                                                           )
                                                 )
                         
                         )
          )
#  setattr(process.RandomNumberGeneratorService,newName,cms.PSet(initialSeed = cms.untracked.uint32(int(num)))) 
  # e.g. process.RandomNumberGeneratorService.flashggJetSystematics3 = cms.PSet(initialSeed = cms.untracked.uint32(3))
  return (getattr(process,newName),cms.InputTag(newName))
  
def createJetSystematics(process,replaceTagList):
  process.load("JetMETCorrections.Configuration.JetCorrectors_cff")
  process.jetCorrectorChain = cms.Sequence(process.ak4PFCHSL1FastL2L3CorrectorChain)
  process.jetSystematicsSequence = cms.Sequence(process.jetCorrectorChain)
  systematicsInputList = cms.VInputTag()
  for jetInputTag in replaceTagList:
    module,tag = createJetSystematicsForTag(process,jetInputTag)
    process.jetSystematicsSequence += module
    systematicsInputList.append(tag)
  createJECESource(process)  
#  createJERESource(process)  
  return systematicsInputList

def createJECESource(process):
    datadir = "%s/src/flashgg/Systematics/data/JEC" % environ['CMSSW_BASE']
    print "WARNING: we are reading JEC from %s so GRID jobs might not work" % datadir
    process.load("CondCore.DBCommon.CondDBCommon_cfi")
    process.load("CondCore.DBCommon.CondDBSetup_cfi")
    process.jec = cms.ESSource("PoolDBESSource",
                               DBParameters = cms.PSet(
        messageLevel = cms.untracked.int32(0)
        ),
                               timetype = cms.string('runnumber'),
                               toGet = cms.VPSet(cms.PSet(
          record = cms.string('JetCorrectionsRecord'),
          tag    = cms.string('JetCorrectorParametersCollection_Spring16_25nsV6_MC_AK4PFchs'),
          label  = cms.untracked.string("AK4PFchs")
          )),
                               connect = cms.string('sqlite_file:%s/Spring16_25nsV6_MC.db' % datadir)
                               )                               
    process.es_prefer_jec = cms.ESPrefer('PoolDBESSource', 'jec')

def createJERESource(process):
    datadir = "%s/src/flashgg/Systematics/data/JER" % environ['CMSSW_BASE']
    print "WARNING: we are reading JER from %s so GRID jobs might not work" % datadir
    process.load('Configuration.StandardSequences.Services_cff')
    process.load("JetMETCorrections.Modules.JetResolutionESProducer_cfi")
    from CondCore.DBCommon.CondDBSetup_cfi import CondDBSetup

    process.jer = cms.ESSource("PoolDBESSource",
                               CondDBSetup,
                               toGet = cms.VPSet(
        # Resolution
        cms.PSet(
          record = cms.string('JetResolutionRcd'),
          tag    = cms.string('JR_Spring16_25nsV6_MC_PtResolution_AK4PFchs'),
          label  = cms.untracked.string('AK4PFchs_pt')
          ),
        
        # Scale factors
        cms.PSet(
          record = cms.string('JetResolutionScaleFactorRcd'),
          tag    = cms.string('JR_Spring16_25nsV6_MC_SF_AK4PFchs'),
          label  = cms.untracked.string('AK4PFchs')
          ),
        ),
# [2016-02-21 14:50:14,703] INFO: Connecting to Systematics/data/JER/Summer15_25nsV6_MC.db [sqlite:///Systematics/data/JER/Summer15_25nsV6_MC.db]
# JR_Summer15_25nsV6_MC_SF_AK4PFchs             Run       JME::JetResolutionObject  any              -1             2016-02-05 20:59:34.061327  New Tag      
# JR_Summer15_25nsV6_MC_PtResolution_AK4PFchs   Run       JME::JetResolutionObject  any              -1             2016-02-05 20:59:34.064554  New Tag      
                               connect = cms.string('sqlite_file:%s/Spring16_25nsV6_MC.db' % datadir)

                               )
    process.es_prefer_jer = cms.ESPrefer('PoolDBESSource', 'jer')

