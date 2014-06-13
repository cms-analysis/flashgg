#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"

#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"
#include "RecoEgamma/EgammaTools/interface/EcalClusterLocal.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "flashgg/MicroAODFormats/interface/Photon.h"



using namespace std;
using namespace edm;

namespace flashgg {

  class PhotonProducer : public EDProducer {

  public:
    PhotonProducer( const ParameterSet & );
  private:
    void produce( Event &, const EventSetup & ) override;
    EDGetTokenT<View<pat::Photon> > photonToken_;
    edm::InputTag ecalHitEBColl;
    edm::InputTag ecalHitEEColl;
    edm::InputTag ecalHitESColl;
  };


  PhotonProducer::PhotonProducer(const ParameterSet & iConfig) :
    photonToken_(consumes<View<pat::Photon> >(iConfig.getUntrackedParameter<InputTag> ("PhotonTag", InputTag("slimmedPhotons"))))
  {
    ecalHitEBColl = iConfig.getParameter<edm::InputTag>("EcalHitEBColl");
    ecalHitEEColl = iConfig.getParameter<edm::InputTag>("EcalHitEEColl");
    ecalHitESColl = iConfig.getParameter<edm::InputTag>("EcalHitESColl");
    
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
      /*************new shower shape variables added here*********************/
      
      EcalClusterLazyTools lazyTool(Event, EventSetup, ecalHitEBColl, ecalHitEEColl);  
      DetId id= pp->superCluster()->seed()->hitsAndFractions()[0].first;
      const reco::CaloClusterPtr  seed_clu = pp->superCluster()->seed();
      std::vector<float> viCov;
      viCov = lazyTool.localCovariances(*seed_clu);
      std::vector<float> cov;
      cov = lazyTool.covariances(*pp->superCluster());

      //fg.setShowerShapeVariable("sipip",viCov[2]);
      //fg.setShowerShapeVariable("sieip",viCov[1];
      //fg.setShowerShapeVariable("zernike20",slazyTool.zernike20(*seed_clu);
      //fg.setShowerShapeVariable("zernike42",slazyTool.zernike42(*seed_clu);
      fg.setShowerShapeVariable("e2nd",lazyTool.e2nd(*seed_clu));
      fg.setShowerShapeVariable("e2x5right",lazyTool.e2x5Right(*seed_clu));
      fg.setShowerShapeVariable("e2x5left",lazyTool.e2x5Left(*seed_clu));
      fg.setShowerShapeVariable("e2x5top",lazyTool.e2x5Top(*seed_clu));
      fg.setShowerShapeVariable("e2x5bottom",lazyTool.e2x5Bottom(*seed_clu));
      fg.setShowerShapeVariable("e2x5max",lazyTool.e2x5max(*seed_clu));
      fg.setShowerShapeVariable("eright",lazyTool.e2x5Right(*seed_clu));
      fg.setShowerShapeVariable("eleft",lazyTool.e2x5Left(*seed_clu));
      fg.setShowerShapeVariable("etop",lazyTool.e2x5Top(*seed_clu));
      fg.setShowerShapeVariable("ebottom",lazyTool.e2x5Bottom(*seed_clu));
      //fg.setShowerShapeVariable("e1x3",lazyTool.e1x3(*seed_clu));
      
      
      photonColl->push_back(fg);
    }
    
    evt.put(photonColl);
  }
}

typedef flashgg::PhotonProducer FlashggPhotonProducer;
DEFINE_FWK_MODULE(FlashggPhotonProducer);
