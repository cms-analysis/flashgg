import FWCore.ParameterSet.Config as cms

from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag

# prefire weight calculator
flashggPrefireWeight = cms.EDProducer('FlashggPrefireWeightProducer',
                               DiPhotonTag=cms.InputTag('flashggPreselectedDiPhotons'),
                               inputTagJets= UnpackedJetCollectionVInputTag,
                               photonFileName = cms.FileInPath("flashgg/Taggers/data/L1prefiring_photonpt_2017BtoF.root"),
                               photonHistName = cms.untracked.string(""),
                               jetFileName = cms.FileInPath("flashgg/Taggers/data/L1prefiring_jetpt_2017BtoF.root"),
                               jetHistName = cms.untracked.string("")
)
