import FWCore.ParameterSet.Config as cms

from flashgg.Taggers.photonjetDumpConfig_cff import photonjetDumpConfig

photonjetDumper = cms.EDAnalyzer('CutBasedPhotonJetDumper',
                                **photonjetDumpConfig.parameters_()
                                )


