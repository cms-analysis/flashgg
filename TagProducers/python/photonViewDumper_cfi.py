import FWCore.ParameterSet.Config as cms

from flashgg.TagAlgos.photonDumpConfig_cff import photonDumpConfig

photonViewDumper = cms.EDAnalyzer('CutBasedSinglePhotonViewDumper',
                                  **photonDumpConfig.parameters_()
                                  )


photonViewDumper.className = 'CutBasedSinglePhotonViewDumper'
photonViewDumper.src = "flashggSinglePhotonViews"
