import FWCore.ParameterSet.Config as cms
from PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMetCorAndUncFromMiniAOD



from os import environ
usePrivateSQlite=True

if usePrivateSQlite:
    from CondCore.DBCommon.CondDBSetup_cfi import *
    import os


#============================================Apply MET correction and syst.=================================================#

def runMETs(process,isMC):
    #================================ Get the most recent JEC ==================================================================#
    # Setup the private SQLite -- Ripped from PhysicsTools/PatAlgos/test/corMETFromMiniAOD.py
    era = "Summer16_23Sep2016"
    if isMC : 
        era += "V4_MC"
    else :
        era += "AllV4_DATA"
        
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
        
        
        
        runMetCorAndUncFromMiniAOD(process, metType="PF",
                                   jetCollUnskimmed="slimmedJets",
                                   electronColl="slimmedElectrons",
                                   photonColl="slimmedPhotons",
                                   muonColl="slimmedMuons",
                                   tauColl="slimmedTaus",
                                   #reclusterJets = False,
                                   recoMetFromPFCs=True,
                                   pfCandColl = "packedPFCandidates",
                                   postfix="",
                                   isData=(not isMC),
                                   )
        
#===========================================================================================================================#

def setMetCorr(process, metCorr):
    
    process.pfMEtMultShiftCorr.parameters                 = metCorr
    process.patPFMetTxyCorr.parameters                    = metCorr
   
