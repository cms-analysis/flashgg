#include "flashgg/DataFormats/interface/VBFMVAResult.h"


namespace flashgg {

    VBFMVAResult::VBFMVAResult() :
        leadJet(),
        subleadJet(),
        dijet_leadEta( -9999. ),
        dijet_subleadEta( -9999. ),
        dijet_abs_dEta( -9999. ),
        dijet_LeadJPt( -9999. ),
        dijet_SubJPt( -9999. ),
        dijet_Zep( -9999. ),
        dijet_dPhi_trunc( -9999. ),
        dijet_Mjj( -9999. ),
        dipho_PToM( -9999. ),
        leadPho_PToM( -9999. ),
        sublPho_PToM( -9999. ),
        vbfMvaResult_value( -9999. ) {}

    VBFMVAResult::VBFMVAResult( edm::Ptr<VBFMVAResult> x )
    {
        leadJet = x->leadJet;
        subleadJet = x->subleadJet;
        dijet_leadEta = x-> dijet_leadEta ;
        dijet_leadEta   = x-> dijet_leadEta    ;
        dijet_subleadEta = x->dijet_subleadEta ;
        dijet_LeadJPt    = x->dijet_LeadJPt ;
        dijet_SubJPt     = x->dijet_SubJPt ;
        dijet_Zep        = x->dijet_Zep ;
        dijet_dPhi_trunc = x->dijet_dPhi_trunc ;
        dijet_Mjj        = x->dijet_Mjj ;
        dipho_PToM       = x->dipho_PToM ;
        leadPho_PToM = x->leadPho_PToM ;
        sublPho_PToM = x->sublPho_PToM ;
        vbfMvaResult_value     = x->vbfMvaResult_value;
    }
}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

