//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Feb  5 09:25:57 2015 by ROOT version 5.34/18
// from TTree ggh_m125_8TeV/ggh_m125_8TeV
// found on file: histograms_CMS-HGG_8TeV_massfac_highstat.root
//////////////////////////////////////////////////////////

#ifndef GlobeTree_h
#define GlobeTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <string>

class GlobeTree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Float_t         run;
   Float_t         lumis;
   Double_t        event;
   Float_t         itype;
   Float_t         nvtx;
   Float_t         rho;
   Float_t         pu_n;
   Float_t         xsec_weight;
   Float_t         pu_weight;
   Float_t         full_weight;
   Float_t         MET;
   Float_t         MET_phi;
   Float_t         sigmaMrvoM;
   Float_t         sigmaMwvoM;
   Float_t         sigmaEoE1;
   Float_t         sigmaEoE2;
   Float_t         ptoM1;
   Float_t         ptoM2;
   Float_t         vtxprob;
   Float_t         ptbal;
   Float_t         ptasym;
   Float_t         logspt2;
   Float_t         p2conv;
   Float_t         nconv;
   Float_t         vtxmva;
   Float_t         vtxdz;
   Float_t         mass;
   Float_t         vbfcat;
   Float_t         dipho_pt;
   Float_t         dipho_mva;
   Float_t         dipho_mva_cat;
   Float_t         et1;
   Float_t         et2;
   Float_t         eta1;
   Float_t         eta2;
   Float_t         cosphi;
   Float_t         idmva1;
   Float_t         idmva2;
   Float_t         full_cat;
   Float_t         genmatch1;
   Float_t         genmatch2;
   Float_t         drtoeltk1;
   Float_t         drtoeltk2;
   Float_t         cicpf4cutlevel1;
   Float_t         cicpf4cutlevel2;
   Float_t         r91;
   Float_t         r92;
   Float_t         sieie1;
   Float_t         sieie2;
   Float_t         chiso1;
   Float_t         chiso2;
   Float_t         hoe1;
   Float_t         hoe2;
   Int_t           conv1;
   Int_t           conv2;
   Float_t         pfchiso2_1;
   Float_t         pfchiso2_2;
   Float_t         chisow1;
   Float_t         chisow2;
   Float_t         phoiso1;
   Float_t         phoiso2;
   Float_t         phoiso041;
   Float_t         phoiso042;
   Float_t         isorv1;
   Float_t         isowv1;
   Float_t         isorv2;
   Float_t         isowv2;
   Float_t         ecaliso03_1;
   Float_t         ecaliso03_2;
   Float_t         hcaliso03_1;
   Float_t         hcaliso03_2;
   Float_t         trkiso03_1;
   Float_t         trkiso03_2;
   Float_t         etawidth1;
   Float_t         etawidth2;
   Float_t         phiwidth1;
   Float_t         phiwidth2;
   Float_t         vtx_x;
   Float_t         vtx_y;
   Float_t         vtx_z;
   Float_t         gv_x;
   Float_t         gv_y;
   Float_t         gv_z;
   Int_t           issyst;
   string          *name1;
   Int_t           isEB1;
   Int_t           isEB2;
   Float_t         sieip1;
   Float_t         sieip2;
   Float_t         s4ratio1;
   Float_t         s4ratio2;
   Float_t         effSigma1;
   Float_t         effSigma2;
   Float_t         regrerr1;
   Float_t         regrerr2;
   Float_t         eleregr1;
   Float_t         eleregrerr1;
   Float_t         eleregr2;
   Float_t         eleregrerr2;
   Float_t         sceta1;
   Float_t         scphi1;
   Float_t         scraw1;
   Float_t         e5x51;
   Float_t         e3x31;
   Float_t         sipip1;
   Float_t         emax1;
   Float_t         e2nd1;
   Float_t         eright1;
   Float_t         eleft1;
   Float_t         etop1;
   Float_t         ebottom1;
   Float_t         bceta1;
   Float_t         bcphi1;
   Float_t         bce1;
   Float_t         bieta1;
   Float_t         biphi1;
   Float_t         betacry1;
   Float_t         bphicry1;
   Float_t         sceta2;
   Float_t         scphi2;
   Float_t         scraw2;
   Float_t         e5x52;
   Float_t         e3x32;
   Float_t         sipip2;
   Float_t         emax2;
   Float_t         e2nd2;
   Float_t         eright2;
   Float_t         eleft2;
   Float_t         etop2;
   Float_t         ebottom2;
   Float_t         bceta2;
   Float_t         bcphi2;
   Float_t         bce2;
   Float_t         bieta2;
   Float_t         biphi2;
   Float_t         betacry2;
   Float_t         bphicry2;
   Float_t         dijet_leadEta;
   Float_t         dijet_subleadEta;
   Float_t         dijet_LeadJPt;
   Float_t         dijet_SubJPt;
   Float_t         dijet_dEta;
   Float_t         dijet_Zep;
   Float_t         dijet_dPhi;
   Float_t         dijet_Mjj;
   Float_t         dijet_MVA;
   Float_t         bdt_combined;
   Float_t         dRphojet1;
   Float_t         dRphojet2;
   Float_t         njets10;
   Float_t         njets15;
   Float_t         njets20;
   Float_t         bdt_combined;

   // List of branches
   TBranch        *b_run;   //!
   TBranch        *b_lumis;   //!
   TBranch        *b_event;   //!
   TBranch        *b_itype;   //!
   TBranch        *b_nvtx;   //!
   TBranch        *b_rho;   //!
   TBranch        *b_pu_n;   //!
   TBranch        *b_xsec_weight;   //!
   TBranch        *b_pu_weight;   //!
   TBranch        *b_full_weight;   //!
   TBranch        *b_MET;   //!
   TBranch        *b_MET_phi;   //!
   TBranch        *b_sigmaMrvoM;   //!
   TBranch        *b_sigmaMwvoM;   //!
   TBranch        *b_sigmaEoE1;   //!
   TBranch        *b_sigmaEoE2;   //!
   TBranch        *b_ptoM1;   //!
   TBranch        *b_ptoM2;   //!
   TBranch        *b_vtxprob;   //!
   TBranch        *b_ptbal;   //!
   TBranch        *b_ptasym;   //!
   TBranch        *b_logspt2;   //!
   TBranch        *b_p2conv;   //!
   TBranch        *b_nconv;   //!
   TBranch        *b_vtxmva;   //!
   TBranch        *b_vtxdz;   //!
   TBranch        *b_mass;   //!
   TBranch        *b_vbfcat;   //!
   TBranch        *b_dipho_pt;   //!
   TBranch        *b_dipho_mva;   //!
   TBranch        *b_dipho_mva_cat;   //!
   TBranch        *b_et1;   //!
   TBranch        *b_et2;   //!
   TBranch        *b_eta1;   //!
   TBranch        *b_eta2;   //!
   TBranch        *b_cosphi;   //!
   TBranch        *b_idmva1;   //!
   TBranch        *b_idmva2;   //!
   TBranch        *b_full_cat;   //!
   TBranch        *b_genmatch1;   //!
   TBranch        *b_genmatch2;   //!
   TBranch        *b_drtoeltk1;   //!
   TBranch        *b_drtoeltk2;   //!
   TBranch        *b_cicpf4cutlevel1;   //!
   TBranch        *b_cicpf4cutlevel2;   //!
   TBranch        *b_r91;   //!
   TBranch        *b_r92;   //!
   TBranch        *b_sieie1;   //!
   TBranch        *b_sieie2;   //!
   TBranch        *b_chiso1;   //!
   TBranch        *b_chiso2;   //!
   TBranch        *b_hoe1;   //!
   TBranch        *b_hoe2;   //!
   TBranch        *b_conv1;   //!
   TBranch        *b_conv2;   //!
   TBranch        *b_pfchiso2_1;   //!
   TBranch        *b_pfchiso2_2;   //!
   TBranch        *b_chisow1;   //!
   TBranch        *b_chisow2;   //!
   TBranch        *b_phoiso1;   //!
   TBranch        *b_phoiso2;   //!
   TBranch        *b_phoiso041;   //!
   TBranch        *b_phoiso042;   //!
   TBranch        *b_isorv1;   //!
   TBranch        *b_isowv1;   //!
   TBranch        *b_isorv2;   //!
   TBranch        *b_isowv2;   //!
   TBranch        *b_ecaliso03_1;   //!
   TBranch        *b_ecaliso03_2;   //!
   TBranch        *b_hcaliso03_1;   //!
   TBranch        *b_hcaliso03_2;   //!
   TBranch        *b_trkiso03_1;   //!
   TBranch        *b_trkiso03_2;   //!
   TBranch        *b_etawidth1;   //!
   TBranch        *b_etawidth2;   //!
   TBranch        *b_phiwidth1;   //!
   TBranch        *b_phiwidth2;   //!
   TBranch        *b_vtx_x;   //!
   TBranch        *b_vtx_y;   //!
   TBranch        *b_vtx_z;   //!
   TBranch        *b_gv_x;   //!
   TBranch        *b_gv_y;   //!
   TBranch        *b_gv_z;   //!
   TBranch        *b_issyst;   //!
   TBranch        *b_name1;   //!
   TBranch        *b_isEB1;   //!
   TBranch        *b_isEB2;   //!
   TBranch        *b_sieip1;   //!
   TBranch        *b_sieip2;   //!
   TBranch        *b_s4ratio1;   //!
   TBranch        *b_s4ratio2;   //!
   TBranch        *b_effSigma1;   //!
   TBranch        *b_effSigma2;   //!
   TBranch        *b_regrerr1;   //!
   TBranch        *b_regrerr2;   //!
   TBranch        *b_eleregr1;   //!
   TBranch        *b_eleregrerr1;   //!
   TBranch        *b_eleregr2;   //!
   TBranch        *b_eleregrerr2;   //!
   TBranch        *b_sceta1;   //!
   TBranch        *b_scphi1;   //!
   TBranch        *b_scraw1;   //!
   TBranch        *b_e5x51;   //!
   TBranch        *b_e3x31;   //!
   TBranch        *b_sipip1;   //!
   TBranch        *b_emax1;   //!
   TBranch        *b_e2nd1;   //!
   TBranch        *b_eright1;   //!
   TBranch        *b_eleft1;   //!
   TBranch        *b_etop1;   //!
   TBranch        *b_ebottom1;   //!
   TBranch        *b_bceta1;   //!
   TBranch        *b_bcphi1;   //!
   TBranch        *b_bce1;   //!
   TBranch        *b_bieta1;   //!
   TBranch        *b_biphi1;   //!
   TBranch        *b_betacry1;   //!
   TBranch        *b_bphicry1;   //!
   TBranch        *b_sceta2;   //!
   TBranch        *b_scphi2;   //!
   TBranch        *b_scraw2;   //!
   TBranch        *b_e5x52;   //!
   TBranch        *b_e3x32;   //!
   TBranch        *b_sipip2;   //!
   TBranch        *b_emax2;   //!
   TBranch        *b_e2nd2;   //!
   TBranch        *b_eright2;   //!
   TBranch        *b_eleft2;   //!
   TBranch        *b_etop2;   //!
   TBranch        *b_ebottom2;   //!
   TBranch        *b_bceta2;   //!
   TBranch        *b_bcphi2;   //!
   TBranch        *b_bce2;   //!
   TBranch        *b_bieta2;   //!
   TBranch        *b_biphi2;   //!
   TBranch        *b_betacry2;   //!
   TBranch        *b_bphicry2;   //!
   TBranch        *b_dijet_leadEta;   //!
   TBranch        *b_dijet_subleadEta;   //!
   TBranch        *b_dijet_LeadJPt;   //!
   TBranch        *b_dijet_SubJPt;   //!
   TBranch        *b_dijet_dEta;   //!
   TBranch        *b_dijet_Zep;   //!
   TBranch        *b_dijet_dPhi;   //!
   TBranch        *b_dijet_Mjj;   //!
   TBranch        *b_dijet_MVA;   //!
   TBranch        *b_bdt_combined;   //!
   TBranch        *b_dRphojet1;   //!
   TBranch        *b_dRphojet2;   //!
   TBranch        *b_njets10;   //!
   TBranch        *b_njets15;   //!
   TBranch        *b_njets20;   //!
   TBranch        *b_bdt_combined;   //!

   GlobeTree(TTree *tree=0);
   virtual ~GlobeTree();
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Int_t    GetEntries();
   virtual void     Init(TTree *tree);
};

#endif

#ifdef GlobeTree_cxx
GlobeTree::GlobeTree(TTree *tree) : fChain(0) 
{
   if (tree == 0)
       return;
   Init(tree);
}

GlobeTree::~GlobeTree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t GlobeTree::GetEntry(Long64_t entry)
{
   if (!fChain)
       return 0;
   return fChain->GetEntry(entry);
}

Int_t GlobeTree::GetEntries()
{
   if (!fChain)
       return 0;
   return fChain->GetEntries();
}

void GlobeTree::Init(TTree *tree)
{
   // Set object pointer
   name1 = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("lumis", &lumis, &b_lumis);
   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("itype", &itype, &b_itype);
   fChain->SetBranchAddress("nvtx", &nvtx, &b_nvtx);
   fChain->SetBranchAddress("rho", &rho, &b_rho);
   fChain->SetBranchAddress("pu_n", &pu_n, &b_pu_n);
   fChain->SetBranchAddress("xsec_weight", &xsec_weight, &b_xsec_weight);
   fChain->SetBranchAddress("pu_weight", &pu_weight, &b_pu_weight);
   fChain->SetBranchAddress("full_weight", &full_weight, &b_full_weight);
   fChain->SetBranchAddress("MET", &MET, &b_MET);
   fChain->SetBranchAddress("MET_phi", &MET_phi, &b_MET_phi);
   fChain->SetBranchAddress("sigmaMrvoM", &sigmaMrvoM, &b_sigmaMrvoM);
   fChain->SetBranchAddress("sigmaMwvoM", &sigmaMwvoM, &b_sigmaMwvoM);
   fChain->SetBranchAddress("sigmaEoE1", &sigmaEoE1, &b_sigmaEoE1);
   fChain->SetBranchAddress("sigmaEoE2", &sigmaEoE2, &b_sigmaEoE2);
   fChain->SetBranchAddress("ptoM1", &ptoM1, &b_ptoM1);
   fChain->SetBranchAddress("ptoM2", &ptoM2, &b_ptoM2);
   fChain->SetBranchAddress("vtxprob", &vtxprob, &b_vtxprob);
   fChain->SetBranchAddress("ptbal", &ptbal, &b_ptbal);
   fChain->SetBranchAddress("ptasym", &ptasym, &b_ptasym);
   fChain->SetBranchAddress("logspt2", &logspt2, &b_logspt2);
   fChain->SetBranchAddress("p2conv", &p2conv, &b_p2conv);
   fChain->SetBranchAddress("nconv", &nconv, &b_nconv);
   fChain->SetBranchAddress("vtxmva", &vtxmva, &b_vtxmva);
   fChain->SetBranchAddress("vtxdz", &vtxdz, &b_vtxdz);
   fChain->SetBranchAddress("mass", &mass, &b_mass);
   fChain->SetBranchAddress("vbfcat", &vbfcat, &b_vbfcat);
   fChain->SetBranchAddress("dipho_pt", &dipho_pt, &b_dipho_pt);
   fChain->SetBranchAddress("dipho_mva", &dipho_mva, &b_dipho_mva);
   fChain->SetBranchAddress("dipho_mva_cat", &dipho_mva_cat, &b_dipho_mva_cat);
   fChain->SetBranchAddress("et1", &et1, &b_et1);
   fChain->SetBranchAddress("et2", &et2, &b_et2);
   fChain->SetBranchAddress("eta1", &eta1, &b_eta1);
   fChain->SetBranchAddress("eta2", &eta2, &b_eta2);
   fChain->SetBranchAddress("cosphi", &cosphi, &b_cosphi);
   fChain->SetBranchAddress("idmva1", &idmva1, &b_idmva1);
   fChain->SetBranchAddress("idmva2", &idmva2, &b_idmva2);
   fChain->SetBranchAddress("full_cat", &full_cat, &b_full_cat);
   fChain->SetBranchAddress("genmatch1", &genmatch1, &b_genmatch1);
   fChain->SetBranchAddress("genmatch2", &genmatch2, &b_genmatch2);
   fChain->SetBranchAddress("drtoeltk1", &drtoeltk1, &b_drtoeltk1);
   fChain->SetBranchAddress("drtoeltk2", &drtoeltk2, &b_drtoeltk2);
   fChain->SetBranchAddress("cicpf4cutlevel1", &cicpf4cutlevel1, &b_cicpf4cutlevel1);
   fChain->SetBranchAddress("cicpf4cutlevel2", &cicpf4cutlevel2, &b_cicpf4cutlevel2);
   fChain->SetBranchAddress("r91", &r91, &b_r91);
   fChain->SetBranchAddress("r92", &r92, &b_r92);
   fChain->SetBranchAddress("sieie1", &sieie1, &b_sieie1);
   fChain->SetBranchAddress("sieie2", &sieie2, &b_sieie2);
   fChain->SetBranchAddress("chiso1", &chiso1, &b_chiso1);
   fChain->SetBranchAddress("chiso2", &chiso2, &b_chiso2);
   fChain->SetBranchAddress("hoe1", &hoe1, &b_hoe1);
   fChain->SetBranchAddress("hoe2", &hoe2, &b_hoe2);
   fChain->SetBranchAddress("conv1", &conv1, &b_conv1);
   fChain->SetBranchAddress("conv2", &conv2, &b_conv2);
   fChain->SetBranchAddress("pfchiso2_1", &pfchiso2_1, &b_pfchiso2_1);
   fChain->SetBranchAddress("pfchiso2_2", &pfchiso2_2, &b_pfchiso2_2);
   fChain->SetBranchAddress("chisow1", &chisow1, &b_chisow1);
   fChain->SetBranchAddress("chisow2", &chisow2, &b_chisow2);
   fChain->SetBranchAddress("phoiso1", &phoiso1, &b_phoiso1);
   fChain->SetBranchAddress("phoiso2", &phoiso2, &b_phoiso2);
   fChain->SetBranchAddress("phoiso041", &phoiso041, &b_phoiso041);
   fChain->SetBranchAddress("phoiso042", &phoiso042, &b_phoiso042);
   fChain->SetBranchAddress("isorv1", &isorv1, &b_isorv1);
   fChain->SetBranchAddress("isowv1", &isowv1, &b_isowv1);
   fChain->SetBranchAddress("isorv2", &isorv2, &b_isorv2);
   fChain->SetBranchAddress("isowv2", &isowv2, &b_isowv2);
   fChain->SetBranchAddress("ecaliso03_1", &ecaliso03_1, &b_ecaliso03_1);
   fChain->SetBranchAddress("ecaliso03_2", &ecaliso03_2, &b_ecaliso03_2);
   fChain->SetBranchAddress("hcaliso03_1", &hcaliso03_1, &b_hcaliso03_1);
   fChain->SetBranchAddress("hcaliso03_2", &hcaliso03_2, &b_hcaliso03_2);
   fChain->SetBranchAddress("trkiso03_1", &trkiso03_1, &b_trkiso03_1);
   fChain->SetBranchAddress("trkiso03_2", &trkiso03_2, &b_trkiso03_2);
   fChain->SetBranchAddress("etawidth1", &etawidth1, &b_etawidth1);
   fChain->SetBranchAddress("etawidth2", &etawidth2, &b_etawidth2);
   fChain->SetBranchAddress("phiwidth1", &phiwidth1, &b_phiwidth1);
   fChain->SetBranchAddress("phiwidth2", &phiwidth2, &b_phiwidth2);
   fChain->SetBranchAddress("vtx_x", &vtx_x, &b_vtx_x);
   fChain->SetBranchAddress("vtx_y", &vtx_y, &b_vtx_y);
   fChain->SetBranchAddress("vtx_z", &vtx_z, &b_vtx_z);
   fChain->SetBranchAddress("gv_x", &gv_x, &b_gv_x);
   fChain->SetBranchAddress("gv_y", &gv_y, &b_gv_y);
   fChain->SetBranchAddress("gv_z", &gv_z, &b_gv_z);
   fChain->SetBranchAddress("issyst", &issyst, &b_issyst);
   fChain->SetBranchAddress("name1", &name1, &b_name1);
   fChain->SetBranchAddress("isEB1", &isEB1, &b_isEB1);
   fChain->SetBranchAddress("isEB2", &isEB2, &b_isEB2);
   fChain->SetBranchAddress("sieip1", &sieip1, &b_sieip1);
   fChain->SetBranchAddress("sieip2", &sieip2, &b_sieip2);
   fChain->SetBranchAddress("s4ratio1", &s4ratio1, &b_s4ratio1);
   fChain->SetBranchAddress("s4ratio2", &s4ratio2, &b_s4ratio2);
   fChain->SetBranchAddress("effSigma1", &effSigma1, &b_effSigma1);
   fChain->SetBranchAddress("effSigma2", &effSigma2, &b_effSigma2);
   fChain->SetBranchAddress("regrerr1", &regrerr1, &b_regrerr1);
   fChain->SetBranchAddress("regrerr2", &regrerr2, &b_regrerr2);
   fChain->SetBranchAddress("eleregr1", &eleregr1, &b_eleregr1);
   fChain->SetBranchAddress("eleregrerr1", &eleregrerr1, &b_eleregrerr1);
   fChain->SetBranchAddress("eleregr2", &eleregr2, &b_eleregr2);
   fChain->SetBranchAddress("eleregrerr2", &eleregrerr2, &b_eleregrerr2);
   fChain->SetBranchAddress("sceta1", &sceta1, &b_sceta1);
   fChain->SetBranchAddress("scphi1", &scphi1, &b_scphi1);
   fChain->SetBranchAddress("scraw1", &scraw1, &b_scraw1);
   fChain->SetBranchAddress("e5x51", &e5x51, &b_e5x51);
   fChain->SetBranchAddress("e3x31", &e3x31, &b_e3x31);
   fChain->SetBranchAddress("sipip1", &sipip1, &b_sipip1);
   fChain->SetBranchAddress("emax1", &emax1, &b_emax1);
   fChain->SetBranchAddress("e2nd1", &e2nd1, &b_e2nd1);
   fChain->SetBranchAddress("eright1", &eright1, &b_eright1);
   fChain->SetBranchAddress("eleft1", &eleft1, &b_eleft1);
   fChain->SetBranchAddress("etop1", &etop1, &b_etop1);
   fChain->SetBranchAddress("ebottom1", &ebottom1, &b_ebottom1);
   fChain->SetBranchAddress("bceta1", &bceta1, &b_bceta1);
   fChain->SetBranchAddress("bcphi1", &bcphi1, &b_bcphi1);
   fChain->SetBranchAddress("bce1", &bce1, &b_bce1);
   fChain->SetBranchAddress("bieta1", &bieta1, &b_bieta1);
   fChain->SetBranchAddress("biphi1", &biphi1, &b_biphi1);
   fChain->SetBranchAddress("betacry1", &betacry1, &b_betacry1);
   fChain->SetBranchAddress("bphicry1", &bphicry1, &b_bphicry1);
   fChain->SetBranchAddress("sceta2", &sceta2, &b_sceta2);
   fChain->SetBranchAddress("scphi2", &scphi2, &b_scphi2);
   fChain->SetBranchAddress("scraw2", &scraw2, &b_scraw2);
   fChain->SetBranchAddress("e5x52", &e5x52, &b_e5x52);
   fChain->SetBranchAddress("e3x32", &e3x32, &b_e3x32);
   fChain->SetBranchAddress("sipip2", &sipip2, &b_sipip2);
   fChain->SetBranchAddress("emax2", &emax2, &b_emax2);
   fChain->SetBranchAddress("e2nd2", &e2nd2, &b_e2nd2);
   fChain->SetBranchAddress("eright2", &eright2, &b_eright2);
   fChain->SetBranchAddress("eleft2", &eleft2, &b_eleft2);
   fChain->SetBranchAddress("etop2", &etop2, &b_etop2);
   fChain->SetBranchAddress("ebottom2", &ebottom2, &b_ebottom2);
   fChain->SetBranchAddress("bceta2", &bceta2, &b_bceta2);
   fChain->SetBranchAddress("bcphi2", &bcphi2, &b_bcphi2);
   fChain->SetBranchAddress("bce2", &bce2, &b_bce2);
   fChain->SetBranchAddress("bieta2", &bieta2, &b_bieta2);
   fChain->SetBranchAddress("biphi2", &biphi2, &b_biphi2);
   fChain->SetBranchAddress("betacry2", &betacry2, &b_betacry2);
   fChain->SetBranchAddress("bphicry2", &bphicry2, &b_bphicry2);
   fChain->SetBranchAddress("dijet_leadEta", &dijet_leadEta, &b_dijet_leadEta);
   fChain->SetBranchAddress("dijet_subleadEta", &dijet_subleadEta, &b_dijet_subleadEta);
   fChain->SetBranchAddress("dijet_LeadJPt", &dijet_LeadJPt, &b_dijet_LeadJPt);
   fChain->SetBranchAddress("dijet_SubJPt", &dijet_SubJPt, &b_dijet_SubJPt);
   fChain->SetBranchAddress("dijet_dEta", &dijet_dEta, &b_dijet_dEta);
   fChain->SetBranchAddress("dijet_Zep", &dijet_Zep, &b_dijet_Zep);
   fChain->SetBranchAddress("dijet_dPhi", &dijet_dPhi, &b_dijet_dPhi);
   fChain->SetBranchAddress("dijet_Mjj", &dijet_Mjj, &b_dijet_Mjj);
   fChain->SetBranchAddress("dijet_MVA", &dijet_MVA, &b_dijet_MVA);
   fChain->SetBranchAddress("bdt_combined", &bdt_combined, &b_bdt_combined);
   fChain->SetBranchAddress("dRphojet1", &dRphojet1, &b_dRphojet1);
   fChain->SetBranchAddress("dRphojet2", &dRphojet2, &b_dRphojet2);
   fChain->SetBranchAddress("njets10", &njets10, &b_njets10);
   fChain->SetBranchAddress("njets15", &njets15, &b_njets15);
   fChain->SetBranchAddress("njets20", &njets20, &b_njets20);
//    fChain->SetBranchAddress("bdt_combined", &bdt_combined, &b_bdt_combined);

}

#endif 
