// system include files
#include <memory>
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "EgammaAnalysis/ElectronTools/interface/EGammaMvaEleEstimator.h"
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

using namespace std;
using namespace edm;

namespace flashgg {

    class ElectronProducer : public edm::EDProducer
    {
    public:
        ElectronProducer( const edm::ParameterSet & );

    private:
        void produce( edm::Event &, const edm::EventSetup & );

        bool verbose_;
        bool applyCuts_;
        edm::EDGetTokenT<View<pat::Electron> > electronToken_;
        edm::EDGetTokenT<View<reco::Vertex> > vertexToken_;
        edm::EDGetTokenT<reco::ConversionCollection> convToken_;
        edm::EDGetTokenT<reco::BeamSpot> beamSpotToken_;

        float _Rho;
        edm::InputTag rhoFixedGrid_;

        string method_;
        vector<string> mvaWeightFiles_;

        // double nontrigmva_;
        EGammaMvaEleEstimator *mvaID_;

    };

    ElectronProducer::ElectronProducer( const ParameterSet &iConfig ):
        electronToken_( consumes<View<pat::Electron> >( iConfig.getParameter<InputTag>( "electronTag" ) ) ),
        vertexToken_( consumes<View<reco::Vertex> >( iConfig.getParameter<InputTag>( "vertexTag" ) ) ),
        convToken_( consumes<reco::ConversionCollection>( iConfig.getParameter<InputTag>( "convTag" ) ) ),
        beamSpotToken_( consumes<reco::BeamSpot >( iConfig.getParameter<InputTag>( "beamSpotTag" ) ) )
    {
        applyCuts_ = iConfig.getUntrackedParameter<bool>( "ApplyCuts", false );
        verbose_ = iConfig.getUntrackedParameter<bool>( "verbose", false );
        method_ = iConfig.getParameter<string>( "method" );
        std::vector<string> fpMvaWeightFiles = iConfig.getParameter<std::vector<std::string> >( "mvaWeightFile" );
        //	eventrhoToken_ = consumes<float>(iConfig.getParameter<edm::InputTag>("Rho"));
        rhoFixedGrid_  = iConfig.getParameter<edm::InputTag>( "rhoFixedGridCollection" );

        mvaID_ = new EGammaMvaEleEstimator();

        EGammaMvaEleEstimator::MVAType type_;

        type_ = EGammaMvaEleEstimator::kNonTrig;

        bool manualCat_ = true;

        string path_mvaWeightFileEleID;
        for( unsigned ifile = 0 ; ifile < fpMvaWeightFiles.size() ; ++ifile ) {
            path_mvaWeightFileEleID = edm::FileInPath( fpMvaWeightFiles[ifile].c_str() ).fullPath();
            mvaWeightFiles_.push_back( path_mvaWeightFileEleID );
        }

        mvaID_->initialize( method_, type_, manualCat_, mvaWeightFiles_ );
        produces<vector<flashgg::Electron> >();
        // nontrigmva_ = 0;
    }

    void ElectronProducer::produce( Event &evt, const EventSetup & )
    {
        //using namespace edm;

        Handle<View<pat::Electron> >  pelectrons;
        evt.getByToken( electronToken_, pelectrons );
        //	const PtrVector<pat::Electron> pelectronPointers = pelectrons->ptrVector();

        _Rho = 0;
        Handle<double> rhoHandle;
        evt.getByLabel( rhoFixedGrid_, rhoHandle );

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


        std::auto_ptr<vector<flashgg::Electron> > elecColl( new vector<flashgg::Electron> );


        for( unsigned int elecIndex = 0; elecIndex < pelectrons->size(); elecIndex++ ) {
            Ptr<pat::Electron> pelec = pelectrons->ptrAt( elecIndex );
            flashgg::Electron felec = flashgg::Electron( *pelec );
            // double nontrigmva_ = -999999;
            double Aeff = 0;

            float pelec_eta = fabs( pelec->superCluster()->eta() );
            float pelec_pt = pelec->pt();

            double nontrigmva = mvaID_->mvaValue( *pelec, *vtxs->ptrAt( 0 ), _Rho, verbose_ );
            felec.setNonTrigMVA( ( float )nontrigmva );

            if( applyCuts_ && nontrigmva < 0.9 ) { continue; }
            if( applyCuts_ && ( pelec_eta > 2.5 || ( pelec_eta > 1.442 && pelec_eta < 1.566 ) ) ) { continue; }

            if( pelec_eta < 1.0 )                   	{ Aeff = 0.135; }
            if( pelec_eta >= 1.0 && pelec_eta < 1.479 ) 	{ Aeff = 0.168; }
            if( pelec_eta >= 1.479 && pelec_eta < 2.0 ) 	{ Aeff = 0.068; }
            if( pelec_eta >= 2.0 && pelec_eta < 2.2 )   	{ Aeff = 0.116; }
            if( pelec_eta >= 2.2 && pelec_eta < 2.3 )   	{ Aeff = 0.162; }
            if( pelec_eta >= 2.3 && pelec_eta < 2.4 )   	{ Aeff = 0.241; }
            if( pelec_eta >= 2.4 )                  	{ Aeff = 0.23; }

            float	pelec_iso = pelec->chargedHadronIso() + std::max( pelec->neutralHadronIso() + pelec->photonIso() - _Rho * Aeff, 0. );
            felec.setStandardHggIso( pelec_iso );

            felec.setHasMatchedConversion( ConversionTools::hasMatchedConversion( *pelec, convs, vertexPoint ) );

            if( applyCuts_ ) {
                if( pelec_iso / pelec_pt > 0.15 ) { continue; }
                //if{} //d0
                //if{} //dz
                /// if(pelec->gsfTrack()->trackerExpectedHitsInner().numberOfHits()>1)continue;
                if( pelec->gsfTrack()->hitPattern().numberOfHits( reco::HitPattern::MISSING_INNER_HITS ) > 1 ) { continue; }
                if( ConversionTools::hasMatchedConversion( *pelec, convs, vertexPoint ) ) { continue; }
            } // otherwise ALL pat::Electrons are converted to flashgg::Electrons -- this is default behavior, especially for studies

            elecColl->push_back( felec );
        }
        evt.put( elecColl );
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

