import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

import FWCore.ParameterSet.VarParsing as VarParsing
from flashgg.MetaData.samples_utils import SamplesManager


process = cms.Process("DiPhotonMVATrainig")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc')
#process.GlobalTag.globaltag = 'POSTLS170_V5::All'
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)
process.source = cms.Source ("PoolSource",fileNames = cms.untracked.vstring("/store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIISpring15-25ns/Spring15BetaV5/GluGluHToGG_M-120_13TeV_powheg_pythia8/RunIISpring15-25ns-Spring15BetaV5-v0-RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/150922_093229/0000/myMicroAODOutputFile_1.root"))
##process.source = cms.Source ("PoolSource",fileNames = cms.untracked.vstring("/store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIISpring15-25ns/Spring15BetaV5/GluGluHToGG_M-120_13TeV_powheg_pythia8/RunIISpring15-25ns-Spring15BetaV5-v0-RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/150922_093229/0000/myMicroAODOutputFile_1.root"))
#process.source = cms.Source ("PoolSource",fileNames = cms.untracked.vstring( "/store/group/phys_higgs/cmshgg/sethzenz/flashgg/HggPhys14/Phys14MicroAODV2/VBF_HToGG_M-125_13TeV-powheg-pythia6/HggPhys14-Phys14MicroAODV2-v0-Phys14DR-PU20bx25_PHYS14_25_V1-v1/150210_160130/0000/myMicroAODOutputFile_1.root"))


process.load("flashgg/Taggers/flashggTagSequence_cfi")
process.load("flashgg/Taggers/flashggPreselectedDiPhotons_cfi")
process.load("flashgg/Systematics/flashggDiPhotonSystematics_cfi")
## process.load("flashgg/Taggers/flashggTagTester_cfi")



from flashgg.Systematics.flashggDiPhotonSystematics_cfi import smearBins, scaleBins, smearBinsRereco, scaleBinsRereco

process.flashggDiPhotonSmeared = cms.EDProducer('FlashggDiPhotonSystematicProducer',
                                        src = cms.InputTag("flashggDiPhotons"),
                                        SystMethods2D = cms.VPSet(),
                                        # the number of syst methods matches the number of nuisance parameters
                                        # assumed for a given systematic uncertainty and is NOT required
                                        # to match 1-to-1 the number of bins above.
                                        SystMethods = cms.VPSet(
        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonSigEoverESmearing"),
                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                  Label = cms.string("MCSigmaEOverESmearing"),
                  NSigmas = cms.vint32(0,0),
                  OverallRange = cms.string("1"),
                  BinList = smearBinsRereco,
                  Debug = cms.untracked.bool(False)
                  ),
        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonSmearConstant"),
                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                  #Label = cms.string("MCSmearHighR9EE"),
                  Label = cms.string("MCSmear"),
                  NSigmas = cms.vint32(-1,1),
                  #OverallRange = cms.string("r9>0.94&&abs(superCluster.eta)>=1.5"),
                  OverallRange = cms.string("1"),
                  BinList = smearBinsRereco,
                  # has to match the labels embedded in the photon object as
                  # defined e.g. in flashgg/MicroAOD/python/flashggRandomizedPerPhotonDiPhotonProducer_cff.py
                  #           or in flashgg/MicroAOD/python/flashggRandomizedPhotonProducer_cff.py (if at MicroAOD prod.)
                  RandomLabel = cms.string("rnd_g_E"),
                  Debug = cms.untracked.bool(False),
                  ExaggerateShiftUp = cms.bool(False),
                                                  ),
        )
                                        )







process.flashggDiPhotonScale = cms.EDProducer('FlashggDiPhotonSystematicProducer',
                                        src = cms.InputTag("flashggDiPhotons"),
                                        SystMethods2D = cms.VPSet(),
                                        # the number of syst methods matches the number of nuisance parameters
                                        # assumed for a given systematic uncertainty and is NOT required
                                        # to match 1-to-1 the number of bins above.
                                        SystMethods = cms.VPSet(
        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonSigEoverESmearing"),
                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                  Label = cms.string("DataSigmaEOverESmearing"),
                  NSigmas = cms.vint32(0,0),
                  OverallRange = cms.string("1"),
                  BinList = smearBinsRereco,
                  Debug = cms.untracked.bool(False)
                  ),
        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScale"),
                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                  Label = cms.string("DataScale"),
                  NSigmas = cms.vint32(0,0),
                  OverallRange = cms.string("1"),
                  BinList = scaleBinsRereco,
                  NoCentralShift = cms.bool(False),
                  Debug = cms.untracked.bool(False)
                  )
        
        )
                                              )






#process.flashggPreselectedDiPhotons.src = cms.InputTag("flashggDiPhotonSmeared")

from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel
process.hltHighLevel= hltHighLevel.clone(HLTPaths = cms.vstring("HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_Mass95_v*") )


from flashgg.Taggers.flashggTagOutputCommands_cff import tagDefaultOutputCommand


process.TFileService = cms.Service("TFileService",
			fileName = cms.string("histo.root"),
			closeFileFast = cms.untracked.bool(True)
			)


process.flashggUntagged.Boundaries=cms.vdouble(-2)

# customization for job splitting, lumi weighting, etc.
from flashgg.MetaData.JobConfig import customize
customize.setDefault("maxEvents",-1)
#customize.setDefault("processIndex",5)
customize.setDefault("targetLumi",1.e+4)

customize.setDefault("puTarget", '1.435e+05,6.576e+05,8.781e+05,1.304e+06,2.219e+06,5.052e+06,1.643e+07,6.709e+07,1.975e+08,3.527e+08,4.44e+08,4.491e+08,3.792e+08,2.623e+08,1.471e+08,6.79e+07,2.748e+07,1.141e+07,5.675e+06,3.027e+06,1.402e+06,5.119e+05,1.467e+05,3.53e+04,8270,2235,721.3,258.8,97.27,36.87,13.73,4.932,1.692,0.5519,0.1706,0.04994,0.01383,0.003627,0.0008996,0.0002111,4.689e-05,9.854e-06,1.959e-06,3.686e-07,6.562e-08,1.105e-08,1.762e-09,2.615e-10,4.768e-11,0,0,0')

customize.options.register('diphoxml',
                           'flashgg/Taggers/data/TMVAClassification_BDT_QCDflattened_pray.weights.xml',
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.string,
                           'diphoxml'
                           )

customize.options.register('runOnZ',
                           '',
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.string,
                           'runOnZ'
                           )
customize.runOnZ = ''
customize.parse()

#move scale if it is data, smear if it MC
if customize.processType == 'data':
    process.flashggPreselectedDiPhotons.src = cms.InputTag("flashggDiPhotonScale")
else:
    process.flashggPreselectedDiPhotons.src = cms.InputTag("flashggDiPhotonSmeared")

if customize.runOnZ != '':
    process.flashggPreselectedDiPhotons.variables[-1] = "-(passElectronVeto - 1)"
    if customize.runOnZ == 'double':
        process.hltHighLevel.HLTPaths = ["HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_DoublePixelSeedMatch_Mass70_v*"]
    if customize.runOnZ == 'single' and customize.processType == 'data':
        process.hltHighLevel.HLTPaths = ["HLT_Ele27_eta2p1_WPLoose_Gsf_v*"]
    if customize.runOnZ == 'single' and customize.processType != 'data':
        process.hltHighLevel.HLTPaths = ["HLT_Ele27_eta2p1_WP75_Gsf_v*"]

import flashgg.Taggers.dumperConfigTools as cfgTools
from flashgg.Taggers.tagsDumpers_cfi import createTagDumper
# ## FIXME switch to preselected diphotons
#process.flashggDiPhotonMVANew.DiPhotonTag = "flashggPreselectedDiPhotons"


process.tagDumper = createTagDumper("UntaggedTag")
process.tagDumper.src = "flashggUntagged"
#process.tagDumper.processIndex=cms.int32(1)
process.tagDumper.splitLumiWeight=cms.untracked.bool(True)
#process.tagDumper.throwOnUnclassified= False
process.tagDumper.dumpTrees = True
process.tagDumper.dumpWorkspace = False
process.tagDumper.quietRooFit = True

process.flashggDiPhotonMVA.diphotonMVAweightfile = customize.diphoxml

minmass=100.
maxmass=180.

if customize.runOnZ:
    minmass=70.
    maxmass=120.

cfgTools.addCategory(process.tagDumper, "Reject",  "diPhoton.mass< %f || diPhoton.mass> %f" %(minmass, maxmass),
-1 ## if nSubcat is -1 do not store anythings
)

cfgTools.addCategories(process.tagDumper,
			[## cuts are applied in cascade
			("All","1",0),
			],
			variables=[
			"leadptom         := diPhotonMVA.leadptom  ",
			"subleadptom      := diPhotonMVA.subleadptom ",
			"leadmva          := diPhotonMVA.leadmva ",
			"subleadmva       := diPhotonMVA.subleadmva    ",
			"leadeta          := diPhotonMVA.leadeta     ",
			"subleadeta       := diPhotonMVA.subleadeta",
			"sigmarv          := diPhotonMVA.sigmarv",
     			"sigmawv          := diPhotonMVA.sigmawv",
			"CosPhi           := diPhotonMVA.CosPhi",
			"vtxprob          := diPhotonMVA.vtxprob",
			"result           := diPhotonMVA.result",
			"mass             := diPhoton.mass",
			"pt               := diPhoton.pt",
#                        "beamSpot         := diPhotonMVA.beamSpot",
                        "leadSigEoE_unsm := ? diPhoton.leadingPhoton().hasUserFloat('unsmaeraedSigmaEoE')? diPhoton.leadingPhoton().userFloat('unsmaeraedSigmaEoE'):0", 
                        "subleadSigEoE_unsm := ? diPhoton.subLeadingPhoton().hasUserFloat('unsmaeraedSigmaEoE')? diPhoton.subLeadingPhoton().userFloat('unsmaeraedSigmaEoE'):0", 
                        "dz               := ?!tagTruth().isNull()?abs(tagTruth().genPV().z-diPhoton().vtx().z):0",
                        "leadMatchType    := diPhoton.leadingPhoton().genMatchType()",
                        "subleadMatchType := diPhoton.subLeadingPhoton().genMatchType()",
                        "leadptgen := ?diPhoton.leadingPhoton().hasMatchedGenPhoton()?diPhoton.leadingPhoton().matchedGenPhoton().pt():0",
                        "subleadptgen := ?diPhoton.subLeadingPhoton().hasMatchedGenPhoton()?diPhoton.subLeadingPhoton().matchedGenPhoton().pt():0",
                        "leadSCeta    := diPhoton.leadingPhoton().superCluster().eta()",
                        "subleadSCeta    := diPhoton.subLeadingPhoton().superCluster().eta()",
                        "leadSCphi    := diPhoton.leadingPhoton().superCluster().phi()",
                        "subleadSCphi    := diPhoton.subLeadingPhoton().superCluster().phi()",
                        "leadR9    := diPhoton.leadingPhoton().r9()",
                        "subleadR9    := diPhoton.subLeadingPhoton().r9()",
                        "leadSigEOverE := diPhoton.leadingPhoton().sigEOverE()",
                        "subleadSigEOverE := diPhoton.subLeadingPhoton().sigEOverE()",
                        "massgen := diPhoton.genP4().mass()"
			],
			histograms=[
			"result>>diphoMVAValue(100,-1,1)",
			]
			)
# split tree, histogram and datasets by process
process.tagDumper.nameTemplate ="$PROCESS_$SQRTS_$LABEL"

process.options = cms.untracked.PSet( allowUnscheduled = cms.untracked.bool(True) )

if customize.processType != 'data'and customize.runOnZ != 'single':
    process.p = cms.Path( process.tagDumper )
else:
    process.p = cms.Path( process.hltHighLevel*process.tagDumper )



customize(process)
