import FWCore.ParameterSet.Config as cms
from PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMetCorAndUncFromMiniAOD
from PhysicsTools.PatUtils.tools.corMETFromMuonAndEG import corMETFromMuonAndEG


from os import environ
usePrivateSQlite=True

if usePrivateSQlite:
    # from CondCore.DBCommon.CondDBSetup_cfi import *
    from CondCore.CondDB.CondDB_cfi import CondDB as CondDBSetup
    import os

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

def updateMETs(process, options):

    """
    Create slimmedMETsUpdated and flashggMETs collections
    """
    runMetCorAndUncFromMiniAOD(process,
                               isData=(options.processType == "data"),
                               # will produce new MET collection: slimmedMETsUpdated
                               postfix = "Updated",
                           )

    process.flashggMets = cms.EDProducer('FlashggMetProducer',
                                         verbose = cms.untracked.bool(False),
                                         metTag = cms.InputTag('slimmedMETsUpdated'),
    )
    process.flashggMetSequence = cms.Sequence(process.fullPatMetSequenceUpdated * process.flashggMets)

#===========================================================================================================================#

def setMetCorr2016(process):
    from flashgg.MicroAOD.METcorr_multPhiCorr_80X_sumPt_cfi import multPhiCorr_MC_DY_80X    
    process.pfMEtMultShiftCorr.parameters                 = multPhiCorr_MC_DY_80X
    process.patPFMetTxyCorr.parameters                    = multPhiCorr_MC_DY_80X

    
