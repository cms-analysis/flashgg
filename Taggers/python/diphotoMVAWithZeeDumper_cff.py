import FWCore.ParameterSet.Config as cms
from flashgg.Taggers.diphotonMVAWithZeeDumpConfig_cff import DiPhotonMVAWithZeeDumpConfig

DiPhotonWithZeeMVADumper = cms.EDAnalyzer('CutBasedDiPhotonMVAResultDumper',
                                         **DiPhotonMVAWithZeeDumpConfig.parameters_()
)
