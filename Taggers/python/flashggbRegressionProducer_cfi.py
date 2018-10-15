import FWCore.ParameterSet.Config as cms

from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag
from flashgg.Taggers.flashggTags_cff import flashggUnpackedJets

#recoJetCollections = UnpackedJetCollectionVInputTag

#print recoJetCollections

#for icoll,coll in enumerate(recoJetCollections):
flashggbRegressionProducer= cms.EDProducer('flashggbRegressionProducer',
#                                               JetTag=coll,
                                           JetTag=cms.InputTag("flashggUnpackedJets","0"),
                                           bRegressionWeightfile= cms.untracked.string("flashgg/Taggers/data/DNN_models/model-18"), 
                                           y_mean = cms.untracked.double(1.0454729795455933),
                                           y_std =cms.untracked.double( 0.31628304719924927),
                                           #For now had to implement it like this, will unfo later when flashgg new method is ready
                                           bRegressionWeightfile_2017= cms.untracked.string("flashgg/Taggers/data/DNN_models/breg_training_2017.pb"), 
                                           y_mean_2017 = cms.untracked.double(1.0610932111740112), 
                                           y_std_2017 =cms.untracked.double(0.39077115058898926) 
                                           )

