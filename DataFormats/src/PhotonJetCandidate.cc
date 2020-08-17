#include "flashgg/DataFormats/interface/PhotonJetCandidate.h"
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"


using namespace flashgg;

PhotonJetCandidate::PhotonJetCandidate() {}

PhotonJetCandidate::~PhotonJetCandidate() {}

PhotonJetCandidate::PhotonJetCandidate( edm::Ptr<flashgg::Photon> photon, edm::Ptr<pat::Jet> jet, edm::Ptr<reco::Vertex> vertex )
{
    //    std::cout << " PhotonJetCandidate::PhotonJetCandidate " << std::endl;
    vertex_ = vertex;

    addDaughter( *photon );
    addDaughter( *jet    );

    // Adding momenta
    // Needs its own object - but why?
    // Copied from example
    AddFourMomenta addP4;
    addP4.set( *this );
}

const flashgg::Photon *PhotonJetCandidate::photon() const
{
    return dynamic_cast<const flashgg::Photon *>( daughter( 0 ) );
}

const pat::Jet *PhotonJetCandidate::jet() const
{
    return dynamic_cast<const pat::Jet *>( daughter( 1 ) );
}


bool PhotonJetCandidate::operator <( const PhotonJetCandidate &b ) const
{
    return ( sumPt() < b.sumPt() );
}

bool PhotonJetCandidate::operator >( const PhotonJetCandidate &b ) const
{
    return ( sumPt() > b.sumPt() );
}

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

