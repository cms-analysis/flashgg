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
#include "flashgg/DataFormats/interface/THQLeptonicTag.h"
//#include "flashgg/DataFormats/interface/THQLeptonicMVAResult.h"
#include "flashgg/DataFormats/interface/Electron.h"
#include "flashgg/DataFormats/interface/Muon.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "DataFormats/TrackReco/interface/HitPattern.h"
#include "flashgg/Taggers/interface/LeptonSelection.h"

#include "DataFormats/Math/interface/deltaR.h"

//#include "flashgg/DataFormats/interface/TagTruthBase.h"
#include "flashgg/DataFormats/interface/VBFTagTruth.h"
#include "DataFormats/Common/interface/RefToPtr.h"

#include <vector>
#include <algorithm>
#include <string>
#include <utility>
#include "TLorentzVector.h"
#include "TMath.h"
#include "TMVA/Reader.h"

// https://github.com/cms-analysis/flashgg/commit/f327ca16c29b4ced8eaf8c309cb9218fac265963 (fixing the tth taggers)
using namespace std;
using namespace edm;


namespace flashgg {
    class THQLeptonicTagProducer : public EDProducer
    {

    public:
        typedef math::XYZPoint Point;

        THQLeptonicTagProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;

        std::vector<edm::EDGetTokenT<View<flashgg::Jet> > > tokenJets_;
        EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
        //EDGetTokenT<View<THQLeptonicMVAResult> > thqleptonicMvaResultToken_;
        //EDGetTokenT<View<Jet> > thejetToken_;
        std::vector<edm::InputTag> inputTagJets_;
        EDGetTokenT<View<Electron> > electronToken_;
        EDGetTokenT<View<flashgg::Muon> > muonToken_;
        EDGetTokenT<View<DiPhotonMVAResult> > mvaResultToken_;
        EDGetTokenT<View<Photon> > photonToken_;
        EDGetTokenT<View<reco::Vertex> > vertexToken_;
        EDGetTokenT<View<reco::GenParticle> > genParticleToken_;
        EDGetTokenT<View<reco::GenJet> > genJetToken_;
        string systLabel_;

        typedef std::vector<edm::Handle<edm::View<flashgg::Jet> > > JetCollectionVector;

        //Thresholds
        double leptonPtThreshold_;
        double leptonEtaThreshold_;
        double leadPhoOverMassThreshold_;
        double subleadPhoOverMassThreshold_;
        double MVAThreshold_;
        double deltaRLepPhoThreshold_;
        double deltaRJetLepThreshold_;

        double deltaRJetLeadPhoThreshold_;
        double deltaRJetSubLeadPhoThreshold_;

        double jetsNumberThreshold_;
        double bjetsNumberThreshold_;
        double jetPtThreshold_;
        double jetEtaThreshold_;

        vector<double> bDiscriminator_;
        string bTag_;
        double muPFIsoSumRelThreshold_;
        double PhoMVAThreshold_;
        double DeltaRTrkElec_;
        double TransverseImpactParam_;
        double LongitudinalImpactParam_;

        double deltaRPhoElectronThreshold_;
        double Zmass_;
        double deltaMassElectronZThreshold_;

        vector<double> nonTrigMVAThresholds_;
        vector<double> nonTrigMVAEtaCuts_;
        double electronIsoThreshold_;
        double electronNumOfHitsThreshold_;
        vector<double>  electronEtaCuts_;

        bool hasGoodElec = false;
        bool hasGoodMuons = false;

    };

    THQLeptonicTagProducer::THQLeptonicTagProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        //thqleptonicMvaResultToken_( consumes<View<flashgg::THQLeptonicMVAResult> >( iConfig.getParameter<InputTag> ( "THQLeptonicMVAResultTag" ) ) ),
        //thejetToken_( consumes<View<flashgg::Jet> >( iConfig.getParameter<InputTag>( "JetTag" ) ) ),
        inputTagJets_( iConfig.getParameter<std::vector<edm::InputTag> >( "inputTagJets" ) ),
        electronToken_( consumes<View<flashgg::Electron> >( iConfig.getParameter<InputTag>( "ElectronTag" ) ) ),
        muonToken_( consumes<View<flashgg::Muon> >( iConfig.getParameter<InputTag>( "MuonTag" ) ) ),
        mvaResultToken_( consumes<View<flashgg::DiPhotonMVAResult> >( iConfig.getParameter<InputTag> ( "MVAResultTag" ) ) ),
        vertexToken_( consumes<View<reco::Vertex> >( iConfig.getParameter<InputTag> ( "VertexTag" ) ) ),
        genParticleToken_( consumes<View<reco::GenParticle> >( iConfig.getParameter<InputTag> ( "GenParticleTag" ) ) ),
        genJetToken_ ( consumes<View<reco::GenJet> >( iConfig.getParameter<InputTag> ( "GenJetTag" ) ) ),
        systLabel_( iConfig.getParameter<string> ( "SystLabel" ) )
    {

        double default_leptonPtThreshold_ = 20.;
        double default_leptonEtaThreshold_ = 2.4;
        double default_leadPhoOverMassThreshold_ = 0.5;
        double default_subleadPhoOverMassThreshold_ = 0.25;
        double default_MVAThreshold_ = -0.6;
        double default_deltaRLepPhoThreshold_ = 0.5;
        double default_deltaRJetLepThreshold_ = 0.5;
        double default_jetsNumberThreshold_ = 2;
        double default_bjetsNumberThreshold_ = 1;
        double default_jetPtThreshold_ = 30.;
        double default_jetEtaThreshold_ = 4.7;


        double default_deltaRJetLeadPhoThreshold_ = 0.5;
        double default_deltaRJetSubLeadPhoThreshold_ = 0.5;

        vector<double> default_bDiscriminator_;
        default_bDiscriminator_.push_back( 0.605 );
        default_bDiscriminator_.push_back( 0.890 );

        string default_bTag_ = "combinedInclusiveSecondaryVertexV2BJetTags";
        double default_muPFIsoSumRelThreshold_ = 0.25;
        double default_PhoMVAThreshold_ = -0.2;
        double default_DeltaRTrkElec_ = 1.;
        double default_TransverseImpactParam_ = 0.02;
        double default_LongitudinalImpactParam_ = 0.2;

        double default_deltaRPhoElectronThreshold_ = 1.;
        double default_Zmass_ = 91.9;
        double default_deltaMassElectronZThreshold_ = 10.;

        vector<double> default_nonTrigMVAThresholds_;
        default_nonTrigMVAThresholds_.push_back(0.913286);
        default_nonTrigMVAThresholds_.push_back(0.805013);
        default_nonTrigMVAThresholds_.push_back(0.358969);

        vector<double> default_nonTrigMVAEtaCuts_;
        default_nonTrigMVAEtaCuts_.push_back(0.8);
        default_nonTrigMVAEtaCuts_.push_back(1.479);
        default_nonTrigMVAEtaCuts_.push_back(2.5);

        double default_electronIsoThreshold_ = 0.15;
        double default_electronNumOfHitsThreshold_ = 1.;

        vector<double> default_electronEtaCuts_;
        default_electronEtaCuts_.push_back( 1.4442 );
        default_electronEtaCuts_.push_back( 1.566 );
        default_electronEtaCuts_.push_back( 2.5 );

        leptonPtThreshold_ = iConfig.getUntrackedParameter<double>( "leptonPtThreshold", default_leptonPtThreshold_ );
        leptonEtaThreshold_ = iConfig.getUntrackedParameter<double>( "leptonEtaThreshold", default_leptonEtaThreshold_ );
        leadPhoOverMassThreshold_ = iConfig.getUntrackedParameter<double>( "leadPhoOverMassThreshold", default_leadPhoOverMassThreshold_ );
        subleadPhoOverMassThreshold_ = iConfig.getUntrackedParameter<double>( "subleadPhoOverMassThreshold", default_subleadPhoOverMassThreshold_ );
        MVAThreshold_ = iConfig.getUntrackedParameter<double>( "MVAThreshold", default_MVAThreshold_ );
        deltaRLepPhoThreshold_ = iConfig.getUntrackedParameter<double>( "deltaRLepPhoThreshold", default_deltaRLepPhoThreshold_ );
        deltaRJetLepThreshold_ = iConfig.getUntrackedParameter<double>( "deltaRJetLepThreshold", default_deltaRJetLepThreshold_ );
        jetsNumberThreshold_ = iConfig.getUntrackedParameter<double>( "jetsNumberThreshold", default_jetsNumberThreshold_ );
        bjetsNumberThreshold_ = iConfig.getUntrackedParameter<double>( "bjetsNumberThreshold", default_bjetsNumberThreshold_ );
        jetPtThreshold_ = iConfig.getUntrackedParameter<double>( "jetPtThreshold", default_jetPtThreshold_ );
        jetEtaThreshold_ = iConfig.getUntrackedParameter<double>( "jetEtaThreshold", default_jetEtaThreshold_ );

        deltaRJetLeadPhoThreshold_ = iConfig.getUntrackedParameter<double>( "deltaRJetLeadPhoThreshold", default_deltaRJetLeadPhoThreshold_ );
        deltaRJetSubLeadPhoThreshold_ = iConfig.getUntrackedParameter<double>( "deltaRJetSubLeadPhoThreshold", default_deltaRJetSubLeadPhoThreshold_ );

        electronEtaCuts_ = iConfig.getUntrackedParameter<vector<double > >( "electronEtaCuts",default_electronEtaCuts_);
        bDiscriminator_ = iConfig.getUntrackedParameter<vector<double > >( "bDiscriminator", default_bDiscriminator_ );
        bTag_ = iConfig.getUntrackedParameter<string>( "bTag", default_bTag_ );

        muPFIsoSumRelThreshold_ = iConfig.getUntrackedParameter<double>( "muPFIsoSumRelThreshold", default_muPFIsoSumRelThreshold_ );
        PhoMVAThreshold_ = iConfig.getUntrackedParameter<double>( "PhoMVAThreshold", default_PhoMVAThreshold_ );
        DeltaRTrkElec_ = iConfig.getUntrackedParameter<double>( "DeltaRTrkElec", default_DeltaRTrkElec_ );
        TransverseImpactParam_ = iConfig.getUntrackedParameter<double>( "TransverseImpactParam", default_TransverseImpactParam_ );
        LongitudinalImpactParam_ = iConfig.getUntrackedParameter<double>( "LongitudinalImpactParam", default_LongitudinalImpactParam_ );

        deltaRPhoElectronThreshold_ = iConfig.getUntrackedParameter<double>( "deltaRPhoElectronThreshold", default_deltaRPhoElectronThreshold_ );
        Zmass_ = iConfig.getUntrackedParameter<double>( "Zmass_", default_Zmass_ );
        deltaMassElectronZThreshold_ = iConfig.getUntrackedParameter<double>( "deltaMassElectronZThreshold_", default_deltaMassElectronZThreshold_ );

        nonTrigMVAThresholds_ =  iConfig.getUntrackedParameter<vector<double > >( "nonTrigMVAThresholds", default_nonTrigMVAThresholds_ );
        nonTrigMVAEtaCuts_ =  iConfig.getUntrackedParameter<vector<double > >( "nonTrigMVAEtaCuts", default_nonTrigMVAEtaCuts_ );
        electronIsoThreshold_ = iConfig.getUntrackedParameter<double>( "electronIsoThreshold", default_electronIsoThreshold_ );
        electronNumOfHitsThreshold_ = iConfig.getUntrackedParameter<double>( "electronNumOfHitsThreshold", default_electronNumOfHitsThreshold_ );

        for (unsigned i = 0 ; i < inputTagJets_.size() ; i++) {
            auto token = consumes<View<flashgg::Jet> >(inputTagJets_[i]);
            tokenJets_.push_back(token);
        }
        produces<vector<THQLeptonicTag> >();
        produces<vector<VBFTagTruth> >();
    }

    void THQLeptonicTagProducer::produce( Event &evt, const EventSetup & )

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

        //Handle<View<flashgg::THQLeptonicMVAResult> > thqleptonicMvaResults;
        //evt.getByToken( thqleptonicMvaResultToken_, thqleptonicMvaResults );

        Handle<View<flashgg::Muon> > theMuons;
        evt.getByToken( muonToken_, theMuons );

        Handle<View<flashgg::Electron> > theElectrons;
        evt.getByToken( electronToken_, theElectrons );

        Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;
        evt.getByToken( mvaResultToken_, mvaResults );

        Handle<View<reco::GenParticle> > genParticles;
        Handle<View<reco::GenJet> > genJets;

        std::auto_ptr<vector<THQLeptonicTag> > thqltags( new vector<THQLeptonicTag> );

        Handle<View<reco::Vertex> > vertices;
        evt.getByToken( vertexToken_, vertices );

        std::auto_ptr<vector<VBFTagTruth> > truths( new vector<VBFTagTruth> );

        unsigned int index_leadq       = std::numeric_limits<unsigned int>::max();
        unsigned int index_subleadq    = std::numeric_limits<unsigned int>::max();
        unsigned int index_subsubleadq    = std::numeric_limits<unsigned int>::max();
        unsigned int index_leadjet       = std::numeric_limits<unsigned int>::max();
        unsigned int index_subleadjet    = std::numeric_limits<unsigned int>::max();
        float pt_leadq = 0., pt_subleadq = 0., pt_subsubleadq = 0.;
        float pt_leadjet = 0., pt_subleadjet = 0.;
        Point higgsVtx;

        if( ! evt.isRealData() ) {
            evt.getByToken( genJetToken_, genJets );
            evt.getByToken( genParticleToken_, genParticles );
            for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) {
                int pdgid = genParticles->ptrAt( genLoop )->pdgId();
                if( pdgid == 25 || pdgid == 22 ) {
                    higgsVtx = genParticles->ptrAt( genLoop )->vertex();
                    break;
                }
            }
            for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) {
                edm::Ptr<reco::GenParticle> part = genParticles->ptrAt( genLoop );
                if( part->isHardProcess() ) {
                    if( abs( part->pdgId() ) <= 5 ) {
                        if( part->pt() > pt_leadq ) {
                            index_subleadq = index_leadq;
                            pt_subleadq = pt_leadq;
                            index_leadq = genLoop;
                            pt_leadq = part->pt();
                        } else if( part->pt() > pt_subleadq ) {
                            index_subsubleadq  = index_subleadq;
                            pt_subsubleadq     = pt_subleadq;
                            index_subleadq = genLoop;
                            pt_subleadq    = part->pt();
                        }else if( part->pt() > pt_subsubleadq ){
                            index_subsubleadq = genLoop;
                            pt_subleadq       = part->pt();
                        }
                    }
                }
            }
        }
        
        edm::RefProd<vector<VBFTagTruth> > rTagTruth = evt.getRefBeforePut<vector<VBFTagTruth> >();
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
            std::vector<edm::Ptr<Jet> > nontagBJets;

            edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotons->ptrAt( diphoIndex );
            edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResults->ptrAt( diphoIndex );

            //edm::Ptr<flashgg::THQLeptonicMVAResult> thqleptonic_mvares = thqleptonicMvaResults->ptrAt( diphoIndex );
            //THQLeptonicMVA_ = thqleptonicMvaResults->ptrAt( diphoIndex );
            //float thqleptonic_mva = thqleptonic_mvares->thqLeptonicMvaResult_value;

            //std::cout << "mva:" << thqleptonic_mva << std::endl;

            flashgg::THQLeptonicTag thqltags_obj( dipho, mvares );

            if( dipho->leadingPhoton()->pt() < ( dipho->mass() )*leadPhoOverMassThreshold_ ) { continue; }

            if( dipho->subLeadingPhoton()->pt() < ( dipho->mass() )*subleadPhoOverMassThreshold_ ) { continue; }


            idmva1 = dipho->leadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );
            idmva2 = dipho->subLeadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );

            if( idmva1 <= PhoMVAThreshold_ || idmva2 <= PhoMVAThreshold_ ) { continue; }

            if( mvares->result < MVAThreshold_ ) { continue; }

            photonSelection = true;
        

            std::vector<edm::Ptr<flashgg::Muon> > goodMuons = selectMuons( theMuons->ptrs(), dipho, vertices->ptrs(), leptonEtaThreshold_ , leptonPtThreshold_,
                    muPFIsoSumRelThreshold_, deltaRLepPhoThreshold_, deltaRLepPhoThreshold_ );


            std::vector<edm::Ptr<Electron> > goodElectrons = selectElectrons( theElectrons->ptrs(), dipho, vertices->ptrs(), leptonPtThreshold_, 
                                                                              TransverseImpactParam_, LongitudinalImpactParam_, nonTrigMVAThresholds_, nonTrigMVAEtaCuts_,
                                                                                 electronIsoThreshold_, electronNumOfHitsThreshold_, electronEtaCuts_ ,
                                                                                 deltaRPhoElectronThreshold_,DeltaRTrkElec_,deltaMassElectronZThreshold_);

            hasGoodElec = ( goodElectrons.size() > 0 );
            hasGoodMuons = ( goodMuons.size() > 0 );
            if( !hasGoodElec && !hasGoodMuons ) { continue; }
            if( hasGoodElec && hasGoodMuons ) { continue; }

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
                        else
                            nontagBJets.push_back( thejet );


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
                        else
                            nontagBJets.push_back( thejet );

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

            
            std::cout << " THQLeptonicTagProducer tagBJets.size()=" << tagBJets.size()
                      << " tagJets.size()=" << tagJets.size()
                      << " photonSelection=" << photonSelection
                      << " tagMuons.size()=" << tagMuons.size() << " muonJets=" << muonJets
                      << " tagElectrons.size()="<< tagElectrons.size() << " ElectronJets=" << ElectronJets
                      << std::endl;
            
        
            if( tagBJets.size() >= bjetsNumberThreshold_ && tagJets.size() >= jetsNumberThreshold_ && photonSelection
                && ( ( (tagMuons.size() == 1 && muonJets) and  (tagElectrons.size() == 0 && !ElectronJets) )  || ( (tagMuons.size() == 0 && !muonJets)  and  (tagElectrons.size() == 1 && ElectronJets) ) ) ) {
                if( tagElectrons.size() > 0 && ElectronJets ) {
                    //                    std::cout << "including electron weights" << std::endl;
                    thqltags_obj.includeWeights( *tagElectrons[0] );
                } else if( tagMuons.size() > 0 && muonJets ) {
                    //                    std::cout << "including muon weights" << std::endl;
                    thqltags_obj.includeWeights( *tagMuons[0] );
                }
                thqltags_obj.includeWeights( *dipho );
                //thqltags_obj.setTHQLeptonicMVA( thqleptonic_mvares );
                thqltags_obj.setJets( tagJets );
                thqltags_obj.setBJets( tagBJets );
                thqltags_obj.setMuons( tagMuons );
                thqltags_obj.setElectrons( tagElectrons );
                thqltags_obj.setDiPhotonIndex( diphoIndex );
                thqltags_obj.setSystLabel( systLabel_ );
                thqltags->push_back( thqltags_obj );
                
                //std::vector<edm::Ptr<flashgg::Jet> > nontagBJets;   
                //std::vector<edm::Ptr<flashgg::Jet> >::iterator it;
                //std::sort (tagBJets.begin(), tagBJets.end());
                //std::sort (tagJets.begin(),tagJets.end());   // 10 20 30 40 50
  
                //it=std::set_difference (tagJets.begin(), tagJets.end(), tagBJets.begin(), tagBJets.end(), nontagBJets.begin());
                if( ! evt.isRealData()  )
                {
                    
                    for( unsigned int jetCollectionIndex = 0; jetCollectionIndex<nontagBJets.size(); ++jetCollectionIndex ) 
                    {
                        //edm::Ptr<flashgg::Jet> thejet = Jets[jetCollectionIndex]->ptrAt( candIndex_outer );
                        edm::Ptr<flashgg::Jet> thejet = tagJets[jetCollectionIndex];
                        //std::cout<< (*it)->pt()<<std::endl;()
                        if( thejet->pt() > pt_leadjet ) 
                        {
                            index_subleadjet = index_leadjet;
                            pt_subleadjet = pt_leadjet;
                            index_leadjet = jetCollectionIndex;
                            pt_leadjet = thejet->pt();
                        } 
                        else if( thejet->pt() > pt_subleadjet )
                        {
                            index_subleadjet = jetCollectionIndex;
                            pt_subleadjet    = thejet->pt();
                        }
                        }
                    std::cout<< index_subleadjet <<" "<< index_leadjet <<" "<< pt_subleadjet << pt_leadjet<<" "<<index_leadq<<  " "<< index_subleadq << " "<< index_subsubleadq << " "<<pt_leadq << " "<<pt_subleadq << " "<<pt_subsubleadq<<std::endl;
                    //edm::Ptr<flashgg::Jet> theleadjet = Jets[jetCollectionIndex]->ptrAt( index_leadjet);
                    
                    //edm::Ptr<flashgg::Jet> thesubleadjet = Jets[jetCollectionIndex]->ptrAt( index_subleadjet );
                    edm::Ptr<flashgg::Jet>  theleadjet; 
                    if (nontagBJets.size()>0)  theleadjet = nontagBJets[ index_leadjet];
                    else   theleadjet = tagJets[ 0 ];
                    //edm::Ptr<flashgg::Jet> thesubleadjet = nontagBJets[ index_subleadjet ];       
                    VBFTagTruth truth_obj;
                    unsigned int index_gp_leadjet = std::numeric_limits<unsigned int>::max();
                    unsigned int index_gp_subleadjet = std::numeric_limits<unsigned int>::max();
                    unsigned int index_gp_leadphoton = std::numeric_limits<unsigned int>::max();
                    unsigned int index_gp_subleadphoton = std::numeric_limits<unsigned int>::max();
                    unsigned int index_gj_leadjet = std::numeric_limits<unsigned int>::max();
                    unsigned int index_gj_subleadjet = std::numeric_limits<unsigned int>::max();
                    
                    float dr_gp_leadjet = 999.;
                    float dr_gp_subleadjet = 999.;
                    float dr_gp_leadphoton = 999.;
                    float dr_gp_subleadphoton = 999.;
                    float dr_gj_leadjet = 999.;
                    float dr_gj_subleadjet = 999.;
                    for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) 
                    {
                        edm::Ptr<reco::GenParticle> part = genParticles->ptrAt( genLoop );
                        if( part->isHardProcess() ) 
                        {
                            
                            float dr = deltaR( theleadjet->eta(), theleadjet ->phi(), part->eta(), part->phi() );
                            if( dr < dr_gp_leadjet ) 
                            {
                                dr_gp_leadjet = dr;
                                index_gp_leadjet = genLoop;
                            }
                            //dr = deltaR( thesubleadjet->eta(), thesubleadjet->phi(), part->eta(), part->phi() );
                            //if( dr < dr_gp_subleadjet ) 
                            // {
                            //   dr_gp_subleadjet = dr;
                            //   index_gp_subleadjet = genLoop;
                            //}
                            dr = deltaR( thqltags_obj.diPhoton()->leadingPhoton()->eta(), thqltags_obj.diPhoton()->leadingPhoton()->phi(), part->eta(), part->phi() );
                            if( dr < dr_gp_leadphoton ) 
                            {
                                dr_gp_leadphoton = dr;
                                index_gp_leadphoton = genLoop;
                            }
                            dr = deltaR( thqltags_obj.diPhoton()->subLeadingPhoton()->eta(), thqltags_obj.diPhoton()->subLeadingPhoton()->phi(), part->eta(), part->phi() );
                            if( dr < dr_gp_subleadphoton ) 
                            {
                                dr_gp_subleadphoton = dr;
                                index_gp_subleadphoton = genLoop;
                            }
                        }
                    }
                    
                    std::cout<< index_gp_leadjet <<" "<< index_gp_subleadjet <<" "<< index_gp_leadphoton << index_gp_subleadphoton <<" "<<index_gj_leadjet <<  " "<< index_gj_subleadjet << " "<< dr_gp_leadjet << " "<<dr_gp_subleadjet << " "<<dr_gp_leadphoton << " "<<dr_gp_subleadphoton<<" "<< dr_gj_leadjet <<" "<< dr_gj_subleadjet<<std::endl;
                    
                    if( index_gp_leadjet < std::numeric_limits<unsigned int>::max() ) { truth_obj.setClosestParticleToLeadingJet( genParticles->ptrAt( index_gp_leadjet ) ); }
                    if( index_gp_subleadjet < std::numeric_limits<unsigned int>::max() ) { truth_obj.setClosestParticleToSubLeadingJet( genParticles->ptrAt( index_gp_subleadjet ) ); }
                    if( index_gp_leadphoton < std::numeric_limits<unsigned int>::max() ) { truth_obj.setClosestParticleToLeadingPhoton( genParticles->ptrAt( index_gp_leadphoton ) ); }
                    if( index_gp_subleadphoton < std::numeric_limits<unsigned int>::max() ) { truth_obj.setClosestParticleToSubLeadingPhoton( genParticles->ptrAt( index_gp_subleadphoton ) ); }
                    
                    for( unsigned int gjLoop = 0 ; gjLoop < genJets->size() ; gjLoop++ ) 
                    {
                        edm::Ptr <reco::GenJet> gj = genJets->ptrAt( gjLoop );
                        float dr = deltaR( theleadjet->eta(), theleadjet->phi(), gj->eta(), gj->phi() );
                        if( dr < dr_gj_leadjet ) 
                        {
                            dr_gj_leadjet = dr;
                            index_gj_leadjet = gjLoop;
                        }
                        //dr = deltaR( thesubleadjet->eta(), thesubleadjet->phi(), gj->eta(), gj->phi() );
                        //if( dr < dr_gj_subleadjet ) 
                        //{
                        //   dr_gj_subleadjet = dr;
                        //index_gj_subleadjet = gjLoop;
                        //}
                    }
                    if( index_gj_leadjet < std::numeric_limits<unsigned int>::max() ) { truth_obj.setClosestGenJetToLeadingJet( genJets->ptrAt( index_gj_leadjet ) ); }
                    if( index_gj_subleadjet < std::numeric_limits<unsigned int>::max() ) { truth_obj.setClosestGenJetToSubLeadingJet( genJets->ptrAt( index_gj_subleadjet ) ); }
                    
                    if( index_leadq < std::numeric_limits<unsigned int>::max() ) { truth_obj.setLeadingParton( genParticles->ptrAt( index_leadq ) ); }
                    if( index_subleadq < std::numeric_limits<unsigned int>::max() ) { truth_obj.setSubLeadingParton( genParticles->ptrAt( index_subleadq ) ); }
                    if( index_subsubleadq < std::numeric_limits<unsigned int>::max()) { truth_obj.setSubSubLeadingParton( genParticles->ptrAt( index_subsubleadq ));}
                    // --------
                    //Partons
                    //Lead
                    std::vector<edm::Ptr<reco::GenParticle>> ptOrderedPartons;
                    for (unsigned int genLoop(0);genLoop < genParticles->size();genLoop++) 
                    {
                        edm::Ptr<reco::GenParticle> gp = genParticles->ptrAt(genLoop);
                        bool isGluon = abs( gp->pdgId() ) < 7 && gp->numberOfMothers() == 0;
                        bool isQuark = gp->pdgId() == 21 && gp->numberOfMothers() == 0;
                        if (isGluon || isQuark) {
                            unsigned int insertionIndex(0);
                            for (unsigned int parLoop(0);parLoop<ptOrderedPartons.size();parLoop++) {
                                if (gp->pt() < ptOrderedPartons[parLoop]->pt()) { insertionIndex = parLoop + 1; }
                            }
                            ptOrderedPartons.insert( ptOrderedPartons.begin() + insertionIndex, gp);
                        }
                    }
                    if ( ptOrderedPartons.size() > 0 ) 
                    {
                        float dr(999.0);
                        unsigned pIndex(0);
                        for (unsigned partLoop(0);partLoop<ptOrderedPartons.size();partLoop++) {
                            float deltaR_temp = deltaR(theleadjet->eta(),theleadjet->phi(),
                                                       ptOrderedPartons[partLoop]->eta(),ptOrderedPartons[partLoop]->phi());
                            if (deltaR_temp < dr) {dr = deltaR_temp; pIndex = partLoop;}
                        }
                        truth_obj.setClosestPartonToLeadingJet( ptOrderedPartons[pIndex] );
                    }
                    
                    //Sublead
                    /*
                    if (ptOrderedPartons.size() > 0) 
                    {
                        float dr(999.0);
                        unsigned pIndex(0);
                        for (unsigned partLoop(0);partLoop<ptOrderedPartons.size();partLoop++) {
                            float deltaR_temp = deltaR(thesubleadjet->eta(),thesubleadjet->phi(),
                                                       ptOrderedPartons[partLoop]->eta(),ptOrderedPartons[partLoop]->phi());
                            if (deltaR_temp < dr) {dr = deltaR_temp; pIndex = partLoop;}
                        }
                        truth_obj.setClosestPartonToSubLeadingJet( ptOrderedPartons[pIndex] );
                    }*/
                    truth_obj.setGenPV( higgsVtx );
                    truths->push_back( truth_obj );
                    thqltags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<VBFTagTruth> >( rTagTruth, idx++ ) ) );
                    }
                } else {
                std::cout << " THQLeptonicTagProducer NO TAG " << std::endl;
            }
            
        }//diPho loop end !
        evt.put( thqltags );
        evt.put( truths );
    }
    
}
typedef flashgg::THQLeptonicTagProducer FlashggTHQLeptonicTagProducer;
DEFINE_FWK_MODULE( FlashggTHQLeptonicTagProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

