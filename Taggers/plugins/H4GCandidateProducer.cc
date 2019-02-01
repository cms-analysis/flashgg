#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/SinglePhotonView.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "flashgg/MicroAOD/interface/VertexSelectorBase.h"
#include "flashgg/DataFormats/interface/DiPhotonMVAResult.h"
#include "flashgg/DataFormats/interface/H4GCandidate.h"
#include "flashgg/DataFormats/interface/TagTruthBase.h"
#include "DataFormats/Common/interface/RefToPtr.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "flashgg/MicroAOD/interface/CutBasedDiPhotonObjectSelector.h"

#include <vector>
#include <algorithm>
#include "TGraph.h"
#include "TLorentzVector.h"


using namespace std;
using namespace edm;

namespace flashgg {

  class H4GCandidateProducer : public EDProducer
  {
  public:
    //---typedef
    typedef math::XYZTLorentzVector LorentzVector;

    //---ctors
    H4GCandidateProducer();
    H4GCandidateProducer( const ParameterSet & );
  private:
    void produce( Event &, const EventSetup & ) override;

    EDGetTokenT<View<Photon> > photonToken_;
    Handle<View<flashgg::Photon> > photons;

    EDGetTokenT<View<DiPhotonCandidate> > diphotonToken_;
    Handle<View<flashgg::DiPhotonCandidate> > diphotons;

    EDGetTokenT<View<reco::Vertex> > vertexToken_;
    Handle<View<reco::Vertex> > vertex;

    EDGetTokenT<View<reco::GenParticle> > genParticleToken_;
    Handle<View<reco::GenParticle> > genParticle;

    //---ID selector
    ConsumesCollector cc_;
    CutBasedDiPhotonObjectSelector idSelector_;

    //----output collection
    auto_ptr<vector<H4GCandidate> > H4GColl_;

  };

  //---constructors
  H4GCandidateProducer::H4GCandidateProducer( ):
  photonToken_(),
  diphotonToken_(),
  genParticleToken_(),
  cc_( consumesCollector() ),
  idSelector_( ParameterSet(), cc_ )

  {}

    //---standard
    H4GCandidateProducer::H4GCandidateProducer( const ParameterSet & pSet):
    photonToken_( consumes<View<Photon> >( pSet.getParameter<InputTag> ( "PhotonTag" ) ) ),
    diphotonToken_( consumes<View<DiPhotonCandidate> >( pSet.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
    vertexToken_( consumes<View<reco::Vertex> >( pSet.getParameter<InputTag> ( "VertexTag" ) ) ),
    genParticleToken_( consumes<View<reco::GenParticle> >( pSet.getParameter<InputTag> ( "GenParticleTag" ) ) ),

    cc_( consumesCollector() ),
    idSelector_( pSet.getParameter<ParameterSet> ( "idSelection" ), cc_ )

    {
      produces<vector<H4GCandidate> > ();
    }

    void H4GCandidateProducer::produce( Event &event, const EventSetup & )
    {
      event.getByToken( photonToken_, photons );
      event.getByToken( diphotonToken_, diphotons );
      event.getByToken( vertexToken_, vertex );
      event.getByToken( genParticleToken_, genParticle );

      //---output collection
      std::unique_ptr<vector<H4GCandidate> > H4GColl_( new vector<H4GCandidate> );

      edm::Ptr<reco::Vertex> vertex_zero = vertex->ptrAt(0);
      reco::GenParticle::Point genVertex;

      //---at least one diphoton should pass the low mass hgg pre-selection
      bool atLeastOneDiphoPass = false;
      std::vector<const flashgg::Photon*> phosTemp;
      for( unsigned int dpIndex = 0; dpIndex < diphotons->size(); dpIndex++ )
      {
        edm::Ptr<flashgg::DiPhotonCandidate> thisDPPtr = diphotons->ptrAt( dpIndex );
        flashgg::DiPhotonCandidate * thisDPPointer = const_cast<flashgg::DiPhotonCandidate *>(thisDPPtr.get());
        atLeastOneDiphoPass |= idSelector_(*thisDPPointer, event);
      }
      int n_photons = photons->size();
      std::vector<flashgg::Photon> phoVector;
      if (atLeastOneDiphoPass)
      {
        for( int phoIndex = 0; phoIndex < n_photons; phoIndex++ )
        {
          edm::Ptr<flashgg::Photon> pho = photons->ptrAt( phoIndex );
          flashgg::Photon * thisPPointer = const_cast<flashgg::Photon *>(pho.get());
          phoVector.push_back(*thisPPointer);
        }
        if (! event.isRealData() )
        {
          for( auto &part : *genParticle ) {
            if( part.pdgId() != 2212 || part.vertex().z() != 0. ) 
            {
              genVertex = part.vertex();
            }
        }
      }
        H4GCandidate h4g(phoVector, vertex_zero, genVertex);
        H4GColl_->push_back(h4g);
      }
      event.put( std::move(H4GColl_) );
    }
  }
  typedef flashgg::H4GCandidateProducer FlashggH4GCandidateProducer;
  DEFINE_FWK_MODULE( FlashggH4GCandidateProducer );
