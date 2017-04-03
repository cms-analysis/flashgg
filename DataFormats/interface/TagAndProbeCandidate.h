#ifndef FLASHgg_TagAndProbeCandidate_h
#define FLASHgg_TagAndProbeCandidate_h

#include "flashgg/DataFormats/interface/WeightedObject.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "flashgg/DataFormats/interface/SinglePhotonView.h"

namespace flashgg {

    class TagAndProbeCandidate: public WeightedObject
    {
    public:
        //---ctors---
        TagAndProbeCandidate() {};
        TagAndProbeCandidate( float mass, edm::Ptr<flashgg::Photon> tag, edm::Ptr<flashgg::Photon> probe)
            {
                mass_ = mass;
                tag_ = SinglePhotonView( tag );
                probe_ = SinglePhotonView( probe );
            };

        //---dtor---
        ~TagAndProbeCandidate() {};

        //---getters---
        const float mass() const {return mass_;};
        const Photon* getTag() const {return tag_.photon();};
        const Photon* getProbe() const {return probe_.photon();};
        const SinglePhotonView& getTagView() const {return tag_;};
        const SinglePhotonView& getProbeView() const {return probe_;};
        
    private:
        float mass_;
        SinglePhotonView tag_;        
        SinglePhotonView probe_;        
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

