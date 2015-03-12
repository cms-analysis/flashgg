import FWCore.ParameterSet.Config as cms

from flashgg.Taggers.vertexViewDumpConfig_cff import vertexViewDumpConfig

vertexViewDumper = cms.EDAnalyzer('CutBasedSingleVertexViewDumper',
                                  **vertexViewDumpConfig.parameters_()
                                  )


