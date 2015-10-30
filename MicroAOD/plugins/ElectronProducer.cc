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

            bool verbose_;
            bool applyCuts_;
            edm::EDGetTokenT<View<pat::Electron> > electronToken_;
            edm::EDGetTokenT<View<reco::Vertex> > vertexToken_;
            edm::EDGetTokenT<reco::ConversionCollection> convToken_;
            edm::EDGetTokenT<reco::BeamSpot> beamSpotToken_;
            edm::EDGetTokenT<edm::ValueMap<float> > mvaValuesMapToken_;

            EffectiveAreas _effectiveAreas;

            float _Rho;
            edm::InputTag rhoFixedGrid_;

    };

    ElectronProducer::ElectronProducer( const ParameterSet &iConfig ):
        electronToken_( consumes<View<pat::Electron> >( iConfig.getParameter<InputTag>( "electronTag" ) ) ),
        vertexToken_( consumes<View<reco::Vertex> >( iConfig.getParameter<InputTag>( "vertexTag" ) ) ),
        convToken_( consumes<reco::ConversionCollection>( iConfig.getParameter<InputTag>( "convTag" ) ) ),
        beamSpotToken_( consumes<reco::BeamSpot >( iConfig.getParameter<InputTag>( "beamSpotTag" ) ) ),
        mvaValuesMapToken_(consumes<edm::ValueMap<float> >(iConfig.getParameter<edm::InputTag>("mvaValuesMap" ) ) ),
        _effectiveAreas( (iConfig.getParameter<edm::FileInPath>("effAreasConfigFile")).fullPath())

    {
        applyCuts_ = iConfig.getUntrackedParameter<bool>( "ApplyCuts", false );
        verbose_ = iConfig.getUntrackedParameter<bool>( "verbose", false );
        //	eventrhoToken_ = consumes<float>(iConfig.getParameter<edm::InputTag>("Rho"));
        rhoFixedGrid_  = iConfig.getParameter<edm::InputTag>( "rhoFixedGridCollection" );

        produces<vector<flashgg::Electron> >();
        // nontrigmva_ = 0;
    }

    void ElectronProducer::produce( Event &evt, const EventSetup & )
    {

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

        edm::Handle<edm::ValueMap<float> > mvaValues;
        evt.getByToken(mvaValuesMapToken_,mvaValues);

        std::auto_ptr<vector<flashgg::Electron> > elecColl( new vector<flashgg::Electron> );


        for( unsigned int elecIndex = 0; elecIndex < pelectrons->size(); elecIndex++ ) {
            Ptr<pat::Electron> pelec = pelectrons->ptrAt( elecIndex );
            flashgg::Electron felec = flashgg::Electron( *pelec );
            
            float Aeff = 0;

            float pelec_eta = fabs( pelec->superCluster()->eta() );

            double nontrigmva = (*mvaValues)[pelec];
            felec.setNonTrigMVA( ( float )nontrigmva );

            if( applyCuts_ && ( pelec_eta > 2.5 || ( pelec_eta > 1.442 && pelec_eta < 1.566 ) ) ) { continue; }

            const reco::GsfElectron::PflowIsolationVariables& pfIso = pelec->pfIsolationVariables();
            const float chad = pfIso.sumChargedHadronPt;
            const float nhad = pfIso.sumNeutralHadronEt;
            const float pho = pfIso.sumPhotonEt;
            Aeff = _effectiveAreas.getEffectiveArea( pelec_eta );   
            //cout << "Aff " << Aeff << endl;
            float iso = chad + std::max(0.0f, nhad + pho - _Rho*Aeff);
          
            felec.setStandardHggIso( iso );

            felec.setHasMatchedConversion( ConversionTools::hasMatchedConversion( *pelec, convs, vertexPoint ) );

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

