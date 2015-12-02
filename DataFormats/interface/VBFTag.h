#ifndef FLASHgg_VBFTag_h
#define FLASHgg_VBFTag_h


#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "flashgg/DataFormats/interface/VBFDiPhoDiJetMVAResult.h"
#include "flashgg/DataFormats/interface/Jet.h"

namespace flashgg {

    class VBFTag: public DiPhotonTagBase
    {
    public:
        VBFTag();
        ~VBFTag();

        VBFTag *clone() const { return ( new VBFTag( *this ) ); }

        VBFTag( edm::Ptr<DiPhotonCandidate>, edm::Ptr<DiPhotonMVAResult>, edm::Ptr<VBFDiPhoDiJetMVAResult> );
        VBFTag( edm::Ptr<DiPhotonCandidate>, DiPhotonMVAResult, VBFDiPhoDiJetMVAResult );

        const VBFDiPhoDiJetMVAResult VBFDiPhoDiJetMVA() const;
        const VBFMVAResult VBFMVA() const ;
        const Jet leadingJet() const; //needs to be validated
        const Jet subLeadingJet() const; //needs to be validated
        const Jet subSubLeadingJet() const; //needs to be validated // 3rd Jet needed for VBF studies
        
        const edm::Ptr<flashgg::Jet> leadingJet_ptr() const; //needs to be validated
        const edm::Ptr<flashgg::Jet> subLeadingJet_ptr() const; //needs to be validated
        const edm::Ptr<flashgg::Jet> subSubLeadingJet_ptr() const; //needs to be validated // 3rd Jet needed for VBF studies
        const bool hasValidVBFTriJet() const; 
    private:
        VBFDiPhoDiJetMVAResult vbfDiPhoDiJet_mva_result_;
    };

}

#endif
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

