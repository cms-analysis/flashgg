import re

import FWCore.ParameterSet.Config as cms


def getMicroAODHLTFilter(datasetName, options):
    """
    Return a TriggerResultsFilter customized accordingly to the data-taking period.
    The returned module is intended to be used as first step in the MicroAOD production sequence
    """

    for dset, analysisType in options["TriggerPaths"].items():
        hlt_paths = []
        for an in analysisType:
            hlt_paths.extend(  [str(trg) for trg in options["TriggerPaths"][dset][an]] )
        triggerConditions = cms.vstring(hlt_paths)
        if re.search(str(dset), datasetName):
            print 'Only events with {} paths will be processed.'.format(hlt_paths)
            triggerSelectionModule = cms.EDFilter("TriggerResultsFilter",
                                                  triggerConditions = triggerConditions,
                                                  hltResults = cms.InputTag( "TriggerResults", "", "HLT" ),
                                                  l1tResults = cms.InputTag( "" ),
                                                  throw = cms.bool(False)
                                              )
            return triggerSelectionModule

    return None
