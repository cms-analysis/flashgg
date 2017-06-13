
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

        bool useZeroth;
    };

    CHSLegacyVertexCandidateProducer::CHSLegacyVertexCandidateProducer( const ParameterSet &iConfig ) :
        vertexToken_( consumes<View<reco::Vertex> >( iConfig.getParameter<InputTag> ( "VertexTag" ) ) ),
        diPhotonsToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        pfcandidateToken_( consumes<View<pat::PackedCandidate> >( iConfig.getParameter<InputTag> ( "PFCandidatesTag" ) ) ),
        vertexCandidateMapToken_( consumes<VertexCandidateMap>( iConfig.getParameter<InputTag>( "VertexCandidateMapTag" ) ) ),
        useZeroth( iConfig.getUntrackedParameter<bool>( "UseZeroth", false ) )
    {
        produces<vector<pat::PackedCandidate> >();
    }


    void CHSLegacyVertexCandidateProducer::produce( Event &evt , const EventSetup & )
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

        edm::Ptr<reco::Vertex> flashVertex;

        //std::cout <<"Run[" << evt.run() <<  "]=evt["<< evt.id().event() << "]\t npart::" <<  pfCandidates->size() << std::endl;
        if( useZeroth ) { flashVertex =  primaryVertices->ptrAt( 0 ); }
        else {
            if( diPhotons->size() == 0 ) {
                flashVertex = primaryVertices->ptrAt( 0 );
            }
            if( diPhotons->size() == 1 ) {
                flashVertex = diPhotons->ptrAt( 0 )->vtx();
            }
            if( diPhotons->size() > 1 ) { //hopefully very rare
                flashVertex = diPhotons->ptrAt( 0 )->vtx();
            }


            for( unsigned int diPhoLoop = 0; diPhoLoop < diPhotons->size() ; diPhoLoop++ ) {
                //if( diPhotons[diPhoLoop]->vertex()->position() != flashVertex->position()){
                // we only have a problem if the mutliple diphotons haev different vertices...
                if( diPhotons->ptrAt( diPhoLoop )->vtx() != flashVertex ) {
                    //replace with Error Logger at some stage, cout is not thread safe.
                    std::cout << "[WARNING] Multiple Diphotons in event, with different PVs "
                              << ". Using 0th Diphoton Vtx for CHS PU subtraction." << std::endl;
                    break;
                }
            }
        }

        std::unique_ptr<vector<pat::PackedCandidate> > result( new vector<pat::PackedCandidate>() );

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
                if( flashVertex == currentVertex ) { result->push_back( *cand ); }
            }
        }



        evt.put( std::move( result ) );
    }
}
typedef flashgg::CHSLegacyVertexCandidateProducer FlashggCHSLegacyVertexCandidateProducer;
DEFINE_FWK_MODULE( FlashggCHSLegacyVertexCandidateProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

