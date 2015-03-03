#ifndef FLASHgg_Muon_h
#define FLASHgg_Muon_h

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

namespace flashgg {

	class Muon : public pat::Muon {

		public:
			Muon();
			Muon(const pat::Muon& );
			~Muon();

			double getMuonIso() const {return muonIso_;}
			void setMuonIso(double val) { muonIso_ = val; }

                        edm::Ptr<reco::Vertex> getMuonVertex() const {return muonVertex_;}
                        void setMuonVertex(edm::Ptr<reco::Vertex> val) { muonVertex_ = val; }

			private:

			double muonIso_;
			edm::Ptr<reco::Vertex> muonVertex_;

	};
}	

#endif
