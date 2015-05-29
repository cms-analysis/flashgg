#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "RecoJets/JetProducers/interface/PileupJetIdAlgo.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "flashgg/DataFormats/interface/VertexCandidateMap.h"


using namespace std;
using namespace edm;

namespace flashgg {

    class JetProducer : public EDProducer
    {

    public:
        JetProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;
        EDGetTokenT<View<pat::Jet> > jetToken_;
        EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
        EDGetTokenT<View<reco::Vertex> >  vertexToken_;
        EDGetTokenT< VertexCandidateMap > vertexCandidateMapToken_;
        unique_ptr<PileupJetIdAlgo>  pileupJetIdAlgo_;
        ParameterSet pileupJetIdParameters_;
        bool usePuppi;
        double minJetPt_; // GeV
    };


    JetProducer::JetProducer( const ParameterSet &iConfig ) :
        jetToken_( consumes<View<pat::Jet> >( iConfig.getUntrackedParameter<InputTag> ( "JetTag", InputTag( "slimmedJets" ) ) ) ),
        diPhotonToken_( consumes<View<DiPhotonCandidate> >( iConfig.getUntrackedParameter<InputTag>( "DiPhotonTag", InputTag( "flashggDiPhotons" ) ) ) ),
        vertexToken_( consumes<View<reco::Vertex> >( iConfig.getUntrackedParameter<InputTag> ( "VertexTag", InputTag( "offlineSlimmedPrimaryVertices" ) ) ) ),
        vertexCandidateMapToken_( consumes<VertexCandidateMap>( iConfig.getParameter<InputTag>( "VertexCandidateMapTag" ) ) ),
        pileupJetIdParameters_( iConfig.getParameter<ParameterSet>( "PileupJetIdParameters" ) ),
        usePuppi( iConfig.getUntrackedParameter<bool>( "UsePuppi", false ) ),
        minJetPt_( iConfig.getUntrackedParameter<double>( "MinJetPt", 0. ) )
    {
        pileupJetIdAlgo_.reset( new PileupJetIdAlgo( pileupJetIdParameters_ ) );

        produces<vector<flashgg::Jet> >();
    }

    void JetProducer::produce( Event &evt, const EventSetup & )
    {

        // input jets
        Handle<View<pat::Jet> > jets;
        evt.getByToken( jetToken_, jets );
        // const PtrVector<pat::Jet>& jetPointers = jets->ptrVector();

        // input DiPhoton candidates
        Handle<View<DiPhotonCandidate> > diPhotons;
        evt.getByToken( diPhotonToken_, diPhotons );
        // const PtrVector<DiPhotonCandidate>& diPhotonPointers = diPhotons->ptrVector();

        Handle<View<reco::Vertex> > primaryVertices;
        evt.getByToken( vertexToken_, primaryVertices );
        // const PtrVector<reco::Vertex>& pvPointers = primaryVertices->ptrVector();
        // std::cout << " jet test ==" << primaryVertices->size() <<  std::endl;

        Handle<VertexCandidateMap> vertexCandidateMap;
        evt.getByToken( vertexCandidateMapToken_, vertexCandidateMap );
        // std::cout << " vtx map ==" << vertexCandidateMap->size() <<  std::endl;
        // output jets
        auto_ptr<vector<flashgg::Jet> > jetColl( new vector<flashgg::Jet> );

        for( unsigned int i = 0 ; i < jets->size() ; i++ ) {
            Ptr<pat::Jet> pjet = jets->ptrAt( i );
            if( pjet->pt() < minJetPt_ ) { continue; }
            flashgg::Jet fjet = flashgg::Jet( *pjet );

            for( unsigned int j = 0 ; j < diPhotons->size() ; j++ ) {
                Ptr<DiPhotonCandidate> diPhoton = diPhotons->ptrAt( j );
                Ptr<reco::Vertex> vtx = diPhoton->vtx();
                if( !usePuppi ) {
                    if( !fjet.hasPuJetId( vtx ) ) {
                        PileupJetIdentifier lPUJetId = pileupJetIdAlgo_->computeIdVariables( pjet.get(), vtx, *vertexCandidateMap, true );
                        fjet.setPuJetId( vtx, lPUJetId ); //temporarily make all jets pass
                    }
                }
            }
            if( !usePuppi ) {
                if( primaryVertices->size() > 0 && !fjet.hasPuJetId( primaryVertices->ptrAt( 0 ) ) ) {
                    PileupJetIdentifier lPUJetId = pileupJetIdAlgo_->computeIdVariables( pjet.get(), primaryVertices->ptrAt( 0 ), *vertexCandidateMap, true );
                    fjet.setPuJetId( primaryVertices->ptrAt( 0 ), lPUJetId );
                }
            }
            jetColl->push_back( fjet );
        }

        evt.put( jetColl );
    }
}

typedef flashgg::JetProducer FlashggJetProducer;
DEFINE_FWK_MODULE( FlashggJetProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

