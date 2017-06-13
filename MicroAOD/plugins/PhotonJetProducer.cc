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
        EDGetTokenT<double> rhoTag_;
        EDGetTokenT<View<reco::Conversion> > conversionTokenSingleLeg_;

        bool useSingleLeg_;

        double minJetPt_;
        double maxJetEta_;
        double minPhotonPt_;
        double minPhotEBHoE_;
        double minPhotEEHoE_;
        double minPhotEBsietaieta_;
        double minPhotEEsietaieta_;
        double minPhotEBChgIso_;
        double minPhotEEChgIso_;
        double minPhotEBNeuIso_;
        double minPhotEENeuIso_;
        double minPhotEBPhoIso_;
        double minPhotEEPhoIso_;

        //https://indico.cern.ch/event/370513/contribution/1/attachments/1183744/1715032/SP15_25_4th_final.pdf
        int iphotIsolnAreaValN_;
        vector<double> photIsolnEAreaVal_;
        vector<double> photIsolnEAreaChgHad_;
        vector<double> photIsolnEAreaNeuHad_;
        vector<double> photIsolnEAreaPhot_;
    };

    PhotonJetProducer::PhotonJetProducer( const ParameterSet &iConfig ) :
        photonToken_( consumes<View<flashgg::Photon> >( iConfig.getParameter<InputTag> ( "PhotonTag" ) ) ),
        jetToken_( consumes<View<pat::Jet> >( iConfig.getParameter<InputTag> ( "JetTag" ) ) ),
        vertexToken_( consumes<View<reco::Vertex> >( iConfig.getParameter<InputTag> ( "VertexTag" ) ) ),
        vertexCandidateMapToken_( consumes<VertexCandidateMap>( iConfig.getParameter<InputTag>( "VertexCandidateMapTag" ) ) ),
        conversionToken_( consumes<View<reco::Conversion> >( iConfig.getParameter<InputTag>( "ConversionTag" ) ) ),
        beamSpotToken_( consumes<reco::BeamSpot >( iConfig.getParameter<InputTag>( "beamSpotTag" ) ) ),
        rhoTag_( consumes<double>( iConfig.getParameter<InputTag>( "rhoTag" ) ) ),
        conversionTokenSingleLeg_( consumes<View<reco::Conversion> >( iConfig.getParameter<InputTag>( "ConversionTagSingleLeg" ) ) )
    {
        const std::string &VertexSelectorName = iConfig.getParameter<std::string>( "VertexSelectorName" );
        vertexSelector_.reset( FlashggVertexSelectorFactory::get()->create( VertexSelectorName, iConfig ) );
        useSingleLeg_ = iConfig.getParameter<bool>( "useSingleLeg" );
        minJetPt_ = iConfig.getParameter<double>( "minJetPt" );
        maxJetEta_ = iConfig.getParameter<double>( "maxJetEta" );
        minPhotonPt_ = iConfig.getParameter<double>( "minPhotonPt" );
        minPhotEBHoE_ = iConfig.getParameter<double>( "minPhotEBHoE" );
        minPhotEEHoE_ = iConfig.getParameter<double>( "minPhotEEHoE" );
        minPhotEBsietaieta_ = iConfig.getParameter<double>( "minPhotEBsietaieta" );
        minPhotEEsietaieta_ = iConfig.getParameter<double>( "minPhotEEsietaieta" );
        minPhotEBChgIso_ = iConfig.getParameter<double>( "minPhotEBChgIso" );
        minPhotEEChgIso_ = iConfig.getParameter<double>( "minPhotEEChgIso" );
        minPhotEBNeuIso_ = iConfig.getParameter<double>( "minPhotEBNeuIso" );
        minPhotEENeuIso_ = iConfig.getParameter<double>( "minPhotEENeuIso" );
        minPhotEBPhoIso_ = iConfig.getParameter<double>( "minPhotEBPhoIso" );
        minPhotEEPhoIso_ = iConfig.getParameter<double>( "minPhotEEPhoIso" );
        iphotIsolnAreaValN_ = iConfig.getParameter<int>( "iphotIsolnAreaValN" );
        photIsolnEAreaVal_ = iConfig.getParameter<vector<double>>( "photIsolnEAreaVal" );
        photIsolnEAreaChgHad_ = iConfig.getParameter<vector<double>>( "photIsolnEAreaChgHad" );
        photIsolnEAreaNeuHad_ = iConfig.getParameter<vector<double>>( "photIsolnEAreaNeuHad" );
        photIsolnEAreaPhot_ = iConfig.getParameter<vector<double>>( "photIsolnEAreaPhot" );
        
        produces<vector<flashgg::PhotonJetCandidate>>();
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

        edm::Handle<double>  rho;
        evt.getByToken(rhoTag_,rho);
        double rho_    = *rho;

        unique_ptr<vector<PhotonJetCandidate> > PhotonJetColl( new vector<PhotonJetCandidate> );

        // --- Photon selection (min pt, photon id)
        for ( unsigned int i = 0 ; i < photons->size() ; i++ ){
            Ptr<flashgg::Photon> photon = photons->ptrAt( i );
            double photPt_ = photon->pt();
            if (photPt_ < minPhotonPt_) continue;
            // photon ID cut based : //https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedPhotonIdentificationRun2#Recommended_Working_points_for_2
            if ( !photon->passElectronVeto() ) continue;
            int iphotEA_ = iphotIsolnAreaValN_ - 1;
            for (int iEA = 0; iEA < iphotIsolnAreaValN_; iEA++){
                if (fabs(photon->eta()) < photIsolnEAreaVal_.at(iEA)) {
                    iphotEA_ = iEA;
                    break;
                }
            }

            if ( photon->isEB() ){
                if ( photon->hadronicOverEm() > minPhotEBHoE_ )  continue;
                if ( photon->full5x5_sigmaIetaIeta() > minPhotEBsietaieta_) continue;
                double photChargedHadronIso_ = max(photon->chargedHadronIso() - rho_*photIsolnEAreaChgHad_[iphotEA_], 0.);
                if (photChargedHadronIso_ > minPhotEBChgIso_ ) continue;
                double photNeutralHadronIso_ = max(photon->neutralHadronIso() - rho_*photIsolnEAreaNeuHad_[iphotEA_], 0.);
                if (photNeutralHadronIso_ - 0.014*photPt_ - 0.000019*photPt_*photPt_ > minPhotEBNeuIso_ ) continue;
                double photPhotonIso_ = max(photon->photonIso() - rho_*photIsolnEAreaPhot_[iphotEA_], 0.);
                if (photPhotonIso_ - 0.0053*photPt_ > minPhotEBPhoIso_ ) continue;
            }
            if ( photon->isEE() ){
                if ( photon->hadronicOverEm() > minPhotEEHoE_ )  continue;
                if ( photon->full5x5_sigmaIetaIeta() > minPhotEEsietaieta_ ) continue;
                double photChargedHadronIso_ = max(photon->chargedHadronIso() - rho_*photIsolnEAreaChgHad_[iphotEA_], 0.);
                if (photChargedHadronIso_ > minPhotEEChgIso_ ) continue;
                double photNeutralHadronIso_ = max(photon->neutralHadronIso() - rho_*photIsolnEAreaNeuHad_[iphotEA_], 0.);
                if (photNeutralHadronIso_ - 0.0139*photPt_ - 0.000025*photPt_*photPt_ > minPhotEENeuIso_ ) continue;
                double photPhotonIso_ = max(photon->photonIso() - rho_*photIsolnEAreaPhot_[iphotEA_], 0.);
                if (photPhotonIso_ - 0.0034*photPt_ > minPhotEEPhoIso_ ) continue;
            }
                        
            // -- loop over jets
            for( unsigned int j = 0 ; j < jets->size() ; j++ ) {
                Ptr<pat::Jet> jet = jets->ptrAt( j );
                if ( jet->pt() < minJetPt_ ) continue;
                if ( fabs(jet->eta()) > maxJetEta_ ) continue;
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
                if (pTrks.Pt()< minJetPt_ ) continue;

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

                int iJetvtx = 0;
                photonjet.setVertexIndexJet( iJetvtx );
                photonjet.setDZfromRecoPV(pvx->position().z() - primaryVertices->ptrAt(iJetvtx)->position().z());
                
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
        
        evt.put( std::move(PhotonJetColl) );
        
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

