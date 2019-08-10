import FWCore.ParameterSet.Config as cms

def printSystematicInfo(process):
    vpsetlist = [process.flashggDiPhotonSystematics.SystMethods, process.flashggMuonSystematics.SystMethods, process.flashggElectronSystematics.SystMethods]
#    from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag
#    vpsetlist += [getattr(process,"flashggJetSystematics%i"%i).SystMethods for i in range(len(UnpackedJetCollectionVInputTag))]
    vpsetlist += [process.flashggJetSystematics0.SystMethods]
    print (14*"-"+" DUMPING SYSTEMATIC OVERVIEW "+14*"-")
    print "%20s %15s %20s" % ("Systematic","Central value?","Systematic shifts?")
    print 57*"-"
    printSystematicVPSet(vpsetlist)
    print (13*"-"+" DUMPING 2D SYSTEMATIC OVERVIEW "+12*"-")
    print "%20s %15s %20s" % ("Systematic","Central value?","Systematic shifts?")
    print 57*"-"
    vpsetlist2D  = [process.flashggDiPhotonSystematics.SystMethods2D, process.flashggMuonSystematics.SystMethods2D, process.flashggElectronSystematics.SystMethods2D]
    vpsetlist2D += [process.flashggJetSystematics0.SystMethods2D]
    vpsetlist +=[process.flashggMetSystematics.SystMethods]
    printSystematicVPSet(vpsetlist2D)

def turnOnAllSystematicsDebug(process):
    vpsetlist = [process.flashggDiPhotonSystematics.SystMethods, process.flashggMuonSystematics.SystMethods, process.flashggElectronSystematics.SystMethods]
    vpsetlist += [process.flashggJetSystematics0.SystMethods]
    vpsetlist += [process.flashggDiPhotonSystematics.SystMethods2D, process.flashggMuonSystematics.SystMethods2D, process.flashggElectronSystematics.SystMethods2D]
    vpsetlist += [process.flashggJetSystematics0.SystMethods2D]
    vpsetlist += [process.flashggMetSystematics.SystMethods]
    for vpset in vpsetlist:
        for pset in vpset:
            pset.Debug = True


def printSystematicVPSet(vpsetlist):
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
            sig2 = ""
            if type(sigmalist) == type([]) and len(sigmalist) > 0:
                for val in sigmalist:
                    sig += "%i " % val
            elif type(sigmalist) == type(cms.PSet()) and (len(sigmalist.firstVar) > 0 or len(sigmalist.secondVar) > 0):
                sig += "1st: "
                for val in sigmalist.firstVar:
                    sig += "%i " % val
                sig2 += "2nd: "
                for val in sigmalist.secondVar:
                        sig2 += "%i " % val
            else:    
                sig += "NO"
            print "%20s %15s %20s" % (syst,cv,sig)
            if (sig2 != ""):
                print "%20s %15s %20s" % ("","",sig2)
        if len(vpset):
            print 57*"-"


def createStandardSystematicsProducers(process, options):
    process.load("flashgg/Taggers/flashggTagSequence_cfi")
    process.load("flashgg.Systematics.flashggDiPhotonSystematics_cfi")
    process.load("flashgg.Systematics.flashggMuonSystematics_cfi")
    process.load("flashgg.Systematics.flashggElectronSystematics_cfi")
    process.load("flashgg.Systematics.flashggMetSystematics_cfi")

    from flashgg.Taggers.flashggTagSequence_cfi import *
    process.flashggTagSequence = flashggPrepareTagSequence(options.metaConditions)
    
    import flashgg.Systematics.flashggDiPhotonSystematics_cfi as diPhotons_syst
    diPhotons_syst.setupDiPhotonSystematics( process, options )

    import flashgg.Systematics.flashggMuonSystematics_cfi as muon_sf
    muon_sf.SetupMuonScaleFactors( process , options.metaConditions["MUON_ID"], options.metaConditions["MUON_ISO"] )
    
    setattr( process.flashggElectronSystematics.SystMethods,"BinList",str("process.flashggElectronSystematics."+options.metaConditions["Ele_ID_eff_bin"]))

    from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag
    from flashgg.Systematics.flashggJetSystematics_cfi import jetSystematicsCustomize
    jetSystematics = jetSystematicsCustomize(process, options)
    jetSystematicsInputTags = jetSystematics.createJetSystematics(UnpackedJetCollectionVInputTag)
    return jetSystematicsInputTags

def modifyTagSequenceForSystematics(process,jetSystematicsInputTags,ZPlusJetMode=False):
    process.flashggTagSequence.remove(process.flashggUnpackedJets) # to avoid unnecessary cloning
    process.flashggTagSequence.remove(process.flashggUpdatedIdMVADiPhotons) # Needs to be run before systematics
    from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet,massSearchReplaceAnyInputTag
    massSearchReplaceAnyInputTag(process.flashggTagSequence,cms.InputTag("flashggUpdatedIdMVADiPhotons"),cms.InputTag("flashggDiPhotonSystematics"))
    massSearchReplaceAnyInputTag(process.flashggTagSequence,cms.InputTag("flashggSelectedElectrons"),cms.InputTag("flashggElectronSystematics"))
    massSearchReplaceAnyInputTag(process.flashggTagSequence,cms.InputTag("flashggSelectedMuons"),cms.InputTag("flashggMuonSystematics"))
    massSearchReplaceAnyInputTag(process.flashggTagSequence,cms.InputTag("flashggMets"),cms.InputTag("flashggMetSystematics"))
    from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag
    for i in range(len(jetSystematicsInputTags)):
        massSearchReplaceAnyInputTag(process.flashggTagSequence,UnpackedJetCollectionVInputTag[i],jetSystematicsInputTags[i])

    if ZPlusJetMode == 2:  # VBF    
        process.flashggSystTagMerger = cms.EDProducer("VBFTagMerger",src=cms.VInputTag("flashggVBFTag"))
    elif ZPlusJetMode:    
        process.flashggSystTagMerger = cms.EDProducer("ZPlusJetTagMerger",src=cms.VInputTag("flashggZPlusJetTag"))
    else:
        process.flashggSystTagMerger = cms.EDProducer("TagMerger",src=cms.VInputTag("flashggTagSorter"))
    process.systematicsTagSequences = cms.Sequence()

def cloneTagSequenceForEachSystematic(process,systlabels=[],phosystlabels=[],metsystlabels=[],jetsystlabels=[],jetSystematicsInputTags=None,ZPlusJetMode=False):
    #process,systlabels,phosystlabels,metsystlabels,jetsystlabels,jetSystematicsInputTags,ZPlusJetMode=False):
    if jetSystematicsInputTags is None:
        raise TypeError
    for systlabel in systlabels:
        if systlabel == "":
            continue
        from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet,massSearchReplaceAnyInputTag
        newseq = cloneProcessingSnippet(process,process.flashggTagSequence,systlabel)
        if systlabel in metsystlabels:
            massSearchReplaceAnyInputTag(newseq,cms.InputTag("flashggMetSystematics"),cms.InputTag("flashggMetSystematics",systlabel))
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
        if ZPlusJetMode == 2:
            process.flashggSystTagMerger.src.append(cms.InputTag("flashggVBFTag" + systlabel))
        elif ZPlusJetMode:
            process.flashggSystTagMerger.src.append(cms.InputTag("flashggZPlusJetTag" + systlabel))
        else:
            process.flashggSystTagMerger.src.append(cms.InputTag("flashggTagSorter" + systlabel))

# ttH tags use large BDTs and DNNs that take up lots of memory and cause crashes with normal workflow
# This function modifies the workflow so that systematic variations are evaluated in a single instance of the tagger, rather than individual instances for each variation
def modifySystematicsWorkflowForttH(process, systlabels, phosystlabels, metsystlabels, jetsystlabels):
    # Set lists of systematics for each tag
    for tag in ["flashggTTHLeptonicTag", "flashggTTHHadronicTag"]:
        getattr(process, tag).DiPhotonSuffixes = cms.vstring(phosystlabels)
        getattr(process, tag).JetsSuffixes = cms.vstring(jetsystlabels)
        getattr(process, tag).MetSuffixes = cms.vstring(metsystlabels)

        getattr(process, tag).ModifySystematicsWorkflow = cms.bool(True)
        getattr(process, tag).UseLargeMVAs = cms.bool(True) # enable memory-intensive MVAs

    # Run cms.Sequence(process.flashggTTHLeptonicTag + process.flashggTTHHadronicTag) once at the beginning, put tag sorters for each systematic afterwards, and finally flashggSystTagMerger at the very end
    process.p.remove(process.flashggTagSorter)
    process.p.replace(process.flashggSystTagMerger, cms.Sequence(process.flashggTTHLeptonicTag + process.flashggTTHHadronicTag)*process.flashggTagSorter*process.flashggSystTagMerger)

    for systlabel in systlabels:
        if systlabel == "":
            continue
        process.p.remove(getattr(process, 'flashggTagSorter' + systlabel))
        process.p.replace(process.flashggSystTagMerger, getattr(process, 'flashggTagSorter' + systlabel) * process.flashggSystTagMerger) 
    setattr(getattr(process, 'flashggTagSorter'+systlabel), 'TagPriorityRanges', cms.VPSet( cms.PSet(TagName = cms.InputTag('flashggTTHLeptonicTag', systlabel)), cms.PSet(TagName = cms.InputTag('flashggTTHHadronicTag', systlabel)) ))

def allowLargettHMVAs(process):
    for tag in ["flashggTTHLeptonicTag", "flashggTTHHadronicTag"]:
        getattr(process, tag).UseLargeMVAs = cms.bool(True) # enable memory-intensive MVAs

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
    vpsetlist += [process.flashggDiPhotonSystematics.SystMethods2D, process.flashggMuonSystematics.SystMethods2D, process.flashggElectronSystematics.SystMethods2D]
    vpsetlist += [getattr(process,"flashggJetSystematics%i"%i).SystMethods2D for i in range(len(UnpackedJetCollectionVInputTag))]
    for vpset in vpsetlist:
        for pset in vpset:
            if type(pset.NSigmas) == type(cms.vint32()):
                pset.NSigmas = cms.vint32() # Do not perform shift
            else:
                pset.NSigmas = cms.PSet( firstVar = cms.vint32(), secondVar = cms.vint32() ) # Do not perform shift - 2D case
            if hasattr(pset,"SetupUncertainties"):
                pset.SetupUncertainties = False

def customizeSystematicsForData(process):
    customizePhotonSystematicsForData(process)
    customizeLeptonSystematicsForData(process)
    customizeJetSystematicsForData(process)

def customizeVPSetForData(systs, phScaleBins):
    newvpset = cms.VPSet()
    for pset in systs:
        if (pset.Label.value().count("Scale") or pset.Label.value().count("SigmaEOverESmearing")) and not pset.Label.value().count("Gain"):
            pset.ApplyCentralValue = cms.bool(True) # Turn on central shift for data (it is off for MC)
            if type(pset.NSigmas) == type(cms.vint32()):
                pset.NSigmas = cms.vint32() # Do not perform shift
            else:
                pset.NSigmas = cms.PSet( firstVar = cms.vint32(), secondVar = cms.vint32() ) # Do not perform shift - 2D case
            if pset.Label.value().count("Scale") and phScaleBins != None: 
                pset.BinList = phScaleBins
            newvpset += [pset]
    return newvpset

def customizePhotonSystematicsForData(process):
    # By default remove the systematic entirely (central value and shifts)
    # For scale: put in central value, but omit shifts
    # TODO: this is wrong for sigE/E and possibly others - check!

    photonScaleBinsData = getattr(process,'photonScaleBinsData',None)
    if hasattr(process,'photonScaleBinsData'):
        print photonScaleBinsData, process.photonScaleBinsData
    process.flashggDiPhotonSystematics.SystMethods = customizeVPSetForData(process.flashggDiPhotonSystematics.SystMethods, photonScaleBinsData)
    process.flashggDiPhotonSystematics.SystMethods2D = customizeVPSetForData(process.flashggDiPhotonSystematics.SystMethods2D, photonScaleBinsData)

def customizeLeptonSystematicsForData(process):
    # Remove systematics entirely

    systprodlist = [process.flashggMuonSystematics,process.flashggElectronSystematics]
    for systprod in systprodlist:
        systprod.SystMethods = cms.VPSet() # empty everything

def customizeJetSystematicsForData(process):
    # By default remove the systematic entirely
    # For JEC, re-do central value in case the global tag has been updated
  #  process.jec.toGet[0].tag = cms.string(process.jec.toGet[0].tag.value().replace("MC","DATA"))
  #  process.jec.connect = cms.string(process.jec.connect.value().replace("MC","DATA"))
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

def useEGMTools(process):
    # remove old scales
    for isyst in [ process.MCScaleHighR9EB, process.MCScaleLowR9EB, process.MCScaleHighR9EE, process.MCScaleLowR9EE ]:
            process.flashggDiPhotonSystematics.SystMethods.remove(isyst)

    # add EGM scales
    for isyst in [ process.MCScaleHighR9EB_EGM, process.MCScaleLowR9EB_EGM, process.MCScaleHighR9EE_EGM, process.MCScaleLowR9EE_EGM ]:
        process.flashggDiPhotonSystematics.SystMethods.insert(0, isyst)

    # remove old smearings
    for isyst in [ process.MCSmearHighR9EE, process.MCSmearLowR9EE, process.MCSmearHighR9EB, process.MCSmearLowR9EB, process.SigmaEOverESmearing, process.SigmaEOverEShift ]:
            process.flashggDiPhotonSystematics.SystMethods.remove(isyst)

    # add EGM smearings (2D)
    process.flashggDiPhotonSystematics.SystMethods2D.extend([
            process.MCSmearHighR9EE_EGM,
            process.MCSmearLowR9EE_EGM,
            process.MCSmearHighR9EB_EGM,
            process.MCSmearLowR9EB_EGM,
            ])
    
    # add sigmaE/E correction and systematics
    process.flashggDiPhotonSystematics.SystMethods.extend( [process.SigmaEOverESmearing_EGM, process.SigmaEOverEShift] )

