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
#include "flashgg/DataFormats/interface/Met.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "DataFormats/TrackReco/interface/HitPattern.h"
#include "flashgg/Taggers/interface/LeptonSelection2018.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "flashgg/DataFormats/interface/TagTruthBase.h"
#include "DataFormats/Common/interface/RefToPtr.h"
#include "SimDataFormats/HTXS/interface/HiggsTemplateCrossSections.h"

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
        EDGetTokenT<View<flashgg::Met> > METToken_;
        EDGetTokenT<View<DiPhotonMVAResult> > mvaResultToken_;
        EDGetTokenT<View<Photon> > photonToken_;
        EDGetTokenT<View<reco::Vertex> > vertexToken_;
        EDGetTokenT<View<reco::GenParticle> > genParticleToken_;
        EDGetTokenT<int> stage0catToken_, stage1catToken_, njetsToken_;
        EDGetTokenT<HTXS::HiggsClassification> newHTXSToken_;
        EDGetTokenT<float> pTHToken_,pTVToken_;
        EDGetTokenT<double> rhoTag_;
        string systLabel_;

        typedef std::vector<edm::Handle<edm::View<flashgg::Jet> > > JetCollectionVector;

        //Thresholds

        double MuonEtaCut_;
        double MuonPtCut_;
        double MuonMiniIsoCut_;
        double MuonPhotonDrCut_;

        double ElePtCut_;
        std::vector<double> EleEtaCuts_;
        double ElePhotonDrCut_;
        double ElePhotonZMassCut_;
        double DeltaRTrkEle_;

        double leadPhoOverMassThreshold_;
        double subleadPhoOverMassThreshold_;
        double MVAThreshold_;
        double deltaRJetLeadPhoThreshold_;
        double deltaRJetSubLeadPhoThreshold_;
        double jetsNumberThreshold_;
        double bjetsNumberThreshold_;
        double jetPtThreshold_;
        double jetEtaThreshold_;
        double deltaRJetLepton_;
        double leadingJetPtThreshold_;
        vector<double> bDiscriminator_;
        string bTag_;
        double PhoMVAThreshold_;

        bool UseCutBasedDiphoId_;
        vector<double> CutBasedDiphoId_;
    };

    TTHLeptonicTagProducer::TTHLeptonicTagProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        //thejetToken_( consumes<View<flashgg::Jet> >( iConfig.getParameter<InputTag>( "JetTag" ) ) ),
        inputTagJets_( iConfig.getParameter<std::vector<edm::InputTag> >( "inputTagJets" ) ),
        electronToken_( consumes<View<flashgg::Electron> >( iConfig.getParameter<InputTag>( "ElectronTag" ) ) ),
        muonToken_( consumes<View<flashgg::Muon> >( iConfig.getParameter<InputTag>( "MuonTag" ) ) ),
        METToken_( consumes<View<flashgg::Met> >( iConfig.getParameter<InputTag>( "MetTag" ) ) ),
        mvaResultToken_( consumes<View<flashgg::DiPhotonMVAResult> >( iConfig.getParameter<InputTag> ( "MVAResultTag" ) ) ),
        vertexToken_( consumes<View<reco::Vertex> >( iConfig.getParameter<InputTag> ( "VertexTag" ) ) ),
        genParticleToken_( consumes<View<reco::GenParticle> >( iConfig.getParameter<InputTag> ( "GenParticleTag" ) ) ),
        rhoTag_( consumes<double>( iConfig.getParameter<InputTag>( "rhoTag" ) ) ),
        systLabel_( iConfig.getParameter<string> ( "SystLabel" ) )
    {

        leadPhoOverMassThreshold_ = iConfig.getParameter<double>( "leadPhoOverMassThreshold");
        subleadPhoOverMassThreshold_ = iConfig.getParameter<double>( "subleadPhoOverMassThreshold");
        MVAThreshold_ = iConfig.getParameter<double>( "MVAThreshold");
        PhoMVAThreshold_ = iConfig.getParameter<double>( "PhoMVAThreshold");
        jetsNumberThreshold_ = iConfig.getParameter<double>( "jetsNumberThreshold");
        bjetsNumberThreshold_ = iConfig.getParameter<double>( "bjetsNumberThreshold");
        jetPtThreshold_ = iConfig.getParameter<double>( "jetPtThreshold");
        jetEtaThreshold_ = iConfig.getParameter<double>( "jetEtaThreshold");
        deltaRJetLepton_ = iConfig.getParameter<double>( "deltaRJetLepton");
        leadingJetPtThreshold_ = iConfig.getParameter<double>("leadingJetPtThreshold");

        MuonEtaCut_ = iConfig.getParameter<double>( "MuonEtaCut");
        MuonPtCut_ = iConfig.getParameter<double>( "MuonPtCut");
        MuonMiniIsoCut_ = iConfig.getParameter<double>( "MuonMiniIsoCut");
        MuonPhotonDrCut_ = iConfig.getParameter<double>( "MuonPhotonDrCut");
 
        EleEtaCuts_ = iConfig.getParameter<std::vector<double>>( "EleEtaCuts");
        ElePtCut_ = iConfig.getParameter<double>( "ElePtCut");
        ElePhotonDrCut_ = iConfig.getParameter<double>( "ElePhotonDrCut");
        ElePhotonZMassCut_ = iConfig.getParameter<double>( "ElePhotonZMassCut");
        DeltaRTrkEle_ = iConfig.getParameter<double>( "DeltaRTrkEle");

        deltaRJetLeadPhoThreshold_ = iConfig.getParameter<double>( "deltaRJetLeadPhoThreshold");
        deltaRJetSubLeadPhoThreshold_ = iConfig.getParameter<double>( "deltaRJetSubLeadPhoThreshold");

        bDiscriminator_ = iConfig.getParameter<vector<double > >( "bDiscriminator");
        bTag_ = iConfig.getParameter<string>( "bTag");

        UseCutBasedDiphoId_ = iConfig.getParameter<bool>( "UseCutBasedDiphoId" );
        CutBasedDiphoId_ = iConfig.getParameter<std::vector<double>>( "CutBasedDiphoId" );

        ParameterSet HTXSps = iConfig.getParameterSet( "HTXSTags" );
        stage0catToken_ = consumes<int>( HTXSps.getParameter<InputTag>("stage0cat") );
        stage1catToken_ = consumes<int>( HTXSps.getParameter<InputTag>("stage1cat") );
        njetsToken_ = consumes<int>( HTXSps.getParameter<InputTag>("njets") );
        pTHToken_ = consumes<float>( HTXSps.getParameter<InputTag>("pTH") );
        pTVToken_ = consumes<float>( HTXSps.getParameter<InputTag>("pTV") );
        newHTXSToken_ = consumes<HTXS::HiggsClassification>( HTXSps.getParameter<InputTag>("ClassificationObj") );

        
        for (unsigned i = 0 ; i < inputTagJets_.size() ; i++) {
            auto token = consumes<View<flashgg::Jet> >(inputTagJets_[i]);
            tokenJets_.push_back(token);
        }

        produces<vector<TTHLeptonicTag> >();
        produces<vector<TagTruthBase> >();
    }

    void TTHLeptonicTagProducer::produce( Event &evt, const EventSetup & )
    {
        Handle<int> stage0cat, stage1cat, njets;
        Handle<float> pTH, pTV;
        evt.getByToken(stage0catToken_, stage0cat);
        evt.getByToken(stage1catToken_,stage1cat);
        evt.getByToken(njetsToken_,njets);
        evt.getByToken(pTHToken_,pTH);
        evt.getByToken(pTVToken_,pTV);
        Handle<HTXS::HiggsClassification> htxsClassification;
        evt.getByToken(newHTXSToken_,htxsClassification);


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

        edm::Handle<double>  rho;
        evt.getByToken(rhoTag_,rho);
        double rho_    = *rho;

        Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;
        evt.getByToken( mvaResultToken_, mvaResults );

        Handle<View<reco::GenParticle> > genParticles;

        Handle<View<reco::Vertex> > vertices;
        evt.getByToken( vertexToken_, vertices );


        std::unique_ptr<vector<TTHLeptonicTag> > tthltags( new vector<TTHLeptonicTag> );
        std::unique_ptr<vector<TagTruthBase> > truths( new vector<TagTruthBase> );
        edm::RefProd<vector<TagTruthBase> > rTagTruth = evt.getRefBeforePut<vector<TagTruthBase> >();
        unsigned int idx = 0;


        Point higgsVtx;

        if( ! evt.isRealData() )
        {
            evt.getByToken( genParticleToken_, genParticles );
            for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ )
            {
                int pdgid = genParticles->ptrAt( genLoop )->pdgId();
                if( pdgid == 25 || pdgid == 22 )
                {
                    higgsVtx = genParticles->ptrAt( genLoop )->vertex();
                    break;
                }
            }
        }

        assert( diPhotons->size() == mvaResults->size() );

        double idmva1 = 0.;
        double idmva2 = 0.;

        for( unsigned int diphoIndex = 0; diphoIndex < diPhotons->size(); diphoIndex++ )
        {
            unsigned int jetCollectionIndex = diPhotons->ptrAt( diphoIndex )->jetCollectionIndex();

            edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotons->ptrAt( diphoIndex );
            edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResults->ptrAt( diphoIndex );

            if( dipho->leadingPhoton()->pt() < ( dipho->mass() )*leadPhoOverMassThreshold_ ) { continue; }
            if( dipho->subLeadingPhoton()->pt() < ( dipho->mass() )*subleadPhoOverMassThreshold_ ) { continue; }
            idmva1 = dipho->leadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );
            idmva2 = dipho->subLeadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );

            if( idmva1 < PhoMVAThreshold_ || idmva2 < PhoMVAThreshold_ ) { continue; }

            bool passDiphotonSelection = true;
            if(UseCutBasedDiphoId_)
            {
                assert(CutBasedDiphoId_.size()==6);
                if(dipho->leadingPhoton()->pt()/dipho->mass() < CutBasedDiphoId_[0]) passDiphotonSelection = false;
                if(dipho->subLeadingPhoton()->pt()/dipho->mass() < CutBasedDiphoId_[1]) passDiphotonSelection = false;
                if(dipho->leadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() ) < CutBasedDiphoId_[2]) passDiphotonSelection = false;
                if(dipho->subLeadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() ) < CutBasedDiphoId_[3]) passDiphotonSelection = false;
                if(abs (dipho->leadingPhoton()->eta() - dipho->subLeadingPhoton()->eta()) > CutBasedDiphoId_[4]) passDiphotonSelection = false;
                if(deltaPhi(dipho->leadingPhoton()->phi(), dipho->subLeadingPhoton()->phi() ) > CutBasedDiphoId_[5] ) passDiphotonSelection = false;
            }
            else
                if( mvares->result < MVAThreshold_ ) passDiphotonSelection = false;

            if(!passDiphotonSelection) continue;

            std::vector<edm::Ptr<flashgg::Muon> >     Muons;
            std::vector<edm::Ptr<flashgg::Electron> > Electrons;
            
            if(theMuons->size()>0)
                Muons = selectMuons(theMuons->ptrs(), dipho, vertices->ptrs(), MuonPtCut_, MuonEtaCut_, MuonMiniIsoCut_, MuonPhotonDrCut_);
            if(theElectrons->size()>0)
                Electrons = selectElectrons(theElectrons->ptrs(), dipho, ElePtCut_, EleEtaCuts_, ElePhotonDrCut_, ElePhotonZMassCut_, DeltaRTrkEle_);

            if( (Muons.size() + Electrons.size()) == 0) continue;

            bool passMuonSelection = false;
            bool passEleSelection  = false;

            int njet_ = 0;
            int njets_btagloose_ = 0;
            int njets_btagmedium_ = 0;
            int njets_btagtight_ = 0;
            float leadingJetPt = -1;
            std::vector<edm::Ptr<Muon> > tagMuons;
            std::vector<edm::Ptr<Electron> > tagElectrons;
            std::vector<edm::Ptr<flashgg::Jet>> tagJets;

            for( unsigned int muonIndex = 0; muonIndex < Muons.size(); muonIndex++ )
            {
                Ptr<flashgg::Muon> muon = Muons[muonIndex];

                njet_ = 0;
                njets_btagloose_ = 0;
                njets_btagmedium_ = 0;
                njets_btagtight_ = 0;
                leadingJetPt = -1;
                tagJets.clear();

                for( unsigned int jetIndex = 0; jetIndex < Jets[jetCollectionIndex]->size() ; jetIndex++ )
                {
                    edm::Ptr<flashgg::Jet> thejet = Jets[jetCollectionIndex]->ptrAt( jetIndex );

                    if( fabs( thejet->eta() ) > jetEtaThreshold_ ) { continue; }
                    if(!thejet->passesJetID  ( flashgg::Tight2017 ) ) { continue; }
                    if( thejet->pt() < jetPtThreshold_ ) { continue; }

                    float dRPhoLeadJet = deltaR( thejet->eta(), thejet->phi(), dipho->leadingPhoton()->superCluster()->eta(), dipho->leadingPhoton()->superCluster()->phi() ) ;
                    float dRPhoSubLeadJet = deltaR( thejet->eta(), thejet->phi(), dipho->subLeadingPhoton()->superCluster()->eta(), dipho->subLeadingPhoton()->superCluster()->phi() );

                    if( dRPhoLeadJet < deltaRJetLeadPhoThreshold_ || dRPhoSubLeadJet < deltaRJetSubLeadPhoThreshold_ ) { continue; }

                    float dRLept = deltaR( thejet->eta(), thejet->phi(), muon->eta(),  muon->phi() ) ;
                    if( dRLept < deltaRJetLepton_) { continue; }

                    njet_++;
                    tagJets.push_back(thejet);
                    if(thejet->pt()>leadingJetPt)
                        leadingJetPt = thejet->pt();

                    float bDiscriminatorValue = -2.;
                    if(bTag_ == "pfDeepCSV") bDiscriminatorValue = thejet->bDiscriminator("pfDeepCSVJetTags:probb")+thejet->bDiscriminator("pfDeepCSVJetTags:probbb") ;
                    else  bDiscriminatorValue = thejet->bDiscriminator( bTag_ );


                    if( bDiscriminatorValue > bDiscriminator_[0] ) njets_btagloose_++;
                    if( bDiscriminatorValue > bDiscriminator_[1] ) njets_btagmedium_++;
                    if( bDiscriminatorValue > bDiscriminator_[2] ) njets_btagtight_++;
                }

                if(njet_ >= jetsNumberThreshold_ && njets_btagmedium_ >= bjetsNumberThreshold_ && leadingJetPt>leadingJetPtThreshold_)

                {
                    passMuonSelection = true;
                    tagMuons.push_back(muon);
                    break;
                }
            }

           for( unsigned int eleIndex = 0; eleIndex < Electrons.size(); eleIndex++ )
            {
                Ptr<flashgg::Electron> ele = Electrons[eleIndex];

                njet_ = 0;
                njets_btagloose_ = 0;
                njets_btagmedium_ = 0;
                njets_btagtight_ = 0;
                leadingJetPt = -1;
                std::vector<flashgg::Jet> ElectronJets;
                ElectronJets.clear();

                for( unsigned int jetIndex = 0; jetIndex < Jets[jetCollectionIndex]->size() ; jetIndex++ )
                {
                    edm::Ptr<flashgg::Jet> thejet = Jets[jetCollectionIndex]->ptrAt( jetIndex );

                    if( fabs( thejet->eta() ) > jetEtaThreshold_ ) { continue; }
                    if(!thejet->passesJetID  ( flashgg::Tight2017 ) ) { continue; }
                    if( thejet->pt() < jetPtThreshold_ ) { continue; }

                    float dRPhoLeadJet = deltaR( thejet->eta(), thejet->phi(), dipho->leadingPhoton()->superCluster()->eta(), dipho->leadingPhoton()->superCluster()->phi() ) ;
                    float dRPhoSubLeadJet = deltaR( thejet->eta(), thejet->phi(), dipho->subLeadingPhoton()->superCluster()->eta(), dipho->subLeadingPhoton()->superCluster()->phi() );

                    if( dRPhoLeadJet < deltaRJetLeadPhoThreshold_ || dRPhoSubLeadJet < deltaRJetSubLeadPhoThreshold_ ) { continue; }

                    float dRLept = deltaR( thejet->eta(), thejet->phi(), ele->eta(),  ele->phi() ) ;
                    if( dRLept < deltaRJetLepton_) { continue; }

                    njet_++;
                    if(std::find(tagJets.begin(), tagJets.end(), thejet) == tagJets.end())
                       tagJets.push_back( thejet );
                    if(thejet->pt()>leadingJetPt)
                        leadingJetPt = thejet->pt();
                   
                    float bDiscriminatorValue = -2.;
                    if(bTag_ == "pfDeepCSV") bDiscriminatorValue = thejet->bDiscriminator("pfDeepCSVJetTags:probb")+thejet->bDiscriminator("pfDeepCSVJetTags:probbb") ;
                    else  bDiscriminatorValue = thejet->bDiscriminator( bTag_ );

                    if( bDiscriminatorValue > bDiscriminator_[0] ) njets_btagloose_++;
                    if( bDiscriminatorValue > bDiscriminator_[1] ) njets_btagmedium_++;
                    if( bDiscriminatorValue > bDiscriminator_[2] ) njets_btagtight_++;
                }

                if(njet_ >= jetsNumberThreshold_ && njets_btagmedium_ >= bjetsNumberThreshold_ && leadingJetPt>leadingJetPtThreshold_)
                {
                    passEleSelection = true;
                    tagElectrons.push_back(ele);
                    break;
                }
            }

            if(passEleSelection || passMuonSelection)
            {
                TTHLeptonicTag tthltags_obj( dipho, mvares );

                for( unsigned num = 0; num < tagJets.size(); num++ )
                    tthltags_obj.includeWeightsByLabel( *tagJets.at(num), "JetBTagCutWeight");

                if(passEleSelection && !passMuonSelection ) 
                    tthltags_obj.includeWeights( *tagElectrons.at(0) );
                else if(passMuonSelection && !passEleSelection)
                    tthltags_obj.includeWeightsByLabel( *tagMuons.at(0), "MuonMiniIsoWeight");
                else
                {
                    if(tagMuons.at(0)->pt() > tagElectrons.at(0)->pt())
                        tthltags_obj.includeWeightsByLabel( *tagMuons.at(0), "MuonMiniIsoWeight" );
                    else
                        tthltags_obj.includeWeights( *tagElectrons.at(0) );
                }

                tthltags_obj.includeWeights( *dipho );
                tthltags_obj.setJets( tagJets );
                tthltags_obj.setMuons( tagMuons );
                tthltags_obj.setElectrons( tagElectrons );
                tthltags_obj.setDiPhotonIndex( diphoIndex );
                tthltags_obj.setSystLabel( systLabel_ );
                tthltags->push_back( tthltags_obj );
 
                if( ! evt.isRealData() )
                {
                    TagTruthBase truth_obj;
                    truth_obj.setGenPV( higgsVtx );
                    if ( stage0cat.isValid() ) 
                    {   truth_obj.setHTXSInfo( *( stage0cat.product() ),
                                               *( stage1cat.product() ),
                                               *( njets.product() ),
                                               *( pTH.product() ),
                                               *( pTV.product() ) );
                    } else if ( htxsClassification.isValid() ) {
                        truth_obj.setHTXSInfo( htxsClassification->stage0_cat,
                                               htxsClassification->stage1_cat_pTjet30GeV,
                                               htxsClassification->jets30.size(),
                                               htxsClassification->p4decay_higgs.pt(),
                                               htxsClassification->p4decay_V.pt() );

                    } else {
                        truth_obj.setHTXSInfo( 0, 0, 0, 0., 0. );
                    }
                    truths->push_back( truth_obj );
                    tthltags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<TagTruthBase> >( rTagTruth, idx++ ) ) );
                }
            }
 
        }//diPho loop end !
        evt.put( std::move( tthltags ) );
        evt.put( std::move( truths ) );
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

