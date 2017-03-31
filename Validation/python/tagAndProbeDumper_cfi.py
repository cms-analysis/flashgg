import FWCore.ParameterSet.Config as cms

from flashgg.Validation.tagAndProbeDumpConfig_cff import tagAndProbeDumpConfig

tagAndProbeDumper = cms.EDAnalyzer('CutBasedTagAndProbeDumper',
                                   **tagAndProbeDumpConfig.parameters_()
                               )
