import FWCore.ParameterSet.Config as cms

def printSystematicInfo(process):
    vpsetlist = [process.flashggDiPhotonSystematics.SystMethods, process.flashggMuonSystematics.SystMethods, process.flashggElectronSystematics.SystMethods]
#    from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag
#    vpsetlist += [getattr(process,"flashggJetSystematics%i"%i).SystMethods for i in range(len(UnpackedJetCollectionVInputTag))]
    vpsetlist += [process.flashggJetSystematics0.SystMethods]
    print (14*"-"+" DUMPING SYSTEMATIC OVERVIEW "+14*"-")
    print "%20s %15s %20s" % ("Systematic","Central value?","Systematic shifts?")
    print 57*"-"
    for vpset in vpsetlist:
        for pset in vpset:
#            if detailed:
#               if hasattr(pset,"PhotonMethodName"):
#                    print pset.PhotonMethodName.value(),pset.Label.value(),pset.OverallRange.value(),
#                else:    
#                    print pset.MethodName.value(),pset.Label.value(),pset.OverallRange.value(),
            syst = pset.Label.value()
            if pset.ApplyCentralValue.value():
                cv = "YES"
            else:
                cv = "NO"
            sigmalist = pset.NSigmas.value()    
            sig = ""
            if len(sigmalist) > 0:
                for val in sigmalist:
                    sig += "%i " % val
            else:    
                sig += "NO"
            print "%20s %15s %20s" % (syst,cv,sig)
        print 57*"-"


def createStandardSystematicsProducers(process):
    process.load("flashgg/Taggers/flashggTagSequence_cfi")
    process.load("flashgg.Systematics.flashggDiPhotonSystematics_cfi")
    process.load("flashgg.Systematics.flashggMuonSystematics_cfi")
    process.load("flashgg.Systematics.flashggElectronSystematics_cfi")

    from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag
    from flashgg.Systematics.flashggJetSystematics_cfi import createJetSystematics
    jetSystematicsInputTags = createJetSystematics(process,UnpackedJetCollectionVInputTag)
    return jetSystematicsInputTags

def modifyTagSequenceForSystematics(process,jetSystematicsInputTags):
    process.flashggTagSequence.remove(process.flashggUnpackedJets) # to avoid unnecessary cloning
    from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet,massSearchReplaceAnyInputTag
    massSearchReplaceAnyInputTag(process.flashggTagSequence,cms.InputTag("flashggDiPhotons"),cms.InputTag("flashggDiPhotonSystematics"))
    massSearchReplaceAnyInputTag(process.flashggTagSequence,cms.InputTag("flashggSelectedElectrons"),cms.InputTag("flashggElectronSystematics"))
    massSearchReplaceAnyInputTag(process.flashggTagSequence,cms.InputTag("flashggSelectedMuons"),cms.InputTag("flashggMuonSystematics"))
    from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag
    for i in range(len(jetSystematicsInputTags)):
        massSearchReplaceAnyInputTag(process.flashggTagSequence,UnpackedJetCollectionVInputTag[i],jetSystematicsInputTags[i])

    process.flashggSystTagMerger = cms.EDProducer("TagMerger",src=cms.VInputTag("flashggTagSorter"))
    process.systematicsTagSequences = cms.Sequence()

def cloneTagSequenceForEachSystematic(process,systlabels,phosystlabels,jetsystlabels,jetSystematicsInputTags):
    for systlabel in systlabels:
        if systlabel == "":
            continue
        from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet,massSearchReplaceAnyInputTag
        newseq = cloneProcessingSnippet(process,process.flashggTagSequence,systlabel)
        if systlabel in phosystlabels:
            massSearchReplaceAnyInputTag(newseq,cms.InputTag("flashggDiPhotonSystematics"),cms.InputTag("flashggDiPhotonSystematics",systlabel))
        if systlabel in jetsystlabels:
            for i in range(len(jetSystematicsInputTags)):
                massSearchReplaceAnyInputTag(newseq,jetSystematicsInputTags[i],cms.InputTag(jetSystematicsInputTags[i].moduleLabel,systlabel))
        for name in newseq.moduleNames():
            module = getattr(process,name)
            if hasattr(module,"SystLabel"):
                module.SystLabel = systlabel
        process.systematicsTagSequences += newseq
        process.flashggSystTagMerger.src.append(cms.InputTag("flashggTagSorter" + systlabel))

def customizeSystematicsForBackground(process):
    # Keep default MC central value behavior, remove all up/down shifts

    from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag
    vpsetlist = [process.flashggDiPhotonSystematics.SystMethods, process.flashggMuonSystematics.SystMethods, process.flashggElectronSystematics.SystMethods]
    vpsetlist += [getattr(process,"flashggJetSystematics%i"%i).SystMethods for i in range(len(UnpackedJetCollectionVInputTag))]
    for vpset in vpsetlist:
        for pset in vpset:
            pset.NSigmas = cms.vint32()

def customizeSystematicsForData(process):
    customizePhotonSystematicsForData(process)
    customizeLeptonSystematicsForData(process)
    customizeJetSystematicsForData(process)

def customizePhotonSystematicsForData(process):
    # By default remove the systematic entirely (central value and shifts)
    # For scale: put in central value, but omit shifts
    # TODO: this is wrong for sigE/E and possibly others - check!

    newvpset = cms.VPSet()
    for pset in process.flashggDiPhotonSystematics.SystMethods:
        if pset.Label.value().count("Scale"):
            pset.ApplyCentralValue = cms.bool(True) # Turn on central shift for data (it is off for MC)
            pset.NSigmas = cms.vint32() # Do not perform shift
            newvpset += [pset]
    process.flashggDiPhotonSystematics.SystMethods = newvpset

def customizeLeptonSystematicsForData(process):
    # Remove systematics entirely

    systprodlist = [process.flashggMuonSystematics,process.flashggElectronSystematics]
    for systprod in systprodlist:
        systprod.SystMethods = cms.VPSet() # empty everything

def customizeJetSystematicsForData(process):
    # By default remove the systematic entirely
    # For JEC, re-do central value in case the global tag has been updated

    from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag
    jetsystprodlist = [getattr(process,"flashggJetSystematics%i"%i) for i in range(len(UnpackedJetCollectionVInputTag))]
    for systprod in jetsystprodlist:
        # For updating bugged or unavailable JEC
        # It should be a noop in cases where they are already correct
        newvpset = cms.VPSet()
        for pset in systprod.SystMethods:
            if pset.Label.value().count("JEC"):
                pset.NSigmas = cms.vint32() # Do not perform shifts, central value only                                                                                            
                newvpset += [pset]
        systprod.SystMethods = newvpset
        systprod.DoCentralJEC = True
        systprod.JECLabel = "ak4PFCHSL1FastL2L3Residual"
        process.load("JetMETCorrections/Configuration/JetCorrectionServices_cff")


