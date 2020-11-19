import FWCore.ParameterSet.Config as cms

from flashgg.Taggers.globalVariables_cff import globalVariables
from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag # should include jet systematics
from flashgg.MicroAOD.flashggJets_cfi import  maxJetCollections, flashggDeepCSV

import flashgg.Taggers.flashggDoubleHReweight_cfi as reweight_settings
from flashgg.Taggers.flashggDoubleHReweight_cfi import flashggDoubleHReweight

# # to not run jets systematics
# UnpackedJetCollectionVInputTag =  cms.VInputTag(
#     cms.InputTag("flashggUnpackedJets","0"), cms.InputTag("flashggUnpackedJets","1"), cms.InputTag("flashggUnpackedJets","2"), cms.InputTag("flashggUnpackedJets","3"), cms.InputTag("flashggUnpackedJets","4"),
#     cms.InputTag("flashggUnpackedJets","5"), cms.InputTag("flashggUnpackedJets","6"), cms.InputTag("flashggUnpackedJets","7"), cms.InputTag("flashggUnpackedJets","8"), cms.InputTag("flashggUnpackedJets","9"),
#     cms.InputTag("flashggUnpackedJets","10"), cms.InputTag("flashggUnpackedJets","11")
# )


# cfi = configuration fragment include
# Clone these params into _cfg
flashggHHWWggTag = cms.EDProducer("FlashggHHWWggTagProducer",
                                    globalVariables=globalVariables,
                                    PhotonTag = cms.InputTag('flashggRandomizedPhotons'),
                                    # DiPhotonTag = cms.InputTag('flashggDiPhotonSystematics'),
                                    DiPhotonTag = cms.InputTag('flashggPreselectedDiPhotons'),
                                    SystLabel = cms.string(""),
                                    JetsName = cms.string("bRegProducer"), #
                                    JetsCollSize = cms.uint32(maxJetCollections), #
                                    JetsSuffixes = cms.vstring(''), #nominal and systematic variations
                                    DiPhotonName = cms.string('flashggPreselectedDiPhotons'), #
                                    VertexTag = cms.InputTag('offlineSlimmedPrimaryVertices'),
                                    GenParticleTag         = cms.InputTag('flashggPrunedGenParticles'),
                                    ElectronTag            = cms.InputTag('flashggSelectedElectrons'),
                                    MuonTag                = cms.InputTag('flashggSelectedMuons'),
                                    METTag                 = cms.InputTag('flashggMets'),
                                    # METTag                 = cms.InputTag('flashggMetsCorr'), # RunIIFall17-3-2-0 contains these and NOT flashggMets
                                    JetTags                = UnpackedJetCollectionVInputTag,
                                    DiPhotonSuffixes = cms.vstring(''), #nominal and systematic variations
                                    useVertex0only=cms.bool(False),
                                    MVAResultTag=cms.InputTag('flashggDiPhotonMVA'),
                                    leptonPtThreshold = cms.double(10.),
                                    muonEtaThreshold = cms.double(2.4),
                                    leadPhoOverMassThreshold = cms.double(0.35), # was 0.375
                                    subleadPhoOverMassThreshold = cms.double(0.25),
                                    MVAThreshold = cms.double(0.0),
                                    deltaRMuonPhoThreshold = cms.double(0.4),
                                    jetsNumberThreshold = cms.double(99.), # originially 3.
                                    jetPtThreshold = cms.double(25.),
                                    jetEtaThreshold= cms.double(2.4),
                                    deltaRPhoLeadJet = cms.double(0.4),
                                    deltaRPhoSubLeadJet = cms.double(0.4),
                                    muPFIsoSumRelThreshold = cms.double(0.15),
                                    deltaRJetMuonThreshold = cms.double(0.4),
                                    PuIDCutoffThreshold = cms.double(0.8),
                                    PhoMVAThreshold = cms.double(-0.9),
                                    METThreshold = cms.double(0.),
                                    DeltaRTrkElec = cms.double(.4),
                                    TransverseImpactParam = cms.double(0.02),
                                    LongitudinalImpactParam = cms.double(0.2),
                                    deltaRPhoElectronThreshold = cms.double(0.4), # was 1
                                    deltaMassElectronZThreshold = cms.double(10.),
                                    # deltaMassElectronZ_FL_Threshold = cms.double(5.), # set in hhwwgg customize instead 
                                    electronEtaThresholds=cms.vdouble(1.4442,1.566,2.5),
                                    nonTrigMVAThresholds = cms.vdouble(0.913286,0.805013,0.358969),
                                    nonTrigMVAEtaCuts = cms.vdouble(0.8,1.479,2.5),
                                    electronIsoThreshold = cms.double(0.15),
                                    electronNumOfHitsThreshold = cms.double(1),
                                    useElectronMVARecipe = cms.bool(False),
                                    useElectronLooseID = cms.bool(True),
                                    rhoTag = cms.InputTag('fixedGridRhoFastjetAll'),
                                    RECOfilters = cms.InputTag('TriggerResults::RECO'),
                                    PATfilters = cms.InputTag('TriggerResults::PAT'),
                                    FLASHfilters = cms.InputTag('TriggerResults::FLASHggMicroAOD'),
                                    # bTag = cms.string(flashggDeepCSV),
                                    # btagThresh = cms.double(100)     # no btag (Save all btags < 100)
                                    btagThresh = cms.double(0.45),
                                    doHHWWggTagCutFlowAnalysis = cms.bool(False), # save events for cut flow analysis
                                    doHHWWggNonResAnalysis = cms.bool(False),
                                    doHHWWggFHptOrdered = cms.bool(False), # for FH final state, choose four leading pT jets as four jets
                                    doHHWWggDebug = cms.bool(False), # False by default to avoid extra print statements, set true with flag 
                                    HHWWggAnalysisChannel = cms.string("SL"), # final state analysis to run. SL by default. Can be SL, FL, or FH
                                    # saveHHWWggFinalStateVars = cms.bool(False)
                                    deltaRLeps = cms.double(0.4),
                                    MetPtThreshold = cms.double(20.),
                                    
                                    ##-- Non-Resonant Reweighting 
                                    doReweight = flashggDoubleHReweight.doReweight,
                                    reweight_producer = cms.string(reweight_settings.reweight_producer),
                                    reweight_names = cms.vstring(reweight_settings.reweight_names),  

                                    EB_Photon_MVA_Threshold = cms.double(0.07),
                                    EE_Photon_MVA_Threshold = cms.double(-0.03),
                                    #bTag = cms.string(flashggDeepCSV),
                                    # btagThresh = cms.double(100)     # no btag (Save all btags < 100)
                                    lep1ptThre = cms.double(20.),
                                    lep2ptThre = cms.double(10.),
                                    lep3ptThre = cms.double(10.),
                                    DiLepPtThre = cms.double(0.),
                                    DiLepMassThre = cms.double(0.),
                                    MassTThre = cms.double(0.),
                                    MassT_l2Thre = cms.double(0.),
                                    SaveOthers = cms.bool(True),

                                    # vertex
                                    HHWWgguseZeroVtx = cms.bool(False)                             
                                    )
# flashggHHWWggTagSequence = cms.Sequence( flashggHHWWggTag ) # not used
