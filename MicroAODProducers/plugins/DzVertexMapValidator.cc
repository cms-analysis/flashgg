
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

  class DzVertexMapValidator : public EDProducer {
    
  public:
    DzVertexMapValidator( const ParameterSet & );
  private:
    void produce( Event &, const EventSetup & ) override;
    EDGetTokenT<View<reco::Vertex> > vertexToken_;
    EDGetTokenT<View<reco::Vertex> > vertexTokenAOD_;
    EDGetTokenT<View<pat::PackedCandidate> > pfcandidateToken_;
    double maxAllowedDz_;
    bool useEachTrackOnce_;
  };

  DzVertexMapValidator::DzVertexMapValidator(const ParameterSet & iConfig) :
    vertexToken_(consumes<View<reco::Vertex> >(iConfig.getUntrackedParameter<InputTag> ("VertexTag", InputTag("offlineSlimmedPrimaryVertices")))),
    vertexTokenAOD_(consumes<View<reco::Vertex> >(iConfig.getUntrackedParameter<InputTag> ("VertexTagAOD", InputTag("offlinePrimaryVertices")))),
    pfcandidateToken_(consumes<View<pat::PackedCandidate> >(iConfig.getUntrackedParameter<InputTag> ("PFCandidatesTag", InputTag("packedPFCandidates")))),
    maxAllowedDz_(iConfig.getParameter<double>("MaxAllowedDz")), // in cm
    useEachTrackOnce_(iConfig.getUntrackedParameter<bool>("UseEachTrackOnce",true))
  {
    produces<VertexCandidateMap>();
  }

  void DzVertexMapValidator::produce( Event & evt , const EventSetup & ) {
    
		Handle<View<reco::Vertex> > primaryVerticesAOD;
    evt.getByToken(vertexTokenAOD_,primaryVerticesAOD);
    const PtrVector<reco::Vertex>& pvPtrsAOD = primaryVerticesAOD->ptrVector();
		
		Handle<View<reco::Vertex> > primaryVertices;
    evt.getByToken(vertexToken_,primaryVertices);
    const PtrVector<reco::Vertex>& pvPtrs = primaryVertices->ptrVector();

    Handle<View<pat::PackedCandidate> > pfCandidates;
    evt.getByToken(pfcandidateToken_,pfCandidates);
    const PtrVector<pat::PackedCandidate>& pfPtrs = pfCandidates->ptrVector();
		
		int trackNumber =0;
		for ( unsigned int i=0 ; i < pvPtrsAOD.size() ;i++)
		{

		int nTracks    = pvPtrsAOD[i]->nTracks()   ;
		int tracksSize = pvPtrsAOD[i]->tracksSize();
		std:: cout <<  nTracks   << std::endl;
		std:: cout <<  tracksSize<< std::endl;
		trackNumber = trackNumber + pvPtrsAOD[i]->tracksSize();

//		for(pvPtrs[i]->tracks_begin(); ! pvPtrs[i]->tracks_end(); )
		}
    

    std::auto_ptr<VertexCandidateMap> assoc(new VertexCandidateMap);

		std::cout << "Hello, world | AOD vertices:  "<< pvPtrsAOD.size() << ", miniAOD vtxs: " << pvPtrs.size() << ", AOD tracks: " << trackNumber <<", miniAOD tracks: " << pfPtrs.size() <<  std::endl;




 /*   if (useEachTrackOnce_) {
      // Associate a track to the closest vertex only, and only if dz < maxAllowedDz_
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
    } else // i.e. if !useEachTrackOnce_ 
		{
      // Allow a track to be associated to multiple vertices if it's close to each of them
      for (unsigned int i = 0 ; i < pfPtrs.size() ; i++) {
	Ptr<pat::PackedCandidate> cand = pfPtrs[i];
	if (cand->charge() == 0) continue; // skip neutrals
	for (unsigned int j = 0 ; j < pvPtrs.size() ; j++) {
	  Ptr<reco::Vertex> vtx = pvPtrs[j];
	  double dz = fabs(cand->dz(vtx->position()));
	  if (dz < maxAllowedDz_) {
	    if (assoc->count(vtx)) {
	      assoc->at(vtx).push_back(cand);
	    } else {
	      assoc->insert(std::make_pair(vtx,edm::PtrVector<pat::PackedCandidate>()));
	    }
	  }
	}
      }
    } // end of !useEachTrackOnce_
*/
//    evt.put(assoc);
  }
}

typedef flashgg::DzVertexMapValidator FlashggDzVertexMapValidator;
DEFINE_FWK_MODULE(FlashggDzVertexMapValidator);
