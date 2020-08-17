import FWCore.ParameterSet.Config as cms
# from HHWWggCandidateDumper_cfi import HHWWggCandidateDumper
# from flashggHHWWggTag_cfi import flashggHHWWggTag

from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag
from flashgg.MicroAOD.flashggJets_cfi import flashggDeepCSV
# print'UnpackedJetCollectionVInputTag = ',UnpackedJetCollectionVInputTag

# cfi = configuration fragment include
# Clone these params into _cfg 
FlashggHHWWggCandidate = cms.EDProducer("FlashggHHWWggCandidateProducer", 
                                    PhotonTag = cms.InputTag('flashggRandomizedPhotons'),
                                    DiPhotonTag = cms.InputTag('flashggDiPhotonSystematics'),
                                    # DiPhotonTag = cms.InputTag('flashggPreselectedDiPhotons'),
                                    VertexTag = cms.InputTag('offlineSlimmedPrimaryVertices'),
                                    GenParticleTag         = cms.InputTag('flashggPrunedGenParticles'),
                                    ElectronTag            = cms.InputTag('flashggSelectedElectrons'),
                                    MuonTag                = cms.InputTag('flashggSelectedMuons'),
                                    METTag                 = cms.InputTag('flashggMets'),
                                    # METTag                 = cms.InputTag('flashggMetsCorr'), # RunIIFall17-3-2-0 contains these and NOT flashggMets
                                    JetTags                = UnpackedJetCollectionVInputTag, 
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
                                    bTag = cms.string(flashggDeepCSV),
                                    btagThresh = cms.double(0.45)                               
                                    )
# flashggHHWWggTagSequence = cms.Sequence( flashggHHWWggTag ) # not used 
