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
#include "TMVA/Reader.h"
#include "TRandom.h"

TRandom* myRandLeptonic = new TRandom();


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

        unique_ptr<TMVA::Reader> DiphotonMva_;
        FileInPath MVAweightfile_;

        //Thresholds

        double MuonEtaCut_;
        double MuonPtCut_;
        double MuonIsoCut_;
        double MuonPhotonDrCut_;

        double ElePtCut_;
        std::vector<double> EleEtaCuts_;
        double ElePhotonDrCut_;
        double ElePhotonZMassCut_;
        double DeltaRTrkEle_;

        double leadPhoOverMassThreshold_;
        double subleadPhoOverMassThreshold_;
        vector<double> MVAThreshold_;
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
        bool debug_;
        vector<double> CutBasedDiphoId_;

        float leadeta_;
        float subleadeta_;
        float leadptom_;
        float subleadptom_;
        float leadIDMVA_;
        float subleadIDMVA_;
        float deltaphi_;
        float leadPSV_;
        float subleadPSV_;
        float nJets_;
        float nJets_bTagMedium_;
        float jet_pt1_;
        float jet_pt2_;
        float jet_pt3_;
        float jet_eta1_;
        float jet_eta2_;
        float jet_eta3_;
        float bTag1_;
        float bTag2_;
        float MetPt_;
        float lepton_leadPt_;
        float lepton_leadEta_;
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
        MVAThreshold_ = iConfig.getParameter<std::vector<double>>( "MVAThreshold");
        PhoMVAThreshold_ = iConfig.getParameter<double>( "PhoMVAThreshold");
        jetsNumberThreshold_ = iConfig.getParameter<double>( "jetsNumberThreshold");
        bjetsNumberThreshold_ = iConfig.getParameter<double>( "bjetsNumberThreshold");
        jetPtThreshold_ = iConfig.getParameter<double>( "jetPtThreshold");
        jetEtaThreshold_ = iConfig.getParameter<double>( "jetEtaThreshold");
        deltaRJetLepton_ = iConfig.getParameter<double>( "deltaRJetLepton");
        leadingJetPtThreshold_ = iConfig.getParameter<double>("leadingJetPtThreshold");

        MuonEtaCut_ = iConfig.getParameter<double>( "MuonEtaCut");
        MuonPtCut_ = iConfig.getParameter<double>( "MuonPtCut");
        MuonIsoCut_ = iConfig.getParameter<double>( "MuonIsoCut");
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
        debug_ = iConfig.getParameter<bool>( "debug" );
        CutBasedDiphoId_ = iConfig.getParameter<std::vector<double>>( "CutBasedDiphoId" );

        ParameterSet HTXSps = iConfig.getParameterSet( "HTXSTags" );
        stage0catToken_ = consumes<int>( HTXSps.getParameter<InputTag>("stage0cat") );
        stage1catToken_ = consumes<int>( HTXSps.getParameter<InputTag>("stage1cat") );
        njetsToken_ = consumes<int>( HTXSps.getParameter<InputTag>("njets") );
        pTHToken_ = consumes<float>( HTXSps.getParameter<InputTag>("pTH") );
        pTVToken_ = consumes<float>( HTXSps.getParameter<InputTag>("pTV") );
        newHTXSToken_ = consumes<HTXS::HiggsClassification>( HTXSps.getParameter<InputTag>("ClassificationObj") );

        MVAweightfile_ = iConfig.getParameter<edm::FileInPath>( "MVAweightfile" );

        DiphotonMva_.reset( new TMVA::Reader( "!Color:Silent" ) );
        DiphotonMva_->AddVariable( "dipho_leadEta", &leadeta_ );
        DiphotonMva_->AddVariable( "dipho_subleadEta", &subleadeta_ );
        DiphotonMva_->AddVariable( "dipho_lead_ptoM", &leadptom_ );
        DiphotonMva_->AddVariable( "dipho_sublead_ptoM", &subleadptom_ );
        DiphotonMva_->AddVariable( "dipho_leadIDMVA", &leadIDMVA_ );
        DiphotonMva_->AddVariable( "dipho_subleadIDMVA", &subleadIDMVA_ );
        DiphotonMva_->AddVariable( "dipho_deltaphi", &deltaphi_ );
        DiphotonMva_->AddVariable( "dipho_lead_PSV", &leadPSV_ );
        DiphotonMva_->AddVariable( "dipho_sublead_PSV", &subleadPSV_ );
        DiphotonMva_->AddVariable( "nJets", &nJets_ );
        DiphotonMva_->AddVariable( "nJets_bTagMedium", &nJets_bTagMedium_ );
        DiphotonMva_->AddVariable( "jet1_pt", &jet_pt1_ );
        DiphotonMva_->AddVariable( "jet2_pt", &jet_pt2_ );
        DiphotonMva_->AddVariable( "jet3_pt", &jet_pt3_ );
        DiphotonMva_->AddVariable( "jet1_eta", &jet_eta1_ );
        DiphotonMva_->AddVariable( "jet2_eta", &jet_eta2_ );
        DiphotonMva_->AddVariable( "jet3_eta", &jet_eta3_ );
        DiphotonMva_->AddVariable( "bTag1", &bTag1_ );
        DiphotonMva_->AddVariable( "bTag2", &bTag2_ );
        DiphotonMva_->AddVariable( "MetPt", &MetPt_ );
        DiphotonMva_->AddVariable( "lepton_leadPt", &lepton_leadPt_ );
        DiphotonMva_->AddVariable( "lepton_leadEta", &lepton_leadEta_ );

        DiphotonMva_->BookMVA( "BDT", MVAweightfile_.fullPath() );

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

        Handle<View<flashgg::Met> > theMet_;
        evt.getByToken( METToken_, theMet_ );


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

            if(debug_)
                cout << "Photon pair with PhoIdMVA values: " << idmva1 << " " << idmva2 << endl;
 
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

            if(!passDiphotonSelection) continue;

            std::vector<edm::Ptr<flashgg::Muon> >     Muons;
            std::vector<edm::Ptr<flashgg::Electron> > Electrons;

            if(theMuons->size()>0)
                Muons = selectMuons(theMuons->ptrs(), dipho, vertices->ptrs(), MuonPtCut_, MuonEtaCut_, MuonIsoCut_, MuonPhotonDrCut_, debug_);
            if(theElectrons->size()>0)
                Electrons = selectElectrons(theElectrons->ptrs(), dipho, ElePtCut_, EleEtaCuts_, ElePhotonDrCut_, ElePhotonZMassCut_, DeltaRTrkEle_, debug_);

            if( (Muons.size() + Electrons.size())!=1) continue;
 
            int njet_ = 0;
            int njets_btagloose_ = 0;
            int njets_btagmedium_ = 0;
            int njets_btagtight_ = 0;
            std::vector<edm::Ptr<flashgg::Jet>> tagJets;
            std::vector<edm::Ptr<flashgg::Jet>> tagBJets;
            std::vector<float> bTags;

            for( unsigned int jetIndex = 0; jetIndex < Jets[jetCollectionIndex]->size() ; jetIndex++ )
            {
                edm::Ptr<flashgg::Jet> thejet = Jets[jetCollectionIndex]->ptrAt( jetIndex );

                if( fabs( thejet->eta() ) > jetEtaThreshold_ ) { continue; }
                if(!thejet->passesJetID  ( flashgg::Loose ) ) { continue; }
                if( thejet->pt() < jetPtThreshold_ ) { continue; }

                float dRPhoLeadJet = deltaR( thejet->eta(), thejet->phi(), dipho->leadingPhoton()->eta(), dipho->leadingPhoton()->phi() ) ;
                float dRPhoSubLeadJet = deltaR( thejet->eta(), thejet->phi(), dipho->subLeadingPhoton()->eta(), dipho->subLeadingPhoton()->phi() );

                if( dRPhoLeadJet < deltaRJetLeadPhoThreshold_ || dRPhoSubLeadJet < deltaRJetSubLeadPhoThreshold_ ) { continue; }

                bool passDrLeptons = 1;                

                for( unsigned int muonIndex = 0; muonIndex < Muons.size(); muonIndex++ )
                {
                    Ptr<flashgg::Muon> muon = Muons[muonIndex];

                    float dRLept = deltaR( thejet->eta(), thejet->phi(), muon->eta(),  muon->phi() ) ;
                    if( dRLept > deltaRJetLepton_) { continue; }

                    passDrLeptons = 0;
                    break;
                }

                if(passDrLeptons)
                {   for( unsigned int eleIndex = 0; eleIndex < Electrons.size(); eleIndex++ )
                    {
                        Ptr<flashgg::Electron> ele = Electrons[eleIndex];
                    
                        float dRLept = deltaR( thejet->eta(), thejet->phi(), ele->eta(),  ele->phi() ) ;
                        if( dRLept > deltaRJetLepton_) { continue; }

                        passDrLeptons = 0;
                        break;
                    }
                }

                if(passDrLeptons)
                {
                    njet_++;
                    tagJets.push_back( thejet );
                    float bDiscriminatorValue = -2.;
                    if(bTag_ == "pfDeepCSV") bDiscriminatorValue = thejet->bDiscriminator("pfDeepCSVJetTags:probb")+thejet->bDiscriminator("pfDeepCSVJetTags:probbb") ;
                    else  bDiscriminatorValue = thejet->bDiscriminator( bTag_ );

                    bDiscriminatorValue >= 0. ? bTags.push_back(bDiscriminatorValue) : bTags.push_back(-1.);

                    if( bDiscriminatorValue > bDiscriminator_[0] ) njets_btagloose_++;
                    if( bDiscriminatorValue > bDiscriminator_[1] ) njets_btagmedium_++;
                    if( bDiscriminatorValue > bDiscriminator_[2] ) njets_btagtight_++;

                    if( bDiscriminatorValue > bDiscriminator_[1] )
                        tagBJets.push_back( thejet );
                }
                 
            }

            if(njet_ < jetsNumberThreshold_ || njets_btagmedium_ < bjetsNumberThreshold_) continue;
 
            if(debug_)
                cout << "Jets after selections " << njet_ << ", bJets " << njets_btagmedium_ << endl;

            std::sort(bTags.begin(),bTags.end(),std::greater<float>());

            // Set variables to compute MVA value

            leadeta_ = dipho->leadingPhoton()->eta();
            subleadeta_ = dipho->subLeadingPhoton()->eta();
            leadptom_ = dipho->leadingPhoton()->pt()/dipho->mass();
            subleadptom_ = dipho->subLeadingPhoton()->pt()/dipho->mass();
            leadIDMVA_ = dipho->leadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );
            subleadIDMVA_ = dipho->subLeadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );
            deltaphi_ = deltaPhi( dipho->leadingPhoton()->phi(), dipho->subLeadingPhoton()->phi() );
            leadPSV_ = dipho->leadingPhoton()->hasPixelSeed();
            subleadPSV_ = dipho->subLeadingPhoton()->hasPixelSeed();
            nJets_ = njet_;
            nJets_bTagMedium_ = njets_btagmedium_;

            if(tagJets.size()==1)
            {
                jet_pt1_ = tagJets[0]->pt();
                jet_pt2_ = -1.;
                jet_pt3_ = -1.;
                jet_eta1_ = tagJets[0]->eta();
                jet_eta2_ = -5.;
                jet_eta3_ = -5.;

                bTag1_ = bTags[0];
                bTag2_ = -1.;
            }
            else if(tagJets.size()==2)
            {
                jet_pt1_ = tagJets[0]->pt();
                jet_pt2_ = tagJets[1]->pt();
                jet_pt3_ = -1.;
                jet_eta1_ = tagJets[0]->eta();
                jet_eta2_ = tagJets[1]->eta();
                jet_eta3_ = -5.;

                bTag1_ = bTags[0];
                bTag2_ = bTags[1];
            }
            else
            {
                jet_pt1_ = tagJets[0]->pt();
                jet_pt2_ = tagJets[1]->pt();
                jet_pt3_ = tagJets[2]->pt();
                jet_eta1_ = tagJets[0]->eta();
                jet_eta2_ = tagJets[1]->eta();
                jet_eta3_ = tagJets[2]->eta();

                bTag1_ = bTags[0];
                bTag2_ = bTags[1];
            }

            if( theMet_ -> size() != 1 )
                std::cout << "WARNING number of MET is not equal to 1" << std::endl;
             MetPt_ = theMet_->ptrAt( 0 ) -> pt();

            int leadMuIndex = 0;
            float leadMuPt = -1;
            int leadEleIndex = 0;
            float leadElePt = -1;

            for( unsigned int muonIndex = 0; muonIndex < Muons.size(); muonIndex++ )
            {
                Ptr<flashgg::Muon> muon = Muons[muonIndex];

                if(muon->pt()>leadMuPt)
                {
                    leadMuPt = muon->pt();
                    leadMuIndex = muonIndex;
                }
            }

            for( unsigned int eleIndex = 0; eleIndex < Electrons.size(); eleIndex++ )
            {
                Ptr<flashgg::Electron> ele = Electrons[eleIndex];

                if(ele->pt()>leadElePt)
                {
                    leadElePt = ele->pt();
                    leadEleIndex = eleIndex;
                }
            }

            if(leadMuPt>=leadElePt)
            {
                lepton_leadPt_ = Muons[leadMuIndex]->pt();
                lepton_leadEta_ = Muons[leadMuIndex]->eta();
            }
            else
            {
                lepton_leadPt_ = Electrons[leadEleIndex]->pt();
                lepton_leadEta_ = Electrons[leadEleIndex]->eta();
            }

            float mvaValue = DiphotonMva_-> EvaluateMVA( "BDT" );
            int catNumber = -1;
            if(mvaValue>MVAThreshold_[0]) catNumber = 0;
            else if(mvaValue>MVAThreshold_[1] && mvaValue<MVAThreshold_[0]) catNumber = 1;

            if(debug_)
            {
                cout << "MVA iput variables: " << endl;
                cout << "Lead and sublead photon eta " << leadeta_ << " " << subleadeta_ << endl;
                cout << "Lead and sublead photon pt/m " << leadptom_ << " " << subleadptom_ << endl;
                cout << "Lead and sublead photon IdMVA " << leadIDMVA_ << " " << subleadIDMVA_ << endl;
                cout << "Lead and sublead photon PSV " << leadPSV_ << " " << subleadPSV_ << endl;
                cout << "Photon delta phi " << deltaphi_ << endl;
                cout << "Number of jets " << nJets_ << endl;
                cout << "Number of b-jets " << nJets_bTagMedium_  << endl;
                cout << "Pt of the three leading jets " << jet_pt1_ << " " << jet_pt2_ << " " << jet_pt3_ << endl;
                cout << "Eta of the three leading jets " << jet_eta1_ << " " << jet_eta2_ << " " << jet_eta3_ << endl;
                cout << "Two highest bTag scores " << bTag1_ << " " << bTag2_ << endl;
                cout << "MetPt " << MetPt_ << endl;
                cout << "Lepton pT and Eta " << lepton_leadPt_ << " " << lepton_leadEta_ << endl;

                cout << "MVA value " << mvaValue << " " << DiphotonMva_-> EvaluateMVA( "BDT" ) << ", category " << catNumber << endl;
            }

            if(catNumber!=-1)
            {
                TTHLeptonicTag tthltags_obj( dipho, mvares );
                tthltags_obj.setCategoryNumber(catNumber);

                for( unsigned int i = 0; i < tagJets.size(); ++i )
                {
                    tthltags_obj.includeWeightsByLabel( *tagJets[i] , "JetBTagReshapeWeight");
                    tthltags_obj.includeWeightsByLabel( *tagJets[i] , "JetBTagCutWeight");
                }


                for( unsigned int i = 0; i < Muons.size(); ++i )
                    tthltags_obj.includeWeights( *Muons.at(i));

               for( unsigned int i = 0; i < Electrons.size(); ++i )
                    tthltags_obj.includeWeights( *Electrons.at(i));

                tthltags_obj.includeWeights( *dipho );
                tthltags_obj.setJets( tagJets );
                tthltags_obj.setMuons( Muons );
                tthltags_obj.setElectrons( Electrons );
                tthltags_obj.setDiPhotonIndex( diphoIndex );
                tthltags_obj.setSystLabel( systLabel_ );
                tthltags_obj.setMvaRes(mvaValue);

                Ptr<flashgg::Met> Met = theMet_->ptrAt( 0 );
                tthltags_obj.setMetPt((float)Met->pt());
                tthltags_obj.setMetPhi((float)Met->phi());
                tthltags_obj.setRand(myRandLeptonic->Rndm());
                
                // find highest pT lep for mT calc
                ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > lep;
                bool single_lepton = Muons.size() + Electrons.size() == 1;
                if (single_lepton)
                    lep = Muons.size() == 1 ? Muons[0]->p4() : Electrons[0]->p4();
                else { // find the highest pT lep
                    bool ele = 1; // highest pt comes from an elec
                    int idx = -1; // idx of highest pt lep
                    double highest_pt = -1;

                    for (unsigned int i = 0; i < Electrons.size(); i++) {
                        if (Electrons[i]->p4().pt() > highest_pt) {
                            highest_pt = Electrons[i]->p4().pt();
                            ele = true;
                            idx = i;
                        }
                    }
                    for (unsigned int i = 0; i < Muons.size(); i++) {
                        if (Muons[i]->p4().pt() > highest_pt) {
                            highest_pt = Muons[i]->p4().pt();
                            ele = false;
                            idx = i;
                        }
                    }
                    lep = ele ? Electrons[idx]->p4() : Muons[idx]->p4();
                }
                     
                float pt1 = lep.pt();
                float pt2 = Met->p4().pt();
                float phi1 = lep.phi();
                float phi2 = Met->p4().phi();
                float MT_ = sqrt( 2 * pt1 * pt2 * ( 1 - cos( phi1 - phi2 ) ) );
                tthltags_obj.setMT(MT_);

                /*
                ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > lep(0,0,0,0);
                for (unsigned int i = 0; i < Muons.size(); i++) {
                    if (Muons[i]->p4().pt() > lep.pt())
                        lep = Muons[i]->p4();
                }
                for (unsigned int i = 0; i < Electrons.size(); i++) {
                    if (Electrons[i]->p4().pt() > lep.pt())
                        lep = Electrons[i]->p4();
                }
                float MT_ = (Met->p4() + lep).Mt(); 
                tthltags_obj.setMT(MT_);
                */
                tthltags_obj.setDiphoMVARes((float)mvares->result);
                tthltags_obj.setNBLoose( njets_btagloose_ );
                tthltags_obj.setNBMedium( njets_btagmedium_ );
                tthltags_obj.setNBTight( njets_btagtight_ );

                if( ! evt.isRealData() ) {
                    evt.getByToken( genParticleToken_, genParticles );
                    int nGoodEls(0), nGoodMus(0), nGoodElsFromTau(0), nGoodMusFromTau(0), nGoodTaus(0);
                    for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) {
                        int pdgid = genParticles->ptrAt( genLoop )->pdgId();
                        double pt = genParticles->ptrAt( genLoop )->p4().pt();
                        int status = genParticles->ptrAt( genLoop )->status(); 
                        bool isPromptFinalState = genParticles->ptrAt( genLoop )->isPromptFinalState();
                        bool isPromptDecayed = genParticles->ptrAt( genLoop )->isPromptDecayed();
                        bool isDirectPromptTauDecayProductFinalState = genParticles->ptrAt( genLoop )->isDirectPromptTauDecayProductFinalState();              
                        if (pt < 20) continue;
                        if (abs(pdgid) == 11 || abs(pdgid) == 13 || abs(pdgid) == 15) {
                            cout << "Found a gen lepton/tau with pT > 20" << endl;
                            cout << "pdgid: " << pdgid << endl;
                            cout << "pt: " << pt << endl;
                            cout << "status: " << status << endl;
                            cout << "isPromptFinalState: " << isPromptFinalState << endl;
                            cout << "isPromptDecayed: " << isPromptDecayed << endl;
                            cout << "isDirectPromptTauDecayProductFinalState: " << isDirectPromptTauDecayProductFinalState << endl;
                        }
                        if (abs(pdgid) == 11 && status == 1 && (isPromptFinalState || isDirectPromptTauDecayProductFinalState)) {
                            nGoodEls++;
                            if (isDirectPromptTauDecayProductFinalState)
                                nGoodElsFromTau++;
                        } 
                        else if (abs(pdgid) == 13 && status == 1 && (isPromptFinalState || isDirectPromptTauDecayProductFinalState)) {
                            nGoodMus++;
                            if (isDirectPromptTauDecayProductFinalState)
                                nGoodMusFromTau++;
                        }
                        if (abs(pdgid) == 15 && status == 2 && isPromptDecayed) {
                            nGoodTaus++;
                        }
                    }
                    bool lead_photon_is_electron(false), sublead_photon_is_electron(false);
                    double lead_photon_eta = dipho->leadingPhoton()->eta();
                    double lead_photon_phi = dipho->leadingPhoton()->phi();
                    double sublead_photon_eta = dipho->subLeadingPhoton()->eta();
                    double sublead_photon_phi = dipho->subLeadingPhoton()->phi();
                    for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) {
                        int pdgid = genParticles->ptrAt( genLoop )->pdgId();
                        if (abs(pdgid) != 11) continue;
                        if (genParticles->ptrAt( genLoop )->p4().pt() < 15) continue;
                        if (!genParticles->ptrAt( genLoop )->isPromptFinalState()) continue;
                        double electron_eta = genParticles->ptrAt( genLoop )->p4().eta();
                        double electron_phi = genParticles->ptrAt( genLoop )->p4().phi();

                        cout << "Found an electron with pT: " << genParticles->ptrAt( genLoop )->p4().pt() << endl;


                        double deltaR_lead = sqrt( pow(electron_eta - lead_photon_eta, 2) + pow(electron_phi - lead_photon_phi, 2));
                        double deltaR_sublead = sqrt( pow(electron_eta - sublead_photon_eta, 2) + pow(electron_phi - sublead_photon_phi, 2));

                        cout << "Delta R with leading photon: " << deltaR_lead << endl;
                        cout << "Delta R with subleading photon: " << deltaR_sublead << endl;

                        const double deltaR_thresh = 0.1;
                        if (deltaR_lead < deltaR_thresh)
                            lead_photon_is_electron = true;
                        if (deltaR_sublead < deltaR_thresh)
                            sublead_photon_is_electron = true;

                    } // end gen loop

                    int lead_photon_type;
                    if (dipho->leadingPhoton()->genMatchType() != 1) { // fake
                        if (!lead_photon_is_electron)
                            lead_photon_type = 3;   // fake
                        else
                            lead_photon_type = 2;   // fake from  electron 
                    }
                    else
                        lead_photon_type = 1; // prompt
                    int sublead_photon_type;
                    if (dipho->subLeadingPhoton()->genMatchType() != 1) { // fake
                        if (!sublead_photon_is_electron)
                            sublead_photon_type = 3;   // fake
                        else
                            sublead_photon_type = 2;   // fake from  electron 
                    }
                    else
                        sublead_photon_type = 1; // prompt


                    // Do our own gen-matching
                    double lead_photon_closest_match = dipho->leadingPhoton()->genMatchType() == 1 ? sqrt( pow(lead_photon_eta - dipho->leadingPhoton()->matchedGenPhoton()->eta(), 2) + pow(lead_photon_phi - dipho->leadingPhoton()->matchedGenPhoton()->phi(), 2)) : 999;
                    double sublead_photon_closest_match = dipho->subLeadingPhoton()->genMatchType() == 1 ? sqrt( pow(sublead_photon_eta - dipho->subLeadingPhoton()->matchedGenPhoton()->eta(), 2) + pow(sublead_photon_phi - dipho->subLeadingPhoton()->matchedGenPhoton()->phi(), 2)) : 999;
                    double lead_photon_closest_match_pt = dipho->leadingPhoton()->genMatchType() == 1 ? dipho->leadingPhoton()->pt() : -1;
                    double sublead_photon_closest_match_pt = dipho->subLeadingPhoton()->genMatchType() == 1 ? dipho->subLeadingPhoton()->pt() : -1;

                    for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) {
                        int pdgid = genParticles->ptrAt( genLoop )->pdgId();
                        if (abs(pdgid) != 22) continue;
                        if (genParticles->ptrAt( genLoop )->p4().pt() < 10) continue;
                        if (!genParticles->ptrAt( genLoop )->isPromptFinalState()) continue;
                        double gen_photon_candidate_eta = genParticles->ptrAt( genLoop )->p4().eta();
                        double gen_photon_candidate_phi = genParticles->ptrAt( genLoop )->p4().phi();

                        double deltaR_lead = sqrt( pow(gen_photon_candidate_eta - lead_photon_eta, 2) + pow(gen_photon_candidate_phi - lead_photon_phi, 2));
                        double deltaR_sublead = sqrt( pow(gen_photon_candidate_eta - sublead_photon_eta, 2) + pow(gen_photon_candidate_phi - sublead_photon_phi, 2));

                        if (deltaR_lead < lead_photon_closest_match) {
                            lead_photon_closest_match = deltaR_lead;
                            lead_photon_closest_match_pt = genParticles->ptrAt( genLoop )->p4().pt();
                        }
                        if (deltaR_sublead < sublead_photon_closest_match) {
                            sublead_photon_closest_match = deltaR_sublead;
                            sublead_photon_closest_match_pt = genParticles->ptrAt( genLoop )->p4().pt();
                        }
                    } // end gen loop
                    string lead_fake = dipho->leadingPhoton()->genMatchType() == 1 ? "prompt" : "fake";
                    //cout << "Leading photon is " << lead_fake << ". Closest match:" << lead_photon_closest_match << endl;

                    string sublead_fake = dipho->subLeadingPhoton()->genMatchType() == 1 ? "prompt" : "fake";
                    //cout << "Subleading photon is " << sublead_fake << ". Closest match:" << sublead_photon_closest_match << endl;                     


                    tthltags_obj.setnGoodEls(nGoodEls);
                    tthltags_obj.setnGoodElsFromTau(nGoodElsFromTau);
                    tthltags_obj.setnGoodMus(nGoodMus);
                    tthltags_obj.setnGoodMusFromTau(nGoodMusFromTau);
                    tthltags_obj.setnGoodTaus(nGoodTaus);
                    tthltags_obj.setLeadPhotonType(lead_photon_type);
                    tthltags_obj.setSubleadPhotonType(sublead_photon_type);
                    tthltags_obj.setLeadPhotonClosestDeltaR(lead_photon_closest_match);
                    tthltags_obj.setSubleadPhotonClosestDeltaR(sublead_photon_closest_match);
                    tthltags_obj.setLeadPhotonClosestPt(lead_photon_closest_match_pt);
                    tthltags_obj.setSubleadPhotonClosestPt(sublead_photon_closest_match_pt);
                }
                else {
                    tthltags_obj.setnGoodEls(-1);
                    tthltags_obj.setnGoodElsFromTau(-1);
                    tthltags_obj.setnGoodMus(-1);
                    tthltags_obj.setnGoodMusFromTau(-1);
                    tthltags_obj.setnGoodTaus(-1);
                    tthltags_obj.setLeadPhotonType(-1);
                    tthltags_obj.setSubleadPhotonType(-1);
                    tthltags_obj.setLeadPhotonClosestDeltaR(-1);
                    tthltags_obj.setSubleadPhotonClosestDeltaR(-1);
                    tthltags_obj.setLeadPhotonClosestPt(-1);
                    tthltags_obj.setSubleadPhotonClosestPt(-1);
                } 


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

