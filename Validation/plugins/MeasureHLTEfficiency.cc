// system include files
#include <memory>
#include <cmath>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/Ptr.h"
//#include "DataFormats/Common/interface/PtrVector.h"

#include "DataFormats/Math/interface/deltaR.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/Electron.h"

#include "flashgg/DataFormats/interface/DiPhotonMVAResult.h"

#include <DataFormats/Math/interface/deltaR.h>

#include <set>
#include "TFile.h"
#include "TH1F.h"
#include "TGraphAsymmErrors.h"
#include "TCanvas.h"
#include "TLegend.h"

class HLTEfficiency : public edm::EDAnalyzer
{
public:
    explicit HLTEfficiency( const edm::ParameterSet & );
    ~HLTEfficiency();
    void init( const edm::TriggerResults &result, const edm::TriggerNames &triggerNames );
    //bool hltEvent(edm::Handle<edm::TriggerResults> triggerBits);
    bool onlineOfflineMatching( const edm::TriggerNames &triggerNames, edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjects,
                                math::XYZTLorentzVector p4, std::string filterLabel, float dRmin );

    void makeEfficiencies();

    //void endJob(const edm::LuminosityBlock& lumiSeg, const edm::EventSetup& c);
    bool L1Matching( edm::Handle<edm::View<l1extra::L1EmParticle>> l1H, math::XYZTLorentzVector cand, float ptThreshold );
    std::vector<math::XYZTLorentzVector> hltP4( const edm::TriggerNames &triggerNames, edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjects,
            std::vector<std::string> filterLabels );

private:
    virtual void analyze( const edm::Event &, const edm::EventSetup & ) override;

    std::string outputFileName_;
    float diphoMVACut_;
    std::vector<std::string> tagFilterName_;
    std::vector<std::string> probeFilterName_;
    std::vector<std::string> filterName_;

    edm::EDGetTokenT<edm::View<flashgg::DiPhotonCandidate>> diphotons_;
    edm::EDGetTokenT<edm::View<flashgg::Electron>> eles_;
    edm::EDGetTokenT<edm::View<l1extra::L1EmParticle>> l1iso_;
    edm::EDGetTokenT<edm::View<l1extra::L1EmParticle>> l1noniso_;
    edm::EDGetTokenT<edm::TriggerResults> triggerBits_;
    edm::EDGetTokenT<pat::TriggerObjectStandAloneCollection> triggerObjects_;
    edm::EDGetTokenT<pat::PackedTriggerPrescales> triggerPrescales_;

    edm::ParameterSetID triggerNamesID_;
    std::map<std::string, unsigned int> trigger_indices;
    std::map<std::string, unsigned int> prescales;
    std::map<std::string, unsigned int> prescale_counter;

    TH1F *TAG_L1_eta;
    TH1F *PROBE_L1_15_eta;
    TH1F *PROBE_L1_10_eta;
    TH1F *PROBE_L1_35_eta;
    TH1F *TAG_L1_pt;
    TH1F *PROBE_L1_15_pt;
    TH1F *PROBE_L1_10_pt;
    TH1F *PROBE_L1_35_pt;
    TH1F *TAG_L1_nvtx;
    TH1F *PROBE_L1_15_nvtx;
    TH1F *PROBE_L1_10_nvtx;
    TH1F *PROBE_L1_35_nvtx;
    TH1F *Zpeak;

    TH1F *PROBE_HLT_OR_eta_twosuite_high;
    TH1F *PROBE_HLT_OR_pt_twosuite_high;
    TH1F *PROBE_HLT_Iso_eta_twosuite_high;
    TH1F *PROBE_HLT_Iso_pt_twosuite_high;
    TH1F *PROBE_HLT_R9_eta_twosuite_high;
    TH1F *PROBE_HLT_R9_pt_twosuite_high;
    TH1F *PROBE_HLT_OR_nvtx_twosuite_high;
    TH1F *PROBE_HLT_Iso_nvtx_twosuite_high;
    TH1F *PROBE_HLT_R9_nvtx_twosuite_high;
    TH1F *TAG_HLT_eta_twosuite_high;
    TH1F *TAG_HLT_nvtx_twosuite_high;
    TH1F *TAG_HLT_pt_twosuite_high;

    TH1F *PROBE_HLT_OR_pt_seeded;
    TH1F *PROBE_HLT_Iso_pt_seeded;
    TH1F *PROBE_HLT_R9_pt_seeded;
    TH1F *PROBE_HLT_OR_eta_seeded;
    TH1F *PROBE_HLT_Iso_eta_seeded;
    TH1F *PROBE_HLT_R9_eta_seeded;
    TH1F *PROBE_HLT_OR_nvtx_seeded;
    TH1F *PROBE_HLT_Iso_nvtx_seeded;
    TH1F *PROBE_HLT_R9_nvtx_seeded;
    TH1F *TAG_HLT_eta_seeded;
    TH1F *TAG_HLT_pt_seeded;
    TH1F *TAG_HLT_nvtx_seeded;



    TH1F *PROBE_HLT_OR_eta_unseeded;
    TH1F *PROBE_HLT_OR_pt_unseeded;
    TH1F *PROBE_HLT_Iso_eta_unseeded;
    TH1F *PROBE_HLT_Iso_pt_unseeded;
    TH1F *PROBE_HLT_R9_eta_unseeded;
    TH1F *PROBE_HLT_R9_pt_unseeded;
    TH1F *PROBE_HLT_OR_nvtx_unseeded;
    TH1F *PROBE_HLT_Iso_nvtx_unseeded;
    TH1F *PROBE_HLT_R9_nvtx_unseeded;
    TH1F *TAG_HLT_eta_unseeded;
    TH1F *TAG_HLT_pt_unseeded;
    TH1F *TAG_HLT_nvtx_unseeded;

    TCanvas *c_eff_L1_7_pt = new TCanvas( "c_eff_L1_7_pt", "c_eff_L1_7_pt" );
    TGraphAsymmErrors *eff_L1_35_pt = new TGraphAsymmErrors();
    TGraphAsymmErrors *eff_L1_15_pt = new TGraphAsymmErrors();
    TGraphAsymmErrors *eff_L1_10_pt = new TGraphAsymmErrors();

    TCanvas *c_eff_L1_7_eta = new TCanvas( "c_eff_L1_7_eta", "c_eff_L1_7_eta" );
    TGraphAsymmErrors *eff_L1_35_eta = new TGraphAsymmErrors();
    TGraphAsymmErrors *eff_L1_15_eta = new TGraphAsymmErrors();
    TGraphAsymmErrors *eff_L1_10_eta = new TGraphAsymmErrors();

    TCanvas *c_eff_L1_7_nvtx = new TCanvas( "c_eff_L1_7_nvtx", "c_eff_L1_7_nvtx" );
    TGraphAsymmErrors *eff_L1_35_nvtx = new TGraphAsymmErrors();
    TGraphAsymmErrors *eff_L1_15_nvtx = new TGraphAsymmErrors();
    TGraphAsymmErrors *eff_L1_10_nvtx = new TGraphAsymmErrors();

    TCanvas *c_eff_HLT_OR_pt_seeded = new TCanvas( "c_eff_HLT_OR_pt_seeded", "c_eff_HLT_OR_pt_seeded" );
    TGraphAsymmErrors *eff_HLT_OR_pt_seeded = new TGraphAsymmErrors();
    TGraphAsymmErrors *eff_HLT_Iso_pt_seeded = new TGraphAsymmErrors();
    TGraphAsymmErrors *eff_HLT_R9_pt_seeded = new TGraphAsymmErrors();
    TCanvas *c_eff_HLT_OR_eta_seeded = new TCanvas( "c_eff_HLT_OR_eta_seeded", "c_eff_HLT_OR_eta_seeded" );
    TGraphAsymmErrors *eff_HLT_OR_eta_seeded = new TGraphAsymmErrors();
    TGraphAsymmErrors *eff_HLT_Iso_eta_seeded = new TGraphAsymmErrors();
    TGraphAsymmErrors *eff_HLT_R9_eta_seeded = new TGraphAsymmErrors();
    TCanvas *c_eff_HLT_OR_nvtx_seeded = new TCanvas( "c_eff_HLT_OR_nvtx_seeded", "c_eff_HLT_OR_nvtx_seeded" );
    TGraphAsymmErrors *eff_HLT_OR_nvtx_seeded = new TGraphAsymmErrors();
    TGraphAsymmErrors *eff_HLT_Iso_nvtx_seeded = new TGraphAsymmErrors();
    TGraphAsymmErrors *eff_HLT_R9_nvtx_seeded = new TGraphAsymmErrors();

    TCanvas *c_eff_HLT_OR_pt_unseeded = new TCanvas( "c_eff_HLT_OR_pt_unseeded", "c_eff_HLT_OR_pt_unseeded" );
    TGraphAsymmErrors *eff_HLT_OR_pt_unseeded = new TGraphAsymmErrors();
    TGraphAsymmErrors *eff_HLT_Iso_pt_unseeded = new TGraphAsymmErrors();
    TGraphAsymmErrors *eff_HLT_R9_pt_unseeded = new TGraphAsymmErrors();
    TCanvas *c_eff_HLT_OR_eta_unseeded = new TCanvas( "c_eff_HLT_OR_eta_unseeded", "c_eff_HLT_OR_eta_unseeded" );
    TGraphAsymmErrors *eff_HLT_OR_eta_unseeded = new TGraphAsymmErrors();
    TGraphAsymmErrors *eff_HLT_Iso_eta_unseeded = new TGraphAsymmErrors();
    TGraphAsymmErrors *eff_HLT_R9_eta_unseeded = new TGraphAsymmErrors();
    TCanvas *c_eff_HLT_OR_nvtx_unseeded = new TCanvas( "c_eff_HLT_OR_nvtx_unseeded", "c_eff_HLT_OR_nvtx_unseeded" );
    TGraphAsymmErrors *eff_HLT_OR_nvtx_unseeded = new TGraphAsymmErrors();
    TGraphAsymmErrors *eff_HLT_Iso_nvtx_unseeded = new TGraphAsymmErrors();
    TGraphAsymmErrors *eff_HLT_R9_nvtx_unseeded = new TGraphAsymmErrors();


};

HLTEfficiency::~HLTEfficiency()
{

    TFile *file = new TFile( outputFileName_.c_str(), "recreate" );

    c_eff_HLT_OR_pt_seeded->Write();
    c_eff_HLT_OR_pt_unseeded->Write();
    c_eff_L1_7_pt->Write();

    c_eff_HLT_OR_eta_seeded->Write();
    c_eff_HLT_OR_eta_unseeded->Write();
    c_eff_L1_7_eta->Write();

    c_eff_HLT_OR_nvtx_seeded->Write();
    c_eff_HLT_OR_nvtx_unseeded->Write();
    c_eff_L1_7_nvtx->Write();

    eff_HLT_OR_pt_seeded->Write();
    eff_HLT_Iso_pt_seeded->Write();
    eff_HLT_R9_pt_seeded->Write();

    eff_HLT_OR_pt_unseeded->Write();
    eff_HLT_Iso_pt_unseeded->Write();
    eff_HLT_R9_pt_unseeded->Write();

    Zpeak->Write();
    TAG_L1_eta->Write();
    PROBE_L1_35_eta->Write();
    PROBE_L1_15_eta->Write();
    PROBE_L1_10_eta->Write();
    TAG_L1_nvtx->Write();
    PROBE_L1_35_nvtx->Write();
    PROBE_L1_15_nvtx->Write();
    PROBE_L1_10_nvtx->Write();
    TAG_L1_pt->Write();
    PROBE_L1_15_pt->Write();
    PROBE_L1_10_pt->Write();
    PROBE_L1_35_pt->Write();

    PROBE_HLT_OR_pt_twosuite_high->Write();
    PROBE_HLT_Iso_pt_twosuite_high->Write();
    PROBE_HLT_R9_pt_twosuite_high->Write();
    PROBE_HLT_OR_eta_twosuite_high->Write();
    PROBE_HLT_Iso_eta_twosuite_high->Write();
    PROBE_HLT_R9_eta_twosuite_high->Write();
    PROBE_HLT_OR_nvtx_twosuite_high->Write();
    PROBE_HLT_Iso_nvtx_twosuite_high->Write();
    PROBE_HLT_R9_nvtx_twosuite_high->Write();
    TAG_HLT_eta_twosuite_high->Write();
    TAG_HLT_pt_twosuite_high->Write();
    TAG_HLT_nvtx_twosuite_high->Write();

    PROBE_HLT_OR_pt_seeded->Write();
    PROBE_HLT_Iso_pt_seeded->Write();
    PROBE_HLT_R9_pt_seeded->Write();
    PROBE_HLT_OR_eta_seeded->Write();
    PROBE_HLT_Iso_eta_seeded->Write();
    PROBE_HLT_R9_eta_seeded->Write();
    PROBE_HLT_OR_nvtx_seeded->Write();
    PROBE_HLT_Iso_nvtx_seeded->Write();
    PROBE_HLT_R9_nvtx_seeded->Write();
    TAG_HLT_pt_seeded->Write();
    TAG_HLT_eta_seeded->Write();
    TAG_HLT_nvtx_seeded->Write();

    PROBE_HLT_OR_pt_unseeded->Write();
    PROBE_HLT_Iso_pt_unseeded->Write();
    PROBE_HLT_R9_pt_unseeded->Write();
    PROBE_HLT_OR_eta_unseeded->Write();
    PROBE_HLT_Iso_eta_unseeded->Write();
    PROBE_HLT_R9_eta_unseeded->Write();
    PROBE_HLT_OR_nvtx_unseeded->Write();
    PROBE_HLT_Iso_nvtx_unseeded->Write();
    PROBE_HLT_R9_nvtx_unseeded->Write();
    TAG_HLT_pt_unseeded->Write();
    TAG_HLT_eta_unseeded->Write();
    TAG_HLT_nvtx_unseeded->Write();
    file->Close();
}

HLTEfficiency::HLTEfficiency( const edm::ParameterSet &iConfig ):
    outputFileName_( iConfig.getParameter<std::string>( "outputFileName" ) ),
    tagFilterName_( iConfig.getParameter<std::vector<std::string> >( "tagFilterName" ) ),
    probeFilterName_( iConfig.getParameter<std::vector<std::string> >( "probeFilterName" ) ),
    filterName_( iConfig.getParameter<std::vector<std::string>>( "filterName" ) ),
    diphotons_( consumes<edm::View<flashgg::DiPhotonCandidate>>( iConfig.getParameter<edm::InputTag>( "diphotons" ) ) ),
    eles_( consumes<edm::View<flashgg::Electron>>( iConfig.getParameter<edm::InputTag>( "electrons" ) ) ),
    l1iso_( consumes<edm::View<l1extra::L1EmParticle>>( iConfig.getParameter<edm::InputTag>( "l1Iso" ) ) ),
    l1noniso_( consumes<edm::View<l1extra::L1EmParticle>>( iConfig.getParameter<edm::InputTag>( "l1NonIso" ) ) ),
    triggerBits_( consumes<edm::TriggerResults>( iConfig.getParameter<edm::InputTag>( "bits" ) ) ),
    triggerObjects_( consumes<pat::TriggerObjectStandAloneCollection>( iConfig.getParameter<edm::InputTag>( "objects" ) ) ),
    triggerPrescales_( consumes<pat::PackedTriggerPrescales>( iConfig.getParameter<edm::InputTag>( "prescales" ) ) )
{

    if( tagFilterName_.size() != probeFilterName_.size() ) {
        std::cout << "Need to specify the same numbers of tag and probe filters." << std::endl;
        abort();
    }

    int etaBin = 30;
    int nvtxBin = 50;
    int ptBin = 30;
    Zpeak = new TH1F( "Zpeak", "", 120, 60, 120 );
    TAG_L1_eta = new TH1F( "DEN_L1_eta", "", etaBin, -3, 3 );
    PROBE_L1_35_eta = new TH1F( "PROBE_L1_35_eta", "", etaBin, -3, 3 );
    PROBE_L1_15_eta = new TH1F( "PROBE_L1_15_eta", "", etaBin, -3, 3 );
    PROBE_L1_10_eta = new TH1F( "PROBE_L1_10_eta", "", etaBin, -3, 3 );
    TAG_L1_pt = new TH1F( "DEN_L1_pt", "", ptBin, 10, 70 );
    PROBE_L1_15_pt = new TH1F( "PROBE_L1_15_pt", "", ptBin, 10, 70 );
    PROBE_L1_10_pt = new TH1F( "PROBE_L1_10_pt", "", ptBin, 10, 70 );
    PROBE_L1_35_pt = new TH1F( "PROBE_L1_35_pt", "", ptBin, 10, 70 );
    TAG_L1_nvtx = new TH1F( "DEN_L1_nvtx", "", nvtxBin, 0, 100 );
    PROBE_L1_35_nvtx = new TH1F( "PROBE_L1_35_nvtx", "", nvtxBin, 0, 100 );
    PROBE_L1_15_nvtx = new TH1F( "PROBE_L1_15_nvtx", "", nvtxBin, 0, 100 );
    PROBE_L1_10_nvtx = new TH1F( "PROBE_L1_10_nvtx", "", nvtxBin, 0, 100 );


    PROBE_HLT_OR_pt_twosuite_high = new TH1F( "PROBE_HLT_OR_pt_twosuite_high", "", ptBin, 10, 70 );
    PROBE_HLT_Iso_pt_twosuite_high = new TH1F( "PROBE_HLT_ISO_pt_twosuite_high", "", ptBin, 10, 70 );
    PROBE_HLT_R9_pt_twosuite_high = new TH1F( "PROBE_HLT_R9_pt_twosuite_high", "", ptBin, 10, 70 );
    PROBE_HLT_OR_eta_twosuite_high = new TH1F( "PROBE_HLT_OR_eta_twosuite_high", "", etaBin, -3, 3 );
    PROBE_HLT_Iso_eta_twosuite_high = new TH1F( "PROBE_HLT_ISO_eta_twosuite_high", "", etaBin, -3, 3 );
    PROBE_HLT_R9_eta_twosuite_high = new TH1F( "PROBE_HLT_R9_eta_twosuite_high", "", etaBin, -3, 3 );
    PROBE_HLT_OR_nvtx_twosuite_high = new TH1F( "PROBE_HLT_OR_nvtx_twosuite_high", "", nvtxBin, 0, 100 );
    PROBE_HLT_Iso_nvtx_twosuite_high = new TH1F( "PROBE_HLT_ISO_nvtx_twosuite_high", "", nvtxBin, 0, 100 );
    PROBE_HLT_R9_nvtx_twosuite_high = new TH1F( "PROBE_HLT_R9_nvtx_twosuite_high", "", nvtxBin, 0, 100 );
    TAG_HLT_eta_twosuite_high = new TH1F( "DEN_HLT_eta_twosuite_high", "", etaBin, -3, 3 );
    TAG_HLT_pt_twosuite_high = new TH1F( "DEN_HLT_pt_twosuite_high", "", ptBin, 10, 70 );
    TAG_HLT_nvtx_twosuite_high = new TH1F( "DEN_HLT_nvtx_twosuite_high", "", nvtxBin, 0, 100 );

    PROBE_HLT_OR_pt_unseeded = new TH1F( "PROBE_HLT_OR_pt_unseeded", "", ptBin, 10, 70 );
    PROBE_HLT_Iso_pt_unseeded = new TH1F( "PROBE_HLT_ISO_pt_unseeded", "", ptBin, 10, 70 );
    PROBE_HLT_R9_pt_unseeded = new TH1F( "PROBE_HLT_R9_pt_unseeded", "", ptBin, 10, 70 );
    PROBE_HLT_OR_eta_unseeded = new TH1F( "PROBE_HLT_OR_eta_unseeded", "", etaBin, -3, 3 );
    PROBE_HLT_Iso_eta_unseeded = new TH1F( "PROBE_HLT_ISO_eta_unseeded", "", etaBin, -3, 3 );
    PROBE_HLT_R9_eta_unseeded = new TH1F( "PROBE_HLT_R9_eta_unseeded", "", etaBin, -3, 3 );
    PROBE_HLT_OR_nvtx_unseeded = new TH1F( "PROBE_HLT_OR_nvtx_unseeded", "", nvtxBin, 0, 100 );
    PROBE_HLT_Iso_nvtx_unseeded = new TH1F( "PROBE_HLT_ISO_nvtx_unseeded", "", nvtxBin, 0, 100 );
    PROBE_HLT_R9_nvtx_unseeded = new TH1F( "PROBE_HLT_R9_nvtx_unseeded", "", nvtxBin, 0, 100 );
    TAG_HLT_eta_unseeded = new TH1F( "DEN_HLT_eta_unseeded", "", etaBin, -3, 3 );
    TAG_HLT_pt_unseeded = new TH1F( "DEN_HLT_pt_unseeded", "", ptBin, 10, 70 );
    TAG_HLT_nvtx_unseeded = new TH1F( "DEN_HLT_nvtx_unseeded", "", nvtxBin, 0, 100 );


    PROBE_HLT_OR_pt_seeded = new TH1F( "PROBE_HLT_OR_pt_seeded", "", ptBin, 10, 70 );
    PROBE_HLT_Iso_pt_seeded = new TH1F( "PROBE_HLT_ISO_pt_seeded", "", ptBin, 10, 70 );
    PROBE_HLT_R9_pt_seeded = new TH1F( "PROBE_HLT_R9_pt_seeded", "", ptBin, 10, 70 );
    PROBE_HLT_OR_eta_seeded = new TH1F( "PROBE_HLT_OR_eta_seeded", "", etaBin, -3, 3 );
    PROBE_HLT_Iso_eta_seeded = new TH1F( "PROBE_HLT_ISO_eta_seeded", "", etaBin, -3, 3 );
    PROBE_HLT_R9_eta_seeded = new TH1F( "PROBE_HLT_R9_eta_seeded", "", etaBin, -3, 3 );
    PROBE_HLT_OR_nvtx_seeded = new TH1F( "PROBE_HLT_OR_nvtx_seeded", "", nvtxBin, 0, 100 );
    PROBE_HLT_Iso_nvtx_seeded = new TH1F( "PROBE_HLT_ISO_nvtx_seeded", "", nvtxBin, 0, 100 );
    PROBE_HLT_R9_nvtx_seeded = new TH1F( "PROBE_HLT_R9_nvtx_seeded", "", nvtxBin, 0, 100 );
    TAG_HLT_pt_seeded = new TH1F( "DEN_HLT_pt_seeded", "", ptBin, 10, 70 );
    TAG_HLT_eta_seeded = new TH1F( "DEN_HLT_eta_seeded", "", etaBin, -3, 3 );
    TAG_HLT_nvtx_seeded = new TH1F( "DEN_HLT_nvtx_seeded", "", nvtxBin, 0, 100 );
}

void HLTEfficiency::init( const edm::TriggerResults &result, const edm::TriggerNames &triggerNames )
{

    std::cout << "in init function" << std::endl;
    trigger_indices.clear();
    for( unsigned int i = 0; i < triggerNames.triggerNames().size(); i++ ) {
        std::string trimmedName = HLTConfigProvider::removeVersion( triggerNames.triggerName( i ) );
        std::cout << triggerNames.triggerName( i ) << std::endl;
        trigger_indices[trimmedName] = triggerNames.triggerIndex( triggerNames.triggerName( i ) );
    }
}

//bool HLTEfficiency::hltEvent(edm::Handle<edm::TriggerResults> triggerBits) {
//
//  for (std::map<std::string, unsigned int>::const_iterator cit = trigger_indices.begin(); cit != trigger_indices.end(); cit++) {
//    if (triggerBits->accept(cit->second)) {
//      std::vector<std::string>::const_iterator it = find(tpTriggerName_.begin(), tpTriggerName_.end(), cit->first);
//      if (it != tpTriggerName_.end())
//	return true;
//    }
//  }
//
//  return false;
//}

bool HLTEfficiency::L1Matching( edm::Handle<edm::View<l1extra::L1EmParticle>> l1H, math::XYZTLorentzVector cand, float ptThreshold )
{

// const edm::PtrVector<l1extra::L1EmParticle>& l1Pointers = l1H->ptrVector();
    for( unsigned int i = 0; i < l1H->size(); i++ ) {

// for (edm::Ptr<l1extra::L1EmParticle> l1Ptr : l1H) {
        //dr < 0.2
        float dR = deltaR( l1H->ptrAt( i )->p4(), cand );
        if( dR < 0.3 and l1H->ptrAt( i )->et() > ptThreshold )
        { return true; }
    }

    return false;
}

std::vector<math::XYZTLorentzVector> HLTEfficiency::hltP4( const edm::TriggerNames &triggerNames,
        edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjects, std::vector<std::string> filterLabels )
{

    std::vector< math::XYZTLorentzVector> triggerCandidates;

    for( pat::TriggerObjectStandAlone obj : *triggerObjects ) {
        obj.unpackPathNames( triggerNames );
        for( std::string filter : filterLabels ) {
            if( obj.hasFilterLabel( filter ) )
            { triggerCandidates.push_back( obj.p4() ); }
        }
    }

    return triggerCandidates;
}

bool HLTEfficiency::onlineOfflineMatching( const edm::TriggerNames &triggerNames, edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjects,
        math::XYZTLorentzVector p4, std::string filterLabel, float dRmin = 0.15 )
{

    for( pat::TriggerObjectStandAlone obj : *triggerObjects ) {
        obj.unpackPathNames( triggerNames );
        if( obj.hasFilterLabel( filterLabel ) ) {
            //for (unsigned h = 0; h < obj.filterLabels().size(); ++h) std::cout << " " << obj.filterLabels()[h];
            float dR = deltaR( p4, obj.p4() );
            //std::cout << "dR: " << dR << std::endl;
            if( dR < dRmin )
            { return true; }
        }
    }

    return false;
}

void HLTEfficiency::analyze( const edm::Event &iEvent, const edm::EventSetup &iSetup )
{

    edm::Handle<edm::TriggerResults> triggerBits;
    edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjects;
    edm::Handle<pat::PackedTriggerPrescales> triggerPrescales;
    edm::Handle<edm::View<flashgg::DiPhotonCandidate>> diphotons;
    edm::Handle<edm::View<flashgg::Electron>> eles;

    iEvent.getByToken( triggerBits_, triggerBits );
    iEvent.getByToken( triggerObjects_, triggerObjects );
    iEvent.getByToken( triggerPrescales_, triggerPrescales );
    iEvent.getByToken( diphotons_, diphotons );
    iEvent.getByToken( eles_, eles );
    if( eles.failedToGet() )
    { return; }

    if( !triggerBits.isValid() ) {
        LogDebug( "" ) << "TriggerResults product not found - returning result=false!";
        return;
    }

    // Apply event selection
    const edm::TriggerNames &triggerNames = iEvent.triggerNames( *triggerBits );

    //temp
    //std::cout << "testing--------------------------------------------------------" << std::endl;
    //endtemp
    if( triggerNamesID_ != triggerNames.parameterSetID() ) {
        triggerNamesID_ = triggerNames.parameterSetID();
        init( *triggerBits, triggerNames );
    }

    int diphotonIndex = -1;
    bool isInverted = false;
    //std::cout << "diphoton size: " << diphotons->size() << std::endl;
    for( size_t i = 0; i < diphotons->size(); i++ ) {

        edm::Ptr<flashgg::DiPhotonCandidate> diphoPtr = diphotons->ptrAt( i );
        // FIXME add check same size filter collections
        //        std::cout << "tag filter size : " << tagFilterName_.size() << std::endl;
        for( size_t f = 0; f < tagFilterName_.size(); f++ ) {
            bool leadMatchedToTag = onlineOfflineMatching( triggerNames, triggerObjects, diphoPtr->leadingPhoton()->p4(), tagFilterName_[f] );
            bool leadMatchedToProbe = onlineOfflineMatching( triggerNames, triggerObjects, diphoPtr->leadingPhoton()->p4(), probeFilterName_[f] );
            bool subLeadMatchedToTag = onlineOfflineMatching( triggerNames, triggerObjects, diphoPtr->subLeadingPhoton()->p4(), tagFilterName_[f] );
            bool subLeadMatchedToProbe = onlineOfflineMatching( triggerNames, triggerObjects, diphoPtr->subLeadingPhoton()->p4(), probeFilterName_[f] );

            if( leadMatchedToTag and subLeadMatchedToProbe ) {
                diphotonIndex = i;
                break;
            }
            if( leadMatchedToProbe and subLeadMatchedToTag ) {
                diphotonIndex = i;
                isInverted = true;
                break;
            }
        }
    }

    if( diphotonIndex == -1 )
    { return; }

    edm::Ptr<flashgg::DiPhotonCandidate> theDiPhoton = diphotons->ptrAt( diphotonIndex );
    const flashgg::Photon *tag = theDiPhoton->leadingPhoton();
    const flashgg::Photon *probe = theDiPhoton->subLeadingPhoton();

    if( isInverted ) {
        tag = theDiPhoton->subLeadingPhoton();
        probe = theDiPhoton->leadingPhoton();
    }
    if( theDiPhoton->mass() < 70 or theDiPhoton->mass() > 110 )
    { return; }

    Zpeak->Fill( theDiPhoton->mass() );
    float probe_weight = 1;

    //////////need to apply reweighting for R9 for photons vs electrons/////////////////
    //  if (pho_r9[PROBE] < 0.8) probe_weight = weights_low[GetR9Bin_low(pho_r9[PROBE])];
    //  else if (pho_r9[PROBE] < 1.2) probe_weight = weights_high[GetR9Bin_high(pho_r9[PROBE])];
    //  if (probe_weight <=0) continue;

    edm::Handle<edm::View<l1extra::L1EmParticle>> l1iso;
    edm::Handle<edm::View<l1extra::L1EmParticle>> l1noniso;

    iEvent.getByToken( l1iso_, l1iso );
    iEvent.getByToken( l1noniso_, l1noniso );

    //TAG is matched to L1 iso 20 object, is it absolute requirement ????
    //Match the Tag to an L1, means the probe is not biased to need an L1 seed
    if( not L1Matching( l1iso, tag->p4(), 22. ) )
    { return; }

    //TAG is matched to HLT seeded leg
    if( !onlineOfflineMatching( triggerNames, triggerObjects, tag->p4(), filterName_[0] ) and
            !onlineOfflineMatching( triggerNames, triggerObjects, tag->p4(), filterName_[1] ) )
    { return; }

    float probe_nvtx = theDiPhoton->nVert();
    TAG_L1_eta->Fill( probe->eta(), probe_weight );
    TAG_L1_pt->Fill( probe->pt(), probe_weight );
    TAG_L1_nvtx->Fill( probe_nvtx, probe_weight );

    bool flag_L1_probe10 = false;
    bool flag_L1_probe15 = false;
    bool flag_L1_probe35 = false;
    bool flag_L1_probe22 = false;
    //=========L1===========
    //see if there's another L1 object > 22 or 15 ??? Controllare il Seed di L1
    if( L1Matching( l1iso, probe->p4(), 40 ) ) {
        PROBE_L1_35_eta->Fill( probe->eta(), probe_weight );
        PROBE_L1_35_pt->Fill( probe->pt(), probe_weight );
        PROBE_L1_35_nvtx->Fill( probe_nvtx, probe_weight );
        flag_L1_probe35 = true;
    }

    if( !flag_L1_probe35 and L1Matching( l1noniso, probe->p4(), 40 ) ) {
        PROBE_L1_35_eta->Fill( probe->eta(), probe_weight );
        PROBE_L1_35_pt->Fill( probe->pt(), probe_weight );
        PROBE_L1_35_nvtx->Fill( probe_nvtx, probe_weight );
        flag_L1_probe35 = true;
    }

    if( L1Matching( l1iso, probe->p4(), 22 ) ) {
        flag_L1_probe22 = true;
    }

    if( !flag_L1_probe22 and L1Matching( l1noniso, probe->p4(), 22 ) ) {
        flag_L1_probe22 = true;
    }

    if( L1Matching( l1iso, probe->p4(), 15 ) ) {
        PROBE_L1_15_eta->Fill( probe->eta(), probe_weight );
        PROBE_L1_15_pt->Fill( probe->pt(), probe_weight );
        PROBE_L1_15_nvtx->Fill( probe_nvtx, probe_weight );
        flag_L1_probe15 = true;
    }

    if( !flag_L1_probe15 and L1Matching( l1noniso, probe->p4(), 15 ) ) {
        PROBE_L1_15_eta->Fill( probe->eta(), probe_weight );
        PROBE_L1_15_pt->Fill( probe->pt(), probe_weight );
        PROBE_L1_15_nvtx->Fill( probe_nvtx, probe_weight );
        flag_L1_probe15 = true;
    }

    if( L1Matching( l1iso, probe->p4(), 10 ) ) {
        PROBE_L1_10_eta->Fill( probe->eta(), probe_weight );
        PROBE_L1_10_pt->Fill( probe->pt(), probe_weight );
        PROBE_L1_10_nvtx->Fill( probe_nvtx, probe_weight );
        flag_L1_probe10 = true;
    }

    if( !flag_L1_probe10 and L1Matching( l1noniso, probe->p4(), 10 ) ) {
        PROBE_L1_10_eta->Fill( probe->eta(), probe_weight );
        PROBE_L1_10_pt->Fill( probe->pt(), probe_weight );
        PROBE_L1_10_nvtx->Fill( probe_nvtx, probe_weight );
        flag_L1_probe10 = true;
    }

    //=======HLT===========
    //PART 0: 42/22
    //if no L1 matches at all, label:

    //PART I: PROBE leg passed L1 EG 10, see if it matches to L1 non-seeded/seeded HLT object
    //if( !flag_L1_probe10 )
    //{ return; }

    //Part I: PROBE leg doesn't have an L1 seed associated with it
    //modified to require a low pT l1 seed and to only look at the subleading leg
    if( flag_L1_probe10 ) {
        TAG_HLT_eta_unseeded->Fill( probe->eta(), probe_weight );
        TAG_HLT_pt_unseeded->Fill( probe->pt(), probe_weight );
        TAG_HLT_nvtx_unseeded->Fill( probe_nvtx, probe_weight );

        bool isoflag = 0;
        bool r9flag = 0;
        //sublead HLT only

        if( onlineOfflineMatching( triggerNames, triggerObjects, probe->p4(), filterName_[2] ) ) {
            r9flag = 1;
            PROBE_HLT_R9_eta_unseeded->Fill( probe->eta(), probe_weight );
            PROBE_HLT_R9_pt_unseeded->Fill( probe->pt(), probe_weight );
            PROBE_HLT_R9_nvtx_unseeded->Fill( probe_nvtx, probe_weight );
        }

        if( onlineOfflineMatching( triggerNames, triggerObjects, probe->p4(), filterName_[3] ) ) {
            isoflag = 1;
            PROBE_HLT_Iso_eta_unseeded->Fill( probe->eta(), probe_weight );
            PROBE_HLT_Iso_pt_unseeded->Fill( probe->pt(), probe_weight );
            PROBE_HLT_Iso_nvtx_unseeded->Fill( probe_nvtx, probe_weight );
        }
        if( isoflag or r9flag ) {
            PROBE_HLT_OR_eta_unseeded->Fill( probe->eta(), probe_weight );
            PROBE_HLT_OR_pt_unseeded->Fill( probe->pt(), probe_weight );
            PROBE_HLT_OR_nvtx_unseeded->Fill( probe_nvtx, probe_weight );
        }
    }
    //Step II: if Probe has a lead L1 seed matched to it
    if( !flag_L1_probe22 )
    { return; }

    TAG_HLT_eta_seeded->Fill( probe->eta(), probe_weight );
    TAG_HLT_pt_seeded->Fill( probe->pt(), probe_weight );
    TAG_HLT_nvtx_seeded->Fill( probe_nvtx, probe_weight );

    bool tempflag = 0;
    if( onlineOfflineMatching( triggerNames, triggerObjects, probe->p4(), filterName_[1] ) ) {
        tempflag = 1;
        PROBE_HLT_R9_eta_seeded->Fill( probe->eta(), probe_weight );
        PROBE_HLT_R9_pt_seeded->Fill( probe->pt(), probe_weight );
        PROBE_HLT_R9_nvtx_seeded->Fill( probe_nvtx, probe_weight );
    }

    if( onlineOfflineMatching( triggerNames, triggerObjects, probe->p4(), filterName_[0] ) ) {
        tempflag = 1;
        PROBE_HLT_Iso_eta_seeded->Fill( probe->eta(), probe_weight );
        PROBE_HLT_Iso_pt_seeded->Fill( probe->pt(), probe_weight );
        PROBE_HLT_Iso_nvtx_seeded->Fill( probe_nvtx, probe_weight );
    }

    if( tempflag ) {
        PROBE_HLT_OR_eta_seeded->Fill( probe->eta(), probe_weight );
        PROBE_HLT_OR_pt_seeded->Fill( probe->pt(), probe_weight );
        PROBE_HLT_OR_nvtx_seeded->Fill( probe_nvtx, probe_weight );
    }

    // SingleEG seeded only
    if( !flag_L1_probe35 )
    { return; }

    TAG_HLT_eta_twosuite_high->Fill( probe->eta(), probe_weight );
    TAG_HLT_pt_twosuite_high->Fill( probe->pt(), probe_weight );
    TAG_HLT_nvtx_twosuite_high->Fill( probe_nvtx, probe_weight );

    bool tempflag1 = 0;
    if( onlineOfflineMatching( triggerNames, triggerObjects, probe->p4(), filterName_[1] ) ) {
        tempflag1 = 1;
        PROBE_HLT_R9_eta_twosuite_high->Fill( probe->eta(), probe_weight );
        PROBE_HLT_R9_pt_twosuite_high->Fill( probe->pt(), probe_weight );
        PROBE_HLT_R9_nvtx_twosuite_high->Fill( probe_nvtx, probe_weight );
    }

    if( onlineOfflineMatching( triggerNames, triggerObjects, probe->p4(), filterName_[0] ) ) {
        tempflag1 = 1;
        PROBE_HLT_Iso_eta_twosuite_high->Fill( probe->eta(), probe_weight );
        PROBE_HLT_Iso_pt_twosuite_high->Fill( probe->pt(), probe_weight );
        PROBE_HLT_Iso_nvtx_twosuite_high->Fill( probe_nvtx, probe_weight );
    }

    if( tempflag1 ) {
        PROBE_HLT_OR_eta_twosuite_high->Fill( probe->eta(), probe_weight );
        PROBE_HLT_OR_pt_twosuite_high->Fill( probe->pt(), probe_weight );
        PROBE_HLT_OR_nvtx_twosuite_high->Fill( probe_nvtx, probe_weight );
    }


    //making efficiency graphs
    makeEfficiencies();
}
void HLTEfficiency::makeEfficiencies()
{
    c_eff_L1_7_pt->cd();
    eff_L1_10_pt ->BayesDivide( PROBE_L1_10_pt, TAG_L1_pt );
    eff_L1_10_pt->SetMinimum( 0.0 );
    eff_L1_10_pt->SetMaximum( 1.1 );
    eff_L1_10_pt->SetTitle( "RelVal z->ee, #sqrt{s} = 13 TeV" );
    eff_L1_10_pt->GetXaxis()->SetTitle( "p_{T} (GeV)" );
    eff_L1_10_pt->GetYaxis()->SetTitle( "L1 Efficiency" );
    eff_L1_10_pt->SetLineColor( 2 );
    eff_L1_10_pt->SetLineWidth( 2 );
    eff_L1_10_pt->SetMarkerStyle( 7 );
    eff_L1_10_pt->SetMarkerColor( 2 );

    eff_L1_10_pt->Draw( "AP" );
    eff_L1_15_pt ->BayesDivide( PROBE_L1_15_pt, TAG_L1_pt );
    eff_L1_15_pt->SetMinimum( 0.0 );
    eff_L1_15_pt->SetMaximum( 1.1 );
    eff_L1_15_pt->SetLineColor( 4 );
    eff_L1_15_pt->SetLineWidth( 2 );
    eff_L1_15_pt->SetMarkerStyle( 7 );
    eff_L1_15_pt->SetMarkerColor( 4 );
    eff_L1_15_pt->Draw( "SAMEP" );

    eff_L1_35_pt ->BayesDivide( PROBE_L1_35_pt, TAG_L1_pt );
    eff_L1_35_pt->SetMinimum( 0.0 );
    eff_L1_35_pt->SetMaximum( 1.1 );
    eff_L1_35_pt->SetLineColor( 6 );
    eff_L1_35_pt->SetMarkerColor( 6 );
    eff_L1_35_pt->SetLineWidth( 2 );
    eff_L1_35_pt->SetMarkerStyle( 7 );
    eff_L1_35_pt->Draw( "SAMEP" );

    TLegend *l_eff_L1_7_pt = new TLegend( 0.45, 0.15, 0.55, 0.32 );
    l_eff_L1_7_pt->SetShadowColor( 0 );
    l_eff_L1_7_pt->SetFillColor( 0 );
    l_eff_L1_7_pt->SetLineColor( 0 );
    l_eff_L1_7_pt->SetTextSize( 0.03 );
    l_eff_L1_7_pt->AddEntry( eff_L1_10_pt, "L1_EG10", "l" );
    l_eff_L1_7_pt->AddEntry( eff_L1_15_pt, "L1_EG15", "l" );
    l_eff_L1_7_pt->AddEntry( eff_L1_35_pt, "L1_EG40", "l" );
    l_eff_L1_7_pt->SetTextSize( 0.03 );
    l_eff_L1_7_pt->Draw();

    //HLT seeded pt efficiencies
    c_eff_HLT_OR_pt_seeded->cd();
    eff_HLT_OR_pt_seeded->BayesDivide( PROBE_HLT_OR_pt_seeded, TAG_HLT_pt_seeded );
    eff_HLT_OR_pt_seeded->SetMinimum( 0.0 );
    eff_HLT_OR_pt_seeded->SetMaximum( 1.1 );
    eff_HLT_OR_pt_seeded->SetTitle( "RelVal Z->ee, #sqrt{s} = 13 TeV" );
    eff_HLT_OR_pt_seeded->GetXaxis()->SetTitle( "p_{T} (GeV)" );
    eff_HLT_OR_pt_seeded->GetYaxis()->SetTitle( "HLT-Only Efficiency" );
    eff_HLT_OR_pt_seeded->SetLineColor( 1 );
    eff_HLT_OR_pt_seeded->SetMarkerColor( 1 );

    eff_HLT_OR_pt_seeded->SetLineWidth( 2 );
    eff_HLT_OR_pt_seeded->SetMarkerStyle( 7 );
    eff_HLT_OR_pt_seeded->Draw( "AP" );
    eff_HLT_Iso_pt_seeded ->BayesDivide( PROBE_HLT_Iso_pt_seeded, TAG_HLT_pt_seeded );
    eff_HLT_Iso_pt_seeded->SetMinimum( 0.0 );
    eff_HLT_Iso_pt_seeded->SetMaximum( 1.1 );
    eff_HLT_Iso_pt_seeded->SetLineColor( 2 );
    eff_HLT_Iso_pt_seeded->SetMarkerColor( 2 );

    eff_HLT_Iso_pt_seeded->SetLineWidth( 2 );
    eff_HLT_Iso_pt_seeded->SetMarkerStyle( 7 );
    eff_HLT_Iso_pt_seeded->Draw( "SAMEP" );
    eff_HLT_R9_pt_seeded ->BayesDivide( PROBE_HLT_R9_pt_seeded, TAG_HLT_pt_seeded );
    eff_HLT_R9_pt_seeded->SetMinimum( 0.0 );
    eff_HLT_R9_pt_seeded->SetMaximum( 1.1 );
    eff_HLT_R9_pt_seeded->SetLineColor( 4 );
    eff_HLT_R9_pt_seeded->SetMarkerColor( 4 );

    eff_HLT_R9_pt_seeded->SetLineWidth( 2 );
    eff_HLT_R9_pt_seeded->SetMarkerStyle( 7 );
    eff_HLT_R9_pt_seeded->Draw( "SAMEP" );
    TLegend *l_eff_HLT_OR_pt_seeded = new TLegend( 0.45, 0.15, 0.55, 0.32 );
    l_eff_HLT_OR_pt_seeded->SetShadowColor( 0 );
    l_eff_HLT_OR_pt_seeded->SetFillColor( 0 );
    l_eff_HLT_OR_pt_seeded->SetLineColor( 0 );
    l_eff_HLT_OR_pt_seeded->SetTextSize( 0.03 );
    l_eff_HLT_OR_pt_seeded->AddEntry( eff_HLT_OR_pt_seeded, "HLT_Photon30_Iso60CaloId_OR_R9Id", "l" );
    l_eff_HLT_OR_pt_seeded->AddEntry( eff_HLT_Iso_pt_seeded, "HLT_Photon30_Iso60CaloId", "l" );
    l_eff_HLT_OR_pt_seeded->AddEntry( eff_HLT_R9_pt_seeded, "HLT_Photon30_R9Id", "l" );
    l_eff_HLT_OR_pt_seeded->SetTextSize( 0.03 );
    l_eff_HLT_OR_pt_seeded->Draw();

    //HLT unseeded pt efficiencies
    c_eff_HLT_OR_pt_unseeded->cd();
    eff_HLT_OR_pt_unseeded->BayesDivide( PROBE_HLT_OR_pt_unseeded, TAG_HLT_pt_unseeded );
    eff_HLT_OR_pt_unseeded->SetMinimum( 0.0 );
    eff_HLT_OR_pt_unseeded->SetMaximum( 1.1 );
    eff_HLT_OR_pt_unseeded->SetTitle( "RelVal Z->ee, #sqrt{s} = 13 TeV" );
    eff_HLT_OR_pt_unseeded->GetXaxis()->SetTitle( "p_{T} (GeV)" );
    eff_HLT_OR_pt_unseeded->GetYaxis()->SetTitle( "HLT-Only Efficiency" );
    eff_HLT_OR_pt_unseeded->SetLineColor( 1 );
    eff_HLT_OR_pt_unseeded->SetMarkerColor( 1 );

    eff_HLT_OR_pt_unseeded->SetLineWidth( 2 );
    eff_HLT_OR_pt_unseeded->SetMarkerStyle( 7 );
    eff_HLT_OR_pt_unseeded->Draw( "AP" );
    eff_HLT_Iso_pt_unseeded ->BayesDivide( PROBE_HLT_Iso_pt_unseeded, TAG_HLT_pt_unseeded );
    eff_HLT_Iso_pt_unseeded->SetMinimum( 0.0 );
    eff_HLT_Iso_pt_unseeded->SetMaximum( 1.1 );
    eff_HLT_Iso_pt_unseeded->SetLineColor( 2 );
    eff_HLT_Iso_pt_unseeded->SetMarkerColor( 2 );

    eff_HLT_Iso_pt_unseeded->SetLineWidth( 2 );
    eff_HLT_Iso_pt_unseeded->SetMarkerStyle( 7 );
    eff_HLT_Iso_pt_unseeded->Draw( "SAMEP" );
    eff_HLT_R9_pt_unseeded ->BayesDivide( PROBE_HLT_R9_pt_unseeded, TAG_HLT_pt_unseeded );
    eff_HLT_R9_pt_unseeded->SetMinimum( 0.0 );
    eff_HLT_R9_pt_unseeded->SetMaximum( 1.1 );
    eff_HLT_R9_pt_unseeded->SetLineColor( 4 );
    eff_HLT_R9_pt_unseeded->SetMarkerColor( 4 );

    eff_HLT_R9_pt_unseeded->SetLineWidth( 2 );
    eff_HLT_R9_pt_unseeded->SetMarkerStyle( 7 );
    eff_HLT_R9_pt_unseeded->Draw( "SAMEP" );
    TLegend *l_eff_HLT_OR_pt_unseeded = new TLegend( 0.45, 0.15, 0.55, 0.32 );
    l_eff_HLT_OR_pt_unseeded->SetShadowColor( 0 );
    l_eff_HLT_OR_pt_unseeded->SetFillColor( 0 );
    l_eff_HLT_OR_pt_unseeded->SetLineColor( 0 );
    l_eff_HLT_OR_pt_unseeded->SetTextSize( 0.03 );
    l_eff_HLT_OR_pt_unseeded->AddEntry( eff_HLT_OR_pt_unseeded, "HLT_Photon18_Iso60CaloId_OR_R9ID", "l" );
    l_eff_HLT_OR_pt_unseeded->AddEntry( eff_HLT_Iso_pt_unseeded, "HLT_Photon18_Iso60CaloId", "l" );
    l_eff_HLT_OR_pt_unseeded->AddEntry( eff_HLT_R9_pt_unseeded, "HLT_Photon18_R9Id", "l" );
    l_eff_HLT_OR_pt_unseeded->SetTextSize( 0.03 );
    l_eff_HLT_OR_pt_unseeded->Draw();


    //eta efficiencies
    c_eff_L1_7_eta->cd();
    eff_L1_10_eta ->BayesDivide( PROBE_L1_10_eta, TAG_L1_eta );
    eff_L1_10_eta->SetMinimum( 0.0 );
    eff_L1_10_eta->SetMaximum( 1.1 );
    eff_L1_10_eta->SetTitle( "RelVal Z->ee, #sqrt{s} = 13 TeV" );
    eff_L1_10_eta->GetXaxis()->SetTitle( "Eta" );
    eff_L1_10_eta->GetYaxis()->SetTitle( "L1 Efficiency" );
    eff_L1_10_eta->SetLineColor( 2 );
    eff_L1_10_eta->SetLineWidth( 2 );
    eff_L1_10_eta->SetMarkerStyle( 7 );
    eff_L1_10_eta->SetMarkerColor( 2 );

    eff_L1_10_eta->Draw( "AP" );
    eff_L1_15_eta ->BayesDivide( PROBE_L1_15_eta, TAG_L1_eta );
    eff_L1_15_eta->SetMinimum( 0.0 );
    eff_L1_15_eta->SetMaximum( 1.1 );
    eff_L1_15_eta->SetLineColor( 4 );
    eff_L1_15_eta->SetLineWidth( 2 );
    eff_L1_15_eta->SetMarkerStyle( 7 );
    eff_L1_15_eta->SetMarkerColor( 4 );
    eff_L1_15_eta->Draw( "SAMEP" );

    eff_L1_35_eta ->BayesDivide( PROBE_L1_35_eta, TAG_L1_eta );
    eff_L1_35_eta->SetMinimum( 0.0 );
    eff_L1_35_eta->SetMaximum( 1.1 );
    eff_L1_35_eta->SetLineColor( 6 );
    eff_L1_35_eta->SetMarkerColor( 6 );
    eff_L1_35_eta->SetLineWidth( 2 );
    eff_L1_35_eta->SetMarkerStyle( 7 );
    eff_L1_35_eta->Draw( "SAMEP" );

    TLegend *l_eff_L1_7_eta = new TLegend( 0.45, 0.15, 0.55, 0.32 );
    l_eff_L1_7_eta->SetShadowColor( 0 );
    l_eff_L1_7_eta->SetFillColor( 0 );
    l_eff_L1_7_eta->SetLineColor( 0 );
    l_eff_L1_7_eta->SetTextSize( 0.03 );
    l_eff_L1_7_eta->AddEntry( eff_L1_10_eta, "L1_EG10", "l" );
    l_eff_L1_7_eta->AddEntry( eff_L1_15_eta, "L1_EG15", "l" );
    l_eff_L1_7_eta->AddEntry( eff_L1_35_eta, "L1_EG40", "l" );
    l_eff_L1_7_eta->SetTextSize( 0.03 );
    l_eff_L1_7_eta->Draw();

    //HLT seeded eta efficiencies
    c_eff_HLT_OR_eta_seeded->cd();
    eff_HLT_OR_eta_seeded->BayesDivide( PROBE_HLT_OR_eta_seeded, TAG_HLT_eta_seeded );
    eff_HLT_OR_eta_seeded->SetMinimum( 0.0 );
    eff_HLT_OR_eta_seeded->SetMaximum( 1.1 );
    eff_HLT_OR_eta_seeded->SetTitle( "RelVal Z->ee, #sqrt{s} = 13 TeV" );
    eff_HLT_OR_eta_seeded->GetXaxis()->SetTitle( "Eta" );
    eff_HLT_OR_eta_seeded->GetYaxis()->SetTitle( "HLT-Only Efficiency" );
    eff_HLT_OR_eta_seeded->SetLineColor( 1 );
    eff_HLT_OR_eta_seeded->SetMarkerColor( 1 );

    eff_HLT_OR_eta_seeded->SetLineWidth( 2 );
    eff_HLT_OR_eta_seeded->SetMarkerStyle( 7 );
    eff_HLT_OR_eta_seeded->Draw( "AP" );
    eff_HLT_Iso_eta_seeded ->BayesDivide( PROBE_HLT_Iso_eta_seeded, TAG_HLT_eta_seeded );
    eff_HLT_Iso_eta_seeded->SetMinimum( 0.0 );
    eff_HLT_Iso_eta_seeded->SetMaximum( 1.1 );
    eff_HLT_Iso_eta_seeded->SetLineColor( 2 );
    eff_HLT_Iso_eta_seeded->SetMarkerColor( 2 );

    eff_HLT_Iso_eta_seeded->SetLineWidth( 2 );
    eff_HLT_Iso_eta_seeded->SetMarkerStyle( 7 );
    eff_HLT_Iso_eta_seeded->Draw( "SAMEP" );
    eff_HLT_R9_eta_seeded ->BayesDivide( PROBE_HLT_R9_eta_seeded, TAG_HLT_eta_seeded );
    eff_HLT_R9_eta_seeded->SetMinimum( 0.0 );
    eff_HLT_R9_eta_seeded->SetMaximum( 1.1 );
    eff_HLT_R9_eta_seeded->SetLineColor( 4 );
    eff_HLT_R9_eta_seeded->SetMarkerColor( 4 );

    eff_HLT_R9_eta_seeded->SetLineWidth( 2 );
    eff_HLT_R9_eta_seeded->SetMarkerStyle( 7 );
    eff_HLT_R9_eta_seeded->Draw( "SAMEP" );
    TLegend *l_eff_HLT_OR_eta_seeded = new TLegend( 0.45, 0.15, 0.55, 0.32 );
    l_eff_HLT_OR_eta_seeded->SetShadowColor( 0 );
    l_eff_HLT_OR_eta_seeded->SetFillColor( 0 );
    l_eff_HLT_OR_eta_seeded->SetLineColor( 0 );
    l_eff_HLT_OR_eta_seeded->SetTextSize( 0.03 );
    l_eff_HLT_OR_eta_seeded->AddEntry( eff_HLT_OR_eta_seeded, "HLT_Photon30_Iso60CaloId_OR_R9Id", "l" );
    l_eff_HLT_OR_eta_seeded->AddEntry( eff_HLT_Iso_eta_seeded, "HLT_Photon30_Iso60CaloId", "l" );
    l_eff_HLT_OR_eta_seeded->AddEntry( eff_HLT_R9_eta_seeded, "HLT_Photon30_R9Id", "l" );
    l_eff_HLT_OR_eta_seeded->SetTextSize( 0.03 );
    l_eff_HLT_OR_eta_seeded->Draw();

    //HLT unseeded eta efficiencies
    c_eff_HLT_OR_eta_unseeded->cd();
    eff_HLT_OR_eta_unseeded->BayesDivide( PROBE_HLT_OR_eta_unseeded, TAG_HLT_eta_unseeded );
    eff_HLT_OR_eta_unseeded->SetMinimum( 0.0 );
    eff_HLT_OR_eta_unseeded->SetMaximum( 1.1 );
    eff_HLT_OR_eta_unseeded->SetTitle( "RelVal Z->ee, #sqrt{s} = 13 TeV" );
    eff_HLT_OR_eta_unseeded->GetXaxis()->SetTitle( "eta " );
    eff_HLT_OR_eta_unseeded->GetYaxis()->SetTitle( "HLT-Only Efficiency" );
    eff_HLT_OR_eta_unseeded->SetLineColor( 1 );
    eff_HLT_OR_eta_unseeded->SetMarkerColor( 1 );

    eff_HLT_OR_eta_unseeded->SetLineWidth( 2 );
    eff_HLT_OR_eta_unseeded->SetMarkerStyle( 7 );
    eff_HLT_OR_eta_unseeded->Draw( "AP" );
    eff_HLT_Iso_eta_unseeded ->BayesDivide( PROBE_HLT_Iso_eta_unseeded, TAG_HLT_eta_unseeded );
    eff_HLT_Iso_eta_unseeded->SetMinimum( 0.0 );
    eff_HLT_Iso_eta_unseeded->SetMaximum( 1.1 );
    eff_HLT_Iso_eta_unseeded->SetLineColor( 2 );
    eff_HLT_Iso_eta_unseeded->SetMarkerColor( 2 );

    eff_HLT_Iso_eta_unseeded->SetLineWidth( 2 );
    eff_HLT_Iso_eta_unseeded->SetMarkerStyle( 7 );
    eff_HLT_Iso_eta_unseeded->Draw( "SAMEP" );
    eff_HLT_R9_eta_unseeded ->BayesDivide( PROBE_HLT_R9_eta_unseeded, TAG_HLT_eta_unseeded );
    eff_HLT_R9_eta_unseeded->SetMinimum( 0.0 );
    eff_HLT_R9_eta_unseeded->SetMaximum( 1.1 );
    eff_HLT_R9_eta_unseeded->SetLineColor( 4 );
    eff_HLT_R9_eta_unseeded->SetMarkerColor( 4 );

    eff_HLT_R9_eta_unseeded->SetLineWidth( 2 );
    eff_HLT_R9_eta_unseeded->SetMarkerStyle( 7 );
    eff_HLT_R9_eta_unseeded->Draw( "SAMEP" );
    TLegend *l_eff_HLT_OR_eta_unseeded = new TLegend( 0.45, 0.15, 0.55, 0.32 );
    l_eff_HLT_OR_eta_unseeded->SetShadowColor( 0 );
    l_eff_HLT_OR_eta_unseeded->SetFillColor( 0 );
    l_eff_HLT_OR_eta_unseeded->SetLineColor( 0 );
    l_eff_HLT_OR_eta_unseeded->SetTextSize( 0.03 );
    l_eff_HLT_OR_eta_unseeded->AddEntry( eff_HLT_OR_eta_unseeded, "HLT_Photon18_Iso60CaloId_OR_R9ID", "l" );
    l_eff_HLT_OR_eta_unseeded->AddEntry( eff_HLT_Iso_eta_unseeded, "HLT_Photon18_Iso60CaloId", "l" );
    l_eff_HLT_OR_eta_unseeded->AddEntry( eff_HLT_R9_eta_unseeded, "HLT_Photon18_R9Id", "l" );
    l_eff_HLT_OR_eta_unseeded->SetTextSize( 0.03 );
    l_eff_HLT_OR_eta_unseeded->Draw();


    //nvtx efficiencies
    c_eff_L1_7_nvtx->cd();
    eff_L1_10_nvtx ->BayesDivide( PROBE_L1_10_nvtx, TAG_L1_nvtx );
    eff_L1_10_nvtx->SetMinimum( 0.0 );
    eff_L1_10_nvtx->SetMaximum( 1.1 );
    eff_L1_10_nvtx->SetTitle( "RelVal Z->ee, #sqrt{s} = 13 TeV" );
    eff_L1_10_nvtx->GetXaxis()->SetTitle( "Nvtx" );
    eff_L1_10_nvtx->GetYaxis()->SetTitle( "L1 Efficiency" );
    eff_L1_10_nvtx->SetLineColor( 2 );
    eff_L1_10_nvtx->SetLineWidth( 2 );
    eff_L1_10_nvtx->SetMarkerStyle( 7 );
    eff_L1_10_nvtx->SetMarkerColor( 2 );

    eff_L1_10_nvtx->Draw( "AP" );
    eff_L1_15_nvtx ->BayesDivide( PROBE_L1_15_nvtx, TAG_L1_nvtx );
    eff_L1_15_nvtx->SetMinimum( 0.0 );
    eff_L1_15_nvtx->SetMaximum( 1.1 );
    eff_L1_15_nvtx->SetLineColor( 4 );
    eff_L1_15_nvtx->SetLineWidth( 2 );
    eff_L1_15_nvtx->SetMarkerStyle( 7 );
    eff_L1_15_nvtx->SetMarkerColor( 4 );
    eff_L1_15_nvtx->Draw( "SAMEP" );

    eff_L1_35_nvtx ->BayesDivide( PROBE_L1_35_nvtx, TAG_L1_nvtx );
    eff_L1_35_nvtx->SetMinimum( 0.0 );
    eff_L1_35_nvtx->SetMaximum( 1.1 );
    eff_L1_35_nvtx->SetLineColor( 6 );
    eff_L1_35_nvtx->SetMarkerColor( 6 );
    eff_L1_35_nvtx->SetLineWidth( 2 );
    eff_L1_35_nvtx->SetMarkerStyle( 7 );
    eff_L1_35_nvtx->Draw( "SAMEP" );

    TLegend *l_eff_L1_7_nvtx = new TLegend( 0.45, 0.15, 0.55, 0.32 );
    l_eff_L1_7_nvtx->SetShadowColor( 0 );
    l_eff_L1_7_nvtx->SetFillColor( 0 );
    l_eff_L1_7_nvtx->SetLineColor( 0 );
    l_eff_L1_7_nvtx->SetTextSize( 0.03 );
    l_eff_L1_7_nvtx->AddEntry( eff_L1_10_nvtx, "L1_EG10", "l" );
    l_eff_L1_7_nvtx->AddEntry( eff_L1_15_nvtx, "L1_EG15", "l" );
    l_eff_L1_7_nvtx->AddEntry( eff_L1_35_nvtx, "L1_EG40", "l" );
    l_eff_L1_7_nvtx->SetTextSize( 0.03 );
    l_eff_L1_7_nvtx->Draw();

    //HLT seeded nvtx efficiencies
    c_eff_HLT_OR_nvtx_seeded->cd();
    eff_HLT_OR_nvtx_seeded->BayesDivide( PROBE_HLT_OR_nvtx_seeded, TAG_HLT_nvtx_seeded );
    eff_HLT_OR_nvtx_seeded->SetMinimum( 0.0 );
    eff_HLT_OR_nvtx_seeded->SetMaximum( 1.1 );
    eff_HLT_OR_nvtx_seeded->SetTitle( "RelVal Z->ee, #sqrt{s} = 13 TeV" );
    eff_HLT_OR_nvtx_seeded->GetXaxis()->SetTitle( "Nvtx" );
    eff_HLT_OR_nvtx_seeded->GetYaxis()->SetTitle( "HLT-Only Efficiency" );
    eff_HLT_OR_nvtx_seeded->SetLineColor( 1 );
    eff_HLT_OR_nvtx_seeded->SetMarkerColor( 1 );

    eff_HLT_OR_nvtx_seeded->SetLineWidth( 2 );
    eff_HLT_OR_nvtx_seeded->SetMarkerStyle( 7 );
    eff_HLT_OR_nvtx_seeded->Draw( "AP" );
    eff_HLT_Iso_nvtx_seeded ->BayesDivide( PROBE_HLT_Iso_nvtx_seeded, TAG_HLT_nvtx_seeded );
    eff_HLT_Iso_nvtx_seeded->SetMinimum( 0.0 );
    eff_HLT_Iso_nvtx_seeded->SetMaximum( 1.1 );
    eff_HLT_Iso_nvtx_seeded->SetLineColor( 2 );
    eff_HLT_Iso_nvtx_seeded->SetMarkerColor( 2 );

    eff_HLT_Iso_nvtx_seeded->SetLineWidth( 2 );
    eff_HLT_Iso_nvtx_seeded->SetMarkerStyle( 7 );
    eff_HLT_Iso_nvtx_seeded->Draw( "SAMEP" );
    eff_HLT_R9_nvtx_seeded ->BayesDivide( PROBE_HLT_R9_nvtx_seeded, TAG_HLT_nvtx_seeded );
    eff_HLT_R9_nvtx_seeded->SetMinimum( 0.0 );
    eff_HLT_R9_nvtx_seeded->SetMaximum( 1.1 );
    eff_HLT_R9_nvtx_seeded->SetLineColor( 4 );
    eff_HLT_R9_nvtx_seeded->SetMarkerColor( 4 );

    eff_HLT_R9_nvtx_seeded->SetLineWidth( 2 );
    eff_HLT_R9_nvtx_seeded->SetMarkerStyle( 7 );
    eff_HLT_R9_nvtx_seeded->Draw( "SAMEP" );
    TLegend *l_eff_HLT_OR_nvtx_seeded = new TLegend( 0.45, 0.15, 0.55, 0.32 );
    l_eff_HLT_OR_nvtx_seeded->SetShadowColor( 0 );
    l_eff_HLT_OR_nvtx_seeded->SetFillColor( 0 );
    l_eff_HLT_OR_nvtx_seeded->SetLineColor( 0 );
    l_eff_HLT_OR_nvtx_seeded->SetTextSize( 0.03 );
    l_eff_HLT_OR_nvtx_seeded->AddEntry( eff_HLT_OR_nvtx_seeded, "HLT_Photon30_Iso60CaloId_OR_R9Id", "l" );
    l_eff_HLT_OR_nvtx_seeded->AddEntry( eff_HLT_Iso_nvtx_seeded, "HLT_Photon30_Iso60CaloId", "l" );
    l_eff_HLT_OR_nvtx_seeded->AddEntry( eff_HLT_R9_nvtx_seeded, "HLT_Photon30_R9Id", "l" );
    l_eff_HLT_OR_nvtx_seeded->SetTextSize( 0.03 );
    l_eff_HLT_OR_nvtx_seeded->Draw();

    //HLT unseeded nvtx efficiencies
    c_eff_HLT_OR_nvtx_unseeded->cd();
    eff_HLT_OR_nvtx_unseeded->BayesDivide( PROBE_HLT_OR_nvtx_unseeded, TAG_HLT_nvtx_unseeded );
    eff_HLT_OR_nvtx_unseeded->SetMinimum( 0.0 );
    eff_HLT_OR_nvtx_unseeded->SetMaximum( 1.1 );
    eff_HLT_OR_nvtx_unseeded->SetTitle( "RelVal Z->ee, #sqrt{s} = 13 TeV" );
    eff_HLT_OR_nvtx_unseeded->GetXaxis()->SetTitle( "nvtx " );
    eff_HLT_OR_nvtx_unseeded->GetYaxis()->SetTitle( "HLT-Only Efficiency" );
    eff_HLT_OR_nvtx_unseeded->SetLineColor( 1 );
    eff_HLT_OR_nvtx_unseeded->SetMarkerColor( 1 );

    eff_HLT_OR_nvtx_unseeded->SetLineWidth( 2 );
    eff_HLT_OR_nvtx_unseeded->SetMarkerStyle( 7 );
    eff_HLT_OR_nvtx_unseeded->Draw( "AP" );
    eff_HLT_Iso_nvtx_unseeded ->BayesDivide( PROBE_HLT_Iso_nvtx_unseeded, TAG_HLT_nvtx_unseeded );
    eff_HLT_Iso_nvtx_unseeded->SetMinimum( 0.0 );
    eff_HLT_Iso_nvtx_unseeded->SetMaximum( 1.1 );
    eff_HLT_Iso_nvtx_unseeded->SetLineColor( 2 );
    eff_HLT_Iso_nvtx_unseeded->SetMarkerColor( 2 );

    eff_HLT_Iso_nvtx_unseeded->SetLineWidth( 2 );
    eff_HLT_Iso_nvtx_unseeded->SetMarkerStyle( 7 );
    eff_HLT_Iso_nvtx_unseeded->Draw( "SAMEP" );
    eff_HLT_R9_nvtx_unseeded ->BayesDivide( PROBE_HLT_R9_nvtx_unseeded, TAG_HLT_nvtx_unseeded );
    eff_HLT_R9_nvtx_unseeded->SetMinimum( 0.0 );
    eff_HLT_R9_nvtx_unseeded->SetMaximum( 1.1 );
    eff_HLT_R9_nvtx_unseeded->SetLineColor( 4 );
    eff_HLT_R9_nvtx_unseeded->SetMarkerColor( 4 );

    eff_HLT_R9_nvtx_unseeded->SetLineWidth( 2 );
    eff_HLT_R9_nvtx_unseeded->SetMarkerStyle( 7 );
    eff_HLT_R9_nvtx_unseeded->Draw( "SAMEP" );
    TLegend *l_eff_HLT_OR_nvtx_unseeded = new TLegend( 0.45, 0.15, 0.55, 0.32 );
    l_eff_HLT_OR_nvtx_unseeded->SetShadowColor( 0 );
    l_eff_HLT_OR_nvtx_unseeded->SetFillColor( 0 );
    l_eff_HLT_OR_nvtx_unseeded->SetLineColor( 0 );
    l_eff_HLT_OR_nvtx_unseeded->SetTextSize( 0.03 );
    l_eff_HLT_OR_nvtx_unseeded->AddEntry( eff_HLT_OR_nvtx_unseeded, "HLT_Photon18_Iso60CaloId_OR_R9ID", "l" );
    l_eff_HLT_OR_nvtx_unseeded->AddEntry( eff_HLT_Iso_nvtx_unseeded, "HLT_Photon18_Iso60CaloId", "l" );
    l_eff_HLT_OR_nvtx_unseeded->AddEntry( eff_HLT_R9_nvtx_unseeded, "HLT_Photon18_R9Id", "l" );
    l_eff_HLT_OR_nvtx_unseeded->SetTextSize( 0.03 );
    l_eff_HLT_OR_nvtx_unseeded->Draw();



}
DEFINE_FWK_MODULE( HLTEfficiency );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

