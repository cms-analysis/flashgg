import FWCore.ParameterSet.Config as cms

from flashgg.Taggers.photonDumpConfig_cff import photonDumpConfig

photonDumper = cms.EDAnalyzer('CutBasedPhotonDumper',
                                **photonDumpConfig.parameters_()
                                )


