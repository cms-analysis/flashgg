import FWCore.ParameterSet.Config as cms

from flashgg.TagAlgos.diphotonDumpConfig_cff import diphotonDumpConfig

diphotonDumper = cms.EDAnalyzer('CutBasedDiPhotonDumper',
                                **diphotonDumpConfig.parameters_()
                                )


