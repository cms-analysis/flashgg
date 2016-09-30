#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"


using namespace flashgg;

DiPhotonCandidate::DiPhotonCandidate() : genP4_(0) {}

DiPhotonCandidate::~DiPhotonCandidate() 
{
    if( genP4_ != 0 ) { 
        delete genP4_;
    }
}

DiPhotonCandidate::DiPhotonCandidate( edm::Ptr<flashgg::Photon> photon1, edm::Ptr<flashgg::Photon> photon2, edm::Ptr<reco::Vertex> vertex )
    : genP4_(0)
{
    //    std::cout << " DiPhotonCandidate::DiPhotonCandidate " << std::endl;
    vertex_ = vertex;
    viewPho1_ = flashgg::SinglePhotonView( photon1, vertex );
    viewPho2_ = flashgg::SinglePhotonView( photon2, vertex );
    computeP4AndOrder();
}

bool DiPhotonCandidate::operator <( const DiPhotonCandidate &b ) const
{
    return ( sumPt() < b.sumPt() );
}

bool DiPhotonCandidate::operator >( const DiPhotonCandidate &b ) const
{
    return ( sumPt() > b.sumPt() );
}

void DiPhotonCandidate::computeP4AndOrder()
{
    //    std::cout << " START of DiPhotonCandidate::computeP4AndOrder PT M PT1 PT2 " << this->pt() << " " << this->mass() << " " << leadingPhoton()->pt() << " " <<
    //              subLeadingPhoton()->pt() << std::endl;
    if( leadingPhoton()->pt() < subLeadingPhoton()->pt() ) {
        std::swap( viewPho1_, viewPho2_ );
    }
    this->setP4( leadingPhoton()->p4() + subLeadingPhoton()->p4() );
    //    std::cout << " END of DiPhotonCandidate::computeP4AndOrder PT M PT1 PT2 " << this->pt() << " " << this->mass() << " " << leadingPhoton()->pt() << " " <<
    //              subLeadingPhoton()->pt() << std::endl;
}

/*
math::XYZTLorentzVector DiPhotonCandidate::PhoP4Corr( edm::Ptr<flashgg::Photon> photon1 ) const
{
    /// vtx should be the chosen vtx, not primary vtx ///
    float vtx_X = vertex_->x();
    float vtx_Y = vertex_->y();
    float vtx_Z = vertex_->z();

    float sc_X = photon1->superCluster()->x();
    float sc_Y = photon1->superCluster()->y();
    float sc_Z = photon1->superCluster()->z();

    math::XYZVector vtx_Pos( vtx_X, vtx_Y, vtx_Z );
    math::XYZVector sc_Pos( sc_X, sc_Y, sc_Z );

    math::XYZVector direction = sc_Pos - vtx_Pos;
    math::XYZVector p = ( direction.Unit() ) * ( photon1->energy() );
    math::XYZTLorentzVector corrected_p4( p.x(), p.y(), p.z(), photon1->energy() );
    return corrected_p4;
}
*/


const DiPhotonCandidate::LorentzVector& DiPhotonCandidate::genP4() const
{
    // DiPhotonCandidate::LorentzVector ret(0,0,0,0);
    if( genP4_ == 0 ) { 
        std::cout<<"genP4 is 0, so we create it"<<std::endl;
        genP4_ = new DiPhotonCandidate::LorentzVector();
        std::cout<<"genP4"<<genP4_<<std::endl;
        if( leadingPhoton()->hasMatchedGenPhoton() && subLeadingPhoton()->hasMatchedGenPhoton() ) {
            std::cout<<"we have both lead and sublead matched photons, so we read them"<<std::endl;
            std::cout<<"Leading gen: "<<std::endl;
            std::cout<<"	energy: "<<leadingPhoton()->matchedGenPhoton()->p4().energy()<<std::endl;
            std::cout<<"	eta: "<<leadingPhoton()->matchedGenPhoton()->p4().eta()<<std::endl;
            std::cout<<"	phi: "<<leadingPhoton()->matchedGenPhoton()->p4().phi()<<std::endl;
            std::cout<<"	px: "<<leadingPhoton()->matchedGenPhoton()->p4().px()<<std::endl;
            std::cout<<"	py: "<<leadingPhoton()->matchedGenPhoton()->p4().py()<<std::endl;
            std::cout<<"	pz: "<<leadingPhoton()->matchedGenPhoton()->p4().pz()<<std::endl;

            std::cout<<"subLeading gen: "<<std::endl;
            std::cout<<"	energy: "<<subLeadingPhoton()->matchedGenPhoton()->p4().energy()<<std::endl;
            std::cout<<"	eta: "<<subLeadingPhoton()->matchedGenPhoton()->p4().eta()<<std::endl;
            std::cout<<"	phi: "<<subLeadingPhoton()->matchedGenPhoton()->p4().phi()<<std::endl;
            std::cout<<"	px: "<<subLeadingPhoton()->matchedGenPhoton()->p4().px()<<std::endl;
            std::cout<<"	py: "<<subLeadingPhoton()->matchedGenPhoton()->p4().py()<<std::endl;
            std::cout<<"	pz: "<<subLeadingPhoton()->matchedGenPhoton()->p4().pz()<<std::endl;
            *genP4_ = (leadingPhoton()->matchedGenPhoton()->p4() + subLeadingPhoton()->matchedGenPhoton()->p4());

            std::cout<<"Dipho gen: "<<std::endl;
            std::cout<<"	energy: "<<genP4_->energy()<<std::endl;
            std::cout<<"	eta: "<<genP4_->eta()<<std::endl;
            std::cout<<"	phi: "<<genP4_->phi()<<std::endl;
            std::cout<<"	px: "<<genP4_->px()<<std::endl;
            std::cout<<"	py: "<<genP4_->py()<<std::endl;
            std::cout<<"	pz: "<<genP4_->pz()<<std::endl;
        }
        else{
            genP4_ = new DiPhotonCandidate::LorentzVector(1e-6,1e-6,1e-6,3e-6);
        }
    }
    return *genP4_;
}


// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

