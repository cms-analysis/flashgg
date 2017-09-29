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
    if ":=" in name:
        name,expr = name.split(":=")
        name = name.replace(" ","")
    else:
        expr = name
    if "pfx" in expr:
        expr = expr.format(pfx=pfx)
        pfx = ""
    if nbins:
        return "gen%s[%i,%f,%f] := %s%s" % ( name.capitalize(), pfx, expr, nbins, vmin, vmax )
    else:
        return "gen%s := %s%s" % ( name.capitalize(), pfx, expr.split("[")[0] )

# ----------------------------------------------------------------------------------------------------------------
def diPhoRecoVariable(name, pfx=None, nbins=None, vmin=None, vmax=None):
    if ":=" in name:
        name,expr = name.split(":=")
        name = name.replace(" ","")
    else:
        expr = name
    if nbins:
        return "reco%s[%i,%f,%f] := %s%s" % ( name.capitalize(), pfx, expr.split("[")[0], nbins, vmin, vmax )
    else:
        ## print "name %s" % (name)
        ## print "name split [ %s" % (name.split("[")[0])
        ## print "reco%s := %s%s" % ( name.capitalize(), pfx, name.split("[")[0] )
        return "reco%s := %s%s" % ( name.capitalize(), pfx, expr.split("[")[0] )

# ----------------------------------------------------------------------------------------------------------------
def genPhoExpr(isRecoTag):
    return ("? diPhoton.leadingPhoton.hasMatchedGenPhoton ? diPhoton.leadingPhoton.matchedGenPhoton.%s : -99.",
            "? diPhoton.subLeadingPhoton.hasMatchedGenPhoton ? diPhoton.subLeadingPhoton.matchedGenPhoton.%s : -99.") if isRecoTag else ("leadingPhoton.%s",
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
#    diPhoVariables = ["pt"]
#    phoVariables = []
    diPhoVariables = ["mass","pt", "eta", "phi", "rapidity := 0.5*log( ( {pfx}energy + {pfx}pz ) / ({pfx}energy - {pfx}pz ) )"] # "rapidity := ? {pfx}pt > 0 ? {pfx}rapidity : 9999."]
    phoVariables = ["pt","eta","phi", "pz", "energy"]
    
    pfx = genDiphoPfx(isRecoTag)
    dipho = map(lambda x: diPhoGenVariable(x,pfx), diPhoVariables)
    
    expressions = genPhoExpr(isRecoTag)
    legs = reduce(lambda z,w: z+w, (map (lambda x: phoGenVariable(x,expressions), phoVariables) ) )
    
    if isRecoTag:
        legs.extend( ["genLeadGenIso[-1,(0.0:10.0:13000.0)] := ? diPhoton.leadingPhoton.hasMatchedGenPhoton ? diPhoton.leadingPhoton.userFloat(\"genIso\") : -99",
                      "genSubleadGenIso[-1,(0.0:10.0:13000.0)] := ? diPhoton.subLeadingPhoton.hasMatchedGenPhoton ? diPhoton.subLeadingPhoton.userFloat(\"genIso\") : -99",
                      "genLeadFromHardProcess := ? diPhoton.leadingPhoton.hasMatchedGenPhoton ? diPhoton.leadingPhoton.matchedGenPhoton.fromHardProcessFinalState : 0",
                      "genSubleadFromHardProcess := ? diPhoton.subLeadingPhoton.hasMatchedGenPhoton ? diPhoton.subLeadingPhoton.matchedGenPhoton.fromHardProcessFinalState : 0"
                      ] )
        
    else:
        legs.extend( ["genLeadGenIso[-1,(0.0:10.0:13000.0)] := leadingExtra.genIso",
                      "genSubleadGenIso[-1,(0.0:10.0:13000.0)]:= subLeadingExtra.genIso",
                      "genLeadFromHardProcess := leadingPhoton.fromHardProcessFinalState",
                      "genSubleadFromHardProcess := subLeadingPhoton.fromHardProcessFinalState"

                      ] )
    return dipho+legs

# ----------------------------------------------------------------------------------------------------------------
def getRecoVariables(isRecoTag=True):
    diPhoVariables = ["mass","pt", "eta","rapidity", "phi"]
    phoVariables = ["pt","eta","phi", "energy", "pz" ]
    
    pfx = recoDiphoPfx(isRecoTag)
    dipho = map(lambda x: diPhoRecoVariable(x,pfx), diPhoVariables)
    
    expressions = recoPhoExpr(isRecoTag)
    legs = reduce(lambda z,w: z+w, (map (lambda x: phoRecoVariable(x,expressions) , phoVariables ) ) )

    ## return dipho
    return dipho+legs

# ----------------------------------------------------------------------------------------------------------------
def bookHadronicActivityProducers(process,processId,tagSequence,recoDiphotons,recoDiphotonTags,genDiphotons,recoJetCollections=None,genJetCollection="slimmedGenJets",genBJetCollection="flashggGenJetsExtra"):
##    from flashgg.Systematics.VBFTagDumper_standard_wsys_cfg import mva_wp
    mva_wp = {
        "none"  : [
            [],[],[]
            #[-1, -1,  -1, -1],
            #[-1, -1,  -1, -1],
            #[-1, -1 , -1, -1]
            ],
        "tight" : [
            [0.26, -0.34, -0.24, -0.26],
            [0.62, -0.21, -0.07, -0.03],
            [0.87, 0.17 , 0.25 ,  0.33]
            ],
        "medium": [
            [-0.49, -0.53, -0.44, -0.42],
            [-0.06, -0.42, -0.30, -0.23],
            [ 0.56, -0.10,  0.01,  0.13]
            ],
        "loose" :[
            [-0.96, -0.64, -0.56, -0.54],
            [-0.92, -0.56, -0.44, -0.39],
            [-0.77, -0.31, -0.20, -0.03]
            ],
        "mixed" :[
            [-0.96, -0.34, -0.24, -0.26],
            [-0.92, -0.21, -0.07, -0.03],
            [-0.77, 0.17 , 0.25 ,  0.33]
            #[0.87, 0.17 , 0.25 ,  0.33]
            ],
        "forward_tight" : [
            [-1, -0.34, -0.24, -0.26],
            [-1, -0.21, -0.07, -0.03],
            [-1, 0.17 , 0.25 ,  0.33]
            ],
        "forward_medium": [
            [-1, -0.53, -0.44, -0.42],
            [-1, -0.42, -0.30, -0.23],
            [-1, -0.10,  0.01,  0.13]
            ],
        "forward_loose" :[
            [-1, -0.64, -0.56, -0.54],
            [-1, -0.56, -0.44, -0.39],
            [-1, -0.31, -0.20, -0.03]
            ]
        }    
    
    if not recoJetCollections:
        from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag
        recoJetCollections = UnpackedJetCollectionVInputTag

    
    recoJets2p5 = cms.VInputTag()
    recoJets4p7 = cms.VInputTag()
    recoJetsBflavor2p5 = cms.VInputTag()
    recoJetsBflavor4p7 = cms.VInputTag()
    recoMet =cms.VInputTag()
    print recoJetCollections
    pos = tagSequence.index(recoDiphotonTags) - 1

    if( not hasattr(process,"filteredRecoMet" ) ): 
        setattr(process,"filteredRecoMet",cms.EDFilter("FlashggMetCandidateSelector",
                                                       src=cms.InputTag("flashggMets"),
                                                       cut=cms.string("1"),
                                                       MetFiltersFlags=cms.vstring("Flag_HBHENoiseFilter","Flag_HBHENoiseIsoFilter","Flag_EcalDeadCellTriggerPrimitiveFilter","Flag_goodVertices","Flag_eeBadScFilter"),
                                                       RECOfilters= cms.InputTag('TriggerResults::RECO'),
                                                       PATfilters= cms.InputTag('TriggerResults::PAT')
                                                                        
                                                                        ) )
        recoMet.append("filteredRecoMet")
        tagSequence.insert(pos, getattr(process,"filteredRecoMet"))
        pos += 1
    for icoll,coll in enumerate(recoJetCollections):        
        if( not hasattr(process,"filteredRecoJetsEta2p5%d" % icoll) ): 
            setattr(process,"filteredRecoJetsEta2p5%d" % icoll,cms.EDFilter("FlashggJetCandidateSelector", ##pujetid is applied in FlashggJetCandidateSelector via CutBasedJetObjectSelector.cc
                                                                            src=coll,
                                                                            cut=cms.string("pt>%f && abs(eta)<2.5 && passesJetID('Loose')" % jetPtCut),
                                                                            
                                                                            ) )
            recoJets2p5.append("filteredRecoJetsEta2p5%d" % icoll)
#            recoJets2p5.append(coll)
            tagSequence.insert(pos, getattr(process,"filteredRecoJetsEta2p5%d" % icoll))
            pos += 1


        if( not hasattr(process,"filteredRecoJetsEta4p7%d" % icoll) ): 
            setattr(process,"filteredRecoJetsEta4p7%d" % icoll,cms.EDFilter("FlashggJetCandidateSelector",
                                                                            src=coll,
                                                                            cut=cms.string("pt>%f && abs(eta)<4.7 && passesJetID('Loose') " % jetPtCut),

                                                                            ) )

            recoJets4p7.append("filteredRecoJetsEta4p7%d" % icoll)
            tagSequence.insert(pos, getattr(process,"filteredRecoJetsEta4p7%d" % icoll))
            pos += 1

        ###Btag reco, bflavor jets
        from flashgg.MicroAOD.flashggJets_cfi import flashggBTag
        from flashgg.Taggers.flashggTags_cff import bDiscriminator80XReReco
        if( not hasattr(process,"filteredRecoJetsBflavorEta2p5%d" % icoll) ): 
            setattr(process,"filteredRecoJetsBflavorEta2p5%d" % icoll,cms.EDFilter("FlashggJetCandidateSelector",
                                                                                    src=coll,
                                                                                    cut=cms.string("pt>%f && abs(eta)<2.5 && passesJetID('Loose') && bDiscriminator( '%s' ) > %f " % (jetPtCut, flashggBTag, bDiscriminator80XReReco[1])),


                                                                                    ) )
            
            recoJetsBflavor2p5.append("filteredRecoJetsBflavorEta2p5%d" % icoll)
            tagSequence.insert(pos, getattr(process,"filteredRecoJetsBflavorEta2p5%d" % icoll))
            pos += 1



        if( not hasattr(process,"filteredRecoJetsBflavorEta4p7%d" % icoll) ): 
            setattr(process,"filteredRecoJetsBflavorEta4p7%d" % icoll,cms.EDFilter("FlashggJetCandidateSelector",
                                                                                   src=coll,
                                                                                   cut=cms.string("pt>%f && abs(eta)<4.7 && passesJetID('Loose') && bDiscriminator( '%s' ) > %f " % (jetPtCut, flashggBTag, bDiscriminator80XReReco[1])),

                                                                                    ) )
            
            recoJetsBflavor4p7.append("filteredRecoJetsBflavorEta4p7%d" % icoll)
            tagSequence.insert(pos, getattr(process,"filteredRecoJetsBflavorEta4p7%d" % icoll))
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

    if( not hasattr(process,"flashggRecoHadronicActivityBflavor2p5") ): 
        process.flashggRecoHadronicActivityBflavor2p5 = cms.EDProducer("FlashggDiPhotonHadronicActivityProducer",
                                                                src=recoJetsBflavor2p5,
                                                                veto=cms.InputTag(recoDiphotons)
                                                                )
        tagSequence.insert(pos, getattr(process,"flashggRecoHadronicActivityBflavor2p5"))
        pos += 1

    if( not hasattr(process,"flashggRecoHadronicActivityBflavor4p7") ): 
        process.flashggRecoHadronicActivityBflavor4p7 = cms.EDProducer("FlashggDiPhotonHadronicActivityProducer",
                                                                src=recoJetsBflavor4p7,
                                                                veto=cms.InputTag(recoDiphotons)
                                                                )
        tagSequence.insert(pos, getattr(process,"flashggRecoHadronicActivityBflavor4p7"))
        pos += 1


    if( not hasattr(process,"flashggRecoHadronicActivityMET") ): 
        process.flashggRecoHadronicActivityMET = cms.EDProducer("FlashggDiPhotonHadronicActivityProducer",
                                                                src=recoMet,
                                                                veto=cms.InputTag(recoDiphotons)
                                                                )
        tagSequence.insert(pos, getattr(process,"flashggRecoHadronicActivityMET"))
        pos += 1


  ##  if( not hasattr(process,"flashggRecoHadronicActivityMuons") ): 
  ##      process.flashggRecoHadronicActivityMuons = cms.EDProducer("FlashggDiPhotonHadronicActivityProducer",
  ##                                                              src=cms.InputTag("flashggSelectedMuons"),
  ##                                                              veto=cms.InputTag(recoDiphotons)
  ##                                                              )
  ##      tagSequence.insert(pos, getattr(process,"flashggRecoHadronicActivityMuons"))
  ##      pos += 1


    recoDiphotonTags.CompositeCandidateTags.jets2p5 = cms.InputTag("flashggRecoHadronicActivity2p5")
    recoDiphotonTags.CompositeCandidateTags.jets4p7 = cms.InputTag("flashggRecoHadronicActivity4p7")
    recoDiphotonTags.CompositeCandidateTags.jetsBflavor2p5 = cms.InputTag("flashggRecoHadronicActivityBflavor2p5")
    recoDiphotonTags.CompositeCandidateTags.jetsBflavor4p7 = cms.InputTag("flashggRecoHadronicActivityBflavor4p7")
    recoDiphotonTags.CompositeCandidateTags.met = cms.InputTag("flashggRecoHadronicActivityMET")
##    recoDiphotonTags.CompositeCandidateTags.muons = cms.InputTag("flashggRecoHadronicActivityMuons")
    
    if not processId=="Data":
        if( not hasattr(process,"filteredGenJetsEta2p5") ): 
            process.filteredGenJetsEta2p5 = cms.EDFilter("GenJetSelector",
                                                         src=cms.InputTag(genJetCollection),
                                                         cut=cms.string("pt>%f && abs(eta)<2.5 && numberOfDaughters > 5" % jetPtCut),
                                                         )

            process.flashggGenHadronicActivity2p5 = cms.EDProducer("FlashggGenHadronicActivityProducer",
                                                                   src=cms.InputTag("filteredGenJetsEta2p5"),
                                                                   veto=cms.InputTag(genDiphotons)
                                                                   )
        if( not hasattr(process,"filteredGenJetsEta4p7") ): 
            process.filteredGenJetsEta4p7 = cms.EDFilter("GenJetSelector",
                                                         src=cms.InputTag(genJetCollection),
                                                         cut=cms.string("pt>%f && abs(eta)<4.7 && numberOfDaughters > 5" % jetPtCut),
                                                     )

            process.flashggGenHadronicActivity4p7 = cms.EDProducer("FlashggGenHadronicActivityProducer",
                                                                   src=cms.InputTag("filteredGenJetsEta4p7"),
                                                                   veto=cms.InputTag(genDiphotons)
                                                                   )

        if( not hasattr(process,"filteredGenJetsBflavorEta4p7") ): 
           process.filteredGenJetsBflavorEta4p7 = cms.EDFilter("GenJetSelector",
                                                               src=cms.InputTag(genBJetCollection),
####                                                               cut=cms.string("cand.pt>%f && abs(cand.eta)<4.7 && cand.numberOfDaughters > 5" % jetPtCut),
##                                                               cut=cms.string("cand.pt>%f && abs(cand.eta)<4.7 && cand.numberOfDaughters > 5 && hasBottom == 1" % jetPtCut),
                                                               cut=cms.string("pt>%f && abs(eta)<4.7 && numberOfDaughters > 5" % jetPtCut),
##                                                               cut=cms.string("pt>5  && hasBottom == 1" ),
##                                                               cut=cms.string("hasBottom == 1"),
                                                    )
            
           process.flashggGenHadronicActivityBflavor4p7 = cms.EDProducer("FlashggGenHadronicActivityProducer",
                                                                         src=cms.InputTag("filteredGenJetsBflavorEta4p7"),
                                                                         veto=cms.InputTag(genDiphotons)
                                                                         )

        if( not hasattr(process,"filteredGenJetsBflavorEta2p5") ): 
           process.filteredGenJetsBflavorEta2p5 = cms.EDFilter("GenJetSelector",
                                                               src=cms.InputTag(genBJetCollection),
##                                                               cut=cms.string("cand.pt>%f && abs(cand.eta)<2.5 && cand.numberOfDaughters > 5" % jetPtCut),
                                                               cut=cms.string("pt>%f && abs(eta)<2.5 && numberOfDaughters > 5" % jetPtCut),
##                                                               cut=cms.string("pt>%f && abs(eta)<2.5 && numberOfDaughters > 5 && hasBottom == 1" % jetPtCut),
##                                                               cut=cms.string("hasBottom == 1"),
                                                    )
            
           process.flashggGenHadronicActivityBflavor2p5 = cms.EDProducer("FlashggGenHadronicActivityProducer",
                                                                         src=cms.InputTag("filteredGenJetsBflavorEta2p5"),
                                                                         veto=cms.InputTag(genDiphotons)
                                                                         )


        if( not hasattr(process,"flashggGenHadronicActivityMET") ): 
            process.flashggGenHadronicActivityMET = cms.EDProducer("FlashggGenHadronicActivityProducer",
                                                                   src=cms.InputTag("flashggMets"),
                                                                   veto=cms.InputTag(genDiphotons)
                                                                   )
           

        if( not hasattr(process,"filteredGenJetsEtaInclusive") ): 
            process.filteredGenJetsEtaInclusive = cms.EDFilter("GenJetSelector",
                                                         src=cms.InputTag(genJetCollection),
#                                                         cut=cms.string("pt>%f && abs(eta)<2.5 && nCarrying(0.90)>1" % jetPtCut),
###                                                         cut=cms.string("pt>%f && numberOfDaughters > 5" % jetPtCut),
                                                         cut=cms.string("1"),
                                                         )
            process.flashggGenHadronicActivityInclusive = cms.EDProducer("FlashggGenHadronicActivityProducer",
                                                                   src=cms.InputTag("filteredGenJetsEtaInclusive"),
                                                                   veto=cms.InputTag(genDiphotons)
                                                                   )
        

        if( not hasattr(process,"flashggGenHadronicActivityLeptons") ): 
#            process.filteredGenJetsEta4p7 = cms.EDFilter("GenJetSelector",
#                                                         src=cms.InputTag(genJetCollection),
#                                                         cut=cms.string("pt>%f && abs(eta)<4.7" % jetPtCut),
#                                                     )

            process.flashggGenHadronicActivityLeptons = cms.EDProducer("FlashggGenHadronicActivityProducer",
                                                                   src=cms.InputTag("flashggGenLeptonsExtra"),
                                                                   veto=cms.InputTag(genDiphotons)
                                                                   )
        
# ----------------------------------------------------------------------------------------------------------------
def getJetKinVariables(pre,post,variables,nmax, getter):
    return reduce(lambda z,w: z+w, 
                  map(lambda x: 
#                      map(lambda y: ("%sJet%s%s%d := ? numberOfDaughters > %d ? daughter(%d).%s : 0" % (pre,post,y.split("[")[0].capitalize(),x,x,x,y.split("[")[0])), variables),
                      map(lambda y: ("%sJet%s%s := ? %snumberOfDaughters > %d ? %sdaughter(%d).%s : -999" % (pre,  post,  (str(x)+"[").join(y.capitalize().rsplit("[",1)) if "[" in y else y.capitalize()+str(x),  getter,  x,  getter,  x,  y.split("[")[0])),  variables),
                      xrange(nmax)
                      )
                  )


# ----------------------------------------------------------------------------------------------------------------
def getLeptonKinVariables(pre,post,variables,nmax, getter):
    return reduce(lambda z,w: z+w, 
                  map(lambda x: 
#                      map(lambda y: ("%sJet%s%s%d := ? numberOfDaughters > %d ? daughter(%d).%s : 0" % (pre,post,y.split("[")[0].capitalize(),x,x,x,y.split("[")[0])), variables),
                      map(lambda y: ("%sLepton%s%s := ? %snumberOfDaughters > %d ? %sdaughter(%d).%s : -999" % (pre,  post,  (str(x)+"[").join(y.capitalize().rsplit("[",1)) if "[" in y else y.capitalize()+str(x),  getter,  x,  getter,  x,  y.split("[")[0])),  variables),
                      xrange(nmax)
                      )
                  )

# ----------------------------------------------------------------------------------------------------------------
def addJetGlobalVariables(process,dumper,src,pre,post,getter=""):    
    import flashgg.Taggers.dumperConfigTools as cfgTools
    from flashgg.MicroAOD.flashggJets_cfi import flashggBTag
    if post=="Muons":
        print "adding variables for muons"

    if getter != "": getter += "."
    variables  = [ "%(pre)sNjets%(post)s[-1,(-0.5:0.5:1.5:2.5:3.5:100)]:= %(getter)snumberOfDaughters" % locals() ]
#    variables  = [ "%sNjets%s:=numberOfDaughters" % (pre,post) ]
#    variables += getJetKinVariables(pre,post,["pt[667,0.0,10005.0]","eta[50,-5.0,5.0]","rapidity[50,0.0,10.0]"],5)
    if pre =="reco":
        variables += getJetKinVariables(pre,post,["pt[-1,(0.0:40.0:65.0:105.0:200.0:13000.0)]","eta","rapidity","phi","px","py","pz", "energy", "numberOfDaughters[200,-0.5,199.5]", "puJetIdMVA", ],6, getter)

#        print  ("%sJet%sBdiscriminant0 := ? %snumberOfDaughters > 0 ? %sdaughter(0).bDiscriminator('pfCombinedInclusiveSecondaryVertexV2BJetTags') : -999" % (pre,  post,  getter,  getter))
#        variables+=[("%sJet%sBdiscriminant0 := ? %snumberOfDaughters > 0 ? %sdaughter(0).bDiscriminator('pfCombinedInclusiveSecondaryVertexV2BJetTags') : -999" % (pre,  post,  getter,  getter))]
    else:
        variables += getJetKinVariables(pre,post,["pt[-1,(0.0:40.0:65.0:105.0:200.0:13000.0)]","eta","rapidity","phi","px","py","pz", "energy", "numberOfDaughters[200,-0.5,199.5]"],6, getter)
    print "jet global variables"
    print variables
###    variables += getJetKinVariables(pre,post,["pt","eta","rapidity"],5)
###    variables += [ "%sDijetMass%s := ? numberOfDaughters > 1 ? sqrt( (daughter(0).energy+daughter(1).energy)^2 - (daughter(0).px+daughter(1).px)^2 - (daughter(0).py+daughter(1).py)^2 - (daughter(0).pz+daughter(1).pz)^2 ) : 0" % (pre,post) ]
    
    if post=="Muons":
        print "variables for muons"
        print variables

    if src:
        cfgTools.addGlobalFloats(process,dumper.globalVariables,src,variables)
    else:
        for cat in dumper.categories:
            cfgTools.addVariables(cat.variables,variables)

# ----------------------------------------------------------------------------------------------------------------
def addLeptonGlobalVariables(process,dumper,src,pre,post,getter=""):    
    import flashgg.Taggers.dumperConfigTools as cfgTools

    if getter != "": getter += "."
    variables  = [ "%(pre)sNleptons%(post)s[-1,(-0.5:0.5:1.5:2.5:3.5:100)]:= %(getter)snumberOfDaughters" % locals() ]
    variables += [ "%(pre)sLeptonDressedPt%(post)s0 := ? %(getter)snumberOfDaughters > 0 ? %(getter)sdaughter(0).p4().pt : -999" % locals() ]
    variables += [ "%(pre)sLeptonDressedPt%(post)s1 := ? %(getter)snumberOfDaughters > 1 ? %(getter)sdaughter(1).p4().pt : -999" % locals() ]

#    variables += [ "%(pre)sLeptonPt%(post)s0 := ? %(getter)snumberOfDaughters > 0 ? %(getter)sdaughter(0).cand().p4().pt : -999" % locals() ]
#    variables += [ "%(pre)sLeptonPt%(post)s1 := ? %(getter)snumberOfDaughters > 1 ? %(getter)sdaughter(1).cand().p4().pt : -999" % locals() ]

###    variables += [ "%(pre)sLepton%(post)s1[-1,(-0.5:0.5:1.5:2.5:3.5:100)]:= ? %(getter)snumberOfDaughters > 1 ? %(getter)sdaughter(1).dressedP4().pt() : -999" % locals() ]
#    variables += getLeptonKinVariables(pre,post,["dressedP4().pt"],3, getter)
#    print variables
###    variables += getLeptonKinVariables(pre,post,["pt","eta","rapidity"],5)
###    variables += [ "%sDijetMass%s := ? numberOfDaughters > 1 ? sqrt( (daughter(0).energy+daughter(1).energy)^2 - (daughter(0).px+daughter(1).px)^2 - (daughter(0).py+daughter(1).py)^2 - (daughter(0).pz+daughter(1).pz)^2 ) : 0" % (pre,post) ]
    
    if src:
        cfgTools.addGlobalFloats(process,dumper.globalVariables,src,variables)
    else:
        for cat in dumper.categories:
            cfgTools.addVariables(cat.variables,variables)
        

# ----------------------------------------------------------------------------------------------------------------
def addMETGlobalVariables(process,dumper,src,pre,post,getter=""):    
    import flashgg.Taggers.dumperConfigTools as cfgTools

    if getter != "": getter += "."
    variables  = [ "%(pre)sMET%(post)s :=  %(getter)sgenMET.pt  " % locals() ]
    
    if src:
        cfgTools.addGlobalFloats(process,dumper.globalVariables,src,variables)
    else:
        for cat in dumper.categories:
            cfgTools.addVariables(cat.variables,variables)
        
# ----------------------------------------------------------------------------------------------------------------
def addRecoMETGlobalVariables(process,dumper,src,pre,post,getter=""):    
    import flashgg.Taggers.dumperConfigTools as cfgTools

    if getter != "": getter += "."
    variables  = [ "%(pre)sMET%(post)sPt :=  %(getter)spt " % locals() ]
    variables += [ "%(pre)sMET%(post)sPx :=  %(getter)spx " % locals() ]
    variables += [ "%(pre)sMET%(post)sPy :=  %(getter)spy " % locals() ]
    variables += [ "%(pre)sMET%(post)sPz :=  %(getter)spz " % locals() ]
    variables += [ "%(pre)sMET%(post)sEta :=  %(getter)seta " % locals() ]
    variables += [ "%(pre)sMET%(post)sPhi :=  %(getter)sphi " % locals() ]
    print "recoMET variables"
    print variables
    if src:
        cfgTools.addGlobalFloats(process,dumper.globalVariables,src,variables)
    else:
        for cat in dumper.categories:
            cfgTools.addVariables(cat.variables,variables)

# ----------------------------------------------------------------------------------------------------------------
def addGenGlobalVariables(process,dumper):   
    addJetGlobalVariables(process,dumper,"flashggGenHadronicActivity2p5","gen","2p5")
    addJetGlobalVariables(process,dumper,"flashggGenHadronicActivity4p7","gen","4p7")
    addJetGlobalVariables(process,dumper,"flashggGenHadronicActivityBflavor2p5","gen","Bflavor2p5")
    addJetGlobalVariables(process,dumper,"flashggGenHadronicActivityBflavor4p7","gen","Bflavor4p7")
    addJetGlobalVariables(process,dumper,"flashggGenHadronicActivityInclusive","gen","Inclusive")

    addLeptonGlobalVariables(process,dumper,"flashggGenHadronicActivityLeptons","gen","all")
    addMETGlobalVariables(process,dumper,"flashggMets","gen","all")

# ----------------------------------------------------------------------------------------------------------------
def addRecoGlobalVariables(process,dumper,tagGetter=""):    
    if tagGetter != "": tagGetter += "."
    addJetGlobalVariables(process,dumper,None,"reco","2p5","%sgetCompCand('jets2p5')" % tagGetter)
    addJetGlobalVariables(process,dumper,None,"reco","4p7","%sgetCompCand('jets4p7')" % tagGetter)
    addJetGlobalVariables(process,dumper,None,"reco","Bflavor2p5","%sgetCompCand('jetsBflavor2p5')" % tagGetter)
    addJetGlobalVariables(process,dumper,None,"reco","Bflavor4p7","%sgetCompCand('jetsBflavor4p7')" % tagGetter)

    addRecoMETGlobalVariables(process,dumper,None,"reco","All","%sgetCompCand('met')" % tagGetter)

#    addJetGlobalVariables(process,dumper,None,"reco","Muons","%sgetCompCand('muons')" % tagGetter)
    
    
# ----------------------------------------------------------------------------------------------------------------
def addGenOnlyAnalysis(process,processId,tagSequence,acceptance,tagList,systlabels,pdfWeights=None,recoJetCollections=None,mH=None,filterEvents=True):
    import itertools
    import flashgg.Taggers.dumperConfigTools as cfgTools
    
    accCut = getAccGenCut()
    cut = "1"
    preselCut = "1"
    if acceptance == "IN":
        cut = accCut
    if acceptance == "OUT": 
        cut = "!(%s)" % (accCut)
    if acceptance == "IN" or acceptance == "OUT":
        preselCut = "(leadingPhoton.fromHardProcessFinalState && subLeadingPhoton.fromHardProcessFinalState)"
    ### if mH:
    ###     ## cut = "( abs(mass-%f) < 1. ) && (%s)" % (mH, accCut)
    ###     preselCut = "( abs(mass-%f) < 1. ) && (%s)" % preselCut

    process.load("flashgg.MicroAOD.flashggGenDiPhotonsSequence_cff")
    ### process.flashggPreselectedGenDiPhotons = process.flashggSelectedGenDiPhotons.clone(filter = cms.bool(filterEvents), cut=cms.string(preselCut))
    ### process.flashggGenDiPhotonsSequence.insert(process.flashggGenDiPhotonsSequence.index(process.flashggSelectedGenDiPhotons),process.flashggPreselectedGenDiPhotons)
    ### process.flashggSelectedGenDiPhotons.src = "flashggPreselectedGenDiPhotons"

    process.flashggSelectedGenDiPhotons.cut = "(%s) && (%s)" % ( preselCut, cut )
    process.flashggSortedGenDiPhotons.maxNumber = 999
    ## only process events where at least one diphoton candidate is selected
    if filterEvents:
        process.flashggSelectedGenDiPhotons.filter = True
        process.genFilter += process.flashggSelectedGenDiPhotons
    
    process.load("flashgg.Taggers.flashggTaggedGenDiphotons_cfi")
    process.flashggTaggedGenDiphotons.src  = "flashggSortedGenDiPhotons"
    process.flashggTaggedGenDiphotons.tags = "flashggTagSorter"
    process.flashggTaggedGenDiphotons.remap = process.tagsDumper.classifierCfg.remap
    ## process.flashggTaggedGenDiphotons.tags = "flashggSystTagMerger"
    
    process.load("flashgg.Taggers.genDiphotonDumper_cfi")
    process.genDiphotonDumper.dumpTrees = True
    process.genDiphotonDumper.maxCandPerEvent = -1
    process.genDiphotonDumper.dumpWorkspace = False
    process.genDiphotonDumper.src = "flashggTaggedGenDiphotons"

    from flashgg.Taggers.globalVariables_cff import globalVariables
    process.genDiphotonDumper.dumpGlobalVariables = True
    process.genDiphotonDumper.globalVariables = globalVariables
    
    ## bookHadronicActivityProducers(process,processId,"flashggTagSorter","flashggTaggedGenDiphotons",recoJetCollections,genJetCollection="slimmedGenJets")
    bookHadronicActivityProducers(process,processId,tagSequence,"flashggDiPhotonSystematics",process.flashggSigmaMoMpToMTag,"flashggTaggedGenDiphotons",recoJetCollections,genJetCollection="slimmedGenJets") ##FIXME 
    addGenGlobalVariables(process,process.genDiphotonDumper)
    addRecoGlobalVariables(process,process.genDiphotonDumper,tagGetter="tag()")

    dumpPdfWeights,nPdfWeights,nAlphaSWeights,nScaleWeights=False,-1,-1,-1 
    if pdfWeights:
        dumpPdfWeights,nPdfWeights,nAlphaSWeights,nScaleWeights=pdfWeights
        
    genVariables  = getGenVariables(False)
    recoVariables = getRecoVariables(False)
    recoVariables.extend( ["leadmva := recoTagObj.diPhoton.leadingView.phoIdMvaWrtChosenVtx", "subleadmva := recoTagObj.diPhoton.subLeadingView.phoIdMvaWrtChosenVtx"] )
##    recoVariables.extend( ["costheta := ( ( recoTagObj.diPhoton.leadingPhoton.energy + recoTagObj.diPhoton.leadingPhoton.p4.pz ) * ( recoTagObj.diPhoton.subLeadingPhoton.energy - recoTagObj.diPhoton.subLeadingPhoton.p4.pz ) - ( recoTagObj.diPhoton.leadingPhoton.energy - recoTagObj.diPhoton.leadingPhoton.p4.pz ) * ( recoTagObj.diPhoton.subLeadingPhoton.energy + recoTagObj.diPhoton.subLeadingPhoton.p4.pz ) ) / ( recoTagObj.diPhoton.mass * sqrt( recoTagObj.diPhoton.mass * recoTagObj.diPhoton.mass + recoTagObj.diPhoton.pt * recoTagObj.diPhoton.pt ) )"] )
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
