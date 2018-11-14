import FWCore.ParameterSet.Config as cms

from flashgg.Taggers.h4gCandidateDumpConfig_cff import h4gCandidateDumpConfig

h4gCandidateDumper = cms.EDAnalyzer('CutBasedH4GCandidateDumper',
                                    **h4gCandidateDumpConfig.parameters_()
                                    )
