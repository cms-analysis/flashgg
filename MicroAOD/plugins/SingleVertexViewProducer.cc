#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "flashgg/DataFormats/interface/SingleVertexView.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include <vector>
#include <algorithm>

using namespace std;
using namespace edm;

namespace flashgg {

    class SingleVertexViewProducer : public EDProducer
    {

    public:
        SingleVertexViewProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;

        EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
        EDGetTokenT<View<reco::GenParticle> > genPartToken_;
        int maxCandidates_;

    };

    SingleVertexViewProducer::SingleVertexViewProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        genPartToken_( consumes<View<reco::GenParticle> >( iConfig.getParameter<InputTag> ( "GenParticlesTag" ) ) ),
        maxCandidates_( iConfig.getParameter<int>( "maxCandidates" ) )
    {
        produces<vector<SingleVertexView> >();
    }

    void SingleVertexViewProducer::produce( Event &evt, const EventSetup & )
    {


        Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
        evt.getByToken( diPhotonToken_, diPhotons );
        //	const PtrVector<flashgg::DiPhotonCandidate>& diPhotonPointers = diPhotons->ptrVector();
        // FIXME: move gen vertex finding to dedicated producer
        bool isData = evt.isRealData();
        reco::GenParticle::Point genV;
        if( ! isData ) {
            Handle<View<reco::GenParticle> > genParts;
            evt.getByToken( genPartToken_, genParts );
            bool found = false;
            for( auto &part : *genParts ) {
                if( part.pdgId() != 2212 || part.vertex().z() != 0. ) {
                    genV = part.vertex();
                    found = true;
                    break;
                }
            }
            assert( found );
        }

        std::unique_ptr<vector<SingleVertexView> > vertexViews( new vector<SingleVertexView> );
        std::map<float, int> sortViews;

        int nCand = maxCandidates_;
        //for(auto & dipho : diPhotons) {
        for( unsigned int i = 0; i < diPhotons->size(); i++ ) {

            for( unsigned int iv = 0; iv < ( diPhotons->ptrAt( i ) )->nVtxInfoSize(); ++iv ) {
                vertexViews->push_back( SingleVertexView( diPhotons->ptrAt( i ), iv ) );
                auto &vi = vertexViews->back();
                float dz = genV.z() - vi.pos().z();
                vi.setDzToGen( dz );
                sortViews[ dz  ] = iv;
            }

            if( --nCand == 0 ) { break; }
        }

        if( ! isData && vertexViews->size() > 0 ) {
            vertexViews->at( sortViews.begin()->second ).setIsClosestToGen();
        }

        //// if( vertexViews->size() != 0 ) {
        //// 	cout << vertexViews->size() << endl;
        //// }
        evt.put( std::move( vertexViews ) );

    }
}

typedef flashgg::SingleVertexViewProducer FlashggSingleVertexViewProducer;
DEFINE_FWK_MODULE( FlashggSingleVertexViewProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

