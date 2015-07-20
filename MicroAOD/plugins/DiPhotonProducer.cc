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

using namespace edm;
using namespace std;

namespace flashgg {

    class DiPhotonProducer : public EDProducer
    {

    public:
        DiPhotonProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;
        EDGetTokenT<View<reco::Vertex> > vertexToken_;
        EDGetTokenT<View<flashgg::Photon> > photonToken_;
        EDGetTokenT< VertexCandidateMap > vertexCandidateMapToken_;
        unique_ptr<VertexSelectorBase> vertexSelector_;
        EDGetTokenT<View<reco::Conversion> > conversionToken_;
        EDGetTokenT<reco::BeamSpot>  beamSpotToken_;
        EDGetTokenT<View<reco::Conversion> > conversionTokenSingleLeg_;
        bool useSingleLeg_;
    };

    DiPhotonProducer::DiPhotonProducer( const ParameterSet &iConfig ) :
        vertexToken_( consumes<View<reco::Vertex> >( iConfig.getParameter<InputTag> ( "VertexTag" ) ) ),
        photonToken_( consumes<View<flashgg::Photon> >( iConfig.getParameter<InputTag> ( "PhotonTag" ) ) ),
        vertexCandidateMapToken_( consumes<VertexCandidateMap>( iConfig.getParameter<InputTag>( "VertexCandidateMapTag" ) ) ),
        conversionToken_( consumes<View<reco::Conversion> >( iConfig.getParameter<InputTag>( "ConversionTag" ) ) ),
        beamSpotToken_( consumes<reco::BeamSpot >( iConfig.getParameter<InputTag>( "beamSpotTag" ) ) ),
        conversionTokenSingleLeg_( consumes<View<reco::Conversion> >( iConfig.getParameter<InputTag>( "ConversionTagSingleLeg" ) ) )
    {
        bool default_useSingleLeg_ = true;
        const std::string &VertexSelectorName = iConfig.getParameter<std::string>( "VertexSelectorName" );
        vertexSelector_.reset( FlashggVertexSelectorFactory::get()->create( VertexSelectorName, iConfig ) );
        useSingleLeg_ = iConfig.getUntrackedParameter<bool>( "useSingleLeg", default_useSingleLeg_ );
        produces<vector<flashgg::DiPhotonCandidate> >();
    }

    void DiPhotonProducer::produce( Event &evt, const EventSetup & )
    {
        Handle<View<reco::Vertex> > primaryVertices;
        evt.getByToken( vertexToken_, primaryVertices );
        // const PtrVector<reco::Vertex>& pvPointers = primaryVertices->ptrVector();

        Handle<View<flashgg::Photon> > photons;
        evt.getByToken( photonToken_, photons );
        //  const PtrVector<flashgg::Photon>& photonPointers = photons->ptrVector();

        Handle<VertexCandidateMap> vertexCandidateMap;
        evt.getByToken( vertexCandidateMapToken_, vertexCandidateMap );

        Handle<View<reco::Conversion> > conversions;
        evt.getByToken( conversionToken_, conversions );
        // const PtrVector<reco::Conversion>& conversionPointers = conversions->ptrVector();

        Handle<reco::BeamSpot> recoBeamSpotHandle;
        evt.getByToken( beamSpotToken_, recoBeamSpotHandle );
        math::XYZPoint vertexPoint;
        //    float beamsig;
        if( recoBeamSpotHandle.isValid() ) {
            vertexPoint = recoBeamSpotHandle->position();
            //      beamsig = recoBeamSpotHandle->sigmaZ();
        }

        Handle<View<reco::Conversion> > conversionsSingleLeg;
        evt.getByToken( conversionTokenSingleLeg_, conversionsSingleLeg );
        //const PtrVector<reco::Conversion>& conversionPointersSingleLeg = conversionsSingleLeg->ptrVector();

        auto_ptr<vector<DiPhotonCandidate> > diPhotonColl( new vector<DiPhotonCandidate> );
//    cout << "evt.id().event()= " << evt.id().event() << "\tevt.isRealData()= " << evt.isRealData() << "\tphotons->size()= " << photons->size() << "\tprimaryVertices->size()= " << primaryVertices->size() << endl;

        for( unsigned int i = 0 ; i < photons->size() ; i++ ) {

            Ptr<flashgg::Photon> pp1 = photons->ptrAt( i );
            for( unsigned int j = i + 1 ; j < photons->size() ; j++ ) {
                Ptr<flashgg::Photon> pp2 = photons->ptrAt( j );

                Ptr<reco::Vertex> pvx = vertexSelector_->select( pp1, pp2, primaryVertices->ptrs(), *vertexCandidateMap, conversions->ptrs(), conversionsSingleLeg->ptrs(),
                                        vertexPoint, useSingleLeg_ );
                // Finding and storing the vertex index to check if it corresponds to the primary vertex.
                // This could be moved within the vertexSelector, but would need rewriting some interface
                int ivtx = 0;
                for( unsigned int k = 0; k < primaryVertices->size() ; k++ )
                    if( pvx == primaryVertices->ptrAt( k ) ) {
                        ivtx = k;
                        break;
                    }

                DiPhotonCandidate dipho( pp1, pp2, pvx );
                dipho.setVertexIndex( ivtx );

                // Obviously the last selection has to be for this diphoton or this is wrong
                vertexSelector_->writeInfoFromLastSelectionTo( dipho );

                // store the diphoton into the collection
                diPhotonColl->push_back( dipho );
            }
        }
        // Sort the final collection (descending) and put it in the event
        std::sort( diPhotonColl->begin(), diPhotonColl->end(), []( const DiPhotonCandidate & a, const DiPhotonCandidate & b ) { return b < a; } );

        evt.put( diPhotonColl );
    }
}

typedef flashgg::DiPhotonProducer FlashggDiPhotonProducer;
DEFINE_FWK_MODULE( FlashggDiPhotonProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

