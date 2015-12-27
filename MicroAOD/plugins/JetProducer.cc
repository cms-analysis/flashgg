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
//#include "RecoJets/JetProducers/interface/PileupJetIdAlgo.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "flashgg/DataFormats/interface/VertexCandidateMap.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "DataFormats/Math/interface/deltaR.h"


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
        //EDGetTokenT<edm::ValueMap<float> > qgToken_; 
        //edm::InputTag qgToken_;
        edm::InputTag qgVariablesInputTag;
        edm::EDGetTokenT<edm::ValueMap<float> > qgToken;
        //        unique_ptr<PileupJetIdAlgo>  pileupJetIdAlgo_;
        //        ParameterSet pileupJetIdParameters_;
        bool usePuppi;
        bool computeSimpleRMS;
    };


    JetProducer::JetProducer( const ParameterSet &iConfig ) :
        jetToken_( consumes<View<pat::Jet> >( iConfig.getParameter<InputTag> ( "JetTag" ) ) ),
        diPhotonToken_( consumes<View<DiPhotonCandidate> >( iConfig.getParameter<InputTag>( "DiPhotonTag" ) ) ),
        vertexToken_  ( consumes<View<reco::Vertex> >( iConfig.getParameter<InputTag> ( "VertexTag" ) ) ),
        vertexCandidateMapToken_( consumes<VertexCandidateMap>( iConfig.getParameter<InputTag>( "VertexCandidateMapTag" ) )),
        qgVariablesInputTag( iConfig.getParameter<edm::InputTag>( "qgVariablesInputTag" ) ),
        computeSimpleRMS( iConfig.getParameter<bool>( "ComputeSimpleRMS" ) )
        //GluonTagSrc_  (iConfig.getParameter<edm::InputTag>("GluonTagSrc") )
        //        pileupJetIdParameters_( iConfig.getParameter<ParameterSet>( "PileupJetIdParameters" ) ),
        //        usePuppi( iConfig.getUntrackedParameter<bool>( "UsePuppi", false ) )
    {
        //        pileupJetIdAlgo_.reset( new PileupJetIdAlgo( pileupJetIdParameters_ ) );
        //qgToken_ = consumes<edm::ValueMap<float> >(edm::InputTag("GluonTagSrc", "qgLikelihood"));
        qgToken  = consumes<edm::ValueMap<float>>( edm::InputTag( qgVariablesInputTag.label(), "qgLikelihood" ) );
        
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
        
        //edm::Handle<edm::ValueMap<float> > qgHandle; 
        //evt.getByToken(qgToken_, qgHandle);
        
        // input QGL
        //edm::Handle<edm::ValueMap<float> >  qgHandle; 
        //evt.getByToken(GluonTagSrc_, qgHandle);
        edm::Handle<edm::ValueMap<float>> qgHandle;
        evt.getByToken( qgToken, qgHandle );
        
        auto_ptr<vector<flashgg::Jet> > jetColl( new vector<flashgg::Jet> );
        
        for( unsigned int i = 0 ; i < jets->size() ; i++ ) {
            Ptr<pat::Jet> pjet = jets->ptrAt( i );
            flashgg::Jet fjet = flashgg::Jet( *pjet );
            
            if (computeSimpleRMS) {
                float sumPtDrSq = 0.;
                float sumPtSq = 0.;
                for ( unsigned k = 0; k < fjet.numberOfSourceCandidatePtrs(); ++k ) {
                    reco::CandidatePtr pfJetConstituent = fjet.sourceCandidatePtr(k);
                    
                    const reco::Candidate* kcand = pfJetConstituent.get();
                    const pat::PackedCandidate* lPack = dynamic_cast<const pat::PackedCandidate *>( kcand );
                    if ( !lPack ) throw cms::Exception( "NoPackedConstituent" ) << " For jet " << i << " failed to get constituent " << k << std::endl;
                    float candPt = kcand->pt();
                    float candDr   = reco::deltaR(*kcand,fjet);
                    sumPtDrSq += candPt*candPt*candDr*candDr;
                    sumPtSq += candPt*candPt;
                }
                
                if (sumPtSq == 0.) throw cms::Exception( "NoConstituents" ) << " For jet " << i << " we get sumPtSq of 0!" << std::endl;
                fjet.setSimpleRMS( sumPtDrSq / sumPtSq );
            }
            
            //--- Retrieve the q/g likelihood
            float qgLikelihood = -99.0;
            if(qgHandle.isValid()) qgLikelihood = ( *qgHandle )[jets->refAt( i )];;
            fjet.setQGL(qgLikelihood);
            //std::cout << "QGL jet["<< i << "] == " << qgLikelihood << std::endl;
            /*
            for( unsigned int j = 0 ; j < diPhotons->size() ; j++ ) {
                Ptr<DiPhotonCandidate> diPhoton = diPhotons->ptrAt( j );
                Ptr<reco::Vertex> vtx = diPhoton->vtx();
                if( !usePuppi ) {
                    if( !fjet.hasPuJetId( vtx ) ) {
                        PileupJetIdentifier lPUJetId = pileupJetIdAlgo_->computeIdVariables( pjet.get(), vtx, *vertexCandidateMap, true );
                        fjet.setPuJetId( vtx, lPUJetId );
                    }
                } else {
                    if( !fjet.hasPuJetId( vtx ) ) {
                        PileupJetIdentifier lPUJetId;
                        lPUJetId.RMS( 0 );
                        lPUJetId.betaStar( 0 );
                        int idFlag( 0 );
                        idFlag += 1 <<  PileupJetIdentifier::kTight;
                        idFlag += 1 <<  PileupJetIdentifier::kMedium;
                        idFlag += 1 <<  PileupJetIdentifier::kLoose;
                        lPUJetId.idFlag( idFlag );
                        fjet.setPuJetId( vtx, lPUJetId ); //temporarily make puppi jets pass
                    }
                }
            }
            if( !usePuppi ) {
                if( primaryVertices->size() > 0 && !fjet.hasPuJetId( primaryVertices->ptrAt( 0 ) ) ) {
                    PileupJetIdentifier lPUJetId = pileupJetIdAlgo_->computeIdVariables( pjet.get(), primaryVertices->ptrAt( 0 ), *vertexCandidateMap, true );
                    fjet.setPuJetId( primaryVertices->ptrAt( 0 ), lPUJetId );
                }
            } else {
                if( !fjet.hasPuJetId( primaryVertices->ptrAt( 0 ) ) ) {
                    PileupJetIdentifier lPUJetId;
                    lPUJetId.RMS( 0 );
                    lPUJetId.betaStar( 0 );
                    int idFlag( 0 );
                    idFlag += 1 <<  PileupJetIdentifier::kTight;
                    idFlag += 1 <<  PileupJetIdentifier::kMedium;
                    idFlag += 1 <<  PileupJetIdentifier::kLoose;
                    lPUJetId.idFlag( idFlag );
                    fjet.setPuJetId( primaryVertices->ptrAt( 0 ), lPUJetId ); //temporarily make puppi jets pass
                }
            }
            */
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

