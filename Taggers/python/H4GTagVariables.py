test_variables = [
"test                    := n_photons"
]
pho_variables = [
      "npho                    := phoVector.size()",
      "pho1_pt                 := phoP4Corrected[0].pt()",
      "pho2_pt                 := phoP4Corrected[1].pt()",
      "pho3_pt                 := ? phoP4Corrected.size() > 2 ? phoP4Corrected[2].pt() : -1",
      "pho4_pt                 := ? phoP4Corrected.size() > 3 ? phoP4Corrected[3].pt() : -1",
      "pho1_eta                := phoP4Corrected[0].eta()",
      "pho2_eta                := phoP4Corrected[1].eta()",
      "pho3_eta                := ? phoP4Corrected.size() > 2 ? phoP4Corrected[2].eta() : -999",
      "pho4_eta                := ? phoP4Corrected.size() > 3 ? phoP4Corrected[3].eta() : -999",
      "pho1_phi                := phoP4Corrected[0].phi()",
      "pho2_phi                := phoP4Corrected[1].phi()",
      "pho3_phi                := ? phoP4Corrected.size() > 2 ? phoP4Corrected[2].phi() : -999",
      "pho4_phi                := ? phoP4Corrected.size() > 3 ? phoP4Corrected[3].phi() : -999",
      "pho1_energy             := phoP4Corrected[0].energy()",
      "pho2_energy             := phoP4Corrected[1].energy()",
      "pho3_energy             := ? phoP4Corrected.size() > 2 ? phoP4Corrected[2].energy() : -1",
      "pho4_energy             := ? phoP4Corrected.size() > 3 ? phoP4Corrected[3].energy() : -1",
      "pho1_old_r9             := phoP4Corrected[0].old_r9()",
      "pho2_old_r9             := phoP4Corrected[1].old_r9()",
      "pho3_old_r9             := ? phoP4Corrected.size() > 2 ? phoP4Corrected[2].old_r9() : -999",
      "pho4_old_r9             := ? phoP4Corrected.size() > 3 ? phoP4Corrected[3].old_r9() : -999",
      "pho1_full5x5_r9         := phoP4Corrected[0].full5x5_r9()",
      "pho2_full5x5_r9         := phoP4Corrected[1].full5x5_r9()",
      "pho3_full5x5_r9         := ? phoP4Corrected.size() > 2 ? phoP4Corrected[2].full5x5_r9() : -999",
      "pho4_full5x5_r9         := ? phoP4Corrected.size() > 3 ? phoP4Corrected[3].full5x5_r9() : -999",
      "pho1_EGMVA              := phoP4Corrected[0].userFloat('PhotonMVAEstimatorRun2Spring15NonTrig25nsV2p1Values')",
      "pho2_EGMVA              := phoP4Corrected[1].userFloat('PhotonMVAEstimatorRun2Spring15NonTrig25nsV2p1Values')",
      "pho3_EGMVA              := ? phoP4Corrected.size() > 2 ? phoP4Corrected[2].userFloat('PhotonMVAEstimatorRun2Spring15NonTrig25nsV2p1Values') : -999",
      "pho4_EGMVA              := ? phoP4Corrected.size() > 3 ? phoP4Corrected[3].userFloat('PhotonMVAEstimatorRun2Spring15NonTrig25nsV2p1Values') : -999",
      "pho1_MVA                := pho1_MVA",
      "pho2_MVA                := pho2_MVA",
      "pho3_MVA                := pho3_MVA",
      "pho4_MVA                := pho4_MVA",
      "pho1_match              := phoP4Corrected[0].genMatchType()",
      "pho2_match              := phoP4Corrected[1].genMatchType()",
      "pho3_match              := ? phoP4Corrected.size() > 2 ? phoP4Corrected[2].genMatchType() : -999",
      "pho4_match              := ? phoP4Corrected.size() > 3 ? phoP4Corrected[3].genMatchType() : -999",
      "pho1_pixelseed          := phoP4Corrected[0].hasPixelSeed()",
      "pho2_pixelseed          := phoP4Corrected[1].hasPixelSeed()",
      "pho3_pixelseed          := ? phoP4Corrected.size() > 2 ? phoP4Corrected[2].hasPixelSeed() : -999",
      "pho4_pixelseed          := ? phoP4Corrected.size() > 3 ? phoP4Corrected[3].hasPixelSeed() : -999",
      "pho1_electronveto       := phoP4Corrected[0].passElectronVeto()",
      "pho2_electronveto       := phoP4Corrected[1].passElectronVeto()",
      "pho3_electronveto       := ? phoP4Corrected.size() > 2 ? phoP4Corrected[2].passElectronVeto() : -999",
      "pho4_electronveto       := ? phoP4Corrected.size() > 3 ? phoP4Corrected[3].passElectronVeto() : -999",
      "pho12_dR                := deltaR( phoP4Corrected[0].eta, phoP4Corrected[0].phi, phoP4Corrected[1].eta, phoP4Corrected[1].phi )",
      "pho13_dR                := ? phoP4Corrected.size() > 2 ? deltaR( phoP4Corrected[0].eta, phoP4Corrected[0].phi, phoP4Corrected[2].eta, phoP4Corrected[2].phi ) : -1",
      "pho14_dR                := ? phoP4Corrected.size() > 3 ? deltaR( phoP4Corrected[0].eta, phoP4Corrected[0].phi, phoP4Corrected[3].eta, phoP4Corrected[3].phi ) : -1",
      "pho23_dR                := ? phoP4Corrected.size() > 2 ? deltaR( phoP4Corrected[1].eta, phoP4Corrected[1].phi, phoP4Corrected[2].eta, phoP4Corrected[2].phi ) : -1",
      "pho24_dR                := ? phoP4Corrected.size() > 3 ? deltaR( phoP4Corrected[1].eta, phoP4Corrected[1].phi, phoP4Corrected[3].eta, phoP4Corrected[3].phi ) : -1",
      "pho34_dR                := ? phoP4Corrected.size() > 3 ? deltaR( phoP4Corrected[2].eta, phoP4Corrected[2].phi, phoP4Corrected[3].eta, phoP4Corrected[3].phi ) : -1",
      "pho12_m                 := h4gPho12.mass",
      "pho13_m                 := h4gPho13.mass",
      "pho14_m                 := h4gPho14.mass",
      "pho23_m                 := h4gPho23.mass",
      "pho24_m                 := h4gPho24.mass",
      "pho34_m                 := h4gPho34.mass",
      "n_vertices              := Vertices.size()",
      "Vtx_x                   := Vertices[0].x()",
      "Vtx_y                   := Vertices[0].y()",
      "Vtx_z                   := Vertices[0].z()",
      "Vtx_hgg_x               := vertex_diphoton.x()",
      "Vtx_hgg_y               := vertex_diphoton.y()",
      "Vtx_hgg_z               := vertex_diphoton.z()",
      "genvertex_x             := genVertex.x()",
      "genvertex_y             := genVertex.y()",
      "genvertex_z             := genVertex.z()",
      "dZ_hggVtx               := genVertex.z() - vertex_diphoton.z()",
      "dZ_zeroVtx              := genVertex.z() - Vertices[0].z()",
      "BS_x                    := BSPoint.x()",
      "BS_y                    := BSPoint.y()",
      "BS_z                    := BSPoint.z()",
      "dZ_BS_zeroVtx           := BSPoint.z() - Vertices[0].z()",
      "dZ_BS_hggVtx            := BSPoint.z() - vertex_diphoton.z()",
      "dZ_sigmaZ               := BSPoint.sigmaZ()"
      # "dZ                      := genVertex.z() - vertex.z()",
      # "dZ_prime_check          := genVertex.z() - vtx_diphoton_z",
      # "Vtx_dipho_x             := vertex_diphoton.x()",
      # "Vtx_dipho_y             := vertex_diphoton.y()",
      # "Vtx_dipho_z             := vertex_diphoton.z()",







      # "gen_X                   := genVtx_x",
      # "gen_Y                   := genVtx_y",
      # "gen_Z                   := genVtx_z"
      # "n_vertices              := Vertices.size()",
      # "test                    := genVertex.z() - Vertices[0].z()"
    ]

dipho_variables = [
    "dp1_mass                := h4gDiPho1.mass() ",
    "dp2_mass                := h4gDiPho2.mass() ",
    "avg_dp_mass             := (h4gDiPho1.mass()+h4gDiPho2.mass())/2",
    "dp1_pt                  := h4gDiPho1.pt() ",
    "dp2_pt                  := h4gDiPho2.pt() ",
    "dp1_eta                 := h4gDiPho1.eta() ",
    "dp2_eta                 := h4gDiPho2.eta() ",
    "dp1_phi                 := h4gDiPho1.phi() ",
    "dp2_phi                 := h4gDiPho2.phi() ",
    "dp1_e                   := h4gDiPho1.energy() ",
    "dp2_e                   := h4gDiPho2.energy() ",
    "dp1_dr                  := deltaR(h4gDiPho1_Pho1.eta(), h4gDiPho1_Pho1.phi(), h4gDiPho1_Pho2.eta(), h4gDiPho1_Pho2.phi())",
    "dp2_dr                  := deltaR(h4gDiPho2_Pho1.eta(), h4gDiPho2_Pho1.phi(), h4gDiPho2_Pho2.eta(), h4gDiPho2_Pho2.phi())",
    "dp1_p1i                 := h4gDiPho1_iPho1",
    "dp1_p2i                 := h4gDiPho1_iPho2",
    "dp2_p1i                 := h4gDiPho2_iPho1",
    "dp2_p2i                 := h4gDiPho2_iPho2",
    "dp1_PtoverMass          := h4gDiPho1.mass()/h4gDiPho1.pt()",
    "dp2_PtoverMass          := h4gDiPho2.mass()/h4gDiPho2.pt()",
    "absCosThetaStar_CS      := abs(getCosThetaStar_CS(6500))",
    "absCosTheta_pho_a1      := abs(CosThetaAngles()[1])",
    "absCosTheta_pho_a2      := abs(CosThetaAngles()[0])"
]

tp_variables = [
    "tp_mass                 := h4gFourVect.mass()",
    "tp_pt                   := h4gFourVect.pt()",
    "tp_eta                  := h4gFourVect.eta()",
    "tp_phi                  := h4gFourVect.phi()",
    "tp_PtoverMass           := h4gFourVect.mass()/h4gFourVect.pt()"
]

ws_variables = [
   "tp_mass                 := h4gFourVect.mass()",
   "dp1_mass                := h4gDiPho1.mass() ",
   "dp2_mass                := h4gDiPho2.mass() ",
   "avg_dp_mass             := (h4gDiPho1.mass()+h4gDiPho2.mass())/2",
   "dZ                      := genVertex.z() - vertex.z()"
]
