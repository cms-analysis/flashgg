#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/TTHLeptonicTag.h"
#include "flashgg/DataFormats/interface/Electron.h"
#include "flashgg/DataFormats/interface/Muon.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "DataFormats/TrackReco/interface/HitPattern.h"
#include "flashgg/Taggers/interface/LeptonSelection.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "flashgg/DataFormats/interface/TagTruthBase.h"
#include "DataFormats/Common/interface/RefToPtr.h"

#include <vector>
#include <algorithm>
#include <string>
#include <utility>
#include "TLorentzVector.h"
#include "TMath.h"

using namespace std;
using namespace edm;


namespace flashgg {
    class TTHLeptonicTagProducer : public EDProducer
    {

    public:
        typedef math::XYZPoint Point;

        TTHLeptonicTagProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;

        std::vector<edm::EDGetTokenT<View<flashgg::Jet> > > tokenJets_;
        EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
        //EDGetTokenT<View<Jet> > thejetToken_;
        std::vector<edm::InputTag> inputTagJets_;
        EDGetTokenT<View<Electron> > electronToken_;
        EDGetTokenT<View<flashgg::Muon> > muonToken_;
        EDGetTokenT<View<DiPhotonMVAResult> > mvaResultToken_;
        EDGetTokenT<View<Photon> > photonToken_;
        EDGetTokenT<View<reco::Vertex> > vertexToken_;
        EDGetTokenT<View<reco::GenParticle> > genParticleToken_;
        string systLabel_;

        typedef std::vector<edm::Handle<edm::View<flashgg::Jet> > > JetCollectionVector;

        //Thresholds
        double leptonPtThreshold_;
        double muonEtaThreshold_;
        double leadPhoOverMassThreshold_;
        double subleadPhoOverMassThreshold_;
        double MVAThreshold_;
        double deltaRMuonPhoThreshold_;
        double deltaRJetLepThreshold_;
        double deltaRJetLeadPhoThreshold_;
        double deltaRJetSubLeadPhoThreshold_;
        double jetsNumberThreshold_;
        double bjetsNumberThreshold_;
        double jetPtThreshold_;
        double jetEtaThreshold_;
        vector<double>  electronEtaThresholds_;
        vector<double> bDiscriminator_;
        string bTag_;
        double muPFIsoSumRelThreshold_;
        double PhoMVAThreshold_;
        double DeltaRTrkElec_;
        double deltaRPhoElectronThreshold_;
        double deltaMassElectronZThreshold_;

        double TransverseImpactParam_;
        double LongitudinalImpactParam_;
        vector<double> nonTrigMVAThresholds_;
        vector<double> nonTrigMVAEtaCuts_;
        double electronIsoThreshold_;
        double electronNumOfHitsThreshold_;
        
        bool useStdElectronID_;
        bool useElectronMVARecipe_;
        bool useElectronLooseID_;

        bool hasGoodElec = false;
        bool hasGoodMuons = false;

    };

    TTHLeptonicTagProducer::TTHLeptonicTagProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        //thejetToken_( consumes<View<flashgg::Jet> >( iConfig.getParameter<InputTag>( "JetTag" ) ) ),
        inputTagJets_( iConfig.getParameter<std::vector<edm::InputTag> >( "inputTagJets" ) ),
        electronToken_( consumes<View<flashgg::Electron> >( iConfig.getParameter<InputTag>( "ElectronTag" ) ) ),
        muonToken_( consumes<View<flashgg::Muon> >( iConfig.getParameter<InputTag>( "MuonTag" ) ) ),
        mvaResultToken_( consumes<View<flashgg::DiPhotonMVAResult> >( iConfig.getParameter<InputTag> ( "MVAResultTag" ) ) ),
        vertexToken_( consumes<View<reco::Vertex> >( iConfig.getParameter<InputTag> ( "VertexTag" ) ) ),
        genParticleToken_( consumes<View<reco::GenParticle> >( iConfig.getParameter<InputTag> ( "GenParticleTag" ) ) ),
        systLabel_( iConfig.getParameter<string> ( "SystLabel" ) )
    {

        leptonPtThreshold_ = iConfig.getParameter<double>( "leptonPtThreshold");
        muonEtaThreshold_ = iConfig.getParameter<double>( "muonEtaThreshold");
        leadPhoOverMassThreshold_ = iConfig.getParameter<double>( "leadPhoOverMassThreshold");
        subleadPhoOverMassThreshold_ = iConfig.getParameter<double>( "subleadPhoOverMassThreshold");
        MVAThreshold_ = iConfig.getParameter<double>( "MVAThreshold");
        deltaRMuonPhoThreshold_ = iConfig.getParameter<double>( "deltaRMuonPhoThreshold");
        deltaRJetLepThreshold_ = iConfig.getParameter<double>( "deltaRJetLepThreshold");
        jetsNumberThreshold_ = iConfig.getParameter<double>( "jetsNumberThreshold");
        bjetsNumberThreshold_ = iConfig.getParameter<double>( "bjetsNumberThreshold");
        jetPtThreshold_ = iConfig.getParameter<double>( "jetPtThreshold");
        jetEtaThreshold_ = iConfig.getParameter<double>( "jetEtaThreshold");

        deltaRJetLeadPhoThreshold_ = iConfig.getParameter<double>( "deltaRJetLeadPhoThreshold");
        deltaRJetSubLeadPhoThreshold_ = iConfig.getParameter<double>( "deltaRJetSubLeadPhoThreshold");

        electronEtaThresholds_ = iConfig.getParameter<vector<double > >( "electronEtaThresholds");
        bDiscriminator_ = iConfig.getParameter<vector<double > >( "bDiscriminator");
        bTag_ = iConfig.getParameter<string>( "bTag");

        muPFIsoSumRelThreshold_ = iConfig.getParameter<double>( "muPFIsoSumRelThreshold");
        PhoMVAThreshold_ = iConfig.getParameter<double>( "PhoMVAThreshold");

        DeltaRTrkElec_ = iConfig.getParameter<double>( "DeltaRTrkElec");
        deltaRPhoElectronThreshold_ = iConfig.getParameter<double>( "deltaRPhoElectronThreshold");
        deltaMassElectronZThreshold_ = iConfig.getParameter<double>( "deltaMassElectronZThreshold");

        TransverseImpactParam_ = iConfig.getParameter<double>( "TransverseImpactParam");
        LongitudinalImpactParam_ = iConfig.getParameter<double>( "LongitudinalImpactParam");
        nonTrigMVAThresholds_ =  iConfig.getParameter<vector<double > >( "nonTrigMVAThresholds");
        nonTrigMVAEtaCuts_ =  iConfig.getParameter<vector<double > >( "nonTrigMVAEtaCuts");
        electronIsoThreshold_ = iConfig.getParameter<double>( "electronIsoThreshold");
        electronNumOfHitsThreshold_ = iConfig.getParameter<double>( "electronNumOfHitsThreshold");
        
        useStdElectronID_=iConfig.getParameter<bool>("useStdElectronID");
        useElectronMVARecipe_=iConfig.getParameter<bool>("useElectronMVARecipe");
        useElectronLooseID_=iConfig.getParameter<bool>("useElectronLooseID");
        
        for (unsigned i = 0 ; i < inputTagJets_.size() ; i++) {
            auto token = consumes<View<flashgg::Jet> >(inputTagJets_[i]);
            tokenJets_.push_back(token);
        }
        produces<vector<TTHLeptonicTag> >();
        produces<vector<TagTruthBase> >();
    }

    void TTHLeptonicTagProducer::produce( Event &evt, const EventSetup & )

    {

        //Handle<View<flashgg::Jet> > theJets;
        //evt.getByToken( thejetToken_, theJets );
        //const PtrVector<flashgg::Jet>& jetPointers = theJets->ptrVector();
        JetCollectionVector Jets( inputTagJets_.size() );
        for( size_t j = 0; j < inputTagJets_.size(); ++j ) {
            evt.getByToken( tokenJets_[j], Jets[j] );
        }

        Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
        evt.getByToken( diPhotonToken_, diPhotons );

        Handle<View<flashgg::Muon> > theMuons;
        evt.getByToken( muonToken_, theMuons );

        Handle<View<flashgg::Electron> > theElectrons;
        evt.getByToken( electronToken_, theElectrons );

        Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;
        evt.getByToken( mvaResultToken_, mvaResults );

        Handle<View<reco::GenParticle> > genParticles;

        std::auto_ptr<vector<TTHLeptonicTag> > tthltags( new vector<TTHLeptonicTag> );

        Handle<View<reco::Vertex> > vertices;
        evt.getByToken( vertexToken_, vertices );

        std::auto_ptr<vector<TagTruthBase> > truths( new vector<TagTruthBase> );

        Point higgsVtx;

        if( ! evt.isRealData() ) {
            evt.getByToken( genParticleToken_, genParticles );
            for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) {
                int pdgid = genParticles->ptrAt( genLoop )->pdgId();
                if( pdgid == 25 || pdgid == 22 ) {
                    higgsVtx = genParticles->ptrAt( genLoop )->vertex();
                    break;
                }
            }
        }

        edm::RefProd<vector<TagTruthBase> > rTagTruth = evt.getRefBeforePut<vector<TagTruthBase> >();
        unsigned int idx = 0;


        assert( diPhotons->size() == mvaResults->size() );

        bool photonSelection = false;
        double idmva1 = 0.;
        double idmva2 = 0.;

        vector<int> numMuonJetsdR;
        vector<int> numElectronJetsdR;
        bool muonJets = false;
        bool ElectronJets = false;

        for( unsigned int diphoIndex = 0; diphoIndex < diPhotons->size(); diphoIndex++ ) {

            hasGoodElec = false;
            hasGoodMuons = false;

            unsigned int jetCollectionIndex = diPhotons->ptrAt( diphoIndex )->jetCollectionIndex();

            std::vector<edm::Ptr<Muon> > tagMuons;
            std::vector<edm::Ptr<Electron> > tagElectrons;
            std::vector<edm::Ptr<Jet> > tagJets;
            std::vector<edm::Ptr<Jet> > tagBJets;

            edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotons->ptrAt( diphoIndex );
            edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResults->ptrAt( diphoIndex );

            TTHLeptonicTag tthltags_obj( dipho, mvares );

            if( dipho->leadingPhoton()->pt() < ( dipho->mass() )*leadPhoOverMassThreshold_ ) { continue; }

            if( dipho->subLeadingPhoton()->pt() < ( dipho->mass() )*subleadPhoOverMassThreshold_ ) { continue; }


            idmva1 = dipho->leadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );
            idmva2 = dipho->subLeadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );

            if( idmva1 < PhoMVAThreshold_ || idmva2 < PhoMVAThreshold_ ) { continue; }

            if( mvares->result < MVAThreshold_ ) { continue; }

            photonSelection = true;

            std::vector<edm::Ptr<flashgg::Muon> > goodMuons = selectMuons( theMuons->ptrs(), dipho, vertices->ptrs(), muonEtaThreshold_ , 
                                                                           leptonPtThreshold_,muPFIsoSumRelThreshold_, deltaRMuonPhoThreshold_, deltaRMuonPhoThreshold_ );
            
            
            std::vector<edm::Ptr<Electron> > goodElectrons;
            if( !useStdElectronID_) goodElectrons= selectElectrons( theElectrons->ptrs(), dipho, vertices->ptrs(), leptonPtThreshold_, 
                                                                    TransverseImpactParam_, LongitudinalImpactParam_, nonTrigMVAThresholds_, nonTrigMVAEtaCuts_,
                                                                    electronIsoThreshold_, electronNumOfHitsThreshold_, electronEtaThresholds_ ,
                                                                    deltaRPhoElectronThreshold_,DeltaRTrkElec_,deltaMassElectronZThreshold_);
            else goodElectrons = selectStdElectrons(theElectrons->ptrs(), dipho, vertices->ptrs(), leptonPtThreshold_,  electronEtaThresholds_ ,
                                                    useElectronMVARecipe_,useElectronLooseID_,
                                                    deltaRPhoElectronThreshold_,DeltaRTrkElec_,deltaMassElectronZThreshold_);
            

            hasGoodElec = ( goodElectrons.size() > 0 );
            hasGoodMuons = ( goodMuons.size() > 0 );
            
            if( !hasGoodElec && !hasGoodMuons ) { continue; }
            
            numMuonJetsdR.clear();
            numElectronJetsdR.clear();
            
            muonJets = false;
            ElectronJets = false;

            if( hasGoodMuons ) {

                for( unsigned int muonIndex = 0; muonIndex < goodMuons.size(); muonIndex++ ) {

                    Ptr<flashgg::Muon> muon = goodMuons[muonIndex];

                    int deltaRMuonJetcount = 0;
                    double bDiscriminatorValue = -999.;

                    for( unsigned int candIndex_outer = 0; candIndex_outer < Jets[jetCollectionIndex]->size() ; candIndex_outer++ ) {
                        edm::Ptr<flashgg::Jet> thejet = Jets[jetCollectionIndex]->ptrAt( candIndex_outer );

                        if( !thejet->passesPuJetId( dipho ) ) { continue; }

                        if( fabs( thejet->eta() ) > jetEtaThreshold_ ) { continue; }

                        if( thejet->pt() < jetPtThreshold_ ) { continue; }

                        float dRPhoLeadJet = deltaR( thejet->eta(), thejet->phi(), dipho->leadingPhoton()->superCluster()->eta(), dipho->leadingPhoton()->superCluster()->phi() ) ;
                        float dRPhoSubLeadJet = deltaR( thejet->eta(), thejet->phi(), dipho->subLeadingPhoton()->superCluster()->eta(),
                                                        dipho->subLeadingPhoton()->superCluster()->phi() );

                        if( dRPhoLeadJet < deltaRJetLeadPhoThreshold_ || dRPhoSubLeadJet < deltaRJetSubLeadPhoThreshold_ ) { continue; }

                        float dRJetMuon = deltaR( thejet->eta(), thejet->phi(), muon->eta(), muon->phi() ) ;

                        if( dRJetMuon < deltaRJetLepThreshold_ ) { continue; }
                        deltaRMuonJetcount++;

                        tagJets.push_back( thejet );

                        bDiscriminatorValue = thejet->bDiscriminator( bTag_.c_str() );

                        if( bDiscriminatorValue > bDiscriminator_[1] ) {
                            tagBJets.push_back( thejet );
                        }


                    }//end of jets loop

                    numMuonJetsdR.push_back( deltaRMuonJetcount );
                    tagMuons.push_back( muon );

                }//end of muons loop

            }

            if( hasGoodElec ) {

                std::vector<const flashgg::Photon *> photons;

                photons.push_back( dipho->leadingPhoton() );
                photons.push_back( dipho->subLeadingPhoton() );


                for( unsigned int ElectronIndex = 0; ElectronIndex < goodElectrons.size(); ElectronIndex++ ) {

                    Ptr<Electron> Electron = goodElectrons[ElectronIndex];

                    int deltaRElectronJetcount = 0;
                    double bDiscriminatorValue = -999.;

                    for( unsigned int candIndex_outer = 0; candIndex_outer < Jets[jetCollectionIndex]->size() ; candIndex_outer++ ) {
                        edm::Ptr<flashgg::Jet> thejet = Jets[jetCollectionIndex]->ptrAt( candIndex_outer );

                        if( !thejet->passesPuJetId( dipho ) ) { continue; }

                        //https://github.com/h2gglobe/h2gglobe/blob/master/PhotonAnalysis/src/PhotonAnalysis.cc#L5367
                        if( fabs( thejet->eta() ) > jetEtaThreshold_ ) { continue; }

                        //https://github.com/h2gglobe/h2gglobe/blob/master/PhotonAnalysis/src/PhotonAnalysis.cc#L5371
                        if( thejet->pt() < jetPtThreshold_ ) { continue; }

                        float dRJetElectron = deltaR( thejet->eta(), thejet->phi(), Electron->eta(), Electron->phi() ) ;

                        //https://github.com/njets_btagmediumh2gglobe/h2gglobe/blob/master/PhotonAnalysis/src/PhotonAnalysis.cc#L5370
                        if( dRJetElectron < deltaRJetLepThreshold_ ) { continue; }
                        deltaRElectronJetcount++;

                        tagJets.push_back( thejet );

                        bDiscriminatorValue = thejet->bDiscriminator( bTag_.c_str() );

                        if( bDiscriminatorValue > bDiscriminator_[1] ) {
                            tagBJets.push_back( thejet );
                        }

                    }//end of jets loop
                    numElectronJetsdR.push_back( deltaRElectronJetcount );

                    tagElectrons.push_back( Electron );

                }//end of electron loop

            }


            for( unsigned num = 0; num < numMuonJetsdR.size(); num++ ) {
                int check = numMuonJetsdR.at( num );
                if( check >= jetsNumberThreshold_ ) {muonJets = true;}
            }

            for( unsigned num = 0; num < numElectronJetsdR.size(); num++ ) {
                int check = numElectronJetsdR.at( num );
                if( check >= jetsNumberThreshold_ ) {ElectronJets = true;}
            }

            /*
            std::cout << " TTHLeptonicTagProducer tagBJets.size()=" << tagBJets.size()
                      << " tagJets.size()=" << tagJets.size()
                      << " photonSelection=" << photonSelection
                      << " tagMuons.size()=" << tagMuons.size() << " muonJets=" << muonJets
                      << " tagElectrons.size()="<< tagElectrons.size() << " ElectronJets=" << ElectronJets
                      << std::endl;
            */

            if( tagBJets.size() >= bjetsNumberThreshold_ && tagJets.size() >= jetsNumberThreshold_ && photonSelection
                    && ( ( tagMuons.size() > 0 && muonJets ) || ( tagElectrons.size() > 0 && ElectronJets ) ) ) {
                //                std::cout << " TTHLeptonicTagProducer TAGGED " << std::endl;
                for( unsigned num = 0; num < tagJets.size(); num++ ) {
                    tthltags_obj.includeWeights( *tagJets[num] );
                }
                if( tagElectrons.size() > 0 && ElectronJets ) {
                    //                    std::cout << "including electron weights" << std::endl;
                    tthltags_obj.includeWeights( *tagElectrons[0] );
                } else if( tagMuons.size() > 0 && muonJets ) {
                    //                    std::cout << "including muon weights" << std::endl;
                    tthltags_obj.includeWeights( *tagMuons[0] );
                }
                tthltags_obj.includeWeights( *dipho );

                tthltags_obj.setJets( tagJets );
                tthltags_obj.setBJets( tagBJets );
                tthltags_obj.setMuons( tagMuons );
                tthltags_obj.setElectrons( tagElectrons );
                tthltags_obj.setDiPhotonIndex( diphoIndex );
                tthltags_obj.setSystLabel( systLabel_ );
                tthltags->push_back( tthltags_obj );
                if( ! evt.isRealData() ) {
                    TagTruthBase truth_obj;
                    truth_obj.setGenPV( higgsVtx );
                    truths->push_back( truth_obj );
                    tthltags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<TagTruthBase> >( rTagTruth, idx++ ) ) );
                }
            } else {
                //                std::cout << " TTHLeptonicTagProducer NO TAG " << std::endl;
            }

        }//diPho loop end !
        evt.put( tthltags );
        evt.put( truths );
    }

}
typedef flashgg::TTHLeptonicTagProducer FlashggTTHLeptonicTagProducer;
DEFINE_FWK_MODULE( FlashggTTHLeptonicTagProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

