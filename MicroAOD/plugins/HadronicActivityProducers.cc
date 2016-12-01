#include "../interface/HadronicActivityProducer.h"

#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "flashgg/DataFormats/interface/GenDiPhoton.h"



namespace flashgg { 

	struct DiPhotonVertexId { size_t operator()(flashgg::DiPhotonCandidate const & obj) { return obj.vertexIndex(); } };
	struct DiPhotonTagVertexId { size_t operator()(flashgg::DiPhotonTagBase const & obj) { return obj.diPhoton()->vertexIndex(); } };
	
}


typedef flashgg::HadronicActivityProducer<edm::View<flashgg::DiPhotonCandidate>,flashgg::DiPhotonVertexId  > FlashggDiPhotonHadronicActivityProducer;
typedef flashgg::HadronicActivityProducer<edm::View<flashgg::DiPhotonTagBase>,flashgg::DiPhotonTagVertexId > FlashggDiPhotonTagHadronicActivityProducer;
typedef flashgg::HadronicActivityProducer<edm::View<flashgg::GenDiPhoton> >                                  FlashggGenHadronicActivityProducer;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(FlashggDiPhotonHadronicActivityProducer);
DEFINE_FWK_MODULE(FlashggDiPhotonTagHadronicActivityProducer);
DEFINE_FWK_MODULE(FlashggGenHadronicActivityProducer);

