import FWCore.ParameterSet.Config as cms

from flashgg.Taggers.genDiphotonDumpConfig_cff import genDiphotonDumpConfig

genDiphotonDumper = cms.EDAnalyzer('CutBasedGenDiPhotonDumper',
                                **genDiphotonDumpConfig.parameters_()
                                )


