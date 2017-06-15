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

// use PackedCandidate::fromPV to produce a flashgg vertex map
// Require fromPV(int) > FromPVCut
// From https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMiniAOD2015#PV_Assignment
//   "The definition normally used for isolation calculations is fromPV() > 1;
//    the definition used for CHS subtraction in jets is fromPV() > 0."

namespace flashgg {

    class VertexMapFromCandidateProducer : public EDProducer
    {

    public:
        VertexMapFromCandidateProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;
        EDGetTokenT<View<reco::Vertex> > vertexToken_;
        EDGetTokenT<View<pat::PackedCandidate> > pfcandidateToken_;
        unsigned int fromPVgt_, fromPVgtIfDz_;
        double dzCut_;
    };

    VertexMapFromCandidateProducer::VertexMapFromCandidateProducer( const ParameterSet &iConfig ) :
        vertexToken_( consumes<View<reco::Vertex> >( iConfig.getParameter<InputTag> ( "VertexTag" ) ) ),
        pfcandidateToken_( consumes<View<pat::PackedCandidate> >( iConfig.getParameter<InputTag> ( "PFCandidatesTag" ) ) ),
        fromPVgt_( iConfig.getParameter<unsigned int>( "FromPVCut" ) ), // 0 for standard CHS, 2 for PUPPI
        fromPVgtIfDz_( iConfig.getParameter<unsigned int>( "FromPVCutIfPassDz" ) ), // 0 for CHS or PUPPI
        dzCut_( iConfig.getParameter<double>( "DzCut" ) )
    {
        produces<VertexCandidateMap>();
    }

    void VertexMapFromCandidateProducer::produce( Event &evt, const EventSetup & )
    {

        Handle<View<reco::Vertex> > primaryVertices;
        evt.getByToken( vertexToken_, primaryVertices );

        Handle<View<pat::PackedCandidate> > pfCandidates;
        evt.getByToken( pfcandidateToken_, pfCandidates );

        std::unique_ptr<VertexCandidateMap> assoc( new VertexCandidateMap );

        for( unsigned int i = 0 ; i < pfCandidates->size() ; i++ ) {
            Ptr<pat::PackedCandidate> cand = pfCandidates->ptrAt( i );
            if( cand->charge() == 0 ) { continue; } // skip neutrals
            for( unsigned int j = 0 ; j < primaryVertices->size() ; j++ ) {
                //                std::cout << "Particle " << i << " vertex " << j << " - cuts at " << fromPVgt_ << " " << fromPVgtIfDz_ << " " << dzCut_ << std::endl;
                if( cand->fromPV( j ) > fromPVgt_ ) {
                    assoc->emplace_back( primaryVertices->ptrAt( j ), cand );
                    //                    std::cout << " Included because fromPV=" << cand->fromPV( j ) << " > " << fromPVgt_ << std::endl;
                } else if( cand->fromPV( j ) > fromPVgtIfDz_ ) {
                    // This section to support extra cut on dZ in puppi code
                    double absdz = fabs( cand->dz( primaryVertices->ptrAt( j )->position() ) );
                    if( absdz  < dzCut_ ) {
                        assoc->emplace_back( primaryVertices->ptrAt( j ), cand );
                        //                        std::cout << " Included because fromPV=" <<cand->fromPV( j) << " > " << fromPVgtIfDz_<< " and dz=" << absdz << " < " << dzCut_ << std::endl;
                        //                    } else {
                        //                        std::cout << " Not included: fromPV=" << cand->fromPV( j) << " dz=" << absdz << " - cuts at " << fromPVgt_ << " " << fromPVgtIfDz_ << " " << dzCut_ << std::endl;
                    }
                    // Are we perfectly emulating logic from PUPPI? It might be more like the lines below
                    // But maybe the basic PackedCandidate::dz already does the 'best' thing???
                    // Sort it out later, keep code simple for now
                    // N.B. trackRef and gsfTrackRef don't exist in PackedCandidate
                    /*
                    double pDZ    = -9999;
                    if      ( cand->trackRef().isNonnull()    ) {
                        pDZ = cand->trackRef()   ->dz(primaryVertices->ptrAt( j )->position());
                    } else if ( cand->gsfTrackRef().isNonnull() ) {
                        pDZ = cand->gsfTrackRef()->dz(primaryVertices->ptrAt( j )->position());
                    }
                    */
                }
            }
        }

        std::stable_sort( assoc->begin(), assoc->end(), flashgg::compare_by_vtx() );

        //        flashgg::print_track_count( *assoc, "FlashggVertexMapFromCandidateProducer" );

        evt.put( std::move( assoc ) );
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

