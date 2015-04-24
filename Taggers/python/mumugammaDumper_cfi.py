#-----------J. Tao from IHEP-Beijing--------------

import FWCore.ParameterSet.Config as cms

from flashgg.Taggers.mumugammaDumpConfig_cff import mumugammaDumpConfig

mumugammaDumper = cms.EDAnalyzer('CutBasedMuMuGammaDumper',
                                **mumugammaDumpConfig.parameters_()
                                )


