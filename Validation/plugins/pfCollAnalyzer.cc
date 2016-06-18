// By L D CORPE
// Adapted from the flashggCommissioning tree maker code  by C. Favaro et al.
// modified by Y.Haddad
//
#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/Ptr.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
//#include "DataFormats/Common/interface/PtrVector.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/PatCandidates/interface/MET.h"

#include "flashgg/MicroAOD/interface/VertexSelectorBase.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"

#include "flashgg/MicroAOD/interface/PhotonIdUtils.h"

#include "flashgg/DataFormats/interface/VertexCandidateMap.h"
#include "flashgg/DataFormats/interface/Jet.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "flashgg/DataFormats/interface/VBFTag.h"
#include "flashgg/DataFormats/interface/UntaggedTag.h"
#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "flashgg/DataFormats/interface/TTHHadronicTag.h"
#include "flashgg/DataFormats/interface/TTHLeptonicTag.h"

#include "DataFormats/VertexReco/interface/Vertex.h"

#include "TMath.h"
#include "TTree.h"
#include "TVector3.h"
#include "TLorentzVector.h"

using namespace std;
using namespace edm;
using namespace flashgg;

// **********************************************************************
struct info_t {
    float pt;
    float eta;
    float phi;
    float energy;
    float mass;
    float uncorrected_pt;
    int pdgId;
};

class FlashggPFCollAnalyzer : public edm::EDAnalyzer
{
public:
    explicit FlashggPFCollAnalyzer( const edm::ParameterSet & );
    ~FlashggPFCollAnalyzer();

    static void fillDescriptions( edm::ConfigurationDescriptions &descriptions );

private:

    edm::Service<TFileService> fs_;

    virtual void beginJob() override;
    virtual void analyze( const edm::Event &, const edm::EventSetup & ) override;
    virtual void endJob() override;

    //void initEventStructure();

    EDGetTokenT<View<reco::Candidate> > pfCollPF_;
    EDGetTokenT<View<reco::Candidate> > pfCollPFCHS0_;
    EDGetTokenT<View<reco::Candidate> > pfCollPFCHSLeg_;
    EDGetTokenT<View<reco::Candidate> > pfCollPUPPI0_;
    EDGetTokenT<View<reco::Candidate> > pfCollPUPPILeg_;
    EDGetTokenT<View<reco::Vertex> > vertexToken_;
    TTree *tree_PF;
    TTree *tree_PFCHS0;
    TTree *tree_PFCHSLeg;
    TTree *tree_PUPPI0;
    TTree *tree_PUPPILeg;

    info_t info_PF;
    info_t info_PFCHS0;
    info_t info_PFCHSLeg;
    info_t info_PUPPI0;
    info_t info_PUPPILeg;

    TH1F *pfPt ;
    TH1F *pfEta ;
    TH1F *pfPhi  ;
    TH1F *pfchs0Pt;
    TH1F *pfchs0Eta;
    TH1F *pfchs0Phi ;
    TH1F *pfchsLegPt ;
    TH1F *pfchsLegEta;
    TH1F *pfchsLegPhi;
    TH1F *puppi0Pt   ;
    TH1F *puppi0Eta  ;
    TH1F *puppi0Phi  ;
    TH1F *puppiLegPt ;
    TH1F *puppiLegEta;
    TH1F *puppiLegPhi;




    //TTree *flashggTreeWithTagSorter;

    // Variables to fill

};



// ******************************************************************************************
// ******************************************************************************************

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
FlashggPFCollAnalyzer::FlashggPFCollAnalyzer( const edm::ParameterSet &iConfig ):
    pfCollPF_( consumes<View<reco::Candidate> >( iConfig.getParameter<InputTag>( "CollTagPF" ) ) ),
    pfCollPFCHS0_( consumes<View<reco::Candidate> >( iConfig.getParameter<InputTag>( "CollTagPFPFCHS0" ) ) ),
    pfCollPFCHSLeg_( consumes<View<reco::Candidate> >( iConfig.getParameter<InputTag>( "CollTagPFPFCHSLeg" ) ) ),
    pfCollPUPPI0_( consumes<View<reco::Candidate> >( iConfig.getParameter<InputTag>( "CollTagPFPUPPI0" ) ) ),
    pfCollPUPPILeg_( consumes<View<reco::Candidate> >( iConfig.getParameter<InputTag>( "CollTagPFPUPPILeg" ) ) ),
    vertexToken_( consumes<View<reco::Vertex> >( iConfig.getUntrackedParameter<InputTag> ( "VertexTag", InputTag( "offlineSlimmedPrimaryVertices" ) ) ) )
{
}

FlashggPFCollAnalyzer::~FlashggPFCollAnalyzer()
{

}

void
FlashggPFCollAnalyzer::analyze( const edm::Event &iEvent, const edm::EventSetup &iSetup )
{

    // ********************************************************************************
    // access edm objects


    Handle<View<reco::Candidate> > pfCollPF;
    iEvent.getByToken( pfCollPF_, pfCollPF );
// const PtrVector<reco::Candidate>& pfPtrs = pfCollPF->ptrVector();

    Handle<View<reco::Candidate> > pfCollPFCHS0;
    iEvent.getByToken( pfCollPFCHS0_, pfCollPFCHS0 );
// const PtrVector<reco::Candidate>& pfchs0Ptrs = pfCollPFCHS0->ptrVector();

    Handle<View<reco::Candidate> > pfCollPFCHSLeg;
    iEvent.getByToken( pfCollPFCHSLeg_, pfCollPFCHSLeg );
// const PtrVector<reco::Candidate>& pfchsLegPtrs = pfCollPFCHSLeg->ptrVector();

    Handle<View<reco::Candidate> > pfCollPUPPI0;
    iEvent.getByToken( pfCollPUPPI0_, pfCollPUPPI0 );
//  const PtrVector<reco::Candidate>& puppi0Ptrs = pfCollPUPPI0->ptrVector();


    Handle<View<reco::Candidate> > pfCollPUPPILeg;
    iEvent.getByToken( pfCollPUPPILeg_, pfCollPUPPILeg );
//  const PtrVector<reco::Candidate>& puppiLegPtrs = pfCollPUPPILeg->ptrVector();
    Handle<View<reco::Vertex> > primaryVertices;
    iEvent.getByToken( vertexToken_, primaryVertices );
//	const PtrVector<reco::Vertex>& vtxs = primaryVertices->ptrVector();


    for( UInt_t Loop = 0; Loop < pfCollPF->size() ; Loop++ ) {

        Float_t pt             = pfCollPF->ptrAt( Loop )->pt();
        //  Float_t uncorrected_pt = pfCollPF->ptrAt(Loop)->correctedJet("raw").pt();
        Float_t mass           = pfCollPF->ptrAt( Loop )->mass();
        Float_t eta            = pfCollPF->ptrAt( Loop )->eta();
        Float_t phi            = pfCollPF->ptrAt( Loop )->phi();
        int     pdgId          = pfCollPF->ptrAt( Loop )->pdgId();

        info_PF.pt             = pt;
        //  info_PF.uncorrected_pt = uncorrected_pt;
        info_PF.mass           = mass;
        info_PF.eta            = eta;
        info_PF.phi            = phi;
        info_PF.pdgId          = int( pdgId );

        pfPt->Fill( pt );
        pfEta->Fill( eta );
        pfPhi->Fill( phi );

        tree_PF->Fill();

    }

    for( UInt_t Loop = 0; Loop < pfCollPFCHS0->size() ; Loop++ ) {

        Float_t pt             = pfCollPFCHS0->ptrAt( Loop )->pt();
        //  Float_t uncorrected_pt = pfCollPFCHS0->ptrAt(Loop)->correctedJet("raw").pt();
        Float_t mass           = pfCollPFCHS0->ptrAt( Loop )->mass();
        Float_t eta            = pfCollPFCHS0->ptrAt( Loop )->eta();
        Float_t phi            = pfCollPFCHS0->ptrAt( Loop )->phi();
        int     pdgId          = pfCollPFCHS0->ptrAt( Loop )->pdgId();

        info_PFCHS0.pt             = pt;
        //  info_PFCHS0.uncorrected_pt = uncorrected_pt;
        info_PFCHS0.mass           = mass;
        info_PFCHS0.eta            = eta;
        info_PFCHS0.phi            = phi;
        info_PFCHS0.pdgId          = int( pdgId );

        //std::cout << "[PFCHS0] particle " << Loop <<  ", eta "<< eta << ", phi " << phi << ", pt " << pt << ", pdgid " << pdgId << ", dz" << pfCollPFCHS0->ptrAt(Loop)->vertex().z()-primaryVertices[0]->position().z() << std::endl;
        if( primaryVertices->size() );
        pfchs0Pt->Fill( pt );
        pfchs0Eta->Fill( eta );
        pfchs0Phi->Fill( phi );

        tree_PFCHS0->Fill();
    }
    for( UInt_t Loop = 0; Loop < pfCollPFCHSLeg->size() ; Loop++ ) {

        Float_t pt             = pfCollPFCHSLeg->ptrAt( Loop )->pt();
        //  Float_t uncorrected_pt = pfCollPFCHSLeg->ptrAt(Loop)->correctedJet("raw").pt();
        Float_t mass           = pfCollPFCHSLeg->ptrAt( Loop )->mass();
        Float_t eta            = pfCollPFCHSLeg->ptrAt( Loop )->eta();
        Float_t phi            = pfCollPFCHSLeg->ptrAt( Loop )->phi();
        int     pdgId          = pfCollPFCHSLeg->ptrAt( Loop )->pdgId();

        info_PFCHSLeg.pt             = pt;
//   info_PFCHSLeg.uncorrected_pt = uncorrected_pt;
        info_PFCHSLeg.mass           = mass;
        info_PFCHSLeg.eta            = eta;
        info_PFCHSLeg.phi            = phi;
        info_PFCHSLeg.pdgId          = int( pdgId );

        pfchsLegPt->Fill( pt );
        pfchsLegEta->Fill( eta );
        pfchsLegPhi->Fill( phi );

        tree_PFCHSLeg->Fill();
    }
    for( UInt_t Loop = 0; Loop < pfCollPUPPI0->size() ; Loop++ ) {

        Float_t pt             = pfCollPUPPI0->ptrAt( Loop )->pt();
        // Float_t uncorrected_pt = pfCollPUPPI0->ptrAt(Loop)->correctedJet("raw").pt();
        Float_t mass           = pfCollPUPPI0->ptrAt( Loop )->mass();
        Float_t eta            = pfCollPUPPI0->ptrAt( Loop )->eta();
        Float_t phi            = pfCollPUPPI0->ptrAt( Loop )->phi();
        int     pdgId          = pfCollPUPPI0->ptrAt( Loop )->pdgId();

        info_PUPPI0.pt             = pt;
//   info_PUPPI0.uncorrected_pt = uncorrected_pt;
        info_PUPPI0.mass           = mass;
        info_PUPPI0.eta            = eta;
        info_PUPPI0.phi            = phi;
        info_PUPPI0.pdgId          = int( pdgId );

        puppi0Pt->Fill( pt );
        puppi0Eta->Fill( eta );
        puppi0Phi->Fill( phi );

        tree_PUPPI0->Fill();
    }

    for( UInt_t Loop = 0; Loop < pfCollPUPPILeg->size() ; Loop++ ) {


        Float_t pt             = pfCollPUPPILeg->ptrAt( Loop )->pt();
        //  Float_t uncorrected_pt = pfCollPUPPILeg->ptrAt(Loop)->correctedJet("raw").pt();
        Float_t mass           = pfCollPUPPILeg->ptrAt( Loop )->mass();
        Float_t eta            = pfCollPUPPILeg->ptrAt( Loop )->eta();
        Float_t phi            = pfCollPUPPILeg->ptrAt( Loop )->phi();
        int     pdgId          = pfCollPUPPILeg->ptrAt( Loop )->pdgId();

        info_PUPPILeg.pt             = pt;
        // info_PUPPILeg.uncorrected_pt = uncorrected_pt;
        info_PUPPILeg.mass           = mass;
        info_PUPPILeg.eta            = eta;
        info_PUPPILeg.phi            = phi;
        info_PUPPILeg.pdgId          = int( pdgId );

        puppiLegPt->Fill( pt );
        puppiLegEta->Fill( eta );
        puppiLegPhi->Fill( phi );

        tree_PUPPILeg->Fill();
    }

}



void
FlashggPFCollAnalyzer::beginJob()
{


    pfPt         = fs_->make<TH1F>( "pfPt", "pfPt", 500, 0, 500 );
    pfEta        = fs_->make<TH1F>( "pfEta", "pfEta", 500, -5, 5 );
    pfPhi        = fs_->make<TH1F>( "pfPhi", "pfPhi", 500, -6.3, 6.3 );
    pfchs0Pt     = fs_->make<TH1F>( "pfchs0Pt", "pfchs0Pt", 500, 0, 500 );
    pfchs0Eta    = fs_->make<TH1F>( "pfchs0Eta", "pfchs0Eta", 500, -5, 5 );
    pfchs0Phi    = fs_->make<TH1F>( "pfchs0Phi", "pfchs0Phi", 500, -6.3, 6.3 );
    pfchsLegPt   = fs_->make<TH1F>( "pfchsLegPt", "pfchsLegPt", 500, 0, 500 );
    pfchsLegEta  = fs_->make<TH1F>( "pfchsLegEta", "pfchsLegEta", 500, -5, 5 );
    pfchsLegPhi  = fs_->make<TH1F>( "pfchsLegPhi", "pfchsLegPhi", 500, -6.3, 6.3 );
    puppi0Pt     = fs_->make<TH1F>( "puppi0Pt", "puppi0Pt", 500, 0, 500 );
    puppi0Eta    = fs_->make<TH1F>( "puppi0Eta", "puppi0Eta", 500, -5, 5 );
    puppi0Phi    = fs_->make<TH1F>( "puppi0Phi", "puppi0Phi", 500, -6.3, 6.3 );
    puppiLegPt   = fs_->make<TH1F>( "puppiLegPt", "puppiLegPt", 500, 0, 500 );
    puppiLegEta  = fs_->make<TH1F>( "puppiLegEta", "puppiLegEta", 500, -5, 5 );
    puppiLegPhi  = fs_->make<TH1F>( "puppiLegPhi", "puppiLegPhi", 500, -6.3, 6.3 );


    tree_PF = fs_->make<TTree>( "tree_PF", "" );
    tree_PF->Branch( "pt"               , &info_PF.pt              , "pt/F" );
//  tree_PF->Branch("uncorrected_pt"   ,&info_PF.uncorrected_pt  ,"uncorrected_pt/F" );
    tree_PF->Branch( "mass"             , &info_PF.phi             , "mass/F" );
    tree_PF->Branch( "eta"              , &info_PF.eta             , "eta/F" );
    tree_PF->Branch( "phi"              , &info_PF.phi             , "phi/F" );
    tree_PF->Branch( "pdgId"            , &info_PF.pdgId           , "pdgId/I" );

    tree_PFCHS0 = fs_->make<TTree>( "tree_PFCHS0", "" );
    tree_PFCHS0->Branch( "pt"               , &info_PFCHS0.pt              , "pt/F" );
//  tree_PFCHS0->Branch("uncorrected_pt"   ,&info_PFCHS0.uncorrected_pt  ,"uncorrected_pt/F" );
    tree_PFCHS0->Branch( "mass"             , &info_PFCHS0.phi             , "mass/F" );
    tree_PFCHS0->Branch( "eta"              , &info_PFCHS0.eta             , "eta/F" );
    tree_PFCHS0->Branch( "phi"              , &info_PFCHS0.phi             , "phi/F" );
    tree_PFCHS0->Branch( "pdgId"            , &info_PFCHS0.pdgId           , "pdgId/I" );


    tree_PFCHSLeg = fs_->make<TTree>( "tree_PFCHSLeg", "" );
    tree_PFCHSLeg->Branch( "pt"               , &info_PFCHSLeg.pt              , "pt/F" );
//  tree_PFCHSLeg->Branch("uncorrected_pt"   ,&info_PFCHSLeg.uncorrected_pt  ,"uncorrected_pt/F" );
    tree_PFCHSLeg->Branch( "mass"             , &info_PFCHSLeg.phi             , "mass/F" );
    tree_PFCHSLeg->Branch( "eta"              , &info_PFCHSLeg.eta             , "eta/F" );
    tree_PFCHSLeg->Branch( "phi"              , &info_PFCHSLeg.phi             , "phi/F" );
    tree_PFCHSLeg->Branch( "pdgId"            , &info_PFCHSLeg.pdgId           , "pdgId/I" );

    tree_PUPPI0 = fs_->make<TTree>( "tree_PUPPI0", "" );
    tree_PUPPI0->Branch( "pt"               , &info_PUPPI0.pt              , "pt/F" );
//  tree_PUPPI0->Branch("uncorrected_pt"   ,&info_PUPPI0.uncorrected_pt  ,"uncorrected_pt/F" );
    tree_PUPPI0->Branch( "mass"             , &info_PUPPI0.phi             , "mass/F" );
    tree_PUPPI0->Branch( "eta"              , &info_PUPPI0.eta             , "eta/F" );
    tree_PUPPI0->Branch( "phi"              , &info_PUPPI0.phi             , "phi/F" );
    tree_PUPPI0->Branch( "pdgId"            , &info_PUPPI0.pdgId           , "pdgId/I" );


    tree_PUPPILeg = fs_->make<TTree>( "tree_PUPPILeg", "" );
    tree_PUPPILeg->Branch( "pt"               , &info_PUPPILeg.pt              , "pt/F" );
// tree_PUPPILeg->Branch("uncorrected_pt"   ,&info_PUPPILeg.uncorrected_pt  ,"uncorrected_pt/F" );
    tree_PUPPILeg->Branch( "mass"             , &info_PUPPILeg.phi             , "mass/F" );
    tree_PUPPILeg->Branch( "eta"              , &info_PUPPILeg.eta             , "eta/F" );
    tree_PUPPILeg->Branch( "phi"              , &info_PUPPILeg.phi             , "phi/F" );
    tree_PUPPILeg->Branch( "pdgId"            , &info_PUPPILeg.pdgId           , "pdgId/I" );

    //pfPt       ->Sumw2();
    //pfEta       ->Sumw2();
    //pfPhi       ->Sumw2();
    //pfchs0Pt    ->Sumw2();
    //pfchs0Eta   ->Sumw2();
    //pfchs0Phi   ->Sumw2();
    //pfchsLegPt  ->Sumw2();
    //pfchsLegEta ->Sumw2();
    //pfchsLegPhi ->Sumw2();
    //puppi0Pt    ->Sumw2();
    //puppi0Eta   ->Sumw2();
    //puppi0Phi   	->Sumw2();
    //puppiLegPt  ->Sumw2();
    //puppiLegEta->Sumw2();
    //puppiLegPhi ->Sumw2();

}
void
FlashggPFCollAnalyzer::endJob()
{

    /*pfPt       ->Write();
      pfEta       ->Write();
      pfPhi       ->Write();
      pfchs0Pt    ->Write();
      pfchs0Eta   ->Write();
      pfchs0Phi   ->Write();
      pfchsLegPt  ->Write();
      pfchsLegEta ->Write();
      pfchsLegPhi ->Write();
      puppi0Pt    ->Write();
      puppi0Eta   ->Write();
      puppi0Phi   	->Write();
      puppiLegPt  ->Write();
      puppiLegEta->Write();
      puppiLegPhi ->Write();*/

}
void
FlashggPFCollAnalyzer::fillDescriptions( edm::ConfigurationDescriptions &descriptions )
{
    //The following says we do not know what parameters are allowed so do no validation
    // Please change this to state exactly what you do use, even if it is no parameters
    edm::ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault( desc );
}

typedef FlashggPFCollAnalyzer FlashggFlashggPFCollAnalyzer;
DEFINE_FWK_MODULE( FlashggFlashggPFCollAnalyzer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

