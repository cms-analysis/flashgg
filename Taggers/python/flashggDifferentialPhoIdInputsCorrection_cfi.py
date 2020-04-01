import FWCore.ParameterSet.Config as cms

from RecoEgamma.EgammaTools.regressionModifier_cfi import regressionModifier 
from flashgg.Taggers.globalVariables_cff import globalVariables
from flashgg.Taggers.flashggTags_cff import *
flashggDifferentialPhoIdInputsCorrection = cms.EDProducer("FlashggDifferentialPhoIdInputsCorrector",
                                                          diphotonSrc              = cms.InputTag("flashggDiPhotons"),
                                                          rhoFixedGridCollection   = cms.InputTag('fixedGridRhoAll'),
                                                          globalVariables=globalVariables,
                                                          
                                                          # photonIdMVAweightfile_EB = cms.FileInPath("flashgg/MicroAOD/data/HggPhoId_94X_barrel_BDT_woisocorr.weights.xml"),
                                                          # photonIdMVAweightfile_EE = cms.FileInPath("flashgg/MicroAOD/data/HggPhoId_94X_endcap_BDT_woisocorr.weights.xml"),
                                                          
                                                          # effAreasConfigFile = cms.FileInPath("RecoEgamma/PhotonIdentification/data/Spring16/effAreaPhotons_cone03_pfPhotons_90percentBased.txt"),
                                                          phoIsoPtScalingCoeff = cms.vdouble(0.0053,0.0034),
                                                          phoIsoCutoff = cms.double(2.5),

                                                          ## Shower shape correction (5x5)
                                                          reRunRegression = cms.bool(False),
                                                          regressionConfig = regressionModifier.clone(manualBunchSpacing = cms.int32(25),              
                                                                                                      rhoCollection = cms.InputTag("fixedGridRhoFastjetAll"),
                                                                                                      vertexCollection = cms.InputTag("offlineSlimmedPrimaryVertices"),
                                                                                                      autoDetectBunchSpacing = cms.bool(False)
                                                                                                  ),
                                                          correctShowerShapes = cms.bool(True),
                                                          correctIsolations = cms.bool(True),

                                                          phoIso_corrector_config_EB = cms.PSet(),
                                                          phoIso_corrector_config_EE = cms.PSet(),
                                                          chIso_corrector_config_EB = cms.PSet(),
                                                          chIso_corrector_config_EE = cms.PSet(),
                                                          # Helper for phoIDMva `True` for 2017, 2018, `False` for 2016
                                                          # is2017 = cms.bool(True)
)


import flashgg.Taggers.dumperConfigTools as cfgTools
import json
import os

def setup_flashggDifferentialPhoIdInputsCorrection( process, metaConditions ):

    process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
                                                       flashggDifferentialPhoIdInputsCorrection=cms.PSet(
                                                           initialSeed=cms.untracked.uint32(
                                                               90)
                                                       )
                                                   )

    process.load("flashgg.Taggers.flashggDifferentialPhoIdInputsCorrection_cfi")
    # print(process)
    
    setattr(flashggDifferentialPhoIdInputsCorrection, "photonIdMVAweightfile_EB", cms.FileInPath(str(metaConditions['flashggPhotons']['photonIdMVAweightfile_EB'])))
    setattr(flashggDifferentialPhoIdInputsCorrection, "photonIdMVAweightfile_EE", cms.FileInPath(str(metaConditions['flashggPhotons']['photonIdMVAweightfile_EE'])))
    setattr(flashggDifferentialPhoIdInputsCorrection, "effAreasConfigFile", cms.FileInPath(str(metaConditions['flashggPhotons']['effAreasConfigFile'])))
    setattr(flashggDifferentialPhoIdInputsCorrection, "is2017", cms.bool(metaConditions['flashggPhotons']['is2017']))
    corrections_summary = {}
    with open(os.path.expandvars('$CMSSW_BASE/src/'+metaConditions['PhoIdInputCorrections']['corrections_summary'])) as json_file:
        corrections_summary = json.load(json_file)

        #---Shower shapes
    for var in corrections_summary['shower_shapes'].keys():
        for subdet in ['EB', 'EE']:
            ss_summary = corrections_summary['shower_shapes'][var][subdet]
            setattr(flashggDifferentialPhoIdInputsCorrection, var+'_corrector_config_'+subdet, 
                    cms.PSet(
                        variables = cms.VPSet(),
                        weights = cms.FileInPath(str(ss_summary['weights'])),
                        regr_output_scaling = cms.string('x[0]*(%f)+(%f)' % (ss_summary['scale'], ss_summary['center'])),
                        regression = cms.bool(True),
                        classifier = cms.string('BDTG_{}_{}'.format(var, subdet))
                    )
                )        
            xgb_config = getattr(flashggDifferentialPhoIdInputsCorrection, var+'_corrector_config_'+subdet)
            # print(list(metaConditions['PhoIdInputCorrections']['SS_variables']))
            cfgTools.addVariables(xgb_config.variables, [str(st) for st in metaConditions['PhoIdInputCorrections']['SS_variables']])
                                  # ['f0 := pt',
                               #     'f1 := superCluster.eta',
                               #     'f2 := phi',
                               #     'f3 := global.rho',
                               #     'f4 := sieip',
                               #     'f5 := s4',
                               #     'f6 := full5x5_r9',
                               #     'f7 := superCluster.phiWidth',
                               #     'f8 := full5x5_sigmaIetaIeta',
                               #     'f9 := superCluster.etaWidth'
                               # ]
                               #    )
                                   

        #---Isolations
    for subdet in ['EB', 'EE']:
        #-photon isolation
        iso_summary = corrections_summary['isolations']['phoIso'][subdet]
        setattr(flashggDifferentialPhoIdInputsCorrection, 'phoIso_corrector_config_'+subdet, 
                cms.PSet(
                    clf_data = cms.PSet(
                        variables = cms.VPSet(),
                        weights = cms.FileInPath(str(iso_summary['peak_tail_clfs']['weights_data'])),
                        classifier = cms.string('BDTG_{}_{}_p0t_data'.format("phoIso", subdet))
                    ),
                    clf_mc = cms.PSet(
                        variables = cms.VPSet(),
                        weights = cms.FileInPath(str(iso_summary['peak_tail_clfs']['weights_mc'])),
                        classifier = cms.string('BDTG_{}_{}_p0t_mc'.format("phoIso", subdet))
                    ),
                    peak2tail = cms.PSet(
                        variables = cms.VPSet(),
                        weights = cms.FileInPath(str(iso_summary['peak2tail']['weights'])),
                        regression = cms.bool(True),
                        classifier = cms.string('BDTG_{}_{}_p2t'.format("phoIso", subdet))
                    ),
                    morphing = cms.PSet(
                        variables = cms.VPSet(),
                        weights = cms.FileInPath(str(iso_summary['morphing']['weights'])),
                        regression = cms.bool(True),
                        classifier = cms.string('BDTG_{}_{}'.format("phoIso", subdet)),
                        regr_output_scaling = cms.string('x[0]*(%f)+(%f)' % (iso_summary['morphing']['scale'], iso_summary['morphing']['center']))
                    )
                )
        )
        phoIso_corrector_config = getattr(flashggDifferentialPhoIdInputsCorrection, 'phoIso_corrector_config_'+subdet)
        cfgTools.addVariables(phoIso_corrector_config.clf_data.variables, [str(x) for x in iso_summary['peak_tail_clfs']['variables']])
        cfgTools.addVariables(phoIso_corrector_config.clf_mc.variables, [str(x) for x in iso_summary['peak_tail_clfs']['variables']])
        cfgTools.addVariables(phoIso_corrector_config.peak2tail.variables, [str(x) for x in iso_summary['peak2tail']['variables']])
        cfgTools.addVariables(phoIso_corrector_config.morphing.variables, [str(x) for x in iso_summary['morphing']['variables']])    

        #-charged isolations
        iso_summary = corrections_summary['isolations']['chIso'][subdet]
        setattr(flashggDifferentialPhoIdInputsCorrection, 'chIso_corrector_config_'+subdet, 
                cms.PSet(
                    clf_data = cms.PSet(
                        variables = cms.VPSet(),
                        weights = cms.FileInPath(str(iso_summary['peak_tail_clfs']['weights_data'])),
                        multiclass = cms.bool(True),
                        classifier = cms.string('BDTG_{}_{}_3Cat_data'.format("chIso", subdet))
                    ),
                    clf_mc = cms.PSet(
                        variables = cms.VPSet(),
                        weights = cms.FileInPath(str(iso_summary['peak_tail_clfs']['weights_mc'])),
                        multiclass = cms.bool(True),
                        classifier = cms.string('BDTG_{}_{}_3Cat_mc'.format("chIso", subdet))
                    ),
                    chIso_peak2tail = cms.PSet(
                        variables = cms.VPSet(),
                        weights = cms.FileInPath(str(iso_summary['chIso_peak2tail']['weights'])),
                        regression = cms.bool(True),
                        classifier = cms.string('BDTG_{}_{}_p2t'.format("chIso", subdet))
                    ),
                    chIso_morphing = cms.PSet(
                        variables = cms.VPSet(),
                        weights = cms.FileInPath(str(iso_summary['chIso_morphing']['weights'])),
                        classifier = cms.string('BDTG_{}_{}'.format("chIso", subdet)),
                        regression = cms.bool(True),
                        regr_output_scaling = cms.string('x[0]*(%f)+(%f)' % (iso_summary['chIso_morphing']['scale'], 
                                                                             iso_summary['chIso_morphing']['center']))
                    ),
                    chIsoWorst_peak2tail = cms.PSet(
                        variables = cms.VPSet(),
                        weights = cms.FileInPath(str(iso_summary['chIsoWorst_peak2tail']['weights'])),
                        regression = cms.bool(True),
                        classifier = cms.string('BDTG_{}_{}_p2t'.format("chIsoWorst", subdet))
                    ),
                    chIsoWorst_morphing = cms.PSet(
                        variables = cms.VPSet(),
                        weights = cms.FileInPath(str(iso_summary['chIsoWorst_morphing']['weights'])),
                        classifier = cms.string('BDTG_{}_{}'.format("chIsoWorst", subdet)),
                        regression = cms.bool(True),
                        regr_output_scaling = cms.string('x[0]*(%f)+(%f)' % (iso_summary['chIsoWorst_morphing']['scale'], 
                                                                             iso_summary['chIsoWorst_morphing']['center']))
                    )
                )
            )
        chIso_corrector_config = getattr(flashggDifferentialPhoIdInputsCorrection, 'chIso_corrector_config_'+subdet)
        cfgTools.addVariables(chIso_corrector_config.clf_data.variables, [str(x) for x in iso_summary['peak_tail_clfs']['variables']])
        cfgTools.addVariables(chIso_corrector_config.clf_mc.variables, [str(x) for x in iso_summary['peak_tail_clfs']['variables']])
        cfgTools.addVariables(chIso_corrector_config.chIso_peak2tail.variables, [str(x) for x in iso_summary['chIso_peak2tail']['variables']])
        cfgTools.addVariables(chIso_corrector_config.chIso_morphing.variables, [str(x) for x in iso_summary['chIso_morphing']['variables']])    
        cfgTools.addVariables(chIso_corrector_config.chIsoWorst_peak2tail.variables, [str(x) for x in iso_summary['chIsoWorst_peak2tail']['variables']])
        cfgTools.addVariables(chIso_corrector_config.chIsoWorst_morphing.variables, [str(x) for x in iso_summary['chIsoWorst_morphing']['variables']])    
    
