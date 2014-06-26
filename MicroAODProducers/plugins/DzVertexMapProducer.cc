
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

namespace flashgg {

  class DzVertexMapProducer : public EDProducer {
    
  public:
    DzVertexMapProducer( const ParameterSet & );
  private:
    void produce( Event &, const EventSetup & ) override;
    EDGetTokenT<View<reco::Vertex> > vertexToken_;
    EDGetTokenT<View<pat::PackedCandidate> > pfcandidateToken_;
    double maxAllowedDz_;
  };

  DzVertexMapProducer::DzVertexMapProducer(const ParameterSet & iConfig) :
    vertexToken_(consumes<View<reco::Vertex> >(iConfig.getUntrackedParameter<InputTag> ("VertexTag", InputTag("offlineSlimmedPrimaryVertices")))),
    pfcandidateToken_(consumes<View<pat::PackedCandidate> >(iConfig.getUntrackedParameter<InputTag> ("PFCandidatesTag", InputTag("packedPFCandidates")))),
    maxAllowedDz_(iConfig.getParameter<double>("MaxAllowedDz")) // in cm
   
  {
    produces<VertexCandidateMap>();
  }

  void DzVertexMapProducer::produce( Event & evt, const EventSetup & ) {
    
    Handle<View<reco::Vertex> > primaryVertices;
    evt.getByToken(vertexToken_,primaryVertices);
    const PtrVector<reco::Vertex>& pvPtrs = primaryVertices->ptrVector();

    Handle<View<pat::PackedCandidate> > pfCandidates;
    evt.getByToken(pfcandidateToken_,pfCandidates);
    const PtrVector<pat::PackedCandidate>& pfPtrs = pfCandidates->ptrVector();

    std::auto_ptr<VertexCandidateMap> assoc(new VertexCandidateMap);

    for (unsigned int i = 0 ; i < pfPtrs.size() ; i++) {
      Ptr<pat::PackedCandidate> cand = pfPtrs[i];
      if (cand->charge() == 0) continue; // skip neutrals
      double closestDz = maxAllowedDz_;
      unsigned int closestDzIndex = -1;
      for (unsigned int j = 0 ; j < pvPtrs.size() ; j++) {
	Ptr<reco::Vertex> vtx = pvPtrs[j];
	double dz = fabs(cand->dz(vtx->position()));
	//	cout << " index_Pf index_Vtx j Dz " << i << " " << j << " " << dz << endl;
	if (dz < closestDz) {
	  closestDz = dz;
	  closestDzIndex = j;
	  //	  cout << "  New closest Dz " << dz << endl;
	}
      }
      if (closestDz < maxAllowedDz_) {
	//	cout << " Final insert index_Pf index_Vtx Dz " << i << " " << closestDzIndex << " " << closestDz << endl;
	Ptr<reco::Vertex> vtx =pvPtrs[closestDzIndex];
	if (assoc->count(vtx)) {
	  assoc->at(vtx).push_back(cand);
	} else {
	  assoc->insert(std::make_pair(vtx,edm::PtrVector<pat::PackedCandidate>()));
	}
      }
    }
    evt.put(assoc);
  }
}

typedef flashgg::DzVertexMapProducer FlashggDzVertexMapProducer;
DEFINE_FWK_MODULE(FlashggDzVertexMapProducer);
