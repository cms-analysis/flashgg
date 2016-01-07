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

def createJetSystematicsForTag(process,jetInputTag):
  num = jetInputTag.productInstanceLabel
  newName = 'flashggJetSystematics'+num
  setattr(process,newName,
          cms.EDProducer('FlashggJetSystematicProducer',
                         src = jetInputTag,
                         DoCentralJEC = cms.bool(False),
                         SystMethods2D = cms.VPSet(),
                         SystMethods = cms.VPSet(cms.PSet( MethodName = cms.string("FlashggJetEnergyCorrector"),
                                                           Label = cms.string("JEC"),
                                                           NSigmas = cms.vint32(-1,1),
                                                           OverallRange = cms.string("abs(eta)<5.0"),
                                                           Debug = cms.untracked.bool(True)
                                                           ),
                                                 cms.PSet( MethodName = cms.string("FlashggJetSmearConstant"),
                                                           Label = cms.string("JER"),
                                                           NSigmas = cms.vint32(-1,1),
                                                           OverallRange = cms.string("abs(eta)<5.0"),
                                                           RandomLabel = cms.string("rnd_g_JER"), # for no-match case
                                                           BinList = smearBins,
                                                           Debug = cms.untracked.bool(True),
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
          

