#include "flashgg/DataFormats/interface/DoubleHTag.h"

using namespace flashgg;

DoubleHTag::DoubleHTag() : DiPhotonTagBase::DiPhotonTagBase(), mva_(-2.) 
{  
}

DoubleHTag::~DoubleHTag() {}

DoubleHTag::DoubleHTag( edm::Ptr<flashgg::DiPhotonCandidate> diPho, edm::Ptr<flashgg::Jet> leadJet, edm::Ptr<flashgg::Jet> subleadJet )
    : mva_(-.2), MX_(0.), leadJet_(leadJet), subleadJet_(subleadJet) 
{
    dipho_ = diPho;
    dijet_ = leadJet_->p4() + subleadJet->p4();
    this->setP4( dijet_ + dipho_->p4() );
}

DoubleHTag *DoubleHTag::clone() const
{
    DoubleHTag *result = new DoubleHTag( *this );
    return result;
}

float DoubleHTag::getCosThetaStar_CS(float ebeam) const {
    // cos theta star angle in the Collins Soper frame
    // Copied directly from here: https://github.com/ResonantHbbHgg/Selection/blob/master/selection.h#L3367-L3385
    TLorentzVector p1, p2;
    p1.SetPxPyPzE(0, 0,  ebeam, ebeam);
    p2.SetPxPyPzE(0, 0, -ebeam, ebeam);

    LorentzVector hh_lor = diPhoton()->p4() + dijet();
    TLorentzVector hh;
    hh.SetPxPyPzE(hh_lor.Px(),hh_lor.Py(),hh_lor.Pz(),hh_lor.E()) ;

    TVector3 boost = - hh.BoostVector();
    p1.Boost(boost);
    p2.Boost(boost);
    LorentzVector h1_lor = diPhoton()->p4();
    TLorentzVector h_1;
    h_1.SetPxPyPzE(h1_lor.Px(),h1_lor.Py(),h1_lor.Pz(),h1_lor.E()) ; 
    h_1.Boost(boost);

    TVector3 CSaxis = p1.Vect().Unit() - p2.Vect().Unit();
    CSaxis.Unit();

    return cos(   CSaxis.Angle( h_1.Vect().Unit() )    );
}


// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

