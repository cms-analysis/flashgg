#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "flashgg/MicroAOD/interface/VertexSelectorBase.h"
#include "flashgg/DataFormats/interface/DiPhotonMVAResult.h"
#include "flashgg/DataFormats/interface/H4GCandidate.h"
#include "flashgg/DataFormats/interface/TagTruthBase.h"
#include "DataFormats/Common/interface/RefToPtr.h"
#include "DataFormats/Math/interface/deltaR.h"

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
  cc_( consumesCollector() ),
  idSelector_( ParameterSet(), cc_ )

  {}

    //---standard
    H4GCandidateProducer::H4GCandidateProducer( const ParameterSet & pSet):
    photonToken_( consumes<View<Photon> >( pSet.getParameter<InputTag> ( "PhotonTag" ) ) ),
    diphotonToken_( consumes<View<DiPhotonCandidate> >( pSet.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
    vertexToken_( consumes<View<reco::Vertex> >( pSet.getParameter<InputTag> ( "VertexTag" ) ) ),
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

      //---output collection
      std::unique_ptr<vector<H4GCandidate> > H4GColl_( new vector<H4GCandidate> );
      std::vector<edm::Ptr<Photon>> phoVec;



      flashgg::Photon pho1 ;
      flashgg::Photon pho2 ;
      flashgg::Photon pho3 ;
      flashgg::Photon pho4 ;

      edm::Ptr<reco::Vertex> vertex_zero = vertex->ptrAt(0);
      //---at least one diphoton should pass the low mass hgg pre-selection
      bool atLeastOneDiphoPass = false;
      for(auto& dipho : *diphotons.product())
      {
        atLeastOneDiphoPass |= idSelector_(dipho, event);
      }

      int n_photons = photons->size();


      std::vector<edm::Ptr<flashgg::Photon>> phoVector;
      std::vector<flashgg::Photon> photonCollection;
      if (atLeastOneDiphoPass)
      {
        for( int phoIndex = 0; phoIndex < n_photons; phoIndex++ )
        {
          edm::Ptr<flashgg::Photon> pho = photons->ptrAt( phoIndex );
          phoVector.push_back(pho);
        }
        /////////////
        // Photons //
        /////////////

        //Prepare reference collection
        for( unsigned int pIndex = 0; pIndex < phoVector.size(); pIndex++ )
        {
          edm::Ptr<flashgg::Photon> thisPPtr = phoVector[ pIndex ];
          flashgg::Photon * thisPPointer = const_cast<flashgg::Photon *>(thisPPtr.get());
          photonCollection.push_back(*thisPPointer);
        }
        if (n_photons == 2){
          pho1 = photonCollection[0];
          pho2 = photonCollection[1];
          H4GCandidate h4g_2pho(pho1, pho2, vertex_zero);
          H4GColl_->push_back(h4g_2pho);
        }
        else if (n_photons == 3){
          pho1 = photonCollection[0];
          pho2 = photonCollection[1];
          pho3 = photonCollection[2];
          H4GCandidate h4g_3pho(pho1, pho2, pho3, vertex_zero);
          H4GColl_->push_back(h4g_3pho);
        }
        else if (n_photons > 3){
          pho1 = photonCollection[0];
          pho2 = photonCollection[1];
          pho3 = photonCollection[2];
          pho4 = photonCollection[3];
          H4GCandidate h4g_4pho(pho1, pho2, pho3, pho4, vertex_zero);
          H4GColl_->push_back(h4g_4pho);
        }

      }
      event.put( std::move(H4GColl_) );
    }

  }




  typedef flashgg::H4GCandidateProducer FlashggH4GCandidateProducer;
  DEFINE_FWK_MODULE( FlashggH4GCandidateProducer );
