#include "../interface/HadronicActivityProducerExtra.h"

#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "flashgg/DataFormats/interface/GenDiPhoton.h"



namespace flashgg { 

	struct DiPhotonVertexId { size_t operator()(flashgg::DiPhotonCandidate const & obj) { return obj.vertexIndex(); } };
	struct DiPhotonTagVertexId { size_t operator()(flashgg::DiPhotonTagBase const & obj) { return obj.diPhoton()->vertexIndex(); } };
	
}


typedef flashgg::HadronicActivityProducerExtra<edm::View<flashgg::DiPhotonCandidate>,flashgg::DiPhotonVertexId  > FlashggDiPhotonHadronicActivityProducerExtra;
typedef flashgg::HadronicActivityProducerExtra<edm::View<flashgg::DiPhotonTagBase>,flashgg::DiPhotonTagVertexId > FlashggDiPhotonTagHadronicActivityProducerExtra;
typedef flashgg::HadronicActivityProducerExtra<edm::View<flashgg::GenDiPhoton> >                                  FlashggGenHadronicActivityProducerExtra;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(FlashggDiPhotonHadronicActivityProducerExtra);
DEFINE_FWK_MODULE(FlashggDiPhotonTagHadronicActivityProducerExtra);
DEFINE_FWK_MODULE(FlashggGenHadronicActivityProducerExtra);

