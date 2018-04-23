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

                                   MVABoundaries  = cms.vdouble(-10, 10.), # category boundaries for MVA
                                   MXBoundaries   = cms.vdouble(-1e+9, 1e+9), # .. and MX
                                   MVAConfig = cms.PSet(variables=cms.VPSet(), # variables are added below
                                                        classifier=cms.string("BDT::bdt"), # classifier name
                                                        weights=cms.FileInPath("flashgg/bbggTools/data/MVA2017/allMC_resWeighting_F_noDR_minDRGJet_edited.weights.xml"), # path to TMVA weights
                                                        regression=cms.bool(False), # this is not a regression
                                                        ),
                                  ) 

cfgTools.addVariables(flashggDoubleHTag.MVAConfig.variables,
                      # here the syntax is VarNameInTMVA := expression
                      ["leadingJet_bDis := leadJet().bDiscriminator('pfCombinedInclusiveSecondaryVertexV2BJetTags')",#FIXME make the btag type configurable?
                       "subleadingJet_bDis := subleadJet().bDiscriminator('pfCombinedInclusiveSecondaryVertexV2BJetTags')",
                       "fabs(CosThetaStar_CS) := abs(getCosThetaStar_CS(6500))",#FIXME get energy from somewhere?
                       "fabs(CosTheta_bb) := 0.8",
                       "fabs(CosTheta_gg) := 0.8",
#                       "diphotonCandidate.Pt()/(diHiggsCandidate.M()) := diPhoton.pt / p4.mass",
                       "diphotonCandidate.Pt()/(diHiggsCandidate.M()) := diphotonPtOverM()",
                       "dijetCandidate.Pt()/(diHiggsCandidate.M()) := dijetPtOverM()",
                       "customLeadingPhotonIDMVA := diPhoton.leadingView.phoIdMvaWrtChosenVtx",
                       "customSubLeadingPhotonIDMVA := diPhoton.subLeadingView.phoIdMvaWrtChosenVtx",
                       "leadingPhotonSigOverE := diPhoton.leadingPhoton.sigEOverE",
                       "subleadingPhotonSigOverE := diPhoton.subLeadingPhoton.sigEOverE",
                       "sigmaMOverMDecorr := sqrt(0.5*(diPhoton.leadingPhoton.sigEOverE*diPhoton.leadingPhoton.sigEOverE + diPhoton.subLeadingPhoton.sigEOverE*diPhoton.subLeadingPhoton.sigEOverE))",
                       "PhoJetMinDr := 0.4",
                       ]

                      ## [ "leadPtOverM := diPhoton.leadingPhoton.pt / diphoton.mass", 
                      ##   "subleadPtOverM := diPhoton.subleadingPhoton.pt / diphoton.mass",
                      ##   ],
                      )


