#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "flashgg/DataFormats/interface/SinglePhotonView.h"
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"

using namespace flashgg;

DiPhotonCandidate::DiPhotonCandidate() {}

DiPhotonCandidate::~DiPhotonCandidate() {}

DiPhotonCandidate::DiPhotonCandidate( edm::Ptr<flashgg::Photon> photon1, edm::Ptr<flashgg::Photon> photon2, edm::Ptr<reco::Vertex> vertex )
{
    addDaughter( *photon1 );
    addDaughter( *photon2 );
    vertex_ = vertex;

    // Adding momenta
    // Needs its own object - but why?
    // Copied from example
    AddFourMomenta addP4;
    addP4.set( *this );
}

DiPhotonCandidate::DiPhotonCandidate( const flashgg::Photon &photon1, const flashgg::Photon &photon2, edm::Ptr<reco::Vertex> vertex )
{
    addDaughter( photon1 );
    addDaughter( photon2 );
    vertex_ = vertex;

    // Adding momenta
    // Needs its own object - but why?
    // Copied from example
    AddFourMomenta addP4;
    addP4.set( *this );
}

flashgg::Photon &DiPhotonCandidate::getLeadingPhoton()
{
    if( daughter( 0 )->pt() > daughter( 1 )->pt() ) {
        return dynamic_cast<flashgg::Photon &>( *daughter( 0 ) );
    } else {
        return dynamic_cast<flashgg::Photon &>( *daughter( 1 ) );
    }
}

flashgg::Photon &DiPhotonCandidate::getSubLeadingPhoton()
{
    if( daughter( 0 )->pt() > daughter( 1 )->pt() ) {
        return dynamic_cast<flashgg::Photon &>( *daughter( 1 ) );
    } else {
        return dynamic_cast<flashgg::Photon &>( *daughter( 0 ) );
    }
}

const flashgg::Photon *DiPhotonCandidate::leadingPhoton() const
{
    if( daughter( 0 )->pt() > daughter( 1 )->pt() ) {
        return dynamic_cast<const flashgg::Photon *>( daughter( 0 ) );
    } else {
        return dynamic_cast<const flashgg::Photon *>( daughter( 1 ) );
    }
}

const flashgg::Photon *DiPhotonCandidate::subLeadingPhoton() const
{
    if( daughter( 0 )->pt() > daughter( 1 )->pt() ) {
        return dynamic_cast<const flashgg::Photon *>( daughter( 1 ) );
    } else {
        return dynamic_cast<const flashgg::Photon *>( daughter( 0 ) );
    }
}

SinglePhotonView DiPhotonCandidate::leadingView() const
{
    if( daughter( 0 )->pt() > daughter( 1 )->pt() ) {
        return SinglePhotonView( this, 0 );
    } else {
        return SinglePhotonView( this, 1 );
    }
}

SinglePhotonView DiPhotonCandidate::subLeadingView() const
{
    if( daughter( 0 )->pt() > daughter( 1 )->pt() ) {
        return SinglePhotonView( this, 1 );
    } else {
        return SinglePhotonView( this, 0 );
    }
}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

