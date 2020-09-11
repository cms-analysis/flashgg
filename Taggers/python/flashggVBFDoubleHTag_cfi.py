import FWCore.ParameterSet.Config as cms

import flashgg.Taggers.dumperConfigTools as cfgTools

from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag
import flashgg.Taggers.PUJID_wps as pujid

from flashgg.Taggers.globalVariables_cff import globalVariables
import flashgg.Taggers.flashggDoubleHReweight_cfi as reweight_settings
from flashgg.Taggers.flashggDoubleHReweight_cfi import flashggDoubleHReweight
from flashgg.MicroAOD.flashggJets_cfi import  maxJetCollections



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

flashggVBFDoubleHTag = cms.EDProducer("FlashggVBFDoubleHTagProducer",
                                   DiPhotonName = cms.string('flashggPreselectedDiPhotons'), # 
                                   DiPhotonSuffixes = cms.vstring(''), #nominal and systematic variations 
                                   JetsName = cms.string("bRegProducer"), # 
                                   JetsCollSize = cms.uint32(maxJetCollections), #
                                   JetsSuffixes = cms.vstring(''), #nominal and systematic variations 
                                   GenParticleTag = cms.InputTag( "flashggPrunedGenParticles" ), # to compute MC-truth info
                                   
                                   VBFJetsInputTag= UnpackedJetCollectionVInputTag,
                                   VBFJetsSystematicsName=cms.string('flashggJetSystematics'),
                                   VBFJetsCollSize = cms.uint32(maxJetCollections), #

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
                                   UseVBFJetID = cms.bool(False),
                                   VBFJetIDLevel = cms.string(jetID), 
                                   VBFMjjCut = cms.double(0.0),
                                   VBFJetEta = cms.double(4.7),
                                   VBFleadJetPt  = cms.double(40.0),
                                   VBFsubleadJetPt = cms.double(30.0),
                                   MVABoundaries  = cms.vdouble(0.87), # category boundaries for MVA with Mjj
                                   MXBoundaries   = cms.vdouble(0,500), 
                                   nMX   = cms.uint32(2), # number of MX categories
                                   MJJBoundariesLower = cms.vdouble(70.0,70.0,70.0,70.0,70.0,70.0,70.0,70.0,70.0,70.0,70.0,70.0),#for each category following the convention cat0=MX0 MVA0, cat1=MX1 MVA0, cat2=MX2 MVA0....
                                   MJJBoundariesUpper = cms.vdouble(190.0,190.0,190.0,190.0,190.0,190.0,190.0,190.0,190.0,190.0,190.0,190.0),#for each category following the convention cat0=MX0 MVA0, cat1=MX1 MVA0, cat2=MX2 MVA0....
                                   MVAConfigCAT0 = cms.PSet(variables=cms.VPSet(), # variables are added below
                                                        classifier=cms.string("BDT::bdt"), # classifier name
                                                        weights=cms.FileInPath("%s"%weightsFile), 
                                                        regression=cms.bool(False), # this is not a regression
                                                        multiclass=cms.bool(True), # this is multiclass 
                                                        multiclassSignalIdx=cms.int32(2), # this is multiclass index for Signal
                                                        ),
                                   MVAConfigCAT1 = cms.PSet(variables=cms.VPSet(), # variables are added below
                                                        classifier=cms.string("BDT::bdt"), # classifier name
                                                        weights=cms.FileInPath("%s"%weightsFile), 
                                                        regression=cms.bool(False), # this is not a regression
                                                        multiclass=cms.bool(True), # this is multiclass 
                                                        multiclassSignalIdx=cms.int32(2), # this is multiclass index for Signal
                                                        ),
                                   doMVAFlattening=cms.bool(True),#do transformation of cumulative to make it flat
                                   MVAscaling=cms.double(MVAscalingValue),
                                      doCategorization=cms.bool(True),#do categorization based on MVA x MX or only fill first tree with all events
                                   MVAFlatteningFileNameCAT0=cms.untracked.FileInPath("%s"%MVAFlatteningFileName),#FIXME, this should be optional, is it?
                                   MVAFlatteningFileNameCAT1=cms.untracked.FileInPath("%s"%MVAFlatteningFileName),#FIXME, this should be optional, is it?
                                   globalVariables=globalVariables,
                                   doReweight = flashggDoubleHReweight.doReweight,
                                   reweight_producer = cms.string(reweight_settings.reweight_producer),
                                   reweight_names = cms.vstring(reweight_settings.reweight_names),

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

MVAvariables =                       [
                      # here the syntax is VarNameInTMVA := expression
                      #### With or without Mjj is customized inside python doubleHCustomize and using options UseMjj
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
                       "PhoJetOtherDr := getPhoJetOtherDr()",			
                       "(VBFleadJet_pt/VBFJet_mjj) :=  getVBFleadJet_pt()/getVBFJet_mjj() ",
                       "VBFleadJet_eta := getVBFleadJet_eta()",
                       "(VBFsubleadJet_pt/VBFJet_mjj) := getVBFsubleadJet_pt()/getVBFJet_mjj() ",
                       "VBFsubleadJet_eta := getVBFsubleadJet_eta()",
                       "VBFCentrality_jg := getVBFCentrality_jg()",
                       "VBFCentrality_jb := getVBFCentrality_jb()",
                       "VBFDeltaR_jg := getVBFDeltaR_jg()",
                       "VBFDeltaR_jb := getVBFDeltaR_jb()",
                       "VBFProd_eta := getVBFProd_eta",
                       "VBFJet_mjj := getVBFJet_mjj()",
                       "VBFJet_Delta_eta := getVBFJet_Delta_eta()",
                       "VBFleadJet_QGL := getVBFleadJet_QGL() ",
                       "VBFsubleadJet_QGL := getVBFsubleadJet_QGL()",
                       "diHiggs_pt := getdiHiggsP4().pt()",
                       "MX := MX()"
                       ]
cfgTools.addVariables(flashggVBFDoubleHTag.MVAConfigCAT0.variables,MVAvariables)
cfgTools.addVariables(flashggVBFDoubleHTag.MVAConfigCAT1.variables,MVAvariables)
