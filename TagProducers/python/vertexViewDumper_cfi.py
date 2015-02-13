import FWCore.ParameterSet.Config as cms

from flashgg.TagAlgos.vertexViewDumpConfig_cff import vertexViewDumpConfig

vertexViewDumper = cms.EDAnalyzer('CutBasedSingleVertexViewDumper',
                                  **vertexViewDumpConfig.parameters_()
                                  )


