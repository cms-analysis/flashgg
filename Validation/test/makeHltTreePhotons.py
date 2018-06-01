import FWCore.ParameterSet.Config as cms
#from FWCore.ParameterSet.VarParsing import VarParsing
import FWCore.Utilities.FileUtils as FileUtils
import sys

process = cms.Process("tnp")


from flashgg.Taggers.flashggPreselectedDiPhotons_cfi import flashggPreselectedDiPhotons
###################################################################
options = dict()
#varOptions = VarParsing('analysis')
#varOptions.parseArguments()

isMC=False
#isData=False
isLEAD=False
#isLEAD=False


#mylist = FileUtils.loadListFromFile ('/afs/cern.ch/user/m/mplaner/flashgg/CMSSW_8_0_8/src/flashgg/microAOD_hlt2016.txt')
#readFiles = cms.untracked.vstring( *mylist)
if(isMC):
    options['HLTProcessName']        = "TEST"
else:
    options['HLTProcessName']        = "HLT"

options['PHOTON_COLL']           = "slimmedPhotons"
#options['DIPHOTON_COLL']         = "flashggPreselectedDiPhotons"
options['DIPHOTON_COLL']         = "flashggDiPhotons"
#options['PHOTON_CUTS']           = ""
#options['PHOTON_PROBE_CUTS']           = ""
#options['PHOTON_TAG_CUTS']       = ""

if(isMC):
    options['TRIGGER_RES']            = "TriggerResults::TEST"
    options['PAT_TRIG']               = "patTrigger"
else:
    options['TRIGGER_RES']            = "TriggerResults::HLT"
    #options['PAT_TRIG']               = "selectedPatTrigger"
    options['PAT_TRIG']               = "patTriggerUnpacker"

#options['PHOTON_CUTS']           = "(abs(eta)<2.5) && (pt)>15.0)"
options['PHOTON_CUTS']           = "(abs(eta)<2.5) && (pt>15) && !(1.4442<=abs(superCluster.eta)<=1.566)"
if(isLEAD):
    options['PHOTON_PROBE_CUTS']           = "(abs(eta)<2.5) && (pt>30.0)"
else:
    options['PHOTON_PROBE_CUTS']           = "(abs(eta)<2.5) && (pt>20.0)"
options['PHOTON_TAG_CUTS']       = "(abs(eta)<=2.1) && !(1.4442<=abs(eta)<=1.566) && (pt>30.0)"

#options['PHOTON_CUTS']           = "(abs(superCluster.eta)<2.5) && ((superCluster.energy*sin(superCluster.position.theta))>15.0)"
#options['PHOTON_PROBE_CUTS']           = "(abs(superCluster.eta)<2.5) && ((superCluster.energy*sin(superCluster.position.theta))>20.0)"
#options['PHOTON_TAG_CUTS']       = "(abs(superCluster.eta)<=2.1) && !(1.4442<=abs(superCluster.eta)<=1.566) && (superCluster.energy*sin(superCluster.position.theta))>30.0"

options['MAXEVENTS']             = cms.untracked.int32(-1) 
options['useAOD']                = cms.bool(False)
options['OUTPUTEDMFILENAME']     = 'edmFile.root'
options['DEBUG']                 = cms.bool(False)
options['LEADING_PRESELECTION']  = "" 

options['SUBLEADING_PRESELECTION'] = ""

from flashgg.Validation.treeMakerOptionsHLT_cfi import *

if (isMC): #isMC
    #options['INPUT_FILE_NAME']       = ('root://eoscms.cern.ch//eos/cms/store/group/phys_higgs/cmshgg/mplaner/flashgg/RunIIFall15-ReHLT-25ns/2016c/microAOD/GluGluHToGG_M-125_13TeV_powheg_pythia8/ReHLT-HLT_2016cmicroAOD-25ns-Hgg-RunIIFall15DR76-25nsFlat10to25TSG_76X_mcRun2_asymptotic_v12-v1/160418_085554/0000/testOpenHLT_Hgg_7.root')
    options['INPUT_FILE_NAME']       = readFiles
                                        #'file:/afs/cern.ch/work/m/mplaner/microAOD/dyM100-200.1.root'
    options['OUTPUT_FILE_NAME']      = "TnPTree_mc.root"
    #options['TnPPATHS']              = cms.vstring("HLT_Ele27_WPLoose_Gsf_v*")
    #options['TnPHLTTagFilters']      = cms.vstring("hltEle27WPLooseGsfTrackIsoFilter")   
    options['TnPPATHS']              = cms.vstring()
    options['TnPHLTTagFilters']      = cms.vstring()   
    options['TnPHLTProbeFilters']    = cms.vstring()
    if(isLEAD):
        options['TagLeadMatchFilters']    = cms.vstring()  #lead eff only
        #options['HLTFILTERTOMEASURE']    = cms.vstring("hltEG30LIso60CaloId15b35eHE12R9Id45b75eEcalIsoLastFilter", "hltEG30LR9Id85b90eHE12R9Id45b75eR9IdLastFilter")  #lead H/E filter
        options['HLTFILTERTOMEASURE']    = cms.vstring("hltEG30LIso60CaloId15b35eHE12R9Id50b80eEcalIsoLastFilter", "hltEG30LR9Id85b90eHE12R9Id50b80eR9IdLastFilter")  #lead H/E filter
        
    else:
        options['TagLeadMatchFilters']    = cms.vstring("hltEG30LIso60CaloId15b35eHE12R9Id50b80eEcalIsoLastFilter", "hltEG30LR9Id85b90eHE12R9Id50b80eR9IdLastFilter")  #sublead eff only
        #options['HLTFILTERTOMEASURE']    = cms.vstring("hltEG18Iso60CaloId15b35eHE12R9Id45b75eTrackIsoUnseededLastFilter", "hltEG18R9Id85b90eHE12R9Id45b75eR9UnseededLastFilter")  #sublead H/E
        options['HLTFILTERTOMEASURE']    = cms.vstring("hltEG18Iso60CaloId15b35eHE12R9Id50b80eTrackIsoUnseededLastFilter", "hltEG18R9Id85b90eHE12R9Id50b80eR9UnseededLastFilter")  #sublead H/E
    
    options['GLOBALTAG']             = 'MCRUN2_74_V9'
    options['EVENTSToPROCESS']       = cms.untracked.VEventRange()
else: 
    options['INPUT_FILE_NAME']       = ("file:myMicroAODOutputFile.root")
    #options['INPUT_FILE_NAME']       = ("/store/group/phys_higgs/cmshgg/mplaner/flashgg/2016hltData_v1/V1/SingleElectron/2016hltData_v1-Run2016B-PromptReco-v2/160518_081245/0000/myMicroAODOutputFile_168.root")
    #options['INPUT_FILE_NAME']        = readFiles
    #                                    )
    #mylist =  FileUtils.loadListFromFile('testFileList.txt')
    #mylist =  FileUtils.loadListFromFile('tempFileList.txt')
    #readFiles      = cms.untracked.vstring(*mylist)
    #options['INPUT_FILE_NAME']       = readFiles
    #options['INPUT_FILE_NAME']       = varOptions.inputFiles
    options['OUTPUT_FILE_NAME']      = "TnPTree_data.root"
    
    options['TnPPATHS']              = cms.vstring("HLT_Ele35_WPTight_Gsf_v*")
    options['TnPHLTTagFilters']      = cms.vstring("hltEle35WPTightGsfTrackIsoFilter")
    options['TnPHLTProbeFilters']    = cms.vstring()

    if(isLEAD):
        options['TagLeadMatchFilters']    = cms.vstring()  #lead eff only                                                                                                                            
        options['HLTFILTERTOMEASURE']    = cms.vstring("hltEG30LIso60CaloId15b35eHE12R9Id50b80eEcalIsoLastFilter", "hltEG30LR9Id85b90eHE12R9Id50b80eR9IdLastFilter")  #lead eff only                 
    else:
        options['TagLeadMatchFilters']    = cms.vstring("hltEG30LIso60CaloId15b35eHE12R9Id50b80eEcalIsoLastFilter", "hltEG30LR9Id85b90eHE12R9Id50b80eR9IdLastFilter")  #sublead eff only             
        options['HLTFILTERTOMEASURE']    = cms.vstring("hltEG22R9Id85b90eHE12R9Id50b80eR9UnseededLastFilter", "hltEG22Iso60CaloId15b35eHE12R9Id50b80eTrackIsoUnseededLastFilter")  #sublead eff only

    options['GLOBALTAG']             = '92X_dataRun2_Prompt_v4'
    options['EVENTSToPROCESS']       = cms.untracked.VEventRange()

###################################################################

setModules(process, options)
from EgammaAnalysis.TnPTreeProducer.egmTreesContent_cff import *

process.load("Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff")
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.GlobalTag.globaltag = options['GLOBALTAG']

process.load('FWCore.MessageService.MessageLogger_cfi')
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )

process.MessageLogger.cerr.threshold = ''
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(options['INPUT_FILE_NAME']),
                            #fileNames = readFiles,
                            eventsToProcess = options['EVENTSToPROCESS'],
                            bypassVersionCheck = cms.untracked.bool(True)
                            )

process.maxEvents = cms.untracked.PSet( input = options['MAXEVENTS'])

###################################################################
## ID
###################################################################

from EgammaAnalysis.TnPTreeProducer.egmPhotonIDModules_cff import *
setIDs(process, options)

###################################################################
##rerunning PAT trigger
##################################################################

if(isMC):
    process.load('PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cff')
    process.patTrigger.processName = cms.string('TEST') #FLASHggMicroAOD                                                                                                                      
    process.patTrigger.onlyStandAlone = cms.bool(False)
    process.patTrigger.packTriggerPrescales = cms.bool(False)
    process.patTrigger.triggerResults = cms.InputTag( "TriggerResults::TEST" )
    process.patTrigger.triggerEvent   = cms.InputTag( "hltTriggerSummaryAOD::TEST" )

###################################################################
## SEQUENCES
###################################################################

process.egmPhotonIDs.physicsObjectSrc = cms.InputTag("photonFromDiPhotons")
if(isMC):
    process.pho_sequence = cms.Sequence(
        process.patTrigger +
        process.photonFromDiPhotons +
        process.goodPhotonTags +
        process.goodPhotonProbes +
        process.goodPhotonProbesIDMVA +
        process.goodPhotonsProbeTnPprobe +
        process.goodPhotonProbesPreselection +
        process.goodPhotonProbesL1 +
        process.goodPhotonsProbeHLT +
        process.goodPhotonTagsIDMVA +
        process.goodPhotonTagsPreselection +
        process.goodPhotonsTagLeadMatch +
        process.goodPhotonsTagHLT 
        )
else:
    process.patTriggerUnpacker = cms.EDProducer("PATTriggerObjectStandAloneUnpacker",
                                                patTriggerObjectsStandAlone = cms.InputTag("slimmedPatTrigger"),
                                                triggerResults = cms.InputTag('TriggerResults'      , '', 'HLT'),
                                                unpackFilterLabels = cms.bool(True)
                                                )
    process.pho_sequence = cms.Sequence(
        process.patTriggerUnpacker +
        process.photonFromDiPhotons +
        process.goodPhotonTags +
        process.goodPhotonProbes +
        process.goodPhotonProbesIDMVA +
        process.goodPhotonsProbeTnPprobe +
        process.goodPhotonProbesPreselection +
        process.goodPhotonProbesL1 +
        process.goodPhotonsProbeHLT +
        process.goodPhotonTagsIDMVA +
        process.goodPhotonTagsPreselection +
        process.goodPhotonsTagLeadMatch +
        process.goodPhotonsTagHLT 
        )    
###################################################################
## TnP PAIRS
###################################################################

process.allTagsAndProbes = cms.Sequence()
process.allTagsAndProbes *= process.noL1RECO
process.allTagsAndProbes *= process.L1RECO

process.mc_sequence = cms.Sequence()

#if (isMC):
#    process.mc_sequence *= (process.McMatchTag + process.McMatchRECO)

##########################################################################
## TREE MAKER OPTIONS
##########################################################################
#if ( isMC):
#    mcTruthCommonStuff = cms.PSet(
#        isMC = cms.bool(True),
#        probeMatches  = cms.InputTag("McMatchRECO"),
#        tagMatches  = cms.InputTag("McMatchRECO"),
#        eventWeight   = cms.InputTag("generator"),
#        motherPdgId   = cms.int32(23),
#        makeMCUnbiasTree = cms.bool(isMC),
#        checkMotherInUnbiasEff = cms.bool(False),
#        )
#else:    
mcTruthCommonStuff = cms.PSet(
    isMC = cms.bool(False)
    )

process.PhotonToRECO = cms.EDAnalyzer("TagProbeFitTreeProducer",
                                      mcTruthCommonStuff, CommonStuffForPhotonProbe,
                                      tagProbePairs = cms.InputTag("noL1RECO"),
                                      #tagProbePairs = cms.InputTag("tagL1RECO"),
                                      arbitration   = cms.string("None"),
                                      flags         = cms.PSet(passingL1  = cms.InputTag("goodPhotonProbesL1"),
                                                               passingHLT     = cms.InputTag("goodPhotonsProbeHLT"),
                                                               ),                                               
                                      allProbes     = cms.InputTag("goodPhotonProbesPreselection"),
                                      genParticles = cms.InputTag("flashggPrunedGenParticles"), #flashggPrunedGenParticles #flashggGenPhotons
                                      useTauDecays = cms.bool(False),
                                      pdgId = cms.int32(22),
                                      checkCharge = cms.bool(False),
                                      )

process.PhotonToRECO.pfMet = cms.InputTag("slimmedMETs")
process.PhotonToRECOL1 = cms.EDAnalyzer("TagProbeFitTreeProducer",
                                        mcTruthCommonStuff, CommonStuffForPhotonProbe,
                                        tagProbePairs = cms.InputTag("L1RECO"),
                                        arbitration   = cms.string("None"),
                                        #arbitration   = cms.string("OneProbe"),
                                        flags         = cms.PSet(passingHLTl1     = cms.InputTag("goodPhotonsProbeHLT"),
                                                                 ),                                               
                                        allProbes     = cms.InputTag("goodPhotonProbesL1"),
                                        genParticles = cms.InputTag("flashggPrunedGenParticles"), #flashggPrunedGenParticles #flashggGenPhotons
                                        useTauDecays = cms.bool(False),
                                        pdgId = cms.int32(22),
                                        checkCharge = cms.bool(False),
                                        )
process.PhotonToRECOL1.pfMet = cms.InputTag("slimmedMETs")
#if(isMC):
#    process.PhotonToRECO.PUWeightSrc   = cms.InputTag("pileupReweightingProducer","pileupWeights")
#    process.PhotonToRECOL1.PUWeightSrc   = cms.InputTag("pileupReweightingProducer","pileupWeights")
#    process.PhotonToRECO.tagVariables.probe_dRTau    = cms.InputTag("GsfDRToNearestTauProbe")
#    process.PhotonToRECOL1.tagVariables.probe_dRTau    = cms.InputTag("GsfDRToNearestTauProbe")
#    process.PhotonToRECO.variables.Pho_dRTau  = cms.InputTag("GsfDRToNearestTauProbe")
#    process.PhotonToRECOL1.variables.Pho_dRTau  = cms.InputTag("GsfDRToNearestTauProbe")
    
process.tree_sequence = cms.Sequence(process.PhotonToRECO)

process.tree_sequence2 = cms.Sequence(process.PhotonToRECOL1)

##########################################################################
## PATHS
##########################################################################

process.out = cms.OutputModule("PoolOutputModule", 
                               fileName = cms.untracked.string(options['OUTPUTEDMFILENAME']),
                               SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring("p"))
                               )
process.outpath = cms.EndPath(process.out)
if (not options['DEBUG']):
    process.outpath.remove(process.out)

##########################################################################################
###### MICROAOD STUFF
##########################################################################################
process.load("flashgg/Taggers/flashggUpdatedIdMVADiPhotons_cfi")
process.load("flashgg/Taggers/flashggDiPhotonMVA_cfi")
process.flashggDiPhotonMVA.DiPhotonTag = cms.InputTag('flashggDiPhotons')
process.load("flashgg/Taggers/flashggPreselectedDiPhotons_cfi")




if (isMC):
    process.p = cms.Path(
        process.patTrigger +
        process.flashggUpdatedIdMVADiPhotons +
        process.flashggDiPhotonMVA +
        process.flashggPreselectedDiPhotons +
        #process.sampleInfo +
        process.hltFilter +
        process.pho_sequence + 
        process.allTagsAndProbes +
        #process.pileupReweightingProducer +
        process.mc_sequence + 
        #process.GsfDRToNearestTauProbe + 
        #process.GsfDRToNearestTauTag + 
        process.tree_sequence +
        process.tree_sequence2
        )
else:
    process.p = cms.Path(
        process.flashggUpdatedIdMVADiPhotons +
        process.flashggDiPhotonMVA +
        process.flashggPreselectedDiPhotons +
        #process.sampleInfo +
        process.hltFilter +
        process.pho_sequence + 
        process.allTagsAndProbes +
        process.mc_sequence +
        process.tree_sequence +
        process.tree_sequence2
        )

process.TFileService = cms.Service(
    "TFileService", fileName = cms.string(options['OUTPUT_FILE_NAME']),
    closeFileFast = cms.untracked.bool(True)
    )

# import flashgg customization
from flashgg.MetaData.JobConfig import customize
# set default options if needed
customize.setDefault("maxEvents",100)
customize.setDefault("targetLumi",10e+3)
# call the customization
customize(process)
