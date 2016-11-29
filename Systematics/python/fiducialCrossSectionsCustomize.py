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
def diPhoGenVariable(name,  pfx):
    return "gen%s := %s%s" % ( name.capitalize(), pfx, name )

# ----------------------------------------------------------------------------------------------------------------
def diPhoRecoVariable(name, pfx=None):
    return "reco%s := %s%s" % ( name.capitalize(), pfx, name )

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
    return map(lambda x: (("gen%s%s := "+x[1]) % (x[0],name.capitalize(),name)), zip(("Lead","Sublead"),expressions))

# ----------------------------------------------------------------------------------------------------------------
def phoRecoVariable(name, expressions):
    return map(lambda x: (("reco%s%s := "+x[1]) % (x[0],name.capitalize(),name)), zip(("Lead","Sublead"),expressions))

# ----------------------------------------------------------------------------------------------------------------
def getGenVariables(isRecoTag=True):
#    diPhoVariables = ["mass","pt","rapidity"]
    diPhoVariables = ["mass", "pt", "pz", "energy"]
#    diPhoVariables = ["mass", "pt"]
    phoVariables = ["pt","eta","phi"]
    
    pfx = genDiphoPfx(isRecoTag)
    dipho = map(lambda x: diPhoGenVariable(x,pfx), diPhoVariables)
    
    expressions = genPhoExpr(isRecoTag)
    legs = reduce(lambda z,w: z+w, (map (lambda x: phoGenVariable(x,expressions), phoVariables) ) )
        
    if isRecoTag:
        legs.extend( ["genLeadGenIso := ? diPhoton.leadingPhoton.hasMatchedGenPhoton ? diPhoton.leadingPhoton.userFloat(\"genIso\") : -99",
                      "genSubleadGenIso := ? diPhoton.subLeadingPhoton.hasMatchedGenPhoton ? diPhoton.subLeadingPhoton.userFloat(\"genIso\") : -99"] )
    else:
        legs.extend( ["genLeadGenIso := leadingExtra.genIso",
                      "genSubleadGenIso:= subLeadingExtra.genIso"] )
        
#    return dipho
    return dipho+legs

# ----------------------------------------------------------------------------------------------------------------
def getRecoVariables(isRecoTag=True):
    diPhoVariables = ["mass","pt","rapidity"]
    phoVariables = ["pt","eta","phi"]

    pfx = recoDiphoPfx(isRecoTag)
    dipho = map(lambda x: diPhoRecoVariable(x,pfx), diPhoVariables)
    
    expressions = recoPhoExpr(isRecoTag)
    legs = reduce(lambda z,w: z+w, (map (lambda x: phoRecoVariable(x,expressions) , phoVariables ) ) )
    
#    return dipho
    return dipho+legs

# ----------------------------------------------------------------------------------------------------------------
def bookHadronicActivityProducers(process,processId,recoDiphotonTags,genDiphotons,recoJetCollections=None,genJetCollection="slimmedGenJets"):
    if not recoJetCollections:
        from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag
        recoJetCollections = UnpackedJetCollectionVInputTag
    

    
    recoJets2p5 = cms.VInputTag()
    recoJets4p7 = cms.VInputTag()
    print recoJetCollections
    for icoll,coll in enumerate(recoJetCollections):        
        if( not hasattr(process,"filteredRecoJetsEta2p5%d" % icoll) ): 
            setattr(process,"filteredRecoJetsEta2p5%d" % icoll,cms.EDFilter("FlashggJetSelector",
                                                                            src=coll,
                                                                            cut=cms.string("pt>%f && abs(eta)<2.5" % jetPtCut),
                                                                            ) )
            recoJets2p5.append("filteredRecoJetsEta2p5%d" % icoll)
        if( not hasattr(process,"filteredRecoJetsEta4p7%d" % icoll) ): 
            setattr(process,"filteredRecoJetsEta4p7%d" % icoll,cms.EDFilter("FlashggJetSelector",
                                                                            src=coll,
                                                                            cut=cms.string("pt>%f && abs(eta)<4.7" % jetPtCut),
                                                                            ) )

            recoJets4p7.append("filteredRecoJetsEta4p7%d" % icoll)


    if( not hasattr(process,"flashggRecoHadronicActivity2p5") ): 
        process.flashggRecoHadronicActivity2p5 = cms.EDProducer("FlashggDiPhotonTagHadronicActivityProducer",
                                                                src=recoJets2p5,
                                                                veto=cms.InputTag(recoDiphotonTags)
                                                                )
    if( not hasattr(process,"flashggRecoHadronicActivity4p7") ): 
        process.flashggRecoHadronicActivity4p7 = cms.EDProducer("FlashggDiPhotonTagHadronicActivityProducer",
                                                                src=recoJets4p7,
                                                                veto=cms.InputTag(recoDiphotonTags)
                                                                )
    if not processId=="Data":
        if( not hasattr(process,"filteredGenJetsEta2p5") ): 
            process.filteredGenJetsEta2p5 = cms.EDFilter("GenJetSelector",
                                                         src=cms.InputTag(genJetCollection),
                                                         cut=cms.string("pt>%f && abs(eta)<2.5" % jetPtCut),
                                                         )
            process.flashggGenHadronicActivity2p5 = cms.EDProducer("FlashggGenHadronicActivityProducer",
                                                                   src=cms.InputTag("filteredGenJetsEta2p5"),
                                                                   veto=cms.InputTag(genDiphotons)
                                                                   )
        if( not hasattr(process,"filteredGenJetsEta4p7") ): 
            process.filteredGenJetsEta4p7 = cms.EDFilter("GenJetSelector",
                                                         src=cms.InputTag(genJetCollection),
                                                         cut=cms.string("pt>%f && abs(eta)<4.7" % jetPtCut),
                                                     )

            process.flashggGenHadronicActivity4p7 = cms.EDProducer("FlashggGenHadronicActivityProducer",
                                                                   src=cms.InputTag("filteredGenJetsEta4p7"),
                                                                   veto=cms.InputTag(genDiphotons)
                                                                   )
        
# ----------------------------------------------------------------------------------------------------------------
def getJetKinVariables(pre,post,variables,nmax):
    return reduce(lambda z,w: z+w, 
                  map(lambda x: 
                      map(lambda y: ("%sJet%s%s%d := ? numberOfDaughters > %d ? daughter(%d).%s : 0" % (pre,post,y.capitalize(),x,x,x,y)), variables),
                      xrange(nmax)
                      )
                  )

# ----------------------------------------------------------------------------------------------------------------
def addJetGlobalVariables(process,dumper,src,pre,post):    
    import flashgg.Taggers.dumperConfigTools as cfgTools

    variables  = [ "%sNjets%s:=numberOfDaughters" % (pre,post) ]
    variables += getJetKinVariables(pre,post,["pt","eta","rapidity"],5)
    variables += [ "%sDijetMass%s := ? numberOfDaughters > 1 ? sqrt( (daughter(0).energy+daughter(1).energy)^2 - (daughter(0).px+daughter(1).px)^2 - (daughter(0).py+daughter(1).py)^2 - (daughter(0).pz+daughter(1).pz)^2 ) : 0" % (pre,post) ]

    cfgTools.addGlobalFloats(process,dumper.globalVariables,src,variables)

# ----------------------------------------------------------------------------------------------------------------
def addGenGlobalVariables(process,dumper):    
    addJetGlobalVariables(process,dumper,"flashggGenHadronicActivity2p5","gen","2p5")
    addJetGlobalVariables(process,dumper,"flashggGenHadronicActivity4p7","gen","4p7")

# ----------------------------------------------------------------------------------------------------------------
def addRecoGlobalVariables(process,dumper):    
    addJetGlobalVariables(process,dumper,"flashggRecoHadronicActivity2p5","reco","2p5")
    addJetGlobalVariables(process,dumper,"flashggRecoHadronicActivity4p7","reco","4p7")
    
    
# ----------------------------------------------------------------------------------------------------------------
def addGenOnlyAnalysis(process,processId,acceptance,tagList,systlabels,pdfWeights=None,recoJetCollections=None):
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
    
    bookHadronicActivityProducers(process,processId,"flashggTagSorter","flashggTaggedGenDiphotons",recoJetCollections,genJetCollection="slimmedGenJets")
    addGenGlobalVariables(process,process.genDiphotonDumper)
    addRecoGlobalVariables(process,process.genDiphotonDumper)

    dumpPdfWeights,nPdfWeights,nAlphaSWeights,nScaleWeights=False,-1,-1,-1 
    if pdfWeights:
        dumpPdfWeights,nPdfWeights,nAlphaSWeights,nScaleWeights=pdfWeights
        
    genVariables  = getGenVariables(False)
    recoVariables = getRecoVariables(False)
    
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
                                 variables=genVariables+recoVariables,
                                 dumpPdfWeights=dumpPdfWeights,
                                 nPdfWeights=nPdfWeights,
                                 nAlphaSWeights=nAlphaSWeights,
                                 nScaleWeights=nScaleWeights
                                 )
            
            
## process.pfid = cms.Path(process.genFilter*process.flashggGenDiPhotonsSequence*process.flashggTaggedGenDiphotons*process.genDiphotonDumper)
    process.pfid = cms.Path(process.genFilter*process.genDiphotonDumper)
    


def addObservables(process, dumper, processId, recoJetCollections=None):
    bookHadronicActivityProducers(process,processId,"flashggTagSorter","flashggTaggedGenDiphotons",recoJetCollections,genJetCollection="slimmedGenJets")
    addRecoGlobalVariables(process,dumper)
    if not processId=="Data":    
        addGenGlobalVariables(process,dumper)
    


