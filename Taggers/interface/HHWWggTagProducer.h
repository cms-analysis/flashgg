#ifndef HHWWGGTAGPRODUCER_H_
#define HHWWGGTAGPRODUCER_H_
#include "stdlib.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/SinglePhotonView.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "flashgg/DataFormats/interface/Electron.h"
#include "flashgg/DataFormats/interface/Muon.h"
#include "flashgg/DataFormats/interface/Met.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "flashgg/DataFormats/interface/DiPhotonMVAResult.h"
#include "flashgg/DataFormats/interface/HHWWggTag.h"
#include "flashgg/DataFormats/interface/TagTruthBase.h"
#include "DataFormats/Common/interface/RefToPtr.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "flashgg/MicroAOD/interface/CutBasedDiPhotonObjectSelector.h"
#include "SimDataFormats/HTXS/interface/HiggsTemplateCrossSections.h"
#include "flashgg/Taggers/interface/LeptonSelection.h"

#include <vector>
#include <algorithm>
#include "TGraph.h"
#include "TLorentzVector.h"

using namespace std;
using namespace edm;


namespace flashgg {

class HHWWggTagProducer : public EDProducer
{
public:
    //---typedef
    typedef math::XYZTLorentzVector LorentzVector;
    
    //---ctors
    HHWWggTagProducer( const ParameterSet & );
    
    //---Outtree
    edm::Service<TFileService> fs;
    
private:
    // bool checkPassMVAs(const flashgg::Photon*& leading_photon, const flashgg::Photon*& subleading_photon, edm::Ptr<reco::Vertex>& diphoton_vertex, double EB_Photon_MVA_Threshold, double EE_Photon_MVA_Threshold);
    std::vector<double> GetMuonVars(const std::vector<edm::Ptr<flashgg::Muon> > &muonPointers, const std::vector<edm::Ptr<reco::Vertex> > &vertexPointers);
    std::vector<double> GetJetVars(const std::vector<edm::Ptr<flashgg::Jet> > &jetPointers, const edm::Ptr<flashgg::DiPhotonCandidate> dipho);
    vector<Ptr<flashgg::Jet>> GetFHPtOrderedJets(bool doHHWWggDebug_, std::vector<edm::Ptr<Jet> > tagJets_);
    vector<Ptr<flashgg::Jet>> GetFHminWHJets(bool doHHWWggDebug_, std::vector<edm::Ptr<Jet> > tagJets_);
    vector<Ptr<flashgg::Jet>> GetFHminWHLead2Jet(bool doHHWWggDebug_, std::vector<edm::Ptr<Jet> > tagJets_);
    vector<Ptr<flashgg::Jet>> GetFHminHiggsMassOnly(bool doHHWWggDebug_, std::vector<edm::Ptr<Jet> > tagJets_);
    int GetNumFLDR(std::vector<edm::Ptr<flashgg::Electron> >, std::vector<edm::Ptr<flashgg::Muon> >, double);
    static bool compEle(const edm::Ptr<flashgg::Electron>& a, const edm::Ptr<flashgg::Electron>& b);
    static bool compMu(const edm::Ptr<flashgg::Muon>& a, const edm::Ptr<flashgg::Muon>& b);
    // template <class flashggPtr>
    // static bool compPt(flashggPtr, flashggPtr);
    static bool compPt(edm::Ptr<reco::GenParticle>, edm::Ptr<reco::GenParticle>);
    float getGenCosThetaStar_CS(TLorentzVector h1, TLorentzVector h2);
    template <class flashggPtr>
    void PrintScaleFactorsPtr(flashggPtr);
    template <class flashggObj>
    void PrintScaleFactorsObj(flashggObj);
    HHWWggTag SetCentralUpDownWeights(HHWWggTag, std::vector<edm::Ptr<flashgg::Electron> >, std::vector<edm::Ptr<flashgg::Muon> >, 
                                      std::vector<edm::Ptr<flashgg::Jet> >, edm::Ptr<flashgg::DiPhotonCandidate>, bool, std::string, double, string);
    double PassPUJetID(vector<double>, vector<double>, Ptr<flashgg::Jet>);    
    HHWWggTag ComputePUJetIDs(vector<Ptr<flashgg::Jet>>, HHWWggTag);
    std::vector<edm::Ptr<reco::GenParticle> > OrderParticles(std::vector<edm::Ptr<reco::GenParticle> >);
    
    void produce( Event &, const EventSetup & ) override;
    std::vector<edm::EDGetTokenT<edm::View<DiPhotonCandidate> > > diPhotonTokens_;
    std::string inputDiPhotonName_;
    
    std::vector<edm::EDGetTokenT<edm::View<flashgg::Jet> > > jetTokens_;
    
    EDGetTokenT<View<DiPhotonCandidate> > diphotonToken_;
    Handle<View<flashgg::DiPhotonCandidate> > diphotons;
    
    EDGetTokenT<View<reco::Vertex> > vertexToken_;
    Handle<View<reco::Vertex> > vertex;
    
    EDGetTokenT<View<reco::GenParticle> > genParticleToken_;
    Handle<View<reco::GenParticle> > genParticle;
    
    EDGetTokenT<View<Electron> > electronToken_;
    Handle<View<flashgg::Electron> > electrons;
    
    EDGetTokenT<View<Muon> > muonToken_;
    Handle<View<flashgg::Muon> > muons;
    
    EDGetTokenT<View<flashgg::Met> > METToken_;
    Handle<View<flashgg::Met> > METs;
    
    EDGetTokenT<View<DiPhotonMVAResult> > mvaResultToken_;
    Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;
    
    Handle<View<reco::Vertex> > vertices;

    // EDGetTokenT<View<reco::GenParticle> > genParticleToken_;
    // Handle<View<reco::GenParticle> > genParticle;    
    
    string systLabel_;
    
    std::vector< std::string > systematicsLabels;
    
    string JetIDLevel_;
    bool applyPUJetID_; 
    std::vector<double> pujid_wp_pt_bin_1_;
    std::vector<double> pujid_wp_pt_bin_2_;    
    
    //---ID selector
    ConsumesCollector cc_;
    
    //----output collection
    double FL_METThreshold_;
    double deltaRLeps_;
    double leptonPtThreshold_;
    double muonEtaThreshold_;
    double leadPhoOverMassThreshold_;
    double subleadPhoOverMassThreshold_;
    double MVAThreshold_;
    double deltaRMuonPhoThreshold_;
    double jetPtThreshold_;
    double jetEtaThreshold_;
    double muPFIsoSumRelThreshold_;
    double deltaRJetMuonThreshold_;
    double deltaRPhoLeadJet_;
    double deltaRPhoSubLeadJet_;
    
    double DeltaRTrkElec_;
    
    double deltaRPhoElectronThreshold_;
    double deltaMassElectronZThreshold_;
    double DiLepMassHigThre_;
    double DiLepMassLowThre_;
    double FL_Dipho_pT_Thre_;
    double FH_Dipho_pT_Thre_;
    bool hasGoodElec = false;
    bool hasGoodMuons = false;
    double diPho_pT = -99; 
    
    vector<double> nonTrigMVAThresholds_;
    vector<double> nonTrigMVAEtaCuts_;
    
    vector<double> electronEtaThresholds_;
    string ElectronID_;
    string MuonID_;
    double btagThresh_;
    vector<string> BTagTypes_;
    bool doHHWWggTagCutFlowAnalysis_;
    bool doHHWWggFHptOrdered_;
    bool doHHWWggFHminWHJets_;
    bool doHHWWggFHminWHLead2Jet_;
    bool doHHWWggFHminHiggsMassOnly_;
    bool doHHWWggDebug_;
    string HHWWggAnalysisChannel_;
    bool FillUntagged_; 
    
    //full-lep
    bool SaveOthers_;
    bool Savejet;
    double lep1ptThre_;
    double lep2ptThre_;
    double lep3ptThre_;
    double DiLepPtThre_;
    double MassTThre_;
    double MassT_l2Thre_;
    double dipho_MVA;
    edm::InputTag genInfo_;
    edm::EDGetTokenT<GenEventInfoProduct> genInfoToken_;
    
    bool HHWWgguseZeroVtx_;
};

//---standard
HHWWggTagProducer::HHWWggTagProducer( const ParameterSet & pSet):
diphotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( pSet.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
vertexToken_( consumes<View<reco::Vertex> >( pSet.getParameter<InputTag> ( "VertexTag" ) ) ),
genParticleToken_( consumes<View<reco::GenParticle> >( pSet.getParameter<InputTag> ( "GenParticleTag" ) ) ),
electronToken_( consumes<View<flashgg::Electron> >( pSet.getParameter<InputTag> ( "ElectronTag" ) ) ),
muonToken_( consumes<View<flashgg::Muon> >( pSet.getParameter<InputTag> ( "MuonTag" ) ) ),
METToken_( consumes<View<Met> >( pSet.getParameter<InputTag> ( "METTag" ) ) ),
mvaResultToken_( consumes<View<flashgg::DiPhotonMVAResult> >( pSet.getParameter<InputTag> ( "MVAResultTag" ) ) ),
systLabel_( pSet.getParameter<string> ( "SystLabel" ) ),
JetIDLevel_( pSet.getParameter<string> ( "JetIDLevel" ) ),
applyPUJetID_( pSet.getParameter<bool> ( "applyPUJetID" )),
pujid_wp_pt_bin_1_( pSet.getParameter<std::vector<double> > ( "pujidWpPtBin1" ) ),
pujid_wp_pt_bin_2_( pSet.getParameter<std::vector<double> > ( "pujidWpPtBin2" ) ),
cc_( consumesCollector() )

{
    inputDiPhotonName_= pSet.getParameter<std::string > ( "DiPhotonName" );
    
    auto jetTags = pSet.getParameter<std::vector<edm::InputTag> > ( "JetTags" );
    for( auto & tag : jetTags ) { jetTokens_.push_back( consumes<edm::View<flashgg::Jet> >( tag ) ); }
    
    genInfo_ = pSet.getUntrackedParameter<edm::InputTag>( "genInfo", edm::InputTag("generator") );
    genInfoToken_ = consumes<GenEventInfoProduct>( genInfo_ );
    
    FL_METThreshold_ =pSet.getParameter<double>( "FL_METThreshold");
    deltaRLeps_ =pSet.getParameter<double>( "deltaRLeps");
    leptonPtThreshold_ = pSet.getParameter<double>( "leptonPtThreshold");
    muonEtaThreshold_ = pSet.getParameter<double>( "muonEtaThreshold");
    leadPhoOverMassThreshold_ = pSet.getParameter<double>( "leadPhoOverMassThreshold");
    subleadPhoOverMassThreshold_ = pSet.getParameter<double>( "subleadPhoOverMassThreshold");
    MVAThreshold_ = pSet.getParameter<double>( "MVAThreshold");
    deltaRMuonPhoThreshold_ = pSet.getParameter<double>( "deltaRMuonPhoThreshold");
    jetPtThreshold_ = pSet.getParameter<double>( "jetPtThreshold");
    jetEtaThreshold_ = pSet.getParameter<double>( "jetEtaThreshold");
    muPFIsoSumRelThreshold_ = pSet.getParameter<double>( "muPFIsoSumRelThreshold");
    deltaRJetMuonThreshold_ = pSet.getParameter<double>( "deltaRJetMuonThreshold");
    deltaRPhoLeadJet_ = pSet.getParameter<double>( "deltaRPhoLeadJet");
    deltaRPhoSubLeadJet_ = pSet.getParameter<double>( "deltaRPhoSubLeadJet");
    
    DeltaRTrkElec_ = pSet.getParameter<double>( "DeltaRTrkElec");
    
    deltaRPhoElectronThreshold_ = pSet.getParameter<double>( "deltaRPhoElectronThreshold");
    deltaMassElectronZThreshold_ = pSet.getParameter<double>( "deltaMassElectronZThreshold");
    nonTrigMVAThresholds_ =  pSet.getParameter<vector<double > >( "nonTrigMVAThresholds");
    nonTrigMVAEtaCuts_ =  pSet.getParameter<vector<double > >( "nonTrigMVAEtaCuts");
    electronEtaThresholds_ = pSet.getParameter<vector<double > >( "electronEtaThresholds");
    ElectronID_ = pSet.getParameter<string>("ElectronID");
    MuonID_ = pSet.getParameter<string>("MuonID");
    btagThresh_ = pSet.getParameter<double>( "btagThresh");
    BTagTypes_ = pSet.getParameter<vector<string>>( "BTagTypes" );
    doHHWWggTagCutFlowAnalysis_ = pSet.getParameter<bool>( "doHHWWggTagCutFlowAnalysis");
    doHHWWggFHptOrdered_ = pSet.getParameter<bool>( "doHHWWggFHptOrdered" );
    doHHWWggFHminWHJets_ = pSet.getParameter<bool>( "doHHWWggFHminWHJets" );
    doHHWWggFHminWHLead2Jet_ = pSet.getParameter<bool>( "doHHWWggFHminWHLead2Jet" );
    doHHWWggFHminHiggsMassOnly_ = pSet.getParameter<bool>( "doHHWWggFHminHiggsMassOnly" );
    doHHWWggDebug_ = pSet.getParameter<bool>( "doHHWWggDebug" );
    HHWWggAnalysisChannel_ = pSet.getParameter<string>( "HHWWggAnalysisChannel" );
    FillUntagged_ = pSet.getParameter<bool>( "FillUntagged" );
    lep1ptThre_ = pSet.getParameter<double>("lep1ptThre");//means > lep1pt
    lep2ptThre_ = pSet.getParameter<double>("lep2ptThre");//means > lep2pt
    lep3ptThre_ = pSet.getParameter<double>("lep3ptThre");//means < lep3pt
    DiLepPtThre_ = pSet.getParameter<double>("DiLepPtThre");
    DiLepMassHigThre_ = pSet.getParameter<double>("DiLepMassHigThre");
    DiLepMassLowThre_ = pSet.getParameter<double>("DiLepMassLowThre");
    FL_Dipho_pT_Thre_ = pSet.getParameter<double>("FL_Dipho_pT_Thre");
    FH_Dipho_pT_Thre_ = pSet.getParameter<double>("FH_Dipho_pT_Thre");
    MassTThre_ = pSet.getParameter<double>("MassTThre");
    MassT_l2Thre_ = pSet.getParameter<double>("MassT_l2Thre");
    HHWWgguseZeroVtx_ = pSet.getParameter<bool>("HHWWgguseZeroVtx");
    produces<vector<HHWWggTag>>();
    produces<vector<TagTruthBase>>();
}

bool HHWWggTagProducer::compEle(const edm::Ptr<flashgg::Electron>& a, const edm::Ptr<flashgg::Electron>& b)
{
    return a->pt() > b->pt();
}

bool HHWWggTagProducer::compMu(const edm::Ptr<flashgg::Muon>& a, const edm::Ptr<flashgg::Muon>& b)
{
    return a->pt() > b->pt();
}

// template<typename flashggPtr> 
bool HHWWggTagProducer::compPt(edm::Ptr<reco::GenParticle> Particle_a, edm::Ptr<reco::GenParticle> Particle_b)
{
    auto FourVec_a = Particle_a->p4();
    auto FourVec_b = Particle_b->p4();

    return FourVec_a.pt() > FourVec_b.pt();
}

// bool HHWWggTagProducer::checkPassMVAs( const flashgg::Photon*& leading_photon, const flashgg::Photon*& subleading_photon, edm::Ptr<reco::Vertex>& diphoton_vertex, double EB_Photon_MVA_Threshold, double EE_Photon_MVA_Threshold){
    
//     // MVA Check variables
//     bool lead_pass_TightPhoID = 0, sublead_pass_TightPhoID = 0;
//     double lp_Hgg_MVA = -99, slp_Hgg_MVA = -99;
//     double leading_pho_eta = -99, sub_leading_pho_eta = -99;
    
//     // Get MVA values wrt diphoton vertex
//     lp_Hgg_MVA = leading_photon->phoIdMvaDWrtVtx( diphoton_vertex );
//     slp_Hgg_MVA = subleading_photon->phoIdMvaDWrtVtx( diphoton_vertex );
    
//     // Get eta values
//     leading_pho_eta = leading_photon->p4().eta();
//     sub_leading_pho_eta = subleading_photon->p4().eta();
    
//     // leading photon
//     // EB
//     if (( abs(leading_pho_eta) > 0) && ( abs(leading_pho_eta) < 1.4442)){
//         if (lp_Hgg_MVA > EB_Photon_MVA_Threshold) lead_pass_TightPhoID = 1;
//     }
    
//     // EE
//     else if (( abs(leading_pho_eta) > 1.566) && ( abs(leading_pho_eta) < 2.5)){
//         if (lp_Hgg_MVA > EE_Photon_MVA_Threshold) lead_pass_TightPhoID = 1;
//     }
    
//     // SubLeading Photon
//     // EB
//     if (( abs(sub_leading_pho_eta) > 0) && ( abs(sub_leading_pho_eta) < 1.4442)){
//         if (slp_Hgg_MVA > EB_Photon_MVA_Threshold) sublead_pass_TightPhoID = 1;
//     }
    
//     // EE
//     else if (( abs(sub_leading_pho_eta) > 1.566) && ( abs(sub_leading_pho_eta) < 2.5)){
//         if (slp_Hgg_MVA > EE_Photon_MVA_Threshold) sublead_pass_TightPhoID = 1;
//     }
    
//     if (lead_pass_TightPhoID && sublead_pass_TightPhoID){
//         return 1;
//     }
    
//     else{
//         return 0;
//     }
    
// }

// FIXME: Need to change to a vector of vectors
std::vector<double> HHWWggTagProducer::GetMuonVars(const std::vector<edm::Ptr<flashgg::Muon> > &muonPointers, const std::vector<edm::Ptr<reco::Vertex> > &vertexPointers)
{
    unsigned int maxMuons = 5; // Shouldn't need more than this
    unsigned int numVars = 6; // 5 IDs + isolation
    unsigned int numVecEntries = maxMuons * numVars;
    std::vector<double> MuonVars_(numVecEntries,-99); // initialize vector with -99 vals
    double isLooseMuon = -99, isMediumMuon = -99, isTightMuon = -99, isSoftMuon = -99, isHighPtMuon = -99;
    double muonIso = -99;
    int vtxInd = 0;
    double dzmin = 9999;
    
    for( unsigned int muonIndex = 0; muonIndex < muonPointers.size(); muonIndex++ ) {
        if(muonIndex >= maxMuons) continue; // only save info from 5 highest pT muon objects
        isLooseMuon = -99, isMediumMuon = -99, isTightMuon = -99, isSoftMuon = -99, isHighPtMuon = -99;
        muonIso = -99;
        Ptr<flashgg::Muon> muon = muonPointers[muonIndex];
        vtxInd = 0;
        dzmin = 9999;
        // If no innertrack, set medium, tight, soft, highpt vals to -99 as they can't be calculated without it
        // I think this is correct because there are non-zero isolation values when isTightMuon is -99
        if( !muon->innerTrack() ){
            isLooseMuon = muon::isLooseMuon( *muon );
            muonIso = ( muon->pfIsolationR04().sumChargedHadronPt
                       + max( 0.,muon->pfIsolationR04().sumNeutralHadronEt
                             + muon->pfIsolationR04().sumPhotonEt - 0.5 * muon->pfIsolationR04().sumPUPt ) ) / ( muon->pt() );
            MuonVars_[muonIndex*numVars + 0] = isLooseMuon;
            MuonVars_[muonIndex*numVars + 1] = isMediumMuon;
            MuonVars_[muonIndex*numVars + 2] = isTightMuon;
            MuonVars_[muonIndex*numVars + 3] = isSoftMuon;
            MuonVars_[muonIndex*numVars + 4] = isHighPtMuon;
            MuonVars_[muonIndex*numVars + 5] = muonIso;
            continue;
        }
        for( size_t ivtx = 0 ; ivtx < vertexPointers.size(); ivtx++ ) {
            Ptr<reco::Vertex> vtx = vertexPointers[ivtx];
            if( fabs( muon->innerTrack()->vz() - vtx->position().z() ) < dzmin ) {
                dzmin = fabs( muon->innerTrack()->vz() - vtx->position().z() );
                vtxInd = ivtx;
            }
        }
        Ptr<reco::Vertex> best_vtx = vertexPointers[vtxInd];
        isLooseMuon = muon::isLooseMuon( *muon );
        isMediumMuon = muon::isMediumMuon( *muon );
        isTightMuon = muon::isTightMuon( *muon, *best_vtx );
        isSoftMuon = muon::isSoftMuon( *muon, *best_vtx );
        isHighPtMuon = muon::isHighPtMuon( *muon, *best_vtx );
        
        muonIso = ( muon->pfIsolationR04().sumChargedHadronPt
                   + max( 0.,muon->pfIsolationR04().sumNeutralHadronEt
                         + muon->pfIsolationR04().sumPhotonEt - 0.5 * muon->pfIsolationR04().sumPUPt ) ) / ( muon->pt() );
        
        MuonVars_[muonIndex*numVars + 0] = isLooseMuon;
        MuonVars_[muonIndex*numVars + 1] = isMediumMuon;
        MuonVars_[muonIndex*numVars + 2] = isTightMuon;
        MuonVars_[muonIndex*numVars + 3] = isSoftMuon;
        MuonVars_[muonIndex*numVars + 4] = isHighPtMuon;
        MuonVars_[muonIndex*numVars + 5] = muonIso;
        
        // if(isLooseMuon==0 && isTightMuon==1){
        //   cout << "--------------------------------" << endl;
        //   cout << "Muon passes tight but not loose" << endl;
        //   cout << "muon->isPFMuon():" << muon->isPFMuon() << endl;
        //   cout << "muon->isGlobalMuon():" << muon->isGlobalMuon() << endl;
        //   cout << "muon->isTrackerMuon():" << muon->isTrackerMuon() << endl;
        // }
    }
    
    return MuonVars_;
}

std::vector<double> HHWWggTagProducer::GetJetVars(const std::vector<edm::Ptr<flashgg::Jet> > &jetPointers, const edm::Ptr<flashgg::DiPhotonCandidate> dipho)
{
    unsigned int maxJets = 5; // Max number of jets to store information for
    unsigned int numVars = 4; // 4 IDs + Jet PU ID
    // unsigned int numVars = 5; // 4 IDs + Jet PU ID
    // unsigned int numVars = 12; // 4 IDs + 8 PUjetIDs
    unsigned int numVecEntries = maxJets * numVars;
    std::vector<double> JetVars_(numVecEntries,-99); // initialize vector with -99 vals
    double passLoose = -99, passTight = -99, passTight2017 = -99, passTight2018 = -99;
    // double passesJetPUIdLoose = -99;
    // double passesJetPuIdnone = -99, passesJetPuIdloose = -99, passesJetPuIdmedium = -99, passesJetPuIdtight = -99;
    // double passesJetPuIdmixed = -99, passesJetPuIdforward_loose = -99, passesJetPuIdforward_medium = -99, passesJetPuIdforward_tight = -99;
    
    for( unsigned int jetIndex = 0; jetIndex < jetPointers.size(); jetIndex++ ) {
        if(jetIndex >= maxJets) continue; // only save info from 5 highest pT Jet objects
        passLoose = -99, passTight = -99, passTight2017 = -99, passTight2018 = -99;
        Ptr<flashgg::Jet> jet = jetPointers[jetIndex];
        
        // passesJetPuId
        // one=0, loose=1, medium=2, tight=3, mixed=4, forward_loose=5, forward_medium=6, forward_tight=7
        
        // Loose=0, Tight=1, Tight2017=2, Tight2018=3
        passLoose = jet->passesJetID  ( flashgg::Loose );
        passTight = jet->passesJetID  ( flashgg::Tight );
        passTight2017 = jet->passesJetID  ( flashgg::Tight2017 );
        passTight2018 = jet->passesJetID  ( flashgg::Tight2018 );
        
        // passesJetPUIdLoose = jet->passesPuJetId(dipho, PileupJetIdentifier::kLoose);
        
        // passesJetPuIdnone = jet->passesPuJetId  ( flashgg::none );
        // passesJetPuIdloose = jet->passesPuJetId  ( flashgg::loose );
        // passesJetPuIdmedium = jet->passesPuJetId  ( flashgg::medium );
        // passesJetPuIdtight = jet->passesPuJetId  ( flashgg::tight );
        // passesJetPuIdmixed = jet->passesPuJetId  ( flashgg::mixed );
        // passesJetPuIdforward_loose = jet->passesPuJetId  ( flashgg::forward_loose );
        // passesJetPuIdforward_medium = jet->passesPuJetId  ( flashgg::forward_medium );
        // passesJetPuIdforward_tight = jet->passesPuJetId  ( flashgg::forward_tight );
        
        JetVars_[jetIndex*numVars + 0] = passLoose;
        JetVars_[jetIndex*numVars + 1] = passTight;
        JetVars_[jetIndex*numVars + 2] = passTight2017;
        JetVars_[jetIndex*numVars + 3] = passTight2018;
        // JetVars_[jetIndex*numVars + 4] = passesJetPUIdLoose;
        
        // JetVars_[jetIndex*numVars + 4] = passesJetPuIdnone;
        // JetVars_[jetIndex*numVars + 5] = passesJetPuIdloose;
        // JetVars_[jetIndex*numVars + 6] = passesJetPuIdmedium;
        // JetVars_[jetIndex*numVars + 7] = passesJetPuIdtight;
        // JetVars_[jetIndex*numVars + 8] = passesJetPuIdmixed;
        // JetVars_[jetIndex*numVars + 9] = passesJetPuIdforward_loose;
        // JetVars_[jetIndex*numVars + 10] = passesJetPuIdforward_medium;
        // JetVars_[jetIndex*numVars + 11] = passesJetPuIdforward_tight;
        
    }
    
    return JetVars_;
}

vector<Ptr<flashgg::Jet> > HHWWggTagProducer::GetFHminWHJets(bool doHHWWggDebug, std::vector<edm::Ptr<Jet> > tagJets_)
{
    // get 4 jets for FH final state with minWH vals
    std::vector<edm::Ptr<flashgg::Jet> > FHJets_;
    bool DEBUG = doHHWWggDebug;
    double TempMinWMass = 999999.0;
    double TempMinHMass = 999999.0;
    
    int OnShellW_LeadingJetIndex = -1;
    int OnShellW_SubLeadingJetIndex = -1;
    int OffShellW_LeadingJetIndex = -1;
    int OffShellW_SubLeadingJetIndex = -1;
    
    Ptr<flashgg::Jet> jet11;
    Ptr<flashgg::Jet> jet12;
    Ptr<flashgg::Jet> jet1;
    Ptr<flashgg::Jet> jet2;
    Ptr<flashgg::Jet> jet3;
    Ptr<flashgg::Jet> jet4;
    
    int nTagJets = tagJets_.size();
    
    // Select 2 jets whose mass closest to W-boson mass
    for (int CountJet1 = 0; CountJet1 < nTagJets-1; CountJet1++) {
        for (int CountJet2 = CountJet1+1; CountJet2 < nTagJets; CountJet2++) {
            if (DEBUG) std::cout << "\t(CountJet1,CountJet2) = (" << CountJet1 << "," << CountJet2 << ")" << std::endl;
            jet11 = tagJets_[CountJet1];
            jet12 = tagJets_[CountJet2];
            
            double deltaMass =  abs((jet11->p4() + jet12->p4()).M() - 80.0);
            if (DEBUG) std::cout << "\tdeltaMass = " << deltaMass << "\t TempMinWMass = " << TempMinWMass << std::endl;
            if ( deltaMass < TempMinWMass)
            {
                if  (jet11->p4().pt() > jet12->p4().pt()) {
                    OnShellW_LeadingJetIndex = CountJet1;
                    OnShellW_SubLeadingJetIndex = CountJet2;
                } else {
                    OnShellW_LeadingJetIndex = CountJet2;
                    OnShellW_SubLeadingJetIndex = CountJet1;
                }
                TempMinWMass = deltaMass;
                if (DEBUG) std::cout << "\t==> (CountJet1,CountJet2) = (" << CountJet1 << "," << CountJet2 << ")" << std::endl;
            }
        }
    }
    if (DEBUG) std::cout << "\t[INFO] Print Jet Index (After W-Selection): " << OnShellW_LeadingJetIndex << "\t" << OnShellW_SubLeadingJetIndex << "\t" << OffShellW_LeadingJetIndex << "\t" << OffShellW_SubLeadingJetIndex  << std::endl;
    
    for (int CountJet1 = 0; CountJet1 < nTagJets-1; CountJet1++) {
        if (CountJet1 == OnShellW_LeadingJetIndex || CountJet1 == OnShellW_SubLeadingJetIndex) continue;
        for (int CountJet2 = CountJet1+1; CountJet2 < nTagJets; CountJet2++) {
            if (CountJet2 == OnShellW_LeadingJetIndex || CountJet2 == OnShellW_SubLeadingJetIndex) continue;
            if (DEBUG) std::cout << "\t(CountJet1,CountJet2) = (" << CountJet1 << "," << CountJet2 << ")" << std::endl;
            jet11 = tagJets_[CountJet1];
            jet12 = tagJets_[CountJet2];
            
            double deltaMass =  abs((jet11->p4() + jet12->p4() + tagJets_[OnShellW_LeadingJetIndex]->p4() + tagJets_[OnShellW_SubLeadingJetIndex]->p4() ).M() - 125.0);
            if (DEBUG) std::cout << "\tdeltaMass = " << deltaMass << "\t TempMinHMass = " << TempMinHMass << std::endl;
            if ( deltaMass < TempMinHMass)
            {
                if  (jet11->p4().pt() > jet12->p4().pt()) {
                    OffShellW_LeadingJetIndex = CountJet1;
                    OffShellW_SubLeadingJetIndex = CountJet2;
                } else {
                    OffShellW_LeadingJetIndex = CountJet2;
                    OffShellW_SubLeadingJetIndex = CountJet1;
                }
                TempMinHMass = deltaMass;
                if (DEBUG) std::cout << "\t==> (CountJet1,CountJet2) = (" << CountJet1 << "," << CountJet2 << ")" << std::endl;
            }
        }
    }
    jet1 = tagJets_[OnShellW_LeadingJetIndex];
    jet2 = tagJets_[OnShellW_SubLeadingJetIndex];
    jet3 = tagJets_[OffShellW_LeadingJetIndex];
    jet4 = tagJets_[OffShellW_SubLeadingJetIndex];
    
    if (DEBUG) std::cout << "\t[INFO] Print pt of 4 selected jets: " << OnShellW_LeadingJetIndex << "\t" << OnShellW_SubLeadingJetIndex << "\t" << OffShellW_LeadingJetIndex << "\t" << OffShellW_SubLeadingJetIndex  << std::endl;
    if (DEBUG) std::cout << "\t[INFO] jet1 pT = " << jet1->p4().pt() << std::endl;
    if (DEBUG) std::cout << "\t[INFO] jet2 pT = " << jet2->p4().pt() << std::endl;
    if (DEBUG) std::cout << "\t[INFO] jet3 pT = " << jet3->p4().pt() << std::endl;
    if (DEBUG) std::cout << "\t[INFO] jet4 pT = " << jet4->p4().pt() << std::endl;
    
    FHJets_.push_back(jet1);
    FHJets_.push_back(jet2);
    FHJets_.push_back(jet3);
    FHJets_.push_back(jet4);
    
    return FHJets_;
}

vector<Ptr<flashgg::Jet> > HHWWggTagProducer::GetFHminWHLead2Jet(bool doHHWWggDebug, std::vector<edm::Ptr<Jet> > tagJets_)
{
    // get 4 jets for FH final state with minWH vals
    std::vector<edm::Ptr<flashgg::Jet> > FHJets_;
    bool DEBUG = doHHWWggDebug;
    double TempMinWMass = 999999.0;
    
    int OnShellW_LeadingJetIndex = -1;
    int OnShellW_SubLeadingJetIndex = -1;
    int OffShellW_LeadingJetIndex = -1;
    int OffShellW_SubLeadingJetIndex = -1;
    
    Ptr<flashgg::Jet> jet11;
    Ptr<flashgg::Jet> jet12;
    Ptr<flashgg::Jet> jet1;
    Ptr<flashgg::Jet> jet2;
    Ptr<flashgg::Jet> jet3;
    Ptr<flashgg::Jet> jet4;
    
    int nTagJets = tagJets_.size();
    if (DEBUG) std::cout << "\tSize of jets: " << nTagJets << std::endl;
    
    // Select 2 jets whose mass closest to W-boson mass
    for (int CountJet1 = 0; CountJet1 < nTagJets-1; CountJet1++) {
        for (int CountJet2 = CountJet1+1; CountJet2 < nTagJets; CountJet2++) {
            if (DEBUG) std::cout << "\t(CountJet1,CountJet2) = (" << CountJet1 << "," << CountJet2 << ")" << std::endl;
            jet11 = tagJets_[CountJet1];
            jet12 = tagJets_[CountJet2];
            
            double deltaMass =  abs((jet11->p4() + jet12->p4()).M() - 80.0);
            if (DEBUG) std::cout << "\tdeltaMass = " << deltaMass << "\t TempMinWMass = " << TempMinWMass << std::endl;
            if ( deltaMass < TempMinWMass)
            {
                if  (jet11->p4().pt() > jet12->p4().pt()) {
                    OnShellW_LeadingJetIndex = CountJet1;
                    OnShellW_SubLeadingJetIndex = CountJet2;
                } else {
                    OnShellW_LeadingJetIndex = CountJet2;
                    OnShellW_SubLeadingJetIndex = CountJet1;
                }
                TempMinWMass = deltaMass;
                if (DEBUG) std::cout << "\t==> (CountJet1,CountJet2) = (" << CountJet1 << "," << CountJet2 << ")" << std::endl;
            }
        }
    }
    if (DEBUG) std::cout << "\t[INFO] Print Jet Index (After W-Selection): " << OnShellW_LeadingJetIndex << "\t" << OnShellW_SubLeadingJetIndex << "\t" << OffShellW_LeadingJetIndex << "\t" << OffShellW_SubLeadingJetIndex  << std::endl;
    
    // 2 Off-shell w-boson jets; out of remaning jets select leading 2 jets.
    // Here, I am selecting first two jets after excluding already selected jets.
    int TempJetCounter = 0;
    for (int CountJet1 = 0; CountJet1 < nTagJets; CountJet1++) {
        if (CountJet1 == OnShellW_LeadingJetIndex || CountJet1 == OnShellW_SubLeadingJetIndex) continue;
        
        jet11 = tagJets_[CountJet1];
        if (DEBUG) std::cout << "\t(CountJet1) = (" << CountJet1 << "), pT(jet): " << jet11->p4().pt() << ",\tTempJetCounter: " << TempJetCounter << std::endl;
        
        if (TempJetCounter==0) OffShellW_LeadingJetIndex = CountJet1;
        if (TempJetCounter==1) OffShellW_SubLeadingJetIndex = CountJet1;
        
        TempJetCounter +=1;
        if (TempJetCounter>1) break;
    }
    jet1 = tagJets_[OnShellW_LeadingJetIndex];
    jet2 = tagJets_[OnShellW_SubLeadingJetIndex];
    jet3 = tagJets_[OffShellW_LeadingJetIndex];
    jet4 = tagJets_[OffShellW_SubLeadingJetIndex];
    
    if (DEBUG) std::cout << "\t[INFO] Print pt of 4 selected jets: " << OnShellW_LeadingJetIndex << "\t" << OnShellW_SubLeadingJetIndex << "\t" << OffShellW_LeadingJetIndex << "\t" << OffShellW_SubLeadingJetIndex  << std::endl;
    if (DEBUG) std::cout << "\t[INFO] jet1 pT = " << jet1->p4().pt() << std::endl;
    if (DEBUG) std::cout << "\t[INFO] jet2 pT = " << jet2->p4().pt() << std::endl;
    if (DEBUG) std::cout << "\t[INFO] jet3 pT = " << jet3->p4().pt() << std::endl;
    if (DEBUG) std::cout << "\t[INFO] jet4 pT = " << jet4->p4().pt() << std::endl;
    
    FHJets_.push_back(jet1);
    FHJets_.push_back(jet2);
    FHJets_.push_back(jet3);
    FHJets_.push_back(jet4);
    
    return FHJets_;
}

vector<Ptr<flashgg::Jet> > HHWWggTagProducer::GetFHminHiggsMassOnly(bool doHHWWggDebug, std::vector<edm::Ptr<Jet> > tagJets_)
{
    // get 4 jets for FH final state with minWH vals
    std::vector<edm::Ptr<flashgg::Jet> > FHJets_;
    bool DEBUG = doHHWWggDebug;
    double TempMinWMass = 999999.0;
    
    int OnShellW_LeadingJetIndex = -1;
    int OnShellW_SubLeadingJetIndex = -1;
    int OffShellW_LeadingJetIndex = -1;
    int OffShellW_SubLeadingJetIndex = -1;
    
    int TempJet[4] = {-1,-1,-1,-1};
    
    Ptr<flashgg::Jet> jet11;
    Ptr<flashgg::Jet> jet12;
    Ptr<flashgg::Jet> jet13;
    Ptr<flashgg::Jet> jet14;
    Ptr<flashgg::Jet> jet1;
    Ptr<flashgg::Jet> jet2;
    Ptr<flashgg::Jet> jet3;
    Ptr<flashgg::Jet> jet4;
    
    int nTagJets = tagJets_.size();
    
    // Select 2 jets whose mass closest to W-boson mass
    for (int CountJet1 = 0; CountJet1 < nTagJets-3; CountJet1++) {
        for (int CountJet2 = CountJet1+1; CountJet2 < nTagJets-2; CountJet2++) {
            for (int CountJet3 = CountJet2+1; CountJet3 < nTagJets-1; CountJet3++) {
                for (int CountJet4 = CountJet3+1; CountJet4 < nTagJets; CountJet4++) {
                    if (DEBUG) std::cout << "\t(CountJet1,CountJet2,CountJet3,CountJet4) = (" << CountJet1 << "," << CountJet2 <<"," << CountJet3 <<"," << CountJet4 << ")" << std::endl;
                    jet11 = tagJets_[CountJet1];
                    jet12 = tagJets_[CountJet2];
                    jet13 = tagJets_[CountJet3];
                    jet14 = tagJets_[CountJet4];
                    
                    double deltaMass =  abs((jet11->p4() + jet12->p4() + jet13->p4() + jet14->p4()).M() - 125.1);
                    if (DEBUG) std::cout << "\tdeltaMass = " << deltaMass << "\t TempMinWMass = " << TempMinWMass << std::endl;
                    if ( deltaMass < TempMinWMass)
                    {
                        TempJet[0] = CountJet1;
                        TempJet[1] = CountJet2;
                        TempJet[2] = CountJet3;
                        TempJet[3] = CountJet4;
                        TempMinWMass = deltaMass;
                        if (DEBUG) std::cout << "\t==> (CountJet1,CountJet2) = (" << CountJet1 << "," << CountJet2 << ")" << std::endl;
                    }
                }
            }
        }
    }
    if (DEBUG) std::cout << "\t[INFO] Print Jet Index (After W-Selection): " << TempJet[0] << "\t" << TempJet[1] << "\t" << TempJet[2] << "\t" << TempJet[3]  << std::endl;
    
    // 2 Off-shell w-boson jets; out of remaning jets select leading 2 jets.
    TempMinWMass = 9999.0;
    for (int i = 0; i < 3; ++i)
    {
        for (int j = i+1; i < 4; ++i)
        {
            jet11 = tagJets_[TempJet[i]];
            jet12 = tagJets_[TempJet[j]];
            double deltaMass = abs( (jet11->p4() + jet12->p4()).M() - 80.0);
            if (deltaMass < TempMinWMass)
            {
                OnShellW_LeadingJetIndex = TempJet[i];
                OnShellW_SubLeadingJetIndex = TempJet[j];
                TempMinWMass = deltaMass;
            }
        }
    }
    
    int TempJetCounter = 0;
    for (int i = 0; i < 4; ++i)
    {
        if (i == OnShellW_LeadingJetIndex || i == OnShellW_SubLeadingJetIndex) continue;
        TempJetCounter +=1;
        if (TempJetCounter == 1) OffShellW_LeadingJetIndex = i;
        if (TempJetCounter == 2) OffShellW_SubLeadingJetIndex = i;
    }
    
    jet1 = tagJets_[OnShellW_LeadingJetIndex];
    jet2 = tagJets_[OnShellW_SubLeadingJetIndex];
    jet3 = tagJets_[OffShellW_LeadingJetIndex];
    jet4 = tagJets_[OffShellW_SubLeadingJetIndex];
    
    if (DEBUG) std::cout << "\t[INFO] Print pt of 4 selected jets: " << OnShellW_LeadingJetIndex << "\t" << OnShellW_SubLeadingJetIndex << "\t" << OffShellW_LeadingJetIndex << "\t" << OffShellW_SubLeadingJetIndex  << std::endl;
    if (DEBUG) std::cout << "\t[INFO] jet1 pT = " << jet1->p4().pt() << std::endl;
    if (DEBUG) std::cout << "\t[INFO] jet2 pT = " << jet2->p4().pt() << std::endl;
    if (DEBUG) std::cout << "\t[INFO] jet3 pT = " << jet3->p4().pt() << std::endl;
    if (DEBUG) std::cout << "\t[INFO] jet4 pT = " << jet4->p4().pt() << std::endl;
    
    FHJets_.push_back(jet1);
    FHJets_.push_back(jet2);
    FHJets_.push_back(jet3);
    FHJets_.push_back(jet4);
    
    return FHJets_;
}


vector<Ptr<flashgg::Jet> > HHWWggTagProducer::GetFHPtOrderedJets(bool doHHWWggDebug, std::vector<edm::Ptr<Jet> > tagJets_)
{
    // get 4 jets for FH final state with minWH vals
    std::vector<edm::Ptr<flashgg::Jet> > FHJets_;
    bool DEBUG = doHHWWggDebug;
    double TempMinWMass = 999999.0;
    
    int OnShellW_LeadingJetIndex = -1;
    int OnShellW_SubLeadingJetIndex = -1;
    int OffShellW_LeadingJetIndex = -1;
    int OffShellW_SubLeadingJetIndex = -1;
    
    Ptr<flashgg::Jet> jet11;
    Ptr<flashgg::Jet> jet12;
    Ptr<flashgg::Jet> jet1;
    Ptr<flashgg::Jet> jet2;
    Ptr<flashgg::Jet> jet3;
    Ptr<flashgg::Jet> jet4;
    
    TempMinWMass = 9999.0;
    for (int i = 0; i < 3; ++i)
    {
        for (int j = i+1; i < 4; ++i)
        {
            jet11 = tagJets_[i];
            jet12 = tagJets_[j];
            double deltaMass = abs( (jet11->p4() + jet12->p4()).M() - 80.0);
            if (deltaMass < TempMinWMass)
            {
                OnShellW_LeadingJetIndex = i;
                OnShellW_SubLeadingJetIndex = j;
                TempMinWMass = deltaMass;
            }
        }
    }
    if (DEBUG) std::cout << "\t[INFO] Print pt of 2 selected jets: " << OnShellW_LeadingJetIndex << "\t" << OnShellW_SubLeadingJetIndex << "\t" << OffShellW_LeadingJetIndex << "\t" << OffShellW_SubLeadingJetIndex  << std::endl;
    
    int TempJetCounter = 0;
    for (int i = 0; i < 4; ++i)
    {
        if (i == OnShellW_LeadingJetIndex || i == OnShellW_SubLeadingJetIndex) continue;
        TempJetCounter +=1;
        if (TempJetCounter == 1) OffShellW_LeadingJetIndex = i;
        if (TempJetCounter == 2) OffShellW_SubLeadingJetIndex = i;
    }
    
    jet1 = tagJets_[OnShellW_LeadingJetIndex];
    jet2 = tagJets_[OnShellW_SubLeadingJetIndex];
    jet3 = tagJets_[OffShellW_LeadingJetIndex];
    jet4 = tagJets_[OffShellW_SubLeadingJetIndex];
    
    if (DEBUG) std::cout << "\t[INFO] Print pt of 4 selected jets: " << OnShellW_LeadingJetIndex << "\t" << OnShellW_SubLeadingJetIndex << "\t" << OffShellW_LeadingJetIndex << "\t" << OffShellW_SubLeadingJetIndex  << std::endl;
    if (DEBUG) std::cout << "\t[INFO] jet1 pT = " << jet1->p4().pt() << std::endl;
    if (DEBUG) std::cout << "\t[INFO] jet2 pT = " << jet2->p4().pt() << std::endl;
    if (DEBUG) std::cout << "\t[INFO] jet3 pT = " << jet3->p4().pt() << std::endl;
    if (DEBUG) std::cout << "\t[INFO] jet4 pT = " << jet4->p4().pt() << std::endl;
    
    FHJets_.push_back(jet1);
    FHJets_.push_back(jet2);
    FHJets_.push_back(jet3);
    FHJets_.push_back(jet4);
    
    return FHJets_;
}

// Compute number of pairs of leptons with a minimum delta R
int HHWWggTagProducer::GetNumFLDR(std::vector<edm::Ptr<flashgg::Electron> > goodElectrons_, std::vector<edm::Ptr<flashgg::Muon> > goodMuons_, double deltaRLeps)
{
    int num_FL_dr_ = 0;
    double dr_ll = 0;
    bool hasGoodElec = goodElectrons_.size() > 0;
    bool hasGoodMuons = goodMuons_.size() > 0;
    if (hasGoodElec && hasGoodMuons){
        for (unsigned int ei = 0; ei < goodElectrons_.size(); ei++){
            Ptr<flashgg::Electron> electron = goodElectrons_[ei];
            for (unsigned int mi = 0; mi < goodMuons_.size(); mi++){
                Ptr<flashgg::Muon> muon = goodMuons_[mi];
                dr_ll = deltaR(electron->eta(), electron->phi(), muon->eta(), muon->phi());
                if (dr_ll > deltaRLeps){
                    num_FL_dr_ += 1;
                }
            }
        }
    }
    
    else if(hasGoodElec && !hasGoodMuons){
        for (unsigned int ei = 0; ei < goodElectrons_.size() - 1; ei++){ // the last electron cannot be the first one in the dR calculation
            Ptr<flashgg::Electron> electroni = goodElectrons_[ei];
            for (unsigned int ej = ei + 1; ej < goodElectrons_.size(); ej++){
                Ptr<flashgg::Electron> electronj = goodElectrons_[ej];
                dr_ll = deltaR(electroni->eta(), electroni->phi(), electronj->eta(), electronj->phi());
                if (dr_ll > deltaRLeps){
                    num_FL_dr_ += 1;
                }
            }
        }
    }
    
    else if(!hasGoodElec && hasGoodMuons){
        for (unsigned int mi = 0; mi < goodMuons_.size() - 1; mi++){
            Ptr<flashgg::Muon> muoni = goodMuons_[mi];
            for (unsigned int mj = mi + 1; mj < goodMuons_.size(); mj++){
                Ptr<flashgg::Muon> muonj = goodMuons_[mj];
                dr_ll = deltaR(muoni->eta(), muoni->phi(), muonj->eta(), muonj->phi());
                if (dr_ll > deltaRLeps){
                    num_FL_dr_ += 1;
                }
            }
        }
    }
    return num_FL_dr_;
}

float HHWWggTagProducer::getGenCosThetaStar_CS(TLorentzVector h1, TLorentzVector h2)
{
    // cos theta star angle in the Collins Soper frame
    TLorentzVector hh = h1 + h2;
    h1.Boost(-hh.BoostVector());
    return h1.CosTheta();
}

template<typename flashggPtr> void HHWWggTagProducer::PrintScaleFactorsPtr(flashggPtr PtrToObject)
{
    for (auto it = PtrToObject->weightListBegin() ; it != PtrToObject->weightListEnd(); it++) {
        string key = *it;
        // if(key.find("Central")!=string::npos) std::cout << " Scale Factor: " << *it << " " << PtrToObject->weight(*it) << std::endl; // Central Weight only
        std::cout << " Scale Factor: " << *it << " " << PtrToObject->weight(*it) << std::endl;
    }
}

// If input weighted objected not a pointer, need to access weightList* methods with "." rather than "->"
template<typename flashggObj> void HHWWggTagProducer::PrintScaleFactorsObj(flashggObj Obj)
{
    for (auto it = Obj.weightListBegin() ; it != Obj.weightListEnd(); it++) {
        string key = *it;
        // if(key.find("Central")!=string::npos) std::cout << " Scale Factor: " << *it << " " << Obj.weight(*it) << std::endl; // Central Weight only
        std::cout << " Scale Factor: " << *it << " " << Obj.weight(*it) << std::endl;
    }
}

// Set Central object weights in output trees for each scale factor
HHWWggTag HHWWggTagProducer::SetCentralUpDownWeights(HHWWggTag tag_obj_, std::vector<edm::Ptr<flashgg::Electron> > goodElectrons, std::vector<edm::Ptr<flashgg::Muon> > goodMuons,
                                                     std::vector<edm::Ptr<flashgg::Jet> > tagJets, edm::Ptr<flashgg::DiPhotonCandidate> dipho, bool doHHWWggDebug_, 
                                                     string MuonID_, double muPFIsoSumRelThreshold_, string Tag_
                                                     )
{
    
    // //-- If debugging, print object weights 
    // if(doHHWWggDebug_){

    //     // Diphoton
    //     cout << "*********************************************************" << endl; 
    //     cout << "Diphoton Scale Factors:" << endl; 
    //     PrintScaleFactorsObj(*dipho);

    //     // Jets
    //     for (unsigned int i = 0; i < tagJets.size(); i++){
    //         if(tagJets.size() > 0){
    //             cout << "*********************************************************" << endl; 
    //             cout << "Good Jet " << i << " Scale Factors:" << endl; 
    //             PrintScaleFactorsPtr(tagJets[i]);
    //         }
    //     }

    //     // Electrons
    //     for (unsigned int i = 0; i < goodElectrons.size(); i++){
    //         if(goodElectrons.size() > 0){
    //             cout << "*********************************************************" << endl; 
    //             cout << "Good Electron " << i << " Scale Factors:" << endl; 
    //             PrintScaleFactorsPtr(goodElectrons[i]);
    //         }
    //     }

    //     // Muons 
    //     for (unsigned int i = 0; i < goodMuons.size(); i++){
    //         if(goodMuons.size() > 0){
    //             cout << "*********************************************************" << endl; 
    //             cout << "Good Muon " << i << " Scale Factors:" << endl; 
    //             PrintScaleFactorsPtr(goodMuons[i]);
    //         }
    //     }
    // }

    //-- Apply scale factors from all weighted objects
    // Diphoton
    tag_obj_.includeWeights( *dipho );
    
    // Electrons
    for (unsigned int elec_i = 0; elec_i < goodElectrons.size(); elec_i++){
        tag_obj_.includeWeights(*goodElectrons.at(elec_i));
    }
    
    // Muons
    for (unsigned int muon_i = 0; muon_i < goodMuons.size(); muon_i++){
        tag_obj_.includeWeights(*goodMuons.at(muon_i));
    }

    // Jets
    for (unsigned int TagJet_i = 0; TagJet_i < tagJets.size(); TagJet_i++){
        tag_obj_.includeWeightsByLabel(*tagJets.at(TagJet_i),"JetBTagReshapeWeight"); // Apply JetBTagReshapeWeight for all final state categories 
    }

    // // Jets
    // for (unsigned int TagJet_i = 0; TagJet_i < tagJets.size(); TagJet_i++){

    //     // Only save BTagCutWeight
    //     if(Tag_ == "FL"){
    //         tag_obj_.includeWeightsByLabel(*tagJets.at(TagJet_i),"JetBTagCutWeight");
    //     }

    //     // Save both b tag SFs for the untagged case in case the event is used for SL, FH or FL after ntuple production 
    //     else if(Tag_ == "Untagged"){ // If using untagged events, will need to divide out SF not being used 
    //         tag_obj_.includeWeightsByLabel(*tagJets.at(TagJet_i),"JetBTagCutWeight"); // for untagged just apply medium WP bveto SF since in the case these events are used, not 100% precise anyway 
    //         // tag_obj_.includeWeightsByLabel(*tagJets.at(TagJet_i),"JetBTagReshapeWeight");
    //     }

    //     // Only save bTagReshapeWeight for MVA final states 
    //     else{
    //         tag_obj_.includeWeightsByLabel(*tagJets.at(TagJet_i),"JetBTagReshapeWeight");
    //     }
    // }
    
    // // //-- Save central, up/down scale factors in output trees for checks / flexibility
    // // //-- Initialize Values with 1 incase there are no good electrons, muons or jets since in end values are set to tag object SF values
    // // double TagObjCentralWeight = 1; // Central weight of HH->WWgg tag object
    
    // // Diphoton
    // double LooseMvaSF = 1, PreselSF = 1, TriggerWeight = 1, electronVetoSF = 1, prefireWeight = 1, diPhoCentralWeight = 1; // Diphoton
    // // // double LooseMvaSF_up = 1, PreselSF_up = 1, TriggerWeight_up = 1, electronVetoSF_up = 1, prefireWeight_up = 1;
    // // // double LooseMvaSF_down = 1, PreselSF_down = 1, TriggerWeight_down = 1, electronVetoSF_down = 1, prefireWeight_down = 1;
    
    // // Electrons
    // double ElectronIDWeight = 1, ElectronRecoWeight = 1;
    // // // double ElectronIDWeight_up = 1, ElectronRecoWeight_up = 1;
    // // // double ElectronIDWeight_down = 1, ElectronRecoWeight_down = 1;
    
    // // Muons
    // double MuonIDWeight = 1, MuonIsoWeight = 1;
    // // double MuonIDWeight_up = 1, MuonIsoWeight_up = 1;
    // // double MuonIDWeight_down = 1, MuonIsoWeight_down = 1;
    
    // // Jets
    // double JetBTagReshapeWeight = 1, JetBTagCutWeight = 1;
    // // double JetBTagReshapeWeight_up = 1, JetBTagCutWeight_up = 1;
    // // double JetBTagReshapeWeight_down = 1, JetBTagCutWeight_down = 1;

    // // Electrons
    // for (unsigned int elec_i = 0; elec_i < goodElectrons.size(); elec_i++){
    //     ElectronIDWeight = ElectronIDWeight * goodElectrons.at(elec_i)->weight("ElectronIDWeightCentral");
    //     ElectronRecoWeight = ElectronRecoWeight * goodElectrons.at(elec_i)->weight("ElectronRecoWeightCentral");
        
    // //     // ElectronIDWeight_up = ElectronIDWeight_up * goodElectrons.at(elec_i)->weight("ElectronIDWeightUp01sigma");
    // //     // ElectronRecoWeight_up = ElectronRecoWeight_up * goodElectrons.at(elec_i)->weight("ElectronRecoWeightUp01sigma");
        
    // //     // ElectronIDWeight_down = ElectronIDWeight_down * goodElectrons.at(elec_i)->weight("ElectronIDWeightDown01sigma");
    // //     // ElectronRecoWeight_down = ElectronRecoWeight_down * goodElectrons.at(elec_i)->weight("ElectronRecoWeightDown01sigma");
    // }
    // tag_obj_.setWeight("ElectronIDWeightCentral",ElectronIDWeight);
    // tag_obj_.setWeight("ElectronRecoWeightCentral",ElectronRecoWeight);
    
    // // // tag_obj_.setWeight("ElectronIDWeightUp01sigma",ElectronIDWeight_up);
    // // // tag_obj_.setWeight("ElectronRecoWeightUp01sigma",ElectronRecoWeight_up);
    
    // // // tag_obj_.setWeight("ElectronIDWeightDown01sigma",ElectronIDWeight_down);
    // // // tag_obj_.setWeight("ElectronRecoWeightDown01sigma",ElectronRecoWeight_down);
    
    // // // // Muons
    // string MuonID_str_central, MuonID_str_up, MuonID_str_down;
    // string MuonISO_str_central, MuonISO_str_up, MuonISO_str_down;
    // if(MuonID_ == "Medium"){
    //     MuonID_str_central = "MuonMediumIDWeightCentral";
    // // // //     MuonID_str_up = "MuonMediumIDWeightUp01sigma";
    // // // //     MuonID_str_down = "MuonMediumIDWeightDown01sigma";
    // }

    // else if(MuonID_ == "Tight"){
    //     MuonID_str_central = "MuonTightIDWeightCentral";
    // // // //     MuonID_str_up = "MuonTightIDWeightUp01sigma";
    // // // //     MuonID_str_down = "MuonTightIDWeightDown01sigma";
    // }    

    // if(muPFIsoSumRelThreshold_ == 0.25){
    //     MuonISO_str_central = "MuonLooseRelISOWeightCentral";
    // // // //     MuonISO_str_up = "MuonLooseRelISOWeightUp01sigma";
    // // // //     MuonISO_str_down = "MuonLooseRelISOWeightDown01sigma";
    // }

    // else if(muPFIsoSumRelThreshold_ == 0.15){
    //     MuonISO_str_central = "MuonTightRelISOWeightCentral";
    // // // //     MuonISO_str_up = "MuonTightRelISOWeightUp01sigma";
    // // // //     MuonISO_str_down = "MuonTightRelISOWeightDown01sigma";
    // }    

    // // // if(doHHWWggDebug_){
    // // //     cout << "MuonID : " << MuonID_ << endl;
    // // //     cout << "muPFIsoSumRelThreshold_ : " << muPFIsoSumRelThreshold_ << endl; 
    // // //     cout << "MuonID_str_central : " << MuonID_str_central << endl; 
    // // //     cout << "MuonISO_str_central : " << MuonISO_str_central << endl; 
    // // // }

    // for (unsigned int muon_i = 0; muon_i < goodMuons.size(); muon_i++){
    // // //     // Set muon weight string based on input falgs

    //     MuonIDWeight = MuonIDWeight * goodMuons.at(muon_i)->weight(MuonID_str_central);
    //     MuonIsoWeight = MuonIsoWeight * goodMuons.at(muon_i)->weight(MuonISO_str_central);
        
    // // // //     MuonIDWeight_up = MuonIDWeight_up * goodMuons.at(muon_i)->weight(MuonID_str_up);
    // // // //     MuonIsoWeight_up = MuonIsoWeight_up * goodMuons.at(muon_i)->weight(MuonISO_str_up);
        
    // // // //     MuonIDWeight_down = MuonIDWeight_down * goodMuons.at(muon_i)->weight(MuonID_str_down);
    // // // //     MuonIsoWeight_down = MuonIsoWeight_down * goodMuons.at(muon_i)->weight(MuonISO_str_down);

    // }

    // tag_obj_.setWeight(MuonID_str_central,MuonIDWeight);
    // tag_obj_.setWeight(MuonISO_str_central,MuonIsoWeight);
    
    // // // tag_obj_.setWeight("MuonIDWeightUp01sigma",MuonIDWeight_up);
    // // // tag_obj_.setWeight("MuonIsoWeightUp01sigma",MuonIsoWeight_up);
    
    // // // tag_obj_.setWeight("MuonIDWeightDown01sigma",MuonIDWeight_down);
    // // // tag_obj_.setWeight("MuonIsoWeightDown01sigma",MuonIsoWeight_down);    
    
    // // Jets

    // // If Fully Leptonic Final state, save SF for JetBTagCut b/c applying bVeto 
    // if(Tag_ == "FL"){
    //     for (unsigned int TagJet_i = 0; TagJet_i < tagJets.size(); TagJet_i++){
    //         JetBTagCutWeight = JetBTagCutWeight * tagJets.at(TagJet_i)->weight("JetBTagCutWeightCentral");
    //         // JetBTagCutWeight_up = JetBTagCutWeight_up * tagJets.at(TagJet_i)->weight("JetBTagCutWeightUp01sigma");
    //         // JetBTagCutWeight_down = JetBTagCutWeight_down * tagJets.at(TagJet_i)->weight("JetBTagCutWeightDown01sigma");
    //     }
    // }

    // // Save both b tag SFs for the untagged case in case the event is used for SL, FH or FL after ntuple production 
    // else if(Tag_ == "Untagged"){
    //     for (unsigned int TagJet_i = 0; TagJet_i < tagJets.size(); TagJet_i++){
    //         JetBTagCutWeight = JetBTagCutWeight * tagJets.at(TagJet_i)->weight("JetBTagCutWeightCentral");
    //         // JetBTagCutWeight_up = JetBTagCutWeight_up * tagJets.at(TagJet_i)->weight("JetBTagCutWeightUp01sigma");
    // //         JetBTagCutWeight_down = JetBTagCutWeight_down * tagJets.at(TagJet_i)->weight("JetBTagCutWeightDown01sigma");
    //         JetBTagReshapeWeight = JetBTagReshapeWeight * tagJets.at(TagJet_i)->weight("JetBTagReshapeWeightCentral");
    // //         JetBTagReshapeWeight_up = JetBTagReshapeWeight_up * tagJets.at(TagJet_i)->weight("JetBTagReshapeWeightUp01sigma");
    // //         JetBTagReshapeWeight_down = JetBTagReshapeWeight_down * tagJets.at(TagJet_i)->weight("JetBTagReshapeWeightDown01sigma");            
    //     }

    // }
    // else{
    //     for (unsigned int TagJet_i = 0; TagJet_i < tagJets.size(); TagJet_i++){
    //         JetBTagReshapeWeight = JetBTagReshapeWeight * tagJets.at(TagJet_i)->weight("JetBTagReshapeWeightCentral");
    // //         JetBTagReshapeWeight_up = JetBTagReshapeWeight_up * tagJets.at(TagJet_i)->weight("JetBTagReshapeWeightUp01sigma");
    // //         JetBTagReshapeWeight_down = JetBTagReshapeWeight_down * tagJets.at(TagJet_i)->weight("JetBTagReshapeWeightDown01sigma");            
    //     }            
    // }    

    // // In any case, save weight values. If not set, should default to 1. 
    // tag_obj_.setWeight("JetBTagCutWeightCentral",JetBTagCutWeight);
    // // tag_obj_.setWeight("JetBTagCutWeightUp01sigma",JetBTagCutWeight_up);
    // // tag_obj_.setWeight("JetBTagCutWeightDown01sigma",JetBTagCutWeight_down);
    // tag_obj_.setWeight("JetBTagReshapeWeightCentral",JetBTagReshapeWeight);
    // // tag_obj_.setWeight("JetBTagReshapeWeightUp01sigma",JetBTagReshapeWeight_up);
    // // tag_obj_.setWeight("JetBTagReshapeWeightDown01sigma",JetBTagReshapeWeight_down);    

    // // Diphoton
    // diPhoCentralWeight = dipho->centralWeight();

    // LooseMvaSF = dipho->weight("LooseMvaSFCentral");
    // PreselSF = dipho->weight("PreselSFCentral");
    // TriggerWeight = dipho->weight("TriggerWeightCentral");
    // electronVetoSF = dipho->weight("electronVetoSFCentral");
    // prefireWeight = dipho->weight("prefireWeightCentral") / diPhoCentralWeight; // prefireweight already multiplied by diphoton central weight 
    
    // // LooseMvaSF_up = dipho->weight("LooseMvaSFUp01sigma");
    // // PreselSF_up = dipho->weight("PreselSFUp01sigma");
    // // TriggerWeight_up = dipho->weight("TriggerWeightUp01sigma");
    // // electronVetoSF_up = dipho->weight("electronVetoSFUp01sigma");
    // // prefireWeight_up = dipho->weight("prefireWeightUp01sigma");
    
    // // LooseMvaSF_down = dipho->weight("LooseMvaSFDown01sigma");
    // // PreselSF_down = dipho->weight("PreselSFDown01sigma");
    // // TriggerWeight_down = dipho->weight("TriggerWeightDown01sigma");
    // // electronVetoSF_down = dipho->weight("electronVetoSFDown01sigma");
    // // prefireWeight_down = dipho->weight("prefireWeightDown01sigma");
    
    
    // Divide by prefireWeight because it's already applied to all previously set diphoton scale factors. Multiplication happens in PrefireDiPhotonProducer when updating diphoton object
    // tag_obj_.setWeight("LooseMvaSFCentral",LooseMvaSF/prefireWeight);
    // tag_obj_.setWeight("PreselSFCentral",PreselSF/prefireWeight);
    // tag_obj_.setWeight("TriggerWeightCentral",TriggerWeight/prefireWeight);
    // tag_obj_.setWeight("electronVetoSFCentral",electronVetoSF/prefireWeight);
    // tag_obj_.setWeight("prefireWeightCentral",prefireWeight);
    
    // // tag_obj_.setWeight("LooseMvaSFUp01sigma",LooseMvaSF_up);
    // // tag_obj_.setWeight("PreselSFUp01sigma",PreselSF_up);
    // // tag_obj_.setWeight("TriggerWeightUp01sigma",TriggerWeight_up);
    // // tag_obj_.setWeight("electronVetoSFUp01sigma",electronVetoSF_up);
    // // tag_obj_.setWeight("prefireWeightUp01sigma",prefireWeight_up);
    
    // // tag_obj_.setWeight("LooseMvaSFDown01sigma",LooseMvaSF_down);
    // // tag_obj_.setWeight("PreselSFDown01sigma",PreselSF_down);
    // // tag_obj_.setWeight("TriggerWeightDown01sigma",TriggerWeight_down);
    // // tag_obj_.setWeight("electronVetoSFDown01sigma",electronVetoSF_down);
    // // tag_obj_.setWeight("prefireWeightDown01sigma",prefireWeight_down);
    
    return tag_obj_;
}

double HHWWggTagProducer::PassPUJetID(vector<double> pujid_wp_pt_bin_1_, vector<double> pujid_wp_pt_bin_2_, Ptr<flashgg::Jet> thejet)
{
    std::vector<std::pair<double,double> > eta_cuts_(4);
    eta_cuts_[0] = std::make_pair (0    , 2.50 );
    eta_cuts_[1] = std::make_pair (2.50 , 2.75 );
    eta_cuts_[2] = std::make_pair (2.75 , 3.00 );
    eta_cuts_[3] = std::make_pair (3.00 , 10);

    double passPUJetID = 0; 
    for (UInt_t eta_bin = 0; eta_bin < pujid_wp_pt_bin_1_.size(); eta_bin++ ){
        if ( fabs( thejet->eta() ) >  eta_cuts_[eta_bin].first &&
            fabs( thejet->eta() ) <= eta_cuts_[eta_bin].second){
            if ( thejet->pt() >  20 &&
                thejet->pt() <= 30 && thejet->puJetIdMVA() > pujid_wp_pt_bin_1_[eta_bin] )
                passPUJetID = 1;
            if ( thejet->pt() >  30 &&
                thejet->pt() <= 50 && thejet->puJetIdMVA() > pujid_wp_pt_bin_2_[eta_bin] )
                passPUJetID = 1;
            if (thejet->pt() > 50) passPUJetID = 1;
        }
    }

    return passPUJetID; 
}

HHWWggTag HHWWggTagProducer::ComputePUJetIDs(vector<Ptr<flashgg::Jet>> jets, HHWWggTag tag_obj)
{
    // Compute booleans for PUJetID working points and set the tag object values to be accessed in HHWWggCustomize and saved in the output trees
    // This way you can run once and check different PUJetID working points afterwards

    double passes_loose, passes_medium, passes_tight = 0;
    std::vector<std::vector<double>> allJetIDs;

    std::vector<double> pujid_wp_pt_bin_1_Loose_ = {-0.97, -0.68, -0.53, -0.47};
    std::vector<double> pujid_wp_pt_bin_2_Loose_ = {-0.89, -0.52, -0.38, -0.30};   

    std::vector<double> pujid_wp_pt_bin_1_Medium_ = {0.18, -0.55, -0.42, -0.36};
    std::vector<double> pujid_wp_pt_bin_2_Medium_ = {0.61, -0.35, -0.23, -0.17};    

    std::vector<double> pujid_wp_pt_bin_1_Tight_ = {0.69, -0.35, -0.26, -0.21};
    std::vector<double> pujid_wp_pt_bin_2_Tight_ = {0.86, -0.10, -0.05, -0.01};           

    for( unsigned int jet_i = 0; jet_i <  jets.size() ; jet_i++ )
        {
            edm::Ptr<flashgg::Jet> thejet = jets[ jet_i ];
            std::vector<double> JetIDs;
            passes_loose = PassPUJetID(pujid_wp_pt_bin_1_Loose_, pujid_wp_pt_bin_2_Loose_, thejet);    
            passes_medium = PassPUJetID(pujid_wp_pt_bin_1_Medium_, pujid_wp_pt_bin_2_Medium_, thejet);
            passes_tight = PassPUJetID(pujid_wp_pt_bin_1_Tight_, pujid_wp_pt_bin_2_Tight_, thejet);
            JetIDs.push_back(passes_loose);
            JetIDs.push_back(passes_medium);
            JetIDs.push_back(passes_tight);
            allJetIDs.push_back(JetIDs);
        }

    tag_obj.SetJetIDs(allJetIDs);

    return tag_obj; 

}

// std::vector<edm::Ptr<reco::GenParticle> > HHWWggTagProducer::OrderParticles(std::vector<edm::Ptr<reco::GenParticle> > GenParticles )
// {
    // double maxpT = -999; 
    // double tempPt; 
    // int N_genParticles = GenParticles.size();
    // edm::Ptr<reco::GenParticle> GenPart; 

    // for (unsigned int i; i < N_genParticles; i ++){
    //     GenPart = GenParticles.at(i);
    //     tempPt = GenPart->p4().pt(); 


    // }

    

// }


}

#endif

