dipho_variables=[
    "dipho_sumpt                  := diPhoton.sumPt",
    "dipho_mass                   := diPhoton.mass",
    "dipho_leadPt                 := diPhoton.leadingPhoton.pt",
    "dipho_leadEta                := diPhoton.leadingPhoton.eta",
    "dipho_leadPhi                := diPhoton.leadingPhoton.phi",
    "dipho_leadE                  := diPhoton.leadingPhoton.energy",
    "dipho_lead_sieie             := diPhoton.leadingPhoton.sigmaIetaIeta",
    "dipho_lead_hoe               := diPhoton.leadingPhoton.hadronicOverEm",
    "dipho_lead_sigmaEoE          := diPhoton.leadingPhoton.sigEOverE",
    "dipho_lead_ptoM              := diPhoton.leadingPhoton.pt/diPhoton.mass",
    "dipho_leadR9                 := diPhoton.leadingPhoton.full5x5_r9",
    "dipho_leadIDMVA              := diPhoton.leadingView.phoIdMvaWrtChosenVtx",
    "dipho_lead_elveto            := diPhoton.leadingPhoton.passElectronVeto",
    "dipho_leadhasPixelSeed       := diPhoton.leadingPhoton.hasPixelSeed",
    "dipho_lead_prompt            := diPhoton.leadingPhoton.genMatchType",
    "dipho_subleadPt              := diPhoton.subLeadingPhoton.pt",
    "dipho_subleadEta             := diPhoton.subLeadingPhoton.eta",
    "dipho_subleadPhi             := diPhoton.subLeadingPhoton.phi",
    "dipho_subleadE               := diPhoton.subLeadingPhoton.energy",
    "dipho_sublead_sieie          := diPhoton.subLeadingPhoton.sigmaIetaIeta",
    "dipho_sublead_hoe            := diPhoton.subLeadingPhoton.hadronicOverEm",
    "dipho_sublead_sigmaEoE       := diPhoton.subLeadingPhoton.sigEOverE",
    "dipho_sublead_ptoM           := diPhoton.subLeadingPhoton.pt/diPhoton.mass",
    "dipho_subleadR9              := diPhoton.subLeadingPhoton.full5x5_r9",
    "dipho_subleadIDMVA           := diPhoton.subLeadingView.phoIdMvaWrtChosenVtx",
    "dipho_sublead_elveto         := diPhoton.subLeadingPhoton.passElectronVeto",
    "dipho_subleadhasPixelSeed    := diPhoton.subLeadingPhoton.hasPixelSeed",
    "dipho_sublead_prompt         := diPhoton.subLeadingPhoton.genMatchType",
    "dipho_mva                    := diPhotonMVA.result",
    "dipho_PToM                   := diPhoton.pt/diPhoton.mass",
    "sigmarv                      := diPhotonMVA.sigmarv",
    "sigmarvDecorr                := diPhotonMVA.decorrSigmarv",
    "sigmawv                      := diPhotonMVA.sigmawv",
    "CosPhi                       := diPhotonMVA.CosPhi",
    "vtxprob                      := diPhotonMVA.vtxprob",
    "pt                           := diPhoton.pt",
    "leadSCeta                    := diPhoton.leadingPhoton.superCluster.eta",
    "subleadSCeta                 := diPhoton.subLeadingPhoton.superCluster.eta"
]

ele_variables=[
    "n_ele      := electrons.size",
    "ele1_Pt    := ?(electrons.size>0)? electrons.at(0).pt               : -999",
    "ele1_Eta   := ?(electrons.size>0)? electrons.at(0).eta              : -999",
    "ele1_Phi   := ?(electrons.size>0)? electrons.at(0).phi              : -999",
    "ele1_E     := ?(electrons.size>0)? electrons.at(0).energy           : -999",
    "ele1_EtaSC := ?(electrons.size>0)? electrons.at(0).superCluster.eta : -999",
    "ele1_PhiSC := ?(electrons.size>0)? electrons.at(0).superCluster.phi : -999",
    "ele2_Pt    := ?(electrons.size>1)? electrons.at(1).pt               : -999",
    "ele2_Eta   := ?(electrons.size>1)? electrons.at(1).eta              : -999",
    "ele2_Phi   := ?(electrons.size>1)? electrons.at(1).phi              : -999",
    "ele2_E     := ?(electrons.size>1)? electrons.at(1).energy           : -999",
    "ele2_EtaSC := ?(electrons.size>1)? electrons.at(1).superCluster.eta : -999",
    "ele2_PhiSC := ?(electrons.size>1)? electrons.at(1).superCluster.phi : -999"
]

mu_variables=[
    "n_mu       := muons.size",
    "mu1_Pt     := ?(muons.size>0)? muons.at(0).pt     : -999",
    "mu1_Eta    := ?(muons.size>0)? muons.at(0).eta    : -999",
    "mu1_Phi    := ?(muons.size>0)? muons.at(0).phi    : -999",
    "mu1_E      := ?(muons.size>0)? muons.at(0).energy : -999",
    "mu2_Pt     := ?(muons.size>1)? muons.at(1).pt     : -999",
    "mu2_Eta    := ?(muons.size>1)? muons.at(1).eta    : -999",
    "mu2_Phi    := ?(muons.size>1)? muons.at(1).phi    : -999",
    "mu2_E      := ?(muons.size>1)? muons.at(1).energy : -999"
]

jet_variables=[
    "n_jets          := jets.size",
    "jet1_Pt         := ?(jets.size>0)? jets.at(0).pt     : -999",
    "jet1_Eta        := ?(jets.size>0)? jets.at(0).eta    : -999",
    "jet1_Phi        := ?(jets.size>0)? jets.at(0).phi    : -999",
    "jet1_E          := ?(jets.size>0)? jets.at(0).energy : -999",
    "jet1_deepbtag   := ?(jets.size>0)? jets.at(0).bDiscriminator('pfDeepCSVJetTags:probb') + jets.at(0).bDiscriminator('pfDeepCSVJetTags:probbb') : -999",
    "jet2_Pt         := ?(jets.size>1)? jets.at(1).pt     : -999",
    "jet2_Eta        := ?(jets.size>1)? jets.at(1).eta    : -999",
    "jet2_Phi        := ?(jets.size>1)? jets.at(1).phi    : -999",
    "jet2_E          := ?(jets.size>1)? jets.at(1).energy : -999",
    "jet2_deepbtag   := ?(jets.size>1)? jets.at(1).bDiscriminator('pfDeepCSVJetTags:probb') + jets.at(1).bDiscriminator('pfDeepCSVJetTags:probbb') : -999"
]

met_variables=[
    "met_Pt  := met.corPt()",
    "met_Phi := met.corPhi()",
    "met_Sig := met.mEtSig()",
    "met_RealSig := met.significance()"
]

#extra_variables=[
#    "minDeltaPhiJetMet := MinDeltaPhiJetMet()"
#]

gen_variables=[
    "hasZ    := tagTruth().associatedZ()",
    "hasW    := tagTruth().associatedW()",
    "VhasDau := tagTruth().VhasDaughters()",
    "VhasNeu := tagTruth().VhasNeutrinos()",
    "VhasLep := tagTruth().VhasLeptons()",
    "VhasHad := tagTruth().VhasHadrons()",
    "Vpt     := tagTruth().Vpt()"
]
