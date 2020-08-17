
// -*- C++ -*-
//
// Package:    Commissioning/vertexTrainingTreeMaker
// Class:      vertexTrainingTreeMaker
//
/**\class vertexTrainingTreeMaker vertexTrainingTreeMaker.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Julie Malcles
//         Created:  Tue, 02 Dec 2014 10:57:22 GMT
//
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
//#include "DataFormats/Common/interface/PtrVector.h"

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

struct SignalInfo {

    int nvertex;
    int ndipho;
    int dipho_index;

    float LogSumPt2;
    float PtBal;
    float PtAsym;
    float NConv;
    float PullConv;

};

struct BackgroundInfo {

    int nvertex;
    int ndipho;
    int dipho_index;

    float LogSumPt2;
    float PtBal;
    float PtAsym;
    float NConv;
    float PullConv;

};

// **********************************************************************

using namespace std;
using namespace edm;
using namespace flashgg;


// **********************************************************************

class vertexTrainingTreeMaker : public edm::EDAnalyzer
{
public:
    explicit vertexTrainingTreeMaker( const edm::ParameterSet & );
    ~vertexTrainingTreeMaker();

    static void fillDescriptions( edm::ConfigurationDescriptions &descriptions );


private:

    edm::Service<TFileService> fs_;



    virtual void beginJob() override;
    virtual void analyze( const edm::Event &, const edm::EventSetup & ) override;
    virtual void endJob() override;

    void initEventStructure();
    int mcTruthVertexIndex( const std::vector<edm::Ptr<reco::GenParticle> > &genParticles, const std::vector<edm::Ptr<reco::Vertex> > &, double dzMatch = 0.1 );
    int sortedIndex( const unsigned int trueVtxIndex, const unsigned int sizemax, const Ptr<flashgg::DiPhotonCandidate> diphoPtr );

    //  edm::EDGetTokenT<edm::View<flashgg::Photon> >            photonToken_;
    edm::EDGetTokenT<edm::View<flashgg::DiPhotonCandidate> > diphotonToken_;
    edm::EDGetTokenT<edm::View<reco::Vertex> >               vertexToken_;

    //edm::EDGetTokenT<View<reco::Conversion> > conversionToken_;
    edm::EDGetTokenT<reco::BeamSpot > beamSpotToken_;
    edm::EDGetTokenT<VertexCandidateMap> vertexCandidateMapTokenDz_;
    edm::EDGetTokenT<View<reco::GenParticle> > genParticleToken_;


    TTree *signalTree;
    TTree *backgroundTree;

    BackgroundInfo bkgInfo;
    SignalInfo sigInfo;
    GenInfo genInfo;



};

// ******************************************************************************************


//
// constructors and destructor
//
vertexTrainingTreeMaker::vertexTrainingTreeMaker( const edm::ParameterSet &iConfig ):
    //  photonToken_(consumes<View<flashgg::Photon> >(iConfig.getUntrackedParameter<InputTag> ("PhotonTag", InputTag("flashggPhotons")))),
    diphotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
    vertexToken_( consumes<View<reco::Vertex> >( iConfig.getUntrackedParameter<InputTag> ( "VertexTag", InputTag( "offlineSlimmedPrimaryVertices" ) ) ) ),
    //  conversionToken_(consumes<View<reco::Conversion> >(iConfig.getUntrackedParameter<InputTag>("ConversionTag",InputTag("reducedConversions")))),
    beamSpotToken_( consumes<reco::BeamSpot >( iConfig.getUntrackedParameter<InputTag>( "BeamSpotTag", InputTag( "offlineBeamSpot" ) ) ) ),
    vertexCandidateMapTokenDz_( consumes<VertexCandidateMap>( iConfig.getParameter<InputTag>( "VertexCandidateMapTagDz" ) ) ),
    genParticleToken_( consumes<View<reco::GenParticle> >( iConfig.getUntrackedParameter<InputTag> ( "GenParticleTag", InputTag( "prunedGenParticles" ) ) ) )
{

}


vertexTrainingTreeMaker::~vertexTrainingTreeMaker()
{


}



void
vertexTrainingTreeMaker::analyze( const edm::Event &iEvent, const edm::EventSetup &iSetup )
{

    // ********************************************************************************

    // access edm objects
    /*
    Handle<View<flashgg::Photon> > photons;
    iEvent.getByToken(photonToken_,photons);
    const PtrVector<flashgg::Photon>& photonPointers = photons->ptrVector();
    */

    Handle<View<flashgg::DiPhotonCandidate> > diphotons;
    iEvent.getByToken( diphotonToken_, diphotons );
    const std::vector<edm::Ptr<flashgg::DiPhotonCandidate> > diphotonsPtrs = diphotons->ptrs();


    Handle<View<reco::Vertex> > primaryVertices;
    iEvent.getByToken( vertexToken_, primaryVertices );
//  const PtrVector<reco::Vertex>& pvPointers = primaryVertices->ptrVector();


    /*
    Handle<View<reco::Conversion> > conversions;
    iEvent.getByToken(conversionToken_,conversions);
    const PtrVector<reco::Conversion>& conversionPointers = conversions->ptrVector();
    */

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
    const std::vector<edm::Ptr<reco::GenParticle> > genParticlesPtrs = genParticles->ptrs();



    // ********************************************************************************

    initEventStructure();

    // gen loop.

    for( unsigned int genLoop = 0 ; genLoop < genParticlesPtrs.size(); genLoop++ ) {

        if( genParticlesPtrs[genLoop]->pdgId() == 25 ) {
            genInfo.genVertexZ = genParticlesPtrs[genLoop]->vz();
            genInfo.genHiggsPt = genParticlesPtrs[genLoop]->pt();

            break;
        }
    }

    // diphoton loop

    for( size_t idipho = 0; idipho < diphotonsPtrs.size(); idipho++ ) {

        sigInfo.nvertex = primaryVertices->size();
        sigInfo.ndipho = diphotonsPtrs.size();
        bkgInfo.nvertex = primaryVertices->size();
        bkgInfo.ndipho = diphotonsPtrs.size();

        sigInfo.dipho_index = idipho;
        bkgInfo.dipho_index = idipho;

        // get true vertex index:

        int trueVtxIndexI = mcTruthVertexIndex( genParticles->ptrs(), primaryVertices->ptrs() );
        if( trueVtxIndexI < 0 ) { continue; }

        Ptr<flashgg::DiPhotonCandidate> diphoPtr = diphotonsPtrs[idipho];
        unsigned int trueVtxIndex = trueVtxIndexI;
        int trueVtxSortedIndexI = sortedIndex( trueVtxIndex, primaryVertices->size(), diphoPtr );
        if( trueVtxSortedIndexI < 0 ) { continue; }

        unsigned int trueVtxSortedIndex = trueVtxSortedIndexI;

        // Fill Signal Info
        if( trueVtxSortedIndex < diphoPtr->nVtxInfoSize() ) {
            sigInfo.LogSumPt2 = diphoPtr->logSumPt2( trueVtxSortedIndex );
            sigInfo.PtBal  =  diphoPtr->ptBal( trueVtxSortedIndex );
            sigInfo.PtAsym  =  diphoPtr->ptAsym( trueVtxSortedIndex );
            sigInfo.NConv  =  diphoPtr->nConv( trueVtxSortedIndex );
            sigInfo.PullConv  =  diphoPtr->pullConv( trueVtxSortedIndex );
            signalTree->Fill();
        }

        vector<int>	pvVecNoTrue;
        for( unsigned int i = 0 ; i < primaryVertices->size() ; i++ ) {
            if( i != trueVtxIndex ) { pvVecNoTrue.push_back( i ); }
        }

        int irand = -999;
        if( pvVecNoTrue.size() > 1 ) { irand = rand() % pvVecNoTrue.size(); }

        int randVtxIndex = -999;
        if( irand != -999 ) { randVtxIndex = pvVecNoTrue[irand]; }

        int randVtxSortedIndexI = sortedIndex( randVtxIndex, primaryVertices->size(), diphoPtr );
        if( randVtxSortedIndexI < 0 ) { continue; }
        unsigned int randVtxSortedIndex = randVtxSortedIndexI;

        // Fill Background Info

        if( randVtxSortedIndex < diphoPtr->nVtxInfoSize() ) {
            bkgInfo.LogSumPt2 = diphoPtr->logSumPt2( randVtxSortedIndex );
            bkgInfo.PtBal  =  diphoPtr->ptBal( randVtxSortedIndex );
            bkgInfo.PtAsym  =  diphoPtr->ptAsym( randVtxSortedIndex );
            bkgInfo.NConv  =  diphoPtr->nConv( randVtxSortedIndex );
            bkgInfo.PullConv  =  diphoPtr->pullConv( randVtxSortedIndex );
            backgroundTree->Fill();
        }

    }  // end diphoton candidate loop


}


void
vertexTrainingTreeMaker::beginJob()
{
    signalTree = fs_->make<TTree>( "signalTree", "per-diphoton tree" );
    signalTree->Branch( "nvertex", &sigInfo.nvertex, "nvertex/I" );
    signalTree->Branch( "ndipho", &sigInfo.ndipho, "ndipho/I" );
    signalTree->Branch( "dipho_index", &sigInfo.dipho_index, "dipho_index/I" );
    signalTree->Branch( "LogSumPt2", &sigInfo.LogSumPt2, "LogSumPt2/F" );
    signalTree->Branch( "PtBal", &sigInfo.PtBal, "PtBal/F" );
    signalTree->Branch( "PtAsym", &sigInfo.PtAsym, "PtAsym/F" );
    signalTree->Branch( "NConv", &sigInfo.NConv, "NConv/F" );
    signalTree->Branch( "PullConv", &sigInfo.PullConv, "PullConv/F" );

    backgroundTree = fs_->make<TTree>( "backgroundTree", "per-diphoton tree" );
    backgroundTree->Branch( "nvertex", &bkgInfo.nvertex, "nvertex/I" );
    backgroundTree->Branch( "ndipho", &bkgInfo.ndipho, "ndipho/I" );
    backgroundTree->Branch( "dipho_index", &bkgInfo.dipho_index, "dipho_index/I" );
    backgroundTree->Branch( "LogSumPt2", &bkgInfo.LogSumPt2, "LogSumPt2/F" );
    backgroundTree->Branch( "PtBal", &bkgInfo.PtBal, "PtBal/F" );
    backgroundTree->Branch( "PtAsym", &bkgInfo.PtAsym, "PtAsym/F" );
    backgroundTree->Branch( "NConv", &bkgInfo.NConv, "NConv/F" );
    backgroundTree->Branch( "PullConv", &bkgInfo.PullConv, "PullConv/F" );

}

void
vertexTrainingTreeMaker::endJob()
{
}

void
vertexTrainingTreeMaker::initEventStructure()
{
    genInfo.genVertexZ = -999.;
    genInfo.genHiggsPt = -999.;

    sigInfo.nvertex = -999;
    sigInfo.ndipho = -999;
    sigInfo.dipho_index = -999;

    sigInfo.LogSumPt2  = -999;
    sigInfo.PtBal  = -999;
    sigInfo.PtAsym  = -999;
    sigInfo.NConv  = -999;
    sigInfo.PullConv  = -999;


    bkgInfo.nvertex = -999;
    bkgInfo.ndipho = -999;
    bkgInfo.dipho_index = -999;

    bkgInfo.LogSumPt2  = -999;
    bkgInfo.PtBal  = -999;
    bkgInfo.PtAsym  = -999;
    bkgInfo.NConv  = -999;
    bkgInfo.PullConv  = -999;

}


/*
void
vertexTrainingTreeMaker::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

/*
void
vertexTrainingTreeMaker::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

/*
void
vertexTrainingTreeMaker::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

/*
void
vertexTrainingTreeMaker::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

void
vertexTrainingTreeMaker::fillDescriptions( edm::ConfigurationDescriptions &descriptions )
{
    //The following says we do not know what parameters are allowed so do no validation
    // Please change this to state exactly what you do use, even if it is no parameters
    edm::ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault( desc );
}
int vertexTrainingTreeMaker::sortedIndex( const unsigned int trueVtxIndex, const unsigned int sizemax, const Ptr<flashgg::DiPhotonCandidate> diphoPtr )
{

    for( unsigned int j = 0; j < sizemax; j++ ) {
        int index = diphoPtr->mvaSortedIndex( j );
        if( index < 0 ) { continue; }
        if( ( unsigned int ) index == trueVtxIndex ) { return j; }
    }
    return -1;
}
int vertexTrainingTreeMaker::mcTruthVertexIndex( const std::vector<edm::Ptr<reco::GenParticle> > &genParticles ,
        const std::vector<edm::Ptr<reco::Vertex> > &vertices, double dzMatch )
{

    reco::Vertex::Point hardVertex( 0, 0, 0 );

    for( unsigned int genLoop = 0 ; genLoop < genParticles.size(); genLoop++ ) {

        if( fabs( genParticles[genLoop]->pdgId() ) < 10 || fabs( genParticles[genLoop]->pdgId() ) == 25 ) {
            hardVertex.SetCoordinates( genParticles[genLoop]->vx(), genParticles[genLoop]->vy(), genParticles[genLoop]->vz() );
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



DEFINE_FWK_MODULE( vertexTrainingTreeMaker );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

