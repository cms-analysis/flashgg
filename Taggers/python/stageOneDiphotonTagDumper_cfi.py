import FWCore.ParameterSet.Config as cms

from flashgg.Taggers.tagsDumpConfig_cff import tagsDumpConfig

tagsDumper = cms.EDAnalyzer('StageOneDiPhotonTagDumper',
                            **tagsDumpConfig.parameters_()
                            )

