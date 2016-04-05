import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import FWCore.ParameterSet.VarParsing as VarParsing

from flashgg.MetaData.samples_utils import SamplesManager

process = cms.Process("opttreeDumper")

process.load("HLTrigger.HLTfilters.hltHighLevel_cfi")
process.hltFilter = process.hltHighLevel.clone()
process.hltFilter.throw = cms.bool(True)

from flashgg.MetaData.JobConfig import  JobConfig
customize = JobConfig(crossSections=["$CMSSW_BASE/src/flashgg/MetaData/data/cross_sections.json"])
customize.setDefault("maxEvents",10000)
customize.setDefault("targetLumi",2.6e+4)
customize.parse()

if ("data" in customize.processId):
    process.hltFilter.HLTPaths = cms.vstring("HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_Mass95_v*",
                                             "HLT_Diphoton30PV_18PV_R9Id_AND_IsoCaloId_AND_HE_R9Id_DoublePixelVeto_Mass55_v*",
                                             "HLT_Diphoton30EB_18EB_R9Id_OR_IsoCaloId_AND_HE_R9Id_DoublePixelVeto_Mass55_v*")
else:
    process.hltFilter.HLTPaths = cms.vstring("*")

process.load('RecoMET.METFilters.eeBadScFilter_cfi')
process.eeBadScFilter.EERecHitSource = cms.InputTag("reducedEgamma","reducedEERecHits") # Saved MicroAOD Collection (data only)
process.dataRequirements = cms.Sequence()
process.dataRequirements += process.hltHighLevel

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 10000 )

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService")

if ("data" in customize.processId):
    process.GlobalTag.globaltag = '76X_dataRun2_v15'
else:
    process.GlobalTag.globaltag = '76X_mcRun2_asymptotic_v12'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source ("PoolSource",fileNames = cms.untracked.vstring("file:/afs/cern.ch/user/s/sani/mounteos/cms/store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIIFall15DR76-1_3_0-25ns/1_3_0/GluGluHToGG_M-125_13TeV_powheg_pythia8/RunIIFall15DR76-1_3_0-25ns-1_3_0-v0-RunIIFall15DR76-25nsFlat10to25TSG_76X_mcRun2_asymptotic_v12-v1/160116_110115/0000/myMicroAODOutputFile_1.root",
))

import flashgg.Taggers.dumperConfigTools as cfgTools
process.load("flashgg.Taggers.globalVariables_cff")
process.globalVariables.puReWeight = cms.bool(True)
if ("data" in customize.processId):
    process.globalVariables.puReWeight = cms.bool(False)
    process.dataRequirements += process.eeBadScFilter

process.load("flashgg.Taggers.diphotonDumper_cfi") 
process.diphotonDumper.src = cms.InputTag("flashggDiPhotonSystematics")
#process.diphotonDumper.src = cms.InputTag("flashggUpdatedIdMVADiPhotons")
#process.diphotonDumper.src = cms.InputTag("flashggDiPhotons")
process.diphotonDumper.dumpHistos = False
process.diphotonDumper.dumpTrees  =  True
process.diphotonDumper.dumpGlobalVariables = cms.untracked.bool(True)
process.diphotonDumper.globalVariables = process.globalVariables

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("histo.root"),
                                   closeFileFast = cms.untracked.bool(True)
                                   )

# split tree, histogram and datasets by process
cfgTools.addCategories(process.diphotonDumper,
                       ## categories definition
                       ## cuts are applied in cascade. Events getting to these categories have already failed the "Reject" selection
                       [("All", "1", 0)
                        #("EB", "abs(superCluster.eta)<1.479", 0),
                        #("EE", "abs(superCluster.eta)>1.566",0)
                        ],
                       variables=["leadPhIso := leadingPhoton.pfPhoIso03()",
                                  "leadNeuIso := leadingPhoton.pfNeutIso03()",
                                  "subLeadNeuIso := subLeadingPhoton.pfNeutIso03()",
                                  "subleadPhIso := subLeadingPhoton.pfPhoIso03()",
                                  "leadsieie := leadingPhoton.full5x5_sigmaIetaIeta",
                                  "leadcovieip := leadingPhoton.sieip",
                                  "leadetawidth := leadingPhoton.superCluster.etaWidth",
                                  "leadphiwidth := leadingPhoton.superCluster.phiWidth",
                                  "leads4ratio := leadingPhoton.s4",
                                  "leadr9 := leadingPhoton.r9",
                                  "leadfull5x5r9 := leadingPhoton.full5x5_r9",
                                  "subleadsieie := subLeadingPhoton.full5x5_sigmaIetaIeta",
                                  "subleadcovieip := subLeadingPhoton.sieip",
                                  "subleadetawidth := subLeadingPhoton.superCluster.etaWidth",
                                  "subleadphiwidth := subLeadingPhoton.superCluster.phiWidth",
                                  "subleads4ratio := subLeadingPhoton.s4",
                                  "subleadr9 := subLeadingPhoton.r9",
                                  "subleadfull5x5r9 := subLeadingPhoton.full5x5_r9",
                                  "leadPt      := leadingPhoton.et",
                                  "subleadPt   := subLeadingPhoton.et",
                                  "leadEta     := leadingPhoton.eta",
                                  "subleadEta  := subLeadingPhoton.eta",
                                  "subIDMVA    := leadingView.phoIdMvaWrtChosenVtx()",
                                  "leadIDMVA   := subLeadingView.phoIdMvaWrtChosenVtx()",
                                  "sigmaEoE1   := leadingPhoton.sigEOverE",
                                  "sigmaEoE2   := subLeadingPhoton.sigEOverE",
                                  "leadChIsoRv := leadingView.pfChIso03WrtChosenVtx()",
                                  "subleadChIsoRv := subLeadingView.pfChIso03WrtChosenVtx()",
                                  "leadChIsoWv := leadingPhoton.pfChgIsoWrtWorstVtx03",
                                  "subleadChIsoWv := subLeadingPhoton.pfChgIsoWrtWorstVtx03",
                                  "leadESSigma := leadingPhoton.esEffSigmaRR",
                                  "subleadESSigma := subLeadingPhoton.esEffSigmaRR",
                                  "dipho_pt := pt",
                                  "mass"
                                  ],
                       histograms=[]                                   
                       )
process.diphotonDumper.nameTemplate ="opttree_$SQRTS_$LABEL_$SUBCAT"

process.load("flashgg/Taggers/flashggDiPhotonMVA_cfi")
##process.flashggDiPhotonMVA.DiPhotonTag = cms.InputTag("flashggDiPhotons")
process.flashggDiPhotonMVA.DiPhotonTag = cms.InputTag("flashggDiPhotonSystematics")
#process.flashggDiPhotonMVA.dumpHistos = False
#process.flashggDiPhotonMVA.dumpTrees  =  True

############################
#       Systematics        #
############################
process.load("flashgg.Systematics.flashggDiPhotonSystematics_cfi")
#from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet,massSearchReplaceAnyInputTag
#massSearchReplaceAnyInputTag(process.diphotonDumper, cms.InputTag("flashggDiPhotons"),cms.InputTag("flashggDiPhotonSystematics"))
#massSearchReplaceAnyInputTag(process.diphotonDumper, cms.InputTag("flashggPreselectedDiPhotons"),cms.InputTag("flashggDiPhotonSystematics"))

process.flashggSystTagMerger = cms.EDProducer("TagMerger",src=cms.VInputTag("flashggTagSorter"))
process.systematicsTagSequences = cms.Sequence()
systlabels = [""]
phosystlabels = []

#if customize.processId.count("h_") or customize.processId.count("vbf_"): # convention: ggh vbf wzh tth    
if ("mc" in customize.processId):
    print "Signal MC, so adding systematics and dZ"
    #variablesToUse = minimalVariables
    for direction in ["Up","Down"]:
        #phosystlabels.append("MvaShift%s01sigma" % direction)
        #phosystlabels.append("SigmaEOverEShift%s01sigma" % direction)
        #variablesToUse.append("LooseMvaSF%s01sigma[1,-999999.,999999.] := weight(\"LooseMvaSF%s01sigma\")" % (direction,direction))
        #variablesToUse.append("PreselSF%s01sigma[1,-999999.,999999.] := weight(\"PreselSF%s01sigma\")" % (direction,direction))
        #variablesToUse.append("FracRVWeight%s01sigma[1,-999999.,999999.] := weight(\"FracRVWeight%s01sigma\")" % (direction,direction))
        for r9 in ["HighR9","LowR9"]:
#            phosystlabels.append("MCSmear%sEE%s01sigma" % (r9,direction))
#            for var in ["Rho","Phi"]:
#                phosystlabels.append("MCSmear%sEB%s%s01sigma" % (r9,var,direction))
            for region in ["EB","EE"]:
                phosystlabels.append("MCSmear%s%s%s01sigma" % (r9,region,direction))
                phosystlabels.append("MCScale%s%s%s01sigma" % (r9,region,direction))
#                variablesToUse.append("LooseMvaSF%s%s%s01sigma[1,-999999.,999999.] := weight(\"LooseMvaSF%s%s%s01sigma\")" % (r9,region,direction,r9,region,direction))
#                variablesToUse.append("PreselSF%s%s%s01sigma[1,-999999.,999999.] := weight(\"PreselSF%s%s%s01sigma\")" % (r9,region,direction,r9,region,direction))
    systlabels += phosystlabels
elif ("data" in customize.processId):
    print "Data, so turn of all shifts and systematics, except for Photon Scale central value"
    #variablesToUse = minimalNonSignalVariables
    newvpset = cms.VPSet()
    for pset in process.flashggDiPhotonSystematics.SystMethods:
        if pset.Label.value().count("Scale"):
            pset.NoCentralShift = cms.bool(False) # Turn on central shift for data (it is off for MC)                        
            pset.NSigmas = cms.vint32() # Do not perform shift
            newvpset += [pset]
    process.flashggDiPhotonSystematics.SystMethods = newvpset
    #systprodlist = [] #[process.flashggMuonSystematics,process.flashggElectronSystematics]
    #systprodlist += [getattr(process,"flashggJetSystematics%i"%i) for i in range(len(UnpackedJetCollectionVInputTag))]
    #for systprod in systprodlist:
    #    systprod.SystMethods = cms.VPSet() # empty everything
else:
    print "Background MC, so store mgg and central only"
    #variablesToUse = minimalNonSignalVariables
    vpsetlist = [process.flashggDiPhotonSystematics.SystMethods] #, process.flashggMuonSystematics.SystMethods, process.flashggElectronSystematics.SystMethods]
    #vpsetlist += [getattr(process,"flashggJetSystematics%i"%i).SystMethods for i in range(len(UnpackedJetCollectionVInputTag))] 
    # i.e. process.flashggJetSystematics0.SystMethods, ...
    for vpset in vpsetlist:
        for pset in vpset:
            pset.NSigmas = cms.vint32() # Do not perform shifts if they will not be read, but still do all central values

#for systlabel in systlabels:
#    if systlabel == "":
#        continue
#    newseq = cloneProcessingSnippet(process,process.flashggTagSequence,systlabel)
#    if systlabel in phosystlabels:
#        massSearchReplaceAnyInputTag(newseq,cms.InputTag("flashggDiPhotonSystematics"),cms.InputTag("flashggDiPhotonSystematics",systlabel))
##    #if systlabel in jetsystlabels:    
##    #    for i in range(len(jetSystematicsInputTags)):
##    #        massSearchReplaceAnyInputTag(newseq,jetSystematicsInputTags[i],cms.InputTag(jetSystematicsInputTags[i].moduleLabel,systlabel))
#    for name in newseq.moduleNames():
#        module = getattr(process,name)
#        if hasattr(module,"SystLabel"):
#            module.SystLabel = systlabel
##    process.systematicsTagSequences += newseq
##    process.flashggSystTagMerger.src.append(cms.InputTag("flashggTagSorter" + systlabel))

############################
#    Systematics end       #
############################

process.load("flashgg.Taggers.flashggUpdatedIdMVADiPhotons_cfi")
process.p = cms.Path(process.flashggUpdatedIdMVADiPhotons*process.dataRequirements*process.diphotonDumper*process.flashggDiPhotonMVA)

customize(process)

