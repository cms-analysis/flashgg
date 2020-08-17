import FWCore.ParameterSet.Config as cms
from flashgg.Taggers.VBFMVATrainingDumpConfNew_cff import VBFMVATrainingDumpConfNew
VBFMVADumperNew = cms.EDAnalyzer('CutBasedVBFMVAResultDumper',
	**VBFMVATrainingDumpConfNew.parameters_()
	)
