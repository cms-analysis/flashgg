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

// define the structures used to create tree branches and fill the trees
// per-event tree:
struct eventInfo {
    int ndiphoton;
};

struct diphotonInfo {
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


class PhotonCorrectionValidationTreeMaker : public edm::EDAnalyzer
{
public:
    explicit PhotonCorrectionValidationTreeMaker( const edm::ParameterSet & );
    ~PhotonCorrectionValidationTreeMaker();
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
// constructors and destructor
//
PhotonCorrectionValidationTreeMaker::PhotonCorrectionValidationTreeMaker( const edm::ParameterSet &iConfig ):
    diphotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) )
{
}

PhotonCorrectionValidationTreeMaker::~PhotonCorrectionValidationTreeMaker()
{
}

void
PhotonCorrectionValidationTreeMaker::analyze( const edm::Event &iEvent, const edm::EventSetup &iSetup )
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
        // diphoton info
        diphoInfo.pt = diphoPtr->pt();
        diphoInfo.eta = diphoPtr->eta();
        diphoInfo.phi = diphoPtr->phi();
        diphoInfo.energy = diphoPtr->energy();
        diphoInfo.mass = diphoPtr->mass();
        // Fill the trees
        diphotonTree->Fill();
    } // end of loop over diphoton
}


void
PhotonCorrectionValidationTreeMaker::beginJob()
{
    // per-event tree
    eventTree = fs_->make<TTree>( "eventTree", "per-event tree" );
    eventTree->Branch( "ndiphoton", &evInfo.ndiphoton, "ndiphoton/I" );
    // per-diphoton tree:
    diphotonTree = fs_->make<TTree>( "diphotonTree", "per-diphoton tree" );
    // diphoton system
    diphotonTree->Branch( "pt", &diphoInfo.pt, "pt/F" );
    diphotonTree->Branch( "eta", &diphoInfo.eta, "eta/F" );
    diphotonTree->Branch( "phi", &diphoInfo.phi, "phi/F" );
    diphotonTree->Branch( "energy", &diphoInfo.energy, "energy/F" );
    diphotonTree->Branch( "mass", &diphoInfo.mass, "mass/F" );
}

void
PhotonCorrectionValidationTreeMaker::endJob()
{

} // end of endJob

void
PhotonCorrectionValidationTreeMaker::initEventStructure()
{
    // per-event tree:
    evInfo.ndiphoton = 0;
    // diphoton system
    diphoInfo.pt = -99.;
    diphoInfo.eta = -99.;
    diphoInfo.phi = -99.;
    diphoInfo.energy = -99.;
    diphoInfo.mass = -99.;
};

void
PhotonCorrectionValidationTreeMaker::fillDescriptions( edm::ConfigurationDescriptions &descriptions )
{
    //The following says we do not know what parameters are allowed so do no validation
    // Please change this to state exactly what you do use, even if it is no parameters
    edm::ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault( desc );
}

typedef PhotonCorrectionValidationTreeMaker FlashggPhotonCorrectionValidationTreeMaker;
DEFINE_FWK_MODULE( FlashggPhotonCorrectionValidationTreeMaker );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

