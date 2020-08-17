import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import FWCore.ParameterSet.VarParsing as VarParsing

from flashgg.MetaData.samples_utils import SamplesManager

process = cms.Process("mytest")

process.load("HLTrigger.HLTfilters.hltHighLevel_cfi")
process.hltFilter = process.hltHighLevel.clone()
process.hltFilter.throw = cms.bool(True)

from flashgg.MetaData.JobConfig import  JobConfig
customize = JobConfig(crossSections=["$CMSSW_BASE/src/flashgg/MetaData/data/cross_sections.json"])
customize.setDefault("maxEvents",15000)
customize.setDefault("targetLumi",2.6e+4)
customize.parse()

if ("data" in customize.processId):
    process.hltFilter.HLTPaths = cms.vstring("HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_Mass95_v*",
                                             "HLT_Diphoton30PV_18PV_R9Id_AND_IsoCaloId_AND_HE_R9Id_DoublePixelVeto_Mass55_v*",
                                             "HLT_Diphoton30EB_18EB_R9Id_OR_IsoCaloId_AND_HE_R9Id_DoublePixelVeto_Mass55_v*")
else :
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

process.source = cms.Source ("PoolSource",fileNames = cms.untracked.vstring("root://eoscms.cern.ch//eos/cms/store/group/phys_higgs/cmshgg/ferriff/flashgg/RunIIFall15DR76-1_3_0-25ns_ext1/1_3_1/GluGluHToGG_M125_13TeV_amcatnloFXFX_pythia8/RunIIFall15DR76-1_3_0-25ns_ext1-1_3_1-v0-RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/160127_023250/0000/myMicroAODOutputFile_1.root",
))

############################
#       Systematics        #
############################
#process.load("flashgg.Systematics.flashggDiPhotonSystematics_cfi")
#from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet,massSearchReplaceAnyInputTag
#massSearchReplaceAnyInputTag(process.diphotonDumper, cms.InputTag("flashggDiPhotons"),cms.InputTag("flashggDiPhotonSystematics"))
#massSearchReplaceAnyInputTag(process.diphotonDumper, cms.InputTag("flashggPreselectedDiPhotons"),cms.InputTag("flashggDiPhotonSystematics"))

process.load("flashgg/Taggers/flashggTagSequence_cfi")
#process.flashggTagSequence.remove(process.flashggPreselectedDiPhotons)
process.flashggTagSorter.massCutUpper=cms.double(100000.)
process.flashggTagSorter.massCutLower=cms.double(90.)
process.flashggUntagged.Boundaries=cms.vdouble(-99999.,0.31,0.62,0.86,0.98)

process.flashggSystTagMerger = cms.EDProducer("TagMerger", src = cms.VInputTag("flashggTagSorter"))

process.load("flashgg/MicroAOD/flashggTagCandidateProducer_cfi")
process.load("flashgg/Taggers/tagCandidateDumper_cfi")
process.tagCandidateDumper.src = cms.InputTag("flashggTagCandidateProducer")

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("histo.root"),
                                   closeFileFast = cms.untracked.bool(True)
                                   )

import flashgg.Taggers.dumperConfigTools as cfgTools
cfgTools.addCategories(process.tagCandidateDumper,
                       ## categories definition
                       ## cuts are applied in cascade. Events getting to these categories have already failed the "Reject" selection
                       [("All", "1", 0)],
                       variables=["bdt_combined := combindedMVA", 
                                  "dijet_MVA := vbfMVA",
                                  "dipho_mva := diPhoMVAValue",
                                  "mass := diPhoton.mass",
                                  "cat := categoryNumber",
                                  "dipho_pt := diPhoton.pt",
                                  "dipho_phi := diPhoton.phi",
                                  "dipho_eta := diPhoton.eta",
                                  "dipho_PtoM := diPhoton.pt/diPhoton.mass",
                                  "cosphi := diPhotonMVA.CosPhi",
                                  "sigmaMrvoM := diPhotonMVA.sigmarv",
                                  "sigmaMwvoM := diPhotonMVA.sigmawv",
                                  # VTX 
                                  "vtxprob := diPhotonMVA.vtxprob",
                                  "ptbal := diPhoton.ptBal",
                                  "ptasym := diPhoton.ptAsym",
                                  "logspt2 := diPhoton.logSumPt2",
                                  "p2conv := diPhoton.pullConv",
                                  "nconv := diPhoton.nConv",
                                  "vtxmva := diPhoton.vtxProbMVA",
                                  "vtxdz := diPhoton.dZ1",
                                  "vtx_x := diPhoton.vtx.x", 
                                  "vtx_y := diPhoton.vtx.y", 
                                  "vtx_z := diPhoton.vtx.z", 
                                  "gv_x := diPhoton.genPV.x", 
                                  "gv_y := diPhoton.genPV.y", 
                                  "gv_z := diPhoton.genPV.z", 
                                  # Trigger
                                  "hlt1 := triggerBit(0)",
                                  "hlt2 := triggerBit(1)",
                                  "hlt3 := triggerBit(2)",
                                  ##MET ????
                                  # PHOTON 1 
                                  "genmatch1 := genMatchLead",
                                  "et1 := diPhoton.leadingPhoton.et",
                                  "eta1 := diPhoton.leadingPhoton.superCluster.eta",
                                  "phi1 := diPhoton.leadingPhoton.superCluster.phi",
                                  "r91 := diPhoton.leadingPhoton.full5x5_r9",
                                  "e1x31 :=  diPhoton.leadingPhoton.e1x3",
                                  "e2x51 :=  diPhoton.leadingPhoton.e2x5",
                                  "e3x31 :=  diPhoton.leadingPhoton.e3x3",
                                  "e5x51 :=  diPhoton.leadingPhoton.e5x5",                                  
                                  "sieie1 := diPhoton.leadingPhoton.full5x5_sigmaIetaIeta",
                                  "hoe1 := diPhoton.leadingPhoton.hadronicOverEm",
                                  "sigmaEoE1 := diPhoton.leadingPhoton.sigEOverE",
                                  "ptoM1 := diPhoton.leadingPhoton.pt / diPhoton.mass",
                                  "chiso1 := diPhoton.leadingView.pfChIso03WrtChosenVtx",
                                  "chisow1 := diPhoton.leadingPhoton.pfChgIsoWrtWorstVtx04",
                                  "phoiso1 := diPhoton.leadingPhoton.pfPhoIso03",
                                  "phoiso041 := diPhoton.leadingPhoton.pfPhoIso04",
                                  "ecaliso03_1 := diPhoton.leadingPhoton.ecalRecHitSumEtConeDR03",
                                  "hcaliso03_1 := diPhoton.leadingPhoton.hcalTowerSumEtConeDR03",
                                  "pfcluecal03_1 := diPhoton.leadingPhoton.ecalPFClusterIso",
                                  "pfcluhcal03_1 := diPhoton.leadingPhoton.hcalPFClusterIso",
                                  "trkiso03_1 := diPhoton.leadingPhoton.trkSumPtHollowConeDR03",
                                  "pfchiso2_1 := diPhoton.leadingView.pfChIso02WrtChosenVtx",
                                  "isEB1 := diPhoton.leadingPhoton.isEB",
                                  "csev1 := diPhoton.leadingPhoton.passElectronVeto",
                                  "haspixelseed1 := diPhoton.leadingPhoton.hasPixelSeed",
                                  "sieip1 := diPhoton.leadingPhoton.sieip",
                                  "etawidth1 := diPhoton.leadingPhoton.superCluster.etaWidth",
                                  "phiwidth1 := diPhoton.leadingPhoton.superCluster.phiWidth",
                                  "regrerr1 := diPhoton.leadingPhoton.sigEOverE * diPhoton.leadingPhoton.energy",
                                  "idmva1 := diPhoton.leadingView.phoIdMvaWrtChosenVtx",
                                  "s4ratio1 :=  diPhoton.leadingPhoton.s4",
                                  "effSigma1 :=  diPhoton.leadingPhoton.esEffSigmaRR",
                                  "scraw1 :=  diPhoton.leadingPhoton.superCluster.rawEnergy",
                                  "ese1 :=  diPhoton.leadingPhoton.superCluster.preshowerEnergy",                                  
                                  # PHOTON 2
                                  "genmatch2 := genMatchSubLead",
                                  "et2 := diPhoton.subLeadingPhoton.et",
                                  "eta2 := diPhoton.subLeadingPhoton.superCluster.eta",
                                  "phi2 := diPhoton.subLeadingPhoton.superCluster.phi",
                                  "r92 := diPhoton.subLeadingPhoton.full5x5_r9",
                                  "e1x32 :=  diPhoton.subLeadingPhoton.e1x3",
                                  "e2x52 :=  diPhoton.subLeadingPhoton.e2x5",
                                  "e3x32 :=  diPhoton.subLeadingPhoton.e3x3",
                                  "e5x52 :=  diPhoton.subLeadingPhoton.e5x5",                                  
                                  "sieie2 := diPhoton.subLeadingPhoton.full5x5_sigmaIetaIeta",
                                  "hoe2 := diPhoton.subLeadingPhoton.hadronicOverEm",
                                  "sigmaEoE2 := diPhoton.subLeadingPhoton.sigEOverE",
                                  "ptoM2 := diPhoton.subLeadingPhoton.pt / diPhoton.mass",
                                  "isEB2 := diPhoton.subLeadingPhoton.isEB",
                                  "chiso2 := diPhoton.subLeadingView.pfChIso03WrtChosenVtx",
                                  "chisow2 := diPhoton.subLeadingPhoton.pfChgIsoWrtWorstVtx04",
                                  "phoiso2 := diPhoton.subLeadingPhoton.pfPhoIso03",                                  
                                  "phoiso042 := diPhoton.subLeadingPhoton.pfPhoIso04",
                                  "ecaliso03_2 := diPhoton.subLeadingPhoton.ecalRecHitSumEtConeDR03",
                                  "hcaliso03_2 := diPhoton.subLeadingPhoton.hcalTowerSumEtConeDR03",
                                  "pfcluecal03_2 := diPhoton.leadingPhoton.ecalPFClusterIso",
                                  "pfcluhcal03_2 := diPhoton.leadingPhoton.hcalPFClusterIso",
                                  "trkiso03_2 := diPhoton.subLeadingPhoton.trkSumPtHollowConeDR03",
                                  "pfchiso2_2 := diPhoton.subLeadingView.pfChIso02WrtChosenVtx",
                                  "csev2 := diPhoton.subLeadingPhoton.passElectronVeto",
                                  "haspixelseed2 := diPhoton.subLeadingPhoton.hasPixelSeed",  
                                  "sieip2 := diPhoton.subLeadingPhoton.sieip",
                                  "etawidth2 := diPhoton.subLeadingPhoton.superCluster.etaWidth",
                                  "phiwidth2 := diPhoton.subLeadingPhoton.superCluster.phiWidth",
                                  "regrerr2 := diPhoton.subLeadingPhoton.sigEOverE* diPhoton.subLeadingPhoton.energy",
                                  "idmva2 := diPhoton.subLeadingView.phoIdMvaWrtChosenVtx",
                                  "s4ratio2 :=  diPhoton.subLeadingPhoton.s4",
                                  "effSigma2 :=  diPhoton.subLeadingPhoton.esEffSigmaRR",
                                  "scraw2 :=  diPhoton.subLeadingPhoton.superCluster.rawEnergy",
                                  "ese2 :=  diPhoton.subLeadingPhoton.superCluster.preshowerEnergy",
                                  # dijet
                                  "dijet_leadEta",
                                  "dijet_subleadEta",
                                  "dijet_dEta := dijet_abs_dEta",
                                  "dijet_LeadJPt",
                                  "dijet_SubJPt",
                                  "dijet_Zep",
                                  "dijet_dPhi := dijet_dipho_dphi",
                                  "dijet_Mjj",
                                  "dijet_minDRJetPho",
                                  ],
                       histograms=[]                                   
                       )
process.tagCandidateDumper.nameTemplate ="opttree_$SQRTS_$LABEL_$SUBCAT"


#process.systematicsTagSequences = cms.Sequence
#systlabels = [""]
#phosystlabels = []
#
##if customize.processId.count("h_") or customize.processId.count("vbf_"): # convention: ggh vbf wzh tth    
#if ("mc" in customize.processId):
#    print "Signal MC, so adding systematics and dZ"
#    #variablesToUse = minimalVariables
#    for direction in ["Up","Down"]:
#        #phosystlabels.append("MvaShift%s01sigma" % direction)
#        #phosystlabels.append("SigmaEOverEShift%s01sigma" % direction)
#        #jetsystlabels.append("JEC%s01sigma" % direction)
#        #jetsystlabels.append("JER%s01sigma" % direction)
#        #variablesToUse.append("LooseMvaSF%s01sigma[1,-999999.,999999.] := weight(\"LooseMvaSF%s01sigma\")" % (direction,direction))
#        #variablesToUse.append("PreselSF%s01sigma[1,-999999.,999999.] := weight(\"PreselSF%s01sigma\")" % (direction,direction))
#        #variablesToUse.append("FracRVWeight%s01sigma[1,-999999.,999999.] := weight(\"FracRVWeight%s01sigma\")" % (direction,direction))
#        for r9 in ["HighR9","LowR9"]:
##            phosystlabels.append("MCSmear%sEE%s01sigma" % (r9,direction))
##            for var in ["Rho","Phi"]:
##                phosystlabels.append("MCSmear%sEB%s%s01sigma" % (r9,var,direction))
#            for region in ["EB","EE"]:
#                phosystlabels.append("MCSmear%s%s%s01sigma" % (r9,region,direction))
#                phosystlabels.append("MCScale%s%s%s01sigma" % (r9,region,direction))
##                variablesToUse.append("LooseMvaSF%s%s%s01sigma[1,-999999.,999999.] := weight(\"LooseMvaSF%s%s%s01sigma\")" % (r9,region,direction,r9,region,direction))
##                variablesToUse.append("PreselSF%s%s%s01sigma[1,-999999.,999999.] := weight(\"PreselSF%s%s%s01sigma\")" % (r9,region,direction,r9,region,direction))
#    systlabels += phosystlabels
#elif ("data" in customize.processId):
#    print "Data, so turn of all shifts and systematics, except for Photon Scale central value"
#    #variablesToUse = minimalNonSignalVariables
#    newvpset = cms.VPSet()
#    for pset in process.flashggDiPhotonSystematics.SystMethods:
#        if pset.Label.value().count("Scale"):
#            pset.NoCentralShift = cms.bool(False) # Turn on central shift for data (it is off for MC)                        
#            pset.NSigmas = cms.vint32() # Do not perform shift
#            newvpset += [pset]
#    process.flashggDiPhotonSystematics.SystMethods = newvpset
#    #systprodlist = [] #[process.flashggMuonSystematics,process.flashggElectronSystematics]
#    #systprodlist += [getattr(process,"flashggJetSystematics%i"%i) for i in range(len(UnpackedJetCollectionVInputTag))]
#    #for systprod in systprodlist:
#    #    systprod.SystMethods = cms.VPSet() # empty everything
#else:
#    print "Background MC, so store mgg and central only"
#    #variablesToUse = minimalNonSignalVariables
#    vpsetlist = [process.flashggDiPhotonSystematics.SystMethods] #, process.flashggMuonSystematics.SystMethods, process.flashggElectronSystematics.SystMethods]
#    #vpsetlist += [getattr(process,"flashggJetSystematics%i"%i).SystMethods for i in range(len(UnpackedJetCollectionVInputTag))] 
#    # i.e. process.flashggJetSystematics0.SystMethods, ...
#    for vpset in vpsetlist:
#        for pset in vpset:
#            pset.NSigmas = cms.vint32() # Do not perform shifts if they will not be read, but still do all central values
#
##print "--- Systematics  with independent collections ---"
##print systlabels
##print "-------------------------------------------------"
##print "--- Variables to be dumped, including systematic weights ---"
##print variablesToUse
##print "------------------------------------------------------------"
#
##for systlabel in systlabels:
##    if systlabel == "":
##        continue
##    newseq = cloneProcessingSnippet(process,process.flashggTagSequence,systlabel)
##    if systlabel in phosystlabels:
##        massSearchReplaceAnyInputTag(newseq,cms.InputTag("flashggDiPhotonSystematics"),cms.InputTag("flashggDiPhotonSystematics",systlabel))
###    #if systlabel in jetsystlabels:    
###    #    for i in range(len(jetSystematicsInputTags)):
###    #        massSearchReplaceAnyInputTag(newseq,jetSystematicsInputTags[i],cms.InputTag(jetSystematicsInputTags[i].moduleLabel,systlabel))
##    for name in newseq.moduleNames():
##        module = getattr(process,name)
##        if hasattr(module,"SystLabel"):
##            module.SystLabel = systlabel
###    process.systematicsTagSequences += newseq
###    process.flashggSystTagMerger.src.append(cms.InputTag("flashggTagSorter" + systlabel))
#
#############################
##    Systematics end       #
#############################
#
#process.load("flashgg.Taggers.flashggUpdatedIdMVADiPhotons_cfi")
##process.p = cms.Path(process.dataRequirements*process.flashggDiPhotonMVA*process.DiPhotonWithZeeMVADumper*process.diphotonDumper)*process.flashggDiPhotonSystematics
#process.p = cms.Path(process.flashggUpdatedIdMVADiPhotons*process.dataRequirements*process.diphotonDumper)

#process.out = cms.OutputModule("PoolOutputModule", 
#                               fileName = cms.untracked.string("pippo.root"),
#                               #SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring("p"))
#                               )
#process.outpath = cms.EndPath(process.out)


process.p = cms.Path(process.flashggTagSequence*process.flashggSystTagMerger*process.flashggTagCandidateProducer*process.tagCandidateDumper)

customize(process)

