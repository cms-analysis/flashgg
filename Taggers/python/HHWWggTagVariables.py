# Abe Tishelman-Charny
# 27 March 2019 
# 
# HH->WWgg Variables to plot in output Event Dumper 
# Can call created functions here to get variables 
# For example: HHWWggCandidate.cc/HHWWggCandidate.h defines vector returning function:
# std::vector<float> HHWWggCandidate::CosThetaAngles()
# std::vector<float> CosThetaAngles() const;
# In this file, you can call it with: 
# CosThetaAngles()[1]   

# Fit_and_Reco = {

# }

Reco_Variables = [

    # Leading Photon 
    # lp = Leading Photon 
    "lp_pt                               := Leading_Photon.p4().pt()",
    "lp_eta                              := Leading_Photon.p4().eta()",
    "lp_SC_eta                           := Leading_Photon.superCluster().eta()",
    "lp_phi                              := Leading_Photon.p4().phi()",
    "lp_E                                := Leading_Photon.p4().E()",
    "lp_initE                            := Leading_Photon.energyAtStep('initial')",
    "lp_r9                               := Leading_Photon.old_r9()",
    "lp_full5x5_r9                       := Leading_Photon.full5x5_r9()",
    "lp_Hgg_MVA                          := lp_Hgg_MVA()",
    "lp_passElectronVeto                 := Leading_Photon.passElectronVeto()",
    "lp_hasPixelSeed                     := Leading_Photon.hasPixelSeed",

    # Subleading Photon
    # slp = Subleading Photon 
    "slp_pt                              := Subleading_Photon.p4().pt()",
    "slp_eta                             := Subleading_Photon.p4().eta()",
    "slp_SC_eta                          := Subleading_Photon.superCluster().eta()",
    "slp_phi                             := Subleading_Photon.p4().phi()",
    "slp_E                               := Subleading_Photon.p4().E()",
    "slp_initE                           := Subleading_Photon.energyAtStep('initial')",
    "slp_r9                              := Subleading_Photon.old_r9()",
    "slp_full5x5_r9                      := Subleading_Photon.full5x5_r9()",
    "slp_Hgg_MVA                         := slp_Hgg_MVA()",
    "slp_passElectronVeto                := Subleading_Photon.passElectronVeto()",
    "slp_hasPixelSeed                    := Subleading_Photon.hasPixelSeed",   

    # DiPhoton(s)
    "n_dipho                             := diphoVector.size()",
    "dipho_MVA                           := dipho_MVA()", 
    "CMS_hgg_mass                        := CMS_hgg_mass() ", # for cuts within HHWWggCandidate.cc before workspace 
    "leading_dpho_pt                     := ? leading_dpho.pt() != 0 ? leading_dpho.pt() : -99",
    "leading_dpho_eta                    := ? leading_dpho.eta() != 0 ? leading_dpho.eta() : -99",
    "leading_dpho_phi                    := ? leading_dpho.phi() != 0 ? leading_dpho.phi() : -99",
    "leading_dpho_E                      := ? leading_dpho.E() != 0 ? leading_dpho.E() : -99",

    # Electrons
    # If there is no leading electron (electronVector_.size() == 0) or no subleading electron (electronVector_.size() <= 1) plot -99 
    "leading_elec_pt                     := ? leading_elec.pt() != 0 ? leading_elec.pt() : -99 ",  
    "leading_elec_eta                    := ? leading_elec.eta() != 0 ? leading_elec.eta() : -99 ",
    "leading_elec_phi                    := ? leading_elec.phi() != 0 ? leading_elec.phi() : -99 ",
    "leading_elec_E                      := ? leading_elec.E() != 0 ? leading_elec.E() : -99 ",
    "subleading_elec_pt                  := ? subleading_elec.pt() != 0 ? subleading_elec.pt() : -99 ",
    "subleading_elec_eta                 := ? subleading_elec.eta() != 0 ? subleading_elec.eta() : -99 ",
    "subleading_elec_phi                 := ? subleading_elec.phi() != 0 ? subleading_elec.phi() : -99 ",
    "subleading_elec_E                   := ? subleading_elec.E() != 0 ? subleading_elec.E() : -99 ",

    # Muons 
    # If there is no leading muon (muonVector_.size() == 0) or no subleading muon (muonVector_.size() <= 1) plot -99 
    "leading_muon_pt                     := ? leading_muon.pt() != 0 ? leading_muon.pt() : -99 ",
    "leading_muon_eta                    := ? leading_muon.eta() != 0 ? leading_muon.eta() : -99 ",
    "leading_muon_phi                    := ? leading_muon.phi() != 0 ? leading_muon.phi() : -99 ",
    "leading_muon_E                      := ? leading_muon.E() != 0 ? leading_muon.E() : -99 ",
    "subleading_muon_pt                  := ? subleading_muon.pt() != 0 ? subleading_muon.pt() : -99 ",
    "subleading_muon_eta                 := ? subleading_muon.eta() != 0 ? subleading_muon.eta() : -99 ",
    "subleading_muon_phi                 := ? subleading_muon.phi() != 0 ? subleading_muon.phi() : -99 ",
    "subleading_muon_E                   := ? subleading_muon.E() != 0 ? subleading_muon.E() : -99 ",

    # Jets 
    "n_jets                              := JetVector.size()", 
        # Using GEN information
        # "mdj_invmass                     := MatchingDiJet.mass()",  
        # "nmdj_invmass                    := NonMatchingDiJet.mass()", 

        # Not using GEN information 
        # JetVector is ordered by pT 
        "jet0_pt                         := ? JetVector.size() >= 1 ? JetVector[0].pt() : -99 ",
        "jet0_eta                        := ? JetVector.size() >= 1 ? JetVector[0].eta() : -99 ",
        "jet0_phi                        := ? JetVector.size() >= 1 ? JetVector[0].phi() : -99 ",
        "jet0_E                          := ? JetVector.size() >= 1 ? JetVector[0].p4().E() : -99 ",
        "jet0_PUID                       := ? JetVector.size() >= 1 ? JetVector[0].puJetIdMVA() : -99 ", 
        "jet0_btag                       := ? JetVector.size() >= 1 ? JetVector[0].bDiscriminator('mini_pfDeepFlavourJetTags:probb') : -99 ",

        "jet1_pt                         := ? JetVector.size() >= 2 ? JetVector[1].pt() : -99 ",
        "jet1_eta                        := ? JetVector.size() >= 2 ? JetVector[1].eta() : -99 ",
        "jet1_phi                        := ? JetVector.size() >= 2 ? JetVector[1].phi() : -99 ",
        "jet1_E                          := ? JetVector.size() >= 2 ? JetVector[1].p4().E() : -99 ",
        "jet1_PUID                       := ? JetVector.size() >= 2 ? JetVector[1].puJetIdMVA() : -99 ",
        "jet1_btag                       := ? JetVector.size() >= 2 ? JetVector[1].bDiscriminator('mini_pfDeepFlavourJetTags:probb') : -99 ",

        "jet2_pt                         := ? JetVector.size() >= 3 ? JetVector[2].pt() : -99 ",
        "jet2_eta                        := ? JetVector.size() >= 3 ? JetVector[2].eta() : -99 ",
        "jet2_phi                        := ? JetVector.size() >= 3 ? JetVector[2].phi() : -99 ",
        "jet2_E                          := ? JetVector.size() >= 3 ? JetVector[2].p4().E() : -99 ",
        "jet2_PUID                       := ? JetVector.size() >= 3 ? JetVector[2].puJetIdMVA() : -99 ",
        "jet2_btag                       := ? JetVector.size() >= 3 ? JetVector[2].bDiscriminator('mini_pfDeepFlavourJetTags:probb') : -99 ",

        "jet3_pt                         := ? JetVector.size() >= 4 ? JetVector[3].pt() : -99 ",
        "jet3_eta                        := ? JetVector.size() >= 4 ? JetVector[3].eta() : -99 ",
        "jet3_phi                        := ? JetVector.size() >= 4 ? JetVector[3].phi() : -99 ",
        "jet3_E                          := ? JetVector.size() >= 4 ? JetVector[3].p4().E() : -99 ",
        "jet3_PUID                       := ? JetVector.size() >= 4 ? JetVector[3].puJetIdMVA() : -99 ",
        "jet3_btag                       := ? JetVector.size() >= 4 ? JetVector[3].bDiscriminator('mini_pfDeepFlavourJetTags:probb') : -99 ",

        "jet4_pt                         := ? JetVector.size() >= 5 ? JetVector[4].pt() : -99 ",
        "jet4_eta                        := ? JetVector.size() >= 5 ? JetVector[4].eta() : -99 ",
        "jet4_phi                        := ? JetVector.size() >= 5 ? JetVector[4].phi() : -99 ",
        "jet4_E                          := ? JetVector.size() >= 5 ? JetVector[4].p4().E() : -99 ",
        "jet4_PUID                       := ? JetVector.size() >= 5 ? JetVector[4].puJetIdMVA() : -99 ",
        "jet4_btag                       := ? JetVector.size() >= 5 ? JetVector[4].bDiscriminator('mini_pfDeepFlavourJetTags:probb') : -99 ",

        "jet5_pt                         := ? JetVector.size() >= 6 ? JetVector[5].pt() : -99 ",
        "jet5_eta                        := ? JetVector.size() >= 6 ? JetVector[5].eta() : -99 ",
        "jet5_phi                        := ? JetVector.size() >= 6 ? JetVector[5].phi() : -99 ",
        "jet5_E                          := ? JetVector.size() >= 6 ? JetVector[5].p4().E() : -99 ",
        "jet5_PUID                       := ? JetVector.size() >= 6 ? JetVector[5].puJetIdMVA() : -99 ",
        "jet5_btag                       := ? JetVector.size() >= 6 ? JetVector[5].bDiscriminator('mini_pfDeepFlavourJetTags:probb') : -99 ",

        "jet6_pt                         := ? JetVector.size() >= 7 ? JetVector[6].pt() : -99 ",
        "jet6_eta                        := ? JetVector.size() >= 7 ? JetVector[6].eta() : -99 ",
        "jet6_phi                        := ? JetVector.size() >= 7 ? JetVector[6].phi() : -99 ",
        "jet6_E                          := ? JetVector.size() >= 7 ? JetVector[6].p4().E() : -99 ",
        "jet6_PUID                       := ? JetVector.size() >= 7 ? JetVector[6].puJetIdMVA() : -99 ",
        "jet6_btag                       := ? JetVector.size() >= 7 ? JetVector[6].bDiscriminator('mini_pfDeepFlavourJetTags:probb') : -99 ",

        "jet7_pt                         := ? JetVector.size() >= 8 ? JetVector[7].pt() : -99 ",
        "jet7_eta                        := ? JetVector.size() >= 8 ? JetVector[7].eta() : -99 ",
        "jet7_phi                        := ? JetVector.size() >= 8 ? JetVector[7].phi() : -99 ",
        "jet7_E                          := ? JetVector.size() >= 8 ? JetVector[7].p4().E() : -99 ",
        "jet7_PUID                       := ? JetVector.size() >= 8 ? JetVector[7].puJetIdMVA() : -99 ",
        "jet7_btag                       := ? JetVector.size() >= 8 ? JetVector[7].bDiscriminator('mini_pfDeepFlavourJetTags:probb') : -99 ",

        "jet8_pt                         := ? JetVector.size() >= 9 ? JetVector[8].pt() : -99 ",
        "jet8_eta                        := ? JetVector.size() >= 9 ? JetVector[8].eta() : -99 ",
        "jet8_phi                        := ? JetVector.size() >= 9 ? JetVector[8].phi() : -99 ",
        "jet8_E                          := ? JetVector.size() >= 9 ? JetVector[8].p4().E() : -99 ",
        "jet8_PUID                       := ? JetVector.size() >= 9 ? JetVector[8].puJetIdMVA() : -99 ",
        "jet8_btag                       := ? JetVector.size() >= 9 ? JetVector[8].bDiscriminator('mini_pfDeepFlavourJetTags:probb') : -99 ",

        "jet9_pt                         := ? JetVector.size() >= 10 ? JetVector[9].pt() : -99 ",
        "jet9_eta                        := ? JetVector.size() >= 10 ? JetVector[9].eta() : -99 ",
        "jet9_phi                        := ? JetVector.size() >= 10 ? JetVector[9].phi() : -99 ",
        "jet9_E                          := ? JetVector.size() >= 10 ? JetVector[9].p4().E() : -99 ",
        "jet9_PUID                       := ? JetVector.size() >= 10 ? JetVector[9].puJetIdMVA() : -99 ",
        "jet9_btag                       := ? JetVector.size() >= 10 ? JetVector[9].bDiscriminator('mini_pfDeepFlavourJetTags:probb') : -99 ",

        # "lsl_dij_mass                    := ? lsl_dij.mass() !=0 ? lsl_dij.mass() : -99 ", # dijet made from leading and subleading jets 

    # MET 
    "theMETcorpt                         := ? theMETcorpt !=0 ? theMETcorpt : -99",
    "MET_pt                              := ? MET_fourvec.pt() !=0 ? MET_fourvec.pt() : -99",
    "MET_eta                             := ? MET_fourvec.eta() !=0 ? MET_fourvec.eta() : -99",
    "MET_phi                             := ? MET_fourvec.phi() !=0 ? MET_fourvec.phi() : -99",
    "MET_px                              := ? MET_fourvec.px() !=0 ? MET_fourvec.px() : -99",
    "MET_py                              := ? MET_fourvec.py() !=0 ? MET_fourvec.py() : -99",
    "MET_pz                              := ? MET_fourvec.pz() !=0 ? MET_fourvec.pz() : -99",
    "MET_E                               := ? MET_fourvec.E() !=0 ? MET_fourvec.E() : -99",

    "d_n_good_leptons                    := Cut_Variables[0]",
    "d_n_good_electrons                  := Cut_Variables[1]",
    "d_n_good_muons                      := Cut_Variables[2]",
    "d_n_good_jets                       := Cut_Variables[3]", 

    "dZ                                   := dZ()",


]

Vertex_Variables = [
    "Gen_Vertex_Z                         := Vertex_Variables[0]",
    "Zero_Vertex_Z                        := Vertex_Variables[1]",
    # "dZ                                   := Vertex_Variables[2]",
    "dZ                                   := dZ()",
    "num_vertices                         := Vertex_Variables[3]",
    # "dZ_Hgg_Vertex                        := Vertex_Variables[4]",
    # "Hgg_Vertex_Z                         := Vertex_Variables[1]",
    # "dZ_Gen_Hgg                           := Vertex_Variables[3]",
    # "Hgg_Vtx_isZero                       := Vertex_Variables[4]",  
]

# Misc_Variables = [
#     "d_n_good_leptons                    := Cut_Variables[0]",
#     "d_n_good_electrons                  := Cut_Variables[1]",
#     "d_n_good_muons                      := Cut_Variables[2]",
#     "d_n_good_jets                       := Cut_Variables[3]",    
# ]

Fit_Variables = [
    "dZ                                  := dZ()",
    "CMS_hgg_mass                        := CMS_hgg_mass() ", # for cuts within HHWWggCandidate.cc before workspace 
]

RECO_GEN_Variables = [


    ## RECO 

    # Electrons
    # If there is no leading electron (electronVector_.size() == 0) or no subleading electron (electronVector_.size() <= 1) plot -99 
    "leading_elec_pt                     := ? leading_elec.pt() != 0 ? leading_elec.pt() : -99 ",  
    "leading_elec_eta                    := ? leading_elec.eta() != 0 ? leading_elec.eta() : -99 ",
    "leading_elec_phi                    := ? leading_elec.phi() != 0 ? leading_elec.phi() : -99 ",
    "leading_elec_E                      := ? leading_elec.E() != 0 ? leading_elec.E() : -99 ",
    "subleading_elec_pt                  := ? subleading_elec.pt() != 0 ? subleading_elec.pt() : -99 ",
    "subleading_elec_eta                 := ? subleading_elec.eta() != 0 ? subleading_elec.eta() : -99 ",
    "subleading_elec_phi                 := ? subleading_elec.phi() != 0 ? subleading_elec.phi() : -99 ",
    "subleading_elec_E                   := ? subleading_elec.E() != 0 ? subleading_elec.E() : -99 ",

    # Muons 
    # If there is no leading muon (muonVector_.size() == 0) or no subleading muon (muonVector_.size() <= 1) plot -99 
    "leading_muon_pt                     := ? leading_muon.pt() != 0 ? leading_muon.pt() : -99 ",
    "leading_muon_eta                    := ? leading_muon.eta() != 0 ? leading_muon.eta() : -99 ",
    "leading_muon_phi                    := ? leading_muon.phi() != 0 ? leading_muon.phi() : -99 ",
    "leading_muon_E                      := ? leading_muon.E() != 0 ? leading_muon.E() : -99 ",
    "subleading_muon_pt                  := ? subleading_muon.pt() != 0 ? subleading_muon.pt() : -99 ",
    "subleading_muon_eta                 := ? subleading_muon.eta() != 0 ? subleading_muon.eta() : -99 ",
    "subleading_muon_phi                 := ? subleading_muon.phi() != 0 ? subleading_muon.phi() : -99 ",
    "subleading_muon_E                   := ? subleading_muon.E() != 0 ? subleading_muon.E() : -99 ",

    # # Leading Photon 
    # # lp = Leading Photon 
    # "lp_pt                               := Leading_Photon.p4().pt()",
    # "lp_eta                              := Leading_Photon.p4().eta()",
    # "lp_SC_eta                           := Leading_Photon.superCluster().eta()",
    # "lp_phi                              := Leading_Photon.p4().phi()",
    # "lp_E                                := Leading_Photon.p4().E()",
    # "lp_initE                            := Leading_Photon.energyAtStep('initial')",
    # "lp_r9                               := Leading_Photon.old_r9()",
    # "lp_full5x5_r9                       := Leading_Photon.full5x5_r9()",
    # "lp_Hgg_MVA                          := lp_Hgg_MVA()",
    # "lp_passElectronVeto                 := Leading_Photon.passElectronVeto()",
    # "lp_hasPixelSeed                     := Leading_Photon.hasPixelSeed",

    # # Subleading Photon
    # # slp = Subleading Photon 
    # "slp_pt                              := Subleading_Photon.p4().pt()",
    # "slp_eta                             := Subleading_Photon.p4().eta()",
    # "slp_SC_eta                          := Subleading_Photon.superCluster().eta()",
    # "slp_phi                             := Subleading_Photon.p4().phi()",
    # "slp_E                               := Subleading_Photon.p4().E()",
    # "slp_initE                           := Subleading_Photon.energyAtStep('initial')",
    # "slp_r9                              := Subleading_Photon.old_r9()",
    # "slp_full5x5_r9                      := Subleading_Photon.full5x5_r9()",
    # "slp_Hgg_MVA                         := slp_Hgg_MVA()",
    # "slp_passElectronVeto                := Subleading_Photon.passElectronVeto()",
    # "slp_hasPixelSeed                    := Subleading_Photon.hasPixelSeed",   

    # Jets 
    "n_jets                              := JetVector.size()", 
        # Using GEN information
        # "mdj_invmass                     := MatchingDiJet.mass()",  
        # "nmdj_invmass                    := NonMatchingDiJet.mass()", 

        # Not using GEN information 
        # JetVector is ordered by pT 
        "jet0_pt                         := ? JetVector.size() >= 1 ? JetVector[0].pt() : -99 ",
        "jet0_eta                        := ? JetVector.size() >= 1 ? JetVector[0].eta() : -99 ",
        "jet0_phi                        := ? JetVector.size() >= 1 ? JetVector[0].phi() : -99 ",
        "jet0_E                          := ? JetVector.size() >= 1 ? JetVector[0].p4().E() : -99 ",
        "jet0_PUID                       := ? JetVector.size() >= 1 ? JetVector[0].puJetIdMVA() : -99 ",

        "jet1_pt                         := ? JetVector.size() >= 2 ? JetVector[1].pt() : -99 ",
        "jet1_eta                        := ? JetVector.size() >= 2 ? JetVector[1].eta() : -99 ",
        "jet1_phi                        := ? JetVector.size() >= 2 ? JetVector[1].phi() : -99 ",
        "jet1_E                          := ? JetVector.size() >= 2 ? JetVector[1].p4().E() : -99 ",
        "jet1_PUID                       := ? JetVector.size() >= 2 ? JetVector[1].puJetIdMVA() : -99 ",

        "jet2_pt                         := ? JetVector.size() >= 3 ? JetVector[2].pt() : -99 ",
        "jet2_eta                        := ? JetVector.size() >= 3 ? JetVector[2].eta() : -99 ",
        "jet2_phi                        := ? JetVector.size() >= 3 ? JetVector[2].phi() : -99 ",
        "jet2_E                          := ? JetVector.size() >= 3 ? JetVector[2].p4().E() : -99 ",
        "jet2_PUID                       := ? JetVector.size() >= 3 ? JetVector[2].puJetIdMVA() : -99 ",

        "jet3_pt                         := ? JetVector.size() >= 4 ? JetVector[3].pt() : -99 ",
        "jet3_eta                        := ? JetVector.size() >= 4 ? JetVector[3].eta() : -99 ",
        "jet3_phi                        := ? JetVector.size() >= 4 ? JetVector[3].phi() : -99 ",
        "jet3_E                          := ? JetVector.size() >= 4 ? JetVector[3].p4().E() : -99 ",
        "jet3_PUID                       := ? JetVector.size() >= 4 ? JetVector[3].puJetIdMVA() : -99 ",

        "jet4_pt                         := ? JetVector.size() >= 5 ? JetVector[4].pt() : -99 ",
        "jet4_eta                        := ? JetVector.size() >= 5 ? JetVector[4].eta() : -99 ",
        "jet4_phi                        := ? JetVector.size() >= 5 ? JetVector[4].phi() : -99 ",
        "jet4_E                          := ? JetVector.size() >= 5 ? JetVector[4].p4().E() : -99 ",
        "jet4_PUID                       := ? JetVector.size() >= 5 ? JetVector[4].puJetIdMVA() : -99 ",

        "jet5_pt                         := ? JetVector.size() >= 6 ? JetVector[5].pt() : -99 ",
        "jet5_eta                        := ? JetVector.size() >= 6 ? JetVector[5].eta() : -99 ",
        "jet5_phi                        := ? JetVector.size() >= 6 ? JetVector[5].phi() : -99 ",
        "jet5_E                          := ? JetVector.size() >= 6 ? JetVector[5].p4().E() : -99 ",
        "jet5_PUID                       := ? JetVector.size() >= 6 ? JetVector[5].puJetIdMVA() : -99 ",

        "jet6_pt                         := ? JetVector.size() >= 7 ? JetVector[6].pt() : -99 ",
        "jet6_eta                        := ? JetVector.size() >= 7 ? JetVector[6].eta() : -99 ",
        "jet6_phi                        := ? JetVector.size() >= 7 ? JetVector[6].phi() : -99 ",
        "jet6_E                          := ? JetVector.size() >= 7 ? JetVector[6].p4().E() : -99 ",
        "jet6_PUID                       := ? JetVector.size() >= 7 ? JetVector[6].puJetIdMVA() : -99 ",

        "jet7_pt                         := ? JetVector.size() >= 8 ? JetVector[7].pt() : -99 ",
        "jet7_eta                        := ? JetVector.size() >= 8 ? JetVector[7].eta() : -99 ",
        "jet7_phi                        := ? JetVector.size() >= 8 ? JetVector[7].phi() : -99 ",
        "jet7_E                          := ? JetVector.size() >= 8 ? JetVector[7].p4().E() : -99 ",
        "jet7_PUID                       := ? JetVector.size() >= 8 ? JetVector[7].puJetIdMVA() : -99 ",

        "jet8_pt                         := ? JetVector.size() >= 9 ? JetVector[8].pt() : -99 ",
        "jet8_eta                        := ? JetVector.size() >= 9 ? JetVector[8].eta() : -99 ",
        "jet8_phi                        := ? JetVector.size() >= 9 ? JetVector[8].phi() : -99 ",
        "jet8_E                          := ? JetVector.size() >= 9 ? JetVector[8].p4().E() : -99 ",
        "jet8_PUID                       := ? JetVector.size() >= 9 ? JetVector[8].puJetIdMVA() : -99 ",

        "jet9_pt                         := ? JetVector.size() >= 10 ? JetVector[9].pt() : -99 ",
        "jet9_eta                        := ? JetVector.size() >= 10 ? JetVector[9].eta() : -99 ",
        "jet9_phi                        := ? JetVector.size() >= 10 ? JetVector[9].phi() : -99 ",
        "jet9_E                          := ? JetVector.size() >= 10 ? JetVector[9].p4().E() : -99 ",
        "jet9_PUID                       := ? JetVector.size() >= 10 ? JetVector[9].puJetIdMVA() : -99 ",

    # MET 
    "theMETcorpt                         := ? theMETcorpt !=0 ? theMETcorpt : -99",
    "MET_pt                              := ? MET_fourvec.pt() !=0 ? MET_fourvec.pt() : -99",
    "MET_eta                             := ? MET_fourvec.eta() !=0 ? MET_fourvec.eta() : -99",
    "MET_phi                             := ? MET_fourvec.phi() !=0 ? MET_fourvec.phi() : -99",
    # "MET_px                              := ? MET_fourvec.px() !=0 ? MET_fourvec.px() : -99",
    # "MET_py                              := ? MET_fourvec.py() !=0 ? MET_fourvec.py() : -99",
    # "MET_pz                              := ? MET_fourvec.pz() !=0 ? MET_fourvec.pz() : -99",
    "MET_E                               := ? MET_fourvec.E() !=0 ? MET_fourvec.E() : -99",

    "d_n_good_leptons                    := Cut_Variables[0]",
    "d_n_good_electrons                  := Cut_Variables[1]",
    "d_n_good_muons                      := Cut_Variables[2]",
    "d_n_good_jets                       := Cut_Variables[3]", 

    ## GEN 

    # Electrons 
    "n_gen_electrons                     := GenElectrons.size()", 
    "gen_leading_elec_pt                 := ? GenElectrons.size() >= 1 ? GenElectrons[0].pt() : -99 ",
    "gen_leading_elec_eta                := ? GenElectrons.size() >= 1 ? GenElectrons[0].eta() : -99 ",
    "gen_leading_elec_phi                := ? GenElectrons.size() >= 1 ? GenElectrons[0].phi() : -99 ",
    "gen_leading_elec_E                  := ? GenElectrons.size() >= 1 ? GenElectrons[0].E() : -99 ",

    # Muons 
    "n_gen_muons                         := GenMuons.size()", 
    "gen_leading_muon_pt                 := ? GenMuons.size() >= 1 ? GenMuons[0].pt() : -99 ",
    "gen_leading_muon_eta                := ? GenMuons.size() >= 1 ? GenMuons[0].eta() : -99 ",
    "gen_leading_muon_phi                := ? GenMuons.size() >= 1 ? GenMuons[0].phi() : -99 ",
    "gen_leading_muon_E                  := ? GenMuons.size() >= 1 ? GenMuons[0].E() : -99 ",

    # Photons 
    "n_gen_photons                       := GenPhotons.size()", 
    "gen_leading_photon_pt               := ? GenPhotons.size() >= 1 ? GenPhotons[0].pt() : -99 ",
    "gen_leading_photon_eta              := ? GenPhotons.size() >= 1 ? GenPhotons[0].eta() : -99 ",
    "gen_leading_photon_phi              := ? GenPhotons.size() >= 1 ? GenPhotons[0].phi() : -99 ",
    "gen_leading_photon_E                := ? GenPhotons.size() >= 1 ? GenPhotons[0].E() : -99 ",
    "gen_subleading_photon_pt            := ? GenPhotons.size() >= 2 ? GenPhotons[1].pt() : -99 ",
    "gen_subleading_photon_eta           := ? GenPhotons.size() >= 2 ? GenPhotons[1].eta() : -99 ",
    "gen_subleading_photon_phi           := ? GenPhotons.size() >= 2 ? GenPhotons[1].phi() : -99 ",
    "gen_subleading_photon_E             := ? GenPhotons.size() >= 2 ? GenPhotons[1].E() : -99 ",

    # Quarks 
    "n_gen_quarks                        := GenQuarks.size()", 
    "gen_leading_quark_pt                := ? GenQuarks.size() >= 1 ? GenQuarks[0].pt() : -99 ",
    "gen_leading_quark_eta               := ? GenQuarks.size() >= 1 ? GenQuarks[0].eta() : -99 ",
    "gen_leading_quark_phi               := ? GenQuarks.size() >= 1 ? GenQuarks[0].phi() : -99 ",
    "gen_leading_quark_E                 := ? GenQuarks.size() >= 1 ? GenQuarks[0].E() : -99 ",
    "gen_subleading_quark_pt             := ? GenQuarks.size() >= 2 ? GenQuarks[1].pt() : -99 ",
    "gen_subleading_quark_eta            := ? GenQuarks.size() >= 2 ? GenQuarks[1].eta() : -99 ",
    "gen_subleading_quark_phi            := ? GenQuarks.size() >= 2 ? GenQuarks[1].phi() : -99 ",
    "gen_subleading_quark_E              := ? GenQuarks.size() >= 2 ? GenQuarks[1].E() : -99 ",

    # Neutrinos  
    "n_gen_neutrinos                     := GenNeutrinos.size()", 
    "gen_leading_neutrino_pt             := ? GenNeutrinos.size() >= 1 ? GenNeutrinos[0].pt() : -99 ",
    "gen_leading_neutrino_eta            := ? GenNeutrinos.size() >= 1 ? GenNeutrinos[0].eta() : -99 ",
    "gen_leading_neutrino_phi            := ? GenNeutrinos.size() >= 1 ? GenNeutrinos[0].phi() : -99 ",
    "gen_leading_neutrino_E              := ? GenNeutrinos.size() >= 1 ? GenNeutrinos[0].E() : -99 ",


    # # Electrons 
    # # Set to -99 if there's no value 
    # "gen_leading_elec_pt                 := ? gen_leading_elec.pt() != 0 ? gen_leading_elec.pt() : -99 ",
    # "gen_leading_elec_eta                := ? gen_leading_elec.eta() != 0 ? gen_leading_elec.eta() : -99 ",
    # "gen_leading_elec_phi                := ? gen_leading_elec.phi() != 0 ? gen_leading_elec.phi() : -99 ",
    # "gen_leading_elec_E                  := ? gen_leading_elec.E() != 0 ? gen_leading_elec.E() : -99 ",

    # "gen_subleading_elec_pt              := ? gen_subleading_elec.pt() != 0 ? gen_subleading_elec.pt() : -99 ",
    # "gen_subleading_elec_eta             := ? gen_subleading_elec.eta() != 0 ? gen_subleading_elec.eta() : -99 ",
    # "gen_subleading_elec_phi             := ? gen_subleading_elec.phi() != 0 ? gen_subleading_elec.phi() : -99 ",
    # "gen_subleading_elec_E               := ? gen_subleading_elec.E() != 0 ? gen_subleading_elec.E() : -99 ",

    # # Muons 
    # # Set to -99 if there's no value 
    # "gen_leading_muon_pt                 := ? gen_leading_muon.pt() != 0 ? gen_leading_muon.pt() : -99 ",
    # "gen_leading_muon_eta                := ? gen_leading_muon.eta() != 0 ? gen_leading_muon.eta() : -99 ",
    # "gen_leading_muon_phi                := ? gen_leading_muon.phi() != 0 ? gen_leading_muon.phi() : -99 ",
    # "gen_leading_muon_E                  := ? gen_leading_muon.E() != 0 ? gen_leading_muon.E() : -99 ",

    # "gen_subleading_muon_pt              := ? gen_subleading_muon.pt() != 0 ? gen_subleading_muon.pt() : -99 ",
    # "gen_subleading_muon_eta             := ? gen_subleading_muon.eta() != 0 ? gen_subleading_muon.eta() : -99 ",
    # "gen_subleading_muon_phi             := ? gen_subleading_muon.phi() != 0 ? gen_subleading_muon.phi() : -99 ",
    # "gen_subleading_muon_E               := ? gen_subleading_muon.E() != 0 ? gen_subleading_muon.E() : -99 ",

    # # Higgs
    # # "Higgs_gg_pt                         := ? hgg_p4.pt() != 0 ? hgg_p4.pt() : -99 ",
    # # "Higgs_gg_eta                        := ? hgg_p4.eta() != 0 ? hgg_p4.eta() : -99 ",
    # # "Higgs_gg_phi                        := ? hgg_p4.phi() != 0 ? hgg_p4.phi() : -99 ",
    # # "Higgs_gg_M                          := ? hgg_p4.M() != 0 ? hgg_p4.M() : -99 ",
    # # "Higgs_gg_E                          := ? hgg_p4.E() != 0 ? hgg_p4.E() : -99 ",

    # # Photons
    # "leading_genpho_pt                   := ? l_genpho.pt() != 0 ? l_genpho.pt() : -99 ",
    # "subleading_genpho_pt                := ? sl_genpho.pt() != 0 ? sl_genpho.pt() : -99 ",

    # # Leading and subleading neutrinos 

    # # # Quarks 
    # # "m_qq                                := ? m_qq() != 0 ? m_qq() : -99",
    # # "mdq_invmass                         := ? MatchingDiQuark.mass() != 0 ? MatchingDiQuark.mass() : -99",
    # # "nmdq_invmass                        := NonMatchingDiQuark.mass()",
    # # "dr_1jet_and_q1                      := ? dr_1() != 0 ? dr_1() : -99",
    # # "dr_1jet_and_q2                      := ? dr_2() != 0 ? dr_2() : -99",
    # # "merged_qs_into_jet                  := ? merged_qs() != 0 ? merged_qs() : -99",
    # # "qone_matches                        := ? qone_matches() != 0 ? qone_matches() : -99",
    # # "qtwo_matches                        := ? qtwo_matches() != 0 ? qtwo_matches() : -99",
]
