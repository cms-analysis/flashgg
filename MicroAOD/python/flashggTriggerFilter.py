import re

import FWCore.ParameterSet.Config as cms

triggerConditionsDict = {
    ".*EGamma.*2018.*" : cms.vstring(
        'HLT_Diphoton30_22_R9Id_OR_IsoCaloId_AND_HE_R9Id_Mass90_v*',
        'HLT_Diphoton30_22_R9Id_OR_IsoCaloId_AND_HE_R9Id_Mass95_v*',
        'HLT_Diphoton30PV_18PV_R9Id_AND_IsoCaloId_AND_HE_R9Id_PixelVeto_Mass55_v*',
        'HLT_Diphoton30PV_18PV_R9Id_AND_IsoCaloId_AND_HE_R9Id_NoPixelVeto_Mass55_v*',
        'HLT_Diphoton30_18_R9IdL_AND_HE_AND_IsoCaloId_NoPixelVeto_Mass55_v*',
        'HLT_Diphoton30_18_R9IdL_AND_HE_AND_IsoCaloId_NoPixelVeto_v*',
        'HLT_Ele*_WPTight_Gsf_v*'
    ),
    
    ".*DoubleMuon.*2018.*" : cms.vstring(
        'HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v*',
        'HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v*',
        'HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v*',
        'HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v*'
    ),

    ".*DoubleEG.*2017.*" : cms.vstring(
        'HLT_Diphoton30_22_R9Id_OR_IsoCaloId_AND_HE_R9Id_Mass90_v*',
        'HLT_Diphoton30_22_R9Id_OR_IsoCaloId_AND_HE_R9Id_Mass95_v*',
        'HLT_Diphoton30PV_18PV_R9Id_AND_IsoCaloId_AND_HE_R9Id_PixelVeto_Mass55_v*',
        'HLT_Diphoton30PV_18PV_R9Id_AND_IsoCaloId_AND_HE_R9Id_NoPixelVeto_Mass55_v*',
        'HLT_Diphoton30_18_R9IdL_AND_HE_AND_IsoCaloId_NoPixelVeto_Mass55_v*',
        'HLT_Diphoton30_18_R9IdL_AND_HE_AND_IsoCaloId_NoPixelVeto_v*'
    )    
}
    

def getMicroAODHLTFilter(datasetName):
    """
    Return a TriggerResultsFilter customized accordingly to the data-taking period.
    The returned module is inteded to be used as first step in the MicroAOD production sequence
    """

    for tag, triggerConditions in triggerConditionsDict.items():
        if re.search(tag, datasetName):
            print 'Only events from these path will be processed: '
            print triggerConditions
            triggerSelectionModule = cms.EDFilter("TriggerResultsFilter",
                                                  triggerConditions = triggerConditions,
                                                  hltResults = cms.InputTag( "TriggerResults", "", "HLT" ),
                                                  l1tResults = cms.InputTag( "" ),
                                                  throw = cms.bool(False)
                                              )
            return triggerSelectionModule

    return None
