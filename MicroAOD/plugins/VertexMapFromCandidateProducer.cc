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

using namespace edm;
using namespace std;

// if the track did not attach to any vertex, attach it to ALL vertices!
// This probably mimics better how PFCHS works
// We want to keep all the tracks for the vertex that's ultimately selected,
// unless they're clearly close to another vertex

namespace flashgg {

    class VertexMapFromCandidateProducer : public EDProducer
    {

    public:
        VertexMapFromCandidateProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;
        EDGetTokenT<View<reco::Vertex> > vertexToken_;
        EDGetTokenT<View<pat::PackedCandidate> > pfcandidateToken_;
        unsigned int fromPVgt_;
    };

    VertexMapFromCandidateProducer::VertexMapFromCandidateProducer( const ParameterSet &iConfig ) :
        vertexToken_( consumes<View<reco::Vertex> >( iConfig.getParameter<InputTag> ( "VertexTag" ) ) ),
        pfcandidateToken_( consumes<View<pat::PackedCandidate> >( iConfig.getParameter<InputTag> ( "PFCandidatesTag" ) ) ),
        fromPVgt_( iConfig.getParameter<unsigned int>( "FromPVCut" ) ) // 0 for standard CHS
    {
        produces<VertexCandidateMap>();
    }

    void VertexMapFromCandidateProducer::produce( Event &evt, const EventSetup & )
    {

        Handle<View<reco::Vertex> > primaryVertices;
        evt.getByToken( vertexToken_, primaryVertices );

        Handle<View<pat::PackedCandidate> > pfCandidates;
        evt.getByToken( pfcandidateToken_, pfCandidates );

        std::auto_ptr<VertexCandidateMap> assoc( new VertexCandidateMap );

        for( unsigned int i = 0 ; i < pfCandidates->size() ; i++ ) {
            Ptr<pat::PackedCandidate> cand = pfCandidates->ptrAt( i );
            if( cand->charge() == 0 ) { continue; } // skip neutrals
            for( unsigned int j = 0 ; j < primaryVertices->size() ; j++ ) {
                if( cand->fromPV( j ) > fromPVgt_ ) {
                    assoc->emplace_back( primaryVertices->ptrAt( j ), cand );
                }
            }
        }

        std::stable_sort( assoc->begin(), assoc->end(), flashgg::compare_by_vtx() );

        //        flashgg::print_track_count( *assoc, "FlashggVertexMapFromCandidateProducer" );

        evt.put( assoc );
    } // produce method
} // namespace flashgg

typedef flashgg::VertexMapFromCandidateProducer FlashggVertexMapFromCandidateProducer;
DEFINE_FWK_MODULE( FlashggVertexMapFromCandidateProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

