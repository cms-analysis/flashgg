
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
#include "flashgg/MicroAODFormats/interface/GenPhotonExtra.h"
#include "flashgg/MicroAODFormats/interface/VertexCandidateMap.h"
#include "flashgg/MicroAODAlgos/interface/PhotonIdUtils.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
// #include "HiggsAnalysis/GBRLikelihoodEGTools/interface/EGEnergyCorrectorSemiParm.h"

using namespace std;
using namespace edm;
using reco::PFCandidate;

namespace flashgg {

    struct CaloIsoParams {
	    CaloIsoParams() {};
	    CaloIsoParams(bool overlapRemoval, PFCandidate::ParticleType type, const std::vector<double>& vetos) : 
		    overlapRemoval_(overlapRemoval), type_(type), vetos_(vetos) {};
	    bool overlapRemoval_;
	    PFCandidate::ParticleType type_;
	    std::vector<double> vetos_;
    };

  class PhotonProducer : public EDProducer {

  public:
    PhotonProducer( const ParameterSet & );
  private:
    void produce( Event &, const EventSetup & ) override;
    
    EDGetTokenT<View<pat::Photon> > photonToken_;
    EDGetTokenT<View<pat::PackedCandidate> > pfcandidateToken_;
    EDGetTokenT<View<reco::Vertex> > vertexToken_;   
    EDGetTokenT<flashgg::VertexCandidateMap> vertexCandidateMapToken_;

    EDGetTokenT<vector<flashgg::GenPhotonExtra> > genPhotonToken_;
    double maxGenDeltaR_;
    bool copyExtraGenInfo_;
    
    edm::EDGetTokenT<EcalRecHitCollection> ecalHitEBToken_;
    edm::EDGetTokenT<EcalRecHitCollection> ecalHitEEToken_;
    edm::EDGetTokenT<EcalRecHitCollection> ecalHitESToken_;
    edm::InputTag rhoFixedGrid_;
	  
	  
    PhotonIdUtils phoTools_;
    edm::FileInPath phoIdMVAweightfileEB_, phoIdMVAweightfileEE_;
    //    edm::FileInPath regressionWeightFile_;
    bool useNonZsLazyTools_, recomputeNonZsClusterShapes_;
    
    /// EGEnergyCorrectorSemiParm corV8_;      
    bool doOverlapRemovalForIsolation_, useVtx0ForNeutralIso_;
    std::vector<CaloIsoParams> extraCaloIsolations_;
  };


  PhotonProducer::PhotonProducer(const ParameterSet & iConfig) :
    photonToken_(consumes<View<pat::Photon> >(iConfig.getParameter<InputTag> ("photonTag"))),
    pfcandidateToken_(consumes<View<pat::PackedCandidate> >(iConfig.getParameter<InputTag> ("pfCandidatesTag"))),
    vertexToken_(consumes<View<reco::Vertex> >(iConfig.getParameter<InputTag> ("vertexTag"))),
    vertexCandidateMapToken_(consumes<VertexCandidateMap>(iConfig.getParameter<InputTag>("vertexCandidateMapTag"))),
    genPhotonToken_(mayConsume<vector<flashgg::GenPhotonExtra> >(iConfig.getParameter<InputTag>("genPhotonTag"))),
    ecalHitEBToken_(consumes<EcalRecHitCollection>(iConfig.getParameter<edm::InputTag>("reducedBarrelRecHitCollection"))),
    ecalHitEEToken_(consumes<EcalRecHitCollection>(iConfig.getParameter<edm::InputTag>("reducedEndcapRecHitCollection"))),
    ecalHitESToken_(consumes<EcalRecHitCollection>(iConfig.getParameter<edm::InputTag>("reducedPreshowerRecHitCollection")))
  {

    rhoFixedGrid_  = iConfig.getParameter<edm::InputTag>("rhoFixedGridCollection");

    phoIdMVAweightfileEB_ = iConfig.getParameter<edm::FileInPath>("photonIdMVAweightfile_EB");
    phoIdMVAweightfileEE_ = iConfig.getParameter<edm::FileInPath>("photonIdMVAweightfile_EE");
    phoTools_.setupMVA( phoIdMVAweightfileEB_.fullPath(), phoIdMVAweightfileEE_.fullPath() );

    //    regressionWeightFile_ = iConfig.getParameter<edm::FileInPath>("regressionWeightFile");
    
    doOverlapRemovalForIsolation_ = iConfig.getParameter<bool>("doOverlapRemovalForIsolation");
    useVtx0ForNeutralIso_ = iConfig.getParameter<bool>("useVtx0ForNeutralIso");

    copyExtraGenInfo_ = iConfig.getParameter<bool>("copyExtraGenInfo");
    maxGenDeltaR_ = iConfig.getParameter<double>("maxGenDeltaR");
    
    std::vector<ParameterSet> extraCaloIsolations = iConfig.getParameter<std::vector<ParameterSet> >("extraCaloIsolations");
    for(std::vector<ParameterSet>::iterator it=extraCaloIsolations.begin(); it!=extraCaloIsolations.end(); ++it) {
	    CaloIsoParams p(it->getParameter<bool>("overlapRemoval"),  (PFCandidate::ParticleType)it->getParameter<int>("type"),
			    it->getParameter<std::vector<double> >("vetoRegions") );
	    assert( p.vetos_.size() == 7 );
	    extraCaloIsolations_.push_back(p);
    }
    
    useNonZsLazyTools_ = iConfig.getParameter<bool>("recomputeNonZsClusterShapes");
    recomputeNonZsClusterShapes_ = iConfig.getParameter<bool>("recomputeNonZsClusterShapes");

    produces<vector<flashgg::Photon> >();
  }

  void PhotonProducer::produce( Event & evt, const EventSetup & iSetup) {

    /// if (!corV8_.IsInitialized()) {
    ///   corV8_.Initialize(regressionWeightFile_.fullPath(),8);
    /// }    
    
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
    Handle<vector<flashgg::GenPhotonExtra> > genPhotonsHandle;
    if( ! evt.isRealData() ) {
      evt.getByToken(genPhotonToken_,genPhotonsHandle);
    }
    
    const PtrVector<pat::Photon>& photonPointers = photons->ptrVector();
    const PtrVector<pat::PackedCandidate>& pfcandidatePointers = pfcandidates->ptrVector();
    const PtrVector<reco::Vertex>& vertexPointers = vertices->ptrVector();
    const flashgg::VertexCandidateMap vtxToCandMap = *(vertexCandidateMap.product());    
    const double rhoFixedGrd = *(rhoHandle.product());
    const reco::Vertex * neutVtx = (useVtx0ForNeutralIso_ ? &vertices->at(0) : 0);

    auto_ptr<vector<flashgg::Photon> > photonColl(new vector<flashgg::Photon>);
    
    //// // this is hacky and dangerous
    //// const reco::VertexCollection* orig_collection = static_cast<const reco::VertexCollection*>(vertices->product());

    EcalClusterLazyTools zsLazyTool(evt, iSetup, ecalHitEBToken_, ecalHitEEToken_, ecalHitESToken_);        
    noZS::EcalClusterLazyTools noZsLazyTool(evt, iSetup, ecalHitEBToken_, ecalHitEEToken_, ecalHitESToken_);        

    for (unsigned int i = 0 ; i < photonPointers.size() ; i++) {

      Ptr<pat::Photon> pp = photonPointers[i];
      flashgg::Photon fg = flashgg::Photon(*pp);

      // Gen matching
      if( ! evt.isRealData() ) {
	unsigned int best = INT_MAX;
	float bestptdiff = 99e15;
	const auto & genPhotons = *genPhotonsHandle;
	for (unsigned int j = 0 ; j < genPhotons.size() ; j++) {
	  auto gen = genPhotons[j].ptr();
	  if (gen->pdgId() != 22) continue;
	  float dR = reco::deltaR(*pp,*gen);
	  if (dR > maxGenDeltaR_) continue;
	  float ptdiff = fabs(pp->pt() - gen->pt());
	  if (ptdiff < bestptdiff) {
	    bestptdiff = ptdiff;
	    best = j;
	  }
	}
	if (best < INT_MAX) {
	  auto & extra = genPhotons[best];
	  fg.setMatchedGenPhoton(extra.ptr());
	  fg.setGenMatchType(extra.type());
	  if( copyExtraGenInfo_ ) {
	    extra.copyTo(fg);
	  }
	}
      }
      
      /// double ecor, sigeovere, mean, sigma, alpha1, n1, alpha2, n2, pdfval;
      /// 
      /// corV8_.CorrectedEnergyWithErrorV8(*pp, *orig_collection, *rhoHandle, lazyTool, iSetup,ecor, sigeovere, mean, sigma, alpha1, n1, alpha2, n2, pdfval);
      /// //      printf("V8:  sceta = %5f, default = %5f, corrected = %5f, sigmaE/E = %5f, alpha1 = %5f, n1 = %5f, alpha2 = %5f, n2 = %5f, pdfval = %5f, meancb = %5f, sigmacb = %5f\n", pp->superCluster()->eta(), pp->energy(),ecor,sigeovere,alpha1,n1,alpha2,n2,pdfval,mean,sigma);
      /// 
      /// fg.updateEnergy("regression",ecor);
      /// fg.setSigEOverE(sigeovere);
      
      if( recomputeNonZsClusterShapes_ ) {
	phoTools_.recomputeNonZsClusterShapes(fg,noZsLazyTool);
      }
      if( useNonZsLazyTools_ ) {
	phoTools_.fillExtraClusterShapes(fg,noZsLazyTool);
      } else {
	phoTools_.fillExtraClusterShapes(fg,zsLazyTool);
      }
      
      phoTools_.removeOverlappingCandidates(doOverlapRemovalForIsolation_);
      
      std::map<edm::Ptr<reco::Vertex>,float> isomap04 = phoTools_.pfIsoChgWrtAllVtx(pp, vertexPointers, vtxToCandMap, 0.4, 0.02, 0.02, 0.1);
      std::map<edm::Ptr<reco::Vertex>,float> isomap03 = phoTools_.pfIsoChgWrtAllVtx(pp, vertexPointers, vtxToCandMap, 0.3, 0.02, 0.02, 0.1);
      fg.setpfChgIso04(isomap04);
      fg.setpfChgIso03(isomap03);
      std::map<edm::Ptr<reco::Vertex>,float>& ref_isomap04 = isomap04;
      std::map<edm::Ptr<reco::Vertex>,float>& ref_isomap03 = isomap03;
      float pfChgIsoWrtWorstVtx04 =  phoTools_.pfIsoChgWrtWorstVtx(ref_isomap04);
      float pfChgIsoWrtWorstVtx03 =  phoTools_.pfIsoChgWrtWorstVtx(ref_isomap03);
      fg.setpfChgIsoWrtWorstVtx04(pfChgIsoWrtWorstVtx04);
      fg.setpfChgIsoWrtWorstVtx03(pfChgIsoWrtWorstVtx03);

      // This map is needed for the photon preselection
      std::map<edm::Ptr<reco::Vertex>,float> isomap02 = phoTools_.pfIsoChgWrtAllVtx(pp, vertexPointers, vtxToCandMap, 0.2, 0.02, 0.02, 0.1);
      fg.setpfChgIso02(isomap02);
      fg.setpfChgIsoWrtChosenVtx02( 0. ); // just to initalize things properly, will be setup for real in the diphoton producer once the vertex is chosen
      
      float pfPhoIso04 = phoTools_.pfCaloIso(pp, pfcandidatePointers, 0.4, 0.0, 0.070, 0.015, 0.0, 0.0, 0.0, PFCandidate::gamma, neutVtx);
      float pfPhoIso03 = phoTools_.pfCaloIso(pp, pfcandidatePointers, 0.3, 0.0, 0.070, 0.015, 0.0, 0.0, 0.0, PFCandidate::gamma, neutVtx);
      fg.setpfPhoIso04(pfPhoIso04);
      fg.setpfPhoIso03(pfPhoIso03);

      float pfNeutIso04 = phoTools_.pfCaloIso(pp, pfcandidatePointers, 0.4, 0.0, 0.000, 0.000, 0.0, 0.0, 0.0, PFCandidate::h0, neutVtx);
      float pfNeutIso03 = phoTools_.pfCaloIso(pp, pfcandidatePointers, 0.3, 0.0, 0.000, 0.000, 0.0, 0.0, 0.0, PFCandidate::h0, neutVtx);
      fg.setpfNeutIso04(pfNeutIso04);
      fg.setpfNeutIso03(pfNeutIso03);
      
      std::map<edm::Ptr<reco::Vertex>,float> mvamap = phoTools_.computeMVAWrtAllVtx(fg, vertexPointers,rhoFixedGrd);
      fg.setPhoIdMvaD(mvamap);
      
      // add extra isolations (useful for tuning)
      if( ! extraCaloIsolations_.empty() ) {
	      for(size_t iso=0; iso<extraCaloIsolations_.size(); ++iso) {
		      CaloIsoParams & p = extraCaloIsolations_[iso];		    
		      phoTools_.removeOverlappingCandidates(p.overlapRemoval_);
		      float val = phoTools_.pfCaloIso(pp, pfcandidatePointers, 
						      p.vetos_[0],p.vetos_[1],p.vetos_[2],p.vetos_[3],p.vetos_[4],p.vetos_[5],p.vetos_[6],
						      p.type_, neutVtx);
		      /// cout << "User Isolation " << iso << " " << val << endl;
		      fg.setUserIso( val, iso );
	      }
      }
      phoTools_.removeOverlappingCandidates(doOverlapRemovalForIsolation_);
      
      photonColl->push_back(fg);
    }
    
    evt.put(photonColl);

    /// orig_collection = 0;
  }
}



typedef flashgg::PhotonProducer FlashggPhotonProducer;
DEFINE_FWK_MODULE(FlashggPhotonProducer);
