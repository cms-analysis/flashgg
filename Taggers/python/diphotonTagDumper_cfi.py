import FWCore.ParameterSet.Config as cms

from flashgg.Taggers.tagsDumpConfig_cff import tagsDumpConfig

tagsDumper = cms.EDAnalyzer('DiPhotonTagDumper',
                            **tagsDumpConfig.parameters_()
                            )

