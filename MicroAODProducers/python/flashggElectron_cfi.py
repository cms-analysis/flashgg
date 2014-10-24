import FWCore.ParameterSet.Config as cms

from EgammaAnalysis.ElectronTools.electronIdMVAProducer_cfi import mvaNonTrigV0 

flashggElectrons = cms.EDProducer('FlashggElectronProducer',
		VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),

		ElectronTag=cms.untracked.InputTag('slimmedElectrons')

		ElctronNonTrigMva=cms.PSet(mvaNonTrigV0)

		)
