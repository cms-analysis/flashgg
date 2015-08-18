// By I.Kucher
// Adapted from J.Malcles vertexTrainingTreeMaker, base code from the flashggCommissioning tree maker code  by C. Favaro et al.

//This produces the tree to check the vtxProb value

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
#include "DataFormats/Common/interface/PtrVector.h"

#include "DataFormats/VertexReco/interface/Vertex.h"

#include "flashgg/MicroAOD/interface/VertexSelectorBase.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"

#include "TTree.h"

// **********************************************************************

// define the structures used to create tree branches and fill the trees

struct GenInfo {
    float genVertexZ;
    float genHiggsPt;
};

struct DiPhoInfo {

    int nvertex;
    int ndipho;
    int dipho_index;
    float LogSumPt2;
    float PtBal;
    float PtAsym;
    float NConv;
    float PullConv;
    float MVA0;
    float MVA1;
    float MVA2;
    float DZ1;
    float DZ2;
    float SumPt;
    float DZtrue;
    float PtLead;
    float PtSubLead;
    float evWeight;
    float pt;
    float VtxProb;
};


// **********************************************************************

using namespace std;
using namespace edm;
using namespace flashgg;


// **********************************************************************

class vertexProbTrainingTreeMaker : public edm::EDAnalyzer
{
public:
    explicit vertexProbTrainingTreeMaker( const edm::ParameterSet & );
    ~vertexProbTrainingTreeMaker();

    static void fillDescriptions( edm::ConfigurationDescriptions &descriptions );


private:

    edm::Service<TFileService> fs_;
    virtual void beginJob() override;
    virtual void analyze( const edm::Event &, const edm::EventSetup & ) override;
    virtual void endJob() override;

    void initEventStructure();
    int getMCTruthVertexIndex( const PtrVector<reco::GenParticle> &gens, const edm::PtrVector<reco::Vertex> &, double dzMatch = 0.1 );
    int getSortedIndex( const unsigned int trueVtxIndex, const unsigned int sizemax, const Ptr<flashgg::DiPhotonCandidate> diphoPtr );

    edm::EDGetTokenT<edm::View<flashgg::DiPhotonCandidate> > diphotonToken_;
    edm::EDGetTokenT<edm::View<reco::Vertex> >               vertexToken_;
    edm::EDGetTokenT<reco::BeamSpot > beamSpotToken_;
    edm::EDGetTokenT<VertexCandidateMap> vertexCandidateMapTokenDz_;
    edm::EDGetTokenT<View<reco::GenParticle> > genParticleToken_;


    TTree *diphoTree;

    DiPhoInfo diphoInfo;
    GenInfo genInfo;

    double evWeight;
};

// ******************************************************************************************


//
// constructors and destructor
//
vertexProbTrainingTreeMaker::vertexProbTrainingTreeMaker( const edm::ParameterSet &iConfig ):
    diphotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getUntrackedParameter<InputTag> ( "DiPhotonTag", InputTag( "flashggDiPhotons" ) ) ) ),
    vertexToken_( consumes<View<reco::Vertex> >( iConfig.getUntrackedParameter<InputTag> ( "VertexTag", InputTag( "offlineSlimmedPrimaryVertices" ) ) ) ),
    beamSpotToken_( consumes<reco::BeamSpot >( iConfig.getUntrackedParameter<InputTag>( "BeamSpotTag", InputTag( "offlineBeamSpot" ) ) ) ),
    vertexCandidateMapTokenDz_( consumes<VertexCandidateMap>( iConfig.getParameter<InputTag>( "VertexCandidateMapTagDz" ) ) ),
    genParticleToken_( consumes<View<reco::GenParticle> >( iConfig.getUntrackedParameter<InputTag> ( "GenParticleTag", InputTag( "prunedGenParticles" ) ) ) )
{
    evWeight            = iConfig.getUntrackedParameter<double>( "evWeight", 1.0 );
}


vertexProbTrainingTreeMaker::~vertexProbTrainingTreeMaker()
{


}



void
vertexProbTrainingTreeMaker::analyze( const edm::Event &iEvent, const edm::EventSetup &iSetup )
{

    // ********************************************************************************

    // access edm objects
    Handle<View<flashgg::DiPhotonCandidate> > diphotons;
    iEvent.getByToken( diphotonToken_, diphotons );
    const std::vector<edm::Ptr<flashgg::DiPhotonCandidate> > &diphotonPointers = diphotons->ptrs();

    Handle<View<reco::Vertex> > primaryVertices;
    iEvent.getByToken( vertexToken_, primaryVertices );
    const std::vector<edm::Ptr<reco::Vertex> > &pvPointers = primaryVertices->ptrs();

    Handle<reco::BeamSpot> recoBeamSpotHandle;
    iEvent.getByToken( beamSpotToken_, recoBeamSpotHandle );
    math::XYZPoint beamSpot;
    if( recoBeamSpotHandle.isValid() ) {
        beamSpot = recoBeamSpotHandle->position();
    } else {
        cout << " WARNING NO VALID BEAM SPOT: this should not happen!" << endl;
    }

    Handle<VertexCandidateMap> vertexCandidateMapDz;

    iEvent.getByToken( vertexCandidateMapTokenDz_, vertexCandidateMapDz );

    Handle<View<reco::GenParticle> > genParticles;
    iEvent.getByToken( genParticleToken_, genParticles );
    const std::vector<edm::Ptr<reco::GenParticle> > &gens = genParticles->ptrs();


    // ********************************************************************************

    initEventStructure();

    for( unsigned int genLoop = 0 ; genLoop < gens.size(); genLoop++ ) {

        if( gens[genLoop]->pdgId() == 25 ) {
            genInfo.genVertexZ = gens[genLoop]->vz();
            genInfo.genHiggsPt = gens[genLoop]->pt();
            break;
        }
    }


    for( size_t idipho = 0; idipho < diphotonPointers.size(); idipho++ ) {

        diphoInfo.nvertex = pvPointers.size();
        diphoInfo.ndipho = diphotonPointers.size();

        Ptr<flashgg::DiPhotonCandidate> diphoPtr = diphotonPointers[idipho];

        diphoInfo.dipho_index = idipho;
        diphoInfo.LogSumPt2 = diphoPtr->logSumPt2();
        diphoInfo.PtBal  =  diphoPtr->ptBal();
        diphoInfo.PtAsym  =  diphoPtr->ptAsym();
        diphoInfo.NConv  =  diphoPtr->nConv();
        diphoInfo.PullConv  =  diphoPtr->pullConv();
        diphoInfo.MVA0 = diphoPtr->mva0();
        diphoInfo.MVA1 = diphoPtr->mva1();
        diphoInfo.MVA2 = diphoPtr->mva2();
        diphoInfo.DZ1 = diphoPtr->dZ1();
        diphoInfo.DZ2 = diphoPtr->dZ2();
        diphoInfo.SumPt = diphoPtr->sumPt();
        diphoInfo.PtLead = diphoPtr->leadingPhoton()->pt();
        diphoInfo.PtSubLead = diphoPtr->subLeadingPhoton()->pt();
        diphoInfo.DZtrue = fabs( diphoPtr->vtx()->position().z() - genInfo.genVertexZ );
        diphoInfo.evWeight = ( float )evWeight;
        diphoInfo.pt = diphoPtr->pt();
        diphoInfo.VtxProb = diphoPtr->vtxProbMVA();

        diphoTree->Fill();

    }  // end diphoton candidate loop


}


void
vertexProbTrainingTreeMaker::beginJob()
{
    diphoTree = fs_->make<TTree>( "diphoTree", "per-diphoton tree" );
    diphoTree->Branch( "nvertex", &diphoInfo.nvertex, "nvertex/I" );
    diphoTree->Branch( "ndipho", &diphoInfo.ndipho, "ndipho/I" );
    diphoTree->Branch( "dipho_index", &diphoInfo.dipho_index, "dipho_index/I" );
    diphoTree->Branch( "LogSumPt2", &diphoInfo.LogSumPt2, "LogSumPt2/F" );
    diphoTree->Branch( "PtBal", &diphoInfo.PtBal, "PtBal/F" );
    diphoTree->Branch( "PtAsym", &diphoInfo.PtAsym, "PtAsym/F" );
    diphoTree->Branch( "NConv", &diphoInfo.NConv, "NConv/F" );
    diphoTree->Branch( "PullConv", &diphoInfo.PullConv, "PullConv/F" );
    diphoTree->Branch( "MVA0", &diphoInfo.MVA0, "MVA0/F" );
    diphoTree->Branch( "MVA1", &diphoInfo.MVA1, "MVA1/F" );
    diphoTree->Branch( "MVA2", &diphoInfo.MVA2, "MVA2/F" );
    diphoTree->Branch( "DZ1", &diphoInfo.DZ1, "DZ1/F" );
    diphoTree->Branch( "DZ2", &diphoInfo.DZ2, "DZ2/F" );
    diphoTree->Branch( "SumPt", &diphoInfo.SumPt, "SumPt/F" );
    diphoTree->Branch( "DZtrue", &diphoInfo.DZtrue, "DZtrue/F" );
    diphoTree->Branch( "PtLead", &diphoInfo.PtLead, "PtLead/F" );
    diphoTree->Branch( "PtSubLead", &diphoInfo.PtSubLead, "PtSubLead/F" );
    diphoTree->Branch( "evWeight", &diphoInfo.evWeight, "evWeight/F" );
    diphoTree->Branch( "pt", &diphoInfo.pt, "pt/F" );
    diphoTree->Branch( "vtxProb", &diphoInfo.VtxProb, "vtxProb/F" );
}

void
vertexProbTrainingTreeMaker::endJob()
{
}

void
vertexProbTrainingTreeMaker::initEventStructure()
{
    genInfo.genVertexZ = -999.;
    genInfo.genHiggsPt = -999.;

    diphoInfo.nvertex = -999;
    diphoInfo.ndipho = -999;
    diphoInfo.dipho_index = -999;
    diphoInfo.LogSumPt2  = -999;
    diphoInfo.PtBal  = -999;
    diphoInfo.PtAsym  = -999;
    diphoInfo.NConv  = -999;
    diphoInfo.PullConv  = -999;
    diphoInfo.MVA0 = -999;
    diphoInfo.MVA1 = -999;
    diphoInfo.MVA2 = -999;
    diphoInfo.DZ1 = -999;
    diphoInfo.DZ2 = -999;
    diphoInfo.SumPt = -999;
    diphoInfo.DZtrue = -999;
    diphoInfo.PtLead = -999;
    diphoInfo.PtSubLead = -999;
    diphoInfo.evWeight = -999;
}


/*
void
vertexProbTrainingTreeMaker::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

/*
void
vertexProbTrainingTreeMaker::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

/*
void
vertexProbTrainingTreeMaker::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

/*
void
vertexProbTrainingTreeMaker::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

void
vertexProbTrainingTreeMaker::fillDescriptions( edm::ConfigurationDescriptions &descriptions )
{
    //The following says we do not know what parameters are allowed so do no validation
    // Please change this to state exactly what you do use, even if it is no parameters
    edm::ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault( desc );
}
int vertexProbTrainingTreeMaker::getSortedIndex( const unsigned int trueVtxIndex, const unsigned int sizemax, const Ptr<flashgg::DiPhotonCandidate> diphoPtr )
{

    for( unsigned int j = 0; j < sizemax; j++ ) {
        int index = diphoPtr->mvaSortedIndex( j );
        if( index < 0 ) { continue; }
        if( ( unsigned int ) index == trueVtxIndex ) { return j; }
    }
    return -1;
}
int vertexProbTrainingTreeMaker::getMCTruthVertexIndex( const PtrVector<reco::GenParticle> &gens , const PtrVector<reco::Vertex> &vertices, double dzMatch )
{

    reco::Vertex::Point hardVertex( 0, 0, 0 );

    for( unsigned int genLoop = 0 ; genLoop < gens.size(); genLoop++ ) {

        if( fabs( gens[genLoop]->pdgId() ) < 10 || fabs( gens[genLoop]->pdgId() ) == 25 ) {
            hardVertex.SetCoordinates( gens[genLoop]->vx(), gens[genLoop]->vy(), gens[genLoop]->vz() );
            break;
        }
    }

    int  ivMatch = 0;
    double dzMin = 999;
    for( unsigned int iv = 0; iv < vertices.size(); iv++ ) {
        double dz = fabs( vertices[iv]->z() - hardVertex.z() );
        if( dz < dzMin ) {
            ivMatch = iv;
            dzMin   = dz;
        }
    }


    if( dzMin < dzMatch ) { return ivMatch; }

    return -1;
}



DEFINE_FWK_MODULE( vertexProbTrainingTreeMaker );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
