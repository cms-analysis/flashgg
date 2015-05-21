import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("FLASHggSyst")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'POSTLS170_V5::All'
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )
#process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 100 )

# Uncomment the following if you notice you have a memory leak
# This is a lightweight tool to digg further
#process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",
#                                        ignoreTotal = cms.untracked.int32(1),
#                                        monitorPssAndPrivate = cms.untracked.bool(True)
#                                       )

processId = "ggh"
process.source = cms.Source ("PoolSource",fileNames = cms.untracked.vstring("file:myMicroAODOutputFile_%s.root" % processId))

process.load("flashgg.Systematics.flashggPhotonSmear_cfi")

# Code to artificially scale photon energies to make different mass points for signal fit tests
srcMass = 130.
targetMass = 130.
process.flashggSmearDiPhoton.SystMethods.append(cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScaleString"),
                                                        MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                        Label = cms.string("FakeMassScale"),
                                                        NSigmas = cms.vint32(0),
                                                        OverallRange = cms.string("1"),
                                                        Bins = cms.VPSet(cms.PSet( Range = cms.string("1"), Shift = cms.double(targetMass/srcMass - 1.), Uncertainty = cms.double(0.))),
                                                        Debug = cms.untracked.bool(True)
                                                        )
                                              )

for pset in process.flashggSmearDiPhoton.SystMethods:
    if pset.MethodName == "FlashggDiPhotonFromPhoton" and pset.PhotonMethodName == "FlashggPhotonScaleString":
        print pset

process.load("flashgg/Taggers/flashggTagSequence_cfi")
process.load("flashgg/Taggers/flashggTagTester_cfi")

from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet,massSearchReplaceAnyInputTag

#process.flashggTagSequence += process.flashggTagTester
massSearchReplaceAnyInputTag(process.flashggTagSequence,cms.InputTag("flashggDiPhotons"),cms.InputTag("flashggSmearDiPhoton"))

#process.flashggUntaggedCategory.DiPhotonTag = cms.InputTag("flashggSmearDiPhoton")
#process.flashggTTHHadronicTag.DiPhotonTag = cms.InputTag("flashggSmearDiPhoton")
#process.flashggVBFTag.DiPhotonTag = cms.InputTag("flashggSmearDiPhoton")
#process.flashggVHEtTag.DiPhotonTag = cms.InputTag("flashggSmearDiPhoton")
#process.flashggTTHLeptonicTag.DiPhotonTag = cms.InputTag("flashggSmearDiPhoton")
#process.flashggVHLooseTag.DiPhotonTag = cms.InputTag("flashggSmearDiPhoton")
#process.flashggVHTightTag.DiPhotonTag = cms.InputTag("flashggSmearDiPhoton")
#process.flashggVHHadronicTag.DiPhotonTag = cms.InputTag("flashggSmearDiPhoton")


#vector<flashgg::DiPhotonCandidate>    "flashggSmearDiPhoton"      ""                "FLASHggSyst"     
#vector<flashgg::DiPhotonCandidate>    "flashggSmearDiPhoton"      "MCScaleBadEBDown01sigma"   "FLASHggSyst"     
#vector<flashgg::DiPhotonCandidate>    "flashggSmearDiPhoton"      "MCScaleBadEBUp01sigma"   "FLASHggSyst"     
#vector<flashgg::DiPhotonCandidate>    "flashggSmearDiPhoton"      "MCScaleBadEEDown01sigma"   "FLASHggSyst"     
#vector<flashgg::DiPhotonCandidate>    "flashggSmearDiPhoton"      "MCScaleBadEEUp01sigma"   "FLASHggSyst"     
#vector<flashgg::DiPhotonCandidate>    "flashggSmearDiPhoton"      "MCScaleGoldEBDown01sigma"   "FLASHggSyst"     
#vector<flashgg::DiPhotonCandidate>    "flashggSmearDiPhoton"      "MCScaleGoldEBUp01sigma"   "FLASHggSyst"     
#vector<flashgg::DiPhotonCandidate>    "flashggSmearDiPhoton"      "MCScaleGoldEEDown01sigma"   "FLASHggSyst"     
#vector<flashgg::DiPhotonCandidate>    "flashggSmearDiPhoton"      "MCScaleGoldEEUp01sigma"   "FLASHggSyst"     

process.flashggSystTagMerger = cms.EDProducer("TagMerger",src=cms.VInputTag("flashggTagSorter"))

process.systematicsTagSequences = cms.Sequence()
for r9 in ["HighR9","LowR9"]:
    for region in ["EB","EE"]:
        for direction in ["Up","Down"]:
            systlabel = "MCScale%s%s%s01sigma" % (r9,region,direction)
            newseq = cloneProcessingSnippet(process,process.flashggTagSequence,systlabel)
            massSearchReplaceAnyInputTag(newseq,cms.InputTag("flashggSmearDiPhoton"),cms.InputTag("flashggSmearDiPhoton",systlabel))
#            print newseq
            process.systematicsTagSequences += newseq
            process.flashggSystTagMerger.src.append(cms.InputTag("flashggTagSorter" + systlabel))

from flashgg.Taggers.flashggTagOutputCommands_cff import tagDefaultOutputCommand

process.out = cms.OutputModule("PoolOutputModule", fileName = cms.untracked.string('myTagOutputFile_%s_%i.root' % (processId,targetMass)),
                               outputCommands = tagDefaultOutputCommand
                               )

process.p = cms.Path(process.flashggSmearDiPhoton*
                     (process.flashggTagSequence+process.systematicsTagSequences)*
                     process.flashggSystTagMerger)

print process.p

process.e = cms.EndPath(process.out)
