import FWCore.ParameterSet.Config as cms

leadCut = 1./3.
subLeadCut = 1./4.
isoCut = 10.
etaCut = 2.5

# ----------------------------------------------------------------------------------------------------------------
def getAccRecoCut():
    return "(leadingPhoton.userFloat(\"genIso\") < %(iso)f && subLeadingPhoton.userFloat(\"genIso\") < %(iso)f && abs(leadingPhoton.matchedGenPhoton.eta) <%(eta)f && abs(subLeadingPhoton.matchedGenPhoton.eta) <%(eta)f && leadingPhoton.matchedGenPhoton.pt / genP4.mass > %(lead)f && subLeadingPhoton.matchedGenPhoton.pt / genP4.mass > %(sub)f)" % {
        "iso" : isoCut,
        "eta" : etaCut,
        "lead" : leadCut,
        "sub"  : subLeadCut
        }

# ----------------------------------------------------------------------------------------------------------------
def getAccGenCut():
    return "(leadingExtra.genIso < %(iso)f && subLeadingExtra.genIso < %(iso)f && abs(leadingPhoton.eta) <%(eta)f && abs(subLeadingPhoton.eta) <%(eta)f && leadingPhoton.pt / mass > %(lead)f && subLeadingPhoton.pt / mass > %(sub)f)" % {
        "iso" : isoCut,
        "eta" : etaCut,
        "lead" : leadCut,
        "sub"  : subLeadCut
        }
    

# ----------------------------------------------------------------------------------------------------------------
def getGenVariables(isRecoTag=True):
    diphoVariables = ["mass","pt","rapidity"]
    phoVariables = ["pt","eta","phi"]
    capPhoVariables = map(lambda x: x[0].capitalize()+x[1:], phoVariables )
    
    pfx = "diPhoton().genP4()." if isRecoTag else ""
    dipho = map( lambda x: "gen%s := %s%s" % ( x.capitalize(), pfx, x ) , diphoVariables )
    expressions = ("? diPhoton().leadingPhoton().hasMatchedGenPhoton() ? diPhoton().leadingPhoton().%s : -99.",
                   "? diPhoton().subLeadingPhoton().hasMatchedGenPhoton() ? diPhoton().subLeadingPhoton().%s : -99.") if isRecoTag else ("leadingPhoton().%s",
                                                                                                                                         "subLeadingPhoton().%s")
                   
    legs = reduce(lambda z,w: z+w,  map(lambda x: map(lambda y: (("gen%s%s := "+x[1]) % (x[0],y[0],y[1])), zip(capPhoVariables,phoVariables)), zip(("Lead","Sublead"),expressions) ) )
    if isRecoTag:
        legs.extend( ["genLeadGenIso := ? diPhoton().leadingPhoton().hasMatchedGenPhoton() ? diPhoton().leadingPhoton().userFloat(\"genIso\") : -99",
                      "genSubleadGenIso := ? diPhoton().subLeadingPhoton().hasMatchedGenPhoton() ? diPhoton().subLeadingPhoton().userFloat(\"genIso\") : -99"] )
    else:
        legs.extend( ["genLeadGenIso := leadingExtra.genIso",
                      "genSubleadGenIso:= subLeadingExtra.genIso"] )
        
    return dipho+legs

# ----------------------------------------------------------------------------------------------------------------
def addGenOnlyAnalysis(process,acceptance,tagList,systlabels,pdfWeights=None):
    import itertools
    import flashgg.Taggers.dumperConfigTools as cfgTools
    
    process.load("flashgg.MicroAOD.flashggGenDiPhotonsSequence_cff")
    accCut = getAccGenCut()
    cut = "1"
    if acceptance == "IN": cut = accCut
    elif acceptance == "OUT": cut = "!(%s)" % accCut
    
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
    
    ### from flashgg.Taggers.globalVariables_cff import globalVariables
    ### process.genDiphotonDumper.dumpGlobalVariables = True
    ### process.genDiphotonDumper.globalVariables = globalVariables
    ### process.genDiphotonDumper.globalVariables.addTriggerBits = cms.PSet(
    ###     tag=cms.InputTag("TriggerResults","",process.name_()),bits=cms.vstring("hltHighLevel")
    ###     )

    variables = getGenVariables(False)
    dumpPdfWeights,nPdfWeights,nAlphaSWeights,nScaleWeights=False,-1,-1,-1 
    if pdfWeights:
        dumpPdfWeights,nPdfWeights,nAlphaSWeights,nScaleWeights=pdfWeights
    
    cfgTools.addCategory(process.genDiphotonDumper,
                         "NoTag", 'isTagged("")',1,
                         variables=variables,
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
                                 variables=variables)
    
    process.pfid = cms.Path(process.genFilter*process.flashggGenDiPhotonsSequence*process.flashggTaggedGenDiphotons*process.genDiphotonDumper)
    
    if( not hasattr(process,"options") ): process.options = cms.untracked.PSet()
    process.options.allowUnscheduled = cms.untracked.bool(True)
