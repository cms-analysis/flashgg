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
#include "flashgg/DataFormats/interface/Electron.h"
#include "flashgg/DataFormats/interface/Muon.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
//#include "DataFormats/PatCandidates/interface/MET.h"

#include "flashgg/DataFormats/interface/Met.h"

#include "DataFormats/TrackReco/interface/HitPattern.h"
#include "flashgg/Taggers/interface/LeptonSelection.h"

#include "DataFormats/Math/interface/deltaR.h"

//include "flashgg/DataFormats/interface/TagTruthBase.h"
#include "flashgg/DataFormats/interface/THQLeptonicTagTruth.h"
#include "DataFormats/Common/interface/RefToPtr.h"

#include "flashgg/Taggers/interface/SemiLepTopQuark.h"
//#include "PhysicsTools/CandUtils/interface/EventShapeVariables.h"
#include "flashgg/Taggers/interface/FoxWolfram.hpp"

#include "flashgg/DataFormats/interface/PDFWeightObject.h"
#include "SimDataFormats/HTXS/interface/HiggsTemplateCrossSections.h"
#include <vector>
#include <algorithm>
#include <string>
#include <utility>
#include "TLorentzVector.h"
#include "TMath.h"
#include "TMVA/Reader.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "flashgg/Taggers/interface/TTH_DNN_Helper.h"
#include "TCanvas.h"
#include <map>
#include <typeinfo>
using namespace std;
using namespace edm;


namespace flashgg {
class CTCVWeightedVariable {
public:
    CTCVWeightedVariable( string name , string title , int nBins , double min , double max ) {
        Name = name;
        edm::Service<TFileService> fs;
        Directory = fs->mkdir( name ) ;
        for (uint i = 0 ; i < 70 ; i++) {
            Histos.push_back( Directory.make< TH1D >( ("ctcv_"+to_string(i)).c_str() , (title + "," + to_string(i)).c_str() , nBins , min, max ) );
        }
    };

    void Fill( double value , std::vector<double> weights) {
        Histos[0]->Fill( value );
        for( uint i = 0 ; i < weights.size() ; i++)
            Histos[i+1]->Fill( value , weights[i] );
    };

    void Write() {
        Directory.make< TCanvas >( ("Canvas_"+Name).c_str() );
        for( auto h : Histos )
            h->DrawNormalized();
    }

    TFileDirectory Directory;
    vector< TH1* > Histos ;
    string Name;
};


class THQLeptonicTagProducer : public EDProducer
{

public:
    typedef math::XYZPoint Point;
    map< string , CTCVWeightedVariable* > CTCVWeightedVariables;
    THQLeptonicTagProducer( const ParameterSet & );
    ~THQLeptonicTagProducer();

private:
    std::string processId_;
//    edm::EDGetTokenT< LHEEventProduct > token_lhe;
//    int  chooseCategory( float, float);
    int  chooseCategory( float );
    void produce( Event &, const EventSetup & ) override;
    virtual void beginJob() override {
        if(processId_.find("thq") != std::string::npos or processId_.find("thw") != std::string::npos) {
            // CTCVWeightedVariables["photon1pt"] = new CTCVWeightedVariable("photon1pt" , "photon1pt" , 20 , 20 , 300 );
            // CTCVWeightedVariables["photon2pt"] = new CTCVWeightedVariable("photon2pt" , "photon2pt" , 20 , 20 , 300 );
            // CTCVWeightedVariables["diPhotonPt"] = new CTCVWeightedVariable("diPhotonPt" , "diPhotonPt" , 20 , 20 , 300 );
            // CTCVWeightedVariables["diPhotonEta"] = new CTCVWeightedVariable("diPhotonEta" , "diPhotonEta" , 10 , 0 , 4 );
            // CTCVWeightedVariables["diPhotonMVA"] = new CTCVWeightedVariable("diPhotonMVA" , "diPhotonMVA" , 20 , -1 , 1 );
            // CTCVWeightedVariables["LeptonPt"] = new CTCVWeightedVariable("LeptonPt" , "LeptonPt" , 20 , 20 , 220 );
            // CTCVWeightedVariables["LeptonEta"] = new CTCVWeightedVariable("LeptonPt" , "LeptonEta" , 5 , 0 , 2.5 );
            // CTCVWeightedVariables["nJets"] = new CTCVWeightedVariable("nJets" , "nJets" , 5 , 0 , 5 );
            // CTCVWeightedVariables["nbJets"] = new CTCVWeightedVariable("nbJets" , "nbJets" , 5 , 0 , 5 );
            // CTCVWeightedVariables["MET"] = new CTCVWeightedVariable("MET" , "MET" , 10 , 30 , 230 );
            // CTCVWeightedVariables["jPrimeEta"] = new CTCVWeightedVariable("jPrimeEta" , "jPrimeEta" , 5, 0 , 5 );
        }
    };

    virtual void endJob() override {
        // CTCVWeightedVariables["photon1pt"]->Write();
        // CTCVWeightedVariables["photon2pt"]->Write();
        // CTCVWeightedVariables["diPhotonPt"]->Write();
        // CTCVWeightedVariables["diPhotonEta"]->Write();
        // CTCVWeightedVariables["diPhotonMVA"]->Write();
        // CTCVWeightedVariables["LeptonPt"]->Write();
        // CTCVWeightedVariables["LeptonEta"]->Write();
        // CTCVWeightedVariables["nJets"]->Write();
        // CTCVWeightedVariables["nbJets"]->Write();
        // CTCVWeightedVariables["MET"]->Write();
        // CTCVWeightedVariables["jPrimeEta"]->Write();
    };

    std::vector<edm::EDGetTokenT<View<flashgg::Jet> > > tokenJets_;
    EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
    std::vector<edm::InputTag> inputTagJets_;
    EDGetTokenT<View<Electron> > electronToken_;
    EDGetTokenT<View<flashgg::Muon> > muonToken_;
    EDGetTokenT<View<DiPhotonMVAResult> > mvaResultToken_;
    EDGetTokenT<View<Photon> > photonToken_;
    EDGetTokenT<View<reco::Vertex> > vertexToken_;
    EDGetTokenT<View<flashgg::Met> > METToken_;
    EDGetTokenT<View<reco::GenParticle> > genParticleToken_;
    EDGetTokenT<View<reco::GenJet> > genJetToken_;
    edm::EDGetTokenT<vector<flashgg::PDFWeightObject> > weightToken_;
    EDGetTokenT<double> rhoTag_;
    string systLabel_;


    typedef std::vector<edm::Handle<edm::View<flashgg::Jet> > > JetCollectionVector;

    //Thresholds
    double muonPtThreshold_;
    double muonEtaThreshold_;
    vector<double> electronEtaThresholds_;
    double electronPtThreshold_;
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

    double deltaRPhoElectronThreshold_;
    double Zmass_;
    double deltaMassElectronZThreshold_;
    double DeltaRbjetfwdjet_;
    double DeltaRtHchainfwdjet_;
    double MVAThreshold_tHqVsttHBDT_;
    double MVAThreshold_tHqVsttHDNN_;
    double MVAThreshold_tHqVsNonHiggsBkg_;

    bool hasGoodElec = false;
    bool hasVetoElec = false;
    bool hasGoodMuons = false;

    unique_ptr<TMVA::Reader> thqLeptonicMva_tHqVsttHBDT;
    unique_ptr<TMVA::Reader> thqLeptonicMva_tHqVsNonHiggsBkg;
    FileInPath MVAweight_tHqVsttHBDT_;
    FileInPath MVAweight_tHqVsNonHiggsBkg_;
    FileInPath tthVstHDNNfile_;
    std::vector<double> tthVstHDNN_global_mean_;
    std::vector<double> tthVstHDNN_global_stddev_;
    std::vector<double> tthVstHDNN_object_mean_;
    std::vector<double> tthVstHDNN_object_stddev_;

    string  MVAMethod_;
    vector<double> boundaries;
    bool use_MVAs_;
    bool use_tthVstHDNN_;
    bool use_tthVstHBDT_;

    float MVAscore_tHqVsttHBDT, topMass;
    float MVAscore_tHqVsNonHiggsBkg;
    std::vector< TLorentzVector > particles_LorentzVector;
    std::vector< math::RhoEtaPhiVector > particles_RhoEtaPhiVector;

    TLorentzVector metL, metW_check, bL,fwdJL, G1, G2;  //temp solution: make met, bjet & jprime global TLorentzVectors
//Variables for TMVA and Likelihood
    edm::Ptr<flashgg::Jet> fwdJet1;
    edm::Ptr<flashgg::Jet> bJet1;
    edm::Ptr<flashgg::Muon> muon1;
    edm::Ptr<flashgg::Electron> ele1; 
    float lepton_ch_;
    float top_mt11_;
    TLorentzVector l1, b1, l1b1, l1b1met, tHchain;
    float dipho_pt_ ;
    float dipho_leadPtOvermass_;
    float dipho_subleadPtOvermass_;
    float dipho_leadEta_;
    float dipho_subleadEta_;
    float dipho_leadIDMVA_;
    float dipho_subleadIDMVA_;
    float dipho_lead_haspixelseed_;
    float dipho_sublead_haspixelseed_;
    float n_jets_;
    float n_bjets_;
    float n_centraljets_;
    float dEta_leptonfwdjet_;

    float dRbjetfwdjet_ ;
    float dRtHchainfwdjet_ ;
    float dRleadphobjet_ ;
    float dRsubleadphobjet_ ;
    float dRleadphofwdjet_ ;
    float dRsubleadphofwdjet_ ;
    float dRleptonbjet_ ;
    float dRleptonfwdjet_ ;
    float bjet1_discr_;
    float bjet2_discr_;
    float bjet3_discr_;
    float bjet1_pt_;
    float bjet2_pt_;
    float bjet3_pt_;
    float bjet1_eta_;
    float bjet2_eta_;
    float bjet3_eta_;
    float jet1_pt_;
    float jet2_pt_;
    float jet3_pt_;
    float jet4_pt_;
    float jet1_eta_;
    float jet2_eta_;
    float jet3_eta_;
    float jet4_eta_;
    float jet1_discr_;
    float jet2_discr_;
    float jet3_discr_;
    float jet4_discr_;
    float fwdJet1_pt_;
    float fwdJet1_eta_;
    float fwdJet1_discr_;
    float lepton_leadPt_;
    float lepton_leadEta_;

    float minPhoID_;
    float maxPhoID_;
    float maxBTagVal_;
    float secondMaxBTagVal_;
    float MVAscore_tHqVsttHDNN;

    bool debug_=false;
    TTH_DNN_Helper* dnn_ttH_vs_tH;
    struct GreaterByPt
    {
    public:
        bool operator()( edm::Ptr<flashgg::Jet> lh, edm::Ptr<flashgg::Jet> rh ) const
        {
            return lh->pt() > rh->pt();
        };
    };

    struct GreaterByEta
    {
    public:
        bool operator()( edm::Ptr<flashgg::Jet> lh, edm::Ptr<flashgg::Jet> rh ) const
        {
            return fabs(lh->eta()) > fabs(rh->eta());
        };
    };

    struct GreaterByBTagging
    {
    public:
        GreaterByBTagging(std::string urName, std::string urName1):
            urName( urName ), urName1(urName1)
        {
        }

        bool operator()( edm::Ptr<flashgg::Jet> lh, edm::Ptr<flashgg::Jet> rh ) const
        {
            return (lh->bDiscriminator(urName.data()) + lh->bDiscriminator(urName1.data())) > (rh->bDiscriminator(urName.data()) + rh->bDiscriminator(urName1.data())) ;
        };
    private:
        const std::string urName, urName1;
//	const std::string urName1;
    };


    int LeptonType;
    std::vector<edm::Ptr<flashgg::Jet> > SelJetVect;
    std::vector<edm::Ptr<flashgg::Jet> > SelJetVect_EtaSorted;
    std::vector<edm::Ptr<flashgg::Jet> > SelJetVect_PtSorted;
    std::vector<edm::Ptr<flashgg::Jet> > SelJetVect_BSorted;
    std::vector<edm::Ptr<flashgg::Jet> > MediumBJetVect, MediumBJetVect_PtSorted, BJetVect;
    std::vector<edm::Ptr<flashgg::Jet> > LooseBJetVect, LooseBJetVect_PtSorted ;
    std::vector<edm::Ptr<flashgg::Jet> > TightBJetVect, TightBJetVect_PtSorted;
    std::vector<edm::Ptr<flashgg::Jet> > centraljet;
    std::vector<edm::Ptr<flashgg::Jet> > forwardjet;

    edm::Ptr<flashgg::Jet> fwdJet;
    edm::Ptr<flashgg::Jet> bJet  ;
    void topReco( std::vector<edm::Ptr<flashgg::Jet> >* bjets ) {
        topMass = -100.;

        if ( bjets->size() < 1 || SelJetVect.size() < 2 || LeptonType == 0) {
            return ;
        }
        fwdJet = SelJetVect_EtaSorted[0];
        bJet = bjets->at(0);
        if( fwdJet == bJet )
            fwdJet = SelJetVect_EtaSorted[1] ;


        bL.SetPtEtaPhiE( bJet->pt(), bJet->eta(), bJet->phi(), bJet->energy());
        fwdJL.SetPtEtaPhiE( fwdJet->pt(),fwdJet->eta(), fwdJet->phi(), fwdJet->energy());


        flashgg::SemiLepTopQuark singletop(bL, metL, lepton.LorentzVector(), fwdJL,fwdJL);
        n_jets = SelJetVect.size();
        metL = singletop.getMET() ;
        jprime_eta  = fabs( fwdJL.Eta() );
        met_pt = metL.Pt();
        metW_check = singletop.neutrino_W () ;
        topMass = singletop.top().M() ;
    };


    //MVA INPUTS
    float  n_jets = 0;
    float jprime_eta,met_pt;

    struct particleinfo {
        float pt, eta, phi , other , w , another; //other : for photon id, for diphoton mass, for jets btagging vals
        unsigned short number;
        bool isSet;
        TLorentzVector lorentzVector_;
        std::map<std::string,float> info;
        particleinfo( double pt_=-999, double eta_=-999, double phi_=-999 , double other_= -999 , double W= 1.0 ) {
            pt = pt_;
            eta = eta_;
            phi = phi_;
            other = other_;
            w = W;
            number = 255;
            isSet = false;
            lorentzVector_.SetPtEtaPhiM(pt,eta,phi,other_);
        };
        void set(double pt_=-999, double eta_=-999, double phi_=-999 , double other_= -999 , double W= 1.0 , double Another= -999 ) {
            pt = pt_;
            eta = eta_;
            phi = phi_;
            other = other_;
            w = W;
            another = Another;
            isSet = true;
            lorentzVector_.SetPtEtaPhiM(pt,eta,phi,0.);
        };
        TLorentzVector LorentzVector() {
            return lorentzVector_;
        };
        void SetLorentzVector(TLorentzVector lorentzVector) {
            lorentzVector_.SetPxPyPzE(lorentzVector.Px(),lorentzVector.Py(),lorentzVector.Pz(),lorentzVector.Energy());
        };
    };

    particleinfo lepton ,  eventshapes;
    particleinfo foxwolf1 ; // foxwolf2 , foxwolf1Met, foxwolf2Met ;
};

THQLeptonicTagProducer::THQLeptonicTagProducer( const ParameterSet &iConfig ) :
    processId_( iConfig.getParameter<string>("processId") ),
    diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
    inputTagJets_( iConfig.getParameter<std::vector<edm::InputTag> >( "inputTagJets" ) ),
    electronToken_( consumes<View<flashgg::Electron> >( iConfig.getParameter<InputTag>( "ElectronTag" ) ) ),
    muonToken_( consumes<View<flashgg::Muon> >( iConfig.getParameter<InputTag>( "MuonTag" ) ) ),
    mvaResultToken_( consumes<View<flashgg::DiPhotonMVAResult> >( iConfig.getParameter<InputTag> ( "MVAResultTag" ) ) ),
    vertexToken_( consumes<View<reco::Vertex> >( iConfig.getParameter<InputTag> ( "VertexTag" ) ) ),
    METToken_( consumes<View<flashgg::Met> >( iConfig.getParameter<InputTag> ( "METTag" ) ) ),
    genParticleToken_( consumes<View<reco::GenParticle> >( iConfig.getParameter<InputTag> ( "GenParticleTag" ) ) ),
    genJetToken_ ( consumes<View<reco::GenJet> >( iConfig.getParameter<InputTag> ( "GenJetTag" ) ) ),
    weightToken_( consumes<vector<flashgg::PDFWeightObject> >( iConfig.getUntrackedParameter<InputTag>( "WeightTag", InputTag( "flashggPDFWeightObject" ) ) ) ),
    rhoTag_( consumes<double>( iConfig.getParameter<InputTag>( "rhoTag" ) ) ),
    systLabel_( iConfig.getParameter<string> ( "SystLabel" ) ),
    MVAMethod_    ( iConfig.getParameter<string> ( "MVAMethod"    ) )
{

/*ps    if(processId_.find("thq") != std::string::npos or processId_.find("thw") != std::string::npos) {
        token_lhe = consumes<LHEEventProduct>( InputTag( "externalLHEProducer" )  );
    }
*/
    double default_Zmass_ = 91.9;
    double default_deltaMassElectronZThreshold_ = 5.;


    vector<double> default_electronEtaCuts_;
    muonEtaThreshold_ = iConfig.getParameter<double>( "muonEtaThreshold" );
    muonPtThreshold_ = iConfig.getParameter<double>( "muonPtThreshold" );
    electronEtaThresholds_ = iConfig.getParameter<vector<double > >( "electronEtaThresholds");
    electronPtThreshold_ = iConfig.getParameter<double>( "electronPtThreshold" );
    leadPhoOverMassThreshold_ = iConfig.getParameter<double>( "leadPhoOverMassThreshold" );
    subleadPhoOverMassThreshold_ = iConfig.getParameter<double>( "subleadPhoOverMassThreshold" );
    MVAThreshold_ = iConfig.getParameter<double>( "MVAThreshold" );
    deltaRLepPhoThreshold_ = iConfig.getParameter<double>( "deltaRLepPhoThreshold" );
    deltaRJetLepThreshold_ = iConfig.getParameter<double>( "deltaRJetLepThreshold" );
    jetsNumberThreshold_ = iConfig.getParameter<double>( "jetsNumberThreshold" );
    bjetsNumberThreshold_ = iConfig.getParameter<double>( "bjetsNumberThreshold" );
    jetPtThreshold_ = iConfig.getParameter<double>( "jetPtThreshold" );
    jetEtaThreshold_ = iConfig.getParameter<double>( "jetEtaThreshold" );

    deltaRJetLeadPhoThreshold_ = iConfig.getParameter<double>( "deltaRJetLeadPhoThreshold" );
    deltaRJetSubLeadPhoThreshold_ = iConfig.getParameter<double>( "deltaRJetSubLeadPhoThreshold" );

    bDiscriminator_ = iConfig.getParameter<vector<double > >( "bDiscriminator" );
    bTag_ = iConfig.getParameter<string>( "bTag" );

    muPFIsoSumRelThreshold_ = iConfig.getParameter<double>( "muPFIsoSumRelThreshold" );
    PhoMVAThreshold_ = iConfig.getParameter<double>( "PhoMVAThreshold" );
    DeltaRTrkElec_ = iConfig.getParameter<double>( "DeltaRTrkElec" );

    deltaRPhoElectronThreshold_ = iConfig.getParameter<double>( "deltaRPhoElectronThreshold" );
    Zmass_ = iConfig.getUntrackedParameter<double>( "Zmass_", default_Zmass_ );
    deltaMassElectronZThreshold_ = iConfig.getUntrackedParameter<double>( "deltaMassElectronZThreshold_", default_deltaMassElectronZThreshold_ );
    DeltaRbjetfwdjet_ = iConfig.getParameter<double>( "DeltaRbjetfwdjet" );
    DeltaRtHchainfwdjet_ = iConfig.getParameter<double>( "DeltaRtHchainfwdjet" );
    MVAThreshold_tHqVsttHBDT_ = iConfig.getParameter<double>( "MVAThreshold_tHqVsttHBDT" );
    MVAThreshold_tHqVsttHDNN_ = iConfig.getParameter<double>( "MVAThreshold_tHqVsttHDNN" );
    MVAThreshold_tHqVsNonHiggsBkg_ = iConfig.getParameter<double>( "MVAThreshold_tHqVsNonHiggsBkg" );
    MVAweight_tHqVsttHBDT_ = iConfig.getParameter<edm::FileInPath>( "MVAweight_tHqVsttHBDT" );
    MVAweight_tHqVsNonHiggsBkg_ = iConfig.getParameter<edm::FileInPath>( "MVAweight_tHqVsNonHiggsBkg");
    tthVstHDNNfile_ = iConfig.getParameter<edm::FileInPath>( "tthVstHDNNfile" );
    tthVstHDNN_global_mean_ = iConfig.getParameter<std::vector<double>>( "tthVstHDNN_global_mean" );
    tthVstHDNN_global_stddev_ = iConfig.getParameter<std::vector<double>>( "tthVstHDNN_global_stddev" );
    tthVstHDNN_object_mean_ = iConfig.getParameter<std::vector<double>>( "tthVstHDNN_object_mean" );
    tthVstHDNN_object_stddev_ = iConfig.getParameter<std::vector<double>>( "tthVstHDNN_object_stddev" );
            
    boundaries = iConfig.getParameter<vector<double > >( "Boundaries" );
    use_MVAs_ = iConfig.getParameter<bool> ( "use_MVAs" );
    use_tthVstHDNN_ = iConfig.getParameter<bool> ( "use_tthVstHDNN" );
    use_tthVstHBDT_ = iConfig.getParameter<bool> ( "use_tthVstHBDT" );
        thqLeptonicMva_tHqVsttHBDT.reset( new TMVA::Reader( "!Color:Silent" ) );

        thqLeptonicMva_tHqVsttHBDT->AddVariable( "dipho_leadPt/dipho_mass", &dipho_leadPtOvermass_ );
        thqLeptonicMva_tHqVsttHBDT->AddVariable( "dipho_subleadPt/dipho_mass", &dipho_subleadPtOvermass_ );
        thqLeptonicMva_tHqVsttHBDT->AddVariable( "dipho_leadEta", &dipho_leadEta_ );
        thqLeptonicMva_tHqVsttHBDT->AddVariable( "dipho_subleadEta", &dipho_subleadEta_ );
        thqLeptonicMva_tHqVsttHBDT->AddVariable( "dipho_leadIDMVA", &dipho_leadIDMVA_ );
        thqLeptonicMva_tHqVsttHBDT->AddVariable( "dipho_subleadIDMVA", &dipho_subleadIDMVA_ );
        thqLeptonicMva_tHqVsttHBDT->AddVariable( "dipho_lead_haspixelseed", &dipho_lead_haspixelseed_ );
        thqLeptonicMva_tHqVsttHBDT->AddVariable( "dipho_sublead_haspixelseed", &dipho_sublead_haspixelseed_ );
        thqLeptonicMva_tHqVsttHBDT->AddVariable( "n_jets"  ,              &n_jets_);  
        thqLeptonicMva_tHqVsttHBDT->AddVariable( "n_bjets",               &n_bjets_ );  
        thqLeptonicMva_tHqVsttHBDT->AddVariable( "n_centraljets",         &n_centraljets_);
        thqLeptonicMva_tHqVsttHBDT->AddVariable( "lepton_charge",         &lepton_ch_);
        thqLeptonicMva_tHqVsttHBDT->AddVariable( "lepton_leadPt",         &lepton_leadPt_);
        thqLeptonicMva_tHqVsttHBDT->AddVariable( "lepton_leadEta",         &lepton_leadEta_);
        thqLeptonicMva_tHqVsttHBDT->AddVariable( "fwdjet1_pt",           &fwdJet1_pt_);
        thqLeptonicMva_tHqVsttHBDT->AddVariable( "fwdjet1_eta",           &fwdJet1_eta_);
        thqLeptonicMva_tHqVsttHBDT->AddVariable( "fwdjet1_discr",           &fwdJet1_discr_);
	    thqLeptonicMva_tHqVsttHBDT->AddVariable( "top_mt",                 &top_mt11_  );
        thqLeptonicMva_tHqVsttHBDT->AddVariable( "dr_tHchainfwdjet",       &dRtHchainfwdjet_  );
        thqLeptonicMva_tHqVsttHBDT->AddVariable( "dr_leptonbjet",          &dRleptonbjet_  );
        thqLeptonicMva_tHqVsttHBDT->AddVariable( "dr_leptonfwdjet",        &dRleptonfwdjet_  );
	    thqLeptonicMva_tHqVsttHBDT->AddVariable( "dr_bjetfwdjet", 	  &dRbjetfwdjet_);
	    thqLeptonicMva_tHqVsttHBDT->AddVariable( "dr_leadphofwdjet",       &dRleadphofwdjet_  );
	    thqLeptonicMva_tHqVsttHBDT->AddVariable( "dr_subleadphofwdjet" ,   &dRsubleadphofwdjet_);
        thqLeptonicMva_tHqVsttHBDT->AddVariable( "bjet1_pt",              &bjet1_pt_);
        thqLeptonicMva_tHqVsttHBDT->AddVariable( "bjet2_pt",              &bjet2_pt_);
        thqLeptonicMva_tHqVsttHBDT->AddVariable( "bjet3_pt",              &bjet3_pt_);
        thqLeptonicMva_tHqVsttHBDT->AddVariable( "bjet1_eta",              &bjet1_eta_);
        thqLeptonicMva_tHqVsttHBDT->AddVariable( "bjet2_eta",              &bjet2_eta_);
        thqLeptonicMva_tHqVsttHBDT->AddVariable( "bjet3_eta",              &bjet3_eta_);
	    thqLeptonicMva_tHqVsttHBDT->AddVariable( "bjet1_discr",            &bjet1_discr_);
	    thqLeptonicMva_tHqVsttHBDT->AddVariable( "bjet2_discr",            &bjet2_discr_);
        thqLeptonicMva_tHqVsttHBDT->AddVariable( "bjet3_discr",            &bjet3_discr_);
	    thqLeptonicMva_tHqVsttHBDT->AddVariable( "jet1_pt",                &jet1_pt_);
        thqLeptonicMva_tHqVsttHBDT->AddVariable( "jet2_pt",                &jet2_pt_);
        thqLeptonicMva_tHqVsttHBDT->AddVariable( "jet3_pt",                &jet3_pt_);
        thqLeptonicMva_tHqVsttHBDT->AddVariable( "jet1_eta",               &jet1_eta_);
        thqLeptonicMva_tHqVsttHBDT->AddVariable( "jet2_eta",               &jet2_eta_);
        thqLeptonicMva_tHqVsttHBDT->AddVariable( "jet3_eta",               &jet3_eta_);
        thqLeptonicMva_tHqVsttHBDT->AddVariable( "jet1_discr",               &jet1_discr_);
        thqLeptonicMva_tHqVsttHBDT->AddVariable( "jet2_discr",               &jet2_discr_);
        thqLeptonicMva_tHqVsttHBDT->AddVariable( "jet3_discr",               &jet3_discr_);

        thqLeptonicMva_tHqVsttHBDT->BookMVA( MVAMethod_.c_str() , MVAweight_tHqVsttHBDT_.fullPath() );

        thqLeptonicMva_tHqVsNonHiggsBkg.reset( new TMVA::Reader( "!Color:Silent" ) );

        thqLeptonicMva_tHqVsNonHiggsBkg->AddVariable( "dipho_leadPt/dipho_mass", &dipho_leadPtOvermass_ );
        thqLeptonicMva_tHqVsNonHiggsBkg->AddVariable( "dipho_subleadPt/dipho_mass", &dipho_subleadPtOvermass_ );
        thqLeptonicMva_tHqVsNonHiggsBkg->AddVariable( "dipho_leadEta", &dipho_leadEta_ );
        thqLeptonicMva_tHqVsNonHiggsBkg->AddVariable( "dipho_subleadEta", &dipho_subleadEta_ );
        thqLeptonicMva_tHqVsNonHiggsBkg->AddVariable( "dipho_leadIDMVA", &dipho_leadIDMVA_ );
        thqLeptonicMva_tHqVsNonHiggsBkg->AddVariable( "dipho_subleadIDMVA", &dipho_subleadIDMVA_ );
        thqLeptonicMva_tHqVsNonHiggsBkg->AddVariable( "dipho_lead_haspixelseed", &dipho_lead_haspixelseed_ );
        thqLeptonicMva_tHqVsNonHiggsBkg->AddVariable( "dipho_sublead_haspixelseed", &dipho_sublead_haspixelseed_ );
        thqLeptonicMva_tHqVsNonHiggsBkg->AddVariable( "n_jets"  ,              &n_jets_);
        thqLeptonicMva_tHqVsNonHiggsBkg->AddVariable( "n_bjets",               &n_bjets_ );
        thqLeptonicMva_tHqVsNonHiggsBkg->AddVariable( "n_centraljets",         &n_centraljets_);
        thqLeptonicMva_tHqVsNonHiggsBkg->AddVariable( "lepton_charge",         &lepton_ch_);
        thqLeptonicMva_tHqVsNonHiggsBkg->AddVariable( "lepton_leadPt",         &lepton_leadPt_);
        thqLeptonicMva_tHqVsNonHiggsBkg->AddVariable( "lepton_leadEta",         &lepton_leadEta_);
        thqLeptonicMva_tHqVsNonHiggsBkg->AddVariable( "fwdjet1_pt",           &fwdJet1_pt_);
        thqLeptonicMva_tHqVsNonHiggsBkg->AddVariable( "fwdjet1_eta",           &fwdJet1_eta_);
        thqLeptonicMva_tHqVsNonHiggsBkg->AddVariable( "fwdjet1_discr",           &fwdJet1_discr_);
        thqLeptonicMva_tHqVsNonHiggsBkg->AddVariable( "top_mt",                 &top_mt11_  );
        thqLeptonicMva_tHqVsNonHiggsBkg->AddVariable( "dr_tHchainfwdjet",       &dRtHchainfwdjet_  );
        thqLeptonicMva_tHqVsNonHiggsBkg->AddVariable( "dr_leptonbjet",          &dRleptonbjet_  );
        thqLeptonicMva_tHqVsNonHiggsBkg->AddVariable( "dr_leptonfwdjet",        &dRleptonfwdjet_  );
        thqLeptonicMva_tHqVsNonHiggsBkg->AddVariable( "dr_bjetfwdjet",    &dRbjetfwdjet_);
        thqLeptonicMva_tHqVsNonHiggsBkg->AddVariable( "dr_leadphofwdjet",       &dRleadphofwdjet_  );
        thqLeptonicMva_tHqVsNonHiggsBkg->AddVariable( "dr_subleadphofwdjet" ,   &dRsubleadphofwdjet_);
        thqLeptonicMva_tHqVsNonHiggsBkg->AddVariable( "bjet1_pt",              &bjet1_pt_);
        thqLeptonicMva_tHqVsNonHiggsBkg->AddVariable( "bjet2_pt",              &bjet2_pt_);
        thqLeptonicMva_tHqVsNonHiggsBkg->AddVariable( "bjet3_pt",              &bjet3_pt_);
        thqLeptonicMva_tHqVsNonHiggsBkg->AddVariable( "bjet1_eta",              &bjet1_eta_);
        thqLeptonicMva_tHqVsNonHiggsBkg->AddVariable( "bjet2_eta",              &bjet2_eta_);
        thqLeptonicMva_tHqVsNonHiggsBkg->AddVariable( "bjet3_eta",              &bjet3_eta_);
        thqLeptonicMva_tHqVsNonHiggsBkg->AddVariable( "bjet1_discr",            &bjet1_discr_);
        thqLeptonicMva_tHqVsNonHiggsBkg->AddVariable( "bjet2_discr",            &bjet2_discr_);
        thqLeptonicMva_tHqVsNonHiggsBkg->AddVariable( "bjet3_discr",            &bjet3_discr_);
        thqLeptonicMva_tHqVsNonHiggsBkg->AddVariable( "jet1_pt",                &jet1_pt_);
        thqLeptonicMva_tHqVsNonHiggsBkg->AddVariable( "jet2_pt",                &jet2_pt_);
        thqLeptonicMva_tHqVsNonHiggsBkg->AddVariable( "jet3_pt",                &jet3_pt_);
        thqLeptonicMva_tHqVsNonHiggsBkg->AddVariable( "jet1_eta",               &jet1_eta_);
        thqLeptonicMva_tHqVsNonHiggsBkg->AddVariable( "jet2_eta",               &jet2_eta_);
        thqLeptonicMva_tHqVsNonHiggsBkg->AddVariable( "jet3_eta",               &jet3_eta_);
        thqLeptonicMva_tHqVsNonHiggsBkg->AddVariable( "jet1_discr",               &jet1_discr_);
        thqLeptonicMva_tHqVsNonHiggsBkg->AddVariable( "jet2_discr",               &jet2_discr_);
        thqLeptonicMva_tHqVsNonHiggsBkg->AddVariable( "jet3_discr",               &jet3_discr_);

        thqLeptonicMva_tHqVsNonHiggsBkg->BookMVA( MVAMethod_.c_str() , MVAweight_tHqVsNonHiggsBkg_.fullPath() );

        dnn_ttH_vs_tH = new TTH_DNN_Helper(tthVstHDNNfile_.fullPath());
        dnn_ttH_vs_tH->SetInputShapes(23, 9, 8);
        dnn_ttH_vs_tH->SetPreprocessingSchemes(tthVstHDNN_global_mean_, tthVstHDNN_global_stddev_, tthVstHDNN_object_mean_, tthVstHDNN_object_stddev_);

    for (unsigned i = 0 ; i < inputTagJets_.size() ; i++) {
        auto token = consumes<View<flashgg::Jet> >(inputTagJets_[i]);
        tokenJets_.push_back(token);
    }
    produces<vector<THQLeptonicTag> >();
    //produces<vector<THQLeptonicTagTruth> >();
}

THQLeptonicTagProducer::~THQLeptonicTagProducer() {
}

int THQLeptonicTagProducer::chooseCategory( float mvavalue )
    {
        for(int n = 0 ; n < ( int )boundaries.size() ; n++ ) {
            if( ( double )mvavalue > boundaries[boundaries.size() - n - 1] ) { return n; }
        }
        return -1;
    }

/*int THQLeptonicTagProducer::chooseCategory( float mvavalue1, float mvavalue2 )
    {
    int n;
        for( n = 0 ; n < ( int )boundaries.size() ; n++ ) {
            if( ( double )mvavalue1 > boundaries[boundaries.size() - n - 1] && ( double )mvavalue2 > boundaries[boundaries.size() - n - 1] ) { return n; }
        }
        return -1;
     }
*/
void THQLeptonicTagProducer::produce( Event &evt, const EventSetup & )

{
    JetCollectionVector Jets( inputTagJets_.size() );
    for( size_t j = 0; j < inputTagJets_.size(); ++j ) {
        evt.getByToken( tokenJets_[j], Jets[j] );
    }

    edm::Handle<double>  rho;
    evt.getByToken(rhoTag_,rho);
    float rho_    = *rho;

    Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
    evt.getByToken( diPhotonToken_, diPhotons );

    Handle<View<flashgg::Muon> > theMuons;
    evt.getByToken( muonToken_, theMuons );

    Handle<View<flashgg::Electron> > theElectrons;
    evt.getByToken( electronToken_, theElectrons );

    Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;
    evt.getByToken( mvaResultToken_, mvaResults );

    std::unique_ptr<vector<THQLeptonicTag> > thqltags( new vector<THQLeptonicTag> );

    Handle<View<reco::Vertex> > vertices;
    evt.getByToken( vertexToken_, vertices );

    Handle<View<flashgg::Met> > METs;
    evt.getByToken( METToken_, METs );

    Handle<View<reco::GenParticle> > genParticles;
    Handle<View<reco::GenJet> > genJets;

    //std::unique_ptr<vector<THQLeptonicTagTruth> > truths( new vector<THQLeptonicTagTruth> );
/*ps    edm::Handle<LHEEventProduct> product_lhe;
    vector< double > CtCvWeights ;
    if( processId_.find("thq") != std::string::npos or processId_.find("thw") != std::string::npos ) {
        evt.getByToken(token_lhe, product_lhe);
        for (uint i = 446 ; i < product_lhe->weights().size() ; i++)
            CtCvWeights.push_back(product_lhe->weights()[i].wgt/product_lhe->originalXWGTUP () );
    }
*/

    //edm::RefProd<vector<THQLeptonicTagTruth> > rTagTruth = evt.getRefBeforePut<vector<THQLeptonicTagTruth> >();
    //unsigned int idx = 0;


    assert( diPhotons->size() == mvaResults->size() );

    bool photonSelection = false;
    double idmva1 = 0.;
    double idmva2 = 0.;
    for( unsigned int diphoIndex = 0; diphoIndex < diPhotons->size(); diphoIndex++ ) {

        hasGoodElec = false;
        hasVetoElec = false;
        hasGoodMuons = false;

        unsigned int jetCollectionIndex = diPhotons->ptrAt( diphoIndex )->jetCollectionIndex();

        edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotons->ptrAt( diphoIndex );
        edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResults->ptrAt( diphoIndex );


        flashgg::THQLeptonicTag thqltags_obj( dipho, mvares );

        // if(processId_.find("thq") != std::string::npos or processId_.find("thw") != std::string::npos)
        // 	CTCVWeightedVariables["photon1pt"]->Fill( dipho->leadingPhoton()->pt() , CtCvWeights );
        if( dipho->leadingPhoton()->pt() < ( dipho->mass() )*leadPhoOverMassThreshold_ ) {
            continue;
        }
        // if(processId_.find("thq") != std::string::npos or processId_.find("thw") != std::string::npos)
        // 	CTCVWeightedVariables["photon2pt"]->Fill( dipho->subLeadingPhoton()->pt() , CtCvWeights );

        if( dipho->subLeadingPhoton()->pt() < ( dipho->mass() )*subleadPhoOverMassThreshold_ ) {
            continue;
        }
        idmva1 = dipho->leadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );
        idmva2 = dipho->subLeadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );
        if( idmva1 < PhoMVAThreshold_ || idmva2 < PhoMVAThreshold_ ) {
            continue;
        }

        // if(processId_.find("thq") != std::string::npos or processId_.find("thw") != std::string::npos){
        // 	CTCVWeightedVariables["diPhotonMVA"]->Fill( mvares->result , CtCvWeights );

        // 	CTCVWeightedVariables["diPhotonPt"]->Fill( dipho->pt() , CtCvWeights );
        // 	CTCVWeightedVariables["diPhotonEta"]->Fill( abs( dipho->eta() ) , CtCvWeights );
        // }

/*        if( mvares->result < MVAThreshold_ ) {            //DiPho_MVA
            continue;
        }
*/
        photonSelection = true;


        G1.SetPtEtaPhiM( diPhotons->ptrAt( diphoIndex )->leadingPhoton()->pt(),
                         diPhotons->ptrAt( diphoIndex )->leadingPhoton()->eta(),
                         diPhotons->ptrAt( diphoIndex )->leadingPhoton()->phi() ,
                         0 );
        particles_LorentzVector.push_back( G1 );

        G2.SetPtEtaPhiM( diPhotons->ptrAt( diphoIndex )->subLeadingPhoton()->pt(),
                         diPhotons->ptrAt( diphoIndex )->subLeadingPhoton()->eta(),
                         diPhotons->ptrAt( diphoIndex )->subLeadingPhoton()->phi(),
                         0 );
        particles_LorentzVector.push_back(G2);

        particles_RhoEtaPhiVector.push_back( math::RhoEtaPhiVector(G1.Pt(), G1.Eta() , G1.Phi() ) );
        particles_RhoEtaPhiVector.push_back( math::RhoEtaPhiVector(G2.Pt(), G2.Eta() , G2.Phi() ) );

        if( METs->size() != 1 ) {
            std::cout << "WARNING - #MET is not 1" << std::endl;
        }
        Ptr<flashgg::Met> theMET = METs->ptrAt( 0 );
        thqltags_obj.setRECOMET(theMET);

        // if(processId_.find("thq") != std::string::npos or processId_.find("thw") != std::string::npos)
        // 	CTCVWeightedVariables["MET"]->Fill( theMET->getCorPt() , CtCvWeights );

        //const pat::MET &met_ = METs->front();
        metL.SetPtEtaPhiE( theMET->getCorPt(),
                           theMET->eta(),
                           theMET->getCorPhi(),
                           theMET->energy()
                         ) ;

        std::vector<edm::Ptr<flashgg::Muon> > LooseMu15 = selectMuons( theMuons->ptrs(), dipho, vertices->ptrs(), muonEtaThreshold_ , muonPtThreshold_,
                0.15 , deltaRLepPhoThreshold_, deltaRLepPhoThreshold_ );
        std::vector<edm::Ptr<flashgg::Muon> > LooseMu25 = selectMuons( theMuons->ptrs(), dipho, vertices->ptrs(), muonEtaThreshold_ , muonPtThreshold_,
                0.25 , deltaRLepPhoThreshold_, deltaRLepPhoThreshold_ );

        std::vector<edm::Ptr<flashgg::Muon> > LooseMu200 = selectMuons( theMuons->ptrs(), dipho, vertices->ptrs(), muonEtaThreshold_ , muonPtThreshold_,
                2. , deltaRLepPhoThreshold_, deltaRLepPhoThreshold_);


        std::vector<edm::Ptr<flashgg::Muon> > MediumMu15 = selectMuons( theMuons->ptrs(), dipho, vertices->ptrs(), muonEtaThreshold_ , muonPtThreshold_,
                0.15 , deltaRLepPhoThreshold_, deltaRLepPhoThreshold_ );
        std::vector<edm::Ptr<flashgg::Muon> > MediumMu25 = selectMuons( theMuons->ptrs(), dipho, vertices->ptrs(), muonEtaThreshold_ , muonPtThreshold_,
                0.25 , deltaRLepPhoThreshold_, deltaRLepPhoThreshold_ );

        std::vector<edm::Ptr<flashgg::Muon> > TightMuo15 = selectMuons( theMuons->ptrs(), dipho, vertices->ptrs(), muonEtaThreshold_ , muonPtThreshold_,
                0.15 , deltaRLepPhoThreshold_, deltaRLepPhoThreshold_);
        std::vector<edm::Ptr<flashgg::Muon> > TightMuo25 = selectMuons( theMuons->ptrs(), dipho, vertices->ptrs(), muonEtaThreshold_ , muonPtThreshold_,
                0.25 , deltaRLepPhoThreshold_, deltaRLepPhoThreshold_);

        std::vector<edm::Ptr<flashgg::Muon> > goodMuons = muPFIsoSumRelThreshold_== 0.15 ? TightMuo15 : TightMuo25 ;


        std::vector<int> looseMus_PassTight;
        for(auto mu: LooseMu200)
            looseMus_PassTight.push_back( std::find( goodMuons.begin() , goodMuons.end() , mu ) != goodMuons.end() );




        std::vector<edm::Ptr<Electron> > vetoNonIsoElectrons = selectStdElectrons(theElectrons->ptrs(), dipho, vertices->ptrs(), electronPtThreshold_,  electronEtaThresholds_ ,
                0,4,
                deltaRPhoElectronThreshold_,DeltaRTrkElec_,deltaMassElectronZThreshold_ , rho_, true ); //evt.isRealData()

        std::vector<edm::Ptr<Electron> > looseElectrons = selectStdElectrons(theElectrons->ptrs(), dipho, vertices->ptrs(), electronPtThreshold_,  electronEtaThresholds_ ,
                0,3,
                deltaRPhoElectronThreshold_,DeltaRTrkElec_,deltaMassElectronZThreshold_ , rho_, true ); //evt.isRealData()


        std::vector<edm::Ptr<Electron> > vetoElectrons = selectStdElectrons(theElectrons->ptrs(), dipho, vertices->ptrs(), electronPtThreshold_,  electronEtaThresholds_ ,
                0,0,
                deltaRPhoElectronThreshold_,DeltaRTrkElec_,deltaMassElectronZThreshold_ , rho_, true ); //evt.isRealData()

        std::vector<edm::Ptr<Electron> > mediumElectrons = selectStdElectrons(theElectrons->ptrs(), dipho, vertices->ptrs(), electronPtThreshold_,  electronEtaThresholds_ ,
                0,2,
                deltaRPhoElectronThreshold_,DeltaRTrkElec_,deltaMassElectronZThreshold_ , rho_, true ); //evt.isRealData()

        std::vector<edm::Ptr<Electron> > goodElectrons = selectStdElectrons(theElectrons->ptrs(), dipho, vertices->ptrs(), electronPtThreshold_,  electronEtaThresholds_ ,
                0,1,
                deltaRPhoElectronThreshold_,DeltaRTrkElec_,deltaMassElectronZThreshold_ , rho_, true);

        std::vector<int> vetoNonIsoElectrons_PassTight;
        std::vector<int> vetoNonIsoElectrons_PassVeto;
        for(auto ele : vetoNonIsoElectrons ) {
            vetoNonIsoElectrons_PassTight.push_back( std::find( goodElectrons.begin() , goodElectrons.end() , ele ) != goodElectrons.end() );
            vetoNonIsoElectrons_PassVeto.push_back( std::find( vetoElectrons.begin() , vetoElectrons.end() , ele ) != vetoElectrons.end() );
        }
//----------------------
        if(goodMuons.size()>=2)
        {
            std::vector<edm::Ptr<flashgg::Muon>> Muons_0;
            Muons_0 = goodMuons;
            std::vector<int> badIndexes;

            for(unsigned int i=0; i<Muons_0.size(); ++i)
            {
                for(unsigned int j=i+1; j<Muons_0.size(); ++j)
                {
                    TLorentzVector l1, l2;
                    l1.SetPtEtaPhiE(Muons_0[i]->pt(), Muons_0[i]->eta(), Muons_0[i]->phi(), Muons_0[i]->energy());
                    l2.SetPtEtaPhiE(Muons_0[j]->pt(), Muons_0[j]->eta(), Muons_0[j]->phi(), Muons_0[j]->energy());

                    if(fabs((l1+l2).M() - Zmass_) < deltaMassElectronZThreshold_)
                    {
                        badIndexes.push_back(i);
                        badIndexes.push_back(j);
                    }
                }
            }

            if(badIndexes.size()!=0)
            {
                goodMuons.clear();
                for(unsigned int i=0; i<Muons_0.size(); ++i)
                {
                    bool isBad = false;
                    for(unsigned int j=0; j<badIndexes.size(); ++j)
                    {
                        if(badIndexes[j]==(int)i)
                            isBad = true;
                    }
                    if(!isBad) goodMuons.push_back(Muons_0[i]);
                }
            }
        }

        if(goodElectrons.size()>=2)
        {
            std::vector<int> badIndexes;
            std::vector<edm::Ptr<flashgg::Electron> > Electrons_0;
            Electrons_0 = goodElectrons;
            for(unsigned int i=0; i<Electrons_0.size(); ++i)
            {
                for(unsigned int j=i+1; j<Electrons_0.size(); ++j)
                {
                    TLorentzVector l1, l2;
                    l1.SetPtEtaPhiE(Electrons_0[i]->pt(), Electrons_0[i]->eta(), Electrons_0[i]->phi(), Electrons_0[i]->energy());
                    l2.SetPtEtaPhiE(Electrons_0[j]->pt(), Electrons_0[j]->eta(), Electrons_0[j]->phi(), Electrons_0[j]->energy());

                    if(fabs((l1+l2).M() - Zmass_) < deltaMassElectronZThreshold_)
                    {
                        badIndexes.push_back(i);
                        badIndexes.push_back(j);
                    }
                }
            }
            if(badIndexes.size()!=0)
            {
                goodElectrons.clear();

                for(unsigned int i=0; i<Electrons_0.size(); ++i)
                {
                    bool isBad = false;
                    for(unsigned int j=0; j<badIndexes.size(); ++j)
                    {
                        if(badIndexes[j]==(int)i)
                            isBad = true;
                    }
                    if(!isBad) goodElectrons.push_back(Electrons_0[i]);
                }
            }
        }

//------------------------------------------
        if((goodElectrons.size() + goodMuons.size()) < 1) {
            continue;
        }
        hasGoodElec = ( goodElectrons.size() == 1 );
        hasVetoElec = ( vetoElectrons.size() > 0 );
        hasGoodMuons = ( goodMuons.size() == 1 );


        LeptonType = 0; //1 : electron, 2:muon


        if( hasGoodMuons && !hasVetoElec) {
            LeptonType = 2;
        }
        for( unsigned int muonIndex = 0; muonIndex < goodMuons.size(); muonIndex++ ) {

            Ptr<flashgg::Muon> muon = goodMuons[muonIndex];

            thqltags_obj.includeWeights( *goodMuons[muonIndex] );

            lepton.set( muon->pt(),
                        muon->eta() ,
                        muon->phi() ,
                        muon->energy(),
                        1. ,
                        muon->charge() );
            particles_LorentzVector.push_back(lepton.LorentzVector());
            particles_RhoEtaPhiVector.push_back( math::RhoEtaPhiVector( lepton.pt, lepton.eta, lepton.phi ) );

        }//end of muons loop


        if( hasGoodElec && !hasGoodMuons) {
            LeptonType = 1;
        }

        for( unsigned int ElectronIndex = 0; ElectronIndex < goodElectrons.size(); ElectronIndex++ ) {

            thqltags_obj.includeWeights( *goodElectrons[ElectronIndex] );

            Ptr<Electron> Electron = goodElectrons[ElectronIndex];
            lepton.set( Electron->pt(),
                        Electron->eta() ,
                        Electron->phi() ,
                        Electron->energy(),
                        1. ,
                        Electron->charge() );
            particles_LorentzVector.push_back(lepton.LorentzVector());
            particles_RhoEtaPhiVector.push_back( math::RhoEtaPhiVector( lepton.pt, lepton.eta, lepton.phi ) );


        }//end of electron loop

//--------------------------------------------------------------------------------------------------
                int leadMuIndex = 0;
                float leadMuPt = -1;
                int leadEleIndex = 0;
                float leadElePt = -1;

                for( unsigned int muonIndex = 0; muonIndex < goodMuons.size(); muonIndex++ )
                {
                    Ptr<flashgg::Muon> muon = goodMuons[muonIndex];

                    if(muon->pt()>leadMuPt)
                    {
                        leadMuPt = muon->pt();
                        leadMuIndex = muonIndex;
                    }
                }

                for( unsigned int eleIndex = 0; eleIndex < goodElectrons.size(); eleIndex++ )
                {
                    Ptr<flashgg::Electron> ele = goodElectrons[eleIndex];

                    if(ele->pt()>leadElePt)
                    {
                        leadElePt = ele->pt();
                        leadEleIndex = eleIndex;
                    }
                }

                if(leadMuPt>=leadElePt)
                {
                    lepton_leadPt_ = goodMuons[leadMuIndex]->pt();
                    lepton_leadEta_ = goodMuons[leadMuIndex]->eta();
                    lepton_ch_ = goodMuons[leadMuIndex]->charge();
                }
                else
                {
                    lepton_leadPt_ = goodElectrons[leadEleIndex]->pt();
                    lepton_leadEta_ = goodElectrons[leadEleIndex]->eta();
                    lepton_ch_ = goodElectrons[leadEleIndex]->charge();
                }
//--------------------------------------------------------------------------------------------------
        // if(processId_.find("thq") != std::string::npos or processId_.find("thw") != std::string::npos){
        // 	CTCVWeightedVariables["LeptonPt"]->Fill( lepton.pt , CtCvWeights );
        // 	CTCVWeightedVariables["LeptonEta"]->Fill( abs(lepton.eta) , CtCvWeights );
        // }

        float ht=0;
        float dRPhoLeadJet=0;
        float dRPhoSubLeadJet=0;
        double minDrLepton = 999.;
        int njets_btagloose_ = 0;
        int njets_btagmedium_ = 0;
        int njets_btagtight_ = 0;
	std::vector<float> bDiscr_bjets;
    std::vector<float> bDiscr_jets;
    std::vector<float> bDiscr_fwdjets;
        for( unsigned int candIndex_outer = 0; candIndex_outer < Jets[jetCollectionIndex]->size() ; candIndex_outer++ ) {
            edm::Ptr<flashgg::Jet> thejet = Jets[jetCollectionIndex]->ptrAt( candIndex_outer );
            std::vector <float> minDrLepton_ele;
            std::vector <float> minDrLepton_muon;


            if( !thejet->passesPuJetId( dipho ) ) {
                continue;
            }

            if( fabs( thejet->eta() ) > jetEtaThreshold_ ) {
                continue;
            }
            if( thejet->pt() < jetPtThreshold_ ) {
                continue;
            }
            dRPhoLeadJet = deltaR( thejet->eta(), thejet->phi(), dipho->leadingPhoton()->superCluster()->eta(), dipho->leadingPhoton()->superCluster()->phi() ) ;
            dRPhoSubLeadJet = deltaR( thejet->eta(), thejet->phi(), dipho->subLeadingPhoton()->superCluster()->eta(),dipho->subLeadingPhoton()->superCluster()->phi() );

            if( dRPhoLeadJet < deltaRJetLeadPhoThreshold_ || dRPhoSubLeadJet < deltaRJetSubLeadPhoThreshold_ ) {
                continue;
            }


            TLorentzVector jet_lorentzVector;
            jet_lorentzVector.SetPtEtaPhiE(  thejet->pt() , thejet->eta() , thejet->phi() , thejet->energy() );
            particles_LorentzVector.push_back( jet_lorentzVector );
            particles_RhoEtaPhiVector.push_back( math::RhoEtaPhiVector( thejet->pt(), thejet->eta(), thejet->phi() ) );

            int goodElectrons_count = 0;
            int goodMuons_count = 0;
            int rejectJet_wrt_muon = 1;
            int rejectJet_wrt_ele = 1;

//deltaR check with ele------------------------------------
            for(auto ele : goodElectrons) {
                float dRJetLepton_ele = deltaR( thejet->eta(), thejet->phi(), ele->eta() , ele->phi() );
                minDrLepton_ele.push_back(dRJetLepton_ele);
                goodElectrons_count++;
            }
            if(minDrLepton_ele.size() > 0) {
                for(unsigned int minDrLepton_ele_index=0; minDrLepton_ele_index < minDrLepton_ele.size() ; minDrLepton_ele_index++ ) {
                    if( minDrLepton_ele.at(minDrLepton_ele_index) < deltaRJetLepThreshold_) {
                        rejectJet_wrt_ele = 0;
                    }
                    rejectJet_wrt_ele = rejectJet_wrt_ele * rejectJet_wrt_ele ;
                }
            }
            if( rejectJet_wrt_ele == 0) {
                continue;
            }
            minDrLepton_ele.clear();
//deltaR check with muon---------------------------
            for(auto mu : goodMuons) {
                float dRJetLepton_muon = deltaR( thejet->eta(), thejet->phi(), mu->eta() , mu->phi() );
                minDrLepton_muon.push_back(dRJetLepton_muon);
                goodMuons_count++;
            }

            if(minDrLepton_muon.size() > 0) {
                for(unsigned int minDrLepton_muon_index=0; minDrLepton_muon_index < minDrLepton_muon.size() ; minDrLepton_muon_index++ ) {
                    if( minDrLepton_muon.at(minDrLepton_muon_index) < deltaRJetLepThreshold_) {
                        rejectJet_wrt_muon = 0;
                    }
                    rejectJet_wrt_muon = rejectJet_wrt_muon * rejectJet_wrt_muon ;
                }
            }
            if( rejectJet_wrt_muon == 0) {
                continue;
            }
            minDrLepton_muon.clear();


            float bDiscriminatorValue; //= -2.;
            if(bTag_ == "pfDeepCSV") bDiscriminatorValue = thejet->bDiscriminator("pfDeepCSVJetTags:probb") + thejet->bDiscriminator("pfDeepCSVJetTags:probbb");
            else
                bDiscriminatorValue = thejet->bDiscriminator( bTag_ );

            if( bDiscriminatorValue > bDiscriminator_[0] ) {
                LooseBJetVect_PtSorted.push_back( thejet );
                LooseBJetVect.push_back( thejet );
                njets_btagloose_++;
            }
            if( bDiscriminatorValue > bDiscriminator_[1] ) {
                MediumBJetVect.push_back( thejet );
                MediumBJetVect_PtSorted.push_back( thejet );
                njets_btagmedium_++;
            }

            if( bDiscriminatorValue > bDiscriminator_[2] ) {
                TightBJetVect_PtSorted.push_back( thejet );
                TightBJetVect.push_back( thejet );
                njets_btagtight_++;
            }

            ht+=thejet->pt();

            if( fabs( thejet->eta() ) < 1) {
                centraljet.push_back( thejet );
            }
            if( fabs( thejet->eta() ) > 1) {
                forwardjet.push_back( thejet );
            }


            SelJetVect.push_back( thejet );
            SelJetVect_EtaSorted.push_back( thejet );
            SelJetVect_PtSorted.push_back( thejet );
        }//end of jets loop

        //Calculate scalar sum of jets

        std::sort(LooseBJetVect_PtSorted.begin(),LooseBJetVect_PtSorted.end(),GreaterByPt());
        std::sort(MediumBJetVect_PtSorted.begin(),MediumBJetVect_PtSorted.end(),GreaterByPt());
        std::sort(TightBJetVect_PtSorted.begin(),TightBJetVect_PtSorted.end(),GreaterByPt());
        std::sort(SelJetVect_EtaSorted.begin(),SelJetVect_EtaSorted.end(),GreaterByEta());
        std::sort(SelJetVect_PtSorted.begin(),SelJetVect_PtSorted.end(),GreaterByPt());

	for(unsigned int bjetsindex = 0 ; bjetsindex < LooseBJetVect.size(); bjetsindex++){
	if(LooseBJetVect[bjetsindex] !=  SelJetVect_EtaSorted[0] ){
	BJetVect.push_back( LooseBJetVect[bjetsindex] );
	bDiscr_bjets.push_back( LooseBJetVect[bjetsindex]->bDiscriminator("pfDeepCSVJetTags:probb") + LooseBJetVect[bjetsindex]->bDiscriminator("pfDeepCSVJetTags:probbb") );
	}
	}
	LooseBJetVect.clear();
	std::sort(BJetVect.begin(),BJetVect.end(), GreaterByBTagging("pfDeepCSVJetTags:probb", "pfDeepCSVJetTags:probbb"));
	std::sort(bDiscr_bjets.begin(), bDiscr_bjets.end(), std::greater<float>());

        if(SelJetVect.size() < jetsNumberThreshold_ || BJetVect.size() < bjetsNumberThreshold_){
         SelJetVect.clear();
         SelJetVect_EtaSorted.clear(); SelJetVect_PtSorted.clear(); SelJetVect_BSorted.clear();
         LooseBJetVect.clear(); LooseBJetVect_PtSorted.clear();
         MediumBJetVect.clear(); MediumBJetVect_PtSorted.clear(); BJetVect.clear(); 
         TightBJetVect.clear(); TightBJetVect_PtSorted.clear();
         centraljet.clear(); forwardjet.clear();
         continue; }	
    float bjet1_discr_=-999;
    float bjet2_discr_=-999;
    float bjet3_discr_=-999;
    float bjet1_pt_=-999;
    float bjet2_pt_=-999;
    float bjet3_pt_=-999;
    float bjet1_eta_=-999;
    float bjet2_eta_=-999;
    float bjet3_eta_=-999;
    float jet1_pt_=-999;
    float jet2_pt_=-999;
    float jet3_pt_=-999;
    float jet4_pt_=-999;
    float jet1_eta_=-999;
    float jet2_eta_=-999;
    float jet3_eta_=-999;
    float jet4_eta_=-999;
    float jet1_discr_=-999;
    float jet2_discr_=-999;
    float jet3_discr_=-999;
    float jet4_discr_=-999;
    float fwdJet1_pt_=-999;
    float fwdJet1_eta_=-999;
    float fwdJet1_discr_=-999;


//------------------------------------Likelihood and MVA-----------------------------------------
//---------------------------------------------------------------------------------------

        fwdJet1 = SelJetVect_EtaSorted[0];
        bJet1 = BJetVect[0];
        l1.SetPtEtaPhiE(0., 0., 0., 0.);
        b1.SetPtEtaPhiE(0., 0., 0., 0.);
        l1b1.SetPtEtaPhiE(0., 0., 0., 0.);
        l1b1met.SetPtEtaPhiE(0., 0., 0., 0.);

        b1.SetPtEtaPhiE(bJet1->pt(), bJet1->eta(), bJet1->phi(), bJet1->energy());

        l1b1 = l1 + b1;
        l1b1met = l1b1 + metL;
        top_mt11_ = sqrt((l1b1met.M() * l1b1met.M()) + (l1b1met.Pt() * l1b1met.Pt()));
	tHchain=G1 + G2 + b1 + metL + l1;

	dipho_pt_ = dipho->pt(); 
    dipho_leadPtOvermass_ = dipho->leadingPhoton()->pt()/dipho->mass();
    dipho_subleadPtOvermass_ = dipho->subLeadingPhoton()->pt()/dipho->mass();
    dipho_leadEta_ = dipho->leadingPhoton()->eta();
    dipho_subleadEta_ = dipho->subLeadingPhoton()->eta();
    dipho_leadIDMVA_ = dipho->leadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );
    dipho_subleadIDMVA_ = dipho->subLeadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );
    dipho_lead_haspixelseed_ = dipho->leadingPhoton()->hasPixelSeed();
    dipho_sublead_haspixelseed_ = dipho->subLeadingPhoton()->hasPixelSeed(); 
	n_jets_ = SelJetVect.size();
	n_bjets_ = BJetVect.size();
	n_centraljets_ = centraljet.size();	
        bjet1_pt_ = bJet1->pt();
	bjet1_eta_ = bJet1->eta();
	dEta_leptonfwdjet_ = std::abs(l1.Eta()-fwdJet1->eta());
	bjet1_discr_ = bDiscr_bjets.at(0);

	if(SelJetVect.size()>0){
	    jet1_pt_ = SelJetVect[0]->pt();
        jet1_eta_ = SelJetVect[0]->eta();
        jet1_discr_= SelJetVect[0]->bDiscriminator("pfDeepCSVJetTags:probb") + SelJetVect[0]->bDiscriminator("pfDeepCSVJetTags:probbb");
	}
    if(SelJetVect.size()>1){
        jet2_pt_ = SelJetVect[1]->pt();
        jet2_eta_ = SelJetVect[1]->eta();
        jet2_discr_= SelJetVect[1]->bDiscriminator("pfDeepCSVJetTags:probb") + SelJetVect[1]->bDiscriminator("pfDeepCSVJetTags:probbb");
    }
    if(SelJetVect.size()>2){
        jet3_pt_ = SelJetVect[2]->pt();
        jet3_eta_ = SelJetVect[2]->eta();
        jet3_discr_= SelJetVect[2]->bDiscriminator("pfDeepCSVJetTags:probb") + SelJetVect[2]->bDiscriminator("pfDeepCSVJetTags:probbb");
    }
    if(SelJetVect.size()>3){
        jet4_pt_ = SelJetVect[3]->pt();
        jet4_eta_ = SelJetVect[3]->eta();
        jet4_discr_= SelJetVect[3]->bDiscriminator("pfDeepCSVJetTags:probb") + SelJetVect[3]->bDiscriminator("pfDeepCSVJetTags:probbb");
    }
    if(BJetVect.size()>0){
        bjet1_pt_ = BJetVect[0]->pt();
        bjet1_eta_ = BJetVect[0]->eta();
        bjet1_discr_= BJetVect[0]->bDiscriminator("pfDeepCSVJetTags:probb") + BJetVect[0]->bDiscriminator("pfDeepCSVJetTags:probbb");
    }
    if(BJetVect.size()>1){
        bjet2_pt_ = BJetVect[1]->pt();
        bjet2_eta_ = BJetVect[1]->eta();
        bjet2_discr_= BJetVect[1]->bDiscriminator("pfDeepCSVJetTags:probb") + BJetVect[1]->bDiscriminator("pfDeepCSVJetTags:probbb");
    }
    if(BJetVect.size()>2){
        bjet3_pt_ = BJetVect[2]->pt();
        bjet3_eta_ = BJetVect[2]->eta();
        bjet3_discr_= BJetVect[2]->bDiscriminator("pfDeepCSVJetTags:probb") + BJetVect[2]->bDiscriminator("pfDeepCSVJetTags:probbb");
    }
    if(SelJetVect_EtaSorted.size()>0){
        fwdJet1_pt_ = SelJetVect_EtaSorted[0]->pt();
        fwdJet1_eta_ = SelJetVect_EtaSorted[0]->eta();
        fwdJet1_discr_= SelJetVect_EtaSorted[0]->bDiscriminator("pfDeepCSVJetTags:probb") + SelJetVect_EtaSorted[0]->bDiscriminator("pfDeepCSVJetTags:probbb");
    }
    bDiscr_jets.push_back(jet1_discr_);
    bDiscr_jets.push_back(jet2_discr_);
    bDiscr_jets.push_back(jet3_discr_);
    bDiscr_jets.push_back(jet4_discr_);
    bDiscr_fwdjets.push_back(fwdJet1_discr_ );

        dRbjetfwdjet_ = deltaR( b1.Eta() , b1.Phi() , fwdJet1->eta() , fwdJet1->phi() );
        //dRtHchainfwdjet_ = deltaR( tHchain.Eta() , tHchain.Phi() , fwdJL.Eta() , fwdJL.Phi() );
        dRtHchainfwdjet_ = deltaR( tHchain.Eta() , tHchain.Phi() , fwdJet1->eta() , fwdJet1->phi() ); //this seems to be the source of the problems
        dRleadphobjet_ = deltaR( G1.Eta() , G1.Phi(), b1.Eta() , b1.Phi());
        dRsubleadphobjet_ = deltaR( G2.Eta() , G2.Phi(), b1.Eta() , b1.Phi());
        dRleadphofwdjet_ = deltaR( G1.Eta() , G1.Phi(), fwdJet1->eta() , fwdJet1->phi());
        dRsubleadphofwdjet_ = deltaR( G2.Eta() , G2.Phi(), fwdJet1->eta() , fwdJet1->phi());
        dRleptonbjet_ = deltaR( l1.Eta() , l1.Phi(), b1.Eta() , b1.Phi());
        dRleptonfwdjet_ = deltaR( l1.Eta() , l1.Phi(), fwdJet1->eta() , fwdJet1->phi());

        minPhoID_=TMath::Min( dipho_leadIDMVA_, dipho_subleadIDMVA_);
        maxPhoID_=TMath::Max( dipho_leadIDMVA_, dipho_subleadIDMVA_);

        maxBTagVal_ = bDiscr_bjets.size() > 0 ? bDiscr_bjets[0] : -1.;
        secondMaxBTagVal_ = bDiscr_bjets.size() > 1 ? bDiscr_bjets[1]: -1.;

//Evaluate MVA-----------------------
        MVAscore_tHqVsttHBDT = thqLeptonicMva_tHqVsttHBDT->EvaluateMVA( MVAMethod_.c_str() );
        MVAscore_tHqVsNonHiggsBkg = thqLeptonicMva_tHqVsNonHiggsBkg->EvaluateMVA( MVAMethod_.c_str() );
//DNN input variables--------------------------------
        std::vector<double> global_features_ttH_vs_tH;
        global_features_ttH_vs_tH.resize(23);        
        global_features_ttH_vs_tH[0] = dipho->leadingPhoton()->eta();
        global_features_ttH_vs_tH[1] = dipho->subLeadingPhoton()->eta();
        global_features_ttH_vs_tH[2] = dipho->leadingPhoton()->phi();
        global_features_ttH_vs_tH[3] = dipho->subLeadingPhoton()->phi();
        global_features_ttH_vs_tH[4] = dipho_leadPtOvermass_;
        global_features_ttH_vs_tH[5] = dipho_subleadPtOvermass_;
        global_features_ttH_vs_tH[6] = maxPhoID_;
        global_features_ttH_vs_tH[7] = minPhoID_;
        global_features_ttH_vs_tH[8] = log(theMET->getCorPt());
        global_features_ttH_vs_tH[9] = theMET->getCorPhi();
        global_features_ttH_vs_tH[10] = dipho_lead_haspixelseed_;
        global_features_ttH_vs_tH[11] = dipho_sublead_haspixelseed_;
        global_features_ttH_vs_tH[12] = dipho->rapidity();
        global_features_ttH_vs_tH[13] = dipho->pt()/dipho->mass();
        global_features_ttH_vs_tH[14] = deltaR( dipho->leadingPhoton()->eta(),dipho->leadingPhoton()->phi(), dipho->subLeadingPhoton()->eta(),dipho->subLeadingPhoton()->phi());
        global_features_ttH_vs_tH[15] = maxBTagVal_;
        global_features_ttH_vs_tH[16] = secondMaxBTagVal_;
        global_features_ttH_vs_tH[17] = SelJetVect.size();
        global_features_ttH_vs_tH[18] = (goodElectrons.size() + goodMuons.size());

        double lep1_charge, lep2_charge;
        calculate_lepton_charges(lep1_charge, lep2_charge, goodMuons, goodElectrons);

        global_features_ttH_vs_tH[19] = lep1_charge;
        global_features_ttH_vs_tH[20] = lep2_charge;
        global_features_ttH_vs_tH[21] = std::abs(fwdJet1_eta_);
        global_features_ttH_vs_tH[22] = log(fwdJet1_pt_);


        dnn_ttH_vs_tH->SetInputs(SelJetVect, goodMuons, goodElectrons, global_features_ttH_vs_tH);
//Evaluate DNN------------------------
        MVAscore_tHqVsttHDNN = dnn_ttH_vs_tH->EvaluateDNN();
//cout<<"ttH_vs_tH_dnn_score=  "<<MVAscore_tHqVsttHDNN<<endl;
//Tagger with MVAs--------------------
     if(use_MVAs_){
        if(use_tthVstHBDT_){
	        if( MVAscore_tHqVsttHBDT < MVAThreshold_tHqVsttHBDT_){
                SelJetVect.clear();
                SelJetVect_EtaSorted.clear(); SelJetVect_PtSorted.clear(); SelJetVect_BSorted.clear();
                LooseBJetVect.clear(); LooseBJetVect_PtSorted.clear();
                BJetVect.clear(); MediumBJetVect_PtSorted.clear();
                TightBJetVect.clear(); TightBJetVect_PtSorted.clear();
                centraljet.clear(); forwardjet.clear();
                continue; 
                }
            }
        if(use_tthVstHDNN_){
            if( MVAscore_tHqVsttHDNN > MVAThreshold_tHqVsttHDNN_){
                SelJetVect.clear();
                SelJetVect_EtaSorted.clear(); SelJetVect_PtSorted.clear(); SelJetVect_BSorted.clear();
                LooseBJetVect.clear(); LooseBJetVect_PtSorted.clear();
                BJetVect.clear(); MediumBJetVect_PtSorted.clear();
                TightBJetVect.clear(); TightBJetVect_PtSorted.clear();
                centraljet.clear(); forwardjet.clear();
                continue;
                }
            }
        if( MVAscore_tHqVsNonHiggsBkg < MVAThreshold_tHqVsNonHiggsBkg_){
            SelJetVect.clear();
            SelJetVect_EtaSorted.clear(); SelJetVect_PtSorted.clear(); SelJetVect_BSorted.clear();
            LooseBJetVect.clear(); LooseBJetVect_PtSorted.clear();
            BJetVect.clear(); MediumBJetVect_PtSorted.clear();
            TightBJetVect.clear(); TightBJetVect_PtSorted.clear();
            centraljet.clear(); forwardjet.clear();
            continue; 
            }
        }
//int catnum = -1;
//catnum = chooseCategory( MVAscore_tHqVsttHBDT );
//catnum = chooseCategory( mvares->result );
//catnum = chooseCategory( idmva1, idmva2 );
//---------------------------------------------------------------------------------------
     if(debug_){
     cout<<"jet1_eta_"<<jet1_eta_<<endl;
     cout<<"jet2_eta_"<<jet2_eta_<<endl;
     cout<<"jet3_eta_"<<jet3_eta_<<endl;
     cout<<"jet4_eta_"<<jet4_eta_<<endl;
     cout<<"jet1_pt_"<<jet1_pt_<<endl;
     cout<<"jet2_pt_"<<jet2_pt_<<endl;
     cout<<"jet3_pt_"<<jet3_pt_<<endl;
     cout<<"jet4_pt_"<<jet4_pt_<<endl;
     cout<<"jet1_discr_"<<jet1_discr_<<endl;
     cout<<"jet2_discr_"<<jet2_discr_<<endl;
     cout<<"jet3_discr_"<<jet3_discr_<<endl;
     cout<<"jet4_discr_"<<jet4_discr_<<endl;
     cout<<"bjet1_pt_"<<bjet1_pt_<<endl;
     cout<<"bjet2_pt_"<<bjet2_pt_<<endl;
     cout<<"bjet3_pt_"<<bjet3_pt_<<endl;
     cout<<"bjet1_eta_"<<bjet1_eta_<<endl;
     cout<<"bjet2_eta_"<<bjet2_eta_<<endl;
     cout<<"bjet3_eta_"<<bjet3_eta_<<endl;
     cout<<"bjet1_discr_"<<bjet1_discr_<<endl;
     cout<<"bjet2_discr_"<<bjet2_discr_<<endl;
     cout<<"bjet3_discr_"<<bjet3_discr_<<endl;
     cout<<"fwdJet1_pt_= "<<fwdJet1_pt_<<endl;
     cout<<"fwdJet1_pt_= "<<fwdJet1_eta_<<endl;
     cout<<"fwdJet1_discr_= "<<fwdJet1_discr_<<endl;
        }
//---------------------------------------------------------------------------------------
            thqltags_obj.setHT(ht);
            // if( processId_.find("thq") != std::string::npos or processId_.find("thw") != std::string::npos ){
            // 	CTCVWeightedVariables["nJets"]->Fill( SelJetVect_EtaSorted.size() , CtCvWeights );
            // 	CTCVWeightedVariables["nbJets"]->Fill( MediumBJetVect.size() , CtCvWeights );
            // 	if( SelJetVect_EtaSorted.size() > 0 )
            // 	  CTCVWeightedVariables["jPrimeEta"]->Fill( abs(SelJetVect_EtaSorted[0]->eta() ) , CtCvWeights );
            // }

            if( photonSelection ) {
                FoxWolfram fwam( particles_LorentzVector );
                std::vector< particleinfo*> allfoxwolfs = {&foxwolf1 };
                for(uint ifw = 1 ; ifw < allfoxwolfs.size()+1 ; ifw++)
                    allfoxwolfs[ifw-1]->set( fwam.getMoment( FoxWolfram::SHAT , ifw ),
                                             fwam.getMoment( FoxWolfram::PT , ifw ),
                                             fwam.getMoment( FoxWolfram::ETA , ifw ),
                                             fwam.getMoment( FoxWolfram::PSUM , ifw ),
                                             fwam.getMoment( FoxWolfram::PZ , ifw ),
                                             fwam.getMoment( FoxWolfram::ONE , ifw ) );
                thqltags_obj.setrho(rho_);

                thqltags_obj.setLeptonType(LeptonType);
                thqltags_obj.includeWeights( *dipho );

                thqltags_obj.photonWeights = dipho->leadingPhoton()->centralWeight()*dipho->subLeadingPhoton()->centralWeight() ;
                thqltags_obj.setJets( SelJetVect_PtSorted , SelJetVect_EtaSorted);
                thqltags_obj.setBJets( BJetVect );
                thqltags_obj.nCentralJets = centraljet.size();
                thqltags_obj.nForwardJets = forwardjet.size();
                thqltags_obj.setcentraljet( centraljet );
                thqltags_obj.setforwardjet( forwardjet );
                thqltags_obj.setdRtHchainfwdjet( dRtHchainfwdjet_ ) ;
                thqltags_obj.setdRbjetfwdjet( dRbjetfwdjet_ ) ;
                thqltags_obj.setdRleadphobjet( dRleadphobjet_ );
                thqltags_obj.setdRsubleadphobjet( dRsubleadphobjet_ );
                thqltags_obj.setdRleadphofwdjet( dRleadphofwdjet_ );
                thqltags_obj.setdRsubleadphofwdjet( dRsubleadphofwdjet_ );
                thqltags_obj.setdRleptonbjet (dRleptonbjet_);
                thqltags_obj.setdRleptonfwdjet (dRleptonfwdjet_);
		        thqltags_obj.setdEtaleptonfwdjet(std::abs(l1.Eta()-fwdJet1->eta()));
		        thqltags_obj.settop_mt(top_mt11_);
		        thqltags_obj.settop_mass(topMass);
		        thqltags_obj.setlepton_ch(lepton_ch_);
                thqltags_obj.setlepton_leadPt(lepton_leadPt_);
                thqltags_obj.setlepton_leadEta(lepton_leadEta_);
                thqltags_obj.setmvaresult ( mvares->result ) ;     //diphoton mva
		        thqltags_obj.setbDiscriminatorValue( bDiscr_bjets, bDiscr_jets, bDiscr_fwdjets );
                thqltags_obj.setthq_mvaresult( MVAscore_tHqVsttHBDT );
                thqltags_obj.setthq_mvaresult_ForNonPeakingBkg( MVAscore_tHqVsNonHiggsBkg );
                thqltags_obj.setMVAscore_ttHvstHDNN( MVAscore_tHqVsttHDNN );
//		thqltags_obj.setCategoryNumber( catnum );
                thqltags_obj.bTagWeight = 1.0;
                thqltags_obj.bTagWeightDown = 1.0;
                thqltags_obj.bTagWeightUp = 1.0;
                for( auto j : SelJetVect_PtSorted ) {
                    thqltags_obj.includeWeights( *j );

                    /*	  if( j->hasWeight("JetBTagCutWeightCentral") ){
                    	      thqltags_obj.bTagWeight *= j->weight( "JetBTagCutWeightCentral" );
                    	      thqltags_obj.bTagWeightDown *= j->weight( "JetBTagCutWeightDown01sigma" );
                    	      thqltags_obj.bTagWeightUp *= j->weight( "JetBTagCutWeightUp01sigma" );
                    	  }
                    	  else
                    	    cout << "BTag weight is not set in jet" << endl;
                    */
                }

                thqltags_obj.setVertices( vertices->ptrs() );

                std::vector <float> a;
                std::vector <float> b;
                std::vector <float> c;
                std::vector <float> d;
                for( unsigned int muonIndex = 0; muonIndex < LooseMu200.size(); muonIndex++ ) {

                    Ptr<flashgg::Muon> muon = LooseMu200[muonIndex];

                    int vtxInd = -1;
                    double dzmin = 9999;
                    for( size_t ivtx = 0 ; ivtx < vertices->ptrs().size(); ivtx++ ) {
                        Ptr<reco::Vertex> vtx = vertices->ptrs()[ivtx];
                        if( !muon->innerTrack() ) continue;
                        if( fabs( muon->innerTrack()->vz() - vtx->position().z() ) < dzmin ) {
                            dzmin = fabs( muon->innerTrack()->vz() - vtx->position().z() );
                            vtxInd = ivtx;
                        }
                    }
                    Ptr<reco::Vertex> best_vtx = vertices->ptrs()[vtxInd];
                    a.push_back(muon->muonBestTrack()->dxy(best_vtx->position()));
                    b.push_back(muon->muonBestTrack()->dz(best_vtx->position()));
                    c.push_back(muon->muonBestTrack()->dxy(dipho->vtx()->position()));
                    d.push_back(muon->muonBestTrack()->dz(dipho->vtx()->position()));
                }//end of muons loop

                thqltags_obj.setLeptonVertices( "muon", a, b, c, d) ;


                thqltags_obj.setMuons( goodMuons , looseMus_PassTight , LooseMu25.size() , LooseMu15.size() , MediumMu25.size() , MediumMu15.size() , TightMuo25.size() , TightMuo15.size() );

                a.clear();
                b.clear();
                c.clear();
                d.clear();
                /*	for( unsigned int ElectronIndex = 0; ElectronIndex < vetoNonIsoElectrons.size(); ElectronIndex++ ) {

                	  Ptr<flashgg::Electron> electron = vetoNonIsoElectrons[ElectronIndex];

                	  int vtxInd = -1;
                	  double dzmin = 9999;
                	  for( size_t ivtx = 0 ; ivtx < vertices->ptrs().size(); ivtx++ ) {
                	    Ptr<reco::Vertex> vtx = vertices->ptrs()[ivtx];
                	    if( fabs( electron->gsfTrack()->dz(vtx->position()) ) < dzmin ) {
                	      dzmin = fabs(electron->gsfTrack()->dz( vtx->position() ));
                	      vtxInd = ivtx;
                	    }
                	  }
                	  Ptr<reco::Vertex> best_vtx = vertices->ptrs()[vtxInd];
                	  a.push_back(electron->gsfTrack()->dxy(best_vtx->position()));
                	  b.push_back(electron->gsfTrack()->dz(best_vtx->position()));
                	  c.push_back(electron->gsfTrack()->dxy(dipho->vtx()->position()));
                	  d.push_back(electron->gsfTrack()->dz(dipho->vtx()->position()));
                	  int elMissedHits = electron->gsfTrack()->hitPattern().numberOfAllHits( reco::HitPattern::MISSING_INNER_HITS);
                	  thqltags_obj.setElectronMisHits(elMissedHits);
                	}//end of electrons loop
                */
                thqltags_obj.setLeptonVertices( "electron", a, b, c, d) ;

                thqltags_obj.setElectrons( goodElectrons , vetoNonIsoElectrons_PassTight , vetoNonIsoElectrons_PassVeto , looseElectrons.size() , vetoElectrons.size() , mediumElectrons.size() , goodElectrons.size() );

                thqltags_obj.setDiPhotonIndex( diphoIndex );
                thqltags_obj.setSystLabel( systLabel_ );

                thqltags_obj.setFoxAndAplanarity( foxwolf1.another , eventshapes.pt );
                thqltags_obj.setMETPtEtaPhiE( "SolvedMET", metW_check.Pt(), metW_check.Eta(), metW_check.Phi(), metW_check.E() );
                topReco( &BJetVect );
                thqltags_obj.nMedium_bJets = MediumBJetVect.size();
                	thqltags_obj.nLoose_bJets = LooseBJetVect_PtSorted.size();
                	thqltags_obj.nTight_bJets = TightBJetVect_PtSorted.size();

                if( ! evt.isRealData() ) {
//Bellow comment out part leads to crash for thq sample.This is related to ctcv weights and currently not using. Need to investigate the crash.
/*                    if(processId_.find("thq") != std::string::npos or processId_.find("thw") != std::string::npos) {
                        //8 QCD scale weights
                        for( uint i = 1 ; i < 9 ; i ++ )
                            thqltags_obj.setScale(i-1,product_lhe->weights()[i].wgt/product_lhe->originalXWGTUP () );
                        //100 NNPDF30 LO weights
                        for( uint i = 9 ; i < 109 ; i ++ )
                            thqltags_obj.setPdf(i-9,product_lhe->weights()[i].wgt/product_lhe->originalXWGTUP () );
                        //1 as down variation
                        thqltags_obj.setAlphaDown(product_lhe->weights()[211].wgt/product_lhe->originalXWGTUP ());
                        //1 NNDF30 NLO weight
                        thqltags_obj.setPdfNLO(product_lhe->weights()[390].wgt/product_lhe->originalXWGTUP () );
                        for (uint i = 446 ; i < product_lhe->weights().size() ; i++) {
                            thqltags_obj.setCtCv(i-446,product_lhe->weights()[i].wgt/product_lhe->originalXWGTUP () );
                        }

                    } else if (processId_.find("h_") != std::string::npos or processId_.find("vbf") != std::string::npos) {
                        //temporary solution till ctcv issue on PDFWeightObject is solved :(
                        Handle<vector<flashgg::PDFWeightObject> > WeightHandle;
                        evt.getByToken( weightToken_, WeightHandle );
                        for( unsigned int weight_index = 0; weight_index < (*WeightHandle).size(); weight_index++ ) {
                            vector<uint16_t> compressed_weights = (*WeightHandle)[weight_index].pdf_weight_container;
                            std::vector<float> uncompressed = (*WeightHandle)[weight_index].uncompress( compressed_weights );
                            vector<uint16_t> compressed_alpha = (*WeightHandle)[weight_index].alpha_s_container;
                            std::vector<float> uncompressed_alpha = (*WeightHandle)[weight_index].uncompress( compressed_alpha );
                            vector<uint16_t> compressed_scale = (*WeightHandle)[weight_index].qcd_scale_container;
                            std::vector<float> uncompressed_scale = (*WeightHandle)[weight_index].uncompress( compressed_scale );
                            vector<uint16_t> compressed_nloweights = (*WeightHandle)[weight_index].pdfnlo_weight_container;
                            std::vector<float> uncompressed_nloweights = (*WeightHandle)[weight_index].uncompress( compressed_nloweights );
                            //   vector<uint16_t> compressed_ctcvweights = (*WeightHandle)[weight_index].ctcv_weight_container;
                            //   std::vector<float> uncompressed_ctcvweights = (*WeightHandle)[weight_index].uncompress( compressed_ctcvweights );
                            //   //std::cout << "size !! "<< uncompressed.size() << " "<< uncompressed_alpha.size() << " "<<uncompressed_scale.size()<<" " << uncompressed_nloweights.size() << " "  <<uncompressed_ctcvweights.size() << std::endl;
                            float central_w = uncompressed_scale[0];

                            for( unsigned int j=0; j<(*WeightHandle)[weight_index].pdf_weight_container.size(); j++ ) {
                                thqltags_obj.setPdf(j,uncompressed[j]/ central_w );
                            }
                            //   // for( unsigned int j=1; j<(*WeightHandle)[weight_index].ctcv_weight_container.size();j++ ) {
                            //   //   thqltags_obj.setCtCv(j,uncompressed_ctcvweights[j]/ central_w );
                            //   // }
                            if (uncompressed_alpha.size()>1)
                            {
                                thqltags_obj.setAlphaUp(uncompressed_alpha[0]/central_w );
                                thqltags_obj.setAlphaDown(uncompressed_alpha[1]/ central_w );
                            }
                            else
                                thqltags_obj.setAlphaDown(uncompressed_alpha[0]/ central_w );
                            for( uint i = 1 ; i < 9 ; i ++ )
                                thqltags_obj.setScale(i-1,uncompressed_scale[i]/central_w );

                            if (uncompressed_nloweights.size()>0)
                                thqltags_obj.setPdfNLO(uncompressed_nloweights[0]/ central_w);
                        }
                    }//end of reading PDF weights from PDFWeightObject
*/
                    evt.getByToken( genParticleToken_, genParticles );
                    evt.getByToken( genJetToken_, genJets );

                    THQLeptonicTagTruth truth_obj;
                    truth_obj.setDiPhoton ( dipho );

                    // --------
                    //gen met
                    TLorentzVector nu_lorentzVector, allnus_LorentzVector, promptnus_LorentzVector;

                    for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) {
                        edm::Ptr<reco::GenParticle> part = genParticles->ptrAt( genLoop );
                        bool fid_cut = (abs(part->eta())<5.0 && part->status()==1) ? 1 : 0;
                        bool isNu = (abs(part->pdgId())==12 || abs(part->pdgId())==14 || abs(part->pdgId())==16) ? 1 : 0;
                        if (!fid_cut || !isNu) continue;
                        if( part->isPromptFinalState() || part->isDirectPromptTauDecayProductFinalState()) {
                            nu_lorentzVector.SetPtEtaPhiE(  part->pt() , part->eta() , part->phi() , part->energy() );
                            promptnus_LorentzVector+=nu_lorentzVector;
                        }
                        else {
                            nu_lorentzVector.SetPtEtaPhiE(  part->pt() , part->eta() , part->phi() , part->energy() );
                            allnus_LorentzVector+=nu_lorentzVector;
                        }
                    }
                    thqltags_obj.setMETPtEtaPhiE( "allPromptNus", promptnus_LorentzVector.Pt(), promptnus_LorentzVector.Eta(), promptnus_LorentzVector.Phi(), promptnus_LorentzVector.Energy() );
                    thqltags_obj.setMETPtEtaPhiE( "allNus", allnus_LorentzVector.Pt(), allnus_LorentzVector.Eta(), allnus_LorentzVector.Phi(), allnus_LorentzVector.Energy() );
                    thqltags_obj.setMETPtEtaPhiE( "genMetTrue", theMET->genMET()->pt(), theMET->genMET()->eta(), theMET->genMET()->phi(), theMET->genMET()->energy() );

                    if(SelJetVect_PtSorted.size() > 1) {
                        unsigned int index_leadq       = std::numeric_limits<unsigned int>::max();
                        unsigned int index_subleadq    = std::numeric_limits<unsigned int>::max();
                        unsigned int index_subsubleadq    = std::numeric_limits<unsigned int>::max();
                        float pt_leadq = 0., pt_subleadq = 0., pt_subsubleadq = 0.;

                        // --------
                        //Partons
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
                                        pt_subsubleadq  = pt_subleadq;
                                        index_subleadq = genLoop;
                                        pt_subleadq  = part->pt();
                                    } else if( part->pt() > pt_subsubleadq ) {
                                        index_subsubleadq = genLoop;
                                        pt_subleadq  = part->pt();
                                    }
                                }
                            }
                        }

                        if( index_leadq < std::numeric_limits<unsigned int>::max() ) {
                            truth_obj.setLeadingParton( genParticles->ptrAt( index_leadq ) );
                        }
                        if( index_subleadq < std::numeric_limits<unsigned int>::max() ) {
                            truth_obj.setSubLeadingParton( genParticles->ptrAt( index_subleadq ) );
                        }
                        if( index_subsubleadq < std::numeric_limits<unsigned int>::max()) {
                            truth_obj.setSubSubLeadingParton( genParticles->ptrAt( index_subsubleadq ));
                        }


                        unsigned int index_gp_leadjet = std::numeric_limits<unsigned int>::max();
                        unsigned int index_gp_subleadjet = std::numeric_limits<unsigned int>::max();
                        unsigned int index_gp_leadphoton = std::numeric_limits<unsigned int>::max();
                        unsigned int index_gp_subleadphoton = std::numeric_limits<unsigned int>::max();
                        unsigned int index_gp_leadmuon = std::numeric_limits<unsigned int>::max();
                        unsigned int index_gp_subleadmuon = std::numeric_limits<unsigned int>::max();
                        unsigned int index_gp_leadelectron = std::numeric_limits<unsigned int>::max();
                        unsigned int index_gp_subleadelectron = std::numeric_limits<unsigned int>::max();

                        float dr_gp_leadjet = 999.;
                        float dr_gp_subleadjet = 999.;
                        float dr_gp_leadphoton = 999.;
                        float dr_gp_subleadphoton = 999.;
                        float dr_gp_leadmuon = 999.;
                        float dr_gp_subleadmuon = 999.;
                        float dr_gp_leadelectron = 999.;
                        float dr_gp_subleadelectron = 999.;

                        if (SelJetVect_PtSorted.size()>0)truth_obj.setLeadingJet( SelJetVect_PtSorted[0] );
                        if (SelJetVect_PtSorted.size()>1)truth_obj.setSubLeadingJet( SelJetVect_PtSorted[1] );
                        if (SelJetVect_PtSorted.size()>2)truth_obj.setSubSubLeadingJet( SelJetVect_PtSorted[2] );
                        if (SelJetVect_PtSorted.size()>0)truth_obj.setLeadingJet( SelJetVect_PtSorted[0] );
                        if (SelJetVect_PtSorted.size()>1)truth_obj.setSubLeadingJet( SelJetVect_PtSorted[1] );
                        if (thqltags_obj.muons().size()>0)truth_obj.setLeadingMuon( thqltags_obj.muons()[0] );
                        if (thqltags_obj.muons().size()>1)truth_obj.setSubLeadingMuon( thqltags_obj.muons()[1] );
                        if (thqltags_obj.electrons().size()>0)truth_obj.setLeadingElectron( thqltags_obj.electrons()[0] );
                        if (thqltags_obj.electrons().size()>1)truth_obj.setSubLeadingElectron( thqltags_obj.electrons()[1] );
                        // --------
                        //GEN-RECO Level Matching
                        for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ )
                        {
                            edm::Ptr<reco::GenParticle> part = genParticles->ptrAt( genLoop );
                            if( part->isHardProcess())
                            {
                                float dr;
                                if (truth_obj.hasLeadingJet()) {
                                    dr = deltaR( truth_obj.leadingJet()->eta(), truth_obj.leadingJet()->phi(), part->eta(), part->phi() );
                                    if( dr < dr_gp_leadjet )
                                    {
                                        dr_gp_leadjet = dr;
                                        index_gp_leadjet = genLoop;
                                    }
                                }

                                if (truth_obj.hasSubLeadingJet()) {
                                    dr = deltaR( truth_obj.subLeadingJet()->eta(), truth_obj.subLeadingJet()->phi(), part->eta(), part->phi() );
                                    if( dr < dr_gp_subleadjet )
                                    {
                                        dr_gp_subleadjet = dr;
                                        index_gp_subleadjet = genLoop;
                                    }
                                }

                                if (truth_obj.hasDiPhoton()) {
                                    dr = deltaR( truth_obj.diPhoton()->leadingPhoton()->eta(), truth_obj.diPhoton()->leadingPhoton()->phi(), part->eta(), part->phi() );
                                    if( dr < dr_gp_leadphoton )
                                    {
                                        dr_gp_leadphoton = dr;
                                        index_gp_leadphoton = genLoop;
                                    }
                                    dr = deltaR( truth_obj.diPhoton()->subLeadingPhoton()->eta(), truth_obj.diPhoton()->subLeadingPhoton()->phi(), part->eta(), part->phi() );
                                    if( dr < dr_gp_subleadphoton )
                                    {
                                        dr_gp_subleadphoton = dr;
                                        index_gp_subleadphoton = genLoop;
                                    }
                                }

                                if (truth_obj.hasLeadingMuon()) {
                                    dr = deltaR( truth_obj.leadingMuon()->eta(), truth_obj.leadingMuon()->phi(), part->eta(), part->phi() );
                                    if( dr < dr_gp_leadmuon )
                                    {
                                        dr_gp_leadmuon = dr;
                                        index_gp_leadmuon = genLoop;
                                    }
                                }

                                if (truth_obj.hasSubLeadingMuon()) {
                                    dr = deltaR( truth_obj.subLeadingMuon()->eta(), truth_obj.subLeadingMuon()->phi(), part->eta(), part->phi() );
                                    if( dr < dr_gp_subleadmuon )
                                    {
                                        dr_gp_subleadmuon = dr;
                                        index_gp_subleadmuon = genLoop;
                                    }
                                }

                                if (truth_obj.hasLeadingElectron()) {
                                    dr = deltaR( truth_obj.leadingElectron()->eta(), truth_obj.leadingElectron()->phi(), part->eta(), part->phi() );
                                    if( dr < dr_gp_leadelectron )
                                    {
                                        dr_gp_leadelectron = dr;
                                        index_gp_leadelectron = genLoop;
                                    }
                                }

                                if (truth_obj.hasSubLeadingElectron()) {
                                    dr = deltaR( truth_obj.subLeadingElectron()->eta(), truth_obj.subLeadingElectron()->phi(), part->eta(), part->phi() );
                                    if( dr < dr_gp_subleadelectron )
                                    {
                                        dr_gp_subleadelectron = dr;
                                        index_gp_subleadelectron = genLoop;
                                    }
                                }

                            }
                        }

                        if( index_gp_leadjet < std::numeric_limits<unsigned int>::max() ) {
                            truth_obj.setClosestParticleToLeadingJet( genParticles->ptrAt( index_gp_leadjet ) );
                        }
                        if( index_gp_subleadjet < std::numeric_limits<unsigned int>::max() ) {
                            truth_obj.setClosestParticleToSubLeadingJet( genParticles->ptrAt( index_gp_subleadjet ) );
                        }
                        if( index_gp_leadphoton < std::numeric_limits<unsigned int>::max() ) {
                            truth_obj.setClosestParticleToLeadingPhoton( genParticles->ptrAt( index_gp_leadphoton ) );
                        }
                        if( index_gp_subleadphoton < std::numeric_limits<unsigned int>::max() ) {
                            truth_obj.setClosestParticleToSubLeadingPhoton( genParticles->ptrAt( index_gp_subleadphoton ) );
                        }


                        if( index_gp_leadmuon < std::numeric_limits<unsigned int>::max() )
                        {
                            truth_obj.setClosestParticleToLeadingMuon( genParticles->ptrAt( index_gp_leadmuon ) );
                            const reco::GenParticle *mcMom;
                            mcMom = static_cast<const reco::GenParticle *>(genParticles->ptrAt( index_gp_leadmuon )->mother());
                            if (mcMom) {
                                if( abs(genParticles->ptrAt( index_gp_leadmuon )->pdgId())==13
                                        && genParticles->ptrAt( index_gp_leadmuon )->status()==1
                                        && abs( mcMom->pdgId())==24 )
                                {   truth_obj.setClosestPromptParticleToLeadingMuon( genParticles->ptrAt( index_gp_leadmuon ) );
                                }
                            }
                        }

                        if( index_gp_subleadmuon < std::numeric_limits<unsigned int>::max() ) {
                            truth_obj.setClosestParticleToSubLeadingMuon( genParticles->ptrAt( index_gp_subleadmuon ) );
                            const reco::GenParticle *mcMom;
                            mcMom = static_cast<const reco::GenParticle *>(genParticles->ptrAt( index_gp_subleadmuon )->mother());
                            if (mcMom) {
                                if( abs(genParticles->ptrAt( index_gp_subleadmuon )->pdgId())==13
                                        && genParticles->ptrAt( index_gp_subleadmuon )->status()==1
                                        && abs( mcMom->pdgId())==24 )
                                {
                                    truth_obj.setClosestPromptParticleToSubLeadingMuon( genParticles->ptrAt( index_gp_subleadmuon ) );
                                }
                            }
                        }
                        if( index_gp_leadelectron < std::numeric_limits<unsigned int>::max() )
                        {

                            truth_obj.setClosestParticleToLeadingElectron( genParticles->ptrAt( index_gp_leadelectron ) );
                            if(truth_obj.hasClosestParticleToLeadingElectron()) {
                            }
                            const reco::GenParticle *mcMom;
                            mcMom = static_cast<const reco::GenParticle *>(genParticles->ptrAt( index_gp_leadelectron )->mother());
                            if (mcMom) {
                                if( abs(genParticles->ptrAt( index_gp_leadelectron )->pdgId())==11
                                        && genParticles->ptrAt( index_gp_leadelectron )->status()==1
                                        && abs( mcMom->pdgId())==24 )
                                {
                                    truth_obj.setClosestPromptParticleToLeadingElectron( genParticles->ptrAt( index_gp_leadelectron ) );
                                }
                                if(truth_obj.hasClosestPromptParticleToLeadingElectron()) {
                                }

                                truth_obj.pt_genPromptParticleMatchingToLeadingElectron();
                            }
                        }
//	    truth_obj.testfunction();
                        if( index_gp_subleadelectron < std::numeric_limits<unsigned int>::max() ) {
                            const reco::GenParticle *mcMom;
                            truth_obj.setClosestParticleToSubLeadingElectron( genParticles->ptrAt( index_gp_subleadelectron ) );
                            mcMom = static_cast<const reco::GenParticle *>(genParticles->ptrAt( index_gp_subleadelectron )->mother());
                            if (mcMom) {
                                if( abs(genParticles->ptrAt( index_gp_subleadelectron )->pdgId())==11
                                        && genParticles->ptrAt( index_gp_subleadelectron )->status()==1
                                        && abs( mcMom->pdgId())==24 )
                                {
                                    truth_obj.setClosestPromptParticleToSubLeadingElectron( genParticles->ptrAt( index_gp_subleadelectron ) );
                                }
                            }
                        }

                        unsigned int index_gj_leadjet = std::numeric_limits<unsigned int>::max();
                        unsigned int index_gj_subleadjet = std::numeric_limits<unsigned int>::max();
                        unsigned int index_gj_subsubleadjet = std::numeric_limits<unsigned int>::max();

                        float dr_gj_leadjet = 999.;
                        float dr_gj_subleadjet = 999.;
                        float dr_gj_subsubleadjet = 999.;
                        // --------
                        //GEN Jet-RECO Jet Matching
                        for( unsigned int gjLoop = 0 ; gjLoop < genJets->size() ; gjLoop++ )
                        {
                            edm::Ptr <reco::GenJet> gj = genJets->ptrAt( gjLoop );
                            float dr = deltaR( SelJetVect_PtSorted[0]->eta(), SelJetVect_PtSorted[0]->phi(), gj->eta(), gj->phi() );
                            if( dr < dr_gj_leadjet )
                            {
                                dr_gj_leadjet = dr;
                                index_gj_leadjet = gjLoop;
                            }
                            //if(  > 1 ){
                            dr = deltaR( SelJetVect_PtSorted[1]->eta(), SelJetVect_PtSorted[1]->phi(), gj->eta(), gj->phi() );
                            if( dr < dr_gj_subleadjet )
                            {
                                dr_gj_subleadjet = dr;
                                index_gj_subleadjet = gjLoop;
                            }
                            //}
                            if (truth_obj.hasSubSubLeadingJet())
                            {
                                dr = deltaR( SelJetVect_PtSorted[2]->eta(), SelJetVect_PtSorted[2]->phi(), gj->eta(), gj->phi() );
                                if( dr < dr_gj_subsubleadjet )
                                {
                                    dr_gj_subsubleadjet = dr;
                                    index_gj_subsubleadjet = gjLoop;
                                }
                            }
                        }
                        if( index_gj_leadjet < std::numeric_limits<unsigned int>::max() ) {
                            truth_obj.setClosestGenJetToLeadingJet( genJets->ptrAt( index_gj_leadjet ) );
                        }
                        if( index_gj_subleadjet < std::numeric_limits<unsigned int>::max() ) {
                            truth_obj.setClosestGenJetToSubLeadingJet( genJets->ptrAt( index_gj_subleadjet ) );
                        }
                        if( index_gj_subsubleadjet < std::numeric_limits<unsigned int>::max() ) {
                            truth_obj.setClosestGenJetToSubSubLeadingJet( genJets->ptrAt( index_gj_subsubleadjet ) );
                        }

                        // --------
                        //Parton-Jet Matching
                        std::vector<edm::Ptr<reco::GenParticle>> ptOrderedPartons;
                        for (unsigned int genLoop(0); genLoop < genParticles->size(); genLoop++)
                        {
                            edm::Ptr<reco::GenParticle> gp = genParticles->ptrAt(genLoop);
                            bool isQuark = abs( gp->pdgId() ) < 7 && gp->numberOfMothers() == 0;
                            bool isGluon = gp->pdgId() == 21 && gp->numberOfMothers() == 0;
                            if (isGluon || isQuark) {
                                unsigned int insertionIndex(0);
                                for (unsigned int parLoop(0); parLoop<ptOrderedPartons.size(); parLoop++) {
                                    if (gp->pt() < ptOrderedPartons[parLoop]->pt()) {
                                        insertionIndex = parLoop + 1;
                                    }
                                }
                                ptOrderedPartons.insert( ptOrderedPartons.begin() + insertionIndex, gp);
                            }
                        }
                        //Lead
                        if ( ptOrderedPartons.size() > 0 && truth_obj.hasLeadingJet())
                        {
                            float dr(999.0);
                            unsigned pIndex(0);
                            for (unsigned partLoop(0); partLoop<ptOrderedPartons.size(); partLoop++) {
                                float deltaR_temp = deltaR(SelJetVect_PtSorted[0]->eta(),SelJetVect_PtSorted[0]->phi(),
                                                           ptOrderedPartons[partLoop]->eta(),ptOrderedPartons[partLoop]->phi());
                                if (deltaR_temp < dr) {
                                    dr = deltaR_temp;
                                    pIndex = partLoop;
                                }
                            }
                            truth_obj.setClosestPartonToLeadingJet( ptOrderedPartons[pIndex] );
                        }
                        //Sublead
                        if (ptOrderedPartons.size() > 0 && truth_obj.hasSubLeadingJet())
                        {
                            float dr(999.0);
                            unsigned pIndex(0);
                            for (unsigned partLoop(0); partLoop<ptOrderedPartons.size(); partLoop++) {
                                float deltaR_temp = deltaR(SelJetVect_PtSorted[1]->eta(),SelJetVect_PtSorted[1]->phi(),
                                                           ptOrderedPartons[partLoop]->eta(),ptOrderedPartons[partLoop]->phi());
                                if (deltaR_temp < dr) {
                                    dr = deltaR_temp;
                                    pIndex = partLoop;
                                }
                            }
                            truth_obj.setClosestPartonToSubLeadingJet( ptOrderedPartons[pIndex] );
                        }
                        //Subsublead
                        if (ptOrderedPartons.size() > 0 && truth_obj.hasSubSubLeadingJet())
                        {
                            float dr(999.0);
                            unsigned pIndex(0);
                            for (unsigned partLoop(0); partLoop<ptOrderedPartons.size(); partLoop++) {
                                float deltaR_temp = deltaR(SelJetVect_PtSorted[2]->eta(),SelJetVect_PtSorted[2]->phi(),
                                                           ptOrderedPartons[partLoop]->eta(),ptOrderedPartons[partLoop]->phi());
                                if (deltaR_temp < dr) {
                                    dr = deltaR_temp;
                                    pIndex = partLoop;
                                }
                            }
                            truth_obj.setClosestPartonToSubSubLeadingJet( ptOrderedPartons[pIndex] );
                        }

                    }
                    thqltags_obj.setStage1recoTag( DiPhotonTagBase::stage1recoTag::RECO_THQ_LEP );
                    thqltags->push_back( thqltags_obj );
                    //truths->push_back( truth_obj );
                    //thqltags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<THQLeptonicTagTruth> >( rTagTruth, idx++ ) ) );
                }// ! evt.isRealData() loop end !
                if (evt.isRealData()) {
                    thqltags_obj.setStage1recoTag( DiPhotonTagBase::stage1recoTag::RECO_THQ_LEP );
                    thqltags->push_back( thqltags_obj ); //FIXME at next iteration!!
                }

            }//thq tag
            else {
                if(false)
                    std::cout << " THQLeptonicTagProducer NO TAG " << std::endl;
            }

            n_jets = 0;

            particles_LorentzVector.clear();
            particles_RhoEtaPhiVector.clear();
            SelJetVect.clear();
            SelJetVect_EtaSorted.clear();
            SelJetVect_PtSorted.clear();
            SelJetVect_BSorted.clear();
            LooseBJetVect.clear();
            LooseBJetVect_PtSorted.clear();
            MediumBJetVect.clear();
	        BJetVect.clear();
            MediumBJetVect_PtSorted.clear();
            TightBJetVect.clear();
            TightBJetVect_PtSorted.clear();
            centraljet.clear();
            forwardjet.clear();
    }//diPho loop end !
    evt.put( std::move( thqltags ) );
    //evt.put( std::move( truths ) );
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
