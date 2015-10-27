import FWCore.ParameterSet.Config as cms

def setModules(process, options):

    process.sampleInfo = cms.EDAnalyzer("tnp::SampleInfoTree",
                                        vertexCollection = cms.InputTag("offlineSlimmedPrimaryVertices"),
                                        genInfo = cms.InputTag("generator")
                                        )

    from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel
    process.hltFilter = hltHighLevel.clone()
    process.hltFilter.throw = cms.bool(True)
    process.hltFilter.HLTPaths = options['TnPPATHS']
    
    process.pileupReweightingProducer = cms.EDProducer("PileupWeightProducer",
                                                       hardcodedWeights = cms.untracked.bool(True)
                                                       )
    
    process.GsfDRToNearestTauProbe = cms.EDProducer("DeltaRNearestGenPComputer",
                                                    probes = cms.InputTag("photonFromDiPhotons"),
                                                    objects = cms.InputTag('flashggPrunedGenParticles'),
                                                    objectSelection = cms.string("abs(pdgId)==15"),
                                                    )
    
    process.GsfDRToNearestTauSC = cms.EDProducer("DeltaRNearestGenPComputer",
                                                 probes = cms.InputTag("superClusterCands"),
                                                 objects = cms.InputTag('flashggPrunedGenParticles'),
                                                 objectSelection = cms.string("abs(pdgId)==15"),
                                                 )
    
    process.GsfDRToNearestTauTag = cms.EDProducer("DeltaRNearestGenPComputer",
                                                  probes = cms.InputTag("photonFromDiPhotons"),
                                                  objects = cms.InputTag('flashggPrunedGenParticles'),
                                                  objectSelection = cms.string("abs(pdgId)==15"),
                                                  )
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
                                                 diphotonMVAThreshold = cms.double(-0.6)
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
                                                   id_cut    = cms.double(0.2)
                                                   )
    
    process.goodPhotonTagsIDMVA = cms.EDProducer("FlashggPhotonSelectorByDoubleValueMap",
                                                 input     = cms.InputTag("goodPhotonTags"),
                                                 cut       = cms.string(options['PHOTON_CUTS']),
                                                 selection = cms.InputTag("photonFromDiPhotons:idmva"),
                                                 id_cut    = cms.double(0.2)
                                                 )
    
    ###################################################################

    process.goodPhotonsTagHLT = cms.EDProducer("FlashggPhotonTriggerCandProducer",
                                               filterNames = options['TnPHLTTagFilters'],
                                               inputs      = cms.InputTag("goodPhotonTags"),
                                               bits        = cms.InputTag('TriggerResults::HLT'),
                                               objects     = cms.InputTag('selectedPatTrigger'),
                                               dR          = cms.double(0.3),
                                               isAND       = cms.bool(True)
                                               )
    
    process.goodPhotonsProbeHLT = cms.EDProducer("FlashggPhotonTriggerCandProducer",
                                                 filterNames = options['TnPHLTProbeFilters'],
                                                 inputs      = cms.InputTag("goodPhotonProbes"),
                                                 bits        = cms.InputTag('TriggerResults::HLT'),
                                                 objects     = cms.InputTag('selectedPatTrigger'),
                                                 dR          = cms.double(0.3),
                                                 isAND       = cms.bool(True)
                                                 )
    
    process.goodPhotonProbesL1 = cms.EDProducer("FlashggPhotonL1CandProducer",
                                                inputs = cms.InputTag("goodPhotonProbes"),
                                                isoObjects = cms.InputTag("l1extraParticles:Isolated"),
                                                nonIsoObjects = cms.InputTag("l1extraParticles:NonIsolated"),
                                                minET = cms.double(40),
                                                dRmatch = cms.double(0.2),
                                                isolatedOnly = cms.bool(False)
                                                )
    
    ###################################################################
    ## PHOTON ISOLATION
    ###################################################################
    process.load("RecoEgamma/PhotonIdentification/PhotonIDValueMapProducer_cfi")

    process.tagTightRECO = cms.EDProducer("CandViewShallowCloneCombiner",
                                      decay = cms.string("goodPhotonsTagHLT goodPhotonsProbeHLT"), 
                                      checkCharge = cms.bool(False),
                                      cut = cms.string("40<mass<1000"),
                                      )

    
    ###################################################################
    ## MC MATCHING
    ###################################################################

    process.McMatchTag = cms.EDProducer("MCTruthDeltaRMatcherNew",
                                        matchPDGId = cms.vint32(11),
                                        src = cms.InputTag("goodPhotonTags"),
                                        distMin = cms.double(0.2),
                                        matched = cms.InputTag("flashggPrunedGenParticles"),
                                        checkCharge = cms.bool(False)
                                        )
    
    process.McMatchRECO = cms.EDProducer("MCTruthDeltaRMatcherNew",
                                         matchPDGId = cms.vint32(11),
                                         src = cms.InputTag("goodPhotonProbes"),
                                         distMin = cms.double(0.2),
                                         matched = cms.InputTag("flashggPrunedGenParticles"),
                                         checkCharge = cms.bool(False)
                                         )
