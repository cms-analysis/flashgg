import FWCore.ParameterSet.Config as cms

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

def createJetSystematicsForTag(process,jetInputTag):
  num = jetInputTag.productInstanceLabel
  newName = 'flashggJetSystematics'+num
  setattr(process,newName,
          cms.EDProducer('FlashggJetSystematicProducer',
                         src = jetInputTag,
                         DoCentralJEC = cms.bool(False),
                         JECLabel = cms.string("UNDEFINED"),
                         SystMethods2D = cms.VPSet(),
                         SystMethods = cms.VPSet(cms.PSet( MethodName = cms.string("FlashggJetEnergyCorrector"),
                                                           Label = cms.string("JEC"),
                                                           NSigmas = cms.vint32(-1,1),
                                                           OverallRange = cms.string("abs(eta)<5.0"),
                                                           Debug = cms.untracked.bool(False),
                                                           ApplyCentralValue = cms.bool(True)
                                                           ),
                                                 cms.PSet( MethodName = cms.string("FlashggJetSmearConstant"),
                                                           Label = cms.string("JER"),
                                                           NSigmas = cms.vint32(-1,1),
                                                           OverallRange = cms.string("abs(eta)<5.0"),
                                                           RandomLabel = cms.string("rnd_g_JER"), # for no-match case
                                                           BinList = smearBins,
                                                           Debug = cms.untracked.bool(False),
                                                           ApplyCentralValue = cms.bool(True)
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
                                                           )
                                                 )
                         
                         )
          )
#  setattr(process.RandomNumberGeneratorService,newName,cms.PSet(initialSeed = cms.untracked.uint32(int(num)))) 
  # e.g. process.RandomNumberGeneratorService.flashggJetSystematics3 = cms.PSet(initialSeed = cms.untracked.uint32(3))
  return (getattr(process,newName),cms.InputTag(newName))
  
def createJetSystematics(process,replaceTagList):
  process.jetSystematicsSequence = cms.Sequence()
  systematicsInputList = cms.VInputTag()
  for jetInputTag in replaceTagList:
    module,tag = createJetSystematicsForTag(process,jetInputTag)
    process.jetSystematicsSequence += module
    systematicsInputList.append(tag)
  return systematicsInputList
          

