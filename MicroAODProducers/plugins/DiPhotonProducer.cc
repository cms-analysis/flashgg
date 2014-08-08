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
  };

  DiPhotonProducer::DiPhotonProducer(const ParameterSet & iConfig) :
    vertexToken_(consumes<View<reco::Vertex> >(iConfig.getUntrackedParameter<InputTag> ("VertexTag", InputTag("offlineSlimmedPrimaryVertices")))),
    photonToken_(consumes<View<flashgg::Photon> >(iConfig.getUntrackedParameter<InputTag> ("PhotonTag", InputTag("flashggPhotons")))),
    vertexCandidateMapToken_(consumes<VertexCandidateMap>(iConfig.getParameter<InputTag>("VertexCandidateMapTag"))),
    conversionToken_(consumes<View<reco::Conversion> >(iConfig.getUntrackedParameter<InputTag>("ConversionTag",InputTag("reducedConversions")))),
    beamSpotToken_(consumes<View<reco::BeamSpot> >(iConfig.getUntrackedParameter<InputTag>("BeamSpotTag",InputTag("offlineBeamSpot"))))

  {
    const std::string& VertexSelectorName = iConfig.getParameter<std::string>("VertexSelectorName");
    vertexSelector_.reset(FlashggVertexSelectorFactory::get()->create(VertexSelectorName,iConfig));

    produces<vector<flashgg::DiPhotonCandidate> >();
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

    math::XYZPoint vertexPoint;

    Handle<reco::BeamSpot> recoBeamSpotHandle;
    evt.getByToken(beamSpotToken_,recoBeamSpotHandle);
    if (recoBeamSpotHandle.isValid()){
      vertexPoint = recoBeamSpotHandle->position();
    }

    std::cout<<"recoBeamSpotHandle->position().z() = "<<vertexPoint.z()<<std::endl;
    
    auto_ptr<vector<DiPhotonCandidate> > diPhotonColl(new vector<DiPhotonCandidate>);

    for (unsigned int i = 0 ; i < photonPointers.size() ; i++) {
      Ptr<flashgg::Photon> pp1 = photonPointers[i];
      for (unsigned int j = i+1 ; j < photonPointers.size() ; j++) {
        // FIXME
        // Once the vertex is chosen, recompute photon 4-momenta accordingly
    	Ptr<flashgg::Photon> pp2 = photonPointers[j];
	Ptr<reco::Vertex> pvx = vertexSelector_->select(pp1,pp2,pvPointers,*vertexCandidateMap,conversionPointers,vertexPoint);
    	diPhotonColl->push_back(DiPhotonCandidate(pp1,pp2,pvx));                                                                                                                 
      }
    }
    
    evt.put(diPhotonColl);
  }
}

typedef flashgg::DiPhotonProducer FlashggDiPhotonProducer;
DEFINE_FWK_MODULE(FlashggDiPhotonProducer);
