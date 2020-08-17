import FWCore.ParameterSet.Config as cms

eleHltSafeEffAreas=cms.PSet( var=cms.string("abs(superCluster.eta)"), 
                   bins=cms.vdouble(0.,    1,     1.479, 2,     2.2,   2.3,   2.4,   3), 
                   vals=cms.vdouble(0.1752,0.1862,0.1411,0.1534,0.1903,0.2243,0.2687) )

eleHltSafeVariables = cms.vstring(
    "full5x5_sigmaIetaIeta",
    "abs(deltaEtaSeedClusterTrackAtCalo)",
    "abs(deltaPhiSuperClusterTrackAtVtx)",
    "abs(1. - eSuperClusterOverP)/ecalEnergy",
    "pfIsolationVariables.sumNeutralHadronEt + pfIsolationVariables.sumPhotonEt",
    "hcalOverEcal",
    "gsfTrack.hitPattern.numberOfHits('MISSING_INNER_HITS')"
        )

eleHltSafeCategories = cms.VPSet(
        cms.PSet(cut=cms.string("abs(superCluster.eta)<1.5"),
                 selection = cms.VPSet(
            cms.PSet(min=cms.string("0.001"), max=cms.string("1.1e-2")), # sigmaIetaIeta
            cms.PSet(max=cms.string("0.004")), # DeltaEta
            cms.PSet(max=cms.string("0.02")),  # DEltaPhi
            cms.PSet(max=cms.string("0.013")), # EoverP
            cms.PSet(max=cms.string("0.1*pt - pfIsolationVariables.sumChargedHadronPt"), # isolation
                     rhocorr=eleHltSafeEffAreas,
                     capped=cms.double(0.)
                     ),
            cms.PSet(max=cms.string("6.e-2")), # H/E
            cms.PSet(), # missing hits
            )
                 ),
        cms.PSet(cut=cms.string("abs(superCluster.eta)>1.5"),
                 selection = cms.VPSet(
            cms.PSet(min=cms.string("0.001"), max=cms.string("3.1e-2")), # sigmaIetaIeta
            cms.PSet(max=cms.string("0.007")), # DeltaEta
            cms.PSet(max=cms.string("0.02")),  # DEltaPhi
            cms.PSet(max=cms.string("0.013")), # EoverP
            cms.PSet(max=cms.string("0.1*pt - pfIsolationVariables.sumChargedHadronPt"), # isolation
                     rhocorr=eleHltSafeEffAreas,
                     capped=cms.double(0.)
                     ),
            cms.PSet(max=cms.string("6.5e-2")), # H/E
            cms.PSet(max=cms.string("3.5")), # missing hits
            )
                 ),                 
        )


flashggHltSafeElectrons = cms.EDFilter(
    "GenericElectronSelector",
    src = cms.InputTag("flashggElectrons"),
    rho = cms.InputTag("fixedGridRhoFastjetAll"),
    cut = cms.string("abs(superCluster.eta)<2.5 && ( abs(superCluster.eta) < 1.4442 || abs(superCluster.eta)>1.566  )"),
    variables = eleHltSafeVariables,
    categories = eleHltSafeCategories,
    )
