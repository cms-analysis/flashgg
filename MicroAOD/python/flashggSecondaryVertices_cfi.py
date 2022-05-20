import FWCore.ParameterSet.Config as cms

from flashgg.MicroAOD.pfSVFlavourTagInfos_cfi import pfSVFlavourTagInfos
# from RecoBTag.ONNXRuntime.boostedJetONNXJetTagsProducer_cfi import boostedJetONNXJetTagsProducer
from flashgg.MicroAOD.pfSVFlavourONNXTagsProducer_cfi import pfSVFlavourONNXTagsProducer

flashggSVFlavourTagInfos = pfSVFlavourTagInfos.clone(
    deltar_match_sv_pfcand = 0.4,
    pf_candidates = cms.InputTag("packedPFCandidates"),
    secondary_vertices = cms.InputTag("slimmedSecondaryVertices"),
    vertices = cms.InputTag("offlineSlimmedPrimaryVertices"),
    bHadrons = cms.InputTag("flashggGenBCHadrons", "bHadrons"),
    cHadrons = cms.InputTag("flashggGenBCHadrons", "cHadrons"),
    debugMode = False,
)

flashggSVs =  pfSVFlavourONNXTagsProducer.clone(
    src = cms.InputTag('flashggSVFlavourTagInfos'),
    phantom_jets = cms.InputTag('flashggSVFlavourTagInfos', 'svPhantomJets'), # a dummy jet selection for easy implementation
    secondary_vertices = cms.InputTag('slimmedSecondaryVertices'),
    preprocess_json = 'flashgg/MicroAOD/data/ParticleNetSV/V02/preprocess_corr.json',
    model_path = 'flashgg/MicroAOD/data/ParticleNetSV/V02/model.onnx',
    flav_names = ['probb', 'probbb', 'probc', 'probcc', 'probunmat'],
    debugMode = False,
)
