import FWCore.ParameterSet.Config as cms

from flashgg.Taggers.bRegressionDumpConfig_cff import bRegressionDumpConfig

bRegressionDumper = cms.EDAnalyzer('CutBasedbRegressionDumper',
                                   **bRegressionDumpConfig.parameters_()
                               )
