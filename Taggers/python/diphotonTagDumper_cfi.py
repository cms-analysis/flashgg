import FWCore.ParameterSet.Config as cms

from flashgg.Taggers.diphotonDumpConfig_cff import diphotonDumpConfig

#diphotonDumper = cms.EDAnalyzer('CutBasedDiPhotonDumper',
diphotonDumper = cms.EDAnalyzer('DiPhotonTagDumper',
                                **diphotonDumpConfig.parameters_()
                                )

