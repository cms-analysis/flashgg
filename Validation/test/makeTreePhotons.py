import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing
import sys

process = cms.Process("tnp")

###################################################################
myoptions = dict()
isMC = False

myoptions['HLTProcessName']        = "HLT"

myoptions['DIPHOTON_COLL']         = "flashggUpdatedIdMVADiPhotons"
myoptions['PHOTON_CUTS']           = "(abs(superCluster.eta)<2.5) && ((superCluster.energy*sin(superCluster.position.theta))>20.0)&& !(1.4442<=abs(superCluster.eta)<=1.566)"
myoptions['PHOTON_TAG_CUTS']       = "(abs(superCluster.eta)<=2.5) && !(1.4442<=abs(superCluster.eta)<=1.566) && (superCluster.energy*sin(superCluster.position.theta))>40.0"
myoptions['MAXEVENTS']             = cms.untracked.int32(1000) # later change to "-1" 
myoptions['useAOD']                = cms.bool(False)
myoptions['OUTPUTEDMFILENAME']     = 'edmFile.root'
myoptions['DEBUG']                 = cms.bool(False)
myoptions['LEADING_PRESELECTION']  = """(abs(leadingPhoton.superCluster.eta) < 2.5 && abs(subLeadingPhoton.superCluster.eta) < 2.5) &&
                                        (leadingPhoton.pt > 30) && (leadingPhoton.hadronicOverEm < 0.08) &&
                                        ((leadingPhoton.full5x5_r9 > 0.5 && leadingPhoton.isEB) || (leadingPhoton.full5x5_r9 > 0.8 && leadingPhoton.isEE)) &&
                                        ((subLeadingPhoton.full5x5_r9 > 0.5 && subLeadingPhoton.isEB) || (subLeadingPhoton.full5x5_r9 > 0.8 && subLeadingPhoton.isEE)) &&
                                        ((leadingPhoton.isEB && 
                                        (leadingPhoton.full5x5_r9>0.85 ||
                                        (leadingPhoton.full5x5_sigmaIetaIeta < 0.015 && leadingPhoton.pfPhoIso03 < 4.0 && leadingPhoton.trkSumPtHollowConeDR03 < 6.0 ))) ||
                                        (leadingPhoton.isEE && 
                                        (leadingPhoton.full5x5_r9>0.9 ||
                                        (leadingPhoton.full5x5_sigmaIetaIeta < 0.035 && leadingPhoton.pfPhoIso03 < 4.0 && leadingPhoton.trkSumPtHollowConeDR03 < 6.0 )))) &&
                                        (leadingPhoton.pt > 14 && leadingPhoton.hadTowOverEm()<0.15 &&
                                        (leadingPhoton.full5x5_r9()>0.8 || leadingPhoton.chargedHadronIso()<20 || leadingPhoton.chargedHadronIso()<0.3*leadingPhoton.pt())) 
""" 

myoptions['SUBLEADING_PRESELECTION'] = """(abs(leadingPhoton.superCluster.eta) < 2.5 && abs(subLeadingPhoton.superCluster.eta) < 2.5) &&
                                          (subLeadingPhoton.pt > 20) && (subLeadingPhoton.hadronicOverEm < 0.08) &&
                                          ((leadingPhoton.full5x5_r9 > 0.5 && leadingPhoton.isEB) || (leadingPhoton.full5x5_r9 > 0.8 && leadingPhoton.isEE)) &&
                                          ((subLeadingPhoton.full5x5_r9 > 0.5 && subLeadingPhoton.isEB) || (subLeadingPhoton.full5x5_r9 > 0.8 && subLeadingPhoton.isEE)) &&
                                          (( subLeadingPhoton.isEB && 
                                          (subLeadingPhoton.full5x5_r9>0.85 ||
                                          (subLeadingPhoton.full5x5_sigmaIetaIeta < 0.015 && subLeadingPhoton.pfPhoIso03 < 4.0 && subLeadingPhoton.trkSumPtHollowConeDR03 < 6.0 ))) ||
                                          (subLeadingPhoton.isEE && 
                                          (subLeadingPhoton.full5x5_r9>0.9 ||
                                          (subLeadingPhoton.full5x5_sigmaIetaIeta < 0.035 && subLeadingPhoton.pfPhoIso03 < 6.0 && subLeadingPhoton.trkSumPtHollowConeDR03 < 6.0 )))) &&
                                          (subLeadingPhoton.pt > 14 && subLeadingPhoton.hadTowOverEm()<0.15 &&
                                          (subLeadingPhoton.full5x5_r9()>0.8 || subLeadingPhoton.chargedHadronIso()<20 || subLeadingPhoton.chargedHadronIso()<0.3*subLeadingPhoton.pt()))
"""


from flashgg.Validation.treeMakerOptionsPhotons_cfi import *

if (isMC):
    # myoptions['INPUT_FILE_NAME']       = ("/store/group/phys_higgs/cmshgg/ferriff/flashgg/RunIIFall15DR76-1_3_0-25ns_ext1/1_3_1/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15DR76-1_3_0-25ns_ext1-1_3_1-v0-RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/160127_112132/0000/myMicroAODOutputFile_9.root")

    myoptions['INPUT_FILE_NAME']       = ("/store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIISummer16-2_4_1-25ns_Moriond17/2_4_1/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16-2_4_1-25ns_Moriond17-2_4_1-v0-RunIISummer16MiniAODv2-PUMoriond17_HCALDebug_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/170114_082421/0000/myMicroAODOutputFile_10.root")

    myoptions['OUTPUT_FILE_NAME']      = "TnPTree_mc.root"
    myoptions['TnPPATHS']              = cms.vstring("HLT_Ele27_WPTight_Gsf_v*")
    myoptions['TnPHLTTagFilters']      = cms.vstring("hltEle27WPTightGsfTrackIsoFilter")
    myoptions['TnPHLTProbeFilters']    = cms.vstring()
    myoptions['HLTFILTERTOMEASURE']    = cms.vstring("")
    myoptions['GLOBALTAG']             = '80X_mcRun2_asymptotic_2016_TrancheIV_v6'
    myoptions['EVENTSToPROCESS']       = cms.untracked.VEventRange()
else:
    myoptions['INPUT_FILE_NAME']       = ("/store/group/phys_higgs/cmshgg/ferriff/flashgg/RunIIFall15DR76-1_3_0-25ns_ext1/1_3_1/SingleElectron/RunIIFall15DR76-1_3_0-25ns_ext1-1_3_1-v0-Run2015D-16Dec2015-v1/160127_024003/0000/myMicroAODOutputFile_10.root")
    myoptions['OUTPUT_FILE_NAME']      = "TnPTree_data.root"
    myoptions['TnPPATHS']              = cms.vstring("HLT_Ele27_WPTight_Gsf_v*")
    myoptions['TnPHLTTagFilters']      = cms.vstring("hltEle27WPTightGsfTrackIsoFilter")
    myoptions['TnPHLTProbeFilters']    = cms.vstring()
    myoptions['HLTFILTERTOMEASURE']    = cms.vstring("")
    myoptions['GLOBALTAG']             = '80X_dataRun2_2016SeptRepro_v3'
    myoptions['EVENTSToPROCESS']       = cms.untracked.VEventRange()

###################################################################

setModules(process, myoptions)
from flashgg.Validation.treeContentPhotons_cfi import *

process.load("Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff")
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.GlobalTag.globaltag = myoptions['GLOBALTAG']

process.load('FWCore.MessageService.MessageLogger_cfi')
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )

process.MessageLogger.cerr.threshold = ''
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(myoptions['INPUT_FILE_NAME']),
                            eventsToProcess = myoptions['EVENTSToPROCESS']
                            )

process.maxEvents = cms.untracked.PSet( input = myoptions['MAXEVENTS'])

###################################################################
## ID
###################################################################

from flashgg.Validation.photonIDModules_cfi import *
setIDs(process, myoptions)

###################################################################
## SEQUENCES
###################################################################

process.egmPhotonIDs.physicsObjectSrc = cms.InputTag("photonFromDiPhotons")

process.pho_sequence = cms.Sequence(
    process.photonFromDiPhotons +
    process.goodPhotonTags +
    process.goodPhotonProbes +
    process.goodPhotonProbesPreselection +
    process.goodPhotonProbesIDMVA +
    process.goodPhotonTagsIDMVA +
    process.goodPhotonsTagHLT +
    process.goodPhotonsProbeHLT +
    process.goodPhotonProbesL1
    )

###################################################################
## TnP PAIRS
###################################################################

process.allTagsAndProbes = cms.Sequence()
process.allTagsAndProbes *= process.tagTightRECO

process.mc_sequence = cms.Sequence()

##########################################################################
## TREE MAKER OPTIONS
##########################################################################
if (not isMC):
    mcTruthCommonStuff = cms.PSet(
        isMC = cms.bool(False)
        )
    
process.PhotonToRECO = cms.EDAnalyzer("TagProbeFitTreeProducer",
                                      mcTruthCommonStuff, CommonStuffForPhotonProbe,
                                      tagProbePairs = cms.InputTag("tagTightRECO"),
                                      arbitration   = cms.string("None"),
                                      flags         = cms.PSet(passingPresel  = cms.InputTag("goodPhotonProbesPreselection"),
                                                               passingIDMVA   = cms.InputTag("goodPhotonProbesIDMVA"),
                                                               ),                                               
                                      allProbes     = cms.InputTag("goodPhotonsProbeHLT"),
                                      )

if (isMC):
    #process.PhotonToRECO.probeMatches  = cms.InputTag("McMatchRECO")
    process.PhotonToRECO.eventWeight   = cms.InputTag("generator")
    process.PhotonToRECO.PUWeightSrc   = cms.InputTag("pileupReweightingProducer","pileupWeights")
    

process.tree_sequence = cms.Sequence(process.PhotonToRECO)

##########################################################################
## PATHS
##########################################################################

process.out = cms.OutputModule("PoolOutputModule", 
                               fileName = cms.untracked.string(myoptions['OUTPUTEDMFILENAME']),
                               SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring("p"))
                               )
process.outpath = cms.EndPath(process.out)
if (not myoptions['DEBUG']):
    process.outpath.remove(process.out)

##########################################################################################
###### MICROAOD STUFF
##########################################################################################
process.load("flashgg/Taggers/flashggUpdatedIdMVADiPhotons_cfi")
process.flashggUpdatedIdMVADiPhotons.DiPhotonTag = cms.InputTag('flashggDiPhotons') # To include shower shape corrections

process.load("flashgg/Taggers/flashggDiPhotonMVA_cfi")
# process.flashggDiPhotonMVA.DiPhotonTag = cms.InputTag('flashggDiPhotons')
process.flashggDiPhotonMVA.DiPhotonTag = cms.InputTag('flashggUpdatedIdMVADiPhotons')

if (isMC):
    process.p = cms.Path(
        process.flashggUpdatedIdMVADiPhotons +
        process.flashggDiPhotonMVA +
        process.sampleInfo +
        process.hltFilter +
        process.pho_sequence + 
        process.allTagsAndProbes +
        process.pileupReweightingProducer +
        process.mc_sequence + 
        process.tree_sequence
        )
else:
    process.p = cms.Path(
        process.flashggUpdatedIdMVADiPhotons +
        process.flashggDiPhotonMVA +
        #process.sampleInfo +
        process.hltFilter +
        process.pho_sequence + 
        process.allTagsAndProbes +
        process.pileupReweightingProducer +
        process.mc_sequence +
        process.tree_sequence
        )

process.TFileService = cms.Service("TFileService", 
                                   #fileName = cms.string(myoptions['OUTPUT_FILE_NAME']),
                                   fileName = cms.string("TnP.root"),
                                   closeFileFast = cms.untracked.bool(True)
                                   )

from flashgg.MetaData.JobConfig import JobConfig

customize = JobConfig(crossSections=["$CMSSW_BASE/src/flashgg/MetaData/data/cross_sections.json"])
customize.setDefault("maxEvents", 1000)
customize.setDefault("targetLumi", 1)
customize(process)

