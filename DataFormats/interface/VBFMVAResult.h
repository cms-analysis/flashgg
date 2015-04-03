#ifndef FLASHgg_VBFMVAResult_h
#define FLASHgg_VBFMVAResult_h

#include "DataFormats/Common/interface/Ptr.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/Jet.h"


namespace flashgg {

    class VBFMVAResult
    {

    public:
        VBFMVAResult();
        //  VBFMVAResult(const VBFMVAResult&) = default;  // C++11 only? Should happen automagically anyway
        VBFMVAResult( edm::Ptr<VBFMVAResult> );
        // diJet Info

        Jet leadJet;
        Jet subleadJet;


        // Input variables
        float dijet_leadEta ;
        float dijet_subleadEta;
        float dijet_abs_dEta;
        float dijet_LeadJPt ;
        float dijet_SubJPt;
        float dijet_Zep;
        float dijet_dPhi_trunc;
        float dijet_Mjj;
        float dipho_PToM;
        float leadPho_PToM;
        float sublPho_PToM;

        float VBFMVAValue() const {return vbfMvaResult_value;}

        // Output
        float vbfMvaResult_value;
    };

    typedef std::map<edm::Ptr<DiPhotonCandidate>, VBFMVAResult> VBFMVAResultMap;

}

#endif
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

