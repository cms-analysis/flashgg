import FWCore.ParameterSet.Config as cms

##########################################################################
## TREE CONTENT
#########################################################################
    

ZVariablesToStore = cms.PSet(
    eta    = cms.string("eta"),
    abseta = cms.string("abs(eta)"),
    pt     = cms.string("pt"),
    mass   = cms.string("mass")
    )   

ProbeVariablesToStore = cms.PSet(
    probe_Pho_eta    = cms.string("eta"),
    probe_Pho_abseta = cms.string("abs(eta)"),
    probe_Pho_et     = cms.string("et"),
    probe_Pho_pt     = cms.string("pt"),
    probe_Pho_e      = cms.string("energy"),
    probe_Pho_sieie  = cms.string("full5x5_sigmaIetaIeta"),
    ## super cluster quantities
    probe_sc_energy  = cms.string("superCluster.energy"),
    probe_sc_rawe    = cms.string("superCluster.rawEnergy"),
    probe_sc_et      = cms.string("superCluster.energy*sin(superCluster.position.theta)"),    
    probe_sc_eta     = cms.string("superCluster.eta"),
    probe_sc_abseta  = cms.string("abs(superCluster.eta)"),
    
    #id based
    probe_Pho_r9            = cms.string("full5x5_r9"),
    probe_Pho_oldr9         = cms.string("old_r9"),
    probe_Pho_ESsigma       = cms.string("esEffSigmaRR"),
    probe_Pho_sieip         = cms.string("sieip"),
    probe_Pho_sipip         = cms.string("sipip"),
    probe_Pho_hoe           = cms.string("hadronicOverEm"),
    probe_Pho_mva           = cms.InputTag("photonFromDiPhotons:idmva"),
    #probe_Pho_mva2          = cms.string("phoIdMvaWrtChosenVtx"),
    probe_Pho_s4            = cms.string("s4"),
    probe_Pho_sigmaEoE      = cms.string("sigEOverE"),
    probe_Pho_etawidth      = cms.string("superCluster.etaWidth"),
    probe_Pho_phiwidth      = cms.string("superCluster.phiWidth"),
    #probe_Pho_trackFbrem() 
    
    #probe_Pho_e1x5          = cms.string("e1x5"),
    #probe_Pho_e2x5          = cms.string("e2x5"),
    #probe_Pho_e3x3          = cms.string("e3x3"),
    probe_Pho_e5x5          = cms.string("e5x5"),
    
    #probe_Pho_full5x5_e1x5  = cms.string("full5x5_e1x5"),
    #probe_Pho_full5x5_e2x5  = cms.string("full5x5_e2x5"),
    #probe_Pho_full5x5_e3x3  = cms.string("full5x5_e3x3"),
    probe_Pho_full5x5_e5x5  = cms.string("full5x5_e5x5"),
    
    #iso
    #probe_Pho_chIso       = cms.InputTag("photonIDValueMapProducer:phoChargedIsolation"),
    probe_Pho_chIso       = cms.string("pfChgIso03WrtVtx0"),
    #probe_Pho_chIsoRv     = cms.string("pfChIso03WrtChosenVtx"),
    probe_Pho_chIsoWv     = cms.string("pfChgIsoWrtWorstVtx03"),
    #probe_Pho_chIso2      = cms.string("egChargedHadronIso"),
    probe_Pho_neuIso03    = cms.string("pfNeutIso03()"),
    probe_Pho_phoIso03    = cms.string("pfPhoIso03"),
    probe_Pho_phoIso03Cor = cms.string("pfPhoIso03Corr"),
    #probe_Pho_neuIso04    = cms.string("pfNeutIso04()"),
    #probe_Pho_phoIso04    = cms.string("pfPhoIso04"),   
    probe_Pho_tkIso       = cms.string("trkSumPtHollowConeDR03"),
    probe_Pho_chIsoPresel = cms.string("chargedHadronIso"),
    )

TagVariablesToStore = cms.PSet(
    Pho_eta    = cms.string("eta"),
    Pho_abseta = cms.string("abs(eta)"),
    Pho_pt     = cms.string("pt"),
    Pho_et     = cms.string("et"),
    Pho_e      = cms.string("energy"),
    
    ## super cluster quantities
    Pho_sc_energy     = cms.string("superCluster.energy"),
    Pho_sc_et         = cms.string("superCluster.energy*sin(superCluster.position.theta)"),    
    Pho_sc_eta        = cms.string("superCluster.eta"),
    Pho_sc_abseta     = cms.string("abs(superCluster.eta)"),
    Pho_mva           = cms.InputTag("photonFromDiPhotons:idmva"),
    Pho_full5x5_r9    = cms.string("full5x5_r9"),
    Pho_r9            = cms.string("old_r9"),
    Pho_sieie         = cms.string("full5x5_sigmaIetaIeta"),
    Pho_s4            = cms.string("s4"),
    Pho_sigmaEoE      = cms.string("sigEOverE"),
    Pho_etawidth      = cms.string("superCluster.etaWidth"),
    Pho_phiwidth      = cms.string("superCluster.phiWidth"),
    Pho_hoe           = cms.string("hadronicOverEm"),
    )

CommonStuffForPhotonProbe = cms.PSet(
    variables               = cms.PSet(ProbeVariablesToStore),
    ignoreExceptions        = cms.bool (True),
    addRunLumiInfo          = cms.bool (True),
    pileupInfoTag           = cms.InputTag("slimmedAddPileupInfo"),
    addEventVariablesInfo   = cms.bool(True),
    vertexCollection        = cms.InputTag("offlineSlimmedPrimaryVertices"),
    beamSpot                = cms.InputTag("offlineBeamSpot"),
    addCaloMet              = cms.bool(False),
    pfMet                   = cms.InputTag("patCaloMet"),
    #pfMet                   = cms.InputTag("slimmedMETsPuppi"),
    rho                     = cms.InputTag("fixedGridRhoAll"),
    pairVariables           = cms.PSet(ZVariablesToStore),
    pairFlags               = cms.PSet(
        mass60to120 = cms.string("60 < mass < 120")
        ),
    tagVariables            = cms.PSet(TagVariablesToStore),
    tagFlags                = cms.PSet(),    
    )

mcTruthCommonStuff = cms.PSet(
    isMC                    = cms.bool(True),
    #tagMatches              = cms.InputTag("McMatchTag"),
    motherPdgId             = cms.vint32(), # Z
    #motherPdgId             = cms.vint32(22,23), # Z
    #motherPdgId             = cms.vint32(443), # JPsi
    #motherPdgId             = cms.vint32(553), # Yupsilon
    makeMCUnbiasTree        = cms.bool(False),
    #checkMotherInUnbiasEff  = cms.bool(False),  
    genParticles            = cms.InputTag("flashggPrunedGenParticles"),
    useTauDecays            = cms.bool(False),
    checkCharge             = cms.bool(False),
    pdgId                   = cms.int32(11),
    mcVariables = cms.PSet(
        probe_eta    = cms.string("eta"),
        probe_abseta = cms.string("abs(eta)"),
        probe_et     = cms.string("et"),
        probe_e      = cms.string("energy"),
        ),
    mcFlags = cms.PSet(
        probe_flag = cms.string("pt>0")
        ),      
    )
