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
    era = "Spring16_25nsV6"
    if isMC : 
        era += "_MC"
    else :
        era += "_DATA"
        
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
                                   reclusterJets = False,
                            	   fixEE2017 = True,
                                   fixEE2017Params = {'userawPt': True, 'PtThreshold':50.0, 'MinEtaThreshold':2.65, 'MaxEtaThreshold': 3.139},
                                   pfCandColl = "packedPFCandidates",
				   # will produce new MET collection: slimmedMETsModifiedMET
                                   postfix="ModifiedMET",
                                   isData=(not isMC),
                                   )
        
#===========================================================================================================================#
