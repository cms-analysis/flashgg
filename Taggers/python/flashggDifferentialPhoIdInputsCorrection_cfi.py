import FWCore.ParameterSet.Config as cms

from RecoEgamma.EgammaTools.regressionModifier_cfi import regressionModifier 
from flashgg.Taggers.globalVariables_cff import globalVariables

flashggDifferentialPhoIdInputsCorrection = cms.EDProducer("FlashggDifferentialPhoIdInputsCorrector",
                                                          diphotonSrc              = cms.InputTag("flashggDiPhotons"),
                                                          rhoFixedGridCollection   = cms.InputTag('fixedGridRhoAll'),
                                                          globalVariables=globalVariables,
                                                          
                                                          photonIdMVAweightfile_EB = cms.FileInPath("flashgg/MicroAOD/data/MVAweights_80X_barrel_ICHEPvtx.xml"),
                                                          photonIdMVAweightfile_EE = cms.FileInPath("flashgg/MicroAOD/data/MVAweights_80X_endcap_ICHEPvtx.xml"),
                                                          

                                                          ## Shower shape correction (5x5)
                                                          reRunRegression = cms.bool(False),
                                                          regressionConfig = regressionModifier.clone(manualBunchSpacing = cms.int32(25),              
                                                                                                      rhoCollection = cms.InputTag("fixedGridRhoFastjetAll"),
                                                                                                      vertexCollection = cms.InputTag("offlineSlimmedPrimaryVertices"),
                                                                                                      autoDetectBunchSpacing = cms.bool(False)
                                                                                                  )
)

import flashgg.Taggers.dumperConfigTools as cfgTools
import json
import os

corrections_summary = {}
with open(os.path.expandvars('$CMSSW_BASE/src/flashgg/Taggers/data/PhoIdInputsCorrections/corrections_summary.json')) as json_file:
    corrections_summary = json.load(json_file)

for var in corrections_summary.keys():
    for subdet in ['EB', 'EE']:
        setattr(flashggDifferentialPhoIdInputsCorrection, var+'_corrector_config_'+subdet, 
                cms.PSet(
                    variables = cms.VPSet(),
                    # classifier = cms.string("BDT"),
                    # regression = cms.bool(True),
                    # multiclass = cms.bool(False),
                    weights = cms.FileInPath(str("flashgg/Taggers/data/PhoIdInputsCorrections/weights_finalRegressor_%s_%s.xgb" % (subdet, var))),
                    regr_output_scaling = cms.string('x[0]*(%f)+(%f)' % (corrections_summary[var][subdet]['scale'], corrections_summary[var][subdet]['center']))
                )
            )        
        xgb_config = getattr(flashggDifferentialPhoIdInputsCorrection, var+'_corrector_config_'+subdet)
        cfgTools.addVariables(xgb_config.variables,
                              ['f0 := pt',
                               'f1 := superCluster.eta',
                               'f2 := phi',
                               'f3 := global.rho',
                               'f4 := sieip',
                               'f5 := s4',
                               'f6 := full5x5_r9',
                               'f7 := superCluster.phiWidth',
                               'f8 := full5x5_sigmaIetaIeta',
                               'f9 := superCluster.etaWidth'
                           ]
        )                        
