import FWCore.ParameterSet.Config as cms

leadCut = 1./3.
subLeadCut = 1./4.
isoCut = 10.
etaCut = 2.5
jetPtCut = 30.

# ----------------------------------------------------------------------------------------------------------------
def getAccRecoCut():
    return "(leadingPhoton.userFloat(\"genIso\") < %(iso)f && subLeadingPhoton.userFloat(\"genIso\") < %(iso)f && abs(leadingPhoton.matchedGenPhoton.eta) <%(eta)f && abs(subLeadingPhoton.matchedGenPhoton.eta) <%(eta)f && leadingPhoton.matchedGenPhoton.pt / genP4.mass > %(lead)f && subLeadingPhoton.matchedGenPhoton.pt / genP4.mass > %(sub)f)" % {
        "iso" : isoCut,
        "eta" : etaCut,
        "lead" : leadCut,
        "sub"  : subLeadCut
        }
#    return "1"

# ----------------------------------------------------------------------------------------------------------------
def getAccGenCut():
    return "(leadingExtra.genIso < %(iso)f && subLeadingExtra.genIso < %(iso)f && abs(leadingPhoton.eta) <%(eta)f && abs(subLeadingPhoton.eta) <%(eta)f && leadingPhoton.pt / mass > %(lead)f && subLeadingPhoton.pt / mass > %(sub)f)" % {
        "iso" : isoCut,
        "eta" : etaCut,
        "lead" : leadCut,
        "sub"  : subLeadCut
        }
#    return "1"

# ----------------------------------------------------------------------------------------------------------------
def genDiphoPfx(isRecoTag):
    return "diPhoton.genP4." if isRecoTag else ""

# ----------------------------------------------------------------------------------------------------------------
def recoDiphoPfx(isRecoTag):
    return "diPhoton." if isRecoTag else "recoTagObj.diPhoton."

# ----------------------------------------------------------------------------------------------------------------
def diPhoGenVariable(name,  pfx, nbins=None, vmin=None, vmax=None):
    if nbins:
        return "gen%s[%i,%f,%f] := %s%s" % ( name.capitalize(), pfx, name, nbins, vmin, vmax )
    else:
        return "gen%s := %s%s" % ( name.capitalize(), pfx, name.split("[")[0] )

# ----------------------------------------------------------------------------------------------------------------
def diPhoRecoVariable(name, pfx=None, nbins=None, vmin=None, vmax=None):
    if nbins:
        return "reco%s[%i,%f,%f] := %s%s" % ( name.capitalize(), pfx, name.split("[")[0], nbins, vmin, vmax )
    else:
        print "name %s" % (name)
        print "name split [ %s" % (name.split("[")[0])
        print "reco%s := %s%s" % ( name.capitalize(), pfx, name.split("[")[0] )
        return "reco%s := %s%s" % ( name.capitalize(), pfx, name.split("[")[0] )

# ----------------------------------------------------------------------------------------------------------------
def genPhoExpr(isRecoTag):
    return ("? diPhoton.leadingPhoton.hasMatchedGenPhoton ? diPhoton.leadingPhoton.%s : -99.",
            "? diPhoton.subLeadingPhoton.hasMatchedGenPhoton ? diPhoton.subLeadingPhoton.%s : -99.") if isRecoTag else ("leadingPhoton.%s",
                                                                                                                                  "subLeadingPhoton.%s")

# ----------------------------------------------------------------------------------------------------------------
def recoPhoExpr(isRecoTag):
    return ("diPhoton.leadingPhoton.%s",
            "diPhoton.subLeadingPhoton.%s") if isRecoTag else("recoTagObj.diPhoton.leadingPhoton.%s",
                                                                  "recoTagObj.diPhoton.subLeadingPhoton.%s")

# ----------------------------------------------------------------------------------------------------------------
def phoGenVariable(name, expressions):
    return map(lambda x: (("gen%s%s := "+x[1]) % (x[0],name.capitalize(),name.split("[")[0]) ), zip(("Lead","Sublead"),expressions))

# ----------------------------------------------------------------------------------------------------------------
def phoRecoVariable(name, expressions):
    print map(lambda x: (("reco%s%s := "+x[1]) % (x[0],name.capitalize(),name.split("[")[0])), zip(("Lead","Sublead"),expressions))
    return map(lambda x: (("reco%s%s := "+x[1]) % (x[0],name.capitalize(),name.split("[")[0])), zip(("Lead","Sublead"),expressions))

# ----------------------------------------------------------------------------------------------------------------
def getGenVariables(isRecoTag=True):
#    diPhoVariables = ["mass","pt","rapidity"]
###    diPhoVariables = ["mass", "pt[-1,(0.0:15.0:30.0:45.0:85.0:125.0:200.0:10000.0)]", "pz", "energy"]
#    diPhoVariables = ["mass", "pt", "pz", "energy"]
    diPhoVariables = ["pt"]
###    phoVariables = ["pt","eta","phi"]
    phoVariables = []
    
    pfx = genDiphoPfx(isRecoTag)
    dipho = map(lambda x: diPhoGenVariable(x,pfx), diPhoVariables)
    
    expressions = genPhoExpr(isRecoTag)
###    legs = reduce(lambda z,w: z+w, (map (lambda x: phoGenVariable(x,expressions), phoVariables) ) )
        
###    if isRecoTag:
####        legs.extend( ["genLeadGenIso[-1,(0.0:10.0:13000.0)] := ? diPhoton.leadingPhoton.hasMatchedGenPhoton ? diPhoton.leadingPhoton.userFloat(\"genIso\") : -99",
####                      "genSubleadGenIso[-1,(0.0:10.0:13000.0)] := ? diPhoton.subLeadingPhoton.hasMatchedGenPhoton ? diPhoton.subLeadingPhoton.userFloat(\"genIso\") : -99"] )
###        legs.extend( ["genLeadGenIso := ? diPhoton.leadingPhoton.hasMatchedGenPhoton ? diPhoton.leadingPhoton.userFloat(\"genIso\") : -99",
###                      "genSubleadGenIso := ? diPhoton.subLeadingPhoton.hasMatchedGenPhoton ? diPhoton.subLeadingPhoton.userFloat(\"genIso\") : -99"] )
###    else:
####        legs.extend( ["genLeadGenIso[-1,(0.0:10.0:13000.0)] := leadingExtra.genIso",
####                      "genSubleadGenIso[-1,(0.0:10.0:13000.0)]:= subLeadingExtra.genIso"] )
###        legs.extend( ["genLeadGenIso := leadingExtra.genIso",
###                      "genSubleadGenIso:= subLeadingExtra.genIso"] )
        
    return dipho
###    return dipho+legs

# ----------------------------------------------------------------------------------------------------------------
def getRecoVariables(isRecoTag=True):
#    diPhoVariables = ["mass","pt[-1,(0.0:15.0:30.0:45.0:85.0:125.0:200.0:10000.0)]","rapidity[-1,(-5.0:-2.5:0.0:2.5:5.0)]"]
#    diPhoVariables = ["mass","pt[-1,(0.0:15.0:30.0:45.0:85.0:125.0:200.0:10000.0)]","rapidity"]
    diPhoVariables = ["pt"]
#    phoVariables = ["pt","eta","phi"]
    phoVariables = []
    

    pfx = recoDiphoPfx(isRecoTag)
    dipho = map(lambda x: diPhoRecoVariable(x,pfx), diPhoVariables)
    
    expressions = recoPhoExpr(isRecoTag)
###    legs = reduce(lambda z,w: z+w, (map (lambda x: phoRecoVariable(x,expressions) , phoVariables ) ) )
    
    return dipho
###    return dipho+legs

# ----------------------------------------------------------------------------------------------------------------
def bookHadronicActivityProducers(process,processId,tagSequence,recoDiphotons,recoDiphotonTags,genDiphotons,recoJetCollections=None,genJetCollection="slimmedGenJets"):
    if not recoJetCollections:
        from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag
        recoJetCollections = UnpackedJetCollectionVInputTag
    

    
    recoJets2p5 = cms.VInputTag()
    recoJets4p7 = cms.VInputTag()
    print recoJetCollections
    pos = tagSequence.index(recoDiphotonTags) - 1
    for icoll,coll in enumerate(recoJetCollections):        
        if( not hasattr(process,"filteredRecoJetsEta2p5%d" % icoll) ): 
            setattr(process,"filteredRecoJetsEta2p5%d" % icoll,cms.EDFilter("FlashggJetSelector",
                                                                            src=coll,
                                                                            cut=cms.string("pt>%f && abs(eta)<2.5 && passesJetID('Loose')" % jetPtCut),
                                                                            ) )
            recoJets2p5.append("filteredRecoJetsEta2p5%d" % icoll)
            tagSequence.insert(pos, getattr(process,"filteredRecoJetsEta2p5%d" % icoll))
            pos += 1
            
        if( not hasattr(process,"filteredRecoJetsEta4p7%d" % icoll) ): 
            setattr(process,"filteredRecoJetsEta4p7%d" % icoll,cms.EDFilter("FlashggJetSelector",
                                                                            src=coll,
                                                                            cut=cms.string("pt>%f && abs(eta)<4.7 && passesJetID('Loose')" % jetPtCut),
                                                                            ) )

            recoJets4p7.append("filteredRecoJetsEta4p7%d" % icoll)
            tagSequence.insert(pos, getattr(process,"filteredRecoJetsEta4p7%d" % icoll))
            pos += 1

    if( not hasattr(process,"flashggRecoHadronicActivity2p5") ): 
        process.flashggRecoHadronicActivity2p5 = cms.EDProducer("FlashggDiPhotonHadronicActivityProducer",
                                                                src=recoJets2p5,
                                                                veto=cms.InputTag(recoDiphotons)
                                                                )
        tagSequence.insert(pos, getattr(process,"flashggRecoHadronicActivity2p5"))
        pos += 1
    if( not hasattr(process,"flashggRecoHadronicActivity4p7") ): 
        process.flashggRecoHadronicActivity4p7 = cms.EDProducer("FlashggDiPhotonHadronicActivityProducer",
                                                                src=recoJets4p7,
                                                                veto=cms.InputTag(recoDiphotons)
                                                                )
        tagSequence.insert(pos, getattr(process,"flashggRecoHadronicActivity4p7"))
        pos += 1
        
    recoDiphotonTags.CompositeCandidateTags.jets2p5 = cms.InputTag("flashggRecoHadronicActivity2p5")
    recoDiphotonTags.CompositeCandidateTags.jets4p7 = cms.InputTag("flashggRecoHadronicActivity4p7")
    
    if not processId=="Data":
        if( not hasattr(process,"filteredGenJetsEta2p5") ): 
            process.filteredGenJetsEta2p5 = cms.EDFilter("GenJetSelector",
                                                         src=cms.InputTag(genJetCollection),
#                                                         cut=cms.string("pt>%f && abs(eta)<2.5 && nCarrying(0.90)>1" % jetPtCut),
                                                         cut=cms.string("pt>%f && abs(eta)<2.5" % jetPtCut),
                                                         )
            process.flashggGenHadronicActivity2p5 = cms.EDProducer("FlashggGenHadronicActivityProducer",
                                                                   src=cms.InputTag("filteredGenJetsEta2p5"),
                                                                   veto=cms.InputTag(genDiphotons)
                                                                   )
        if( not hasattr(process,"filteredGenJetsEta4p7") ): 
            process.filteredGenJetsEta4p7 = cms.EDFilter("GenJetSelector",
                                                         src=cms.InputTag(genJetCollection),
#                                                         cut=cms.string("pt>%f && abs(eta)<4.7 && nCarrying(0.90)>1" % jetPtCut),
                                                         cut=cms.string("pt>%f && abs(eta)<4.7" % jetPtCut),
                                                     )

            process.flashggGenHadronicActivity4p7 = cms.EDProducer("FlashggGenHadronicActivityProducer",
                                                                   src=cms.InputTag("filteredGenJetsEta4p7"),
                                                                   veto=cms.InputTag(genDiphotons)
                                                                   )
        if( not hasattr(process,"filteredGenJetsEtaInclusive") ): 
            process.filteredGenJetsEtaInclusive = cms.EDFilter("GenJetSelector",
                                                         src=cms.InputTag(genJetCollection),
#                                                         cut=cms.string("pt>%f && abs(eta)<2.5 && nCarrying(0.90)>1" % jetPtCut),
                                                         cut=cms.string("pt>%f" % jetPtCut),
                                                         )
            process.flashggGenHadronicActivityInclusive = cms.EDProducer("FlashggGenHadronicActivityProducer",
                                                                   src=cms.InputTag("filteredGenJetsEtaInclusive"),
                                                                   veto=cms.InputTag(genDiphotons)
                                                                   )
        
# ----------------------------------------------------------------------------------------------------------------
def getJetKinVariables(pre,post,variables,nmax, getter):
    return reduce(lambda z,w: z+w, 
                  map(lambda x: 
#                      map(lambda y: ("%sJet%s%s%d := ? numberOfDaughters > %d ? daughter(%d).%s : 0" % (pre,post,y.split("[")[0].capitalize(),x,x,x,y.split("[")[0])), variables),
                      map(lambda y: ("%sJet%s%s := ? %snumberOfDaughters > %d ? %sdaughter(%d).%s : -999" % (pre,  post,  (str(x)+"[").join(y.capitalize().rsplit("[",1)),  getter,  x,  getter,  x,  y.split("[")[0])),  variables),
                      xrange(nmax)
                      )
                  )

# ----------------------------------------------------------------------------------------------------------------
def addJetGlobalVariables(process,dumper,src,pre,post,getter=""):    
    import flashgg.Taggers.dumperConfigTools as cfgTools

    if getter != "": getter += "."
    variables  = [ "%(pre)sNjets%(post)s[-1,(-0.5:0.5:1.5:2.5:3.5:100)]:= %(getter)snumberOfDaughters" % locals() ]
#    variables  = [ "%sNjets%s:=numberOfDaughters" % (pre,post) ]
#    variables += getJetKinVariables(pre,post,["pt[667,0.0,10005.0]","eta[50,-5.0,5.0]","rapidity[50,0.0,10.0]"],5)
    variables += getJetKinVariables(pre,post,["pt[-1,(0.0:40.0:65.0:105.0:200.0:13000.0)]","eta","rapidity","numberOfDaughters[200,-0.5,199.5]"],6, getter)
#    print variables
###    variables += getJetKinVariables(pre,post,["pt","eta","rapidity"],5)
###    variables += [ "%sDijetMass%s := ? numberOfDaughters > 1 ? sqrt( (daughter(0).energy+daughter(1).energy)^2 - (daughter(0).px+daughter(1).px)^2 - (daughter(0).py+daughter(1).py)^2 - (daughter(0).pz+daughter(1).pz)^2 ) : 0" % (pre,post) ]
    
    if src:
        cfgTools.addGlobalFloats(process,dumper.globalVariables,src,variables)
    else:
        for cat in dumper.categories:
            cfgTools.addVariables(cat.variables,variables)
        
# ----------------------------------------------------------------------------------------------------------------
def addGenGlobalVariables(process,dumper):    
    addJetGlobalVariables(process,dumper,"flashggGenHadronicActivity2p5","gen","2p5")
    addJetGlobalVariables(process,dumper,"flashggGenHadronicActivity4p7","gen","4p7")
    addJetGlobalVariables(process,dumper,"flashggGenHadronicActivityInclusive","gen","Inclusive")

# ----------------------------------------------------------------------------------------------------------------
def addRecoGlobalVariables(process,dumper,tagGetter=""):    
    if tagGetter != "": tagGetter += "."
    addJetGlobalVariables(process,dumper,None,"reco","2p5","%sgetCompCand('jets2p5')" % tagGetter)
    addJetGlobalVariables(process,dumper,None,"reco","4p7","%sgetCompCand('jets4p7')" % tagGetter)
    
    
# ----------------------------------------------------------------------------------------------------------------
def addGenOnlyAnalysis(process,processId,tagSequence,acceptance,tagList,systlabels,pdfWeights=None,recoJetCollections=None):
    import itertools
    import flashgg.Taggers.dumperConfigTools as cfgTools
    
    accCut = getAccGenCut()
    cut = "1"
    if acceptance == "IN": cut = accCut
    elif acceptance == "OUT": cut = "!(%s)" % accCut
    

    process.load("flashgg.MicroAOD.flashggGenDiPhotonsSequence_cff")
    process.flashggSelectedGenDiPhotons.cut = cut
    process.flashggSortedGenDiPhotons.maxNumber = 1

    process.load("flashgg.Taggers.flashggTaggedGenDiphotons_cfi")
    process.flashggTaggedGenDiphotons.src  = "flashggSortedGenDiPhotons"
    process.flashggTaggedGenDiphotons.tags = "flashggTagSorter"
    process.flashggTaggedGenDiphotons.remap = process.tagsDumper.classifierCfg.remap
## process.flashggTaggedGenDiphotons.tags = "flashggSystTagMerger"

    process.load("flashgg.Taggers.genDiphotonDumper_cfi")
    process.genDiphotonDumper.dumpTrees = True
    process.genDiphotonDumper.dumpWorkspace = False
    process.genDiphotonDumper.src = "flashggTaggedGenDiphotons"

    from flashgg.Taggers.globalVariables_cff import globalVariables
    process.genDiphotonDumper.dumpGlobalVariables = True
    process.genDiphotonDumper.globalVariables = globalVariables
    
    ## bookHadronicActivityProducers(process,processId,"flashggTagSorter","flashggTaggedGenDiphotons",recoJetCollections,genJetCollection="slimmedGenJets")
    bookHadronicActivityProducers(process,processId,tagSequence,"flashggDiPhotonSystematics",process.flashggSigmaMoMpToMTag,"flashggTaggedGenDiphotons",recoJetCollections,genJetCollection="slimmedGenJets") ##FIXME 
    addGenGlobalVariables(process,process.genDiphotonDumper)
    addRecoGlobalVariables(process,process.genDiphotonDumper,"tag()")

    dumpPdfWeights,nPdfWeights,nAlphaSWeights,nScaleWeights=False,-1,-1,-1 
    if pdfWeights:
        dumpPdfWeights,nPdfWeights,nAlphaSWeights,nScaleWeights=pdfWeights
        
    genVariables  = getGenVariables(False)
    recoVariables = getRecoVariables(False)
    extravars = ["leadmva := recoTagObj.diPhoton.leadingView.phoIdMvaWrtChosenVtx", "subleadmva := recoTagObj.diPhoton.subLeadingView.phoIdMvaWrtChosenVtx"]

#    return ("diPhoton.leadingPhoton.%s",
#            "diPhoton.subLeadingPhoton.%s") if isRecoTag else("recoTagObj.diPhoton.leadingPhoton.%s",
#                                                                  "recoTagObj.diPhoton.subLeadingPhoton.%s")

    cfgTools.addCategory(process.genDiphotonDumper,
                         "NoTag", 'isTagged("")',1,
                         variables=genVariables,
                         dumpPdfWeights=dumpPdfWeights,
                         nPdfWeights=nPdfWeights,
                         nAlphaSWeights=nAlphaSWeights,
                         nScaleWeights=nScaleWeights
                         )

    for tag in tagList:
        tagName,subCats = tag
    # need to define all categories explicitely because cut-based classifiers does not look at sub-category number
        for isub in xrange(subCats):
            cfgTools.addCategory(process.genDiphotonDumper,
                                 "%s_%d" % ( tagName, isub ), 
                                 'isTagged("%s") && categoryNumber == %d' % (tagName, isub),0,
                                 variables=genVariables+recoVariables+extravars,
                                 dumpPdfWeights=dumpPdfWeights,
                                 nPdfWeights=nPdfWeights,
                                 nAlphaSWeights=nAlphaSWeights,
                                 nScaleWeights=nScaleWeights
                                 )
            
            
## process.pfid = cms.Path(process.genFilter*process.flashggGenDiPhotonsSequence*process.flashggTaggedGenDiphotons*process.genDiphotonDumper)
    process.pfid = cms.Path(process.genFilter*process.genDiphotonDumper)
    

def bookCompositeObjects(process,processId,tagSequence,recoJetCollections=None):
    ## bookHadronicActivityProducers(process,processId,"flashggTagSorter","flashggTaggedGenDiphotons",recoJetCollections,genJetCollection="slimmedGenJets")
    bookHadronicActivityProducers(process,processId,tagSequence,"flashggDiPhotonSystematics",process.flashggSigmaMoMpToMTag,"flashggTaggedGenDiphotons",recoJetCollections,genJetCollection="slimmedGenJets")

def addObservables(process, dumper, processId, recoJetCollections=None):
    addRecoGlobalVariables(process,dumper)
    if not processId=="Data":    
        addGenGlobalVariables(process,dumper)
    
###def extraReplacementsHook(newseq,systlabel,systtype):
###    from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet,massSearchReplaceAnyInputTag
###    if systtype=="jet":
###        from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag
###        recoJetCollections = UnpackedJetCollectionVInputTag
###        for icoll,coll in enumerate(recoJetCollections):        
###            massSearchReplaceAnyInputTag(newseq,cms.InputTag("filteredRecoJetsEta2p5%d" % icoll),cms.InputTag("filteredRecoJetsEta2p5%d%s" % (icoll,systlabel)))
###            massSearchReplaceAnyInputTag(newseq,cms.InputTag("filteredRecoJetsEta4p7%d" % icoll),cms.InputTag("filteredRecoJetsEta4p7%d%s" % (icoll,systlabel)))
###        massSearchReplaceAnyInputTag(newseq,cms.InputTag("flashggRecoHadronicActivity2p5"),cms.InputTag("flashggRecoHadronicActivity2p5%s" % (systlabel)))
###        massSearchReplaceAnyInputTag(newseq,cms.InputTag("flashggRecoHadronicActivity4p7"),cms.InputTag("flashggRecoHadronicActivity4p7%s" % (systlabel)))
