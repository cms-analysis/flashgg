// system include files
#include <memory>
// user include file
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "flashgg/DataFormats/interface/Electron.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/IPTools/interface/IPTools.h"
#include "RecoEgamma/EgammaTools/interface/EffectiveAreas.h"

using namespace std;
using namespace edm;
using namespace reco;

namespace flashgg {

    class ElectronProducer : public edm::EDProducer
    {
    public:
        ElectronProducer( const edm::ParameterSet & );

    private:
        void produce( edm::Event &, const edm::EventSetup & );

        edm::EDGetTokenT<View<pat::Electron> > electronToken_;
        edm::EDGetTokenT<View<reco::Vertex> > vertexToken_;
        edm::EDGetTokenT<reco::ConversionCollection> convToken_;
        edm::EDGetTokenT<reco::BeamSpot> beamSpotToken_;
        edm::EDGetTokenT<edm::ValueMap<float> > mvaValuesMapToken_;  
        // edm::EDGetTokenT<edm::ValueMap<float> > mvaNoIsoValuesMapToken_;
        edm::EDGetTokenT<double> rhoToken_;

        EffectiveAreas effectiveAreas_;
        // edm::EDGetTokenT<edm::ValueMap<bool> > eleMVALooseIdMapToken_;
        edm::EDGetTokenT<edm::ValueMap<bool> > eleMVAMediumIdMapToken_;
        edm::EDGetTokenT<edm::ValueMap<bool> > eleMVATightIdMapToken_;
        // edm::EDGetTokenT<edm::ValueMap<bool> > eleMVALooseNoIsoIdMapToken_;
        // edm::EDGetTokenT<edm::ValueMap<bool> > eleMVAMediumNoIsoIdMapToken_;
        // edm::EDGetTokenT<edm::ValueMap<bool> > eleMVATightNoIsoIdMapToken_;

        edm::EDGetTokenT<edm::ValueMap<bool> > eleLooseIdMapToken_;
        edm::EDGetTokenT<edm::ValueMap<bool> > eleMediumIdMapToken_;
        edm::EDGetTokenT<edm::ValueMap<bool> > eleTightIdMapToken_;
        edm::EDGetTokenT<edm::ValueMap<bool> > eleVetoIdMapToken_;

        edm::EDGetTokenT<View<pat::PackedCandidate> > pfcandidateToken_;
        double elecminiso_r_min_ = 0.05;
        double elecminiso_r_max_ = 0.2;
        double elecminiso_kt_scale_ = 10.;
        double elecEBminiso_deadcone_ch_ = 0.0;
        double elecEBminiso_deadcone_pu_ = 0.0;
        double elecEBminiso_deadcone_ph_ = 0.0;
        double elecEBminiso_deadcone_nh_ = 0.0;
        double elecEEminiso_deadcone_ch_ = 0.015;
        double elecEEminiso_deadcone_pu_ = 0.015;
        double elecEEminiso_deadcone_ph_ = 0.08;
        double elecEEminiso_deadcone_nh_ = 0.0;
        double elecminiso_ptThresh_ = 0.0;
    };

    ElectronProducer::ElectronProducer( const ParameterSet &iConfig ):
        electronToken_( consumes<View<pat::Electron> >( iConfig.getParameter<InputTag>( "electronTag" ) ) ),
        vertexToken_( consumes<View<reco::Vertex> >( iConfig.getParameter<InputTag>( "vertexTag" ) ) ),
        convToken_( consumes<reco::ConversionCollection>( iConfig.getParameter<InputTag>( "convTag" ) ) ),
        beamSpotToken_( consumes<reco::BeamSpot >( iConfig.getParameter<InputTag>( "beamSpotTag" ) ) ),
        mvaValuesMapToken_(consumes<edm::ValueMap<float> >(iConfig.getParameter<edm::InputTag>("mvaValuesMap" ) ) ),
        // mvaNoIsoValuesMapToken_(consumes<edm::ValueMap<float> >(iConfig.getParameter<edm::InputTag>("mvaNoIsoValuesMap" ) ) ),
        rhoToken_(consumes<double>(iConfig.getParameter <edm::InputTag>("rhoFixedGridCollection"))),
        effectiveAreas_((iConfig.getParameter<edm::FileInPath>("effAreasConfigFile")).fullPath()),
        // eleMVALooseIdMapToken_(consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("eleMVALooseIdMap"))),
        eleMVAMediumIdMapToken_(consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("eleMVAMediumIdMap"))),
        eleMVATightIdMapToken_(consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("eleMVATightIdMap"))),
        // eleMVALooseNoIsoIdMapToken_(consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("eleMVALooseNoIsoIdMap"))),
        // eleMVAMediumNoIsoIdMapToken_(consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("eleMVAMediumNoIsoIdMap"))),
        // eleMVATightNoIsoIdMapToken_(consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("eleMVATightNoIsoIdMap"))),
        eleLooseIdMapToken_(consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("eleLooseIdMap"))),
        eleMediumIdMapToken_(consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("eleMediumIdMap"))),
        eleTightIdMapToken_(consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("eleTightIdMap"))),
        eleVetoIdMapToken_(consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("eleVetoIdMap"))),
        pfcandidateToken_( consumes<View<pat::PackedCandidate> >( iConfig.getParameter<InputTag> ( "pfCandidatesTag" ) ) )
    {
        

        elecminiso_r_min_ = iConfig.getParameter<double>( "elecminiso_r_min" );
        elecminiso_r_max_ = iConfig.getParameter<double>( "elecminiso_r_max" );
        elecminiso_kt_scale_ = iConfig.getParameter<double>( "elecminiso_kt_scale" );
        elecEBminiso_deadcone_ch_ = iConfig.getParameter<double>( "elecEBminiso_deadcone_ch" );
        elecEBminiso_deadcone_pu_ = iConfig.getParameter<double>( "elecEBminiso_deadcone_pu" );
        elecEBminiso_deadcone_ph_ = iConfig.getParameter<double>( "elecEBminiso_deadcone_ph" );
        elecEBminiso_deadcone_nh_ = iConfig.getParameter<double>( "elecEBminiso_deadcone_nh" );
        elecEEminiso_deadcone_ch_ = iConfig.getParameter<double>( "elecEEminiso_deadcone_ch" );
        elecEEminiso_deadcone_pu_ = iConfig.getParameter<double>( "elecEEminiso_deadcone_pu" );
        elecEEminiso_deadcone_ph_ = iConfig.getParameter<double>( "elecEEminiso_deadcone_ph" );
        elecEEminiso_deadcone_nh_ = iConfig.getParameter<double>( "elecEEminiso_deadcone_nh" );
        elecminiso_ptThresh_ = iConfig.getParameter<double>( "elecminiso_ptThresh" );

        produces<vector<flashgg::Electron> >();
        // nontrigmva_ = 0;
    }

    void ElectronProducer::produce( Event &evt, const EventSetup & )
    {
        //using namespace edm;

        Handle<View<pat::Electron> >  pelectrons;
        evt.getByToken( electronToken_, pelectrons );
        //	const PtrVector<pat::Electron> pelectronPointers = pelectrons->ptrVector();

        double rho=0;
        edm::Handle< double > rhoH;
        evt.getByToken(rhoToken_,rhoH);
        rho = *rhoH;

        Handle<View<reco::Vertex> >  vtxs;
        evt.getByToken( vertexToken_, vtxs );
        //	const PtrVector<reco::Vertex> vertexPointers = vtxs->ptrVector();

        Handle<reco::ConversionCollection> convs;
        evt.getByToken( convToken_, convs );
        //		const PtrVector<reco::Conversion> convPointers = convs->ptrVector();

        Handle<reco::BeamSpot> recoBeamSpotHandle;
        evt.getByToken( beamSpotToken_, recoBeamSpotHandle );
        math::XYZPoint vertexPoint;
        if( recoBeamSpotHandle.isValid() )
        { vertexPoint = recoBeamSpotHandle->position(); }

        edm::Handle<edm::ValueMap<float> > mvaValues;
        evt.getByToken(mvaValuesMapToken_,mvaValues);

        // edm::Handle<edm::ValueMap<bool> > looseMVA_wp;
        edm::Handle<edm::ValueMap<bool> > mediumMVA_wp;
        edm::Handle<edm::ValueMap<bool> > tightMVA_wp;
        // 
        // evt.getByToken(eleMVAMediumIdMapToken_,looseMVA_wp);
        evt.getByToken(eleMVAMediumIdMapToken_,mediumMVA_wp);
        evt.getByToken(eleMVATightIdMapToken_,tightMVA_wp);
        
        // edm::Handle<edm::ValueMap<bool> > loosenoisoMVA_wp;
        // edm::Handle<edm::ValueMap<bool> > mediumnoisoMVA_wp;
        // edm::Handle<edm::ValueMap<bool> > tightnoisoMVA_wp; 
        // evt.getByToken(eleMVAMediumNoIsoIdMapToken_,loosenoisoMVA_wp);
        // evt.getByToken(eleMVAMediumNoIsoIdMapToken_,mediumnoisoMVA_wp);
        // evt.getByToken(eleMVATightNoIsoIdMapToken_,tightnoisoMVA_wp);

        edm::Handle<edm::ValueMap<bool> > loose_wp;
        edm::Handle<edm::ValueMap<bool> > medium_wp;
        edm::Handle<edm::ValueMap<bool> > tight_wp; 
        edm::Handle<edm::ValueMap<bool> > veto_wp;
        evt.getByToken(eleLooseIdMapToken_,loose_wp);
        evt.getByToken(eleMediumIdMapToken_,medium_wp);
        evt.getByToken(eleTightIdMapToken_,tight_wp);
        evt.getByToken(eleVetoIdMapToken_,veto_wp);       

        Handle<View<pat::PackedCandidate> > pfcandidates;
        evt.getByToken( pfcandidateToken_, pfcandidates );
        const std::vector<edm::Ptr<pat::PackedCandidate> > &pfcands = pfcandidates->ptrs();

        std::unique_ptr<vector<flashgg::Electron> > elecColl( new vector<flashgg::Electron> );

        for( unsigned int elecIndex = 0; elecIndex < pelectrons->size(); elecIndex++ ) {
            Ptr<pat::Electron> pelec = pelectrons->ptrAt( elecIndex );
            flashgg::Electron felec = flashgg::Electron( *pelec );

            float Aeff = 0;
            Aeff = effectiveAreas_.getEffectiveArea( fabs( pelec->superCluster()->eta()));
                        
            float pelec_eta = fabs( pelec->superCluster()->eta() );

            double nontrigmva = (*mvaValues)[pelec];
            felec.setNonTrigMVA( ( float )nontrigmva );

            if( pelec_eta > 2.5 || ( pelec_eta > 1.4442 && pelec_eta < 1.566 ) ) { continue; }

            GsfElectron::PflowIsolationVariables pfIso = pelec->pfIsolationVariables();
            float pelec_egiso=( pfIso.sumChargedHadronPt + std::max( 0.0d, pfIso.sumNeutralHadronEt + pfIso.sumPhotonEt - Aeff*rho) ) / pelec->pt() ;
        
            felec.setStandardHggIso( pelec_egiso );

            felec.setHasMatchedConversion( ConversionTools::hasMatchedConversion( *pelec->superCluster(), *convs.product(), vertexPoint ) );

            // bool passMVALooseId = (*looseMVA_wp)[pelec];
            bool passMVAMediumId = (*mediumMVA_wp)[pelec];
            bool passMVATightId = (*tightMVA_wp)[pelec];

            // bool passMVALooseNoIsoId = (*loosenoisoMVA_wp)[pelec];
            // bool passMVAMediumNoIsoId = (*mediumnoisoMVA_wp)[pelec];
            // bool passMVATightNoIsoId = (*tightnoisoMVA_wp)[pelec];

            bool passLooseId = (*loose_wp)[pelec];
            bool passMediumId = (*medium_wp)[pelec];
            bool passTightId = (*tight_wp)[pelec];
            bool passVetoId = (*veto_wp)[pelec];
            
          
            felec.setPassMVATightId(passMVATightId);
            felec.setPassMVAMediumId(passMVAMediumId);
            // felec.setPassMVALooseId(passMVALooseId);
            // felec.setPassMVATightNoIsoId(passMVATightNoIsoId);
            // felec.setPassMVAMediumNoIsoId(passMVAMediumNoIsoId);
            // felec.setPassMVALooseNoIsoId(passMVALooseNoIsoId);
            felec.setPassTightId(passTightId);
            felec.setPassMediumId(passMediumId);
            felec.setPassLooseId(passLooseId);
            felec.setPassVetoId(passVetoId);

            //MiniIsolation : https://twiki.cern.ch/twiki/bin/view/CMS/MiniIsolationSUSY
            float fggMiniIsoSumRel_ = 99999.;
            double iso_nh_(0.); double iso_ch_(0.); 
            double iso_ph_(0.); double iso_pu_(0.);
            double elecminiso_deadcone_ch_ = elecEBminiso_deadcone_ch_;
            double elecminiso_deadcone_pu_ = elecEBminiso_deadcone_pu_;
            double elecminiso_deadcone_ph_ = elecEBminiso_deadcone_ph_;
            double elecminiso_deadcone_nh_ = elecEBminiso_deadcone_nh_;
            if(pelec_eta > 1.479){
                elecminiso_deadcone_ch_ = elecEEminiso_deadcone_ch_;
                elecminiso_deadcone_pu_ = elecEEminiso_deadcone_pu_;
                elecminiso_deadcone_ph_ = elecEEminiso_deadcone_ph_;
                elecminiso_deadcone_nh_ = elecEEminiso_deadcone_nh_;
            }
            double par_pt_ =  pelec->pt();
            if(par_pt_ > 5.){
                double r_iso_ = max(elecminiso_r_min_,min(elecminiso_r_max_, elecminiso_kt_scale_/par_pt_ ));
                for( unsigned int ipfc = 0 ; ipfc < pfcands.size() ; ipfc++ ) {
                    Ptr<pat::PackedCandidate> pfc = pfcands[ ipfc ];
                    if (abs(pfc->pdgId())<7) continue;
                    
                    double dr_ = reco::deltaR( *pfc, *pelec );
                    //double dr_ = deltaR(pfc->eta(), pfc->phi(), pelec->eta(), pelec->phi() );
                    if (dr_ > r_iso_) continue;
                    
                    //////////////////  NEUTRALS  /////////////////////////
                    if (pfc->charge()==0){
                        if (pfc->pt()>elecminiso_ptThresh_) {
                            /////////// PHOTONS ////////////
                            if (abs(pfc->pdgId())==22) {
                                if(dr_ < elecminiso_deadcone_ph_) continue;
                                iso_ph_ += pfc->pt();
                                /////////// NEUTRAL HADRONS ////////////
                            } else if (abs(pfc->pdgId())==130) {
                                if(dr_ < elecminiso_deadcone_nh_) continue;
                                iso_nh_ += pfc->pt();
                            }
                        }
                        //////////////////  CHARGED from PV  /////////////////////////
                    } else if (pfc->fromPV()>1){
                        if (abs(pfc->pdgId())==211) {
                            if(dr_ < elecminiso_deadcone_ch_) continue;
                            iso_ch_ += pfc->pt();
                        }
                        //////////////////  CHARGED from PU  /////////////////////////
                    } else {
                        if (pfc->pt()>elecminiso_ptThresh_){
                            if(dr_ < elecminiso_deadcone_pu_) continue;
                            iso_pu_ += pfc->pt();
                        }
                    }
                }// pfcand loop
                //fggMiniIsoSumRel_ = ( iso_ch_ + max( 0., iso_nh_ + iso_ph_ - 0.5 * iso_pu_) ) / par_pt_ ;
                fggMiniIsoSumRel_ = ( iso_ch_ + max( 0., iso_nh_ + iso_ph_ - Aeff*rho*(r_iso_/0.3)*(r_iso_/0.3) ) ) / par_pt_ ;
            }
            felec.setFggMiniIsoSumRel( fggMiniIsoSumRel_ );

            elecColl->push_back( felec );
           
        }
        evt.put( std::move( elecColl ) );
    }
}

typedef flashgg::ElectronProducer FlashggElectronProducer;
DEFINE_FWK_MODULE( FlashggElectronProducer );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

