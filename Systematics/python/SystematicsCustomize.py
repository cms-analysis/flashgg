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

def modifyTagSequenceForSystematics(process,jetSystematicsInputTags,ZPlusJetMode=False):
    process.flashggTagSequence.remove(process.flashggUnpackedJets) # to avoid unnecessary cloning
    process.flashggTagSequence.remove(process.flashggUpdatedIdMVADiPhotons) # Needs to be run before systematics
    from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet,massSearchReplaceAnyInputTag
    massSearchReplaceAnyInputTag(process.flashggTagSequence,cms.InputTag("flashggUpdatedIdMVADiPhotons"),cms.InputTag("flashggDiPhotonSystematics"))
    massSearchReplaceAnyInputTag(process.flashggTagSequence,cms.InputTag("flashggSelectedElectrons"),cms.InputTag("flashggElectronSystematics"))
    massSearchReplaceAnyInputTag(process.flashggTagSequence,cms.InputTag("flashggSelectedMuons"),cms.InputTag("flashggMuonSystematics"))
    from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag
    for i in range(len(jetSystematicsInputTags)):
        massSearchReplaceAnyInputTag(process.flashggTagSequence,UnpackedJetCollectionVInputTag[i],jetSystematicsInputTags[i])

    if ZPlusJetMode:    
        process.flashggSystTagMerger = cms.EDProducer("ZPlusJetTagMerger",src=cms.VInputTag("flashggZPlusJetTag"))
    else:
        process.flashggSystTagMerger = cms.EDProducer("TagMerger",src=cms.VInputTag("flashggTagSorter"))
    process.systematicsTagSequences = cms.Sequence()

def cloneTagSequenceForEachSystematic(process,systlabels,phosystlabels,jetsystlabels,jetSystematicsInputTags,ZPlusJetMode=False):
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
        if ZPlusJetMode:
            process.flashggSystTagMerger.src.append(cms.InputTag("flashggZPlusJetTag" + systlabel))
        else:
            process.flashggSystTagMerger.src.append(cms.InputTag("flashggTagSorter" + systlabel))


def customizeSystematicsForMC(process):
    customizePhotonSystematicsForMC(process)

def customizePhotonSystematicsForMC(process):
    photonSmearBins = getattr(process,'photonSmearBins',None)
    photonScaleUncertBins = getattr(process,'photonScaleUncertBins',None)
    for pset in process.flashggDiPhotonSystematics.SystMethods:
        if photonSmearBins and pset.Label.value().startswith("MCSmear"):
            pset.BinList = photonSmearBins
        elif photonScaleUncertBins and pset.Label.value().count("Scale"):
            pset.BinList = photonScaleUncertBins
    
def customizeSystematicsForSignal(process):
    customizeSystematicsForMC(process)

def customizeSystematicsForBackground(process):
    # Keep default MC central value behavior, remove all up/down shifts
    customizeSystematicsForMC(process)
    from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag
    vpsetlist = [process.flashggDiPhotonSystematics.SystMethods, process.flashggMuonSystematics.SystMethods, process.flashggElectronSystematics.SystMethods]
    vpsetlist += [getattr(process,"flashggJetSystematics%i"%i).SystMethods for i in range(len(UnpackedJetCollectionVInputTag))]
    for vpset in vpsetlist:
        for pset in vpset:
            pset.NSigmas = cms.vint32()
            if hasattr(pset,"SetupUncertainties"):
                pset.SetupUncertainties = False

def customizeSystematicsForData(process):
    customizePhotonSystematicsForData(process)
    customizeLeptonSystematicsForData(process)
    customizeJetSystematicsForData(process)

def customizePhotonSystematicsForData(process):
    # By default remove the systematic entirely (central value and shifts)
    # For scale: put in central value, but omit shifts
    # TODO: this is wrong for sigE/E and possibly others - check!

    photonScaleBinsData = getattr(process,'photonScaleBinsData',None)
    print photonScaleBinsData, process.photonScaleBinsData
    newvpset = cms.VPSet()
    for pset in process.flashggDiPhotonSystematics.SystMethods:
        if pset.Label.value().count("Scale") or pset.Label.value().count("SigmaEOverESmearing"):
            pset.ApplyCentralValue = cms.bool(True) # Turn on central shift for data (it is off for MC)
            pset.NSigmas = cms.vint32() # Do not perform shift
            if pset.Label.value().count("Scale") and photonScaleBinsData != None: 
                pset.BinList = photonScaleBinsData 
            newvpset += [pset]
        if pset.Label.value().count("SigmaEOverESmearing"):
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
                pset.SetupUncertainties = False
                pset.JetCorrectorTag = cms.InputTag("ak4PFCHSL1FastL2L3ResidualCorrector")
                newvpset += [pset]
        systprod.SystMethods = newvpset
        process.load("JetMETCorrections/Configuration/JetCorrectionServices_cff")
    process.jetCorrectorChain = cms.Sequence(process.ak4PFCHSL1FastL2L3ResidualCorrectorChain)

