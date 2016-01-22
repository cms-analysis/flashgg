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
        edm::EDGetTokenT<double> rhoToken_;

        EffectiveAreas effectiveAreas_;
        //        edm::EDGetTokenT<edm::ValueMap<bool> > eleMVAMediumIdMapToken_;
        //        edm::EDGetTokenT<edm::ValueMap<bool> > eleMVATightIdMapToken_;
    };

    ElectronProducer::ElectronProducer( const ParameterSet &iConfig ):
        electronToken_( consumes<View<pat::Electron> >( iConfig.getParameter<InputTag>( "electronTag" ) ) ),
        vertexToken_( consumes<View<reco::Vertex> >( iConfig.getParameter<InputTag>( "vertexTag" ) ) ),
        convToken_( consumes<reco::ConversionCollection>( iConfig.getParameter<InputTag>( "convTag" ) ) ),
        beamSpotToken_( consumes<reco::BeamSpot >( iConfig.getParameter<InputTag>( "beamSpotTag" ) ) ),
        mvaValuesMapToken_(consumes<edm::ValueMap<float> >(iConfig.getParameter<edm::InputTag>("mvaValuesMap" ) ) ),
        rhoToken_(consumes<double>(iConfig.getParameter <edm::InputTag>("rhoFixedGridCollection"))),
        effectiveAreas_((iConfig.getParameter<edm::FileInPath>("effAreasConfigFile")).fullPath())
        //        eleMVAMediumIdMapToken_(consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("eleMVAMediumIdMap"))),
        //        eleMVATightIdMapToken_(consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("eleMVATightIdMap")))
    {
        

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

        
        //        edm::Handle<edm::ValueMap<bool> > mediumMVA_wp;
        //        edm::Handle<edm::ValueMap<bool> > tightMVA_wp; 
        //        evt.getByToken(eleMVAMediumIdMapToken_,mediumMVA_wp);
        //        evt.getByToken(eleMVATightIdMapToken_,tightMVA_wp);


        std::auto_ptr<vector<flashgg::Electron> > elecColl( new vector<flashgg::Electron> );

        for( unsigned int elecIndex = 0; elecIndex < pelectrons->size(); elecIndex++ ) {
            Ptr<pat::Electron> pelec = pelectrons->ptrAt( elecIndex );
            flashgg::Electron felec = flashgg::Electron( *pelec );

            double Aeff = 0;
            Aeff = effectiveAreas_.getEffectiveArea( fabs( pelec->superCluster()->eta()));

            float pelec_eta = fabs( pelec->superCluster()->eta() );
            //float pelec_pt = pelec->pt();

            double nontrigmva = (*mvaValues)[pelec];
            felec.setNonTrigMVA( ( float )nontrigmva );

            if( pelec_eta > 2.5 || ( pelec_eta > 1.4442 && pelec_eta < 1.566 ) ) { continue; }

            float	pelec_iso = pelec->chargedHadronIso() + std::max( pelec->neutralHadronIso() + pelec->photonIso() - rho * Aeff, 0. );
            felec.setStandardHggIso( pelec_iso );
            felec.setHasMatchedConversion( ConversionTools::hasMatchedConversion( *pelec, convs, vertexPoint ) );

            //   bool passMVAMediumId = (*mediumMVA_wp)[pelec];
            //   bool passMVATightId = (*tightMVA_wp)[pelec];
            
            // felec.setPassEGWP90(passMVAMediumId);
            // felec.setPassEGWP80(passMVATightId);


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

