import FWCore.ParameterSet.Config as cms

###################################################################
## ID MODULES
###################################################################

from PhysicsTools.SelectorUtils.tools.vid_id_tools import *

def setIDs(process, options):

    dataFormat = DataFormat.MiniAOD
    if (options['useAOD']):
        dataFormat = DataFormat.AOD
        
    switchOnVIDPhotonIdProducer(process, dataFormat)
        
    # define which IDs we want to produce
    my_id_modules = ['RecoEgamma.PhotonIdentification.Identification.cutBasedPhotonID_Spring15_50ns_V1_cff',
                     'RecoEgamma.PhotonIdentification.Identification.mvaPhotonID_Spring15_50ns_nonTrig_V2_cff']
                 
    for idmod in my_id_modules:
        setupAllVIDIdsInModule(process, idmod, setupVIDPhotonSelection)

    process.goodPhotonsPROBECutBasedLoose = cms.EDProducer("PatPhotonSelectorByValueMap",
                                                           input     = cms.InputTag("goodPhotons"),
                                                           cut       = cms.string(options['PHOTON_CUTS']),
                                                           selection = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-Spring15-50ns-V1-standalone-loose"),
                                                           id_cut    = cms.bool(True)
                                                           )

    process.goodPhotonsPROBECutBasedMedium = process.goodPhotonsPROBECutBasedLoose.clone()
    process.goodPhotonsPROBECutBasedMedium.selection = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-Spring15-50ns-V1-standalone-medium")
    process.goodPhotonsPROBECutBasedTight = process.goodPhotonsPROBECutBasedLoose.clone()
    process.goodPhotonsPROBECutBasedTight.selection = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-Spring15-50ns-V1-standalone-tight")
    
    process.goodPhotonsTAGCutBasedLoose = cms.EDProducer("PatPhotonSelectorByValueMap",
                                                         input     = cms.InputTag("goodPhotons"),
                                                         cut       = cms.string(options['PHOTON_TAG_CUTS']),
                                                         selection = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-Spring15-50ns-V1-standalone-loose"),
                                                         id_cut    = cms.bool(True)
                                                         )
    
    process.goodPhotonsTAGCutBasedMedium = process.goodPhotonsTAGCutBasedLoose.clone()
    process.goodPhotonsTAGCutBasedMedium.selection = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-Spring15-50ns-V1-standalone-medium")
    process.goodPhotonsTAGCutBasedTight = process.goodPhotonsTAGCutBasedLoose.clone()
    process.goodPhotonsTAGCutBasedTight.selection = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-Spring15-50ns-V1-standalone-tight")
