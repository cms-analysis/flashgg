
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "flashgg/DataFormats/interface/GenPhotonExtra.h"
#include "flashgg/DataFormats/interface/VertexCandidateMap.h"
#include "flashgg/MicroAOD/interface/PhotonMCUtils.h"


using namespace std;
using namespace edm;

namespace flashgg {

  class GenPhotonExtraProducer : public EDProducer {

  public:
    GenPhotonExtraProducer( const ParameterSet & );
  private:
    void produce( Event &, const EventSetup & ) override;
    
    EDGetTokenT<View<pat::PackedGenParticle> > genPhotonsToken_;
    EDGetTokenT<View<pat::PackedGenParticle> > genParticlesToken_;
    double isoConeSize_, epsilon0_, n0_;
  };


  GenPhotonExtraProducer::GenPhotonExtraProducer(const ParameterSet & iConfig) :
    genPhotonsToken_(consumes<View<pat::PackedGenParticle> >(iConfig.getParameter<InputTag>("genPhotons"))),
    genParticlesToken_(consumes<View<pat::PackedGenParticle> >(iConfig.getParameter<InputTag>("genParticles"))),
    isoConeSize_(iConfig.getParameter<double>("isoConeSize")),
    epsilon0_(iConfig.getParameter<double>("epsilon0")),
    n0_(iConfig.getParameter<double>("n0"))
  {
    produces<vector<flashgg::GenPhotonExtra> >();
  }

  void GenPhotonExtraProducer::produce( Event & evt, const EventSetup & iSetup) {

    Handle<View<pat::PackedGenParticle> > genPhotons, genParticles;
    evt.getByToken(genPhotonsToken_,genPhotons);
    evt.getByToken(genParticlesToken_,genParticles);

    auto_ptr<vector<flashgg::GenPhotonExtra> > extraColl(new vector<flashgg::GenPhotonExtra>);

   // auto & genPhotonPointers = genPhotons->ptrVector();
   // for (auto & genPho : genPhotons ) {
		for (unsigned int i =0; i< genPhotons->size() ; i++){
	    flashgg::GenPhotonExtra extra(genPhotons->ptrAt(i));
	    extra.setType(PhotonMCUtils::determineMatchType(genPhotons->ptrAt(i)));
	    extra.setGenIso(PhotonMCUtils::isoSum(genPhotons->ptrAt(i), *genParticles, isoConeSize_));
	    extra.setFrixioneIso(PhotonMCUtils::frixioneIso(genPhotons->ptrAt(i), *genParticles, isoConeSize_, epsilon0_, n0_));
	    
	    extraColl->push_back(extra);
    }
    
    evt.put(extraColl);

    /// orig_collection = 0;
  }
}



typedef flashgg::GenPhotonExtraProducer FlashggGenPhotonExtraProducer;
DEFINE_FWK_MODULE(FlashggGenPhotonExtraProducer);
