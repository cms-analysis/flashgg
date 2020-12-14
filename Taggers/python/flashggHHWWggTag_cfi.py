import FWCore.ParameterSet.Config as cms

from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag # should include jet systematics
from flashgg.MicroAOD.flashggJets_cfi import  maxJetCollections

import flashgg.Taggers.flashggDoubleHReweight_cfi as reweight_settings
from flashgg.Taggers.flashggDoubleHReweight_cfi import flashggDoubleHReweight

# cfi = configuration fragment include
# Clone these params into _cfg
flashggHHWWggTag = cms.EDProducer("FlashggHHWWggTagProducer",

                                    # PhotonTag = cms.InputTag('flashggRandomizedPhotons'), ##-- Photons not used 
                                    DiPhotonTag = cms.InputTag('flashggPreselectedDiPhotons'),
                                    SystLabel = cms.string(""),
                                    JetsCollSize = cms.uint32(maxJetCollections), 
                                    JetsSuffixes = cms.vstring(''), # Nominal and systematic variations
                                    DiPhotonName = cms.string('flashggPreselectedDiPhotons'), #
                                    VertexTag = cms.InputTag('offlineSlimmedPrimaryVertices'),
                                    GenParticleTag         = cms.InputTag('flashggPrunedGenParticles'),
                                    ElectronTag            = cms.InputTag('flashggElectronSystematics'),
                                    MuonTag                = cms.InputTag('flashggMuonSystematics'),
                                    METTag                 = cms.InputTag('flashggMetSystematics'),
                                    JetTags                = UnpackedJetCollectionVInputTag,
                                    DiPhotonSuffixes = cms.vstring(''), #nominal and systematic variations
                                    MVAResultTag=cms.InputTag('flashggDiPhotonMVA'),

                                    ##-- Common Object Selections among Three Final States 
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
                                    deltaRPhoElectronThreshold = cms.double(0.4), 
                                    # deltaMassElectronZThreshold = cms.double(10.),
                                    deltaMassElectronZThreshold = cms.double(5.), ##-- Keep at loosest, can select further by category after 
                                    electronEtaThresholds=cms.vdouble(1.4442,1.566,2.5),
                                    nonTrigMVAThresholds = cms.vdouble(0.913286,0.805013,0.358969),
                                    nonTrigMVAEtaCuts = cms.vdouble(0.8,1.479,2.5),
                                    electronIsoThreshold = cms.double(0.15),
                                    electronNumOfHitsThreshold = cms.double(1),
                                    useElectronMVARecipe = cms.bool(False),
                                    useElectronLooseID = cms.bool(True),

                                    ##-- Misc 
                                    rhoTag = cms.InputTag('fixedGridRhoFastjetAll'),
                                    RECOfilters = cms.InputTag('TriggerResults::RECO'),
                                    PATfilters = cms.InputTag('TriggerResults::PAT'),
                                    FLASHfilters = cms.InputTag('TriggerResults::FLASHggMicroAOD'),
                                    btagThresh = cms.double(0.45),
                                    BTagTypes = cms.vstring('mini_pfDeepFlavourJetTags:probb','mini_pfDeepFlavourJetTags:probbb','mini_pfDeepFlavourJetTags:problepb'),
                                    doHHWWggTagCutFlowAnalysis = cms.bool(False), # save events for cut flow analysis
                                    doHHWWggNonResAnalysis = cms.bool(False),
                                    doHHWWggFHptOrdered = cms.bool(False), # for FH final state, choose four leading pT jets as four jets
                                    doHHWWggDebug = cms.bool(False), # False by default to avoid extra print statements, set true with flag 
                                    HHWWggAnalysisChannel = cms.string("SL"), # final state analysis to run. SL by default. Can be SL, FH, FL, or all 
                                    deltaRLeps = cms.double(0.4),
                                    MetPtThreshold = cms.double(20.),
                                    JetIDLevel = cms.string(''), # Tight, Tight2017 or Tight2018 as set in 

                                    ##-- Fully Leptonic Final State Parameters 
                                    lep1ptThre = cms.double(20.),
                                    lep2ptThre = cms.double(10.),
                                    lep3ptThre = cms.double(10.),
                                    DiLepPtThre = cms.double(0.),
                                    DiLepMassThre = cms.double(0.),
                                    MassTThre = cms.double(0.),
                                    MassT_l2Thre = cms.double(0.),
                                    SaveOthers = cms.bool(True),

                                    # Vertex choice 
                                    HHWWgguseZeroVtx = cms.bool(False)                             
                                    )
