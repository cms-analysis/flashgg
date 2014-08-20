#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "flashgg/MicroAODAlgos/interface/VertexSelectorBase.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "flashgg/MicroAODFormats/interface/VertexCandidateMap.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"


using namespace edm;
using namespace std;

namespace flashgg {

  class DiPhotonProducer : public EDProducer {
    
  public:
    DiPhotonProducer( const ParameterSet & ); 
  private:
    void produce( Event &, const EventSetup & ) override;
    EDGetTokenT<View<reco::Vertex> > vertexToken_;
    EDGetTokenT<View<flashgg::Photon> > photonToken_;
    EDGetTokenT< VertexCandidateMap > vertexCandidateMapToken_;
    unique_ptr<VertexSelectorBase> vertexSelector_;
    EDGetTokenT<View<reco::Conversion> > conversionToken_;
    EDGetTokenT<View<reco::BeamSpot> > beamSpotToken_;
    EDGetTokenT<View<reco::GenParticle> > genParticleToken_;

    double dRexclude;
    double sigma1Pix;
    double sigma1Tib;
    double sigma1Tob;
    double sigma1PixFwd;
    double sigma1Tid;
    double sigma1Tec;
    double sigma2Pix;
    double sigma2Tib;
    double sigma2Tob;
    double sigma2PixFwd;
    double sigma2Tid;
    double sigma2Tec;
    double singlelegsigma1Pix;
    double singlelegsigma1Tib;
    double singlelegsigma1Tob;
    double singlelegsigma1PixFwd;
    double singlelegsigma1Tid;
    double singlelegsigma1Tec;
    double singlelegsigma2Pix;
    double singlelegsigma2Tib;
    double singlelegsigma2Tob;
    double singlelegsigma2PixFwd;
    double singlelegsigma2Tid;
    double singlelegsigma2Tec;    
    std::map<std::string,double> param;
  };

  DiPhotonProducer::DiPhotonProducer(const ParameterSet & iConfig) :
    vertexToken_(consumes<View<reco::Vertex> >(iConfig.getUntrackedParameter<InputTag> ("VertexTag", InputTag("offlineSlimmedPrimaryVertices")))),
    photonToken_(consumes<View<flashgg::Photon> >(iConfig.getUntrackedParameter<InputTag> ("PhotonTag", InputTag("flashggPhotons")))),
    vertexCandidateMapToken_(consumes<VertexCandidateMap>(iConfig.getParameter<InputTag>("VertexCandidateMapTag"))),
    conversionToken_(consumes<View<reco::Conversion> >(iConfig.getUntrackedParameter<InputTag>("ConversionTag",InputTag("reducedConversions")))),
    beamSpotToken_(consumes<View<reco::BeamSpot> >(iConfig.getUntrackedParameter<InputTag>("BeamSpotTag",InputTag("offlineBeamSpot")))),
    genParticleToken_(consumes<View<reco::GenParticle> > (iConfig.getUntrackedParameter<InputTag>("GenParticleTag",InputTag("prunedGenParticles")))){
    
    const std::string& VertexSelectorName = iConfig.getParameter<std::string>("VertexSelectorName");
    vertexSelector_.reset(FlashggVertexSelectorFactory::get()->create(VertexSelectorName,iConfig));
    produces<vector<flashgg::DiPhotonCandidate> >();

    //R around photons
    dRexclude=iConfig.getUntrackedParameter<double>("dRexclude"); param["dRexclude"] = dRexclude;

    //sigmas
    sigma1Pix             =iConfig.getUntrackedParameter<double>("sigma1Pix");   param["sigma1Pix"] = sigma1Pix;
    sigma1Tib		  =iConfig.getUntrackedParameter<double>("sigma1Tib"); param["sigma1Tib"] = sigma1Tib;
    sigma1Tob		  =iConfig.getUntrackedParameter<double>("sigma1Tob"); param["sigma1Tob"] = sigma1Tob;
    sigma1PixFwd          =iConfig.getUntrackedParameter<double>("sigma1PixFwd");   param["sigma1PixFwd"] = sigma1PixFwd;
    sigma1Tid		  =iConfig.getUntrackedParameter<double>("sigma1Tid"); param["sigma1Tid"] = sigma1Tid;
    sigma1Tec		  =iConfig.getUntrackedParameter<double>("sigma1Tec"); param["sigma1Tec"] = sigma1Tec;
    sigma2Pix		  =iConfig.getUntrackedParameter<double>("sigma2Pix"); param["sigma2Pix"] = sigma2Pix;
    sigma2Tib		  =iConfig.getUntrackedParameter<double>("sigma2Tib"); param["sigma2Tib"] = sigma2Tib;
    sigma2Tob		  =iConfig.getUntrackedParameter<double>("sigma2Tob"); param["sigma2Tob"] = sigma2Tob;
    sigma2PixFwd	  =iConfig.getUntrackedParameter<double>("sigma2PixFwd");         param["sigma2PixFwd"] = sigma2PixFwd;
    sigma2Tid		  =iConfig.getUntrackedParameter<double>("sigma2Tid"); param["sigma2Tid"] = sigma2Tid;
    sigma2Tec		  =iConfig.getUntrackedParameter<double>("sigma2Tec"); param["sigma2Tec"] = sigma2Tec;
    singlelegsigma1Pix	  =iConfig.getUntrackedParameter<double>("singlelegsigma1Pix"); param["singlelegsigma1Pix"] = singlelegsigma1Pix;
    singlelegsigma1Tib	  =iConfig.getUntrackedParameter<double>("singlelegsigma1Tib"); param["singlelegsigma1Tib"] = singlelegsigma1Tib;
    singlelegsigma1Tob	  =iConfig.getUntrackedParameter<double>("singlelegsigma1Tob"); param["singlelegsigma1Tob"] = singlelegsigma1Tob;
    singlelegsigma1PixFwd =iConfig.getUntrackedParameter<double>("singlelegsigma1PixFwd");   param["singlelegsigma1PixFwd"] = singlelegsigma1PixFwd;
    singlelegsigma1Tid	  =iConfig.getUntrackedParameter<double>("singlelegsigma1Tid"); param["singlelegsigma1Tid"] = singlelegsigma1Tid;
    singlelegsigma1Tec	  =iConfig.getUntrackedParameter<double>("singlelegsigma1Tec"); param["singlelegsigma1Tec"] = singlelegsigma1Tec;
    singlelegsigma2Pix	  =iConfig.getUntrackedParameter<double>("singlelegsigma2Pix"); param["singlelegsigma2Pix"] = singlelegsigma2Pix;
    singlelegsigma2Tib	  =iConfig.getUntrackedParameter<double>("singlelegsigma2Tib"); param["singlelegsigma2Tib"] = singlelegsigma2Tib;
    singlelegsigma2Tob	  =iConfig.getUntrackedParameter<double>("singlelegsigma2Tob"); param["singlelegsigma2Tob"] = singlelegsigma2Tob;
    singlelegsigma2PixFwd =iConfig.getUntrackedParameter<double>("singlelegsigma2PixFwd");   param["singlelegsigma2PixFwd"] = singlelegsigma2PixFwd; 
    singlelegsigma2Tid    =iConfig.getUntrackedParameter<double>("singlelegsigma2Tid");   param["singlelegsigma2Tid"] = singlelegsigma2Tid;
    singlelegsigma2Tec    =iConfig.getUntrackedParameter<double>("singlelegsigma2Tec");   param["singlelegsigma2Tec"] = singlelegsigma2Tec;

  }

  void DiPhotonProducer::produce( Event & evt, const EventSetup & ) {
    
    Handle<View<reco::Vertex> > primaryVertices;
    evt.getByToken(vertexToken_,primaryVertices);
    const PtrVector<reco::Vertex>& pvPointers = primaryVertices->ptrVector();
    
    Handle<View<flashgg::Photon> > photons;
    evt.getByToken(photonToken_,photons);
    const PtrVector<flashgg::Photon>& photonPointers = photons->ptrVector();

    Handle<VertexCandidateMap> vertexCandidateMap;
    evt.getByToken(vertexCandidateMapToken_,vertexCandidateMap);
    
    Handle<View<reco::Conversion> > conversions; 
    evt.getByToken(conversionToken_,conversions);
    const PtrVector<reco::Conversion>& conversionPointers = conversions->ptrVector();
   
    Handle<reco::BeamSpot> recoBeamSpotHandle;
    evt.getByToken(beamSpotToken_,recoBeamSpotHandle);
    math::XYZPoint vertexPoint;
    if (recoBeamSpotHandle.isValid()){
      vertexPoint = recoBeamSpotHandle->position();
    }

    Handle<View<reco::GenParticle> > genParticles;
    evt.getByToken(genParticleToken_,genParticles);
    const PtrVector<reco::GenParticle>& gens = genParticles->ptrVector();

    
    auto_ptr<vector<DiPhotonCandidate> > diPhotonColl(new vector<DiPhotonCandidate>);

    for (unsigned int i = 0 ; i < photonPointers.size() ; i++) {
      Ptr<flashgg::Photon> pp1 = photonPointers[i];
      for (unsigned int j = i+1 ; j < photonPointers.size() ; j++) {
    	Ptr<flashgg::Photon> pp2 = photonPointers[j];
	Ptr<reco::Vertex> pvx = vertexSelector_->select(pp1,pp2,pvPointers,*vertexCandidateMap,conversionPointers,vertexPoint,param,gens);
    	diPhotonColl->push_back(DiPhotonCandidate(pp1,pp2,pvx));                                                                                                                 
        flashgg::Photon *photon1 = pp1->clone();
        flashgg::Photon *photon2 = pp2->clone();
        // FIXME - recompute photon 4-momenta accordingly (yet to be done)
        // - compute isolations with respect to chosen vertex needed for preselection
        photon1->setpfChgIsoWrtChosenVtx02( photon1->getpfChgIso02WrtVtx( pvx ) );
        photon2->setpfChgIsoWrtChosenVtx02( photon2->getpfChgIso02WrtVtx( pvx ) );
        // store the diphoton into the collection
    	diPhotonColl->push_back(DiPhotonCandidate(*photon1,*photon2,pvx));
      }
    }
    
    evt.put(diPhotonColl);
  }
}

typedef flashgg::DiPhotonProducer FlashggDiPhotonProducer;
DEFINE_FWK_MODULE(FlashggDiPhotonProducer);
