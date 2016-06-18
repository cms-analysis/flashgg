import FWCore.ParameterSet.Config as cms
from flashgg.Taggers.DiPhotonMVATrainingDumpConf_cff import DiPhotonMVATrainingDumpConf

DiPhotonMVADumper = cms.EDAnalyzer('CutBasedDiPhotonMVAResultDumper',
                                **DiPhotonMVATrainingDumpConf.parameters_()
                                )

