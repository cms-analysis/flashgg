#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "flashgg/MicroAODFormats/interface/Photon.h"
#include "flashgg/MicroAODAlgos/interface/PhotonPreselectorBase.h"
#include "flashgg/MicroAODAlgos/interface/PhotonIdUtils.h"


using namespace std;
using namespace edm;

namespace flashgg {

  class PhotonProducer : public EDProducer {

  public:
    PhotonProducer( const ParameterSet & );
  private:
    void produce( Event &, const EventSetup & ) override;
    
    EDGetTokenT<View<pat::Photon> > photonToken_;
    EDGetTokenT<View<pat::PackedCandidate> > pfcandidateToken_;
    EDGetTokenT<View<reco::Vertex> > vertexToken_;   // CF
    unique_ptr<PhotonPreselectorBase> photonPreselector_;

    edm::InputTag ecalHitEBColl_;
    edm::InputTag ecalHitEEColl_;
    edm::InputTag ecalHitESColl_;

    PhotonIdUtils phoTools_;
  };


  PhotonProducer::PhotonProducer(const ParameterSet & iConfig) :
    photonToken_(consumes<View<pat::Photon> >(iConfig.getUntrackedParameter<InputTag> ("PhotonTag", InputTag("slimmedPhotons")))),
    pfcandidateToken_(consumes<View<pat::PackedCandidate> >(iConfig.getUntrackedParameter<InputTag> ("PFCandidatesTag", InputTag("packedPFCandidates"))))
  {
    const std::string& PhotonPreselectorName = iConfig.getParameter<std::string>("PhotonPreselectorName");
    photonPreselector_.reset(FlashggPhotonPreselectorFactory::get()->create(PhotonPreselectorName, iConfig));

    ecalHitEBColl_ = iConfig.getParameter<edm::InputTag>("reducedBarrelRecHitCollection");
    ecalHitEEColl_ = iConfig.getParameter<edm::InputTag>("reducedEndcapRecHitCollection");
    ecalHitESColl_ = iConfig.getParameter<edm::InputTag>("reducedPreshowerRecHitCollection");

    phoTools_.setupMVA( );

    produces<vector<flashgg::Photon> >();
  }

  void PhotonProducer::produce( Event & evt, const EventSetup & iSetup) {
    
    Handle<View<pat::Photon> > photons;
    evt.getByToken(photonToken_,photons);
    Handle<View<pat::PackedCandidate> > pfcandidates;
    evt.getByToken(pfcandidateToken_,pfcandidates);
    Handle<View<reco::Vertex> > vertices; 
    evt.getByToken(vertexToken_,vertices);

    
    const PtrVector<pat::Photon>& photonPointers = photons->ptrVector();
    const PtrVector<pat::PackedCandidate>& pfcandidatePointers = pfcandidates->ptrVector();
    const PtrVector<reco::Vertex>& vertexPointers = vertices->ptrVector();
    
    auto_ptr<vector<flashgg::Photon> > photonColl(new vector<flashgg::Photon>);

    for (unsigned int i = 0 ; i < photonPointers.size() ; i++) {

      Ptr<pat::Photon> pp = photonPointers[i];
      flashgg::Photon fg = flashgg::Photon(*pp);
     
      if( ! photonPreselector_->ispreselected( pp, pfcandidatePointers ) )
          continue;
      
      EcalClusterLazyTools lazyTool(evt, iSetup, ecalHitEBColl_, ecalHitEEColl_);        
      
      const reco::CaloClusterPtr  seed_clu = pp->superCluster()->seed();
      std::vector<float> viCov;
      viCov = lazyTool.localCovariances(*seed_clu);
      
      fg.setSipip(viCov[2]);
      fg.setSieip(viCov[1]);
      fg.setZernike20(lazyTool.zernike20(*seed_clu));
      fg.setZernike42(lazyTool.zernike42(*seed_clu));
      fg.setE2nd(lazyTool.e2nd(*seed_clu));
      fg.setE2x5right(lazyTool.e2x5Right(*seed_clu));
      fg.setE2x5left(lazyTool.e2x5Left(*seed_clu));
      fg.setE2x5top(lazyTool.e2x5Top(*seed_clu));
      fg.setE2x5bottom(lazyTool.e2x5Bottom(*seed_clu));
      fg.setE2x5max(lazyTool.e2x5Max(*seed_clu));
      fg.setEright(lazyTool.e2x5Right(*seed_clu));
      fg.setEleft(lazyTool.e2x5Left(*seed_clu));
      fg.setEtop(lazyTool.e2x5Top(*seed_clu));
      fg.setEbottom(lazyTool.e2x5Bottom(*seed_clu));
      fg.setE1x3(lazyTool.e1x3(*seed_clu));
      fg.setS4(lazyTool.e2x2(*seed_clu)/pp->e5x5());

      std::map<edm::Ptr<reco::Vertex>,float> isomap = phoTools_.pfIsoChgWrtAllVtx(pp, vertexPointers, pfcandidatePointers, 0.3, 0.02, 0.02, 0.0, 0.2, 0.1);
      fg.setpfChgIso03(isomap);
      
      std::map<edm::Ptr<reco::Vertex>,float> mvamap = phoTools_.computeMVAWrtAllVtx(fg, vertexPointers);
      fg.setPhoIdMvaD(mvamap);

      photonColl->push_back(fg);
    }
    
    evt.put(photonColl);
  }
}



typedef flashgg::PhotonProducer FlashggPhotonProducer;
DEFINE_FWK_MODULE(FlashggPhotonProducer);
