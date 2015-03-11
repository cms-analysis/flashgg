import FWCore.ParameterSet.Config as cms
from flashgg.TagAlgos.diphotonMVAWithZeeDumpConfig_cff import DiPhotonMVAWithZeeDumpConfig

DiPhotonWithZeeMVADumper = cms.EDAnalyzer('CutBasedDiPhotonMVAResultDumper',
                                         **DiPhotonMVAWithZeeDumpConfig.parameters_()
)
