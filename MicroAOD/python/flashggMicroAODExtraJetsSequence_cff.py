import FWCore.ParameterSet.Config as cms

from flashgg.MicroAOD.flashggExtraJets_cfi import addFlashggPF#, addFlashggPFCHS0
from flashgg.MicroAOD.flashggMicroAODGenSequence_cff import *




flashggMicroAODExtraJetsSequence = cms.Sequence()# addFlashggPF + addFlashggPFCHS0)

# test the sequence parcing
addFlashggPF     (flashggMicroAODExtraJetsSequence,True)
#addFlashggPFCHS0 (flashggMicroAODExtraJetsSequence,True)

