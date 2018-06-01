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
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/EgammaCandidates/interface/PhotonCore.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "flashgg/DataFormats/interface/Electron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronCore.h"
#include "DataFormats/Common/interface/RefToPtr.h"


#include <set>

using namespace edm;
using namespace std;

namespace flashgg {

    class EGammaMinimizer : public EDProducer
    {

    public:
        EGammaMinimizer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;
        EDGetTokenT<View<flashgg::DiPhotonCandidate> > diPhotonToken_;
        EDGetTokenT<View<flashgg::Electron> > electronToken_;
        string photonCollectionName_, electronCollectionName_, diPhotonCollectionName_;
        string superClusterCollectionName_, photonCoreCollectionName_, electronCoreCollectionName_;
        bool debug_;
    };

    EGammaMinimizer::EGammaMinimizer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        electronToken_( consumes<View<flashgg::Electron> >( iConfig.getParameter<InputTag> ( "ElectronTag" ) ) ),
        photonCollectionName_( iConfig.getParameter<string> ( "PhotonCollectionName" ) ),
        electronCollectionName_( iConfig.getParameter<string> ( "ElectronCollectionName" ) ),
        diPhotonCollectionName_( iConfig.getParameter<string> ( "DiPhotonCollectionName" ) ),
        superClusterCollectionName_( iConfig.getParameter<string> ( "SuperClusterCollectionName" ) ),
        photonCoreCollectionName_( iConfig.getParameter<string> ( "PhotonCoreCollectionName" ) ),
        electronCoreCollectionName_( iConfig.getParameter<string> ( "ElectronCoreCollectionName" ) ),
        debug_( iConfig.getUntrackedParameter( "Debug", false ) )
    {
        produces<vector<flashgg::DiPhotonCandidate> >( diPhotonCollectionName_ );
        produces<vector<flashgg::Photon> >( photonCollectionName_ );
        produces<vector<flashgg::Electron> >( electronCollectionName_ );
        produces<vector<reco::SuperCluster> >( superClusterCollectionName_ );
        produces<vector<reco::PhotonCore> >( photonCoreCollectionName_ );
        produces<vector<reco::GsfElectronCore> >( electronCoreCollectionName_ );
        if( debug_ ) {
            std::cout << "FLASHggEGammaMinimizer producing: " << std::endl;
            std::cout << " diPhotonCollectionName_ " << diPhotonCollectionName_ << std::endl;
            std::cout << " photonCollectionName_ " << photonCollectionName_ << std::endl;
            std::cout << " photonCoreCollectionName_ " << photonCoreCollectionName_ << std::endl;
            std::cout << " electronCollectionName_ " << electronCollectionName_ << std::endl;
            std::cout << " electronCoreCollectionName_ " << electronCoreCollectionName_ << std::endl;
            std::cout << " superClusterCollectionName_ " << superClusterCollectionName_ << std::endl;
        }
    }

    void EGammaMinimizer::produce( Event &evt, const EventSetup & )
    {
        Handle<View<DiPhotonCandidate> > diPhotons;
        evt.getByToken( diPhotonToken_, diPhotons );

        Handle<View<Electron> > electrons;
        evt.getByToken( electronToken_, electrons );

        unique_ptr<vector<DiPhotonCandidate> > diPhotonColl( new vector<DiPhotonCandidate> );
        unique_ptr<vector<Photon> > photonColl( new vector<Photon> );
        unique_ptr<vector<reco::SuperCluster> > scColl( new vector<reco::SuperCluster> );
        unique_ptr<vector<reco::PhotonCore> > photonCoreColl( new vector<reco::PhotonCore> );
        unique_ptr<vector<Electron> > electronColl( new vector<Electron> );
        unique_ptr<vector<reco::GsfElectronCore> > electronCoreColl( new vector<reco::GsfElectronCore> );

        edm::RefProd<vector<Photon> > rPhoton = evt.getRefBeforePut<vector<Photon> >( photonCollectionName_ );
        edm::RefProd<vector<reco::SuperCluster> > rSuperCluster = evt.getRefBeforePut<vector<reco::SuperCluster> >( superClusterCollectionName_ );
        edm::RefProd<vector<reco::PhotonCore> > rPhotonCore = evt.getRefBeforePut<vector<reco::PhotonCore> >( photonCoreCollectionName_ );
        edm::RefProd<vector<reco::GsfElectronCore> > rElectronCore = evt.getRefBeforePut<vector<reco::GsfElectronCore> >( electronCoreCollectionName_ );

        // Same order as output photon collection to keep track of what's copied already
        vector<Ptr<Photon> > usedPhotonPtrs;

        // Refs to old super clusters to keep track of what's been copied already
        vector<edm::Ref<vector<reco::SuperCluster> > > usedSuperClusterRefs;

        if( debug_ ) {
            std::cout << " Input DiPhoton collection size: " << diPhotons->size() << std::endl;
        }

        std::set<edm::Ptr<reco::Vertex> > usedVertices;
        for( unsigned int i = 0 ; i < diPhotons->size() ; i++ ) {
            usedVertices.insert( diPhotons->ptrAt( i )->vtx() );
        }

        for( unsigned int i = 0 ; i < diPhotons->size() ; i++ ) {
            if( debug_ ) { std::cout << "  DiPhoton " << i << std::endl; }
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
                usedSuperClusterRefs.push_back( oldpp1->superCluster() );
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
                usedSuperClusterRefs.push_back( oldpp2->superCluster() );
            }

            // Can't use ordinary DiPhoton constructor with Ptrs because pp1 and pp2 are pointing into a collection that's not saved yet
            // But all the old values are still valid, so swapping in pp1 and pp2 after won't break anything
            // But be very very very careful emulating this!
            DiPhotonCandidate dipho( *oldpdp );
            dipho.getLeadingView().replacePtr( pp1 );
            dipho.getSubLeadingView().replacePtr( pp2 );

            diPhotonColl->push_back( dipho );
        }

        // Now we have to do the electrons too so that they point into the same superCluster collection in the case of overlaps
        for( unsigned int i = 0 ; i < electrons->size() ; i++ ) {
            Electron ele( *electrons->ptrAt( i ) );
            reco::GsfElectronCore elec( *ele.core() );
            bool done = false;
            for( unsigned int j = 0 ; j < usedSuperClusterRefs.size() ; j++ ) {
                if( elec.superCluster() == usedSuperClusterRefs[j] ) {
                    done = true;
                    elec.setSuperCluster( edm::Ref<vector<reco::SuperCluster> >( rSuperCluster, j ) );
                    if( debug_ ) { std::cout << "  Electron " << i << " with pt " << ele.pt() << " already has its supercluster at place " << j << std::endl; }
                    break;
                }
            }
            if( !done ) {
                if( debug_ ) { std::cout << "  Electron " << i << " with pt " << ele.pt() << " has no supercluster yet, storing it at place " << usedSuperClusterRefs.size() << std::endl; }
                elec.setSuperCluster( edm::Ref<vector<reco::SuperCluster> >( rSuperCluster, usedSuperClusterRefs.size() ) );
                scColl->push_back( *ele.superCluster() ); // copy old sc from old core
                usedSuperClusterRefs.push_back( ele.superCluster() ); // ref to old sc from old core
            }
            ele.setCore( edm::Ref<vector<reco::GsfElectronCore> >( rElectronCore, i ) );
            electronCoreColl->push_back( elec );
            electronColl->push_back( ele );
        }

        if( debug_ ) {
            std::cout << " Final diphoton collection size: " << diPhotonColl->size() << std::endl;
            std::cout << " Final photon collection size: " << photonColl->size() << std::endl;
            std::cout << " Final photon core collection size: " << photonCoreColl->size() << std::endl;
            std::cout << " Final electron collection size: " << electronColl->size() << std::endl;
            std::cout << " Final electron core collection size: " << electronCoreColl->size() << std::endl;
            std::cout << " Final supercluster collection size: " << scColl->size() << std::endl;
            std::cout << " Final diphoton collection name: " <<  diPhotonCollectionName_ << std::endl;
            std::cout << " Final photon collection name: " << photonCollectionName_ << std::endl;
            std::cout << " Final photon core collection name: " << photonCoreCollectionName_ << std::endl;
            std::cout << " Final electron collection name: " << electronCollectionName_ << std::endl;
            std::cout << " Final electron core collection name: " << electronCoreCollectionName_ << std::endl;
            std::cout << " Final supercluster collection name: " << superClusterCollectionName_ << std::endl;

        }

        evt.put( std::move( scColl) , superClusterCollectionName_ );
        evt.put( std::move( photonCoreColl) , photonCoreCollectionName_ );
        evt.put( std::move( photonColl) , photonCollectionName_ );
        evt.put( std::move( electronCoreColl) , electronCoreCollectionName_ );
        evt.put( std::move( electronColl) , electronCollectionName_ );

        if( debug_ ) {
            std::cout << " We put the photons in, now we check diphoton pt before and after recomputation: " << std::endl;
            for( unsigned int i = 0 ; i < diPhotonColl->size() ; i++ ) {
                std::cout << "   DiPhoton " << i << " pt before recomputation: " << diPhotonColl->at( i ).pt() << std::endl;
                diPhotonColl->at( i ).computeP4AndOrder();
                std::cout << "   DiPhoton " << i << " pt after recomputation: " << diPhotonColl->at( i ).pt() << std::endl;
            }
        }

        evt.put( std::move( diPhotonColl) , diPhotonCollectionName_ );
    }
}

typedef flashgg::EGammaMinimizer FlashggEGammaMinimizer;
DEFINE_FWK_MODULE( FlashggEGammaMinimizer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

