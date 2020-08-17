#include "flashgg/DataFormats/interface/VBFMVAResult.h"


namespace flashgg {
    VBFMVAResult::VBFMVAResult () :
        leadJet                (),
        subleadJet             (),
        subsubleadJet          (),
        leadJet_ptr            (),
        subleadJet_ptr         (),
        subsubleadJet_ptr      (),
        hasValidVBFTriJet      (  0),
        n_rec_jets             ( -1),
        n_gen_jets             ( -1),
        n_diphotons            ( -1),
        dijet_leadEta          ( -9999. ),
        dijet_subleadEta       ( -9999. ),
        dijet_abs_dEta         ( -9999. ),
        dijet_LeadJPt          ( -9999. ),
        dijet_pt          ( -9999. ),
        dijet_SubJPt           ( -9999. ),
        dijet_Zep              ( -9999. ),
        dijet_dphi_trunc       ( -9999. ),
        dijet_dipho_dphi       ( -9999. ),
        dijet_dphi             ( -9999. ),
        dijet_Mjj              ( -9999. ),
        dijet_dy               ( -9999. ),
        dijet_leady            ( -9999. ),
        dijet_subleady         ( -9999. ),
        dijet_dipho_pt         ( -9999. ),
        dijet_minDRJetPho      ( -9999. ),
        dijet_centrality_gg    ( -9999. ),
        dijet_centrality_j3    ( -9999. ),
        dijet_centrality_g     ( -9999. ),
        
        dipho_PToM             ( -9999. ),
        leadPho_PToM           ( -9999. ),
        sublPho_PToM           ( -9999. ),
        
        vbfMvaResult_value     ( -9999. ),
        vbfMvaResult_value_bdt ( -9999. ),
        vbfMvaResult_value_bdtg( -9999. )
    {}
    
    VBFMVAResult::VBFMVAResult( edm::Ptr<VBFMVAResult> x )
    {
        leadJet          = x->leadJet;
        subleadJet       = x->subleadJet;
        
        leadJet_ptr      = x->leadJet_ptr;
        subleadJet_ptr   = x->subleadJet_ptr;
        
        // 3-jets additional variables
        subsubleadJet     = x->subsubleadJet;
        subsubleadJet_ptr = x->subsubleadJet_ptr;
        hasValidVBFTriJet = x->hasValidVBFTriJet;
        
        n_rec_jets       = x->n_rec_jets;
        n_gen_jets       = x->n_gen_jets;
        n_diphotons      = x->n_diphotons;
        
        dijet_leadEta    = x->dijet_leadEta    ;
        dijet_leadEta    = x->dijet_leadEta    ;
        
        dijet_subleadEta = x->dijet_subleadEta ;
        dijet_LeadJPt    = x->dijet_LeadJPt    ;
        dijet_pt         = x->dijet_pt    ;
        dijet_SubJPt     = x->dijet_SubJPt     ; 
        dijet_Zep        = x->dijet_Zep        ;
        
        dijet_dphi_trunc = x->dijet_dphi_trunc ;
        dijet_dipho_dphi = x->dijet_dipho_dphi ;
        dijet_dphi       = x->dijet_dphi ;
        dijet_dipho_pt   = x->dijet_dipho_pt   ;
        dijet_Mjj        = x->dijet_Mjj        ;
        
        dipho_PToM       = x->dipho_PToM     ;
        leadPho_PToM     = x->leadPho_PToM   ;
        sublPho_PToM     = x->sublPho_PToM   ;
        dijet_minDRJetPho= x->dijet_minDRJetPho    ;

        dijet_centrality_gg = x->dijet_centrality_gg ;
        dijet_centrality_g  = x->dijet_centrality_g ;
        dijet_centrality_j3 = x->dijet_centrality_j3 ;
        
        dijet_leady      = x->dijet_leady    ;
        dijet_subleady   = x->dijet_subleady ;
        dijet_dy         = x->dijet_dy;
        
        // VBF MVA results different methods
        // need to be remove at some point ?
        vbfMvaResult_value      = x->vbfMvaResult_value;
        vbfMvaResult_value_bdt  = x->vbfMvaResult_value_bdt;
        vbfMvaResult_value_bdtg = x->vbfMvaResult_value_bdtg;
    }
}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

