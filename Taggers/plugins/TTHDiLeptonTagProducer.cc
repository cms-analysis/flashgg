#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "flashgg/DataFormats/interface/TTHDiLeptonTag.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/Electron.h"
#include "flashgg/DataFormats/interface/Muon.h"
#include "flashgg/DataFormats/interface/Met.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "DataFormats/TrackReco/interface/HitPattern.h"
#include "flashgg/Taggers/interface/LeptonSelection2018.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "DataFormats/Common/interface/RefToPtr.h"

#include <vector>
#include <algorithm>
#include <string>
#include <utility>
#include "TLorentzVector.h"
#include "TMath.h"
#include "TMVA/Reader.h"


using namespace std;
using namespace edm;


namespace flashgg {
    class TTHDiLeptonTagProducer : public EDProducer
    {

    public:
        TTHDiLeptonTagProducer( const ParameterSet & );
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

        double LeptonsZMassCut_;

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

    TTHDiLeptonTagProducer::TTHDiLeptonTagProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        //thejetToken_( consumes<View<flashgg::Jet> >( iConfig.getParameter<InputTag>( "JetTag" ) ) ),
        inputTagJets_( iConfig.getParameter<std::vector<edm::InputTag> >( "inputTagJets" ) ),
        electronToken_( consumes<View<flashgg::Electron> >( iConfig.getParameter<InputTag>( "ElectronTag" ) ) ),
        muonToken_( consumes<View<flashgg::Muon> >( iConfig.getParameter<InputTag>( "MuonTag" ) ) ),
        METToken_( consumes<View<flashgg::Met> >( iConfig.getParameter<InputTag>( "MetTag" ) ) ),
        mvaResultToken_( consumes<View<flashgg::DiPhotonMVAResult> >( iConfig.getParameter<InputTag> ( "MVAResultTag" ) ) ),
        vertexToken_( consumes<View<reco::Vertex> >( iConfig.getParameter<InputTag> ( "VertexTag" ) ) ),
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
        MuonIsoCut_ = iConfig.getParameter<double>( "MuonIsoCut");
        MuonPhotonDrCut_ = iConfig.getParameter<double>( "MuonPhotonDrCut");
 
        EleEtaCuts_ = iConfig.getParameter<std::vector<double>>( "EleEtaCuts");
        ElePtCut_ = iConfig.getParameter<double>( "ElePtCut");
        ElePhotonDrCut_ = iConfig.getParameter<double>( "ElePhotonDrCut");
        ElePhotonZMassCut_ = iConfig.getParameter<double>( "ElePhotonZMassCut");
        DeltaRTrkEle_ = iConfig.getParameter<double>( "DeltaRTrkEle");

        LeptonsZMassCut_ = iConfig.getParameter<double>( "LeptonsZMassCut");

        deltaRJetLeadPhoThreshold_ = iConfig.getParameter<double>( "deltaRJetLeadPhoThreshold");
        deltaRJetSubLeadPhoThreshold_ = iConfig.getParameter<double>( "deltaRJetSubLeadPhoThreshold");

        bDiscriminator_ = iConfig.getParameter<vector<double > >( "bDiscriminator");
        bTag_ = iConfig.getParameter<string>( "bTag");

        UseCutBasedDiphoId_ = iConfig.getParameter<bool>( "UseCutBasedDiphoId" );
        debug_ = iConfig.getParameter<bool>( "debug" );
        CutBasedDiphoId_ = iConfig.getParameter<std::vector<double>>( "CutBasedDiphoId" );

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
        produces<vector<TTHDiLeptonTag> >();
    }

    void TTHDiLeptonTagProducer::produce( Event &evt, const EventSetup & )
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

        edm::Handle<double>  rho;
        evt.getByToken(rhoTag_,rho);
        double rho_    = *rho;

        Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;
        evt.getByToken( mvaResultToken_, mvaResults );

        Handle<View<reco::Vertex> > vertices;
        evt.getByToken( vertexToken_, vertices );

        Handle<View<flashgg::Met> > theMet_;
        evt.getByToken( METToken_, theMet_ );


        std::unique_ptr<vector<TTHDiLeptonTag> > tthtags( new vector<TTHDiLeptonTag> );

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

            std::vector<edm::Ptr<flashgg::Muon> >     Muons_0;
            std::vector<edm::Ptr<flashgg::Electron> > Electrons_0;

            if(theMuons->size()>0)
                Muons_0 = selectMuons(theMuons->ptrs(), dipho, vertices->ptrs(), MuonPtCut_, MuonEtaCut_, MuonIsoCut_, MuonPhotonDrCut_, debug_);
            if(theElectrons->size()>0)
                Electrons_0 = selectElectrons(theElectrons->ptrs(), dipho, ElePtCut_, EleEtaCuts_, ElePhotonDrCut_, ElePhotonZMassCut_, DeltaRTrkEle_, debug_);

            if( (Muons_0.size() + Electrons_0.size()) < 2) continue;

 
            //Exclude leptons pairs with mass compatible with a Z boson

            std::vector<edm::Ptr<flashgg::Muon> >     Muons;
            std::vector<edm::Ptr<flashgg::Electron> > Electrons;
            if(Muons_0.size()>=2)
            {
                std::vector<int> badIndexes;

                for(unsigned int i=0; i<Muons_0.size(); ++i)
                {
                    for(unsigned int j=i+1; j<Muons_0.size(); ++j)
                    {
                        TLorentzVector l1, l2;
                        l1.SetPtEtaPhiE(Muons_0[i]->pt(), Muons_0[i]->eta(), Muons_0[i]->phi(), Muons_0[i]->energy());
                        l2.SetPtEtaPhiE(Muons_0[j]->pt(), Muons_0[j]->eta(), Muons_0[j]->phi(), Muons_0[j]->energy());

                        if(fabs((l1+l2).M() - 91.187) < LeptonsZMassCut_)
                        {
                            badIndexes.push_back(i);
                            badIndexes.push_back(j);
                        }
                    }
                }

               for(unsigned int i=0; i<Muons_0.size(); ++i)
               {
                    bool isBad = false;
                    for(unsigned int j=0; j<badIndexes.size(); ++j)
                    {
                        if(badIndexes[j]==(int)i)
                            isBad = true;
                    }
                    if(!isBad) Muons.push_back(Muons_0[i]);
               }
            }        

            if(Electrons_0.size()>=2)
            {
                std::vector<int> badIndexes;

                for(unsigned int i=0; i<Electrons_0.size(); ++i)
                {
                    for(unsigned int j=i+1; j<Electrons_0.size(); ++j)
                    {
                        TLorentzVector l1, l2;
                        l1.SetPtEtaPhiE(Electrons_0[i]->pt(), Electrons_0[i]->eta(), Electrons_0[i]->phi(), Electrons_0[i]->energy());
                        l2.SetPtEtaPhiE(Electrons_0[j]->pt(), Electrons_0[j]->eta(), Electrons_0[j]->phi(), Electrons_0[j]->energy());

                        if(fabs((l1+l2).M() - 91.187) < LeptonsZMassCut_)
                        {
                            badIndexes.push_back(i);
                            badIndexes.push_back(j);
                        }
                    }
                }

               for(unsigned int i=0; i<Electrons_0.size(); ++i)
               {
                    bool isBad = false;
                    for(unsigned int j=0; j<badIndexes.size(); ++j)
                    {
                        if(badIndexes[j]==(int)i)
                            isBad = true;
                    }
                    if(!isBad) Electrons.push_back(Electrons_0[i]);
               }
            }        

            if( (Muons.size() + Electrons.size()) < 2) continue;
  
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

                cout << "MVA value " << mvaValue << " " << DiphotonMva_-> EvaluateMVA( "BDT" ) << endl;
            }

            if( mvaValue>MVAThreshold_ )
            {
                TTHDiLeptonTag tthtags_obj( dipho, mvares );

                 for( unsigned int i = 0; i < tagJets.size(); ++i )
                {
                    tthtags_obj.includeWeightsByLabel( *tagJets[i] , "JetBTagReshapeWeight");
                    tthtags_obj.includeWeightsByLabel( *tagJets[i] , "JetBTagCutWeight");
                }


                for( unsigned int i = 0; i < Muons.size(); ++i )
                    tthtags_obj.includeWeights( *Muons.at(i));

               for( unsigned int i = 0; i < Electrons.size(); ++i )
                    tthtags_obj.includeWeights( *Electrons.at(i));
                
                tthtags_obj.includeWeights( *dipho );

                tthtags_obj.setJets( tagJets );
                tthtags_obj.setMuons( Muons );
                tthtags_obj.setElectrons( Electrons );
                tthtags_obj.setDiPhotonIndex( diphoIndex );
                tthtags_obj.setSystLabel( systLabel_ );
                tthtags_obj.setMvaRes(mvaValue);
                tthtags->push_back( tthtags_obj );

             }
        }//diPho loop end !

        evt.put( std::move(tthtags) );
    }

}
typedef flashgg::TTHDiLeptonTagProducer FlashggTTHDiLeptonTagProducer;
DEFINE_FWK_MODULE( FlashggTTHDiLeptonTagProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

