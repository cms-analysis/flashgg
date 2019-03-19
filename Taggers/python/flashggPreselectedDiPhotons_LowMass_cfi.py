##--inherited from the low mass Hgg analysis

import FWCore.ParameterSet.Config as cms

phoEffArea=cms.PSet( var=cms.string("abs(superCluster.eta)"), bins=cms.vdouble(0.,0.9,1.5,2,2.2,3), vals=cms.vdouble(0.16544,0.16544,0.13212,0.13212,0.13212) )

rediscoveryHLTvariables = cms.vstring(
    "pfPhoIso03",
    "trkSumPtHollowConeDR03",
    "full5x5_sigmaIetaIeta",
    "full5x5_r9",
 #   "passElectronVeto"
     "hasPixelSeed"
   )

#cuts to mimic category trigger cuts
rediscoveryHLTcutsV1 = cms.VPSet(
    cms.PSet(cut=cms.string("isEB && full5x5_r9>0.85"), ##EB high R9
             selection = cms.VPSet(
            cms.PSet(max=cms.string("4.0"),
                     rhocorr=phoEffArea,
                     ),
            cms.PSet(max=cms.string("6.0")),
            cms.PSet(max=cms.string("0.0105")),
            cms.PSet(min=cms.string("0.5")),
            cms.PSet(max=cms.string("0.5"))
            ),
             ),

    cms.PSet(cut=cms.string("isEE && full5x5_r9>0.90"),  ##EE high R9
             selection = cms.VPSet(
            cms.PSet(max=cms.string("4.0"),
                     rhocorr=phoEffArea,
                     ),
            cms.PSet(max=cms.string("6.0")),
            cms.PSet(max=cms.string("0.0275")),
            cms.PSet(min=cms.string("0.8")),
            cms.PSet(max=cms.string("0.5"))
            ),
             ),
    cms.PSet(cut=cms.string("isEB && full5x5_r9<=0.85"),  #EB low R9
             selection = cms.VPSet(
            cms.PSet(max=cms.string("4.0"),
                     rhocorr=phoEffArea,
                     ),
            cms.PSet(max=cms.string("6.0")),
            cms.PSet(max=cms.string("0.0105")),
            cms.PSet(min=cms.string("0.5")),
            cms.PSet(max=cms.string("0.5"))
            ),
             ),
    cms.PSet(cut=cms.string("isEE && full5x5_r9<=0.90"),  ##EE low R9, in fact no low R9
             selection = cms.VPSet(
            cms.PSet(max=cms.string("0.0"),
                     rhocorr=phoEffArea,
                     ),
            cms.PSet(max=cms.string("0.0")),
            cms.PSet(max=cms.string("0.0")),
            cms.PSet(min=cms.string("0.9")), #make sure again
            cms.PSet(max=cms.string("0.5"))
            ),
             )
    )

#cuts here mimic the miniAOD photon cuts and the non-category based trigger cuts
#Also included: the super-loose ID MVA cuts
flashggPreselectedDiPhotonsLowMass = cms.EDFilter(
    "GenericDiPhotonCandidateSelector",
    src = cms.InputTag("flashggUpdatedIdMVADiPhotons"),
    rho = cms.InputTag("fixedGridRhoAll"),
    cut = cms.string(
        "    (leadingPhoton.full5x5_r9>0.8||leadingPhoton.egChargedHadronIso<20||leadingPhoton.egChargedHadronIso/leadingPhoton.pt<0.3)"
        " && (subLeadingPhoton.full5x5_r9>0.8||subLeadingPhoton.egChargedHadronIso<20||subLeadingPhoton.egChargedHadronIso/subLeadingPhoton.pt<0.3)"
       " && (leadingPhoton.hadronicOverEm < 0.08 && subLeadingPhoton.hadronicOverEm < 0.08)"
        " && ( ((abs(leadingPhoton.superCluster.eta) < 1.4442 && leadingPhoton.hadronicOverEm < 0.07)||(abs(leadingPhoton.superCluster.eta) > 1.566 && leadingPhoton.hadronicOverEm < 0.035)) && ((abs(subLeadingPhoton.superCluster.eta) < 1.4442 &&  subLeadingPhoton.hadronicOverEm < 0.07)||(abs(subLeadingPhoton.superCluster.eta) > 1.566 && subLeadingPhoton.hadronicOverEm < 0.035) ) )" #tighter H/E
        " && ( ((abs(leadingPhoton.superCluster.eta) < 1.4442) ||(abs(leadingPhoton.superCluster.eta) > 1.566))&& ((abs(subLeadingPhoton.superCluster.eta) < 1.4442 )||(abs(subLeadingPhoton.superCluster.eta) > 1.566 ) ) )" #no tighter H/E. FIXME!!!
        " && (leadingPhoton.pt > 30.0 && subLeadingPhoton.pt > 20.0)"  # pTs
        " && (abs(leadingPhoton.superCluster.eta) < 2.5 && abs(subLeadingPhoton.superCluster.eta) < 2.5)"
        " && (abs(leadingPhoton.superCluster.eta) < 1.4442 || abs(leadingPhoton.superCluster.eta) > 1.566 )"
        " && (abs(subLeadingPhoton.superCluster.eta) < 1.4442 || abs(subLeadingPhoton.superCluster.eta) > 1.566)"
        " && ( (abs(leadingPhoton.superCluster.eta) < 1.4442 && abs(subLeadingPhoton.superCluster.eta) < 1.4442)" #EB-EB
        "   || (abs(leadingPhoton.superCluster.eta) < 1.4442 && leadingPhoton.full5x5_r9>0.85 && abs(subLeadingPhoton.superCluster.eta) > 1.566 && subLeadingPhoton.full5x5_r9>0.90 ) "  #EB-EE with R9
        "   || (abs(leadingPhoton.superCluster.eta) > 1.566 && leadingPhoton.full5x5_r9>0.90 && abs(subLeadingPhoton.superCluster.eta) < 1.4442 && subLeadingPhoton.full5x5_r9>0.85 )"   #EE-EB with R9
        "   || (abs(leadingPhoton.superCluster.eta) > 1.566 && leadingPhoton.full5x5_r9>0.90 && abs(subLeadingPhoton.superCluster.eta) > 1.566 && subLeadingPhoton.full5x5_r9>0.90 ) )" #EE-EE with R9
                     " && mass > 55" ##### Change mass cut if needed
                     " && (leadingPhoton.pt > 0.47*mass && subLeadingPhoton.pt > 0.28*mass)"  #Scaled pTs
                     " && (!leadingPhoton.hasPixelSeed) && (!subLeadingPhoton.hasPixelSeed)"  #E-veto
                     " && (leadPhotonId > -0.9 && subLeadPhotonId > -0.9)"  #Photon ID MVA
       # " && (leadingPhoton.passElectronVeto) && (subLeadingPhoton.passElectronVeto)"
        ),
    variables = rediscoveryHLTvariables,
    categories = rediscoveryHLTcutsV1
    )
