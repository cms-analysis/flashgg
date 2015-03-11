import FWCore.ParameterSet.Config as cms

from flashgg.MicroAOD.flashggMicroAODSequence_cff import *
from flashgg.MicroAOD.flashggHLTDiPhotons_cfi import flashggHLTDiPhotons

flashggMicroAODHLTEfficiencySequence = cms.Sequence(flashggMicroAODSequence)
flashggMicroAODHLTEfficiencySequence.remove(flashggPreselectedDiPhotons)
flashggMicroAODHLTEfficiencySequence.remove(flashggJets)
flashggMicroAODHLTEfficiencySequence += (flashggHLTDiPhotons) 

