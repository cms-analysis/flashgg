#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/MicroAOD/interface/PhotonIdUtils.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "flashgg/MicroAOD/interface/VertexSelectorBase.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "flashgg/DataFormats/interface/VertexCandidateMap.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/EgammaCandidates/interface/PhotonCore.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"

#include <set>

using namespace edm;
using namespace std;

namespace flashgg {

    class PhotonCollectionFromDiPhotons : public EDProducer
    {

    public:
        PhotonCollectionFromDiPhotons( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;
        EDGetTokenT<View<flashgg::DiPhotonCandidate> > diPhotonToken_;
        bool debug_;
    };

    PhotonCollectionFromDiPhotons::PhotonCollectionFromDiPhotons( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        debug_( iConfig.getUntrackedParameter( "Debug", false ) )
    {
        // vector<reco::PhotonCore>              "reducedEgamma"             "reducedGedPhotonCores"   "PAT"
        // vector<reco::SuperCluster>            "reducedEgamma"             "reducedSuperClusters"   "PAT"
        // recoSuperClusters_reducedEgamma_reducedSuperClusters_PAT. 3021.95 1028.47
        // recoPhotonCores_reducedEgamma_reducedGedPhotonCores_PAT. 265.606 55.832

        produces<vector<flashgg::DiPhotonCandidate> >();
        produces<vector<flashgg::Photon> >();
        produces<vector<reco::SuperCluster> >();
        produces<vector<reco::PhotonCore> >();
    }

    void PhotonCollectionFromDiPhotons::produce( Event &evt, const EventSetup & )
    {
        Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
        evt.getByToken( diPhotonToken_, diPhotons );

        auto_ptr<vector<DiPhotonCandidate> > diPhotonColl( new vector<DiPhotonCandidate> );
        auto_ptr<vector<Photon> > photonColl( new vector<Photon> );
        auto_ptr<vector<reco::SuperCluster> > scColl( new vector<reco::SuperCluster> );
        auto_ptr<vector<reco::PhotonCore> > photonCoreColl( new vector<reco::PhotonCore> );

        edm::RefProd<vector<Photon> > rPhoton = evt.getRefBeforePut<vector<Photon> >();
        edm::RefProd<vector<reco::SuperCluster> > rSuperCluster = evt.getRefBeforePut<vector<reco::SuperCluster> >();
        edm::RefProd<vector<reco::PhotonCore> > rPhotonCore = evt.getRefBeforePut<vector<reco::PhotonCore> >();


        // Same order as output photon collection to keep track of what's copied already
        vector<Ptr<Photon> > usedPhotonPtrs;

        if( debug_ ) {
            std::cout << std::cout << " Input DiPhoton collection size: " << diPhotons->size() << std::endl;
        }

        std::set<edm::Ptr<reco::Vertex> > usedVertices;
        for( unsigned int i = 0 ; i < diPhotons->size() ; i++ ) {
            usedVertices.insert( diPhotons->ptrAt( i )->vtx() );
        }

        for( unsigned int i = 0 ; i < diPhotons->size() ; i++ ) {
            //            std::cout << "  DiPhoton " << i << std::endl;
            Ptr<DiPhotonCandidate> oldpdp = diPhotons->ptrAt( i );
            Ptr<Photon> oldpp1 = oldpdp->leadingView()->originalPhoton();
            Ptr<Photon> oldpp2 = oldpdp->subLeadingView()->originalPhoton();
            Ptr<Photon> pp1;
            Ptr<Photon> pp2;

            bool donepp1 = false;
            bool donepp2 = false;
            for( unsigned int j = 0 ; j < usedPhotonPtrs.size() ; j++ ) {
                // If photon already copied into new collection, point to same one
                if( oldpp1 == usedPhotonPtrs[j] ) {
                    donepp1 = true;
                    pp1 = edm::refToPtr( edm::Ref<vector<Photon> >( rPhoton, j ) );
                    if( debug_ ) { std::cout << "   Leading photon with pt " << oldpp1->pt() << " already in collection at place " << j << std::endl; }
                }
                if( oldpp2 == usedPhotonPtrs[j] ) {
                    donepp2 = true;
                    pp2 = edm::refToPtr( edm::Ref<vector<Photon> >( rPhoton, j ) );
                    if( debug_ ) { std::cout << "   Subleading photon with pt " << oldpp2->pt() << " already in collection at place " << j << std::endl; }
                }
            }

            // If photon not yet copied: copy photon, create pointers into new collection
            if( !donepp1 ) {
                Photon p1( *oldpp1 );
                p1.removeVerticesExcept( usedVertices );
                pp1 = edm::refToPtr( edm::Ref<vector<Photon> >( rPhoton, usedPhotonPtrs.size() ) );
                if( debug_ ) { std::cout << "   Putting copy of Leading photon with pt " << oldpp1->pt() << " in at place " << usedPhotonPtrs.size() << std::endl; }

                reco::PhotonCore p1c( *( p1.photonCore() ) );
                p1c.setSuperCluster( edm::Ref<vector<reco::SuperCluster> >( rSuperCluster, usedPhotonPtrs.size() ) );
                p1.setPhotonCore( edm::Ref<vector<reco::PhotonCore> >( rPhotonCore, usedPhotonPtrs.size() ) );

                scColl->push_back( *( oldpp1->superCluster() ) );
                photonCoreColl->push_back( p1c );
                photonColl->push_back( p1 );
                usedPhotonPtrs.push_back( oldpp1 );
            }
            if( !donepp2 ) {
                Photon p2( *oldpp2 ); // copy Photon
                p2.removeVerticesExcept( usedVertices );
                pp2 = edm::refToPtr( edm::Ref<vector<Photon> >( rPhoton, usedPhotonPtrs.size() ) );
                if( debug_ ) { std::cout << "   Putting copy of Subleading photon with pt " << oldpp2->pt() << " in at place " << usedPhotonPtrs.size() << std::endl; }

                reco::PhotonCore p2c( *( p2.photonCore() ) );
                p2c.setSuperCluster( edm::Ref<vector<reco::SuperCluster> >( rSuperCluster, usedPhotonPtrs.size() ) );
                p2.setPhotonCore( edm::Ref<vector<reco::PhotonCore> >( rPhotonCore, usedPhotonPtrs.size() ) );

                scColl->push_back( *( oldpp2->superCluster() ) );
                photonCoreColl->push_back( p2c );
                photonColl->push_back( p2 );
                usedPhotonPtrs.push_back( oldpp2 );
            }

            // Can't use ordinary DiPhoton constructor with Ptrs because pp1 and pp2 are pointing into a collection that's not saved yet
            // But all the old values are still valid, so swapping in pp1 and pp2 after won't break anything
            // But be very very very careful emulating this!
            DiPhotonCandidate dipho( *oldpdp );
            dipho.getLeadingView().replacePtr( pp1 );
            dipho.getSubLeadingView().replacePtr( pp2 );

            diPhotonColl->push_back( dipho );
        }

        if( debug_ ) {
            std::cout << " Final diphoton collection size: " << diPhotonColl->size() << std::endl;
            std::cout << " Final photon collection size: " << photonColl->size() << std::endl;
        }

        evt.put( scColl );
        evt.put( photonCoreColl );
        evt.put( photonColl );

        if( debug_ ) {
            std::cout << " We put the photons in, now we check diphoton pt before and after recomputation: " << std::endl;
            for( unsigned int i = 0 ; i < diPhotonColl->size() ; i++ ) {
                std::cout << "   DiPhoton " << i << " pt before recomputation: " << diPhotonColl->at( i ).pt() << std::endl;
                diPhotonColl->at( i ).computeP4AndOrder();
                std::cout << "   DiPhoton " << i << " pt after recomputation: " << diPhotonColl->at( i ).pt() << std::endl;
            }
        }

        evt.put( diPhotonColl );
    }
}

typedef flashgg::PhotonCollectionFromDiPhotons FlashggPhotonCollectionFromDiPhotons;
DEFINE_FWK_MODULE( FlashggPhotonCollectionFromDiPhotons );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

