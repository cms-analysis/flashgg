#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "flashgg/MicroAODFormats/interface/Photon.h"
#include "flashgg/MicroAODAlgos/interface/PhotonPreselectorBase.h"



using namespace std;
using namespace edm;

namespace flashgg {

  class PhotonProducer : public EDProducer {

  public:
    PhotonProducer( const ParameterSet & );
  private:
    void produce( Event &, const EventSetup & ) override;
    EDGetTokenT<View<pat::Photon> > photonToken_;
    unique_ptr<PhotonPreselectorBase> photonPreselector_;
  };


  PhotonProducer::PhotonProducer(const ParameterSet & iConfig) :
    photonToken_(consumes<View<pat::Photon> >(iConfig.getUntrackedParameter<InputTag> ("PhotonTag", InputTag("slimmedPhotons"))))
  {
    const std::string& PhotonPreselectorName = iConfig.getParameter<std::string>("PhotonPreselectorName");
    photonPreselector_.reset(FlashggPhotonPreselectorFactory::get()->create(PhotonPreselectorName, iConfig));

    produces<vector<flashgg::Photon> >();
  }

  void PhotonProducer::produce( Event & evt, const EventSetup & ) {
    
    Handle<View<pat::Photon> > photons;
    evt.getByToken(photonToken_,photons);
    const PtrVector<pat::Photon>& photonPointers = photons->ptrVector();
    
    auto_ptr<vector<flashgg::Photon> > photonColl(new vector<flashgg::Photon>);

    for (unsigned int i = 0 ; i < photonPointers.size() ; i++) {
      Ptr<pat::Photon> pp = photonPointers[i];
      flashgg::Photon fg = flashgg::Photon(*pp);
      fg.setTestVariable(i); // The index of the photon is as good an example of distinctive test data as any
      // Apply photon preselection on pat::Photon
      if( photonPreselector_->ispreselected( pp ) )
          photonColl->push_back(fg);
    }
    
    evt.put(photonColl);
  }
}

typedef flashgg::PhotonProducer FlashggPhotonProducer;
DEFINE_FWK_MODULE(FlashggPhotonProducer);
