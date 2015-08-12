import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("FLASHggSyst")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'POSTLS170_V5::All'
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )
#process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 100 )

# Uncomment the following if you notice you have a memory leak
# This is a lightweight tool to digg further
#process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",
#                                        ignoreTotal = cms.untracked.int32(1),
#                                        monitorPssAndPrivate = cms.untracked.bool(True)
#                                       )

processId = "tth"
process.source = cms.Source ("PoolSource",fileNames = cms.untracked.vstring("file:myMicroAODOutputFile_%s.root" % processId),
#                             skipEvents=cms.untracked.uint32(4965)
)

process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
                                                   flashggDiPhotonSystematics = cms.PSet(initialSeed = cms.untracked.uint32(664)),
						   flashggElectronSystematics = cms.PSet(initialSeed = cms.untracked.uint32(11)),
						   flashggMuonSystematics = cms.PSet(initialSeed = cms.untracked.uint32(13))
                                                  )

process.load("flashgg.Systematics.flashggDiPhotonSystematics_cfi")
process.load("flashgg.Systematics.flashggMuonSystematics_cfi")
process.load("flashgg.Systematics.flashggElectronSystematics_cfi")

# Code to artificially scale photon energies to make different mass points for signal fit tests
srcMass = 125.
targetMass = 120.
process.flashggDiPhotonSystematics.SystMethods.append(cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScale"),
                                                          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                          Label = cms.string("FakeMassScale"),
                                                          NSigmas = cms.vint32(0),
                                                          OverallRange = cms.string("1"),
                                                          BinList = cms.PSet(variables=cms.vstring(),
                                                                             bins=cms.VPSet(cms.PSet (lowBounds = cms.vdouble(), 
                                                                                                      upBounds =  cms.vdouble(), 
                                                                                                      values = cms.vdouble( targetMass/srcMass - 1. ), 
                                                                                                      uncertainties = cms.vdouble( 0. )))),
                                                          Debug = cms.untracked.bool(True)
                                                        )
                                                )

for pset in process.flashggDiPhotonSystematics.SystMethods:
    print "=== 1D syst method pset ==="
    print pset
    print

for pset in process.flashggDiPhotonSystematics.SystMethods2D:
    print "=== 2D syst method pset ==="
    print pset
    print

process.load("flashgg/Taggers/flashggTagSequence_cfi")
process.load("flashgg/Taggers/flashggTagTester_cfi")

from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet,massSearchReplaceAnyInputTag

#process.flashggTagSequence += process.flashggTagTester
#massSearchReplaceAnyInputTag(process.flashggTagSequence,cms.InputTag("flashggFinalEGamma","finalDiPhotons"),cms.InputTag("flashggDiPhotonSystematics"))
#massSearchReplaceAnyInputTag(process.flashggTagSequence,cms.InputTag("flashggFinalEGamma","finalElectrons"),cms.InputTag("flashggElectronSystematics"))
massSearchReplaceAnyInputTag(process.flashggTagSequence,cms.InputTag("flashggDiPhotons"),cms.InputTag("flashggDiPhotonSystematics"))
massSearchReplaceAnyInputTag(process.flashggTagSequence,cms.InputTag("flashggSelectedElectrons"),cms.InputTag("flashggElectronSystematics"))
massSearchReplaceAnyInputTag(process.flashggTagSequence,cms.InputTag("flashggSelectedMuons"),cms.InputTag("flashggMuonSystematics"))

process.flashggSystTagMerger = cms.EDProducer("TagMerger",src=cms.VInputTag("flashggTagSorter"))

process.systematicsTagSequences = cms.Sequence()
systlabels = []
for r9 in ["HighR9","LowR9"]:
    for direction in ["Up","Down"]:
        systlabels.append("MCSmear%sEE%s01sigma" % (r9,direction))
        for var in ["Rho","Phi"]:
            systlabels.append("MCSmear%sEB%s%s01sigma" % (r9,var,direction))
        for region in ["EB","EE"]:
            systlabels.append("MCScale%s%s%s01sigma" % (r9,region,direction))

for systlabel in systlabels:
    newseq = cloneProcessingSnippet(process,process.flashggTagSequence,systlabel)
    massSearchReplaceAnyInputTag(newseq,cms.InputTag("flashggDiPhotonSystematics"),cms.InputTag("flashggDiPhotonSystematics",systlabel))
    for name in newseq.moduleNames():
        module = getattr(process,name)
        if hasattr(module,"SystLabel"):
            module.SystLabel = systlabel
    process.systematicsTagSequences += newseq
    process.flashggSystTagMerger.src.append(cms.InputTag("flashggTagSorter" + systlabel))


from flashgg.Taggers.flashggTagOutputCommands_cff import tagDefaultOutputCommand

process.out = cms.OutputModule("PoolOutputModule", fileName = cms.untracked.string('myTagOutputFile_%s_%i.root' % (processId,targetMass)),
                               outputCommands = tagDefaultOutputCommand
                               )

process.p = cms.Path((process.flashggDiPhotonSystematics+process.flashggMuonSystematics+process.flashggElectronSystematics)*
                     (process.flashggTagSequence+process.systematicsTagSequences)*
                     process.flashggSystTagMerger+process.flashggTagTester)
print process.p

process.e = cms.EndPath(process.out)
