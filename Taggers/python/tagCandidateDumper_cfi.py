import FWCore.ParameterSet.Config as cms

from flashgg.Taggers.tagCandidateDumpConfig_cff import tagCandidateDumpConfig

tagCandidateDumper = cms.EDAnalyzer('CutBasedTagCandidateDumper',
                                    **tagCandidateDumpConfig.parameters_()
                                    )
