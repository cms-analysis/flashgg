import FWCore.ParameterSet.Config as cms
from flashgg.Taggers.VBFMVATrainingDumpConf_cff import VBFMVATrainingDumpConf
VBFMVADumper = cms.EDAnalyzer('CutBasedVBFMVAResultDumper',
**VBFMVATrainingDumpConf.parameters_()
)

