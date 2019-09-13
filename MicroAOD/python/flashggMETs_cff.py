import FWCore.ParameterSet.Config as cms
from PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMetCorAndUncFromMiniAOD
from PhysicsTools.PatUtils.tools.corMETFromMuonAndEG import corMETFromMuonAndEG


from os import environ
usePrivateSQlite=True

if usePrivateSQlite:
    # from CondCore.DBCommon.CondDBSetup_cfi import *
    from CondCore.CondDB.CondDB_cfi import CondDB as CondDBSetup
    import os

# Store a flag in the flashgg MET to tell us if the event passes the ecalBadCalibFilter
# This filter has to be rerun on top of miniAOD. All other MET filters implemented in workspaceStd.py
# Here we follow the recipe from:  https://twiki.cern.ch/twiki/bin/view/CMS/MissingETOptionalFiltersRun2#Analysis_Recommendations_for_ana
def runEcalBadCalibFilter(process, options):
    process.load('RecoMET.METFilters.ecalBadCalibFilter_cfi')
    baddetEcallist = cms.vuint32(
    [872439604,872422825,872420274,872423218,
     872423215,872416066,872435036,872439336,
     872420273,872436907,872420147,872439731,
     872436657,872420397,872439732,872439339,
     872439603,872422436,872439861,872437051,
     872437052,872420649,872422436,872421950,
     872437185,872422564,872421566,872421695,
     872421955,872421567,872437184,872421951,
     872421694,872437056,872437057,872437313]) 
    process.ecalBadCalibReducedMINIAODFilter = cms.EDFilter(
        "EcalBadCalibFilter",
        EcalRecHitSource = cms.InputTag("reducedEgamma:reducedEERecHits"),
        ecalMinEt        = cms.double(50.),
        baddetEcal    = baddetEcallist,
        taggingMode = cms.bool(True),
        debug = cms.bool(False)
    )
    process.p *= process.ecalBadCalibReducedMINIAODFilter
    

#============================================Apply MET correction and syst.=================================================#

def runMETs2016(process, options):
    
    # dBFile = os.path.expandvars(era+".db")
    # print dBFile
    # if usePrivateSQlite:
    #     process.jec = cms.ESSource("PoolDBESSource",
    #                                CondDBSetup,
    #                                connect = cms.string("sqlite_file:"+dBFile),
    #                                toGet =  cms.VPSet(
    #                                    cms.PSet(
    #                                        record = cms.string("JetCorrectionsRecord"),
    #                                        tag = cms.string("JetCorrectorParametersCollection_"+era+"_AK4PF"),
    #                                        label= cms.untracked.string("AK4PF")
    #                                    ),
    #             cms.PSet(
    #                 record = cms.string("JetCorrectionsRecord"),
    #                 tag = cms.string("JetCorrectorParametersCollection_"+era+"_AK4PFchs"),
    #                 label= cms.untracked.string("AK4PFchs")
    #             ),
    #             )
    #                                )
    #     process.es_prefer_jec = cms.ESPrefer("PoolDBESSource",'jec')
    #     #===========================================================================================================================#
    # isMC = False
    # if 'MC' in dBFile:
    #     isMC = True

    ##only corrects JEC/JER uncertainties for the baseline collection; means only muon corrected and EG/Muon corrected MET collections are guaranteed to have the correct JEC uncertainties.
    runMetCorAndUncFromMiniAOD(process, metType="PF",
                               recoMetFromPFCs=False,
                               postfix="",
                               isData=(options.processType == "data"),
                           )

    runEcalBadCalibFilter(process, options)
    process.flashggMets = cms.EDProducer('FlashggMetProducer',
                                         verbose = cms.untracked.bool(False),
                                         metTag = cms.InputTag('slimmedMETs'),
                                     )
        
    process.flashggMetSequence = cms.Sequence(process.fullPatMetSequence*process.flashggMets)       

def runMETs2017(process, options):
    """
    Create slimmedMets and flashggMETs collections
    """
    
    runMetCorAndUncFromMiniAOD(process,
                               isData=(options.processType == "data"),
                               fixEE2017 = True,
                               # will produce new MET collection: slimmedMETsModifiedMET
                               postfix = "",
                           )

    runEcalBadCalibFilter(process, options)
    process.flashggMets = cms.EDProducer('FlashggMetProducer',
                                         verbose = cms.untracked.bool(False),
                                         metTag = cms.InputTag('slimmedMETs'),
    )
    process.flashggMetSequence = cms.Sequence(process.fullPatMetSequence*process.flashggMets)
        
#===========================================================================================================================#

def setMetCorr2016(process):
    from flashgg.MicroAOD.METcorr_multPhiCorr_80X_sumPt_cfi import multPhiCorr_MC_DY_80X    
    process.pfMEtMultShiftCorr.parameters                 = multPhiCorr_MC_DY_80X
    process.patPFMetTxyCorr.parameters                    = multPhiCorr_MC_DY_80X

    
