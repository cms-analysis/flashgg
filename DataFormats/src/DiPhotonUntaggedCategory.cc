#include "flashgg/DataFormats/interface/DiPhotonUntaggedCategory.h"

using namespace flashgg;

DiPhotonUntaggedCategory::DiPhotonUntaggedCategory() : DiPhotonTagBase::DiPhotonTagBase() {}

DiPhotonUntaggedCategory::~DiPhotonUntaggedCategory() {}

DiPhotonUntaggedCategory::DiPhotonUntaggedCategory( edm::Ptr<flashgg::DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvaRes )
    : DiPhotonUntaggedCategory::DiPhotonUntaggedCategory( diPho, *mvaRes ) {}

DiPhotonUntaggedCategory::DiPhotonUntaggedCategory( edm::Ptr<DiPhotonCandidate> dipho, DiPhotonMVAResult mvares ) :  DiPhotonTagBase::DiPhotonTagBase( dipho,
            mvares ) {}

DiPhotonUntaggedCategory *DiPhotonUntaggedCategory::clone() const
{
    DiPhotonUntaggedCategory *result = new DiPhotonUntaggedCategory( diPhoton(), diPhotonMVA() );
    result->setCategoryNumber( categoryNumber() );
    result->setDiPhotonIndex( diPhotonIndex() );
    return result;
}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

