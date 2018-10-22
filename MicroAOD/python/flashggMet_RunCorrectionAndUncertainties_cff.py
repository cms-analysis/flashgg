import FWCore.ParameterSet.Config as cms
from PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMetCorAndUncFromMiniAOD
from PhysicsTools.PatUtils.tools.corMETFromMuonAndEG import corMETFromMuonAndEG


from os import environ
usePrivateSQlite=True

if usePrivateSQlite:
    from CondCore.DBCommon.CondDBSetup_cfi import *
    import os


#============================================Apply MET correction and syst.=================================================#

def runMETs(process,era):
        
    dBFile = os.path.expandvars(era+".db")
    print dBFile
    if usePrivateSQlite:
        process.jec = cms.ESSource("PoolDBESSource",
                                   CondDBSetup,
                                   connect = cms.string("sqlite_file:"+dBFile),
                                   toGet =  cms.VPSet(
                cms.PSet(
                    record = cms.string("JetCorrectionsRecord"),
                    tag = cms.string("JetCorrectorParametersCollection_"+era+"_AK4PF"),
                    label= cms.untracked.string("AK4PF")
                    ),
                cms.PSet(
                        record = cms.string("JetCorrectionsRecord"),
                        tag = cms.string("JetCorrectorParametersCollection_"+era+"_AK4PFchs"),
                        label= cms.untracked.string("AK4PFchs")
                        ),
                )
                                   )
        process.es_prefer_jec = cms.ESPrefer("PoolDBESSource",'jec')
#===========================================================================================================================#
    isMC = False
    if 'MC' in dBFile:
        isMC = True

    ##only corrects JEC/JER uncertainties for the baseline collection; means only muon corrected and EG/Muon corrected MET collections are guaranteed to have the correct JEC uncertainties.
    runMetCorAndUncFromMiniAOD(process, metType="PF",
                               recoMetFromPFCs=False,
                               postfix="",
                               isData=(not isMC),
                               )

    if isMC or os.environ["CMSSW_VERSION"].count("CMSSW_9"):
        ###---Run both uncorrected and corrected MET (recipe v2) for 2017
        runMetCorAndUncFromMiniAOD(process,
                                   isData=(not isMC),
                                   fixEE2017 = True,
                                   fixEE2017Params = {'userawPt': True, 'PtThreshold':50.0, 'MinEtaThreshold':2.65, 'MaxEtaThreshold': 3.139},
                                   # will produce new MET collection: slimmedMETsModifiedMET
                                   postfix = "ModifiedMET",
                               )
        process.flashggMetsStd = cms.EDProducer('FlashggMetProducer',
                                                verbose = cms.untracked.bool(False),
                                                metTag = cms.InputTag('slimmedMETs'),
                                            )
        process.flashggMetsCorr = cms.EDProducer('FlashggMetProducer',
                                                 verbose = cms.untracked.bool(False),
                                                 metTag = cms.InputTag('slimmedMETsModifiedMET'),
                                             )
        process.flashggMetSequence = cms.Sequence(process.fullPatMetSequence*process.flashggMetsStd*
                                                  process.fullPatMetSequenceModifiedMET*process.flashggMetsCorr)
             
    if not isMC and os.environ["CMSSW_VERSION"].count("CMSSW_8_0_28"):
        corMETFromMuonAndEG(process, 
                            pfCandCollection="", #not needed
                            electronCollection="slimmedElectronsBeforeGSFix",
                            photonCollection="slimmedPhotonsBeforeGSFix",
                            corElectronCollection="slimmedElectrons",
                            corPhotonCollection="slimmedPhotons",
                            allMETEGCorrected=True,
                            muCorrection=False,
                            eGCorrection=True,
                            runOnMiniAOD=True,
                            postfix="FullMETClean",
                            )
        
        process.slimmedMETsFullMETClean = process.slimmedMETs.clone()
        process.slimmedMETsFullMETClean.src = cms.InputTag("patPFMetT1FullMETClean")
        process.slimmedMETsFullMETClean.rawVariation =  cms.InputTag("patPFMetRawFullMETClean")
        process.slimmedMETsFullMETClean.t1Uncertainties = cms.InputTag("patPFMetT1%sFullMETClean")
        del process.slimmedMETsFullMETClean.caloMET
        
        process.egcorrMET = cms.Sequence(
            process.cleanedPhotonsFullMETClean+process.cleanedCorPhotonsFullMETClean+
            process.matchedPhotonsFullMETClean + process.matchedElectronsFullMETClean +
            process.corMETPhotonFullMETClean+process.corMETElectronFullMETClean+
            process.patPFMetT1FullMETClean+process.patPFMetRawFullMETClean+
            process.patPFMetT1SmearFullMETClean+process.patPFMetT1TxyFullMETClean+
            process.patPFMetTxyFullMETClean+process.patPFMetT1JetEnUpFullMETClean+
            process.patPFMetT1JetResUpFullMETClean+process.patPFMetT1SmearJetResUpFullMETClean+
            process.patPFMetT1ElectronEnUpFullMETClean+process.patPFMetT1PhotonEnUpFullMETClean+
            process.patPFMetT1MuonEnUpFullMETClean+process.patPFMetT1TauEnUpFullMETClean+
            process.patPFMetT1UnclusteredEnUpFullMETClean+process.patPFMetT1JetEnDownFullMETClean+
            process.patPFMetT1JetResDownFullMETClean+process.patPFMetT1SmearJetResDownFullMETClean+
            process.patPFMetT1ElectronEnDownFullMETClean+process.patPFMetT1PhotonEnDownFullMETClean+
            process.patPFMetT1MuonEnDownFullMETClean+process.patPFMetT1TauEnDownFullMETClean+
            process.patPFMetT1UnclusteredEnDownFullMETClean+process.slimmedMETsFullMETClean)
        process.flashggMets = cms.EDProducer('FlashggMetProducer',
                                             verbose = cms.untracked.bool(False),
                                             metTag = cms.InputTag('slimmedMETsModifiedMET'),
                                             )
#            process.flashggMetsMuons = cms.EDProducer('FlashggMetProducer',
#                             verbose = cms.untracked.bool(False),
#                             metTag = cms.InputTag('slimmedMETs'),
#                             )
#            process.flashggMetsEG = cms.EDProducer('FlashggMetProducer',
#                             verbose = cms.untracked.bool(False),
#                             metTag = cms.InputTag('slimmedMETsEGClean'),
#                             )
#            process.flashggMets = cms.EDProducer('FlashggMetProducer',
#                             verbose = cms.untracked.bool(False),
#                             metTag = cms.InputTag('slimmedMETsFullMETClean'),
#                             )
#            process.flashggMetsEGmuon = cms.EDProducer('FlashggMetProducer',
#                                         verbose = cms.untracked.bool(False),
#                                         metTag = cms.InputTag('slimmedMETsMuEGClean'),
#                                         )
#            process.flashggMetsUncorr = cms.EDProducer('FlashggMetProducer',
#                             verbose = cms.untracked.bool(False),
#                             metTag = cms.InputTag('slimmedMETsUncorrected'),
#                             )
#            process.flashggMetSequence = cms.Sequence(process.flashggMetsMuons *process.flashggMetsEGmuon*process.flashggMets*process.flashggMetsUncorr)
        process.flashggMetSequence = cms.Sequence(process.flashggMets)       
        

        
#===========================================================================================================================#

def setMetCorr(process, metCorr):
    
    process.pfMEtMultShiftCorr.parameters                 = metCorr
    process.patPFMetTxyCorr.parameters                    = metCorr
   
