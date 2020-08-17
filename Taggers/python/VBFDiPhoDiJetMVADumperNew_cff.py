import FWCore.ParameterSet.Config as cms
from flashgg.Taggers.VBFDiPhoDiJetMVATrainingDumpConfNew_cff import VBFDiPhoDiJetMVATrainingDumpConfNew
VBFDiPhoDiJetMVADumperNew = cms.EDAnalyzer('CutBasedVBFDiPhoDiJetMVAResultDumper',
**VBFDiPhoDiJetMVATrainingDumpConfNew.parameters_()
)
