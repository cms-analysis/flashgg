import FWCore.ParameterSet.Config as cms

import flashgg.Taggers.dumperConfigTools as cfgTools

from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag
import flashgg.Taggers.PUJID_wps as pujid

flashggDoubleHTag = cms.EDProducer("FlashggDoubleHTagProducer",
                                   DiPhotonTag=cms.InputTag('flashggPreselectedDiPhotons'), # diphoton collection (will be replaced by systematics machinery at run time)
                                   JetTags= UnpackedJetCollectionVInputTag, # one jet per vertex
                                   GenParticleTag = cms.InputTag( "flashggPrunedGenParticles" ), # to compute MC-truth info
                                   SystLabel      = cms.string(""), # used by systematics machinery
                                   
                                   VetoConeSize   = cms.double(0.4),
                                   MinLeadPhoPt   = cms.double(1./3.),
                                   MinSubleadPhoPt   = cms.double(0.25),
                                   ScalingPtCuts = cms.bool(True),
                                   DoSigmaMDecorr =cms.untracked.uint32(1),#transformation of sigmaM/M
                                   SigmaMDecorrFile = cms.untracked.FileInPath("flashgg/Taggers/data/diphoMVA_sigmaMoMdecorr_split_Mgg40_180.root"),
                                   ApplyEGMPhotonID = cms.untracked.bool(True),
                                   PhotonIDCut = cms.double(0.2),#this is loose id for 2016
                                   PhotonElectronVeto =cms.untracked.vint32(1, 1), #0: Pho1, 1: Pho2

                                   MinJetPt   = cms.double(20.),
                                   MaxJetEta   = cms.double(2.5),
                                   MJJBoundaries = cms.vdouble(70.,190.),
                                   BTagType = cms.untracked.string('pfCombinedInclusiveSecondaryVertexV2BJetTags'), #string for btag algorithm
                                   UseJetID = cms.bool(True),
                                   JetIDLevel = cms.string('Loose'),

                                   MVABoundaries  = cms.vdouble(0.271,0.543, 0.740), # category boundaries for MVA
                                   MXBoundaries   = cms.vdouble(250., 341.4, 426.1, 544.), # .. and MX
                                   MVAConfig = cms.PSet(variables=cms.VPSet(), # variables are added below
                                                        classifier=cms.string("BDT::bdt"), # classifier name
                                                        weights=cms.FileInPath("flashgg/bbggTools/data/MVA2017/allMC_resWeighting_F_noDR_minDRGJet_edited.weights.xml"), # path to TMVA weights
                                                        regression=cms.bool(False), # this is not a regression
                                                        ),
                                   doMVAFlattening=cms.bool(True),#do transformation of cumulative to make it flat
                                   MVAFlatteningFileName=cms.untracked.FileInPath("flashgg/bbggTools/data/MVA2017/cumulativeTransformation_output_GluGluToHHTo2B2G_node_SM_13TeV-madgraph.root")#FIXME, this should be optional, is it?
                                  ) 

cfgTools.addVariables(flashggDoubleHTag.MVAConfig.variables,
                      # here the syntax is VarNameInTMVA := expression
                      ["leadingJet_bDis := leadJet().bDiscriminator('pfCombinedInclusiveSecondaryVertexV2BJetTags')",#FIXME make the btag type configurable?
                       "subleadingJet_bDis := subleadJet().bDiscriminator('pfCombinedInclusiveSecondaryVertexV2BJetTags')",
                       "fabs(CosThetaStar_CS) := abs(getCosThetaStar_CS(6500))",#FIXME get energy from somewhere?
                       "fabs(CosTheta_bb) := abs(CosThetaAngles()[1])",
                       "fabs(CosTheta_gg) := abs(CosThetaAngles()[0])",
                       "diphotonCandidate.Pt()/(diHiggsCandidate.M()) := diphotonPtOverM()",
                       "dijetCandidate.Pt()/(diHiggsCandidate.M()) := dijetPtOverM()",
                       "customLeadingPhotonIDMVA := diPhoton.leadingView.phoIdMvaWrtChosenVtx",
                       "customSubLeadingPhotonIDMVA := diPhoton.subLeadingView.phoIdMvaWrtChosenVtx",
                       "leadingPhotonSigOverE := diPhoton.leadingPhoton.sigEOverE",
                       "subleadingPhotonSigOverE := diPhoton.subLeadingPhoton.sigEOverE",
                       "sigmaMOverMDecorr := getSigmaMDecorr()",
                       "PhoJetMinDr := getPhoJetMinDr()",
                       ]
                      )


