// system include files
#include <memory>
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "flashgg/MicroAODFormats/interface/Muon.h"
#include "DataFormats/VertexReco/interface/Vertex.h" 

using namespace std;
using namespace edm;

namespace flashgg {

	class MuonProducer : public edm::EDProducer {
		public:
			MuonProducer(const edm::ParameterSet&);

		private:
			void produce(edm::Event&, const edm::EventSetup&);

			edm::EDGetTokenT<View<pat::Muon> > muonToken_;
			edm::EDGetTokenT<View<reco::Vertex> > vertexToken_;


	};

	MuonProducer::MuonProducer(const ParameterSet& iConfig): 
		muonToken_(consumes<View<pat::Muon> >(iConfig.getUntrackedParameter<InputTag>("muonTag",InputTag("slimmedMuons")))),
		vertexToken_(consumes<View<reco::Vertex> >(iConfig.getUntrackedParameter<InputTag>("vertexTag",InputTag("offlineSlimmedPrimaryVertices"))))

	{		
	}

	void MuonProducer::produce(Event& evt, const EventSetup& ) {
		Handle<View<pat::Muon> > muons;
		evt.getByToken(muonToken_,muons);
		const PtrVector<pat::Muon> muonPointers = muons->ptrVector();


		Handle<View<reco::Vertex> >  vtxs;
		evt.getByToken(vertexToken_,vtxs);
		const PtrVector<reco::Vertex> vertexPointers = vtxs->ptrVector();

		std::auto_ptr<vector<flashgg::Muon> > muonColl(new vector<flashgg::Muon>);

		for(unsigned int muonIndex = 0; muonIndex < muonPointers.size(); muonIndex++ ){

		 Ptr<pat::Muon> pat_muon = muonPointers[muonIndex];
		 flashgg::Muon flashgg_muon = flashgg::Muon(*pat_muon);

		  int vtxInd = 0;
    		  double dzmin = 9999;

		  for( size_t ivtx = 0 ; ivtx < vertexPointers.size(); ivtx++ ) {

		   Ptr<reco::Vertex> vtx = vertexPointers[ivtx];
    		   if (!pat_muon->innerTrack()) continue;
   	      	   if(fabs(pat_muon->innerTrack()->vz()- vtx->position().z()) < dzmin ) {
 		   dzmin = fabs(pat_muon->innerTrack()->vz() - vtx->position().z());
  		   vtxInd = ivtx;
		   }

	   	 }

		  double muPFIsoSumRel =(pat_muon->pfIsolationR04().sumChargedHadronPt+max(0.,pat_muon->pfIsolationR04().sumNeutralHadronEt+pat_muon->pfIsolationR04().sumPhotonEt-0.5 *pat_muon->pfIsolationR04().sumPUPt))/(pat_muon->pt());
		  flashgg_muon.setMuonVertex(vertexPointers[vtxInd]);
		  flashgg_muon.setMuonIso(muPFIsoSumRel);
		  muonColl->push_back(flashgg_muon);
		}

		evt.put(muonColl);
		}
}

typedef flashgg::MuonProducer FlashggMuonProducer;
DEFINE_FWK_MODULE(FlashggMuonProducer);

