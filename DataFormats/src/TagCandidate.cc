#include "flashgg/DataFormats/interface/TagCandidate.h"
#include "flashgg/DataFormats/interface/VBFTag.h"

using namespace flashgg;

TagCandidate::TagCandidate(): tag_(0) {
    vbfMVA_            = -9999.;
    combindedMVA_      = -9999.;
    dijet_leadEta_     = -9999.;
    dijet_subleadEta_  = -9999.;
    dijet_abs_dEta_    = -9999.;
    dijet_LeadJPt_     = -9999.;
    dijet_SubJPt_      = -9999.;
    dijet_Zep_         = -9999.;
    dijet_dphi_trunc_  = -9999.;
    dijet_dipho_dphi_  = -9999.;
    dijet_Mjj_         = -9999.;
    dijet_dy_          = -9999.;
    dijet_leady_       = -9999.;
    dijet_subleady_    = -9999.;
    dijet_dipho_pt_    = -9999.;
    dijet_minDRJetPho_ = -9999.;
    cat_ = 2;
    for (int i=0; i<3; i++)
        triggerBits_[i] = 0;
}

TagCandidate::~TagCandidate() 
{}

TagCandidate::TagCandidate( const flashgg::DiPhotonTagBase* tag, int triggerBits[3] ): tag_(tag) {
    cat_ = 2; 
        
    for (int i=0; i<3; i++)
        triggerBits_[i] = triggerBits[i];

    //const UntaggedTag *untagged = dynamic_cast<const UntaggedTag *>( tag_ );
    const VBFTag *vbf = dynamic_cast<const VBFTag *>( tag_ );
    if( vbf != NULL ) {
        cat_ = 0;
        vbfMVA_            = vbf->VBFMVA().VBFMVAValue();
        combindedMVA_      = vbf->VBFDiPhoDiJetMVA().VBFDiPhoDiJetMVAValue();
        dijet_leadEta_     = vbf->VBFMVA().dijet_leadEta;
        dijet_subleadEta_  = vbf->VBFMVA().dijet_subleadEta;
        dijet_abs_dEta_    = vbf->VBFMVA().dijet_abs_dEta;
        dijet_LeadJPt_     = vbf->VBFMVA().dijet_LeadJPt;
        dijet_SubJPt_      = vbf->VBFMVA().dijet_SubJPt;
        dijet_Zep_         = vbf->VBFMVA().dijet_Zep;
        dijet_dphi_trunc_  = vbf->VBFMVA().dijet_dphi_trunc;
        dijet_dipho_dphi_  = vbf->VBFMVA().dijet_dipho_dphi;
        dijet_Mjj_         = vbf->VBFMVA().dijet_Mjj;
        dijet_dy_          = vbf->VBFMVA().dijet_dy;
        dijet_leady_       = vbf->VBFMVA().dijet_leady;
        dijet_subleady_    = vbf->VBFMVA().dijet_subleady;
        dijet_dipho_pt_    = vbf->VBFMVA().dijet_dipho_pt;
        dijet_minDRJetPho_ = vbf->VBFMVA().dijet_minDRJetPho;
    } else {
        vbfMVA_            = -9999.;
        combindedMVA_      = -9999.;
        dijet_leadEta_     = -9999.;
        dijet_subleadEta_  = -9999.;
        dijet_abs_dEta_    = -9999.;
        dijet_LeadJPt_     = -9999.;
        dijet_SubJPt_      = -9999.;
        dijet_Zep_         = -9999.;
        dijet_dphi_trunc_  = -9999.;
        dijet_dipho_dphi_  = -9999.;
        dijet_Mjj_         = -9999.;
        dijet_dy_          = -9999.;
        dijet_leady_       = -9999.;
        dijet_subleady_    = -9999.;
        dijet_dipho_pt_    = -9999.;
        dijet_minDRJetPho_ = -9999.;
    }     
}

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

