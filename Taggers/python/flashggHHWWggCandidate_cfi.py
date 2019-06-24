import FWCore.ParameterSet.Config as cms
from HHWWggCandidateDumper_cfi import HHWWggCandidateDumper
from flashggHHWWggTag_cfi import flashggHHWWggTag

from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag

# cfi = configuration fragment include
# Clone these params into _cfg 
FlashggHHWWggCandidate = cms.EDProducer("FlashggHHWWggCandidateProducer", 
                                     PhotonTag              = cms.InputTag('flashggRandomizedPhotons'),
                                     DiPhotonTag            = cms.InputTag('flashggPreselectedDiPhotons'),
                                     #DiPhotonTag            = cms.InputTag('flashggDiPhotons'),
                                     #DiPhotonTag            = cms.InputTag('flashggPreselectedDiPhotons'),
                                     VertexTag              = cms.InputTag('offlineSlimmedPrimaryVertices'),
                                     GenParticleTag         = cms.InputTag('flashggPrunedGenParticles'),
                                     ElectronTag            = cms.InputTag('flashggSelectedElectrons'),
                                     #METTag                 = cms.InputTag('slimmedMETs'),
                                     MuonTag                = cms.InputTag('flashggSelectedMuons'),
                                     METTag                 = cms.InputTag('flashggMets'),
                                     #JetTag                 = cms.InputTag('flashggJets'),
                                     JetTags                = UnpackedJetCollectionVInputTag, 
                                     useVertex0only=cms.bool(False),
                                     MVAResultTag=cms.InputTag('flashggDiPhotonMVA'),
                                     leptonPtThreshold = cms.double(20),
                                     muonEtaThreshold = cms.double(2.4),
                                     leadPhoOverMassThreshold = cms.double(0.375),
                                     subleadPhoOverMassThreshold = cms.double(0.25),
                                     MVAThreshold = cms.double(0.0),                                                     
                                     deltaRMuonPhoThreshold = cms.double(0.5),
                                     jetsNumberThreshold = cms.double(99.), # originially 3. 
                                     jetPtThreshold = cms.double(20.),
                                     jetEtaThreshold= cms.double(2.4),
                                     deltaRPhoLeadJet = cms.double(0.4),
                                     deltaRPhoSubLeadJet = cms.double(0.4),
                                     muPFIsoSumRelThreshold = cms.double(0.25),
                                     deltaRJetMuonThreshold = cms.double(0.4),
                                     PuIDCutoffThreshold = cms.double(0.8),
                                     PhoMVAThreshold = cms.double(-0.9),
                                     METThreshold = cms.double(45.),
                                     DeltaRTrkElec = cms.double(.4),
                                     TransverseImpactParam = cms.double(0.02),
                                     LongitudinalImpactParam = cms.double(0.2),
                                     deltaRPhoElectronThreshold = cms.double(1.),
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
                                     )
flashggHHWWggTagSequence = cms.Sequence( flashggHHWWggTag )
