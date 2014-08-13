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
#include "flashgg/MicroAODFormats/interface/VertexCandidateMap.h"
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
    EDGetTokenT<View<reco::Vertex> > vertexToken_;   
    EDGetTokenT<flashgg::VertexCandidateMap> vertexCandidateMapToken_;

    edm::InputTag ecalHitEBColl_;
    edm::InputTag ecalHitEEColl_;
    edm::InputTag ecalHitESColl_;
    edm::InputTag rhoFixedGrid_;

    PhotonIdUtils phoTools_;
    string phoIdMVAweightfileEB_, phoIdMVAweightfileEE_;

  };


  PhotonProducer::PhotonProducer(const ParameterSet & iConfig) :
    photonToken_(consumes<View<pat::Photon> >(iConfig.getUntrackedParameter<InputTag> ("PhotonTag", InputTag("slimmedPhotons")))),
    pfcandidateToken_(consumes<View<pat::PackedCandidate> >(iConfig.getUntrackedParameter<InputTag> ("PFCandidatesTag", InputTag("packedPFCandidates")))),
    vertexToken_(consumes<View<reco::Vertex> >(iConfig.getUntrackedParameter<InputTag> ("VertexTag", InputTag("offlineSlimmedPrimaryVertices")))),
    vertexCandidateMapToken_(consumes<VertexCandidateMap>(iConfig.getParameter<InputTag>("VertexCandidateMapTag")))
  {

    ecalHitEBColl_ = iConfig.getParameter<edm::InputTag>("reducedBarrelRecHitCollection");
    ecalHitEEColl_ = iConfig.getParameter<edm::InputTag>("reducedEndcapRecHitCollection");
    ecalHitESColl_ = iConfig.getParameter<edm::InputTag>("reducedPreshowerRecHitCollection");
    rhoFixedGrid_  = iConfig.getParameter<edm::InputTag>("rhoFixedGridCollection");

    phoIdMVAweightfileEB_ = iConfig.getParameter<std::string>("photonIdMVAweightfile_EB");
    phoIdMVAweightfileEE_ = iConfig.getParameter<std::string>("photonIdMVAweightfile_EE");
    phoTools_.setupMVA( phoIdMVAweightfileEB_, phoIdMVAweightfileEE_ );

    produces<vector<flashgg::Photon> >();
  }

  void PhotonProducer::produce( Event & evt, const EventSetup & iSetup) {
    
    Handle<View<pat::Photon> > photons;
    evt.getByToken(photonToken_,photons);
    Handle<View<pat::PackedCandidate> > pfcandidates;
    evt.getByToken(pfcandidateToken_,pfcandidates);
    Handle<View<reco::Vertex> > vertices; 
    evt.getByToken(vertexToken_,vertices);
    Handle<VertexCandidateMap> vertexCandidateMap;
    evt.getByToken(vertexCandidateMapToken_,vertexCandidateMap);
    Handle<double> rhoHandle;        // the old way for now...move to getbytoken?
    evt.getByLabel(rhoFixedGrid_, rhoHandle );

    
    const PtrVector<pat::Photon>& photonPointers = photons->ptrVector();
    const PtrVector<pat::PackedCandidate>& pfcandidatePointers = pfcandidates->ptrVector();
    const PtrVector<reco::Vertex>& vertexPointers = vertices->ptrVector();
    const flashgg::VertexCandidateMap vtxToCandMap = *(vertexCandidateMap.product());    
    const double rhoFixedGrd = *(rhoHandle.product());


    auto_ptr<vector<flashgg::Photon> > photonColl(new vector<flashgg::Photon>);

    for (unsigned int i = 0 ; i < photonPointers.size() ; i++) {

      Ptr<pat::Photon> pp = photonPointers[i];
      flashgg::Photon fg = flashgg::Photon(*pp);
     
      EcalClusterLazyTools lazyTool(evt, iSetup, ecalHitEBColl_, ecalHitEEColl_,ecalHitESColl_);        
      
      const reco::CaloClusterPtr  seed_clu = pp->superCluster()->seed();
      const reco::SuperClusterRef super_clu= pp->superCluster();

      std::vector<float> viCov;
      viCov = lazyTool.localCovariances(*seed_clu);
      
      fg.setSipip(viCov[2]);
      fg.setSieip(viCov[1]);
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
      fg.setESEffSigmaRR(lazyTool.eseffsirir(*super_clu));

      std::map<edm::Ptr<reco::Vertex>,float> isomap03 = phoTools_.pfIsoChgWrtAllVtx(pp, vertexPointers, vtxToCandMap, 0.3, 0.02, 0.02, 0.1);
      fg.setpfChgIso03(isomap03);
      std::map<edm::Ptr<reco::Vertex>,float>& ref_isomap03 = isomap03;
      float pfChgIsoWrtWorstVtx03 =  phoTools_.pfIsoChgWrtWorstVtx(ref_isomap03);
      fg.setpfChgIsoWrtWorstVtx03(pfChgIsoWrtWorstVtx03);

      // This map is needed for the photon preselection
      std::map<edm::Ptr<reco::Vertex>,float> isomap02 = phoTools_.pfIsoChgWrtAllVtx(pp, vertexPointers, vtxToCandMap, 0.2, 0.02, 0.02, 0.1);
      fg.setpfChgIso02(isomap02);
      fg.setpfChgIsoWrtChosenVtx02( 0. ); // just to initalize things properly, will be setup for real in the diphoton producer once the vertex is chosen

      float pfPhoIso03 = phoTools_.pfIsoGamma(pp, pfcandidatePointers, 0.3, 0.0, 0.070, 0.015, 0.0, 0.0, 0.0);
      fg.setpfPhoIso03(pfPhoIso03);

      std::map<edm::Ptr<reco::Vertex>,float> mvamap = phoTools_.computeMVAWrtAllVtx(fg, vertexPointers,rhoFixedGrd);
      fg.setPhoIdMvaD(mvamap);

      photonColl->push_back(fg);
    }
    
    evt.put(photonColl);
  }
}



typedef flashgg::PhotonProducer FlashggPhotonProducer;
DEFINE_FWK_MODULE(FlashggPhotonProducer);
