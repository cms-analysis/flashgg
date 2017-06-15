/** MultiCHSLegVertexCandProducer.cc ---
 *
 * Author: yhaddad <yhaddad@cern.ch>
 *
 */

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "flashgg/DataFormats/interface/VertexCandidateMap.h"
#include "DataFormats/Common/interface/RefToPtr.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"

using namespace edm;
using namespace std;

namespace flashgg {
    class MultiCHSLegacyVertexCandProducer : public EDProducer
    {

    public:
        MultiCHSLegacyVertexCandProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;
        EDGetTokenT<View<reco::Vertex> >               vertexToken_;
        EDGetTokenT<View<flashgg::DiPhotonCandidate> > diPhotonsToken_;
        EDGetTokenT<View<pat::PackedCandidate> >       pfcandidateToken_;
        EDGetTokenT< VertexCandidateMap >              vertexCandidateMapToken_;

        unsigned     indexVtx_;
        bool         debug_;
        unsigned int eventNb;
    };

    MultiCHSLegacyVertexCandProducer::MultiCHSLegacyVertexCandProducer( const ParameterSet &iConfig ) :
        vertexToken_( consumes<View<reco::Vertex> >( iConfig.getParameter<InputTag> ( "VertexTag" ) ) ),
        diPhotonsToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        pfcandidateToken_( consumes<View<pat::PackedCandidate> >( iConfig.getParameter<InputTag> ( "PFCandidatesTag" ) ) ),
        vertexCandidateMapToken_( consumes<VertexCandidateMap>( iConfig.getParameter<InputTag>( "VertexCandidateMapTag" ) ) ),
        indexVtx_( iConfig.getParameter<unsigned> ( "vertexIndex" ) ),
        debug_( iConfig.getUntrackedParameter<bool>( "debug"      ,  false ) )
    {
        produces<vector<pat::PackedCandidate> >();
        eventNb = 0 ;
    }

    void MultiCHSLegacyVertexCandProducer::produce( Event &evt , const EventSetup & )
    {
        // diphotons)
        Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
        evt.getByToken( diPhotonsToken_, diPhotons );
        // const PtrVector<flashgg::DiPhotonCandidate>& diPhotonPointers= diPhotons->ptrVector();

        // Primary Vertices in case no diphoton found
        Handle<View<reco::Vertex> > primaryVertices;
        evt.getByToken( vertexToken_, primaryVertices );
        //const PtrVector<reco::Vertex>& pvPtrs = primaryVertices->ptrVector();

        // packed cands
        Handle<View<pat::PackedCandidate> > pfCandidates;
        evt.getByToken( pfcandidateToken_, pfCandidates );
        // const PtrVector<pat::PackedCandidate>& pfPtrs = pfCandidates->ptrVector();

        Handle<VertexCandidateMap> vtxmap;
        // Handle<View<reco::Vertex> > primaryVertices;
        // Handle<reco::VertexCollection> primaryVertices;
        evt.getByToken( vertexCandidateMapToken_, vtxmap );

        // Non-const copy so we can sort by candidates (which may be associated to more than one vertex)
        VertexCandidateMap vtxmapbycand( *vtxmap );
        std::stable_sort( vtxmapbycand.begin(), vtxmapbycand.end(), flashgg::compare_by_cand() );


        edm::Ptr<reco::Vertex> choosenVertex;


        // force the use of the MiniAOD default vertex
        int  diphoton_index = -1;
        bool keep_event     = false;
        if( indexVtx_ == 0 ) {
            choosenVertex =  primaryVertices->ptrAt( 0 );
            keep_event = true;
        }
        // Use the other vertices :
        // we are interested only by the di-photon candidates verticies
        // then retain only the vertex of the di-photon that matters
        else {
            for( unsigned int diPhoLoop = 0; diPhoLoop < diPhotons->size() ; diPhoLoop++ ) {
                // we only have a problem if the mutliple diphotons haev different vertices...
                if( debug_ ) std::cout << setw( 13 ) << "(diphoton)=" << diPhoLoop
                                           << setw( 13 ) << "(smartidx)=" << diPhotons->ptrAt( diPhoLoop )->jetCollectionIndex()
                                           << std::endl;

                if( diPhotons->ptrAt( diPhoLoop )->jetCollectionIndex() == indexVtx_ ) {
                    choosenVertex  = diPhotons->ptrAt( diPhoLoop )->vtx();
                    diphoton_index = diPhoLoop;
                    keep_event = true;
                    break;
                }
            }
        }

        std::unique_ptr<vector<pat::PackedCandidate> > result( new vector<pat::PackedCandidate>() );
        if( keep_event ) {
            for( unsigned int pfCandLoop = 0 ; pfCandLoop < pfCandidates->size() ; pfCandLoop++ ) {
                edm::Ptr<pat::PackedCandidate> cand = pfCandidates->ptrAt( pfCandLoop );
                if( cand->charge() == 0 ) { //keep all neutral objects.
                    assert( !( cand.isNull() ) );
                    result->push_back( *cand );
                    continue;
                }
                // Keep charged candidate if it's associated to the appropriate vertex
                auto mapRange = std::equal_range( vtxmapbycand.begin(), vtxmapbycand.end(), cand, flashgg::compare_with_cand() );
                for( auto pair_iter = mapRange.first ; pair_iter != mapRange.second ; pair_iter++ ) {
                    edm::Ptr<reco::Vertex> currentVertex = pair_iter->first;
                    if( choosenVertex == currentVertex ) { result->push_back( *cand ); }
                }
            }
        }
        if( debug_ ) std::cout << setw( 13 ) << "event nb=" << eventNb
                                   << setw( 13 ) << "ndiphoto=" << diPhotons->size()
                                   << setw( 13 ) << "input id=" << indexVtx_
                                   << setw( 13 ) << "dipho id=" << diphoton_index
                                   << setw( 13 ) << "result n=" << result->size()
                                   << std::endl;

        if( debug_ ) { std::cout << setw( 13 ) << "-----------------------------" << std::endl; }
        evt.put( std::move( result ) );
        eventNb++;
    }

}// end flashgg namespace

typedef flashgg::MultiCHSLegacyVertexCandProducer FlashggMultiCHSLegacyVertexCandProducer;
DEFINE_FWK_MODULE( FlashggMultiCHSLegacyVertexCandProducer );
