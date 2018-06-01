import FWCore.ParameterSet.Config as cms

def setModules(process, options):

    #process.sampleInfo = cms.EDProducer("tnp::FlashggSampleInfoTree",
    #                                    )

    from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel
    process.hltFilter = hltHighLevel.clone()
    process.hltFilter.throw = cms.bool(False)
    process.hltFilter.HLTPaths = options['TnPPATHS']
    
    from EgammaAnalysis.TnPTreeProducer.pileupConfiguration_cfi import pileupProducer
    process.pileupReweightingProducer = pileupProducer.clone()    

    ###################################################################
    ## ELECTRON MODULES
    ###################################################################
    

    #produces photons for tag+probe                                                                                                                                                       
    process.photonFromDiPhotons = cms.EDProducer("FlashggPhotonFromDiPhotonProducer",
                                                 src = cms.InputTag(options['DIPHOTON_COLL']),
                                                 cut = cms.string(options['PHOTON_CUTS']),
                                                 leadingPreselection = cms.string(options['LEADING_PRESELECTION']),
                                                 subleadingPreselection = cms.string(options['SUBLEADING_PRESELECTION']),
                                                 vertexSelection = cms.int32(-1), # -1 means take the chosen vertex, otherwise use the index to select 2it                                
                                                 #diPhotonMVATag = cms.InputTag("flashggUpdatedIdMVADiPhotons"),                                                                          
                                                 diPhotonMVATag = cms.InputTag("flashggDiPhotonMVA"),
                                                 diphotonMVAThreshold = cms.double(-0.6)
                                                 )

    #produces tag collection from diphotons                                                                                                                                               
    process.goodPhotonTags = cms.EDFilter("FlashggPhotonRefSelector",
                                          src = cms.InputTag("photonFromDiPhotons"),
                                          cut = cms.string(options['PHOTON_TAG_CUTS'])
                                          )


    #produces probe collection from diphotons                                                                                                                                             
    process.goodPhotonProbes = cms.EDFilter("FlashggPhotonRefSelector",
                                            src = cms.InputTag("photonFromDiPhotons"),
                                            cut = cms.string(options['PHOTON_PROBE_CUTS'])
                                            )

    ###################################################################                                                                                                                   
    
#IDMVA for probes                                                                                                                                                                     
    process.goodPhotonProbesIDMVA = cms.EDProducer("FlashggPhotonSelectorByDoubleValueMap",
                                                   input     = cms.InputTag("goodPhotonProbes"),
                                                   cut       = cms.string(options['PHOTON_PROBE_CUTS']),
                                                   selection = cms.InputTag("photonFromDiPhotons:idmva"),
                                                   id_cut    = cms.double(-0.9)
                                                   )
    process.goodPhotonsProbeTnPprobe = cms.EDProducer("FlashggPhotonTriggerCandProducer",
                                                      filterNames = options['TnPHLTProbeFilters'],
                                                      inputs      = cms.InputTag("goodPhotonProbesIDMVA"),
                                                      bits        = cms.InputTag(options['TRIGGER_RES']),
                                                      objects     = cms.InputTag(options['PAT_TRIG']),
                                                      dR          = cms.double(0.3),
                                                      isAND       = cms.bool(False)
                                                      )
    #numerator for Preselection                                                                                                                                                           
    process.goodPhotonProbesPreselection = cms.EDProducer("FlashggPhotonSelectorByValueMap",
                                                          input     = cms.InputTag("goodPhotonsProbeTnPprobe"),
                                                          cut       = cms.string(options['PHOTON_PROBE_CUTS']),
                                                          selection = cms.InputTag("photonFromDiPhotons:preselection"),
                                                          id_cut    = cms.bool(True)
                                                          )
    
    #probes match to l1                                                                                                                                                                   
    process.goodPhotonProbesL1 = cms.EDProducer("FlashggPhotonL1CandProducer",
    #process.goodPhotonProbesL1 = cms.EDProducer("FlashggPhotonL1Stage2CandProducer",
                                                inputs = cms.InputTag("goodPhotonProbesPreselection"),
                                                #isoObjects = cms.InputTag("l1extraParticles:Isolated"),
                                                #nonIsoObjects = cms.InputTag("l1extraParticles:NonIsolated"),
                                                #objects = cms.InputTag("hltCaloStage2Digis:EGamma"),
                                                objects = cms.InputTag("caloStage2Digis:EGamma"),
                                                #minET = cms.double(15), #lead eff only
                                                minET = cms.double(10), #sublead eff only
                                                dRmatch = cms.double(0.2), #defined to match L1 online matching to hlt (0.261)
                                                #dRmatchEE = cms.double(0.2), #defined to match L1 online matching to hlt (should probably be tightened for stage2)
                                                isolatedOnly = cms.bool(False)
                                                )

    #numerator passing HLT requiring preselection and L1  
    process.goodPhotonsProbeHLT = cms.EDProducer("FlashggPhotonTriggerCandProducer",
                                                    filterNames = options['HLTFILTERTOMEASURE'],
                                                    inputs      = cms.InputTag("goodPhotonProbesPreselection"),
                                                    bits        = cms.InputTag(options['TRIGGER_RES']),
                                                    objects     = cms.InputTag(options['PAT_TRIG']),
                                                    dR          = cms.double(0.3),
                                                    isAND       = cms.bool(False)
                                                    )
    
    
    ###################################################################                                                                                                                   
    #IDMVA for tags                                                                                                                                                                       
    process.goodPhotonTagsIDMVA = cms.EDProducer("FlashggPhotonSelectorByDoubleValueMap",
                                                 input     = cms.InputTag("goodPhotonTags"),
                                                 cut       = cms.string(options['PHOTON_TAG_CUTS']),
                                                 selection = cms.InputTag("photonFromDiPhotons:idmva"),
                                                 id_cut    = cms.double(-0.5)
                                                 )
    #preselection for tags                                                                                                                                                                
    process.goodPhotonTagsPreselection = cms.EDProducer("FlashggPhotonSelectorByValueMap",
                                                          input     = cms.InputTag("goodPhotonTagsIDMVA"),
                                                          cut       = cms.string(options['PHOTON_TAG_CUTS']),
                                                          selection = cms.InputTag("photonFromDiPhotons:preselection"),
                                                          id_cut    = cms.bool(True)
                                                          )


    #good match to lead HggFilter tag photons for denominator PRESELECTED                                    
    process.goodPhotonsTagLeadMatch = cms.EDProducer("FlashggPhotonTriggerCandProducer",
                                                        filterNames = options['TagLeadMatchFilters'],
                                                        inputs      = cms.InputTag("goodPhotonTagsPreselection"),
                                                        bits        = cms.InputTag(options['TRIGGER_RES']),
                                                        objects     = cms.InputTag(options['PAT_TRIG']),
                                                        dR          = cms.double(0.3),
                                                        isAND       = cms.bool(False)
                                                        )
    #good preselected tag photons for denominator                                                                                                                                         
    process.goodPhotonsTagHLT = cms.EDProducer("FlashggPhotonTriggerCandProducer",
                                                  filterNames = options['TnPHLTTagFilters'],
                                                  inputs      = cms.InputTag("goodPhotonsTagLeadMatch"),
                                                  bits        = cms.InputTag(options['TRIGGER_RES']),
                                                  objects     = cms.InputTag(options['PAT_TRIG']),
                                                  dR          = cms.double(0.3),
                                                  isAND       = cms.bool(False)
                                                  )




    ###################################################################                                                                                                                   
    ## PHOTON ISOLATION                                                                                                                                                                   
    ###################################################################            
    process.load("RecoEgamma/PhotonIdentification/PhotonIDValueMapProducer_cfi")
    process.noL1RECO = cms.EDProducer("CandViewShallowCloneCombiner",
                                      decay = cms.string("goodPhotonsTagHLT goodPhotonProbesPreselection"),
                                      #decay = cms.string("goodPhotonsTagHLT goodPhotonsProbeHLT"),
                                      checkCharge = cms.bool(False),
                                      cut = cms.string("40<mass<180"),
                                      )
    
    #efficiency for HLT with L1 requirement in denominator                                                                                                                                
    process.L1RECO = cms.EDProducer("CandViewShallowCloneCombiner",
                                    decay = cms.string("goodPhotonsTagHLT goodPhotonProbesL1"),
                                    checkCharge = cms.bool(False),
                                    cut = cms.string("40<mass<180"),
                                    )
    

