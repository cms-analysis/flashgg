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

namespace flashgg {

    class DzVertexMapProducer : public EDProducer
    {

    public:
        DzVertexMapProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;
        EDGetTokenT<View<reco::Vertex> > vertexToken_;
        EDGetTokenT<View<pat::PackedCandidate> > pfcandidateToken_;
        double maxAllowedDz_;
        bool useEachTrackOnce_;
    };

    DzVertexMapProducer::DzVertexMapProducer( const ParameterSet &iConfig ) :
        vertexToken_( consumes<View<reco::Vertex> >( iConfig.getParameter<InputTag> ( "VertexTag" ) ) ),
        pfcandidateToken_( consumes<View<pat::PackedCandidate> >( iConfig.getParameter<InputTag> ( "PFCandidatesTag" ) ) ),
        maxAllowedDz_( iConfig.getParameter<double>( "MaxAllowedDz" ) ), // in cm
        useEachTrackOnce_( iConfig.getParameter<bool>( "UseEachTrackOnce" ) )
    {
        produces<VertexCandidateMap>();
    }

    void DzVertexMapProducer::produce( Event &evt, const EventSetup & )
    {

        Handle<View<reco::Vertex> > primaryVertices;
        evt.getByToken( vertexToken_, primaryVertices );
        //const PtrVector<reco::Vertex>& pvPtrs = primaryVertices->ptrVector();

        Handle<View<pat::PackedCandidate> > pfCandidates;
        evt.getByToken( pfcandidateToken_, pfCandidates );
        //const PtrVector<pat::PackedCandidate>& pfPtrs = pfCandidates->ptrVector();

        std::unique_ptr<VertexCandidateMap> assoc( new VertexCandidateMap );

        if( useEachTrackOnce_ ) {
            // Associate a track to the closest vertex only, and only if dz < maxAllowedDz_
            for( unsigned int i = 0 ; i < pfCandidates->size() ; i++ ) {
                Ptr<pat::PackedCandidate> cand = pfCandidates->ptrAt( i );
                //	if (abs(cand->pdgId() != 211) continue; // skip neutrals
                if( cand->charge() == 0 ) { continue; } // skip neutrals
                double closestDz = maxAllowedDz_;
                unsigned int closestDzIndex = -1;
                for( unsigned int j = 0 ; j < primaryVertices->size() ; j++ ) {
                    Ptr<reco::Vertex> vtx = primaryVertices->ptrAt( j );
                    double dz = fabs( cand->dz( vtx->position() ) );
                    //	cout << " index_Pf index_Vtx j Dz " << i << " " << j << " " << dz << endl;
                    if( dz < closestDz ) {
                        closestDz = dz;
                        closestDzIndex = j;
                        //	  cout << "  New closest Dz " << dz << endl;
                    }
                }
                if( closestDz < maxAllowedDz_ ) {
                    //	cout << " Final insert index_Pf index_Vtx Dz " << i << " " << closestDzIndex << " " << closestDz << endl;
                    Ptr<reco::Vertex> vtx = primaryVertices->ptrAt( closestDzIndex );
                    assoc->emplace_back( vtx, cand );
                }
            }
        } else { /* i.e. if !useEachTrackOnce_ */
            // Allow a track to be associated to multiple vertices if it's close to each of them
            for( unsigned int i = 0 ; i < pfCandidates->size() ; i++ ) {
                Ptr<pat::PackedCandidate> cand = pfCandidates->ptrAt( i );
                if( cand->charge() == 0 ) { continue; } // skip neutrals
                for( unsigned int j = 0 ; j < primaryVertices->size() ; j++ ) {
                    Ptr<reco::Vertex> vtx = primaryVertices->ptrAt( j );
                    double dz = fabs( cand->dz( vtx->position() ) );
                    if( dz < maxAllowedDz_ ) {
                        assoc->emplace_back( vtx, cand );
                    }
                }
            }
        } // end of !useEachTrackOnce_
        std::stable_sort( assoc->begin(), assoc->end(), flashgg::compare_by_vtx() );
        evt.put( std::move( assoc ) );
    }
}

typedef flashgg::DzVertexMapProducer FlashggDzVertexMapProducer;
DEFINE_FWK_MODULE( FlashggDzVertexMapProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

