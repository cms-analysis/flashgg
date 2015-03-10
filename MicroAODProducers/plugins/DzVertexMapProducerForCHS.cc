#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "flashgg/MicroAODFormats/interface/VertexCandidateMap.h"

using namespace edm;
using namespace std;

// if the track did not attach to any vertex, attach it to ALL vertices!                                                                                                   
// This probably mimics better how PFCHS works
// We want to keep all the tracks for the vertex that's ultimately selected, 
// unless they're clearly close to another vertex

namespace flashgg {

  class DzVertexMapProducerForCHS : public EDProducer {
    
  public:
    DzVertexMapProducerForCHS( const ParameterSet & );
  private:
    void produce( Event &, const EventSetup & ) override;
    EDGetTokenT<View<reco::Vertex> > vertexToken_;
    EDGetTokenT<View<pat::PackedCandidate> > pfcandidateToken_;
    double maxAllowedDz_;
  };

  DzVertexMapProducerForCHS::DzVertexMapProducerForCHS(const ParameterSet & iConfig) :
    vertexToken_(consumes<View<reco::Vertex> >(iConfig.getUntrackedParameter<InputTag> ("VertexTag", InputTag("offlineSlimmedPrimaryVertices")))),
    pfcandidateToken_(consumes<View<pat::PackedCandidate> >(iConfig.getUntrackedParameter<InputTag> ("PFCandidatesTag", InputTag("packedPFCandidates")))),
    maxAllowedDz_(iConfig.getParameter<double>("MaxAllowedDz")) // in cm
  {
    produces<VertexCandidateMap>();
  }

  void DzVertexMapProducerForCHS::produce( Event & evt, const EventSetup & ) {
    
    Handle<View<reco::Vertex> > primaryVertices;
    evt.getByToken(vertexToken_,primaryVertices);
    const PtrVector<reco::Vertex>& pvPtrs = primaryVertices->ptrVector();

    Handle<View<pat::PackedCandidate> > pfCandidates;
    evt.getByToken(pfcandidateToken_,pfCandidates);
    const PtrVector<pat::PackedCandidate>& pfPtrs = pfCandidates->ptrVector();

    std::auto_ptr<VertexCandidateMap> assoc(new VertexCandidateMap);
    
    // Create empty vector for each vertex in advance
    for (unsigned int j = 0 ; j < pvPtrs.size() ; j++) {
      assoc->insert(std::make_pair(pvPtrs[j],edm::PtrVector<pat::PackedCandidate>()));
    }

    for (unsigned int i = 0 ; i < pfPtrs.size() ; i++) {
      Ptr<pat::PackedCandidate> cand = pfPtrs[i];
      if (cand->charge() == 0) continue; // skip neutrals
      double closestDz = maxAllowedDz_;
      unsigned int closestDzIndex = -1;
      for (unsigned int j = 0 ; j < pvPtrs.size() ; j++) {
	Ptr<reco::Vertex> vtx = pvPtrs[j];
	double dz = fabs(cand->dz(vtx->position()));
	if (dz < closestDz) {
	  closestDz = dz;
	  closestDzIndex = j;
	}
      }
      if (closestDz < maxAllowedDz_) {
	// Within specified distance of a vertex, so attach only to it (or the closest one if multiple)
	Ptr<reco::Vertex> vtx =pvPtrs[closestDzIndex];
	assoc->at(vtx).push_back(cand);
      } else { 
	// if the track did not attach to any vertex, attach it to ALL vertices!
	for (unsigned int j = 0 ; j < pvPtrs.size() ; j++) {
	  Ptr<reco::Vertex> vtx = pvPtrs[j];
	  assoc->at(vtx).push_back(cand);
	}
      }
    } // loop over pf 
    evt.put(assoc);
  } // produce method
} // namespace flashgg

typedef flashgg::DzVertexMapProducerForCHS FlashggDzVertexMapProducerForCHS;
DEFINE_FWK_MODULE(FlashggDzVertexMapProducerForCHS);
