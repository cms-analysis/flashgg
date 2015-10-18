#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "flashgg/DataFormats/interface/PhotonJetCandidate.h"
#include "flashgg/MicroAOD/interface/PhotonIdUtils.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "flashgg/MicroAOD/interface/VertexSelectorBase.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "flashgg/DataFormats/interface/VertexCandidateMap.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"

#include "TLorentzVector.h"
#include <map>

using namespace edm;
using namespace std;

namespace flashgg {

    class PhotonJetProducer : public EDProducer
    {

    public:
        PhotonJetProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;
        EDGetTokenT<View<flashgg::Photon> > photonToken_;
        EDGetTokenT<View<pat::Jet> > jetToken_;

        EDGetTokenT<View<reco::Vertex> > vertexToken_;
        EDGetTokenT< VertexCandidateMap > vertexCandidateMapToken_;
        unique_ptr<VertexSelectorBase> vertexSelector_;
        EDGetTokenT<View<reco::Conversion> > conversionToken_;
        EDGetTokenT<reco::BeamSpot>  beamSpotToken_;
        EDGetTokenT<View<reco::Conversion> > conversionTokenSingleLeg_;

        bool useSingleLeg_;

        double minJetPt_;
        double minPhotonPt_;
    };

    PhotonJetProducer::PhotonJetProducer( const ParameterSet &iConfig ) :
        photonToken_( consumes<View<flashgg::Photon> >( iConfig.getParameter<InputTag> ( "PhotonTag" ) ) ),
        jetToken_( consumes<View<pat::Jet> >( iConfig.getParameter<InputTag> ( "JetTag" ) ) ),
        vertexToken_( consumes<View<reco::Vertex> >( iConfig.getParameter<InputTag> ( "VertexTag" ) ) ),
        vertexCandidateMapToken_( consumes<VertexCandidateMap>( iConfig.getParameter<InputTag>( "VertexCandidateMapTag" ) ) ),
        conversionToken_( consumes<View<reco::Conversion> >( iConfig.getParameter<InputTag>( "ConversionTag" ) ) ),
        beamSpotToken_( consumes<reco::BeamSpot >( iConfig.getParameter<InputTag>( "beamSpotTag" ) ) ),
        conversionTokenSingleLeg_( consumes<View<reco::Conversion> >( iConfig.getParameter<InputTag>( "ConversionTagSingleLeg" ) ) )
    {
        bool default_useSingleLeg_ = true;
        const std::string &VertexSelectorName = iConfig.getParameter<std::string>( "VertexSelectorName" );
        vertexSelector_.reset( FlashggVertexSelectorFactory::get()->create( VertexSelectorName, iConfig ) );
        useSingleLeg_ = iConfig.getUntrackedParameter<bool>( "useSingleLeg", default_useSingleLeg_ );
        minJetPt_ = iConfig.getUntrackedParameter<double>( "minJetPt", 30. );
        minPhotonPt_ = iConfig.getUntrackedParameter<double>( "minPhotonPt", 30. );
        useSingleLeg_ = iConfig.getUntrackedParameter<bool>( "useSingleLeg", default_useSingleLeg_ );
        produces<vector<flashgg::PhotonJetCandidate> >();
    }

    void PhotonJetProducer::produce( Event &evt, const EventSetup & )
    {
        // input flashgg photons
        Handle<View<flashgg::Photon> > photons;
        evt.getByToken( photonToken_, photons );

        // input jets
        Handle<View<pat::Jet> > jets;
        evt.getByToken( jetToken_, jets );

        Handle<View<reco::Vertex> > primaryVertices;
        evt.getByToken( vertexToken_, primaryVertices );

        Handle<VertexCandidateMap> vertexCandidateMap;
        evt.getByToken( vertexCandidateMapToken_, vertexCandidateMap );

        Handle<View<reco::Conversion> > conversions;
        evt.getByToken( conversionToken_, conversions );

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

        auto_ptr<vector<PhotonJetCandidate> > PhotonJetColl( new vector<PhotonJetCandidate> );

        // --- Photon selection (min pt, photon id)
        for ( unsigned int i = 0 ; i < photons->size() ; i++ ){
            Ptr<flashgg::Photon> photon = photons->ptrAt( i );
            if (photon->pt() < minPhotonPt_) continue;
            // photon ID cut based - medium
            if ( photon->isEB() ){
                if ( photon->hadronicOverEm() > 0.05 )  continue;
                if ( photon->full5x5_sigmaIetaIeta() > 0.01) continue;
                // ... add isolation cuts
            }
            if ( photon->isEE() ){
                if ( photon->hadronicOverEm() > 0.05 )  continue;
                if ( photon->full5x5_sigmaIetaIeta() > 0.0267) continue;
                // ... add isolation cuts
            }
                        
            // -- loop over jets
            for( unsigned int j = 0 ; j < jets->size() ; j++ ) {
                Ptr<pat::Jet> jet = jets->ptrAt( j );
                if ( jet->pt() < minJetPt_ ) continue;
                if ( fabs(jet->eta()) > 2.5 ) continue;
                // -- check that the jet is not overlapping with the photon
                float dR = reco::deltaR(photon->eta(), photon->phi(), jet->eta(), jet->phi());
                if ( dR < 0.4 ) continue;
                // -- check sumPttracks > 30 
                TLorentzVector pTrks(0.,0.,0.,0.);
                for (unsigned int icand = 0; icand < jet->numberOfDaughters(); icand++){
                    const reco::Candidate *jetconst = jet->daughter(icand);
                    //std::cout << jetconst->charge() << " " << jetconst->px()<< " "<< jetconst->py() << " " << jetconst->pz()<<std::endl;
                    if ( jetconst->charge()==0 ) continue;
                    TLorentzVector pTrk(jetconst->px(), jetconst->py(), jetconst->pz(), jetconst->energy());
                    pTrks+=pTrk;
                }
                //std::cout << "track sum pt = "<< pTrks.Pt() <<std::endl;
                if (pTrks.Pt()<30) continue;

                // -- now build gamma+jet candidate 
                Ptr<reco::Vertex> pvx = vertexSelector_->select( photon, jet, primaryVertices->ptrs(), *vertexCandidateMap, conversions->ptrs(), conversionsSingleLeg->ptrs(),
                                                                 vertexPoint, useSingleLeg_ );


                int ivtx = 0;
                for( unsigned int k = 0; k < primaryVertices->size() ; k++ )
                    if( pvx == primaryVertices->ptrAt( k ) ) {
                        ivtx = k;
                        break;
                    }


                PhotonJetCandidate photonjet( photon, jet, pvx );
                photonjet.setVertexIndex( ivtx );
                photonjet.setDZfromRecoPV(pvx->position().z() - primaryVertices->ptrAt(0)->position().z());
                
                TVector3 Photon1Dir;
                TVector3 Photon1Dir_uv;
                TLorentzVector p1, p2;
                Photon1Dir.SetXYZ( photon->superCluster()->position().x() - pvx->position().x(), photon->superCluster()->position().y() - pvx->position().y(),
                                   photon->superCluster()->position().z() - pvx->position().z() );
                Photon1Dir_uv = Photon1Dir.Unit() * photon->superCluster()->rawEnergy();
                p1.SetPxPyPzE( Photon1Dir_uv.x(), Photon1Dir_uv.y(), Photon1Dir_uv.z(), photon->superCluster()->rawEnergy() );
                p2.SetPxPyPzE( jet->px(), jet->py(), jet->pz(), jet->energy());
                photonjet.setPhotonJetPt((p1+p2).Pt());
                
                // Obviously the last selection has to be for this diphoton or this is wrong
                vertexSelector_->writeInfoFromLastSelectionTo( photonjet );

                PhotonJetColl -> push_back(photonjet);

            }//-- end loop over jets
            
        }// end loop over photons
        
        evt.put(PhotonJetColl);
        
    }
}

typedef flashgg::PhotonJetProducer FlashggPhotonJetProducer;
DEFINE_FWK_MODULE( FlashggPhotonJetProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

