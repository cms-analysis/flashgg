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
        TagAndProbeCandidate( edm::Ptr<flashgg::Photon> tag, edm::Ptr<flashgg::Photon> probe)
            {
                tag_ = SinglePhotonView( tag );
                probe_ = SinglePhotonView( probe );
            };

        //---dtor---
        ~TagAndProbeCandidate() {};

        //---getters---
        Photon& getTag() {return tag_.getPhoton();};
        Photon& getProbe() {return probe_.getPhoton();};
        SinglePhotonView& getTagView() {return tag_;};
        SinglePhotonView& getProbeView() {return probe_;};
        
    private:
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

