import FWCore.ParameterSet.Config as cms

import flashgg.Taggers.dumperConfigTools as cfgTools

from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag
import flashgg.Taggers.PUJID_wps as pujid

from flashgg.Taggers.globalVariables_cff import globalVariables
import flashgg.Taggers.flashggDoubleHReweight_cfi as reweight_settings
from flashgg.Taggers.flashggDoubleHReweight_cfi import flashggDoubleHReweight
from flashgg.MicroAOD.flashggJets_cfi import  maxJetCollections
from flashgg.Taggers.flashggTags_cff import flashggTTHLeptonicTag


jetID = ''
weightsFile=""# path to TMVA weights
MVAscalingValue=1.#scale MVA output before the cumulative transformation for 2017(2016 kept unchanged for simplicity, we will probably change that once we have all 3 years.)
MVAFlatteningFileName=""
ttHWeightfile=""

ttHKiller_mean = cms.vdouble()
ttHKiller_std = cms.vdouble()
ttHKiller_listmean = cms.vdouble()
ttHKiller_liststd = cms.vdouble()
MaxJetEta = 2.5

flashggDoubleHTag = cms.EDProducer("FlashggDoubleHTagProducer",
                                   DiPhotonName = cms.string('flashggPreselectedDiPhotons'), # 
                                   DiPhotonSuffixes = cms.vstring(''), #nominal and systematic variations 
                                   JetsName = cms.string("bRegProducer"), # 
                                   JetsCollSize = cms.uint32(maxJetCollections), #
                                   JetsSuffixes = cms.vstring(''), #nominal and systematic variations 
                                   GenParticleTag = cms.InputTag( "flashggPrunedGenParticles" ), # to compute MC-truth info
                                   
                                   VetoConeSize   = cms.double(0.4),
                                   MinLeadPhoPt   = cms.double(1./3.),
                                   MinSubleadPhoPt   = cms.double(0.25),
                                   ScalingPtCuts = cms.bool(True),
                                   DoSigmaMDecorr =cms.untracked.uint32(1),#transformation of sigmaM/M
                                   SigmaMDecorrFile = cms.untracked.FileInPath("flashgg/Taggers/data/diphoMVA_sigmaMoMdecorr_split_Mgg40_180.root"),
                                   ApplyEGMPhotonID = cms.untracked.bool(False),
                                   PhotonIDCut = cms.double(0.2),#this is loose id for 2016
                                   PhotonElectronVeto =cms.untracked.vint32(1, 1), #0: Pho1, 1: Pho2

                                   MinJetPt   = cms.double(25.),
                                   MaxJetEta   = cms.double(MaxJetEta),
                                   MJJBoundaries = cms.vdouble(70.,190.),
                                   #BTagType = cms.vstring('pfDeepCSVJetTags:probb','pfDeepCSVJetTags:probbb'), #string for btag algorithm
                                   BTagType = cms.vstring('mini_pfDeepFlavourJetTags:probb','mini_pfDeepFlavourJetTags:probbb','mini_pfDeepFlavourJetTags:problepb'), #string for btag algorithm
                                   UseJetID = cms.bool(True),
                                   JetIDLevel = cms.string(jetID),

                                   #MVABoundaries  = cms.vdouble(0.29,0.441, 0.724), # category boundaries for MVA w/o Mjj
                                   #MXBoundaries   = cms.vdouble(250., 354., 478., 560.), # .. and MX w/o Mjj
                                   #MJJBoundariesLower = cms.vdouble(98.0,95.0,97.0,96.0,95.0,95.0,95.0,95.0,95.0,95.0,95.0,95.0),#for each category following the convention cat0=MX0 MVA0, cat1=MX1 MVA0, cat2=MX2 MVA0....
                                   #MJJBoundariesUpper = cms.vdouble(150.0,150.0,143.0,150.0,150.0,150.0,150.0,145.0,155.0,142.0,146.0,152.0),#for each category following the convention cat0=MX0 MVA0, cat1=MX1 MVA0, cat2=MX2 MVA0....
                                   #MVABoundaries  = cms.vdouble(0.23,0.455, 0.709), # category boundaries for MVA with Mjj
                                   #MXBoundaries   = cms.vdouble(250., 336., 411., 556.), # .. and MX for MVA with Mjj
                                   MVABoundaries  = cms.vdouble(0.32,0.54, 0.70), # category boundaries for MVA with Mjj
                                   MXBoundaries   = cms.vdouble(250., 370.,480.,585.,250.,335.,380.,545.,250.,330.,360.,530.), # .. and MX for MVA with Mjj
                                   nMX   = cms.uint32(4), # number of MX categories
                                   MJJBoundariesLower = cms.vdouble(70.0,70.0,70.0,70.0,70.0,70.0,70.0,70.0,70.0,70.0,70.0,70.0),#for each category following the convention cat0=MX0 MVA0, cat1=MX1 MVA0, cat2=MX2 MVA0....
                                   MJJBoundariesUpper = cms.vdouble(190.0,190.0,190.0,190.0,190.0,190.0,190.0,190.0,190.0,190.0,190.0,190.0),#for each category following the convention cat0=MX0 MVA0, cat1=MX1 MVA0, cat2=MX2 MVA0....
                                   MVAConfig = cms.PSet(variables=cms.VPSet(), # variables are added below
                                                        classifier=cms.string("BDT::bdt"), # classifier name
                                                        weights=cms.FileInPath("%s"%weightsFile), 
                                                        regression=cms.bool(False), # this is not a regression
                                                        multiclass=cms.bool(True), # this is multiclass 
                                                        multiclassSignalIdx=cms.int32(2), # this is multiclass index for Signal
                                                        ),

                                   doMVAFlattening=cms.bool(True),#do transformation of cumulative to make it flat
                                   MVAscaling=cms.double(MVAscalingValue),
                                   doCategorization=cms.bool(True),#do categorization based on MVA x MX or only fill first tree with all events
                                   MVAFlatteningFileName=cms.untracked.FileInPath("%s"%MVAFlatteningFileName),#FIXME, this should be optional, is it?
                                   globalVariables=globalVariables,
                                   doReweight = flashggDoubleHReweight.doReweight,
                                   reweight_producer = cms.string(reweight_settings.reweight_producer),
                                   reweight_names = cms.vstring(reweight_settings.reweight_names),
                                   #lepton info
                                   TTHLeptonictag_MuonEtaCut = flashggTTHLeptonicTag.MuonEtaCut,
                                   TTHLeptonictag_MuonPtCut = flashggTTHLeptonicTag.MuonPtCut,
                                   TTHLeptonictag_MuonIsoCut = flashggTTHLeptonicTag.MuonIsoCut,
                                   TTHLeptonictag_MuonPhotonDrCut = flashggTTHLeptonicTag.MuonPhotonDrCut,
                                   TTHLeptonictag_EleEtaCuts = flashggTTHLeptonicTag.EleEtaCuts,
                                   TTHLeptonictag_ElePtCut = flashggTTHLeptonicTag.ElePtCut ,
                                   TTHLeptonictag_ElePhotonDrCut = flashggTTHLeptonicTag.ElePhotonDrCut,
                                   TTHLeptonictag_ElePhotonZMassCut = flashggTTHLeptonicTag.ElePhotonZMassCut,
                                   TTHLeptonictag_DeltaRTrkEle =flashggTTHLeptonicTag.DeltaRTrkEle ,

                                   dottHTagger=cms.bool(False), #whether to do ttH killer. 

                                   ElectronTag=cms.InputTag('flashggSelectedElectrons'),
                                   MuonTag=cms.InputTag('flashggSelectedMuons'),
                                   VertexTag=cms.InputTag('offlineSlimmedPrimaryVertices'),
                                   METTag=cms.InputTag('flashggMets'),
                                   rhoTag = cms.InputTag('fixedGridRhoFastjetAll'),
                                   looseLeptonPtThreshold = cms.double(10.),
                                   muonEtaThreshold = cms.double(2.4),
                                   muPFIsoSumRelThreshold = cms.double(0.25),
                                   deltaRPhoElectronThreshold = cms.double(1.),
                                   deltaRPhoMuonThreshold = cms.double(0.5),
                                   deltaRJetLepThreshold = cms.double(0.4),
                                   useElectronMVARecipe = cms.bool(False),
                                   useElectronLooseID = cms.bool(True),
                                   electronEtaThresholds=cms.vdouble(1.4442,1.566,2.5),
                                   ttHWeightfile = cms.untracked.FileInPath("%s"%ttHWeightfile), # for now
                                   ttHScoreThreshold = cms.double(0.), #to be updated
                                   # For standardization
                                   ttHKiller_mean = ttHKiller_mean,
                                   ttHKiller_std = ttHKiller_std,
                                   ttHKiller_listmean = ttHKiller_listmean, 
                                   ttHKiller_liststd = ttHKiller_liststd 
                                  ) 

cfgTools.addVariables(flashggDoubleHTag.MVAConfig.variables,
                      # here the syntax is VarNameInTMVA := expression
                      #### With or without Mjj is customized inside python doubleHCustomize and using options UseMjj
                      [
                       "Mjj := dijet().M()",
                       "leadingJet_DeepFlavour := leadJet().bDiscriminator('mini_pfDeepFlavourJetTags:probb')+leadJet().bDiscriminator('mini_pfDeepFlavourJetTags:probbb')+leadJet().bDiscriminator('mini_pfDeepFlavourJetTags:problepb')",
                       "subleadingJet_DeepFlavour := subleadJet().bDiscriminator('mini_pfDeepFlavourJetTags:probb')+subleadJet().bDiscriminator('mini_pfDeepFlavourJetTags:probbb')+subleadJet().bDiscriminator('mini_pfDeepFlavourJetTags:problepb')",
                       "absCosThetaStar_CS := abs(getCosThetaStar_CS)",
                       "absCosTheta_bb := abs(CosThetaAngles()[1])",
                       "absCosTheta_gg := abs(CosThetaAngles()[0])",
                       "diphotonCandidatePtOverdiHiggsM := diphotonPtOverM()",
                       "dijetCandidatePtOverdiHiggsM := dijetPtOverM()",
                       "customLeadingPhotonIDMVA := diPhoton.leadingView.phoIdMvaWrtChosenVtx",
                       "customSubLeadingPhotonIDMVA := diPhoton.subLeadingView.phoIdMvaWrtChosenVtx",
                       "leadingPhotonSigOverE := diPhoton.leadingPhoton.sigEOverE",
                       "subleadingPhotonSigOverE := diPhoton.subLeadingPhoton.sigEOverE",
                       "sigmaMOverM := sqrt(0.5*(diPhoton.leadingPhoton.sigEOverE*diPhoton.leadingPhoton.sigEOverE + diPhoton.subLeadingPhoton.sigEOverE*diPhoton.subLeadingPhoton.sigEOverE))",
                       "(leadingPhoton_pt/CMS_hgg_mass) := diPhoton.leadingPhoton.pt/diPhoton().mass",
                       "(subleadingPhoton_pt/CMS_hgg_mass) := diPhoton.subLeadingPhoton.pt/diPhoton().mass",
                       "(leadingJet_pt/Mjj) := leadJet().pt/dijet().mass",
                       "(subleadingJet_pt/Mjj) := subleadJet().pt/dijet().mass",
                       "rho := global.rho",
                       "(leadingJet_bRegNNResolution*1.4826) := leadJet().userFloat('bRegNNResolution')*1.4826",
                       "(subleadingJet_bRegNNResolution*1.4826) := subleadJet().userFloat('bRegNNResolution')*1.4826",
                       "(sigmaMJets*1.4826) := getSigmaMOverMJets()*1.4826",
                       "PhoJetMinDr := getPhoJetMinDr()",
                       "PhoJetOtherDr := getPhoJetOtherDr()" 
                       ]
                      )


