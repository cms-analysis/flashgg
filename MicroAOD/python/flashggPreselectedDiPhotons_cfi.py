import FWCore.ParameterSet.Config as cms

phoEffArea=cms.PSet( var=cms.string("abs(superCluster.eta)"), bins=cms.vdouble(0.,0.9,1.5,2,2.2,3), vals=cms.vdouble(0.16544,0.16544,0.13212,0.13212,0.13212) )

rediscoveryHLTvariables = cms.vstring(
    "pfPhoIso03", 
    "trkSumPtHollowConeDR03",
    "full5x5_sigmaIetaIeta",
    "full5x5_r9"
    )

#cuts to mimic category trigger cuts
rediscoveryHLTcutsV1 = cms.VPSet(
    cms.PSet(cut=cms.string("isEB && full5x5_r9>0.85"), ##EB high R9
             selection = cms.VPSet(
            cms.PSet(max=cms.string("100000.0"), 
                     rhocorr=phoEffArea,
                     ),
            cms.PSet(max=cms.string("100000.0")),
            cms.PSet(max=cms.string("100000.0")),
            cms.PSet(min=cms.string("0.5"))
            ),
             ),
    
    cms.PSet(cut=cms.string("isEE && full5x5_r9>0.90"),  ##EE high R9
             selection = cms.VPSet(
            cms.PSet(max=cms.string("100000.0"), 
                     rhocorr=phoEffArea,
                     ),
            cms.PSet(max=cms.string("100000.0")),
            cms.PSet(max=cms.string("100000.0")),
            cms.PSet(min=cms.string("0.8"))
            ),
             ),
    cms.PSet(cut=cms.string("isEB && full5x5_r9<=0.85"),  #EB low R9
             selection = cms.VPSet(
            cms.PSet(max=cms.string("4.0"), 
                     rhocorr=phoEffArea,
                     ),
            cms.PSet(max=cms.string("6.0")),
            cms.PSet(max=cms.string("0.015")),
            cms.PSet(min=cms.string("0.5"))
            ),       
             ),       
    cms.PSet(cut=cms.string("isEE && full5x5_r9<=0.90"),  ##EE low R9
             selection = cms.VPSet(
            cms.PSet(max=cms.string("4.0"), 
                     rhocorr=phoEffArea,
                     ),
            cms.PSet(max=cms.string("6.0")),
            cms.PSet(max=cms.string("0.015")),
            cms.PSet(min=cms.string("0.8"))
            ),
             )
    )

#cuts here mimic the miniAOD photon cuts and the non-category based trigger cuts
flashggPreselectedDiPhotons = cms.EDFilter(
    "GenericDiPhotonCandidateSelector",
    src = cms.InputTag("flashggDiPhotons"),
    rho = cms.InputTag("fixedGridRhoAll"),
    cut = cms.string(
        "    (leadingPhoton.full5x5_r9>0.8||leadingPhoton.egChargedHadronIso<20||leadingPhoton.egChargedHadronIso/leadingPhoton.pt<0.3)"
        " && (subLeadingPhoton.full5x5_r9>0.8||subLeadingPhoton.egChargedHadronIso<20||subLeadingPhoton.egChargedHadronIso/subLeadingPhoton.pt<0.3)"
        " && (leadingPhoton.hadronicOverEm < 0.08 && subLeadingPhoton.hadronicOverEm < 0.08)"
        " && (leadingPhoton.pt >30.0 && subLeadingPhoton.pt > 20.0)"
        " && (abs(leadingPhoton.superCluster.eta) < 2.5 && abs(subLeadingPhoton.superCluster.eta) < 2.5)"
        ),
    variables = rediscoveryHLTvariables,
    categories = rediscoveryHLTcutsV1
    )
