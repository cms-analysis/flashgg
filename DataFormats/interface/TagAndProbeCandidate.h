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
                tag_ = tag;
                probe_ = probe;
            };

        //---dtor---
        ~TagAndProbeCandidate() {};

        //---getters---
        edm::Ptr<flashgg::Photon> getTag() {return tag_;};
        edm::Ptr<flashgg::Photon> getProbe() {return probe_;};
        
    private:
        edm::Ptr<flashgg::Photon> tag_;
        edm::Ptr<flashgg::Photon> probe_;
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

