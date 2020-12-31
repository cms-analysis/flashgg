import FWCore.ParameterSet.Config as cms
from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag 

##-- PileupJetID 
pujidPtBin1_loose  = [-0.97, -0.68, -0.53, -0.47]
pujidPtBin2_loose  = [-0.89, -0.52, -0.38, -0.30]

pujidPtBin1_medium = [0.18, -0.55, -0.42, -0.36]
pujidPtBin2_medium = [0.61, -0.35, -0.23, -0.17]

pujidPtBin1_tight  = [0.69, -0.35, -0.26, -0.21]
pujidPtBin2_tight  = [0.86, -0.10, -0.05, -0.01]

# cfi = configuration fragment include
# Clone these params into _cfg
flashggHHWWggTag = cms.EDProducer("FlashggHHWWggTagProducer",

                                    ##-- Collections 
                                    DiPhotonTag                      = cms.InputTag('flashggPreselectedDiPhotons'),
                                    SystLabel                        = cms.string(""),
                                    DiPhotonName                     = cms.string('flashggPreselectedDiPhotons'), 
                                    VertexTag                        = cms.InputTag('offlineSlimmedPrimaryVertices'),
                                    GenParticleTag                   = cms.InputTag('flashggPrunedGenParticles'),
                                    ElectronTag                      = cms.InputTag('flashggElectronSystematics'),
                                    MuonTag                          = cms.InputTag('flashggMuonSystematics'),
                                    METTag                           = cms.InputTag('flashggMetSystematics'),
                                    JetTags                          = UnpackedJetCollectionVInputTag,
                                    MVAResultTag                     = cms.InputTag('flashggDiPhotonMVA'),

                                    ##-- Common Object Selections among Three Final States 
                                    leptonPtThreshold = cms.double(10.),
                                    muonEtaThreshold = cms.double(2.4),
                                    leadPhoOverMassThreshold = cms.double(0.33), 
                                    subleadPhoOverMassThreshold = cms.double(0.25),
                                    MVAThreshold = cms.double(0.0),
                                    deltaRMuonPhoThreshold = cms.double(0.4),
                                    jetPtThreshold = cms.double(25.),
                                    jetEtaThreshold=  cms.double(2.4),
                                    deltaRPhoLeadJet = cms.double(0.4),
                                    deltaRPhoSubLeadJet = cms.double(0.4),
                                    muPFIsoSumRelThreshold = cms.double(0.25), ##-- Set in HHWWggCustomize based on metaconditions. 0.25 is loose. 0.15 tight? 
                                    deltaRJetMuonThreshold = cms.double(0.4),
                                    DeltaRTrkElec = cms.double(.4),
                                    deltaRPhoElectronThreshold = cms.double(0.4), 
                                    deltaMassElectronZThreshold = cms.double(5.), ##-- Keep at loosest, can select further by category after 
                                    electronEtaThresholds=cms.vdouble(1.4442,1.566,2.5),
                                    nonTrigMVAThresholds = cms.vdouble(0.913286,0.805013,0.358969),
                                    nonTrigMVAEtaCuts = cms.vdouble(0.8,1.479,2.5),
                                    ElectronID = cms.string("passMVAMediumId"), ##-- Set in metaconditions, pass through HHWWggCustomize 
                                    MuonID = cms.string("isLooseMuon"), ##-- Set in metaconditions, pass through HHWWggCustomize 

                                    ##-- Misc 
                                    btagThresh = cms.double(2), ##-- anything greater than 1 should save all events 
                                    BTagTypes = cms.vstring('mini_pfDeepFlavourJetTags:probb','mini_pfDeepFlavourJetTags:probbb','mini_pfDeepFlavourJetTags:problepb'),
                                    doHHWWggTagCutFlowAnalysis = cms.bool(False), # save events for cut flow analysis
                                    doHHWWggNonResAnalysis = cms.bool(False),
                                    doHHWWggFHptOrdered = cms.bool(False), # for FH final state, choose four leading pT jets as four jets
                                    doHHWWggFHminWHJets = cms.bool(True), # for FH final state, choose 2jet using W another 2 using H
                                    doHHWWggFHminWHLead2Jet = cms.bool(False), # for FH final state, choose 2jet using W another 2 highest pT
                                    doHHWWggFHminHiggsMassOnly = cms.bool(False), # for FH final state, choose 4 jets using H mass
                                    doHHWWggDebug = cms.bool(False), # False by default to avoid extra print statements, set true with flag, set in HHWWggCustomize 
                                    HHWWggAnalysisChannel = cms.string("all"), # final state analysis to run. Can be SL, FH, FL, all. all by default. 
                                    FillUntagged = cms.bool(False), # Fill untagged category if event does not fall into HHWWggAnalysisChannel
                                    deltaRLeps = cms.double(0.4),
                                    FL_METThreshold = cms.double(20.), ##-- Only used for Fully-Leptonic Final State 
                                    JetIDLevel = cms.string(''), # Tight, Tight2017 or Tight2018 as set in HHWWggCustomize from MetaConditions 
                                    applyPUJetID = cms.bool(True),
                                    ## define the pujid working point 
                                    pujidWpPtBin1 = cms.vdouble(pujidPtBin1_tight), ## WP for 20 < pT < 30 
                                    pujidWpPtBin2 = cms.vdouble(pujidPtBin2_tight), ## WP for 30 < pT < 50                                    

                                    ##-- Fully Leptonic Final State Parameters 
                                    lep1ptThre = cms.double(20.),
                                    lep2ptThre = cms.double(10.),
                                    lep3ptThre = cms.double(10.),
                                    DiLepPtThre = cms.double(0.),
                                    DiLepMassThre = cms.double(0.),
                                    MassTThre = cms.double(0.),
                                    MassT_l2Thre = cms.double(0.),
                                    DiLepMassHigThre = cms.double(100.),
                                    DiLepMassLowThre = cms.double(80.),
                                    FL_Dipho_pT_Thre = cms.double(54.),

                                    ##-- Fully Hadronic Final State 
                                    FH_Dipho_pT_Thre = cms.double(160.),

                                    # Vertex choice 
                                    HHWWgguseZeroVtx = cms.bool(False) ##-- Set with cmssw config flag, done in HHWWggCustomize                          
                                    )
