import FWCore.ParameterSet.Config as cms

leadCut = 1./3.
subLeadCut = 1./4.
isoCut = 10.
etaCut = 2.5
jetPtCut = 30.

doBJetsAndMET=False
doJets=False
# ----------------------------------------------------------------------------------------------------------------
def setupTagSequenceForFiducial(process, options):
    from flashgg.Taggers.flashggPreselectedDiPhotons_cfi import flashggPreselectedDiPhotons
    from flashgg.Taggers.flashggDiPhotonMVA_cfi import flashggDiPhotonMVA
    from flashgg.Taggers.flashggTags_cff import flashggSigmaMoMpToMTag
    from flashgg.MicroAOD.flashggJets_cfi import flashggUnpackedJets
    from flashgg.Taggers.flashggPrefireDiPhotons_cff import flashggPrefireDiPhotons
    from flashgg.Taggers.flashggTagSorter_cfi import flashggTagSorter
    from flashgg.Taggers.flashggDifferentialPhoIdInputsCorrection_cfi import flashggDifferentialPhoIdInputsCorrection, setup_flashggDifferentialPhoIdInputsCorrection

    process.load("flashgg.Taggers.flashggTagSequence_cfi")
    
    setup_flashggDifferentialPhoIdInputsCorrection(process, options.metaConditions)
    flashggPreselectedDiPhotons.src = "flashggPrefireDiPhotons"

    flashggTagSequence = cms.Sequence( flashggDifferentialPhoIdInputsCorrection
                                       * flashggPrefireDiPhotons
                                       * flashggPreselectedDiPhotons
                                       * flashggDiPhotonMVA
                                       * flashggUnpackedJets
                                       * flashggSigmaMoMpToMTag
                                       * flashggTagSorter
    )
    
    return flashggTagSequence

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
    # print map(lambda x: (("reco%s%s := "+x[1]) % (x[0],name.capitalize(),name.split("[")[0])), zip(("Lead","Sublead"),expressions))
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
    phoVariables = ["pt","eta","phi", "energy", "pz", "full5x5_r9", "sigEOverE" ]
    
    pfx = recoDiphoPfx(isRecoTag)
    dipho = map(lambda x: diPhoRecoVariable(x,pfx), diPhoVariables)
    
    expressions = recoPhoExpr(isRecoTag)
    legs = reduce(lambda z,w: z+w, (map (lambda x: phoRecoVariable(x,expressions) , phoVariables ) ) )

    ## return dipho
    return dipho+legs

# ----------------------------------------------------------------------------------------------------------------
def bookHadronicActivityProducers(process,processId,tagSequence,recoDiphotons,recoDiphotonTags,genDiphotons,recoJetCollections=None,genJetCollection="slimmedGenJets",genBJetCollection="flashggGenJetsExtra"):
    
    if not recoJetCollections:
        from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag
        recoJetCollections = UnpackedJetCollectionVInputTag

    if doJets:
        recoJets2p5 = cms.VInputTag()
        recoJets4p7 = cms.VInputTag()
    if doBJetsAndMET:
#        recoJetsBflavorMedium2p5 = cms.VInputTag()
        recoJetsBflavorTight2p5 = cms.VInputTag()
        recoMet =cms.VInputTag()
        recoMuons =cms.VInputTag()
        recoElectrons =cms.VInputTag()
    print recoJetCollections
    recoPos = tagSequence.index(recoDiphotonTags) - 1
    
    if doBJetsAndMET:
        if( not hasattr(process,"filteredRecoMet" ) ): 
            setattr(process,"filteredRecoMet",cms.EDFilter("FlashggMetCandidateSelector",
                                                           src=cms.InputTag("flashggMets"),
                                                           cut=cms.string("1"),
                                                           MetFiltersFlags=cms.vstring("Flag_HBHENoiseFilter","Flag_HBHENoiseIsoFilter","Flag_EcalDeadCellTriggerPrimitiveFilter","Flag_goodVertices","Flag_eeBadScFilter"),
                                                           RECOfilters= cms.InputTag('TriggerResults::RECO'),
                                                           PATfilters= cms.InputTag('TriggerResults::PAT')
                                                                            
                                                                            ) )
            recoMet.append("filteredRecoMet")
            tagSequence.insert(recoPos, getattr(process,"filteredRecoMet"))
            recoPos += 1
    
    ##    from flashgg.Taggers.flashggTags_cff import TTHLeptonicTag
        if( not hasattr(process,"filteredRecoMuons" ) ): 
            setattr(process,"filteredRecoMuons",cms.EDFilter("FlashggMuonCandidateSelector",
                                                             src=cms.InputTag("flashggSelectedMuons"),
                                                             cut=cms.string("abs(eta) < 2.4 && pt > 20"),
                                                             VertexTag=cms.InputTag('offlineSlimmedPrimaryVertices'),
                                                             muPFIsoSumRelThreshold = cms.double(0.25), 
                                                                            ) )
            recoMuons.append("filteredRecoMuons")
            tagSequence.insert(recoPos, getattr(process,"filteredRecoMuons"))
            recoPos += 1
    
    ####electrons
        if( not hasattr(process,"filteredRecoElectrons" ) ): 
            setattr(process,"filteredRecoElectrons",cms.EDFilter("FlashggElectronCandidateSelector",
                                                                 src=cms.InputTag("flashggSelectedElectrons"),
                                                                 DiphotonTag=cms.InputTag("flashggPreselectedDiPhotons"),
                                                                 cut=cms.string("1"),###eta < 2.4 && pt > 20"),
                                                                 ElectronPtThreshold = cms.double(20.),
                                                                 ElectronEtaCuts = cms.vdouble(1.4442,1.566,2.4),
                                                                 useMVARecipe = cms.bool(False),
                                                                 useLooseID = cms.bool(True),
                                                                 deltaRPhoElectronThreshold = cms.double(0.35),
                                                                 deltaRTrkElec = cms.double(0.35),
                                                                 deltaMassElectronZThreshold = cms.double(5.0)
    
                                                                            ) )
            recoElectrons.append("filteredRecoElectrons")
            tagSequence.insert(recoPos, getattr(process,"filteredRecoElectrons"))
            recoPos += 1


    for icoll,coll in enumerate(recoJetCollections):        
        if doJets:
    	    if( not hasattr(process,"filteredRecoJetsEta2p5%d" % icoll) ): 
    	        setattr(process,"filteredRecoJetsEta2p5%d" % icoll,cms.EDFilter("FlashggJetCandidateSelector", ##pujetid is applied in FlashggJetCandidateSelector via CutBasedJetObjectSelector.cc
    	                                                                        src=coll,
    	                                                                        cut=cms.string("pt>%f && abs(eta)<2.5 && passesJetID('Loose')" % jetPtCut),
    	                                                                        
    	                                                                        ) )

    	        recoJets2p5.append("filteredRecoJetsEta2p5%d" % icoll)
#   	         recoJets2p5.append(coll)
    	        tagSequence.insert(recoPos, getattr(process,"filteredRecoJetsEta2p5%d" % icoll))
    	        recoPos += 1
    	
    	
    	    if( not hasattr(process,"filteredRecoJetsEta4p7%d" % icoll) ): 
    	        setattr(process,"filteredRecoJetsEta4p7%d" % icoll,cms.EDFilter("FlashggJetCandidateSelector",
    	                                                                        src=coll,
    	                                                                        cut=cms.string("pt>%f && abs(eta)<4.7 && passesJetID('Loose') " % jetPtCut),
    	
    	                                                                        ) )
    	
    	        recoJets4p7.append("filteredRecoJetsEta4p7%d" % icoll)
    	        tagSequence.insert(recoPos, getattr(process,"filteredRecoJetsEta4p7%d" % icoll))
    	        recoPos += 1

        ###Btag reco, bflavor jets
        if doBJetsAndMET:
        	from flashgg.MicroAOD.flashggJets_cfi import flashggBTag
        	from flashgg.Taggers.flashggTags_cff import bDiscriminator80XReReco
##        	if( not hasattr(process,"filteredRecoJetsBflavorMediumEta2p5%d" % icoll) ): 
##        	    setattr(process,"filteredRecoJetsBflavorMediumEta2p5%d" % icoll,cms.EDFilter("FlashggJetCandidateSelector",
##        	                                                                            src=coll,
##        	                                                                            cut=cms.string("pt>%f && abs(eta)<2.5 && passesJetID('Loose') && bDiscriminator( '%s' ) > %f " % (jetPtCut, flashggBTag, bDiscriminator80XReReco[1])),
##        	
##        	
##        	                                                                            ) )
##        	    
##        	    recoJetsBflavorMedium2p5.append("filteredRecoJetsBflavorMediumEta2p5%d" % icoll)
##        	    tagSequence.insert(recoPos, getattr(process,"filteredRecoJetsBflavorMediumEta2p5%d" % icoll))
##        	    recoPos += 1
        	
        	
        	
        	
#####Tig	ht b-jets
        	if( not hasattr(process,"filteredRecoJetsBflavorTightEta2p5%d" % icoll) ): 
        	    setattr(process,"filteredRecoJetsBflavorTightEta2p5%d" % icoll,cms.EDFilter("FlashggJetCandidateSelector",
        	                                                                            src=coll,
        	                                                                            cut=cms.string("pt>%f && abs(eta)<2.5 && passesJetID('Loose') && bDiscriminator( '%s' ) > %f " % (jetPtCut, flashggBTag, bDiscriminator80XReReco[2])),
        	
        	
        	                                                                            ) )
        	    
        	    recoJetsBflavorTight2p5.append("filteredRecoJetsBflavorTightEta2p5%d" % icoll)
        	    tagSequence.insert(recoPos, getattr(process,"filteredRecoJetsBflavorTightEta2p5%d" % icoll))
        	    recoPos += 1

            
##standard jets
    if doJets:
	    if( not hasattr(process,"flashggRecoHadronicActivity2p5") ): 
	        process.flashggRecoHadronicActivity2p5 = cms.EDProducer("FlashggDiPhotonHadronicActivityProducer",
	                                                                src=recoJets2p5,
	                                                                veto=cms.InputTag(recoDiphotons)
	                                                                )
	        tagSequence.insert(recoPos, getattr(process,"flashggRecoHadronicActivity2p5"))
	        recoPos += 1
	    if( not hasattr(process,"flashggRecoHadronicActivity4p7") ): 
	        process.flashggRecoHadronicActivity4p7 = cms.EDProducer("FlashggDiPhotonHadronicActivityProducer",
	                                                                src=recoJets4p7,
	                                                                veto=cms.InputTag(recoDiphotons)
	                                                                )
	        tagSequence.insert(recoPos, getattr(process,"flashggRecoHadronicActivity4p7"))
	        recoPos += 1

    if doBJetsAndMET:
#	    if( not hasattr(process,"flashggRecoHadronicActivityBflavorMedium2p5") ): 
#	        process.flashggRecoHadronicActivityBflavorMedium2p5 = cms.EDProducer("FlashggDiPhotonHadronicActivityProducer",
#	                                                                src=recoJetsBflavorMedium2p5,
#	                                                                veto=cms.InputTag(recoDiphotons) ##veto also muons!
#	                                                                )
#	        tagSequence.insert(recoPos, getattr(process,"flashggRecoHadronicActivityBflavorMedium2p5"))
#	        recoPos += 1
	
	
	
	    if( not hasattr(process,"flashggRecoHadronicActivityBflavorTight2p5") ): 
	        process.flashggRecoHadronicActivityBflavorTight2p5 = cms.EDProducer("FlashggDiPhotonHadronicActivityProducer",
	                                                                src=recoJetsBflavorTight2p5,
	                                                                veto=cms.InputTag(recoDiphotons) ##veto also muons!
	                                                                )
	        tagSequence.insert(recoPos, getattr(process,"flashggRecoHadronicActivityBflavorTight2p5"))
	        recoPos += 1


    if doBJetsAndMET:
        if( not hasattr(process,"flashggRecoHadronicActivityMET") ): 
            process.flashggRecoHadronicActivityMET = cms.EDProducer("FlashggDiPhotonHadronicActivityProducer",
                                                                    src=recoMet,
                                                                    veto=cms.InputTag(recoDiphotons)
                                                                    )
            tagSequence.insert(recoPos, getattr(process,"flashggRecoHadronicActivityMET"))
            recoPos += 1
    if doBJetsAndMET:
        if( not hasattr(process,"flashggRecoHadronicActivityMuons") ): 
            process.flashggRecoHadronicActivityMuons = cms.EDProducer("FlashggDiPhotonHadronicActivityProducer",
                                                                    src=recoMuons,
                                                                    veto=cms.InputTag(recoDiphotons) 
                                                                    )
            tagSequence.insert(recoPos, getattr(process,"flashggRecoHadronicActivityMuons"))
            recoPos += 1
    
        if( not hasattr(process,"flashggRecoHadronicActivityElectrons") ): 
            process.flashggRecoHadronicActivityElectrons = cms.EDProducer("FlashggDiPhotonHadronicActivityProducer",
                                                                    src=recoElectrons,
                                                                    veto=cms.InputTag(recoDiphotons) 
                                                                    )
            tagSequence.insert(recoPos, getattr(process,"flashggRecoHadronicActivityElectrons"))
            recoPos += 1


  ##  if( not hasattr(process,"flashggRecoHadronicActivityMuons") ): 
  ##      process.flashggRecoHadronicActivityMuons = cms.EDProducer("FlashggDiPhotonHadronicActivityProducer",
  ##                                                              src=cms.InputTag("flashggSelectedMuons"),
  ##                                                              veto=cms.InputTag(recoDiphotons)
  ##                                                              )
  ##      tagSequence.insert(recoPos, getattr(process,"flashggRecoHadronicActivityMuons"))
  ##      recoPos += 1

    if doJets:
        recoDiphotonTags.CompositeCandidateTags.jets2p5 = cms.InputTag("flashggRecoHadronicActivity2p5")
        recoDiphotonTags.CompositeCandidateTags.jets4p7 = cms.InputTag("flashggRecoHadronicActivity4p7")
    if doBJetsAndMET:
##        recoDiphotonTags.CompositeCandidateTags.jetsBflavorMedium2p5 = cms.InputTag("flashggRecoHadronicActivityBflavorMedium2p5")
        recoDiphotonTags.CompositeCandidateTags.jetsBflavorTight2p5 = cms.InputTag("flashggRecoHadronicActivityBflavorTight2p5")
    if doBJetsAndMET:
        recoDiphotonTags.CompositeCandidateTags.met = cms.InputTag("flashggRecoHadronicActivityMET")
        recoDiphotonTags.CompositeCandidateTags.muons = cms.InputTag("flashggRecoHadronicActivityMuons")
        recoDiphotonTags.CompositeCandidateTags.electrons = cms.InputTag("flashggRecoHadronicActivityElectrons")
##    recoDiphotonTags.CompositeCandidateTags.muons = cms.InputTag("flashggRecoHadronicActivityMuons")

    if (not processId=="Data") and hasattr(process,"genSequence"):
        genPos = process.genSequence.index(getattr(process,genDiphotons))+1
        if doJets:
            if( not hasattr(process,"filteredGenJetsEta2p5") ):
	        process.filteredGenJetsEta2p5 = cms.EDFilter("GenJetSelector",
	                                                     src=cms.InputTag(genJetCollection),
	                                                     cut=cms.string("pt>%f && abs(eta)<2.5 && numberOfDaughters > 5" % jetPtCut),
	        )
                process.genSequence.insert(genPos, getattr(process,"filteredGenJetsEta2p5"))
                genPos += 1
	        process.flashggGenHadronicActivity2p5 = cms.EDProducer("FlashggGenHadronicActivityProducer",
	                                                               src=cms.InputTag("filteredGenJetsEta2p5"),
	                                                               veto=cms.InputTag(genDiphotons)
	        )
                process.genSequence.insert(genPos, getattr(process,"flashggGenHadronicActivity2p5"))
                genPos += 1
	    if( not hasattr(process,"filteredGenJetsEta4p7") ): 
	        process.filteredGenJetsEta4p7 = cms.EDFilter("GenJetSelector",
	                                                     src=cms.InputTag(genJetCollection),
	                                                     cut=cms.string("pt>%f && abs(eta)<4.7 && numberOfDaughters > 5" % jetPtCut),
	        )
	        process.genSequence.insert(genPos, getattr(process,"filteredGenJetsEta4p7"))
                genPos += 1
	        process.flashggGenHadronicActivity4p7 = cms.EDProducer("FlashggGenHadronicActivityProducer",
	                                                                   src=cms.InputTag("filteredGenJetsEta4p7"),
	                                                                   veto=cms.InputTag(genDiphotons)
	                                                                   )
                process.genSequence.insert(genPos, getattr(process,"flashggGenHadronicActivity4p7"))
                genPos += 1
        if doBJetsAndMET:
            if genBJetCollection == "flashggGenJetsExtra" and not hasattr(process,"flashggGenJetsExtra"):
                process.load("flashgg.MicroAOD.flashggGenJetsExtra_cfi")
                process.genSequence.insert(genPos, getattr(process,"flashggGenJetsExtra"))
                genPos += 1
                
            if( not hasattr(process,"filteredGenJetsBflavorEta2p5") ): 
        	process.filteredGenJetsBflavorEta2p5 = cms.EDFilter("GenJetSelector",
        	                                                    src=cms.InputTag(genBJetCollection),
                                                                    ##      	                                                         cut=cms.string("cand.pt>%f && abs(cand.eta)<2.5 && cand.numberOfDaughters > 5" % jetPtCut),
        	                                                    cut=cms.string("pt>%f && abs(eta)<2.5 && numberOfDaughters > 5 && (hasBottom==1 || (hasBquark==1 && deltaRBquarkGenJet<0.15 && jetPtOverBquarkPt > 0.5))" % jetPtCut),
                                                                    #       	                                                        cut=cms.string("pt>%f && abs(eta)<2.5 && numberOfDaughters > 5" % jetPtCut),
                                                                    ##      	                                                         cut=cms.string("pt>%f && abs(eta)<2.5 && numberOfDaughters > 5 && hasBottom == 1" % jetPtCut),
                                                                    ##      	                                                         cut=cms.string("hasBottom == 1"),
        	                                            )
        	process.genSequence.insert(genPos, getattr(process,"filteredGenJetsBflavorEta2p5"))
                genPos += 1
        	process.flashggGenHadronicActivityBflavor2p5 = cms.EDProducer("FlashggGenHadronicActivityProducer",
        	                                                              src=cms.InputTag("filteredGenJetsBflavorEta2p5"),
        	                                                              veto=cms.InputTag(genDiphotons)
        	)
                process.genSequence.insert(genPos, getattr(process,"flashggGenHadronicActivityBflavor2p5"))
                genPos += 1
                
        if doBJetsAndMET:
            if( not hasattr(process,"flashggGenHadronicActivityMET") ): 
                process.flashggGenHadronicActivityMET = cms.EDProducer("FlashggGenHadronicActivityProducer",
                                                                       src=cms.InputTag("flashggMets"),
                                                                       veto=cms.InputTag(genDiphotons)
                                                                       )
                process.genSequence.insert(genPos, getattr(process,"flashggGenHadronicActivityMET"))
                genPos += 1

        if( not hasattr(process,"filteredGenJetsEtaInclusive") ): 
            process.filteredGenJetsEtaInclusive = cms.EDFilter("GenJetSelector",
                                                         src=cms.InputTag(genJetCollection),
#                                                         cut=cms.string("pt>%f && abs(eta)<2.5 && nCarrying(0.90)>1" % jetPtCut),
###                                                         cut=cms.string("pt>%f && numberOfDaughters > 5" % jetPtCut),
                                                         cut=cms.string("1"),
                                                         )
            process.genSequence.insert(genPos, getattr(process, "filteredGenJetsEtaInclusive"))
            genPos += 1
            process.flashggGenHadronicActivityInclusive = cms.EDProducer("FlashggGenHadronicActivityProducer",
                                                                   src=cms.InputTag("filteredGenJetsEtaInclusive"),
                                                                   veto=cms.InputTag(genDiphotons)
                                                                   )
            process.genSequence.insert(genPos, getattr(process,"flashggGenHadronicActivityInclusive"))
            genPos += 1
        if doBJetsAndMET:
            if not hasattr(process, "flashggGenLeptons"):
                process.load("flashgg.MicroAOD.flashggGenLeptons_cfi")
                process.genSequence.insert(genPos, getattr(process, "flashggGenLeptons"))
                genPos += 1
                
            if not hasattr(process, "flashggGenLeptonsExtra"):
                process.load("flashgg.MicroAOD.flashggGenLeptonsExtra_cfi")
                process.genSequence.insert(genPos, getattr(process, "flashggGenLeptonsExtra"))
                genPos += 1
                
            if( not hasattr(process,"flashggGenHadronicActivityLeptons2p4") ):
                    
                process.filteredGenLeptonsEta2p4 = cms.EDFilter("GenLeptonExtraSelector",
                                                             src=cms.InputTag("flashggGenLeptonsExtra"),
                                                             cut=cms.string("pt>20 && abs(eta)<2.4"),
                                                         )
                process.genSequence.insert(genPos, getattr(process,"filteredGenLeptonsEta2p4"))
                genPos += 1            
                process.flashggGenHadronicActivityLeptons2p4 = cms.EDProducer("FlashggGenHadronicActivityProducer",
                                                                       src=cms.InputTag("filteredGenLeptonsEta2p4"),
                                                                       veto=cms.InputTag(genDiphotons)
                                                                       )
                process.genSequence.insert(genPos, getattr(process,"flashggGenHadronicActivityLeptons2p4"))
                genPos += 1
        
# ----------------------------------------------------------------------------------------------------------------
def getJetKinVariables(pre,post,variables,nmax, getter):
    return reduce(lambda z,w: z+w, 
                  map(lambda x: 
#                      map(lambda y: ("%sJet%s%s%d := ? numberOfDaughters > %d ? daughter(%d).%s : 0" % (pre,post,y.split("[")[0].capitalize(),x,x,x,y.split("[")[0])), variables),
                      map(lambda y: ("%sJet%s%s := ? %snumberOfDaughters > %d ? %sdaughter(%d).%s : -999" % (pre,  post,  (str(x)+"[").join(y.capitalize().rsplit("[",1)) if "[" in y else y.capitalize()+str(x),  getter,  x,  getter,  x,  y.split("[")[0])),  variables),
                      xrange(nmax)
                      )
                  )

#----------------------------------------------------------------------------------------------------------------
def getObjKinVariables(pre,post,obj,variables,nmax, getter):
    return reduce(lambda z,w: z+w, 
                  map(lambda x: 
#                      map(lambda y: ("%sJet%s%s%d := ? numberOfDaughters > %d ? daughter(%d).%s : 0" % (pre,post,y.split("[")[0].capitalize(),x,x,x,y.split("[")[0])), variables),
                      map(lambda y: ("%s%s%s%s := ? %snumberOfDaughters > %d ? %sdaughter(%d).%s : -999" % (pre, obj,  post,  (str(x)+"[").join(y.capitalize().rsplit("[",1)) if "[" in y else y.capitalize()+str(x),  getter,  x,  getter,  x,  y.split("[")[0])),  variables),
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
def addJetGlobalVariables(process,dumper,src,pre,post,tagSequence,getter=""):    
    import flashgg.Taggers.dumperConfigTools as cfgTools
    from flashgg.MicroAOD.flashggJets_cfi import flashggBTag
    if post=="Muons":
        print "adding variables for muons"

    if getter != "": getter += "."
    variables  = [ "%(pre)sNjets%(post)s[-1,(-0.5:0.5:1.5:2.5:3.5:100)]:= %(getter)snumberOfDaughters" % locals() ]
#    variables  = [ "%sNjets%s:=numberOfDaughters" % (pre,post) ]
#    variables += getJetKinVariables(pre,post,["pt[667,0.0,10005.0]","eta[50,-5.0,5.0]","rapidity[50,0.0,10.0]"],5)
    if pre =="reco":
        variables += getJetKinVariables(pre,post,["pt","eta","rapidity","phi","px","py","pz", "energy", "numberOfDaughters[200,-0.5,199.5]", "puJetIdMVA", ],6, getter)

#        print  ("%sJet%sBdiscriminant0 := ? %snumberOfDaughters > 0 ? %sdaughter(0).bDiscriminator('pfCombinedInclusiveSecondaryVertexV2BJetTags') : -999" % (pre,  post,  getter,  getter))
#        variables+=[("%sJet%sBdiscriminant0 := ? %snumberOfDaughters > 0 ? %sdaughter(0).bDiscriminator('pfCombinedInclusiveSecondaryVertexV2BJetTags') : -999" % (pre,  post,  getter,  getter))]
    else:
        variables += getJetKinVariables(pre,post,["pt","eta","rapidity","phi","px","py","pz", "energy", "numberOfDaughters[200,-0.5,199.5]"],6, getter)
    # print "jet global variables"
    # print variables
###    variables += getJetKinVariables(pre,post,["pt","eta","rapidity"],5)
###    variables += [ "%sDijetMass%s := ? numberOfDaughters > 1 ? sqrt( (daughter(0).energy+daughter(1).energy)^2 - (daughter(0).px+daughter(1).px)^2 - (daughter(0).py+daughter(1).py)^2 - (daughter(0).pz+daughter(1).pz)^2 ) : 0" % (pre,post) ]
    
    # if post=="Muons":
    #     print "variables for muons"
    #     print variables

    if src:
        cfgTools.addGlobalFloats(process,dumper.globalVariables,src,variables,tagSequence)
    else:
        for cat in dumper.categories:
            cfgTools.addVariables(cat.variables,variables)


def addBflavorJetGlobalVariables(process,dumper,src,pre,post,tagSequence,getter=""):    
    import flashgg.Taggers.dumperConfigTools as cfgTools
    from flashgg.MicroAOD.flashggJets_cfi import flashggBTag

    if getter != "": getter += "."
    variables  = [ "%(pre)sNjets%(post)s[-1,(-0.5:0.5:1.5:2.5:3.5:100)]:= %(getter)snumberOfDaughters" % locals() ]
    if pre =="reco":
        variables += getJetKinVariables(pre,post,["pt","eta","rapidity","phi","px","py","pz", "energy", "numberOfDaughters", "puJetIdMVA", ],6, getter)

    else:
#        variables += getJetKinVariables(pre,post,["pt","eta","rapidity","phi","px","py","pz", "energy", "cand().hasBottom", "cand().hasBquark", "cand().deltaRBquarkGenJet", "cand().jetPtOverBquarkPt", "numberOfDaughters"],6, getter)
        variables += getJetKinVariables(pre,post,["pt","eta","rapidity","phi","px","py","pz", "energy", "numberOfDaughters"],6, getter)
#    variables += [ "%(pre)sBjetHasBottom%(post)s0 := ? %(getter)snumberOfDaughters > 0 ? %(getter)sdaughter(0).hasBottom : -999" % locals() ]
    # print "jet global variables"
    # print variables


    if src:
        cfgTools.addGlobalFloats(process,dumper.globalVariables,src,variables,tagSequence)
    else:
        for cat in dumper.categories:
            cfgTools.addVariables(cat.variables,variables)




# ----------------------------------------------------------------------------------------------------------------
def addLeptonGlobalVariables(process,dumper,src,pre,post,tagSequence,getter=""):    
    import flashgg.Taggers.dumperConfigTools as cfgTools

    if getter != "": getter += "."
    variables  = [ "%(pre)sNleptons%(post)s[-1,(-0.5:0.5:1.5:2.5:3.5:100)]:= %(getter)snumberOfDaughters" % locals() ]

    variables += getObjKinVariables(pre,post,"Lepton",["pt","eta","rapidity","phi","px","py","pz", "energy" ],4, getter)

##    variables += [ "%(pre)sLeptonPt%(post)s0 := ? %(getter)snumberOfDaughters > 0 ? %(getter)sdaughter(0).p4().pt : -999" % locals() ]
##    variables += [ "%(pre)sLeptonPt%(post)s1 := ? %(getter)snumberOfDaughters > 1 ? %(getter)sdaughter(1).p4().pt : -999" % locals() ]
##    variables += [ "%(pre)sLeptonPt%(post)s2 := ? %(getter)snumberOfDaughters > 2 ? %(getter)sdaughter(2).p4().pt : -999" % locals() ]
##    variables += [ "%(pre)sLeptonPt%(post)s3 := ? %(getter)snumberOfDaughters > 3 ? %(getter)sdaughter(3).p4().pt : -999" % locals() ]
##
##    variables += [ "%(pre)sLeptonEta%(post)s0 := ? %(getter)snumberOfDaughters > 0 ? %(getter)sdaughter(0).eta : -999" % locals() ]
##    variables += [ "%(pre)sLeptonEta%(post)s1 := ? %(getter)snumberOfDaughters > 1 ? %(getter)sdaughter(1).eta : -999" % locals() ]
##    variables += [ "%(pre)sLeptonEta%(post)s2 := ? %(getter)snumberOfDaughters > 2 ? %(getter)sdaughter(2).eta : -999" % locals() ]
##    variables += [ "%(pre)sLeptonEta%(post)s3 := ? %(getter)snumberOfDaughters > 3 ? %(getter)sdaughter(3).eta : -999" % locals() ]
##
##    variables += [ "%(pre)sLeptonPhi%(post)s0 := ? %(getter)snumberOfDaughters > 0 ? %(getter)sdaughter(0).phi : -999" % locals() ]
##    variables += [ "%(pre)sLeptonPhi%(post)s1 := ? %(getter)snumberOfDaughters > 1 ? %(getter)sdaughter(1).phi : -999" % locals() ]
##    variables += [ "%(pre)sLeptonPhi%(post)s2 := ? %(getter)snumberOfDaughters > 2 ? %(getter)sdaughter(2).phi : -999" % locals() ]
##    variables += [ "%(pre)sLeptonPhi%(post)s3 := ? %(getter)snumberOfDaughters > 3 ? %(getter)sdaughter(3).phi : -999" % locals() ]


#    variables += [ "%(pre)sLeptonPt%(post)s0 := ? %(getter)snumberOfDaughters > 0 ? %(getter)sdaughter(0).cand().p4().pt : -999" % locals() ]
#    variables += [ "%(pre)sLeptonPt%(post)s1 := ? %(getter)snumberOfDaughters > 1 ? %(getter)sdaughter(1).cand().p4().pt : -999" % locals() ]

###    variables += [ "%(pre)sLepton%(post)s1[-1,(-0.5:0.5:1.5:2.5:3.5:100)]:= ? %(getter)snumberOfDaughters > 1 ? %(getter)sdaughter(1).dressedP4().pt() : -999" % locals() ]
#    variables += getLeptonKinVariables(pre,post,["dressedP4().pt"],3, getter)
#    print variables
###    variables += getLeptonKinVariables(pre,post,["pt","eta","rapidity"],5)
###    variables += [ "%sDijetMass%s := ? numberOfDaughters > 1 ? sqrt( (daughter(0).energy+daughter(1).energy)^2 - (daughter(0).px+daughter(1).px)^2 - (daughter(0).py+daughter(1).py)^2 - (daughter(0).pz+daughter(1).pz)^2 ) : 0" % (pre,post) ]
    
    if src:
        cfgTools.addGlobalFloats(process,dumper.globalVariables,src,variables,tagSequence)
    else:
        for cat in dumper.categories:
            cfgTools.addVariables(cat.variables,variables)




# ----------------------------------------------------------------------------------------------------------------
def addMuonGlobalVariables(process,dumper,src,pre,post,tagSequence,getter=""):    
    import flashgg.Taggers.dumperConfigTools as cfgTools

    if getter != "": getter += "."
    variables  = [ "%(pre)sNmuons%(post)s[-1,(-0.5:0.5:1.5:2.5:3.5:100)]:= %(getter)snumberOfDaughters" % locals() ]

    variables += getObjKinVariables(pre,post,"Muon",["pt","eta","rapidity","phi","px","py","pz", "energy" ],4, getter)


##    variables += [ "%(pre)sMuonPt%(post)s0 := ? %(getter)snumberOfDaughters > 0 ? %(getter)sdaughter(0).pt : -999" % locals() ]
##    variables += [ "%(pre)sMuonPt%(post)s1 := ? %(getter)snumberOfDaughters > 1 ? %(getter)sdaughter(1).pt : -999" % locals() ]
##    variables += [ "%(pre)sMuonPt%(post)s2 := ? %(getter)snumberOfDaughters > 2 ? %(getter)sdaughter(2).pt : -999" % locals() ]
##    variables += [ "%(pre)sMuonPt%(post)s3 := ? %(getter)snumberOfDaughters > 3 ? %(getter)sdaughter(3).pt : -999" % locals() ]
##
##    variables += [ "%(pre)sMuonPt%(post)s0 := ? %(getter)snumberOfDaughters > 0 ? %(getter)sdaughter(0).pt : -999" % locals() ]
##    variables += [ "%(pre)sMuonPt%(post)s1 := ? %(getter)snumberOfDaughters > 1 ? %(getter)sdaughter(1).pt : -999" % locals() ]
##    variables += [ "%(pre)sMuonPt%(post)s2 := ? %(getter)snumberOfDaughters > 2 ? %(getter)sdaughter(2).pt : -999" % locals() ]
##    variables += [ "%(pre)sMuonPt%(post)s3 := ? %(getter)snumberOfDaughters > 3 ? %(getter)sdaughter(3).pt : -999" % locals() ]
##
##
##
##    variables += [ "%(pre)sMuonEta%(post)s0 := ? %(getter)snumberOfDaughters > 0 ? %(getter)sdaughter(0).eta : -999" % locals() ]
##    variables += [ "%(pre)sMuonEta%(post)s1 := ? %(getter)snumberOfDaughters > 1 ? %(getter)sdaughter(1).eta : -999" % locals() ]
##
##    variables += [ "%(pre)sMuonPhi%(post)s0 := ? %(getter)snumberOfDaughters > 0 ? %(getter)sdaughter(0).phi : -999" % locals() ]
##    variables += [ "%(pre)sMuonPhi%(post)s1 := ? %(getter)snumberOfDaughters > 1 ? %(getter)sdaughter(1).phi : -999" % locals() ]

#    variables += [ "%(pre)sLeptonPt%(post)s0 := ? %(getter)snumberOfDaughters > 0 ? %(getter)sdaughter(0).cand().p4().pt : -999" % locals() ]
#    variables += [ "%(pre)sLeptonPt%(post)s1 := ? %(getter)snumberOfDaughters > 1 ? %(getter)sdaughter(1).cand().p4().pt : -999" % locals() ]

###    variables += [ "%(pre)sLepton%(post)s1[-1,(-0.5:0.5:1.5:2.5:3.5:100)]:= ? %(getter)snumberOfDaughters > 1 ? %(getter)sdaughter(1).dressedP4().pt() : -999" % locals() ]
#    variables += getLeptonKinVariables(pre,post,["dressedP4().pt"],3, getter)
#    print variables
###    variables += getLeptonKinVariables(pre,post,["pt","eta","rapidity"],5)
###    variables += [ "%sDijetMass%s := ? numberOfDaughters > 1 ? sqrt( (daughter(0).energy+daughter(1).energy)^2 - (daughter(0).px+daughter(1).px)^2 - (daughter(0).py+daughter(1).py)^2 - (daughter(0).pz+daughter(1).pz)^2 ) : 0" % (pre,post) ]
    
    if src:
        cfgTools.addGlobalFloats(process,dumper.globalVariables,src,variables,tagSequence)
    else:
        for cat in dumper.categories:
            cfgTools.addVariables(cat.variables,variables)
        


# ----------------------------------------------------------------------------------------------------------------
def addElectronGlobalVariables(process,dumper,src,pre,post,tagSequence,getter=""):    
    import flashgg.Taggers.dumperConfigTools as cfgTools

    if getter != "": getter += "."
    variables  = [ "%(pre)sNelectrons%(post)s[-1,(-0.5:0.5:1.5:2.5:3.5:100)]:= %(getter)snumberOfDaughters" % locals() ]
    variables += getObjKinVariables(pre,post,"Electron",["pt","eta","rapidity","phi","px","py","pz", "energy" ],4, getter)

##    variables += [ "%(pre)sElectronPt%(post)s0 := ? %(getter)snumberOfDaughters > 0 ? %(getter)sdaughter(0).pt : -999" % locals() ]
##    variables += [ "%(pre)sElectronPt%(post)s1 := ? %(getter)snumberOfDaughters > 1 ? %(getter)sdaughter(1).pt : -999" % locals() ]
##
##    variables += [ "%(pre)sElectronEta%(post)s0 := ? %(getter)snumberOfDaughters > 0 ? %(getter)sdaughter(0).eta : -999" % locals() ]
##    variables += [ "%(pre)sElectronEta%(post)s1 := ? %(getter)snumberOfDaughters > 1 ? %(getter)sdaughter(1).eta : -999" % locals() ]
##
##    variables += [ "%(pre)sElectronPhi%(post)s0 := ? %(getter)snumberOfDaughters > 0 ? %(getter)sdaughter(0).phi : -999" % locals() ]
##    variables += [ "%(pre)sElectronPhi%(post)s1 := ? %(getter)snumberOfDaughters > 1 ? %(getter)sdaughter(1).phi : -999" % locals() ]

    if src:
        cfgTools.addGlobalFloats(process,dumper.globalVariables,src,variables,tagSequence)
    else:
        for cat in dumper.categories:
            cfgTools.addVariables(cat.variables,variables)
        

# ----------------------------------------------------------------------------------------------------------------
def addMETGlobalVariables(process,dumper,src,pre,post,tagSequence,getter=""):    
    import flashgg.Taggers.dumperConfigTools as cfgTools

    if getter != "": getter += "."
    variables  = [ "%(pre)sMET%(post)s :=  %(getter)sgenMET.pt  " % locals() ]
    variables += [ "%(pre)sMET%(post)sRapidity :=  %(getter)sgenMET.rapidity " % locals() ]    
    variables += [ "%(pre)sMET%(post)sEta :=  %(getter)sgenMET.eta " % locals() ]
    variables += [ "%(pre)sMET%(post)sPhi :=  %(getter)sgenMET.phi " % locals() ]    
    variables += [ "%(pre)sMET%(post)sPx :=  %(getter)sgenMET.px " % locals() ]
    variables += [ "%(pre)sMET%(post)sPy :=  %(getter)sgenMET.py " % locals() ]
    variables += [ "%(pre)sMET%(post)sPz :=  %(getter)sgenMET.pz " % locals() ]
    variables += [ "%(pre)sMET%(post)sEnergy :=  %(getter)sgenMET.energy " % locals() ]


    if src:
        cfgTools.addGlobalFloats(process,dumper.globalVariables,src,variables,tagSequence)
    else:
        for cat in dumper.categories:
            cfgTools.addVariables(cat.variables,variables)
        
# ----------------------------------------------------------------------------------------------------------------
def addRecoMETGlobalVariables(process,dumper,src,pre,post,tagSequence,getter=""):    
    import flashgg.Taggers.dumperConfigTools as cfgTools

    if getter != "": getter += "."
    variables  = [ "%(pre)sMET%(post)sPt :=  %(getter)spt " % locals() ]
    variables += [ "%(pre)sMET%(post)sRapidity :=  %(getter)srapidity " % locals() ]
    variables += [ "%(pre)sMET%(post)sEta :=  %(getter)seta " % locals() ]
    variables += [ "%(pre)sMET%(post)sPhi :=  %(getter)sphi " % locals() ]
    variables += [ "%(pre)sMET%(post)sPx :=  %(getter)spx " % locals() ]
    variables += [ "%(pre)sMET%(post)sPy :=  %(getter)spy " % locals() ]
    variables += [ "%(pre)sMET%(post)sPz :=  %(getter)spz " % locals() ]
    variables += [ "%(pre)sMET%(post)sEnergy :=  %(getter)senergy " % locals() ]

    # print "recoMET variables"
    # print variables
    if src:
        cfgTools.addGlobalFloats(process,dumper.globalVariables,src,variables,tagSequence)
    else:
        for cat in dumper.categories:
            cfgTools.addVariables(cat.variables,variables)

# ----------------------------------------------------------------------------------------------------------------
def addGenGlobalVariables(process,dumper,tagSequence):
    if doJets:
        addJetGlobalVariables(process,dumper,"flashggGenHadronicActivity2p5","gen","2p5",tagSequence)
        addJetGlobalVariables(process,dumper,"flashggGenHadronicActivity4p7","gen","4p7",tagSequence)
        addJetGlobalVariables(process,dumper,"flashggGenHadronicActivityInclusive","gen","Inclusive",tagSequence)
    if doBJetsAndMET:
        addBflavorJetGlobalVariables(process,dumper,"flashggGenHadronicActivityBflavor2p5","gen","Bflavor2p5",tagSequence)


        addLeptonGlobalVariables(process,dumper,"flashggGenHadronicActivityLeptons2p4","gen","2p4",tagSequence)
    if doBJetsAndMET:
        addMETGlobalVariables(process,dumper,"flashggMets","gen","all",tagSequence)

# ----------------------------------------------------------------------------------------------------------------
def addRecoGlobalVariables(process,dumper,tagSequence,tagGetter=""):
    if tagGetter != "": tagGetter += "."
    if doJets:
        addJetGlobalVariables(process,dumper,None,"reco","2p5",tagSequence,"%sgetCompCand('jets2p5')" % tagGetter)
        addJetGlobalVariables(process,dumper,None,"reco","4p7",tagSequence,"%sgetCompCand('jets4p7')" % tagGetter)
    if doBJetsAndMET:
##        addJetGlobalVariables(process,dumper,None,"reco","BflavorMedium2p5","%sgetCompCand('jetsBflavorMedium2p5')" % tagGetter)
        addJetGlobalVariables(process,dumper,None,"reco","BflavorTight2p5",tagSequence,"%sgetCompCand('jetsBflavorTight2p5')" % tagGetter)

    if doBJetsAndMET:
        addRecoMETGlobalVariables(process,dumper,None,"reco","All",tagSequence,"%sgetCompCand('met')" % tagGetter)
        addMuonGlobalVariables(process,dumper,None,"reco","All",tagSequence,"%sgetCompCand('muons')" % tagGetter)
        addElectronGlobalVariables(process,dumper,None,"reco","All",tagSequence,"%sgetCompCand('electrons')" % tagGetter)

#    addJetGlobalVariables(process,dumper,None,"reco","Muons","%sgetCompCand('muons')" % tagGetter)
    
    
# ----------------------------------------------------------------------------------------------------------------
def addGenOnlyAnalysis(process,processId,tagSequence,acceptance,tagList,systlabels,pdfWeights=None,recoJetCollections=None,mH=None,filterEvents=True): #NNLOPSreweight=False, genToReweight=None
    import itertools
    import flashgg.Taggers.dumperConfigTools as cfgTools
    

    print 'pdfWeights in addGenOnlyAnalysis'
    print pdfWeights
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
    from flashgg.MicroAOD.flashggGenDiPhotons_cfi import flashggGenDiPhotons
    process.flashggGenDiPhotons = flashggGenDiPhotons.clone()
    #process.p.insert(0, process.flashggGenDiPhotons+process.flashggSelectedGenDiPhotons+process.flashggSortedGenDiPhotons)
    #tagSequence.insert(tagSequence.index(process.flashggSigmaMoMpToMTag)+1,getattr(process,"flashggGenDiPhotons")+getattr(process,"flashggSelectedGenDiPhotons")+getattr(process,"flashggSortedGenDiPhotons"))
    ###process.flashggPreselectedGenDiPhotons = process.flashggSelectedGenDiPhotons.clone(filter = cms.bool(filterEvents), cut=cms.string(preselCut))
    ### process.flashggGenDiPhotonsSequence.insert(process.flashggGenDiPhotonsSequence.index(process.flashggSelectedGenDiPhotons),process.flashggPreselectedGenDiPhotons)
    ### process.flashggSelectedGenDiPhotons.src = "flashggPreselectedGenDiPhotons"

    process.load("flashgg.Taggers.flashggTaggedGenDiphotons_cfi")
    process.flashggTaggedGenDiphotons.src  = "flashggSortedGenDiPhotons"
    process.flashggTaggedGenDiphotons.tags = "flashggTagSorter"
    process.flashggTaggedGenDiphotons.remap = process.tagsDumper.classifierCfg.remap
    ## process.flashggTaggedGenDiphotons.tags = "flashggSystTagMerger"
    ##process.p += process.flashggTaggedGenDiphotons
    ##tagSequence.insert(tagSequence.index(getattr(process,"flashggTagSorter"))+1,getattr(process,"flashggTaggedGenDiphotons"))
    
    process.flashggSelectedGenDiPhotons.cut = "(%s) && (%s)" % ( preselCut, cut )
    process.flashggSortedGenDiPhotons.maxNumber = 999
    ## only process events where at least one diphoton candidate is selected
    if filterEvents:
        process.flashggSelectedGenDiPhotons.filter = True
        process.genFilter += process.flashggGenDiPhotons+process.flashggSelectedGenDiPhotons+process.flashggSortedGenDiPhotons
    
    process.load("flashgg.Taggers.genDiphotonDumper_cfi")
    process.genDiphotonDumper.dumpTrees = True
    process.genDiphotonDumper.dumpHistos = False
    process.genDiphotonDumper.maxCandPerEvent = -1
    process.genDiphotonDumper.dumpWorkspace = False
    process.genDiphotonDumper.src = "flashggTaggedGenDiphotons"

    from flashgg.Taggers.globalVariables_cff import globalVariables
    process.genDiphotonDumper.dumpGlobalVariables = True
    process.genDiphotonDumper.globalVariables = globalVariables

    process.genSequence = cms.Sequence(process.flashggTaggedGenDiphotons)
    
    ## bookHadronicActivityProducers(process,processId,"flashggTagSorter","flashggTaggedGenDiphotons",recoJetCollections,genJetCollection="slimmedGenJets")
    bookHadronicActivityProducers(process,processId,tagSequence,"flashggDiPhotonSystematics",process.flashggSigmaMoMpToMTag,"flashggTaggedGenDiphotons",recoJetCollections,genJetCollection="slimmedGenJets") ##FIXME 
    addGenGlobalVariables(process,process.genDiphotonDumper,process.genSequence)
    addRecoGlobalVariables(process,process.genDiphotonDumper,tagSequence,tagGetter="tag()")

    #process.p.insert(process.p.index(tagSequence)+1,getattr(process,"genSequence"))
    
    dumpPdfWeights,nPdfWeights,nAlphaSWeights,nScaleWeights=False,-1,-1,-1 
    if pdfWeights:
        dumpPdfWeights,nPdfWeights,nAlphaSWeights,nScaleWeights=pdfWeights
    # print "genDiPhotonDumper: NNLOPSreweight ",NNLOPSreweight
    # if NNLOPSreweight:
    #     process.genDiphotonDumper.dumpNNLOPSweight = cms.untracked.bool(True)
    #     process.genDiphotonDumper.NNLOPSWeight=cms.FileInPath("flashgg/Taggers/data/NNLOPS_reweight.root")
    #     if genToReweight == "amcatnlo":
    #         print "GenDiphotonDumper: Gluon fusion amcatnlo: read NNLOPS reweighting file"
    #         process.genDiphotonDumper.generatorToBeReweightedToNNLOPS = cms.string("mcatnlo")
    #     if genToReweight == "powheg":
    #         print "GenDiphotonDumper: Gluon fusion powheg: read NNLOPS reweighting file"
    #         process.genDiphotonDumper.generatorToBeReweightedToNNLOPS = cms.string("powheg")
    # else:
    #     print "GenDiphotonDumper: NOT gluon fusion amcatnlo: set NNLOPS weights to 1."
    #     process.genDiphotonDumper.dumpNNLOPSweight = cms.untracked.bool(True)
    #     process.genDiphotonDumper.NNLOPSWeight=cms.double(1.0)
####        print "Data or background: no NNLOPS weights"
####        process.tagsDumper.dumpNNLOPSweight = cms.untracked.bool(False)

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
                         nScaleWeights=nScaleWeights,
                         splitPdfByStage0Bin=False
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
                                 nScaleWeights=nScaleWeights,
                                 splitPdfByStage0Bin=False
                                 )
            
            
    # process.pfid = cms.Path(process.genFilter*process.flashggGenDiPhotonsSequence*process.flashggTaggedGenDiphotons*process.genDiphotonDumper)
    process.pfid = cms.Path(process.genFilter*process.genSequence*process.genDiphotonDumper)
    

def bookCompositeObjects(process,processId,tagSequence,recoJetCollections=None):
    ## bookHadronicActivityProducers(process,processId,"flashggTagSorter","flashggTaggedGenDiphotons",recoJetCollections,genJetCollection="slimmedGenJets")
    bookHadronicActivityProducers(process,processId,tagSequence,"flashggDiPhotonSystematics",process.flashggSigmaMoMpToMTag,"flashggTaggedGenDiphotons",recoJetCollections,genJetCollection="slimmedGenJets")

def addObservables(process, dumper, processId, tagSequence, recoJetCollections=None):
    addRecoGlobalVariables(process,dumper,tagSequence)
    if not processId=="Data":
        addGenGlobalVariables(process,dumper,tagSequence)
    
###def extraReplacementsHook(newseq,systlabel,systtype):
###    from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet,massSearchReplaceAnyInputTag
###    if systtype=="jet":
###        from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag
###        recoJetCollections = UnpackedJetCollectionVInputTag
###        for icoll,coll in enumerate(recoJetCollections):        
###            massSearchReplaceAnyInputTag(newseq,cms.InputTag("filteredRecoJetsEta2p5%d" % icoll),cms.InputTag("filteredRecoJetsEta2p5%d%s" % (icoll,systlabel)))
###            massSearchReplaceAnyInputTag(newseq,cms.InputTag("filteredRecoJetsEta4p7%d" % icoll),cms.InputTag("filteredRecoJetsEta4p7%d%s" % (icoll,systlabel)))
###        massSearchReplaceAnyInputTag(newseq,cms.InputTag("flashggRecoHadronicActivity2p5"),cms.InputTag("flashggRecoHadronicActivity2p5%s" % (systlabel)))
###        massSearchReplaceAnyInputTag(newseq,cms.InputTag("flashggRecoHadronicActivity4p7"),cms.InputTag("flashggRecoHadronicActivityb4p7%s" % (systlabel)))
