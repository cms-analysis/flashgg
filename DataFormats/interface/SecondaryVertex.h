#ifndef FLASHgg_SecondaryVertex_h
#define FLASHgg_SecondaryVertex_h

#include "DataFormats/Candidate/interface/VertexCompositePtrCandidate.h"
#include "flashgg/DataFormats/interface/WeightedObject.h"

namespace flashgg {

    class SecondaryVertex : public reco::VertexCompositePtrCandidate, public WeightedObject
    {

    public:
        SecondaryVertex();
        SecondaryVertex( const reco::VertexCompositePtrCandidate & );
        ~SecondaryVertex();

        int svGenFlav() const {return svGenFlav_;}
        void setSvGenFlav( int val ) { svGenFlav_ = val; }

        int svNBHadrons() const {return svNBHadrons_;}
        void setSvNBHadrons( int val ) { svNBHadrons_ = val; }

        int svNCHadrons() const {return svNCHadrons_;}
        void setSvNCHadrons( int val ) { svNCHadrons_ = val; }

        std::vector<float> svTagProbs() const {return svTagProbs_;}
        void setSvTagProbs( std::vector<float> val ) { svTagProbs_ = val; }

        SecondaryVertex *clone() const { return ( new SecondaryVertex( *this ) ); }

    private:
        int svGenFlav_;
        int svNBHadrons_;
        int svNCHadrons_;
        vector<float> svTagProbs_;
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

