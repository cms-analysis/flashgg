import FWCore.ParameterSet.Config as cms

def setModules(process, options):

    #process.sampleInfo = cms.EDProducer("tnp::FlashggSampleInfoTree")

    from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel
    process.hltFilter = hltHighLevel.clone()
    process.hltFilter.throw = cms.bool(False)
    process.hltFilter.HLTPaths = options['TnPPATHS']
    
    from EgammaAnalysis.TnPTreeProducer.pileupConfiguration_cfi import pileupProducer
    process.pileupReweightingProducer = pileupProducer.clone()    

    ###################################################################
    ## ELECTRON MODULES
    ###################################################################
    
    process.photonFromDiPhotons = cms.EDProducer("FlashggPhotonFromDiPhotonProducer",
                                                 src = cms.InputTag(options['DIPHOTON_COLL']),
                                                 cut = cms.string(options['PHOTON_CUTS']),
                                                 leadingPreselection = cms.string(options['LEADING_PRESELECTION']),
                                                 subleadingPreselection = cms.string(options['SUBLEADING_PRESELECTION']),
                                                 vertexSelection = cms.int32(-1), # -1 means take the chosen vertex, otherwise use the index to select 2it
                                                 diPhotonMVATag = cms.InputTag("flashggDiPhotonMVA"),
                                                 diphotonMVAThreshold = cms.double(-0.6) #Previous value was -1.0
                                                 )

    process.goodPhotonTagL1 = cms.EDProducer("FlashggPhotonL1CandProducer",
                                             inputs = cms.InputTag("goodPhotonTags"),
                                             isoObjects = cms.InputTag("goodPhotonTags"),
                                             nonIsoObjects = cms.InputTag(""),
                                             minET = cms.double(25),
                                             dRmatch = cms.double(0.2),
                                             isolatedOnly = cms.bool(False)
                                             )
    
    process.goodPhotonTags = cms.EDFilter("FlashggPhotonRefSelector",
                                          src = cms.InputTag("photonFromDiPhotons"),
                                          cut = cms.string(options['PHOTON_TAG_CUTS'])
                                          )
    
    process.goodPhotonProbes = cms.EDFilter("FlashggPhotonRefSelector",
                                            src = cms.InputTag("photonFromDiPhotons"),
                                            cut = cms.string(options['PHOTON_CUTS'])
                                            )
    
    ###################################################################
    
    process.goodPhotonProbesPreselection = cms.EDProducer("FlashggPhotonSelectorByValueMap",
                                                          input     = cms.InputTag("goodPhotonProbes"),
                                                          cut       = cms.string(options['PHOTON_CUTS']),
                                                          selection = cms.InputTag("photonFromDiPhotons:preselection"),
                                                          id_cut    = cms.bool(True)
                                                          )
    
    process.goodPhotonProbesIDMVA = cms.EDProducer("FlashggPhotonSelectorByDoubleValueMap",
                                                   input     = cms.InputTag("goodPhotonProbes"),
                                                   cut       = cms.string(options['PHOTON_CUTS']),
                                                   selection = cms.InputTag("photonFromDiPhotons:idmva"),
                                                   id_cut    = cms.double(-0.9)
                                                   )
    
    process.goodPhotonTagsIDMVA = cms.EDProducer("FlashggPhotonSelectorByDoubleValueMap",
                                                 input     = cms.InputTag("goodPhotonTags"),
                                                 cut       = cms.string(options['PHOTON_CUTS']),
                                                 selection = cms.InputTag("photonFromDiPhotons:idmva"),
                                                 id_cut    = cms.double(-0.6)
                                                 )
    
    ###################################################################

    process.goodPhotonsTagHLT = cms.EDProducer("FlashggPhotonTriggerCandProducer",
                                               filterNames = options['TnPHLTTagFilters'],
                                               inputs      = cms.InputTag("goodPhotonTagsIDMVA"),
                                               bits        = cms.InputTag('TriggerResults::HLT'),
                                               objects     = cms.InputTag('slimmedPatTrigger'),
                                               dR          = cms.double(0.3),
                                               isAND       = cms.bool(True)
                                               )
    
    process.goodPhotonsProbeHLT = cms.EDProducer("FlashggPhotonTriggerCandProducer",
                                                 filterNames = options['TnPHLTProbeFilters'],
                                                 inputs      = cms.InputTag("goodPhotonProbes"),
                                                 bits        = cms.InputTag('TriggerResults::HLT'),
                                                 objects     = cms.InputTag('slimmedPatTrigger'),
                                                 dR          = cms.double(0.3),
                                                 isAND       = cms.bool(True)
                                                 )
    
    process.goodPhotonProbesL1 = cms.EDProducer("FlashggPhotonL1Stage2CandProducer",
                                                inputs = cms.InputTag("goodPhotonProbes"),
                                                objects = cms.InputTag("caloStage2Digis:EGamma"),
                                                minET = cms.double(40),
                                                dRmatch = cms.double(0.2),
                                                dRmatchEE = cms.double(0.2),
                                                isolatedOnly = cms.bool(False)
                                                )
    
    ###################################################################
    ## PHOTON ISOLATION
    ###################################################################
    process.load("RecoEgamma/PhotonIdentification/PhotonIDValueMapProducer_cfi")

    process.tagTightRECO = cms.EDProducer("CandViewShallowCloneCombiner",
                                      decay = cms.string("goodPhotonsTagHLT goodPhotonsProbeHLT"), 
                                      checkCharge = cms.bool(False),
                                      cut = cms.string("40<mass<130"),
                                      )

    
