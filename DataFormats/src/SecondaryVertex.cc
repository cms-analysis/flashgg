#include "flashgg/DataFormats/interface/SecondaryVertex.h"

using namespace flashgg;

SecondaryVertex::SecondaryVertex() : reco::VertexCompositePtrCandidate()
{}

SecondaryVertex::~SecondaryVertex()
{}

SecondaryVertex::SecondaryVertex( const reco::VertexCompositePtrCandidate &asv ) : reco::VertexCompositePtrCandidate( asv )
{}

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

