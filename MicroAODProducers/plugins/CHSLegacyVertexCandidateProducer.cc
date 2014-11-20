
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
#include "DataFormats/Common/interface/RefToPtr.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"


using namespace edm;
using namespace std;

namespace flashgg {

	class CHSLegacyVertexCandidateProducer : public EDProducer 
	{

		public:
			CHSLegacyVertexCandidateProducer( const ParameterSet & );
		private:
			void produce( Event &, const EventSetup & ) override;
			EDGetTokenT<View<reco::Vertex> > vertexToken_;
			EDGetTokenT<View<flashgg::DiPhotonCandidate> > diPhotonsToken_;
			EDGetTokenT<View<pat::PackedCandidate> > pfcandidateToken_;
			EDGetTokenT< VertexCandidateMap > vertexCandidateMapToken_;
			//			double maxAllowedDz_;
			//			bool useEachTrackOnce_;
	};

	CHSLegacyVertexCandidateProducer::CHSLegacyVertexCandidateProducer(const ParameterSet & iConfig) :
		vertexToken_(consumes<View<reco::Vertex> >(iConfig.getUntrackedParameter<InputTag> ("VertexTag", InputTag("offlineSlimmedPrimaryVertices")))),
		diPhotonsToken_(consumes<View<flashgg::DiPhotonCandidate> >(iConfig.getUntrackedParameter<InputTag> ("DiPhotonTag", InputTag("flashggDiPhotons")))),
		pfcandidateToken_(consumes<View<pat::PackedCandidate> >(iConfig.getUntrackedParameter<InputTag> ("PFCandidatesTag", InputTag("packedPFCandidates")))),
		vertexCandidateMapToken_(consumes<VertexCandidateMap>(iConfig.getParameter<InputTag>("VertexCandidateMapTag")))
	{
 produces<vector<pat::PackedCandidate> >();
}


	void CHSLegacyVertexCandidateProducer::produce( Event & evt , const EventSetup & ) 
	{

		// diphotons)
		Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
		evt.getByToken(diPhotonsToken_,diPhotons);
		const PtrVector<flashgg::DiPhotonCandidate>& diPhotonPointers= diPhotons->ptrVector();

		// Primary Vertices in case no diphoton found
		Handle<View<reco::Vertex> > primaryVertices;
		evt.getByToken(vertexToken_,primaryVertices);
		const PtrVector<reco::Vertex>& pvPtrs = primaryVertices->ptrVector();

		// packed cands
		Handle<View<pat::PackedCandidate> > pfCandidates;
		evt.getByToken(pfcandidateToken_,pfCandidates);
		const PtrVector<pat::PackedCandidate>& pfPtrs = pfCandidates->ptrVector();

		Handle<VertexCandidateMap> vtxmap;
		// Handle<View<reco::Vertex> > primaryVertices;
		// // Handle<reco::VertexCollection> primaryVertices;
		evt.getByToken(vertexCandidateMapToken_,vtxmap);


		edm::Ptr<reco::Vertex> flashVertex;
		if (diPhotonPointers.size()==0){
			std::cout << "[WARNING] No Diphotons in event. Using 0th Vtx for CHS PU subtraction."<< std::cout ; //replace with Error Logger at some stage, cout is not thread safe.
			std::cout << "[DEBUG] 0 diphotons" << std::endl;
			flashVertex = pvPtrs[0];
		}
		if (diPhotonPointers.size() ==1){
			flashVertex = diPhotonPointers[0]->getVertex();
			std::cout << "[DEBUG] 1 diphoton" << std::endl;
		}
		if (diPhotonPointers.size() >1){//hopefully very rare
			flashVertex = diPhotonPointers[0]->getVertex();
			
			std::cout << "[DEBUG] >1 diphoton" << std::endl;
			for (unsigned int diPhoLoop = 0; diPhoLoop< diPhotonPointers.size() ; diPhoLoop++){
				//if( diPhotonPointers[diPhoLoop]->getVertex()->position() != flashVertex->position()){
				if( diPhotonPointers[diPhoLoop]->getVertex() != flashVertex){ // we only have a problem if the mutliple diphotons haev different vertices...
					std::cout <<"[WARNING] Multiple Diphotons in event, with different PVs. Using 0th Diphoton Vtx for CHS PU subtraction.";//replace with Error Logger at some stage, cout is not thread safe.
					break;
				}
			}
		}

		std::auto_ptr<vector<pat::PackedCandidate> > result(new vector<pat::PackedCandidate>());


		for(unsigned int pfCandLoop =0 ; pfCandLoop < pfPtrs.size() ; pfCandLoop++)
		{

			std::cout << "[DEBUG] Candidate " << pfCandLoop << " ... "  ;
			if(pfPtrs[pfCandLoop]->charge() ==0){ //keep all neutral objects. 
				std::cout << "is neutral: push back... "<< std::endl;
				assert(!(pfPtrs[pfCandLoop].isNull()));
				result->push_back(*((pfPtrs[pfCandLoop])->clone()));
				Ptr<pat::PackedCandidate> a;
				result->push_back(*a);
				continue;
			}
			std::cout << "is charged ..  ";
			//other wise, if it is charged, want to check if track comes from flashggVertex
			for (std::map<edm::Ptr<reco::Vertex>,edm::PtrVector<pat::PackedCandidate> >::const_iterator vi = vtxmap->begin() ; vi != vtxmap->end() ; vi++) {
				const edm::Ptr<reco::Vertex> currentVertex = (vi->first);

				// the arugment of the if returns 1 if pfPackedCand is in the PtrVector of PackedCandidates corresponding to currentVertex in the Map.
				if (std::count((vtxmap->at(currentVertex).begin()),vtxmap->at(currentVertex).end(),pfPtrs[pfCandLoop])) {

					//Now check if the currentVertex is the same as the legacy PV.
					//Trying to do if (lpv == *currentVertex) gave weird compilation errors, so matchign positions is next best thing.
					//	if (flashVertex->position() == currentVertex->position())
					if (flashVertex == currentVertex)

			std::cout << "RIGHT vertex.. push back  "<< std::endl;
			result->push_back(*((pfPtrs[pfCandLoop])->clone()));
				} else {

			std::cout << " WRONG vertex  "<< std::endl;
				}
			}

			evt.put(result);	

		}
	}
}
typedef flashgg::CHSLegacyVertexCandidateProducer FlashggCHSLegacyVertexCandidateProducer;
DEFINE_FWK_MODULE(FlashggCHSLegacyVertexCandidateProducer);
