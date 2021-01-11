#include "flashgg/DataFormats/interface/GluGluHMVAResult.h"


namespace flashgg {
    GluGluHMVAResult::GluGluHMVAResult () :
        leadJet                (),
        subleadJet             (),
        subsubleadJet          (),
        leadJet_ptr            (),
        subleadJet_ptr         (),
        subsubleadJet_ptr      (),

        n_rec_jets            (-1.),
        dijet_Mjj             (-999.),
        dijet_leadEta         (-999.),
        dijet_subleadEta      (-999.),
        dijet_subsubleadEta   (-999.),
        dijet_leadJPt         (-999.),
        dijet_SubleadJPt      (-999.), 
        dijet_SubsubleadJPt   (-999.), 
        dijet_leadPUMVA       (-999.),
        dijet_subleadPUMVA    (-999.),
        dijet_subsubleadPUMVA (-999.),
        dijet_leadDeltaPhi    (-999.),
        dijet_subleadDeltaPhi (-999.),
        dijet_subsubleadDeltaPhi  (-999.),
        dijet_leadDeltaEta        (-999.),
        dijet_subleadDeltaEta     (-999.),
        dijet_subsubleadDeltaEta  (-999.),

        dipho_leadIDMVA       (-999.),
        dipho_subleadIDMVA    (-999.),
        dipho_lead_ptoM       (-999.),
        dipho_sublead_ptoM    (-999.),
        dipho_leadEta         (-999.),
        dipho_subleadEta      (-999.),
        CosPhi                (-999.),
        vtxprob               (-999.),
        sigmarv               (-999.),
        sigmawv               (-999.),
        diphopt               (-999.),

        ggHMVAResult_prob_0J_PTH_0_10   (-999.),
        ggHMVAResult_prob_0J_PTH_GT10   (-999.),
        ggHMVAResult_prob_1J_PTH_0_60   (-999.),
        ggHMVAResult_prob_1J_PTH_60_120 (-999.),
        ggHMVAResult_prob_1J_PTH_120_200 (-999.),
        ggHMVAResult_prob_GE2J_MJJ_0_350_PTH_0_60 (-999.),
        ggHMVAResult_prob_GE2J_MJJ_0_350_PTH_60_120 (-999.),
        ggHMVAResult_prob_GE2J_MJJ_0_350_PTH_120_200 (-999.),
        ggHMVAResult_prob_PTH_GT200 (-999.)

    {}
    
    GluGluHMVAResult::GluGluHMVAResult( edm::Ptr<GluGluHMVAResult> x )
    {
        leadJet          = x->leadJet;
        subleadJet       = x->subleadJet;
        
        leadJet_ptr      = x->leadJet_ptr;
        subleadJet_ptr   = x->subleadJet_ptr;
        
        n_rec_jets            = x->n_rec_jets;
        dijet_Mjj             = x->dijet_Mjj;
        dijet_leadEta         = x->dijet_leadEta;
        dijet_subleadEta      = x->dijet_subleadEta;
        dijet_subsubleadEta   = x->dijet_subsubleadEta;
        dijet_leadJPt         = x->dijet_leadJPt;
        dijet_SubleadJPt      = x->dijet_SubleadJPt;
        dijet_SubsubleadJPt   = x->dijet_SubsubleadJPt;
        dijet_leadPUMVA       = x->dijet_leadPUMVA;
        dijet_subleadPUMVA    = x->dijet_subleadPUMVA;
        dijet_subsubleadPUMVA = x->dijet_subsubleadPUMVA;
        dijet_leadDeltaPhi    = x->dijet_leadDeltaPhi;
        dijet_subleadDeltaPhi = x->dijet_subleadDeltaPhi;
        dijet_subsubleadDeltaPhi = x->dijet_subsubleadDeltaPhi;
        dijet_leadDeltaEta = x->dijet_leadDeltaEta;      
        dijet_subleadDeltaEta = x->dijet_subleadDeltaEta;
        dijet_subsubleadDeltaEta = x->dijet_subsubleadDeltaEta;

        dipho_leadIDMVA       = x->dipho_leadIDMVA;
        dipho_subleadIDMVA    = x->dipho_subleadIDMVA;
        dipho_lead_ptoM       = x->dipho_lead_ptoM;
        dipho_sublead_ptoM    = x->dipho_sublead_ptoM;
        dipho_leadEta         = x->dipho_leadEta;
        dipho_subleadEta      = x->dipho_subleadEta;
        CosPhi                = x->CosPhi;
        vtxprob               = x->vtxprob;
        sigmarv               = x->sigmarv;
        sigmawv               = x->sigmawv;
        diphopt               = x->diphopt;

        ggHMVAResult_prob_0J_PTH_0_10 = x->ggHMVAResult_prob_0J_PTH_0_10;
        ggHMVAResult_prob_0J_PTH_GT10 = x->ggHMVAResult_prob_0J_PTH_GT10; 
        ggHMVAResult_prob_1J_PTH_0_60 = x->ggHMVAResult_prob_1J_PTH_0_60;
        ggHMVAResult_prob_1J_PTH_60_120 = x->ggHMVAResult_prob_1J_PTH_60_120;
        ggHMVAResult_prob_1J_PTH_120_200 = x->ggHMVAResult_prob_1J_PTH_120_200;
        ggHMVAResult_prob_GE2J_MJJ_0_350_PTH_0_60 = x->ggHMVAResult_prob_GE2J_MJJ_0_350_PTH_0_60;
        ggHMVAResult_prob_GE2J_MJJ_0_350_PTH_60_120 = x->ggHMVAResult_prob_GE2J_MJJ_0_350_PTH_60_120;
        ggHMVAResult_prob_GE2J_MJJ_0_350_PTH_120_200 = x->ggHMVAResult_prob_GE2J_MJJ_0_350_PTH_120_200;
        ggHMVAResult_prob_PTH_GT200 = x->ggHMVAResult_prob_PTH_GT200;
        
        
    }
}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

