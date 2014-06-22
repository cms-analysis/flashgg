
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/Common/interface/AssociationMap.h"

using namespace edm;
using namespace std;

namespace flashgg {

  class DzVertexAssociationProducer : public EDProducer {
    
  public:
    DzVertexAssociationProducer( const ParameterSet & );
  private:
    void produce( Event &, const EventSetup & ) override;
    EDGetTokenT<View<reco::Vertex> > vertexToken_;
    EDGetTokenT<View<pat::PackedCandidate> > pfcandidateToken_;
    double maxAllowedDz_;
  };

  DzVertexAssociationProducer::DzVertexAssociationProducer(const ParameterSet & iConfig) :
    vertexToken_(consumes<View<reco::Vertex> >(iConfig.getUntrackedParameter<InputTag> ("VertexTag", InputTag("offlineSlimmedPrimaryVertices")))),
    pfcandidateToken_(consumes<View<pat::PackedCandidate> >(iConfig.getUntrackedParameter<InputTag> ("PFCandidatesTag", InputTag("packedPFCandidates")))),
    maxAllowedDz_(iConfig.getParameter<double>("MaxAllowedDz")) // in cm
   
  {
    produces<edm::AssociationMap<edm::OneToMany<reco::VertexCollection, pat::PackedCandidateCollection> > >();
  }

  void DzVertexAssociationProducer::produce( Event & evt, const EventSetup & ) {
    
    Handle<View<reco::Vertex> > primaryVertices;
    evt.getByToken(vertexToken_,primaryVertices);
    const RefToBaseVector<reco::Vertex>& pvRefs = primaryVertices->refVector();

    Handle<View<pat::PackedCandidate> > pfCandidates;
    evt.getByToken(pfcandidateToken_,pfCandidates);
    const RefToBaseVector<pat::PackedCandidate>& pfRefs = pfCandidates->refVector();

    std::auto_ptr<edm::AssociationMap<edm::OneToMany<reco::VertexCollection, pat::PackedCandidateCollection> > >
      assoc (new edm::AssociationMap<edm::OneToMany<reco::VertexCollection, pat::PackedCandidateCollection> >);

    // I do not really understand what this does - Seth
    // Get a reference before to put in the event
    //    edm::RefProd<pat::PackedCandidateCollection> candRefProd = evt.getRefBeforePut<pat::PackedCandidateCollection>();

    for (unsigned int i = 0 ; i < pfRefs.size() ; i++) {
      double closestDz = maxAllowedDz_;
      unsigned int closestDzIndex = -1;
      Ref<pat::PackedCandidateCollection> cand = pfRefs[i].castTo<edm::Ref<pat::PackedCandidateCollection> >();
      for (unsigned int j = 0 ; j < pvRefs.size() ; j++) {
	Ref<reco::VertexCollection> vtx = pvRefs[j].castTo<edm::Ref<reco::VertexCollection> >();
	double dz = cand->dz(vtx->position());
	cout << " Vtx Pf i j Dz " << i << " " << j << " " << dz << endl;
	if (dz < closestDz) {
	  closestDz = dz;
	  closestDzIndex = j;
	  cout << "  New closest Dz " << dz << endl;
	}
      }
      if (closestDz < maxAllowedDz_) {
	cout << " Final insert i j Dz " << i << " " << closestDzIndex << " " << closestDz << endl;
        Ref<reco::VertexCollection> vtx = pvRefs[closestDzIndex].castTo<edm::Ref<reco::VertexCollection> >();
	//	assoc->insert(vtx,Ref<pat::PackedCandidateCollection>(candRefProd,i));
	assoc->insert(vtx,cand);
      }
    }

    evt.put(assoc);
  }
}

typedef flashgg::DzVertexAssociationProducer FlashggDzVertexAssociationProducer;
DEFINE_FWK_MODULE(FlashggDzVertexAssociationProducer);
