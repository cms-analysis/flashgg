#include "flashgg/DataFormats/interface/EleEleGammaCandidate.h"
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"

using namespace flashgg;

EleEleGammaCandidate::EleEleGammaCandidate() {}

EleEleGammaCandidate::~EleEleGammaCandidate() {}

EleEleGammaCandidate::EleEleGammaCandidate( edm::Ptr<flashgg::DiElectronCandidate> dielectron, edm::Ptr<flashgg::Photon> photon, edm::Ptr<reco::Vertex> vertex )
{
    addDaughter( *dielectron );
    addDaughter( *photon );
    vertex_ = vertex;
    dieleptr_ = dielectron;
    // Adding momenta
    // Needs its own object - but why?
    // Copied from example
    AddFourMomenta addP4;
    addP4.set( *this );
}

EleEleGammaCandidate::EleEleGammaCandidate( edm::Ptr<flashgg::DiElectronCandidate> dielectron,  const flashgg::Photon &photon, edm::Ptr<reco::Vertex> vertex )
{
    addDaughter( *dielectron );
    addDaughter( photon );
    vertex_ = vertex;
    dieleptr_ = dielectron;

    AddFourMomenta addP4;
    addP4.set( *this );
}


const flashgg::DiElectronCandidate *EleEleGammaCandidate::EEG_DiEle() const
{
    return dynamic_cast<const flashgg::DiElectronCandidate *>( daughter( 0 ) );
}


const flashgg::Photon *EleEleGammaCandidate::EEG_Photon() const
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
