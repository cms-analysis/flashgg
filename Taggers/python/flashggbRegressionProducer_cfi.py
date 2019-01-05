import FWCore.ParameterSet.Config as cms

from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag
from flashgg.Taggers.flashggTags_cff import flashggUnpackedJets

import os
import flashgg.Systematics.settings as settings

year = settings.year
bRegressionWeightfile_str = cms.untracked.string(os.environ["CMSSW_BASE"]+"/src/flashgg/Taggers/data/DNN_models/model-18.pb")
y_mean_str = cms.untracked.double(1.0454729795455933)
y_std_str = cms.untracked.double( 0.31628304719924927) 
year_str = cms.untracked.string(year)
if year == "2016":
   bRegressionWeightfile_str = cms.untracked.string(os.environ["CMSSW_BASE"]+"/src/flashgg/Taggers/data/DNN_models/model-18.pb")
   y_mean_str = cms.untracked.double(1.0454729795455933)
   y_std_str = cms.untracked.double( 0.31628304719924927)
   year_str = cms.untracked.string(year)
elif year == "2017":
   bRegressionWeightfile_str = cms.untracked.string(os.environ["CMSSW_BASE"]+"/src/flashgg/Taggers/data/DNN_models/breg_training_2017_updated.pb")
   y_mean_str = cms.untracked.double(1.0596693754196167)
   y_std_str = cms.untracked.double(0.28492164611816406)
   year_str = cms.untracked.string(year)

 
flashggbRegressionProducer= cms.EDProducer('flashggbRegressionProducer',
                                           JetTag=cms.InputTag("flashggUnpackedJets","0"),
                                           rhoFixedGridCollection = cms.InputTag('fixedGridRhoFastjetAll'),
                                           bRegressionWeightfile= bRegressionWeightfile_str, 
                                           y_mean = y_mean_str ,
                                           y_std = y_std_str,
                                           year = year_str
                                           )

