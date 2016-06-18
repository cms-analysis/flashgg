import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import FWCore.ParameterSet.VarParsing as VarParsing

from flashgg.MetaData.samples_utils import SamplesManager

process = cms.Process("zeeValidationDumper")

process.load("HLTrigger.HLTfilters.hltHighLevel_cfi")
process.hltFilter = process.hltHighLevel.clone()
process.hltFilter.throw = cms.bool(True)

from flashgg.MetaData.JobConfig import  JobConfig
customize = JobConfig(crossSections=["$CMSSW_BASE/src/flashgg/MetaData/data/cross_sections.json"])
customize.setDefault("maxEvents",10000)
customize.setDefault("targetLumi",2.6e+4)
customize.parse()

if ("data_single" in customize.processId):
    process.hltFilter.HLTPaths = cms.vstring("HLT_Ele22_eta2p1_WPLoose_Gsf_v*")
elif ("data_double" in customize.processId):
    process.hltFilter.HLTPaths = cms.vstring("HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_DoublePixelSeedMatch_Mass70_v*")
elif ("mc_single" in customize.processId):
    process.hltFilter.HLTPaths = cms.vstring("HLT_Ele22_eta2p1_WP75_Gsf_v*")
elif ("mc_double" in customize.processId):
    process.hltFilter.HLTPaths = cms.vstring("HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_DoublePixelSeedMatch_Mass70_v*")

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

process.source = cms.Source ("PoolSource",fileNames = cms.untracked.vstring("file:/afs/cern.ch/user/s/sani/mounteos/cms/store/group/phys_higgs/cmshgg/ferriff/flashgg/RunIIFall15DR76-1_3_0-25ns_ext1/1_3_1/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15DR76-1_3_0-25ns_ext1-1_3_1-v0-RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/160127_112132/0000/myMicroAODOutputFile_34.root",
))

import flashgg.Taggers.dumperConfigTools as cfgTools
process.load("flashgg.Taggers.globalVariables_cff")
process.globalVariables.puReWeight = cms.bool(True)
if ("data" in customize.processId):
    process.globalVariables.puReWeight = cms.bool(False)
    process.dataRequirements += process.eeBadScFilter

process.load("flashgg.Taggers.diphotoMVAWithZeeDumper_cff")
process.DiPhotonWithZeeMVADumper.dumpHistos = False
process.DiPhotonWithZeeMVADumper.dumpTrees  =  True
process.DiPhotonWithZeeMVADumper.dumpGlobalVariables = cms.untracked.bool(True)
process.DiPhotonWithZeeMVADumper.globalVariables = process.globalVariables

#process.load("flashgg.Taggers.photonDumper_cfi")

process.load("flashgg.Taggers.diphotonDumper_cfi") 
#process.diphotonDumper.src = cms.InputTag("flashggDiPhotonSystematics")
process.diphotonDumper.src = cms.InputTag("flashggUpdatedIdMVADiPhotons")
#process.diphotonDumper.src = cms.InputTag("flashggDiPhotons")
process.diphotonDumper.dumpHistos = False
process.diphotonDumper.dumpTrees  =  True
process.diphotonDumper.dumpGlobalVariables = cms.untracked.bool(True)
process.diphotonDumper.globalVariables = process.globalVariables

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("histo.root"),
                                   closeFileFast = cms.untracked.bool(True)
                                   )

#DIPHOTON MVA
cfgTools.addCategories(process.DiPhotonWithZeeMVADumper,
                       [("All","1", 0),],
                       variables=["dipho_mva:=mvaValue"],
                       histograms=["dipho_mva>>dipho_mva(100,-1,1)",]
)
# split tree, histogram and datasets by process
process.DiPhotonWithZeeMVADumper.nameTemplate ="zeevalidation_$SQRTS_$LABEL_$SUBCAT"


cfgTools.addCategories(process.diphotonDumper,
                       ## categories definition
                       ## cuts are applied in cascade. Events getting to these categories have already failed the "Reject" selection
                       [("All",
"""
(abs(leadingPhoton.superCluster.eta) < 2.5 && abs(subLeadingPhoton.superCluster.eta) < 2.5 && leadingPhoton.passElectronVeto==0) &&
                                        (leadingPhoton.pt > 33) &&
                                        (leadingPhoton.hadronicOverEm < 0.1) &&
                                        ((leadingPhoton.full5x5_r9 > 0.5 && leadingPhoton.isEB) || (leadingPhoton.full5x5_r9 > 0.8 && leadingPhoton.isEE)) &&
                                        ((subLeadingPhoton.full5x5_r9 > 0.5 && subLeadingPhoton.isEB) || (subLeadingPhoton.full5x5_r9 > 0.8 && subLeadingPhoton.isEE)) &&
                                        ((leadingPhoton.isEB &&
                                        (leadingPhoton.full5x5_r9>0.85 ||
                                        (leadingPhoton.full5x5_sigmaIetaIeta < 0.015 && leadingPhoton.pfPhoIso03 < 4.0 && leadingPhoton.trkSumPtHollowConeDR03 < 6.0 ))) ||
                                        (leadingPhoton.isEE &&
                                        (leadingPhoton.full5x5_r9>0.9 ||
                                        (leadingPhoton.full5x5_sigmaIetaIeta < 0.035 && leadingPhoton.pfPhoIso03 < 4.0 && leadingPhoton.trkSumPtHollowConeDR03 < 6.0 )))) &&
                                        (leadingPhoton.pt > 14 && leadingPhoton.hadTowOverEm()<0.15 &&
                                        (leadingPhoton.r9()>0.8 || leadingPhoton.chargedHadronIso()<20 || leadingPhoton.chargedHadronIso()<0.3*leadingPhoton.pt())) &&

                         (abs(leadingPhoton.superCluster.eta) < 2.5 && abs(subLeadingPhoton.superCluster.eta) < 2.5 && subLeadingPhoton.passElectronVeto==0) &&
                                          (subLeadingPhoton.pt > 25) && 
                                          (subLeadingPhoton.hadronicOverEm < 0.1) &&
                                          ((leadingPhoton.full5x5_r9 > 0.5 && leadingPhoton.isEB) || (leadingPhoton.full5x5_r9 > 0.8 && leadingPhoton.isEE)) &&
                                          ((subLeadingPhoton.full5x5_r9 > 0.5 && subLeadingPhoton.isEB) || (subLeadingPhoton.full5x5_r9 > 0.8 && subLeadingPhoton.isEE)) &&
                                          (( subLeadingPhoton.isEB &&
                                          (subLeadingPhoton.full5x5_r9>0.85 ||
                                          (subLeadingPhoton.full5x5_sigmaIetaIeta < 0.015 && subLeadingPhoton.pfPhoIso03 < 4.0 && subLeadingPhoton.trkSumPtHollowConeDR03 < 6.0 ))) ||
                                          (subLeadingPhoton.isEE &&
                                          (subLeadingPhoton.full5x5_r9>0.9 ||
                                          (subLeadingPhoton.full5x5_sigmaIetaIeta < 0.035 && subLeadingPhoton.pfPhoIso03 < 6.0 && subLeadingPhoton.trkSumPtHollowConeDR03 < 6.0 )))) &&
                                          (subLeadingPhoton.pt > 14 && subLeadingPhoton.hadTowOverEm()<0.15 &&
                                          (subLeadingPhoton.r9()>0.8 || subLeadingPhoton.chargedHadronIso()<20 || subLeadingPhoton.chargedHadronIso()<0.3*subLeadingPhoton.pt()))

""", 0)
                        #("EB", "abs(superCluster.eta)<1.479", 0),
                        #("EE", "abs(superCluster.eta)>1.566",0)
                        ],
                       variables=["leadPhIso := leadingPhoton.pfPhoIso03()",
                                  "leadNeuIso := leadingPhoton.pfNeutIso03()",
                                  "subLeadNeuIso := subLeadingPhoton.pfNeutIso03()",
                                  "subleadPhIso := subLeadingPhoton.pfPhoIso03()",
                                  #"rho", 
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
                                  #"scEta := superCluster.eta",
                                  #"CMS_hgg_mass[120,60,120]:=mass", 
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
process.diphotonDumper.nameTemplate ="zeevalidation_$SQRTS_$LABEL_$SUBCAT"

process.load("flashgg/Taggers/flashggDiPhotonMVA_cfi")
#process.flashggDiPhotonMVA.DiPhotonTag = cms.InputTag("flashggDiPhotons")
process.flashggDiPhotonMVA.DiPhotonTag = cms.InputTag("flashggDiPhotonSystematics")

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
        #jetsystlabels.append("JEC%s01sigma" % direction)
        #jetsystlabels.append("JER%s01sigma" % direction)
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

#print "--- Systematics  with independent collections ---"
#print systlabels
#print "-------------------------------------------------"
#print "--- Variables to be dumped, including systematic weights ---"
#print variablesToUse
#print "------------------------------------------------------------"

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
#process.p = cms.Path(process.dataRequirements*process.flashggDiPhotonMVA*process.DiPhotonWithZeeMVADumper*process.diphotonDumper)*process.flashggDiPhotonSystematics
process.p = cms.Path(process.flashggUpdatedIdMVADiPhotons*process.dataRequirements*process.diphotonDumper)


customize(process)

