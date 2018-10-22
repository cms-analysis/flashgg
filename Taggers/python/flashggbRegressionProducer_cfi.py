import FWCore.ParameterSet.Config as cms

from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag
from flashgg.Taggers.flashggTags_cff import flashggUnpackedJets

import os
#recoJetCollections = UnpackedJetCollectionVInputTag

#print recoJetCollections

#for icoll,coll in enumerate(recoJetCollections):
flashggbRegressionProducer= cms.EDProducer('flashggbRegressionProducer',
#                                               JetTag=coll,
                                           JetTag=cms.InputTag("flashggUnpackedJets","0"),
                                           rhoFixedGridCollection = cms.InputTag('fixedGridRhoFastjetAll'),
                                           bRegressionWeightfile= cms.untracked.string(os.environ["CMSSW_BASE"]+"flashgg/Taggers/data/DNN_models/model-18"), 
                                           y_mean = cms.untracked.double(1.0454729795455933),
                                           y_std =cms.untracked.double( 0.31628304719924927),
                                           #For now had to implement it like this, will unfo later when flashgg new method is ready
                                           bRegressionWeightfile_2017= cms.untracked.string(os.environ["CMSSW_BASE"]+"/src/flashgg/Taggers/data/DNN_models/breg_training_2017_updated.pb"), 
                                           y_mean_2017 = cms.untracked.double(1.0596693754196167), 
                                           y_std_2017 =cms.untracked.double(0.28492164611816406) 
                                           )

