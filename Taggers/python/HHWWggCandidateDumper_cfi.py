import FWCore.ParameterSet.Config as cms

from flashgg.Taggers.HHWWggCandidateDumpConfig_cff import HHWWggCandidateDumpConfig

HHWWggCandidateDumper = cms.EDAnalyzer('CutBasedHHWWggCandidateDumper',
                                    **HHWWggCandidateDumpConfig.parameters_() 
                                    )
