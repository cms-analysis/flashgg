import FWCore.ParameterSet.Config as cms

import flashgg.Taggers.dumperConfigTools as cfgTools

from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag
import flashgg.Taggers.PUJID_wps as pujid

from flashgg.Taggers.globalVariables_cff import globalVariables
import flashgg.Taggers.flashggDoubleHReweight_cfi as reweight_settings
from flashgg.Taggers.flashggDoubleHReweight_cfi import flashggDoubleHReweight
from flashgg.MicroAOD.flashggJets_cfi import  maxJetCollections



#default values first
year_norm = 0
jetID = 'Loose'
weightsFile="flashgg/Taggers/data/HHTagger/training_with_10_12_2018_commonTraining_2016.weights.xml"# path to TMVA weights
MVAscalingValue=1.#scale MVA output before the cumulative transformation for 2017(2016 kept unchanged for simplicity, we will probably change that once we have all 3 years.)
MVAFlatteningFileName="flashgg/Taggers/data/HHTagger/cumulativeTransformation_20190321_2016_2017.root"
ttHWeightfile="flashgg/Taggers/data/ttHKiller/2017model.pb"

ttHKiller_mean = cms.vdouble(2.86329618e+02,  7.08058280e+01,  1.51705583e-01,  2.01783465e-03,
 2.97495115e-03,  1.27818958e+00,  5.00813342e+00,  1.09232817e+01,
1.98370734e+02,  6.75976357e+01,  3.79198017e+01,  6.48033320e+01,
 3.71474043e+01,  1.32235881e+02,  1.23636325e-02, -1.90268192e-02,
 -4.32500136e-03, -3.56787374e-02, -3.77824101e-03, -1.47459903e-02,
 8.49414658e-03,  2.54511156e-03, -2.81678797e-03,  1.50134999e-03,
5.15499904e-01,  4.89883682e-01)

ttHKiller_std = cms.vdouble(2.10155580e+02, 5.75043629e+01, 1.90354344e+00, 1.85750063e+00,
 1.82667164e+00, 5.86412476e-01, 1.61136045e+00, 2.30744881e+01,
 3.77189642e+02, 5.30695227e+01, 2.44528358e+01, 5.03981834e+01,
 2.43547708e+01, 1.01677139e+02, 1.10120412e+00, 1.17757987e+00,
1.08501127e+00, 1.12699494e+00, 1.35765654e+00, 1.79804818e+00,
1.80435878e+00, 1.81725954e+00, 1.78700277e+00, 1.81540181e+00,
2.90404729e-01, 2.85301766e-01)
ttHKiller_listmean = cms.vdouble(9.77379993e+01, -2.75249574e-03,  6.81701973e-02)
ttHKiller_liststd = cms.vdouble(85.75455047,  1.31191137,  1.85627069)


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
                                   MaxJetEta   = cms.double(2.5),
                                   MJJBoundaries = cms.vdouble(70.,190.),
                                  # BTagType = cms.untracked.string('pfCombinedInclusiveSecondaryVertexV2BJetTags'), #string for btag algorithm
                                 #c  BTagType = cms.untracked.string('pfDeepCSVJetTags:probb'), #string for btag algorithm
                                   BTagType = cms.vstring('pfDeepCSVJetTags:probb','pfDeepCSVJetTags:probbb'), #string for btag algorithm
                                   UseJetID = cms.bool(True),
                                   JetIDLevel = cms.string(jetID),

                                   #MVABoundaries  = cms.vdouble(0.29,0.441, 0.724), # category boundaries for MVA w/o Mjj
                                   #MXBoundaries   = cms.vdouble(250., 354., 478., 560.), # .. and MX w/o Mjj
                                   #MJJBoundariesLower = cms.vdouble(98.0,95.0,97.0,96.0,95.0,95.0,95.0,95.0,95.0,95.0,95.0,95.0),#for each category following the convention cat0=MX0 MVA0, cat1=MX1 MVA0, cat2=MX2 MVA0....
                                   #MJJBoundariesUpper = cms.vdouble(150.0,150.0,143.0,150.0,150.0,150.0,150.0,145.0,155.0,142.0,146.0,152.0),#for each category following the convention cat0=MX0 MVA0, cat1=MX1 MVA0, cat2=MX2 MVA0....
                                   MVABoundaries  = cms.vdouble(0.23,0.455, 0.709), # category boundaries for MVA with Mjj
                                   MXBoundaries   = cms.vdouble(250., 336., 411., 556.), # .. and MX for MVA with Mjj
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
                                   doCategorization=cms.bool(False),#do categorization based on MVA x MX or only fill first tree with all events
                                   #MVAFlatteningFileName=cms.untracked.FileInPath("flashgg/Taggers/data/HHTagger/cumulativeTransformation_20181210_common_2016_2017.root"),#for BDT w/o Mjj
                                   MVAFlatteningFileName=cms.untracked.FileInPath("%s"%MVAFlatteningFileName),#FIXME, this should be optional, is it?
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
                                   ttHScoreThreshold = cms.double(0.0), #to be updated
                                   # For standardization
                                   ttHKiller_mean = ttHKiller_mean,
                                   ttHKiller_std = ttHKiller_std,
                                   ttHKiller_listmean = ttHKiller_listmean, 
                                   ttHKiller_liststd = ttHKiller_liststd 
                                  ) 

cfgTools.addVariables(flashggDoubleHTag.MVAConfig.variables,
                      # here the syntax is VarNameInTMVA := expression
                      [
                       "Mjj := dijet().M()",
                       "leadingJet_DeepCSV := leadJet().bDiscriminator('pfDeepCSVJetTags:probb')+leadJet().bDiscriminator('pfDeepCSVJetTags:probbb')",#FIXME make the btag type configurable?
                       "subleadingJet_DeepCSV := subleadJet().bDiscriminator('pfDeepCSVJetTags:probb')+subleadJet().bDiscriminator('pfDeepCSVJetTags:probbb')",
                       "absCosThetaStar_CS := abs(getCosThetaStar_CS)",
                       "absCosTheta_bb := abs(CosThetaAngles()[1])",
                       "absCosTheta_gg := abs(CosThetaAngles()[0])",
                       "diphotonCandidatePtOverdiHiggsM := diphotonPtOverM()",
                       "dijetCandidatePtOverdiHiggsM := dijetPtOverM()",
                       "customLeadingPhotonIDMVA := diPhoton.leadingView.phoIdMvaWrtChosenVtx",
                       "customSubLeadingPhotonIDMVA := diPhoton.subLeadingView.phoIdMvaWrtChosenVtx",
                       "leadingPhotonSigOverE := diPhoton.leadingPhoton.sigEOverE",
                       "subleadingPhotonSigOverE := diPhoton.subLeadingPhoton.sigEOverE",
                     #  "sigmaMOverMDecorr := getSigmaMDecorr()",
                       "sigmaMOverM := sqrt(0.5*(diPhoton.leadingPhoton.sigEOverE*diPhoton.leadingPhoton.sigEOverE + diPhoton.subLeadingPhoton.sigEOverE*diPhoton.subLeadingPhoton.sigEOverE))",
                       "PhoJetMinDr := getPhoJetMinDr()",
                       "rho := global.rho",
                       "(leadingJet_bRegNNResolution*1.4826) := leadJet().userFloat('bRegNNResolution')*1.4826",
                       "(subleadingJet_bRegNNResolution*1.4826) := subleadJet().userFloat('bRegNNResolution')*1.4826",
                       "(sigmaMJets*1.4826) := getSigmaMOverMJets()*1.4826",
                     #  "year := %d"%year_norm,
                       ]
                      )


