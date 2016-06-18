
import FWCore.ParameterSet.Config as cms

from flashgg.Taggers.DiPhotonMVATrainingDumpConfNew_cff import DiPhotonMVATrainingDumpConfNew

DiPhotonMVADumperNew = cms.EDAnalyzer('CutBasedDiPhotonMVAResultDumper',
                                **DiPhotonMVATrainingDumpConfNew.parameters_()
                                )
