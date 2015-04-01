import FWCore.ParameterSet.Config as cms
from flashgg.Taggers.VBFDiPhoDiJetMVATrainingDumpConf_cff import VBFDiPhoDiJetMVATrainingDumpConf
VBFDiPhoDiJetMVADumper = cms.EDAnalyzer('CutBasedVBFDiPhoDiJetMVAResultDumper',
**VBFDiPhoDiJetMVATrainingDumpConf.parameters_()
)
