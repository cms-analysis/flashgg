import FWCore.ParameterSet.Config as cms

from flashgg.MicroAOD.flashggHiggsBJets_cfi import flashggHiggsBJets
from flashgg.MicroAOD.flashggGenDiPhotonDiBJets_cfi import flashggGenDiPhotonDiBJets
from flashgg.MicroAOD.flashggSelectedGenDiPhotons_cfi import flashggSelectedGenDiPhotons

flashggSelectedGenDiPhotonDiBJets = flashggSelectedGenDiPhotons.clone(
    src = "flashggGenDiPhotonDiBJets",
    ## cut = "abs(mass-125.)<0.1"
    cut = "leadingPhoton.fromHardProcessFinalState && subLeadingPhoton.fromHardProcessFinalState"
)

flashggGenDiPhotonDiBJetsSequence = cms.Sequence(flashggHiggsBJets*flashggGenDiPhotonDiBJets*flashggSelectedGenDiPhotonDiBJets)
