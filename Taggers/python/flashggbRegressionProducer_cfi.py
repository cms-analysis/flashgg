import FWCore.ParameterSet.Config as cms

from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag
from flashgg.MicroAOD.flashggJets_cfi import  maxJetCollections

import os

bRegressionWeightfile_str = cms.untracked.string(os.environ["CMSSW_BASE"]+"/src/flashgg/Taggers/data/DNN_models/model-18.pb")
y_mean_str = cms.untracked.double(1.0454729795455933)
y_std_str = cms.untracked.double( 0.31628304719924927) 
year_str = cms.untracked.string("2016")

jetnames = cms.vstring()
for icoll,coll in enumerate(UnpackedJetCollectionVInputTag):
    jetnames.append(coll.moduleLabel)

 
flashggbRegressionProducer= cms.EDProducer('flashggbRegressionProducer',
                                           JetNames = jetnames, # one jet collection per vertex
                                           JetSuffixes = cms.vstring(''), #nominal and systematic variations 
                                           rhoFixedGridCollection = cms.InputTag('fixedGridRhoFastjetAll'),
                                           bRegressionWeightfile= bRegressionWeightfile_str, 
                                           y_mean = y_mean_str ,
                                           y_std = y_std_str,
                                           year = year_str
                                           )

