// By O. Bondu
// Adapted from S. Senz VertexValidationTreeMaker, base code from the flashggCommissioning tree maker code  by C. Favaro et al.

#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/Ptr.h"
//#include "DataFormats/Common/interface/PtrVector.h"

#include "DataFormats/VertexReco/interface/Vertex.h"

#include "flashgg/MicroAOD/interface/VertexSelectorBase.h"
#include "DataFormats/EgammaCandidates/interface/PhotonCore.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"

#include "flashgg/MicroAOD/interface/PhotonIdUtils.h"

#include "flashgg/DataFormats/interface/VertexCandidateMap.h"
#include "TTree.h"

// **********************************************************************

// define the structures used to create tree branches and fill the trees

// per-event tree:
struct eventInfo {

    int ndiphoton;

};

// per-photon tree:
struct diphotonInfo {

// leading photon
    int pho1_isEB;
    int pho1_isEE;
    float pho1_pt;
    float pho1_eta;
    float pho1_phi;
    float pho1_energy;
    float pho1_r9;
    float pho1_hoe;
    float pho1_sieie;
    float pho1_full5x5_r9;
    float pho1_full5x5_hoe;
    float pho1_full5x5_sieie;
    float pho1_hcalTowerSumEtConeDR03;
    float pho1_trkSumPtHollowConeDR03;
    float pho1_pfChgIsoWrtChosenVtx02;
// subleading photon
    int pho2_isEB;
    int pho2_isEE;
    float pho2_pt;
    float pho2_eta;
    float pho2_phi;
    float pho2_energy;
    float pho2_r9;
    float pho2_hoe;
    float pho2_sieie;
    float pho2_full5x5_r9;
    float pho2_full5x5_hoe;
    float pho2_full5x5_sieie;
    float pho2_hcalTowerSumEtConeDR03;
    float pho2_trkSumPtHollowConeDR03;
    float pho2_pfChgIsoWrtChosenVtx02;
// diphoton info
    float pt;
    float eta;
    float phi;
    float energy;
    float mass;

};
// **********************************************************************

using namespace std;
using namespace edm;
using namespace flashgg;

// **********************************************************************

class DiphotonPreselectionValidationTreeMaker : public edm::EDAnalyzer
{
public:
    explicit DiphotonPreselectionValidationTreeMaker( const edm::ParameterSet & );
    ~DiphotonPreselectionValidationTreeMaker();

    static void fillDescriptions( edm::ConfigurationDescriptions &descriptions );


private:

    edm::Service<TFileService> fs_;

    virtual void beginJob() override;
    virtual void analyze( const edm::Event &, const edm::EventSetup & ) override;
    virtual void endJob() override;

    void initEventStructure();


    TTree *eventTree;
    TTree *diphotonTree;
    eventInfo evInfo;
    diphotonInfo diphoInfo;


    edm::EDGetTokenT<edm::View<flashgg::DiPhotonCandidate> > diphotonToken_;

};

// ******************************************************************************************
// ******************************************************************************************

//
// constants, enums and typedefs
//

//
// static data member definitions
//

// data to compute preselection efficiencies per lead/sublead photon category
unsigned int nphoton_processed[8] = {0};
unsigned int nphoton_preselected[8] = {0};
unsigned int nphoton_passing_r9[8] = {0};
unsigned int nphoton_passing_hoe[8] = {0};
unsigned int nphoton_passing_sieie[8] = {0};
unsigned int nphoton_passing_hcalISO[8] = {0};
unsigned int nphoton_passing_trkISO[8] = {0};
unsigned int nphoton_passing_pfISO[8] = {0};


//
// constructors and destructor
//
DiphotonPreselectionValidationTreeMaker::DiphotonPreselectionValidationTreeMaker( const edm::ParameterSet &iConfig ):
    diphotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) )
{

}


DiphotonPreselectionValidationTreeMaker::~DiphotonPreselectionValidationTreeMaker()
{


}



void
DiphotonPreselectionValidationTreeMaker::analyze( const edm::Event &iEvent, const edm::EventSetup &iSetup )
{

    // ********************************************************************************

    // access edm objects

    Handle<View<flashgg::DiPhotonCandidate> > diphotons;
    iEvent.getByToken( diphotonToken_, diphotons );
// const PtrVector<flashgg::DiPhotonCandidate>& diphotonPointers = diphotons->ptrVector();

    // ********************************************************************************
    initEventStructure();
    evInfo.ndiphoton = diphotons->size();
    eventTree->Fill();
    for( size_t idipho = 0; idipho < diphotons->size(); idipho++ ) {

        Ptr<flashgg::DiPhotonCandidate> diphoPtr = diphotons->ptrAt( idipho );
// leading photon
        diphoInfo.pho1_isEB = diphoPtr->leadingPhoton()->isEB();
        diphoInfo.pho1_isEE = diphoPtr->leadingPhoton()->isEE();
        diphoInfo.pho1_pt = diphoPtr->leadingPhoton()->pt();
        diphoInfo.pho1_eta = diphoPtr->leadingPhoton()->eta();
        diphoInfo.pho1_phi = diphoPtr->leadingPhoton()->phi();
        diphoInfo.pho1_energy = diphoPtr->leadingPhoton()->energy();
        diphoInfo.pho1_r9 = diphoPtr->leadingPhoton()->r9();
        diphoInfo.pho1_hoe = diphoPtr->leadingPhoton()->hadronicOverEm();
        diphoInfo.pho1_sieie = diphoPtr->leadingPhoton()->sigmaIetaIeta();
        diphoInfo.pho1_full5x5_r9 = diphoPtr->leadingPhoton()->full5x5_r9();
        diphoInfo.pho1_full5x5_hoe = diphoPtr->leadingPhoton()->hadronicOverEm();
        diphoInfo.pho1_full5x5_sieie = diphoPtr->leadingPhoton()->full5x5_sigmaIetaIeta();
        diphoInfo.pho1_hcalTowerSumEtConeDR03 = diphoPtr->leadingPhoton()->hcalTowerSumEtConeDR03();
        diphoInfo.pho1_trkSumPtHollowConeDR03 = diphoPtr->leadingPhoton()->trkSumPtHollowConeDR03();
        diphoInfo.pho1_pfChgIsoWrtChosenVtx02 = diphoPtr->leadingPhoton()->pfChgIsoWrtChosenVtx02();
// subleading photon
        diphoInfo.pho2_isEB = diphoPtr->subLeadingPhoton()->isEB();
        diphoInfo.pho2_isEE = diphoPtr->subLeadingPhoton()->isEE();
        diphoInfo.pho2_pt = diphoPtr->subLeadingPhoton()->pt();
        diphoInfo.pho2_eta = diphoPtr->subLeadingPhoton()->eta();
        diphoInfo.pho2_phi = diphoPtr->subLeadingPhoton()->phi();
        diphoInfo.pho2_energy = diphoPtr->subLeadingPhoton()->energy();
        diphoInfo.pho2_r9 = diphoPtr->subLeadingPhoton()->r9();
        diphoInfo.pho2_hoe = diphoPtr->subLeadingPhoton()->hadronicOverEm();
        diphoInfo.pho2_sieie = diphoPtr->subLeadingPhoton()->sigmaIetaIeta();
        diphoInfo.pho2_full5x5_r9 = diphoPtr->subLeadingPhoton()->full5x5_r9();
        diphoInfo.pho2_full5x5_hoe = diphoPtr->subLeadingPhoton()->hadronicOverEm();
        diphoInfo.pho2_full5x5_sieie = diphoPtr->subLeadingPhoton()->full5x5_sigmaIetaIeta();
        diphoInfo.pho2_hcalTowerSumEtConeDR03 = diphoPtr->subLeadingPhoton()->hcalTowerSumEtConeDR03();
        diphoInfo.pho2_trkSumPtHollowConeDR03 = diphoPtr->subLeadingPhoton()->trkSumPtHollowConeDR03();
        diphoInfo.pho2_pfChgIsoWrtChosenVtx02 = diphoPtr->subLeadingPhoton()->pfChgIsoWrtChosenVtx02();
// diphoton info
        diphoInfo.pt = diphoPtr->pt();
        diphoInfo.eta = diphoPtr->eta();
        diphoInfo.phi = diphoPtr->phi();
        diphoInfo.energy = diphoPtr->energy();
        diphoInfo.mass = diphoPtr->mass();

// ***** *****
// Counters for the efficiencies
// ***** *****
        bool photon_category[8];
        photon_category[0] = ( diphoInfo.pho1_isEB && diphoInfo.pho1_r9 >  .9 );
        photon_category[1] = ( diphoInfo.pho1_isEB && diphoInfo.pho1_r9 <= .9 );
        photon_category[2] = ( diphoInfo.pho1_isEE && diphoInfo.pho1_r9 >  .9 );
        photon_category[3] = ( diphoInfo.pho1_isEE && diphoInfo.pho1_r9 <= .9 );
        photon_category[4] = ( diphoInfo.pho2_isEB && diphoInfo.pho2_r9 >  .9 );
        photon_category[5] = ( diphoInfo.pho2_isEB && diphoInfo.pho2_r9 <= .9 );
        photon_category[6] = ( diphoInfo.pho2_isEE && diphoInfo.pho2_r9 >  .9 );
        photon_category[7] = ( diphoInfo.pho2_isEE && diphoInfo.pho2_r9 <= .9 );
// nprocessed
        for( int icat = 0 ; icat < 8 ; icat++ ) {
            if( photon_category[icat] ) { nphoton_processed[icat]++; }
        }
        // setting things up for n-1 efficiencies
        bool hoe_cut[8];
        bool sieie_cut[8];
        bool hcalISO_cut[8];
        bool trkISO_cut[8];
        bool pfISO_cut[8];
        hoe_cut[0]      = ( diphoInfo.pho1_hoe < 0.082 );
        hoe_cut[1]      = ( diphoInfo.pho1_hoe < 0.075 );
        hoe_cut[2]      = ( diphoInfo.pho1_hoe < 0.075 );
        hoe_cut[3]      = ( diphoInfo.pho1_hoe < 0.075 );
        hoe_cut[4]      = ( diphoInfo.pho2_hoe < 0.082 );
        hoe_cut[5]      = ( diphoInfo.pho2_hoe < 0.075 );
        hoe_cut[6]      = ( diphoInfo.pho2_hoe < 0.075 );
        hoe_cut[7]      = ( diphoInfo.pho2_hoe < 0.075 );
        sieie_cut[0]    = ( diphoInfo.pho1_sieie < 0.014 );
        sieie_cut[1]    = ( diphoInfo.pho1_sieie < 0.014 );
        sieie_cut[2]    = ( diphoInfo.pho1_sieie < 0.034 );
        sieie_cut[3]    = ( diphoInfo.pho1_sieie < 0.034 );
        sieie_cut[4]    = ( diphoInfo.pho2_sieie < 0.014 );
        sieie_cut[5]    = ( diphoInfo.pho2_sieie < 0.014 );
        sieie_cut[6]    = ( diphoInfo.pho2_sieie < 0.034 );
        sieie_cut[7]    = ( diphoInfo.pho2_sieie < 0.034 );
        hcalISO_cut[0]  = ( diphoInfo.pho1_hcalTowerSumEtConeDR03 - 0.005 * diphoInfo.pho1_pt < 50. );
        hcalISO_cut[1]  = ( diphoInfo.pho1_hcalTowerSumEtConeDR03 - 0.005 * diphoInfo.pho1_pt <  4. );
        hcalISO_cut[2]  = ( diphoInfo.pho1_hcalTowerSumEtConeDR03 - 0.005 * diphoInfo.pho1_pt < 50. );
        hcalISO_cut[3]  = ( diphoInfo.pho1_hcalTowerSumEtConeDR03 - 0.005 * diphoInfo.pho1_pt <  4. );
        hcalISO_cut[4]  = ( diphoInfo.pho2_hcalTowerSumEtConeDR03 - 0.005 * diphoInfo.pho2_pt < 50. );
        hcalISO_cut[5]  = ( diphoInfo.pho2_hcalTowerSumEtConeDR03 - 0.005 * diphoInfo.pho2_pt <  4. );
        hcalISO_cut[6]  = ( diphoInfo.pho2_hcalTowerSumEtConeDR03 - 0.005 * diphoInfo.pho2_pt < 50. );
        hcalISO_cut[7]  = ( diphoInfo.pho2_hcalTowerSumEtConeDR03 - 0.005 * diphoInfo.pho2_pt <  4. );
        trkISO_cut[0]   = ( diphoInfo.pho1_trkSumPtHollowConeDR03 - 0.002 * diphoInfo.pho1_pt < 50. );
        trkISO_cut[1]   = ( diphoInfo.pho1_trkSumPtHollowConeDR03 - 0.002 * diphoInfo.pho1_pt <  4. );
        trkISO_cut[2]   = ( diphoInfo.pho1_trkSumPtHollowConeDR03 - 0.002 * diphoInfo.pho1_pt < 50. );
        trkISO_cut[3]   = ( diphoInfo.pho1_trkSumPtHollowConeDR03 - 0.002 * diphoInfo.pho1_pt <  4. );
        trkISO_cut[4]   = ( diphoInfo.pho2_trkSumPtHollowConeDR03 - 0.002 * diphoInfo.pho2_pt < 50. );
        trkISO_cut[5]   = ( diphoInfo.pho2_trkSumPtHollowConeDR03 - 0.002 * diphoInfo.pho2_pt <  4. );
        trkISO_cut[6]   = ( diphoInfo.pho2_trkSumPtHollowConeDR03 - 0.002 * diphoInfo.pho2_pt < 50. );
        trkISO_cut[7]   = ( diphoInfo.pho2_trkSumPtHollowConeDR03 - 0.002 * diphoInfo.pho2_pt <  4. );
        pfISO_cut[0]    = ( diphoInfo.pho1_pfChgIsoWrtChosenVtx02 < 4. );
        pfISO_cut[1]    = ( diphoInfo.pho1_pfChgIsoWrtChosenVtx02 < 4. );
        pfISO_cut[2]    = ( diphoInfo.pho1_pfChgIsoWrtChosenVtx02 < 4. );
        pfISO_cut[3]    = ( diphoInfo.pho1_pfChgIsoWrtChosenVtx02 < 4. );
        pfISO_cut[4]    = ( diphoInfo.pho2_pfChgIsoWrtChosenVtx02 < 4. );
        pfISO_cut[5]    = ( diphoInfo.pho2_pfChgIsoWrtChosenVtx02 < 4. );
        pfISO_cut[6]    = ( diphoInfo.pho2_pfChgIsoWrtChosenVtx02 < 4. );
        pfISO_cut[7]    = ( diphoInfo.pho2_pfChgIsoWrtChosenVtx02 < 4. );
        bool allcut[8];
        for( int icat = 0 ; icat < 8 ; icat++ ) {
            allcut[icat]      = hoe_cut[icat] && sieie_cut[icat] && hcalISO_cut[icat] && trkISO_cut[icat] && pfISO_cut[icat];
        }

        for( int icat = 0 ; icat < 8 ; icat++ ) {
            if( photon_category[icat] && allcut[icat] )         { nphoton_preselected[icat]++; }
            if( photon_category[icat] && hoe_cut[icat] )        { nphoton_passing_hoe[icat]++; }
            if( photon_category[icat] && sieie_cut[icat] )      { nphoton_passing_sieie[icat]++; }
            if( photon_category[icat] && hcalISO_cut[icat] )    { nphoton_passing_hcalISO[icat]++; }
            if( photon_category[icat] && trkISO_cut[icat] )     { nphoton_passing_trkISO[icat]++; }
            if( photon_category[icat] && pfISO_cut[icat] )      { nphoton_passing_pfISO[icat]++; }
        }
// Fill the trees
        diphotonTree->Fill();
    } // end of loop over diphoton
}

void
DiphotonPreselectionValidationTreeMaker::beginJob()
{
// per-event tree
    eventTree = fs_->make<TTree>( "eventTree", "per-event tree" );
    eventTree->Branch( "ndiphoton", &evInfo.ndiphoton, "ndiphoton/I" );
// per-diphoton tree:
    diphotonTree = fs_->make<TTree>( "diphotonTree", "per-diphoton tree" );
// leading photon
    diphotonTree->Branch( "pho1_isEB", &diphoInfo.pho1_isEB, "pho1_isEB/I" );
    diphotonTree->Branch( "pho1_isEE", &diphoInfo.pho1_isEE, "pho1_isEE/I" );
    diphotonTree->Branch( "pho1_pt", &diphoInfo.pho1_pt, "pho1_pt/F" );
    diphotonTree->Branch( "pho1_eta", &diphoInfo.pho1_eta, "pho1_eta/F" );
    diphotonTree->Branch( "pho1_phi", &diphoInfo.pho1_phi, "pho1_phi/F" );
    diphotonTree->Branch( "pho1_energy", &diphoInfo.pho1_energy, "pho1_energy/F" );
    diphotonTree->Branch( "pho1_r9", &diphoInfo.pho1_r9, "pho1_r9/F" );
    diphotonTree->Branch( "pho1_hoe", &diphoInfo.pho1_hoe, "pho1_hoe/F" );
    diphotonTree->Branch( "pho1_sieie", &diphoInfo.pho1_sieie, "pho1_sieie/F" );
    diphotonTree->Branch( "pho1_full5x5_r9", &diphoInfo.pho1_full5x5_r9, "pho1_full5x5_r9/F" );
    diphotonTree->Branch( "pho1_full5x5_hoe", &diphoInfo.pho1_full5x5_hoe, "pho1_full5x5_hoe/F" );
    diphotonTree->Branch( "pho1_full5x5_sieie", &diphoInfo.pho1_full5x5_sieie, "pho1_full5x5_sieie/F" );
    diphotonTree->Branch( "pho1_hcalTowerSumEtConeDR03", &diphoInfo.pho1_hcalTowerSumEtConeDR03, "pho1_hcalTowerSumEtConeDR03/F" );
    diphotonTree->Branch( "pho1_trkSumPtHollowConeDR03", &diphoInfo.pho1_trkSumPtHollowConeDR03, "pho1_trkSumPtHollowConeDR03/F" );
    diphotonTree->Branch( "pho1_pfChgIsoWrtChosenVtx02", &diphoInfo.pho1_pfChgIsoWrtChosenVtx02, "pho1_pfChgIsoWrtChosenVtx02/F" );
// subleading photon
    diphotonTree->Branch( "pho2_isEB", &diphoInfo.pho2_isEB, "pho2_isEB/I" );
    diphotonTree->Branch( "pho2_isEE", &diphoInfo.pho2_isEE, "pho2_isEE/I" );
    diphotonTree->Branch( "pho2_pt", &diphoInfo.pho2_pt, "pho2_pt/F" );
    diphotonTree->Branch( "pho2_eta", &diphoInfo.pho2_eta, "pho2_eta/F" );
    diphotonTree->Branch( "pho2_phi", &diphoInfo.pho2_phi, "pho2_phi/F" );
    diphotonTree->Branch( "pho2_energy", &diphoInfo.pho2_energy, "pho2_energy/F" );
    diphotonTree->Branch( "pho2_r9", &diphoInfo.pho2_r9, "pho2_r9/F" );
    diphotonTree->Branch( "pho2_hoe", &diphoInfo.pho2_hoe, "pho2_hoe/F" );
    diphotonTree->Branch( "pho2_sieie", &diphoInfo.pho2_sieie, "pho2_sieie/F" );
    diphotonTree->Branch( "pho2_full5x5_r9", &diphoInfo.pho2_full5x5_r9, "pho2_full5x5_r9/F" );
    diphotonTree->Branch( "pho2_full5x5_hoe", &diphoInfo.pho2_full5x5_hoe, "pho2_full5x5_hoe/F" );
    diphotonTree->Branch( "pho2_full5x5_sieie", &diphoInfo.pho2_full5x5_sieie, "pho2_full5x5_sieie/F" );
    diphotonTree->Branch( "pho2_hcalTowerSumEtConeDR03", &diphoInfo.pho2_hcalTowerSumEtConeDR03, "pho2_hcalTowerSumEtConeDR03/F" );
    diphotonTree->Branch( "pho2_trkSumPtHollowConeDR03", &diphoInfo.pho2_trkSumPtHollowConeDR03, "pho2_trkSumPtHollowConeDR03/F" );
    diphotonTree->Branch( "pho2_pfChgIsoWrtChosenVtx02", &diphoInfo.pho2_pfChgIsoWrtChosenVtx02, "pho2_pfChgIsoWrtChosenVtx02/F" );
// diphoton system
    diphotonTree->Branch( "pt", &diphoInfo.pt, "pt/F" );
    diphotonTree->Branch( "eta", &diphoInfo.eta, "eta/F" );
    diphotonTree->Branch( "phi", &diphoInfo.phi, "phi/F" );
    diphotonTree->Branch( "energy", &diphoInfo.energy, "energy/F" );
    diphotonTree->Branch( "mass", &diphoInfo.mass, "mass/F" );


}

void
DiphotonPreselectionValidationTreeMaker::endJob()
{
    edm::LogInfo( "EfficienciesInfo" ) << "Beginning of efficiencies summary";
    for( int ilead = 0 ; ilead < 2 ; ilead++ ) {
        int ntot_preselected        = 0;
        int ntot_processed          = 0;
        int ntot_passing_hoe        = 0;
        int ntot_passing_sieie      = 0;
        int ntot_passing_hcalISO    = 0;
        int ntot_passing_trkISO     = 0;
        int ntot_passing_pfISO      = 0;
        string whichPhoton = ilead == 0 ? "leading" : "subleading";
        whichPhoton += " photon";
        for( int icat = 0 + ilead * 4; icat < 4 + ilead * 4; icat++ ) {
            ntot_processed          += nphoton_processed[icat];
            ntot_preselected        += nphoton_preselected[icat];
            ntot_passing_hoe        += nphoton_passing_hoe[icat];
            ntot_passing_sieie      += nphoton_passing_sieie[icat];
            ntot_passing_hcalISO    += nphoton_passing_hcalISO[icat];
            ntot_passing_trkISO     += nphoton_passing_trkISO[icat];
            ntot_passing_pfISO      += nphoton_passing_pfISO[icat];
        }
        edm::LogVerbatim( "EfficienciesInfo" ) << whichPhoton << ": yields";
        edm::LogVerbatim( "EfficienciesInfo" ) << "category\tprocessed\tpreselected\tpassing_hoe\tpassing_sieie\tpassing_hcalISO\tpassing_trkISO\tpassing_pfISO";
        edm::LogVerbatim( "EfficienciesInfo" ) << "allcat" << "\t" << ntot_processed << "\t" << ntot_preselected << "\t" << ntot_passing_hoe << "\t" <<
                                               ntot_passing_sieie << "\t" << ntot_passing_hcalISO << "\t" << ntot_passing_trkISO << "\t" << ntot_passing_pfISO;
        for( int icat = 0 + ilead * 4; icat < 4 + ilead * 4; icat++ ) {
            edm::LogVerbatim( "EfficienciesInfo" ) << "cat" << icat % 4 << "\t" << nphoton_processed[icat] << "\t" << nphoton_preselected[icat] << "\t" <<
                                                   nphoton_passing_hoe[icat] << "\t" << nphoton_passing_sieie[icat] << "\t" << nphoton_passing_hcalISO[icat] << "\t" << nphoton_passing_trkISO[icat] << "\t" <<
                                                   nphoton_passing_pfISO[icat];
        } // end of loop over categories
        edm::LogVerbatim( "EfficienciesInfo" ) << whichPhoton << ": efficiencies";
        edm::LogVerbatim( "EfficienciesInfo" ) << "category\tprocessed\tpreselected\tpassing_hoe\tpassing_sieie\tpassing_hcalISO\tpassing_trkISO\tpassing_pfISO";
        if( ntot_processed == 0 )
        { edm::LogVerbatim( "EfficienciesInfo" ) << "allcat has no processed event, skipping category"; }
        else
        { edm::LogVerbatim( "EfficienciesInfo" ) << "allcat" << "\t" << ntot_processed << "\t" << ( float )ntot_preselected / ( float )ntot_processed << "\t" << ( float )ntot_passing_hoe / ( float )ntot_processed << "\t" << ( float )ntot_passing_sieie / ( float )ntot_processed << "\t" << ( float )ntot_passing_hcalISO / ( float )ntot_processed << "\t" << ( float )ntot_passing_trkISO / ( float )ntot_processed << "\t" << ( float )ntot_passing_pfISO / ( float )ntot_processed; }
        for( int icat = 0 + ilead * 4; icat < 4 + ilead * 4; icat++ ) {
            if( nphoton_processed[icat] == 0 ) {
                edm::LogVerbatim( "EfficienciesInfo" ) << "cat " << icat % 4 << " has no processed event, skipping category";
                continue;
            }
            edm::LogVerbatim( "EfficienciesInfo" ) << "cat" << icat % 4 << "\t" << nphoton_processed[icat] << "\t" << ( float )nphoton_preselected[icat] /
                                                   ( float )nphoton_processed[icat] << "\t" << ( float )nphoton_passing_hoe[icat] / ( float )nphoton_processed[icat] << "\t" <<
                                                   ( float )nphoton_passing_sieie[icat] / ( float )nphoton_processed[icat] << "\t" << ( float )nphoton_passing_hcalISO[icat] /
                                                   ( float )nphoton_processed[icat] << "\t" << ( float )nphoton_passing_trkISO[icat] / ( float )nphoton_processed[icat] << "\t" <<
                                                   ( float )nphoton_passing_pfISO[icat] / ( float )nphoton_processed[icat];
        }
    } // end of loop over lead / sublead
    edm::LogInfo( "EfficienciesInfo" ) << "End of efficiencies summary";
} // end of endJob

void
DiphotonPreselectionValidationTreeMaker::initEventStructure()
{
// per-event tree:
    evInfo.ndiphoton = 0;

// per-photon tree:
// leading photon
    diphoInfo.pho1_isEB = -99;
    diphoInfo.pho1_isEE = -99;
    diphoInfo.pho1_pt = -99.;
    diphoInfo.pho1_eta = -99.;
    diphoInfo.pho1_phi = -99.;
    diphoInfo.pho1_energy = -99.;
    diphoInfo.pho1_r9 = -99.;
    diphoInfo.pho1_hoe = -99.;
    diphoInfo.pho1_sieie = -99.;
    diphoInfo.pho1_full5x5_r9 = -99.;
    diphoInfo.pho1_full5x5_hoe = -99.;
    diphoInfo.pho1_full5x5_sieie = -99.;
    diphoInfo.pho1_hcalTowerSumEtConeDR03 = -99.;
    diphoInfo.pho1_trkSumPtHollowConeDR03 = -99.;
    diphoInfo.pho1_pfChgIsoWrtChosenVtx02 = -99.;
// subleading photon
    diphoInfo.pho2_isEB = -99;
    diphoInfo.pho2_isEE = -99;
    diphoInfo.pho2_pt = -99.;
    diphoInfo.pho2_eta = -99.;
    diphoInfo.pho2_phi = -99.;
    diphoInfo.pho2_energy = -99.;
    diphoInfo.pho2_r9 = -99.;
    diphoInfo.pho2_hoe = -99.;
    diphoInfo.pho2_sieie = -99.;
    diphoInfo.pho2_full5x5_r9 = -99.;
    diphoInfo.pho2_full5x5_hoe = -99.;
    diphoInfo.pho2_full5x5_sieie = -99.;
    diphoInfo.pho2_hcalTowerSumEtConeDR03 = -99.;
    diphoInfo.pho2_trkSumPtHollowConeDR03 = -99.;
    diphoInfo.pho2_pfChgIsoWrtChosenVtx02 = -99.;
// diphoton system
    diphoInfo.pt = -99.;
    diphoInfo.eta = -99.;
    diphoInfo.phi = -99.;
    diphoInfo.energy = -99.;
    diphoInfo.mass = -99.;

};


void
DiphotonPreselectionValidationTreeMaker::fillDescriptions( edm::ConfigurationDescriptions &descriptions )
{
    //The following says we do not know what parameters are allowed so do no validation
    // Please change this to state exactly what you do use, even if it is no parameters
    edm::ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault( desc );
}

typedef DiphotonPreselectionValidationTreeMaker FlashggDiphotonPreselectionValidationTreeMaker;
DEFINE_FWK_MODULE( FlashggDiphotonPreselectionValidationTreeMaker );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

