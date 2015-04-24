#include "flashgg/DataFormats/interface/MuMuGammaCandidate.h"
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"

//-----------J. Tao from IHEP-Beijing--------------

using namespace flashgg;

MuMuGammaCandidate::MuMuGammaCandidate() {}

MuMuGammaCandidate::~MuMuGammaCandidate() {}

MuMuGammaCandidate::MuMuGammaCandidate( edm::Ptr<flashgg::DiMuonCandidate> dimuon, edm::Ptr<flashgg::Photon> photon )
{
    addDaughter( *dimuon );
    addDaughter( *photon );

    // Adding momenta
    // Needs its own object - but why?
    // Copied from example
    AddFourMomenta addP4;
    addP4.set( *this );
}

MuMuGammaCandidate::MuMuGammaCandidate( const flashgg::DiMuonCandidate &dimuon, const flashgg::Photon &photon )
{
    addDaughter( dimuon );
    addDaughter( photon );

    // Adding momenta
    // Needs its own object - but why?
    // Copied from example
    AddFourMomenta addP4;
    addP4.set( *this );
}

MuMuGammaCandidate::MuMuGammaCandidate( edm::Ptr<flashgg::DiMuonCandidate> dimuon,  const flashgg::Photon &photon )
{
    addDaughter( *dimuon );
    addDaughter( photon );

    AddFourMomenta addP4;
    addP4.set( *this );
}


const flashgg::DiMuonCandidate *MuMuGammaCandidate::MMG_DiMu() const
{
    return dynamic_cast<const flashgg::DiMuonCandidate *>( daughter( 0 ) );
}

const flashgg::Photon *MuMuGammaCandidate::MMG_Photon() const
{
    return dynamic_cast<const flashgg::Photon *>( daughter( 1 ) );
}

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
