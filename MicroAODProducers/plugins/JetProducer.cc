#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "flashgg/MicroAODFormats/interface/Jet.h"
#include "RecoJets/JetProducers/interface/PileupJetIdAlgo.h"


using namespace std;
using namespace edm;

namespace flashgg {

  class JetProducer : public EDProducer {

  public:
    JetProducer( const ParameterSet & );
  private:
    void produce( Event &, const EventSetup & ) override;
    EDGetTokenT<View<pat::Jet> > photonToken_;
    unique_ptr<PileupJetIdAlgo>  pileupJetIdAlgo_;
  };


  JetProducer::JetProducer(const ParameterSet & iConfig) :
    photonToken_(consumes<View<pat::Jet> >(iConfig.getUntrackedParameter<InputTag> ("JetTag", InputTag("slimmedJets"))))
  {
    pileupJetIdAlgo_.reset(new PileupJetIdAlgo(iConfig));

    produces<vector<flashgg::Jet> >();
  }

  void JetProducer::produce( Event & evt, const EventSetup & ) {
    
    Handle<View<pat::Jet> > photons;
    evt.getByToken(photonToken_,photons);
    const PtrVector<pat::Jet>& photonPointers = photons->ptrVector();
    
    auto_ptr<vector<flashgg::Jet> > photonColl(new vector<flashgg::Jet>);

    for (unsigned int i = 0 ; i < photonPointers.size() ; i++) {
      Ptr<pat::Jet> pp = photonPointers[i];
      flashgg::Jet fg = flashgg::Jet(*pp);
      photonColl->push_back(fg);
    }
    
    evt.put(photonColl);
  }
}

typedef flashgg::JetProducer FlashggJetProducer;
DEFINE_FWK_MODULE(FlashggJetProducer);
