#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "flashgg/DataFormats/interface/GenPhotonExtra.h"
#include "flashgg/DataFormats/interface/VertexCandidateMap.h"
#include "flashgg/MicroAOD/interface/PhotonIdUtils.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
// #include "HiggsAnalysis/GBRLikelihoodEGTools/interface/EGEnergyCorrectorSemiParm.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"
#include "flashgg/MicroAOD/interface/IsolationAlgoBase.h"
#include "DataFormats/Common/interface/ValueMap.h"

#include "RecoEgamma/EgammaTools/interface/EffectiveAreas.h"

using namespace std;
using namespace edm;
using reco::PFCandidate;

namespace flashgg {

    struct CaloIsoParams {
        CaloIsoParams() {};
        CaloIsoParams( bool overlapRemoval, PFCandidate::ParticleType type, const std::vector<double> &vetos ) :
            overlapRemoval_( overlapRemoval ), type_( type ), vetos_( vetos ) {};
        bool overlapRemoval_;
        PFCandidate::ParticleType type_;
        std::vector<double> vetos_;
    };

    class PhotonProducer : public EDProducer
    {

    public:
        PhotonProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;

        EDGetTokenT<View<pat::Photon> > photonToken_;
        EDGetTokenT<View<pat::PackedCandidate> > pfcandidateToken_;
        EDGetTokenT<View<reco::Vertex> > vertexToken_;
        EDGetTokenT<flashgg::VertexCandidateMap> vertexCandidateMapToken_;

        EDGetTokenT<vector<flashgg::GenPhotonExtra> > genPhotonToken_;
        double maxGenDeltaR_;
        bool copyExtraGenInfo_;

        edm::EDGetTokenT<EcalRecHitCollection> ecalHitEBToken_;
        edm::EDGetTokenT<EcalRecHitCollection> ecalHitEEToken_;
        edm::EDGetTokenT<EcalRecHitCollection> ecalHitESToken_;
        edm::EDGetTokenT<double> rhoToken_;
        //        edm::InputTag rhoFixedGrid_;

        float lxyMin_ = 2.0;
        float probMin_ = 1e-6;
        int nHitsBeforeVtxMax_ = 0;
        //        string electronLabel_;
        edm::EDGetTokenT<std::vector<pat::Electron> > electronToken_;

        edm::EDGetTokenT<reco::ConversionCollection> convToken_;

        edm::EDGetTokenT<reco::BeamSpot> beamSpotToken_;

        PhotonIdUtils phoTools_;
        edm::FileInPath phoIdMVAweightfileEB_, phoIdMVAweightfileEE_;
        //    edm::FileInPath regressionWeightFile_;
        bool useNonZsLazyTools_, recomputeNonZsClusterShapes_, addRechitFlags_;

        /// EGEnergyCorrectorSemiParm corV8_;
        bool doOverlapRemovalForIsolation_, useVtx0ForNeutralIso_;
        std::vector<CaloIsoParams> extraCaloIsolations_;
        std::vector<std::unique_ptr<IsolationAlgoBase> > extraIsoAlgos_;

        bool is2017_;

        EffectiveAreas _effectiveAreas;
        vector<double> _phoIsoPtScalingCoeff;
        double _phoIsoCutoff;

        edm::EDGetTokenT<edm::ValueMap<float> > egmMvaValuesMapToken_;
    };


    PhotonProducer::PhotonProducer( const ParameterSet &iConfig ) :
        photonToken_( consumes<View<pat::Photon> >( iConfig.getParameter<InputTag> ( "photonTag" ) ) ),
        pfcandidateToken_( consumes<View<pat::PackedCandidate> >( iConfig.getParameter<InputTag> ( "pfCandidatesTag" ) ) ),
        vertexToken_( consumes<View<reco::Vertex> >( iConfig.getParameter<InputTag> ( "vertexTag" ) ) ),
        vertexCandidateMapToken_( consumes<VertexCandidateMap>( iConfig.getParameter<InputTag>( "vertexCandidateMapTag" ) ) ),
        genPhotonToken_( mayConsume<vector<flashgg::GenPhotonExtra> >( iConfig.getParameter<InputTag>( "genPhotonTag" ) ) ),
        ecalHitEBToken_( consumes<EcalRecHitCollection>( iConfig.getParameter<edm::InputTag>( "reducedBarrelRecHitCollection" ) ) ),
        ecalHitEEToken_( consumes<EcalRecHitCollection>( iConfig.getParameter<edm::InputTag>( "reducedEndcapRecHitCollection" ) ) ),
        ecalHitESToken_( consumes<EcalRecHitCollection>( iConfig.getParameter<edm::InputTag>( "reducedPreshowerRecHitCollection" ) ) ),
        rhoToken_( consumes<double>( iConfig.getParameter<edm::InputTag>( "rhoFixedGridCollection" ) ) ),
        electronToken_( consumes<std::vector<pat::Electron> >( iConfig.getParameter<edm::InputTag>( "elecTag") ) ),
        convToken_( consumes<reco::ConversionCollection>( iConfig.getParameter<edm::InputTag>( "convTag" ) ) ),
        beamSpotToken_( consumes<reco::BeamSpot >( iConfig.getParameter<edm::InputTag>( "beamSpotTag" ) ) ),
        _effectiveAreas((iConfig.getParameter<edm::FileInPath>("effAreasConfigFile")).fullPath()),
        _phoIsoPtScalingCoeff(iConfig.getParameter<std::vector<double >>("phoIsoPtScalingCoeff")),
        _phoIsoCutoff(iConfig.getParameter<double>("phoIsoCutoff")),
        egmMvaValuesMapToken_( consumes<edm::ValueMap<float> >(iConfig.getParameter<edm::InputTag>("egmMvaValuesMap")) )
    {
        is2017_ = iConfig.getParameter<bool>( "is2017" );
        
        phoIdMVAweightfileEB_ = iConfig.getParameter<edm::FileInPath>( "photonIdMVAweightfile_EB" );
        phoIdMVAweightfileEE_ = iConfig.getParameter<edm::FileInPath>( "photonIdMVAweightfile_EE" );
        phoTools_.setupMVA( phoIdMVAweightfileEB_.fullPath(), phoIdMVAweightfileEE_.fullPath(), true, is2017_);

        doOverlapRemovalForIsolation_ = iConfig.getParameter<bool>( "doOverlapRemovalForIsolation" );
        useVtx0ForNeutralIso_ = iConfig.getParameter<bool>( "useVtx0ForNeutralIso" );

        copyExtraGenInfo_ = iConfig.getParameter<bool>( "copyExtraGenInfo" );
        maxGenDeltaR_ = iConfig.getParameter<double>( "maxGenDeltaR" );

        std::vector<ParameterSet> extraCaloIsolations = iConfig.getParameter<std::vector<ParameterSet> >( "extraCaloIsolations" );
        for( std::vector<ParameterSet>::iterator it = extraCaloIsolations.begin(); it != extraCaloIsolations.end(); ++it ) {
            CaloIsoParams p( it->getParameter<bool>( "overlapRemoval" ), ( PFCandidate::ParticleType )it->getParameter<int>( "type" ),
                             it->getParameter<std::vector<double> >( "vetoRegions" ) );
            assert( p.vetos_.size() == 7 );
            extraCaloIsolations_.push_back( p );
        }

        if( iConfig.exists("extraIsolations") ) {
            std::vector<ParameterSet> extraIsolationPlugins = iConfig.getParameter<std::vector<ParameterSet> >("extraIsolations");
            extraIsoAlgos_.resize(extraIsolationPlugins.size());
            for(size_t ip=0; ip<extraIsolationPlugins.size(); ++ip) {
                auto & plugin = extraIsolationPlugins[ip];
                auto className = plugin.getParameter<std::string>("algo");
                extraIsoAlgos_[ip].reset( FlashggIsolationAlgoFactory::get()->create(className,plugin,consumesCollector()) );
            }
        }

        useNonZsLazyTools_ = iConfig.getParameter<bool>( "useNonZsLazyTools" );
        recomputeNonZsClusterShapes_ = iConfig.getParameter<bool>( "recomputeNonZsClusterShapes" );
        addRechitFlags_ = iConfig.getParameter<bool>( "addRechitFlags" );

        produces<vector<flashgg::Photon> >();
    }

    void PhotonProducer::produce( Event &evt, const EventSetup &iSetup )
    {
        edm::ESHandle<CaloTopology> topology ;
        iSetup.get<CaloTopologyRecord>().get( topology );

        Handle<View<pat::Photon> > photons;
        evt.getByToken( photonToken_, photons );
        Handle<View<pat::PackedCandidate> > pfcandidates;
        evt.getByToken( pfcandidateToken_, pfcandidates );
        Handle<View<reco::Vertex> > vertices;
        evt.getByToken( vertexToken_, vertices );
        Handle<VertexCandidateMap> vertexCandidateMap;
        evt.getByToken( vertexCandidateMapToken_, vertexCandidateMap );
        Handle<double> rhoHandle; 
        evt.getByToken( rhoToken_, rhoHandle );
        //        evt.getByLabel( rhoFixedGrid_, rhoHandle );
        Handle<vector<flashgg::GenPhotonExtra> > genPhotonsHandle;
        if( ! evt.isRealData() ) {
            evt.getByToken( genPhotonToken_, genPhotonsHandle );
        }
        Handle<std::vector<pat::Electron> > electronHandle;
        evt.getByToken( electronToken_, electronHandle );
        //        evt.getByLabel( electronLabel_, electronHandle );

        Handle<reco::ConversionCollection> convs;
        evt.getByToken( convToken_, convs );

        Handle<reco::BeamSpot> recoBeamSpotHandle;
        evt.getByToken( beamSpotToken_, recoBeamSpotHandle );

        const reco::BeamSpot &beamspot = *recoBeamSpotHandle.product();

        edm::Handle<edm::ValueMap<float> > egmMvaValues;
        evt.getByToken(egmMvaValuesMapToken_,egmMvaValues);

        // const PtrVector<pat::Photon>& photonPointers = photons->ptrVector();
        // const PtrVector<pat::PackedCandidate>& pfcandidatePointers = pfcandidates->ptrVector();
        // const PtrVector<reco::Vertex>& vertexPointers = vertices->ptrVector();
        const flashgg::VertexCandidateMap vtxToCandMap = *( vertexCandidateMap.product() );
        const double rhoFixedGrd = *( rhoHandle.product() );
        const reco::Vertex *neutVtx = ( useVtx0ForNeutralIso_ ? &vertices->at( 0 ) : 0 );

        unique_ptr<vector<flashgg::Photon> > photonColl( new vector<flashgg::Photon> );

        //// // this is hacky and dangerous
        //// const reco::VertexCollection* orig_collection = static_cast<const reco::VertexCollection*>(vertices->product());

        EcalClusterLazyTools zsLazyTool( evt, iSetup, ecalHitEBToken_, ecalHitEEToken_, ecalHitESToken_ );
        noZS::EcalClusterLazyTools noZsLazyTool( evt, iSetup, ecalHitEBToken_, ecalHitEEToken_, ecalHitESToken_ );

        for( unsigned int i = 0 ; i < photons->size() ; i++ ) {

            Ptr<pat::Photon> pp = photons->ptrAt( i );
            flashgg::Photon fg = flashgg::Photon( *pp );

            double egmMvaValue = (*egmMvaValues)[pp];
            fg.addUserFloat("EGMPhotonMVA", (float) egmMvaValue);

            // Get electron veto flag value from miniAOD PAT photons
            fg.setPassElectronVeto(pp->passElectronVeto());

            // Gen matching
            if( ! evt.isRealData() ) {
                unsigned int best = INT_MAX;
                float bestptdiff = 99e15;
                const auto &genPhotons = *genPhotonsHandle;
                for( unsigned int j = 0 ; j < genPhotons.size() ; j++ ) {
                    auto gen = genPhotons[j].ptr();
                    if( gen->pdgId() != 22 ) { continue; }
                    float dR = reco::deltaR( *pp, *gen );
                    if( dR > maxGenDeltaR_ ) { continue; }
                    float ptdiff = fabs( pp->pt() - gen->pt() );
                    if( ptdiff < bestptdiff ) {
                        bestptdiff = ptdiff;
                        best = j;
                    }
                }
                if( best < INT_MAX ) {
                    auto &extra = genPhotons[best];
                    fg.setMatchedGenPhoton( extra.ptr() );
                    fg.setGenMatchType( extra.type() );
                    if( copyExtraGenInfo_ ) {
                        extra.copyTo( fg );
                    }
                }
            }

            /// double ecor, sigeovere, mean, sigma, alpha1, n1, alpha2, n2, pdfval;
            ///
            /// corV8_.CorrectedEnergyWithErrorV8(*pp, *orig_collection, *rhoHandle, lazyTool, iSetup,ecor, sigeovere, mean, sigma, alpha1, n1, alpha2, n2, pdfval);
            /// //      printf("V8:  sceta = %5f, default = %5f, corrected = %5f, sigmaE/E = %5f, alpha1 = %5f, n1 = %5f, alpha2 = %5f, n2 = %5f, pdfval = %5f, meancb = %5f, sigmacb = %5f\n", pp->superCluster()->eta(), pp->energy(),ecor,sigeovere,alpha1,n1,alpha2,n2,pdfval,mean,sigma);
            ///
            /// fg.updateEnergy("regression",ecor);
            /// fg.setSigEOverE(sigeovere);

            if( recomputeNonZsClusterShapes_ ) {
                phoTools_.recomputeNonZsClusterShapes( fg, noZsLazyTool );
            }
            if( useNonZsLazyTools_ ) {
                phoTools_.fillExtraClusterShapes( fg, noZsLazyTool );
            } else {
                phoTools_.fillExtraClusterShapes( fg, zsLazyTool );
            }
            if( addRechitFlags_ ) {
                phoTools_.fillRechHitFlags( fg, noZsLazyTool, topology.product() );
            }

            phoTools_.removeOverlappingCandidates( doOverlapRemovalForIsolation_ );

            //                                                                                                                  outer cone size
            //                                                                                                                       inner (veto) cone size barrel
            //                                                                                                                             inner (veto) cone size endcap
            //                                                                                                                                   min track pt
            std::map<edm::Ptr<reco::Vertex>, float> isomap04 = phoTools_.pfIsoChgWrtAllVtx( pp, vertices->ptrs(), vtxToCandMap, 0.4, 0.02, 0.02, 0.1 );
            std::map<edm::Ptr<reco::Vertex>, float> isomap03 = phoTools_.pfIsoChgWrtAllVtx( pp, vertices->ptrs(), vtxToCandMap, 0.3, 0.02, 0.02, 0.1 );
            fg.setpfChgIso04( isomap04 );
            fg.setpfChgIso03( isomap03 );
            std::map<edm::Ptr<reco::Vertex>, float> &ref_isomap04 = isomap04;
            std::map<edm::Ptr<reco::Vertex>, float> &ref_isomap03 = isomap03;
            float pfChgIsoWrtWorstVtx04 =  phoTools_.pfIsoChgWrtWorstVtx( ref_isomap04 );
            float pfChgIsoWrtWorstVtx03 =  phoTools_.pfIsoChgWrtWorstVtx( ref_isomap03 );
            fg.setpfChgIsoWrtWorstVtx04( pfChgIsoWrtWorstVtx04 );
            fg.setpfChgIsoWrtWorstVtx03( pfChgIsoWrtWorstVtx03 );

            // This map is needed for the photon preselection
            std::map<edm::Ptr<reco::Vertex>, float> isomap02 = phoTools_.pfIsoChgWrtAllVtx( pp, vertices->ptrs(), vtxToCandMap, 0.2, 0.02, 0.02, 0.1 );
            fg.setpfChgIso02( isomap02 );
            fg.setpfChgIsoWrtChosenVtx02( 0. ); // just to initalize things properly, will be setup for real in the diphoton producer once the vertex is chosen

            float pfPhoIso04 = phoTools_.pfCaloIso( pp, pfcandidates->ptrs(), 0.4, 0.0, 0.070, 0.015, 0.0, 0.0, 0.0, PFCandidate::gamma, neutVtx );
            float pfPhoIso03 = phoTools_.pfCaloIso( pp, pfcandidates->ptrs(), 0.3, 0.0, 0.070, 0.015, 0.0, 0.0, 0.0, PFCandidate::gamma, neutVtx );
            fg.setpfPhoIso04( pfPhoIso04 );
            fg.setpfPhoIso03( pfPhoIso03 );

            float pfNeutIso04 = phoTools_.pfCaloIso( pp, pfcandidates->ptrs(), 0.4, 0.0, 0.000, 0.000, 0.0, 0.0, 0.0, PFCandidate::h0, neutVtx );
            float pfNeutIso03 = phoTools_.pfCaloIso( pp, pfcandidates->ptrs(), 0.3, 0.0, 0.000, 0.000, 0.0, 0.0, 0.0, PFCandidate::h0, neutVtx );
            fg.setpfNeutIso04( pfNeutIso04 );
            fg.setpfNeutIso03( pfNeutIso03 );

            double eA_pho = _effectiveAreas.getEffectiveArea( abs(pp->superCluster()->eta()) );

            std::map<edm::Ptr<reco::Vertex>, float> mvamap = phoTools_.computeMVAWrtAllVtx( fg, vertices->ptrs(), rhoFixedGrd, eA_pho, _phoIsoPtScalingCoeff, _phoIsoCutoff );
            fg.setPhoIdMvaD( mvamap );

            // add extra isolations (useful for tuning)
            if( ! extraCaloIsolations_.empty() ) {
                for( size_t iso = 0; iso < extraCaloIsolations_.size(); ++iso ) {
                    CaloIsoParams &p = extraCaloIsolations_[iso];
                    phoTools_.removeOverlappingCandidates( p.overlapRemoval_ );
                    float val = phoTools_.pfCaloIso( pp, pfcandidates->ptrs(),
                                                     p.vetos_[0], p.vetos_[1], p.vetos_[2], p.vetos_[3], p.vetos_[4], p.vetos_[5], p.vetos_[6],
                                                     p.type_, neutVtx );
                    /// cout << "User Isolation " << iso << " " << val << endl;
                    fg.setUserIso( val, iso );
                }
            }

            if( ! extraIsoAlgos_.empty() ) {
                for( auto &algo : extraIsoAlgos_ ) {
                    std::map<edm::Ptr<reco::Vertex>, float> iso;
                    algo->begin( *pp, evt, iSetup );
                    if( algo->hasChargedIsolation() ) {
                        auto vpts = vertices->ptrs();
                        for( auto &vtx : vpts ) {
                            iso[vtx] = algo->chargedIsolation( pp, vtx, vtxToCandMap );
                        }
                        fg.setExtraChIso( algo->name(), iso );
                    }
                    if( algo->hasCaloIsolation( PFCandidate::gamma ) ) {
                        fg.setExtraPhoIso( algo->name(), algo->caloIsolation( pp, pfcandidates->ptrs(), PFCandidate::gamma, neutVtx ) );
                    }
                    if( algo->hasCaloIsolation( PFCandidate::h0 ) ) {
                        fg.setExtraNeutIso( algo->name(), algo->caloIsolation( pp, pfcandidates->ptrs(), PFCandidate::h0, neutVtx ) );
                    }
                    algo->end( fg );
                }
            }


            phoTools_.removeOverlappingCandidates( doOverlapRemovalForIsolation_ );

            photonColl->push_back( fg );
        }

        evt.put( std::move( photonColl ) );

        /// orig_collection = 0;
    }
}



typedef flashgg::PhotonProducer FlashggPhotonProducer;
DEFINE_FWK_MODULE( FlashggPhotonProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

