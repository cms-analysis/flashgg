import FWCore.ParameterSet.Config as cms
from flashgg.Taggers.flashggTags_cff import HTXSInputTags

flashggTagWeighter = cms.EDProducer('FlashggTagWeighter',
                                  diphotonTag = cms.InputTag('flashggTagSorter'),
                                  tagTruth = cms.InputTag('flashggTagSorter'),
                                  HTXSTags = HTXSInputTags,
                                  isGluonFusion = cms.bool(False),
                                  NNLOPSWeightFile = cms.FileInPath("flashgg/Taggers/data/NNLOPS_reweight.root"),
                                  #reweighGGHforNNLOPS = cms.untracked.bool(bool(customize.processId.count("ggh"))),
                                  applyNNLOPSweight = cms.bool(False),
                                  debug = cms.untracked.bool(False)
                                  )
