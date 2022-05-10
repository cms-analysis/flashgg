import FWCore.ParameterSet.Config as cms

from flashgg.MicroAOD.pfSVFlavourTagInfos_cfi import pfSVFlavourTagInfos
# from RecoBTag.ONNXRuntime.boostedJetONNXJetTagsProducer_cfi import boostedJetONNXJetTagsProducer
from flashgg.MicroAOD.pfSVFlavourONNXTagsProducer_cfi import pfSVFlavourONNXTagsProducer

flashggSVFlavourTagInfos = pfSVFlavourTagInfos.clone(
    deltar_match_sv_pfcand = 0.4,
    pf_candidates = "packedPFCandidates",
    secondary_vertices = "slimmedSecondaryVertices",
    vertices = "offlineSlimmedPrimaryVertices",
    debugMode = False,
)

flashggSVs =  pfSVFlavourONNXTagsProducer.clone(
    src = 'flashggSVFlavourTagInfos',
    phantom_jets = cms.InputTag('flashggSVFlavourTagInfos', 'svPhantomJets'), # a dummy jet selection for easy implementation
    secondary_vertices = 'slimmedSecondaryVertices',
    preprocess_json = 'flashgg/MicroAOD/data/ParticleNetSV/V01/preprocess_corr.json',
    model_path = 'flashgg/MicroAOD/data/ParticleNetSV/V01/model.onnx',
    flav_names = ['probb', 'probc', 'probcfromb', 'probl'],
    debugMode = False,
)
