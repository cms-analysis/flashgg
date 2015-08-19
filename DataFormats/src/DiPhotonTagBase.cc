#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"

using namespace flashgg;

DiPhotonTagBase::DiPhotonTagBase()
{
    category_number_ = -1;
}

DiPhotonTagBase::DiPhotonTagBase( edm::Ptr<flashgg::DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvaRes )
    : DiPhotonTagBase::DiPhotonTagBase( diPho, *mvaRes ) {}

DiPhotonTagBase::DiPhotonTagBase( edm::Ptr<flashgg::DiPhotonCandidate> diPho, DiPhotonMVAResult mvaRes )
{
    mva_result_ = mvaRes;
    category_number_ = -1;
    dipho_ = diPho;
}

DiPhotonTagBase::~DiPhotonTagBase()
{
}


bool DiPhotonTagBase::operator <( const DiPhotonTagBase &b ) const
{
    // For choosing which of two tags OF THE SAME TYPE are of higher priority
    // Comparison of different tags not currently supported - is it ever needed?
    // Overloading may be appropriate if different tags have different priorities

    if( categoryNumber() == b.categoryNumber() ) {
        return ( sumPt() < b.sumPt() );
    } else {
        return ( categoryNumber() < b.categoryNumber() );
    }
}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

