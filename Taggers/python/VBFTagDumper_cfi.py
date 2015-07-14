import FWCore.ParameterSet.Config as cms

#from flashgg.Taggers.diphotonDumpConfig_cff import diphotonDumpConfig
from flashgg.Taggers.tagsDumpConfig_cff import tagsDumpConfig

VBFTagDumper = cms.EDAnalyzer('CutBasedVBFTagDumper',
                                **tagsDumpConfig.parameters_()
                                )

