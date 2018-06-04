import FWCore.ParameterSet.Config as cms

from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag
from flashgg.Taggers.flashggTags_cff import flashggUnpackedJets

#recoJetCollections = UnpackedJetCollectionVInputTag

#print recoJetCollections

#for icoll,coll in enumerate(recoJetCollections):
flashggbRegressionProducer80= cms.EDProducer('flashggbRegressionProducer80',
#                                               JetTag=coll,
                                           JetTag=cms.InputTag("flashggUnpackedJets","0"),
                                           bRegressionWeightfile= cms.untracked.string("flashgg/MetaData/data/DNN_models/model-18"), 
                                           y_mean = cms.untracked.double(1.0454729795455933),
                                           y_std =cms.untracked.double( 0.31628304719924927)
                                           )

