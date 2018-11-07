#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

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
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "flashgg/MicroAOD/interface/CutBasedDiPhotonObjectSelector.h"

#include <vector>
#include <algorithm>
#include "TGraph.h"
#include "TLorentzVector.h"
#include "TTree.h"


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

    // //Out tree elements:
    edm::Service<TFileService> fs;
    // TH1F* cutFlow;
    // TH1F* presel_pho;
    // TH1F* pho_presel;
    // TH1F* phoHisto;


  private:
    // edm::Service<TFileService> fs_;
    // double genTotalWeight;

    void produce( Event &, const EventSetup & ) override;

    EDGetTokenT<View<Photon> > photonToken_;
    Handle<View<flashgg::Photon> > photons;

    EDGetTokenT<View<DiPhotonCandidate> > diphotonToken_;
    Handle<View<flashgg::DiPhotonCandidate> > diphotons;

    EDGetTokenT<View<reco::Vertex> > vertexToken_;
    Handle<View<reco::Vertex> > vertex;

    EDGetTokenT<View<reco::GenParticle> > genParticleToken_;
    Handle<View<reco::GenParticle> > genParticle;

    EDGetTokenT<reco::BeamSpot>  beamSpotToken_;
    Handle<reco::BeamSpot>  recoBeamSpotHandle;

    //---ID selector
    ConsumesCollector cc_;
    CutBasedDiPhotonObjectSelector idSelector_;

    //----output collection
    auto_ptr<vector<H4GCandidate> > H4GColl_;

    // //--for cut FLow
    // edm::InputTag genInfo_;
    // edm::EDGetTokenT<GenEventInfoProduct> genInfoToken_;

  };
  //---constructors
  H4GCandidateProducer::H4GCandidateProducer( ):
  photonToken_(),
  diphotonToken_(),
  genParticleToken_(),
  beamSpotToken_(),
  cc_( consumesCollector() ),
  idSelector_( ParameterSet(), cc_ )
  {}
    //---standard
    H4GCandidateProducer::H4GCandidateProducer( const ParameterSet & pSet):
    photonToken_( consumes<View<Photon> >( pSet.getParameter<InputTag> ( "PhotonTag" ) ) ),
    diphotonToken_( consumes<View<DiPhotonCandidate> >( pSet.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
    vertexToken_( consumes<View<reco::Vertex> >( pSet.getParameter<InputTag> ( "VertexTag" ) ) ),
    genParticleToken_( consumes<View<reco::GenParticle> >( pSet.getParameter<InputTag> ( "GenParticleTag" ) ) ),
    beamSpotToken_( consumes<reco::BeamSpot> ( pSet.getParameter<InputTag> ( "beamSpotTag" ) ) ),
    cc_( consumesCollector() ),
    idSelector_( pSet.getParameter<ParameterSet> ( "idSelection" ), cc_ )

    {
      // genInfo_ = pSet.getUntrackedParameter<edm::InputTag>( "genInfo", edm::InputTag("generator") );
      // genInfoToken_ = consumes<GenEventInfoProduct>( genInfo_ );
      // cutFlow = fs->make<TH1F> ("cutFlow","Cut FLow",10,0,10);
      // presel_pho = fs->make<TH1F> ("presel_pho","",10,0,10);
      // pho_presel = fs->make<TH1F> ("pho_presel","",10,0,10);
      // phoHisto = fs->make<TH1F>("N_photons","N_photons",15,0.,15);

      produces<vector<H4GCandidate> > ();
    }
    void H4GCandidateProducer::produce( Event &event, const EventSetup & )
    {
      event.getByToken( photonToken_, photons );
      event.getByToken( diphotonToken_, diphotons );
      event.getByToken( vertexToken_, vertex );
      event.getByToken( genParticleToken_, genParticle );
      event.getByToken( beamSpotToken_, recoBeamSpotHandle );

      math::XYZPoint BSPoint;
      if( recoBeamSpotHandle.isValid() ) {
        BSPoint = recoBeamSpotHandle->position();
      }

      //---output collection
      std::unique_ptr<vector<H4GCandidate> > H4GColl_( new vector<H4GCandidate> );

      std::vector <edm::Ptr<reco::Vertex> > Vertices; // Collection of vertices
      for( int v = 0; v < (int) vertex->size(); v++ )
      {
        edm::Ptr<reco::Vertex> vtx = vertex->ptrAt( v );
        Vertices.push_back(vtx);
      }
      reco::GenParticle::Point genVertex;
      edm::Ptr<reco::Vertex> vertex_diphoton;
      //---at least one diphoton should pass the low mass hgg pre-selection
      bool atLeastOneDiphoPass = false;
      std::vector<const flashgg::Photon*> phosTemp;
      std::vector <edm::Ptr<flashgg::DiPhotonCandidate>> diPhoPtrs;
      for( unsigned int dpIndex = 0; dpIndex < diphotons->size(); dpIndex++ )
      {
        edm::Ptr<flashgg::DiPhotonCandidate> thisDPPtr = diphotons->ptrAt( dpIndex );
        vertex_diphoton = diphotons->ptrAt( dpIndex )->vtx();
        flashgg::DiPhotonCandidate * thisDPPointer = const_cast<flashgg::DiPhotonCandidate *>(thisDPPtr.get());
        atLeastOneDiphoPass |= idSelector_(*thisDPPointer, event);
      }
      int n_photons = -999;

      // Handle<GenEventInfoProduct> genInfo;
      // if( ! event.isRealData() ) {
      //   event.getByToken(genInfoToken_, genInfo);
      //   genTotalWeight = genInfo->weight();
      // } else {
      //   genTotalWeight = 1;
      // }
      n_photons = photons->size();
      // cutFlow->Fill(0.0,genTotalWeight);
      // presel_pho->Fill(0.0,genTotalWeight);
      // pho_presel->Fill(0.0,genTotalWeight);
      //
      // phoHisto->Fill(n_photons);

      // if(atLeastOneDiphoPass)
      // {
      //   presel_pho->Fill(1.0,genTotalWeight);
      //   if(n_photons > 3)
      //   {
      //     presel_pho->Fill(2.0,genTotalWeight);
      //   }
      // }

      // if(n_photons > 3)
      // {
      //   pho_presel->Fill(1.0,genTotalWeight);
      //   if(atLeastOneDiphoPass)
      //   {
      //     pho_presel->Fill(2.0,genTotalWeight);
      //   }
      // }


      std::vector<flashgg::Photon> phoVector;

      if (atLeastOneDiphoPass)
      {
        for( unsigned int dpIndex = 0; dpIndex < diphotons->size(); dpIndex++ )
        {
          edm::Ptr<flashgg::DiPhotonCandidate> thisDPPtr = diphotons->ptrAt( dpIndex );
          diPhoPtrs.push_back(thisDPPtr);
        }
        // cutFlow->Fill(1.0,genTotalWeight);
        // if (n_photons == 2) {cutFlow->Fill(2.0,genTotalWeight);}
        // if (n_photons == 3) {cutFlow->Fill(3.0,genTotalWeight);}
        // if (n_photons == 4) {cutFlow->Fill(4.0,genTotalWeight);}
        // if (n_photons > 3) {cutFlow->Fill(5.0,genTotalWeight);}
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
        H4GCandidate h4g(phoVector, Vertices, vertex_diphoton, genVertex, BSPoint, diPhoPtrs );
        H4GColl_->push_back(h4g);
      }
      event.put( std::move(H4GColl_) );
    }
  }
  typedef flashgg::H4GCandidateProducer FlashggH4GCandidateProducer;
  DEFINE_FWK_MODULE( FlashggH4GCandidateProducer );
