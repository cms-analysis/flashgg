#include "flashgg/DataFormats/interface/THQLeptonicTag.h"
#include <algorithm>

using namespace flashgg;

THQLeptonicTag::THQLeptonicTag() : DiPhotonTagBase::DiPhotonTagBase()
{}

THQLeptonicTag::~THQLeptonicTag()
{}

// N.B. Other attributes are set using methods in header file
THQLeptonicTag::THQLeptonicTag( edm::Ptr<DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvares ) : THQLeptonicTag::THQLeptonicTag( diPho, *mvares ) {}
THQLeptonicTag::THQLeptonicTag( edm::Ptr<DiPhotonCandidate> dipho, DiPhotonMVAResult mvares ) : DiPhotonTagBase::DiPhotonTagBase( dipho, mvares ) {}
