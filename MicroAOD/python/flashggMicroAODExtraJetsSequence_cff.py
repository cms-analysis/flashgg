import FWCore.ParameterSet.Config as cms
from flashgg.MicroAOD.flashggExtraJets_cfi import flashggJetsPF, flashggJetsPFCHS0 

from flashgg.MicroAOD.flashggMicroAODGenSequence_cff import *

flashggMicroAODExtraJetsSequence = cms.Sequence( flashggJetsPF + flashggJetsPFCHS0)
