/** file: flashggPuppiProducer.cc ** C/C++ **
 *
 * Author: yhaddad <yhaddad@cern.ch>
 *
 */

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include <memory>
#include <algorithm>    // std::search

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Math/interface/PtEtaPhiMass.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/Common/interface/RefToPtr.h"
#include "DataFormats/Common/interface/Association.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"

#include "CommonTools/PileupAlgos/interface/PuppiContainer.h"

#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/VertexCandidateMap.h"

#include "fastjet/PseudoJet.hh"

using namespace edm;
using namespace std;
using namespace flashgg;

//namespace flashgg {
class flashggPuppiProducer : public EDProducer
{

public:
    flashggPuppiProducer( const ParameterSet & );
    virtual ~flashggPuppiProducer();

    static void fillDescriptions( edm::ConfigurationDescriptions &descriptions );
    // --- puppi stuff
    typedef math::XYZTLorentzVector           LorentzVector;
    typedef std::vector<LorentzVector>        LorentzVectorCollection;
    typedef reco::VertexCollection            VertexCollection;
    typedef edm::View<reco::Candidate >       CandidateView;
    typedef std::vector<reco::PFCandidate >   PFInputCollection;
    typedef std::vector<reco::PFCandidate >   PFOutputCollection;
    typedef edm::View<reco::PFCandidate>      PFView;

private:
    virtual void beginJob() ;
    virtual void produce( edm::Event &, const edm::EventSetup & );
    virtual void endJob() ;

    virtual void beginRun( edm::Run &, edm::EventSetup const & );
    virtual void endRun( edm::Run &, edm::EventSetup const & );
    virtual void beginLuminosityBlock( edm::LuminosityBlock &, edm::EventSetup const & );
    virtual void endLuminosityBlock( edm::LuminosityBlock &, edm::EventSetup const & );


    EDGetTokenT<View<reco::Vertex> >               vertexToken_;
    EDGetTokenT<View<flashgg::DiPhotonCandidate> > diPhotonsToken_;
    EDGetTokenT<View<pat::PackedCandidate> >       pfcandidateToken_;
    EDGetTokenT< VertexCandidateMap >              vertexCandidateMapToken_;


    // --- puppi stuff
    std::string     fPuppiName;
    std::string     fPFName;
    std::string     fPVName;
    bool            fPuppiDiagnostics;
    bool            fPuppiForLeptons;
    bool            fUseDZ;
    float           fDZCut;
    std::unique_ptr< PuppiContainer>     fPuppiContainer;
    std::vector< RecoObj>                fRecoObjCollection;
    std::auto_ptr< PFOutputCollection >  fPuppiCandidates;

    // --- muti vertex stuff
    unsigned     indexVtx_;
    bool         debug_;
    unsigned int eventNb;
};

flashggPuppiProducer::flashggPuppiProducer( const ParameterSet &iConfig ) :
    vertexToken_( consumes<View<reco::Vertex> >( iConfig.getParameter<InputTag>( "vertexName" ) ) ),
    diPhotonsToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag>( "diPhotonTag" ) ) ),
    pfcandidateToken_( consumes<View<pat::PackedCandidate> >( iConfig.getParameter<InputTag>( "candName" ) ) ),
    vertexCandidateMapToken_( consumes<VertexCandidateMap>( iConfig.getParameter<InputTag>( "VertexCandidateMapTag" ) ) )
{

    indexVtx_         = iConfig.getUntrackedParameter<unsigned> ( "vertexIndex",  0 );
    debug_            = iConfig.getUntrackedParameter<bool> ( "debug"      ,  true );

    debug_ = true;

    fPuppiDiagnostics = iConfig.getParameter<bool> ( "puppiDiagnostics" );
    fPuppiForLeptons  = iConfig.getParameter<bool> ( "puppiForLeptons" );
    fUseDZ            = iConfig.getParameter<bool> ( "UseDeltaZCut" );
    fDZCut            = iConfig.getParameter<double>( "DeltaZCut" );

    fPuppiContainer = std::unique_ptr<PuppiContainer> ( new PuppiContainer( iConfig ) );

    produces<edm::ValueMap<float> > ();
    produces<edm::ValueMap<LorentzVector> > ();
    produces< edm::ValueMap<reco::CandidatePtr> >();
    produces<PFOutputCollection>();

    if( fPuppiDiagnostics ) {
        produces<double> ( "PuppiNAlgos" );
        produces<std::vector<double>> ( "PuppiRawAlphas" );
        produces<std::vector<double>> ( "PuppiAlphas" );
        produces<std::vector<double>> ( "PuppiAlphasMed" );
        produces<std::vector<double>> ( "PuppiAlphasRms" );
    }


    if( debug_ ) { std::cout << "  :: process puppi for vertex :: " << indexVtx_ << std::endl; }
    eventNb = 0 ;
}

flashggPuppiProducer::~flashggPuppiProducer() {;}

void
flashggPuppiProducer::produce( Event &evt , const EventSetup & )
{

    if( debug_ ) { std::cout << "------  event :: " << eventNb << "  with smart index :: " << indexVtx_ << std::endl; }
    // --- diphotons
    Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
    evt.getByToken( diPhotonsToken_, diPhotons );
    // const PtrVector<flashgg::DiPhotonCandidate>& diPhotonPointers= diPhotons->ptrVector();

    // --- Primary Vertices in case no diphoton found
    Handle<View<reco::Vertex> > primaryVertices;
    evt.getByToken( vertexToken_, primaryVertices );
    //const PtrVector<reco::Vertex>& pvPtrs = primaryVertices->ptrVector();
    int npv  = 0;
    for( unsigned int i = 0; i < primaryVertices->size(); i++ ) {
        if( !primaryVertices->ptrAt( i )->isFake()  &&
                primaryVertices->ptrAt( i )->ndof() >= 4 &&
                fabs( primaryVertices->ptrAt( i )->z() ) <= 24 ) { npv++; }
    }

    if( debug_ ) { std::cout << setw( 13 ) << " (n good pv) = " << npv << std::endl; }
    // --- packed candicates
    Handle<View<pat::PackedCandidate> > pfCandidates;
    evt.getByToken( pfcandidateToken_, pfCandidates );
    // const PtrVector<pat::PackedCandidate>& pfPtrs = pfCandidates->ptrVector();

    // --- flashgg vertex map
    Handle<VertexCandidateMap> vtxmap;
    evt.getByToken( vertexCandidateMapToken_, vtxmap );

    // ---  Non-const copy so we can sort by candidates
    //      (which may be associated to more than one vertex)
    VertexCandidateMap vtxmapbycand( *vtxmap );
    std::stable_sort( vtxmapbycand.begin(), vtxmapbycand.end(), flashgg::compare_by_cand() );

    edm::Ptr<reco::Vertex> choosenVertex;

    // force the use of the MiniAOD default vertex
    int  diphoton_index  = -1;
    int  counterFindVtx  =  0;
    bool keep_event      = false;


    if( indexVtx_ == 0 ) {
        choosenVertex =  primaryVertices->ptrAt( 0 );
        keep_event = true;
    }

    // Use the other vertices :
    // we are interested only by the di-photon candidates verticies
    // then retain only the vertex of the di-photon that matters
    else {
        for( unsigned int diPhoLoop = 0; diPhoLoop < diPhotons->size() ; diPhoLoop++ ) {
            // we only have a problem if the mutliple diphotons haev different vertices...
            std::cout << "\t *diphoton[" << diPhoLoop << "] --> " << diPhotons->ptrAt( diPhoLoop )->jetCollectionIndex() << std::endl;
            if( diPhotons->ptrAt( diPhoLoop )->jetCollectionIndex() == indexVtx_ ) {
                choosenVertex  = diPhotons->ptrAt( diPhoLoop )->vtx();
                diphoton_index = diPhoLoop;
                keep_event = true;
                break;
            }
        }
    }

    if( debug_ ) std::cout << setw( 13 ) << " nbr(diphoton) = " << diPhotons->size()
                               << setw( 13 ) << " nbr(pfcandid) = " << pfCandidates->size()
                               << setw( 13 ) << " diphoton inx  = " << diphoton_index << std::endl;
    if( debug_ ) { std::cout << setw( 13 ) << " (keep_event)  = " << keep_event << std::endl; }


    // -----------------------------------
    // fill the fRecoObject for puppi
    // -----------------------------------
    fRecoObjCollection.clear();
    if( keep_event ) {
        for( unsigned int pfCandLoop = 0 ; pfCandLoop < pfCandidates->size() ; pfCandLoop++ ) {
            edm::Ptr<pat::PackedCandidate> cand = pfCandidates->ptrAt( pfCandLoop );

            RecoObj pReco;
            pReco.pt       = cand->pt();
            pReco.eta      = cand->eta();
            pReco.phi      = cand->phi();
            pReco.m        = cand->mass();
            pReco.rapidity = cand->rapidity();
            pReco.charge   = cand->charge();


            pReco.dZ    = -9999;
            pReco.d0    = -9999;
            pReco.vtxId = -9999;

            edm::Ptr<reco::Vertex> closestVertex;
            const flashgg::VertexCandidateMap &vtxmap_ = *vtxmap;
            for( flashgg::VertexCandidateMap::const_iterator iv = vtxmap_.begin() ; iv != vtxmap_.end() ; iv++ ) {
                edm::Ptr<reco::Vertex> currentVertex = iv->first;
                //flashgg::compare_with_cand();
                auto bounds = std::equal_range( vtxmap_.begin(), vtxmap_.end(), cand,
                                                flashgg::compare_with_cand() );
                if( bounds.first != bounds.second ) {
                    closestVertex = currentVertex;
                    if( choosenVertex == currentVertex ) {
                        // -- pVtxId == 0 when the cand's associated vertex matches the primary vertex
                        //    puppi standard .... no comment !!

                        pReco.vtxId = 0;
                        counterFindVtx++;
                        break;
                    }
                    pReco.vtxId = 1;
                }
            }
            pReco.dZ = cand->dz( choosenVertex->position() );
            pReco.d0 = cand->dxy( choosenVertex->position() );

            if( closestVertex.isNull() ) { pReco.vtxId = -1; } // means no matchign vertex from map to lpv is found.
            //if(closestVertex != 0) pReco.vtxId = pVtxId;

            pReco.id = 0;
            if( fabs( pReco.charge ) > 0 ) {
                if( closestVertex.isNonnull() && pReco.vtxId == 0 ) {pReco.id = 1;}
                if( closestVertex.isNonnull() && pReco.vtxId  > 0 ) {pReco.id = 2;}

                if( fUseDZ && pReco.dZ > -9999 && closestVertex.isNull() && ( fabs( pReco.dZ ) < fDZCut ) ) {pReco.id = 1;}
                if( fUseDZ && pReco.dZ > -9999 && closestVertex.isNull() && ( fabs( pReco.dZ ) > fDZCut ) ) {pReco.id = 2;}
            }

            //if( debug_ ) std::cout << setw( 18 ) << "(vtxID)   =" << pReco.vtxId
            //                       << setw( 18 ) << "(precoid) =" << pReco.id
            //                       << std::endl;

            fRecoObjCollection.push_back( pReco );
        }
    }
    // -----------------------------------
    // call puppi algorithm
    // -----------------------------------
    std::auto_ptr<edm::ValueMap<reco::CandidatePtr> > pfMap_p( new edm::ValueMap<reco::CandidatePtr>() );
    // for diagnostic
    std::auto_ptr<std::vector<double> > theAlphas( new std::vector<double>() );
    std::auto_ptr<std::vector<double> > theAlphasMed( new std::vector<double>() );
    std::auto_ptr<std::vector<double> > theAlphasRms( new std::vector<double>() );
    std::auto_ptr<std::vector<double> > alphas( new std::vector<double>() );
    std::auto_ptr<double> nalgos( new double() );

    if( keep_event ) {
        fPuppiContainer->initialize( fRecoObjCollection );
        //Compute the weights
        const std::vector<double> lWeights = fPuppiContainer->puppiWeights();
        //Fill it into the event
        std::auto_ptr<edm::ValueMap<float> > lPupOut( new edm::ValueMap<float>() );
        edm::ValueMap<float>::Filler  lPupFiller( *lPupOut );
        lPupFiller.insert( pfCandidates, lWeights.begin(), lWeights.end() );
        lPupFiller.fill();

        // This is a dummy to access the "translate" method which is a
        // non-static member function even though it doesn't need to be.
        // Will fix in the future.
        static const reco::PFCandidate dummySinceTranslateIsNotStatic;

        //Fill a new PF Candidate Collection and write out the ValueMap of the new p4s.
        // Since the size of the ValueMap must be equal to the input collection, we need
        // to search the "puppi" particles to find a match for each input. If none is found,
        // the input is set to have a four-vector of 0,0,0,0
        const std::vector<fastjet::PseudoJet> lCandidates = fPuppiContainer->puppiParticles();
        fPuppiCandidates.reset( new PFOutputCollection );
        std::auto_ptr<edm::ValueMap<LorentzVector> > p4PupOut( new edm::ValueMap<LorentzVector>() );
        LorentzVectorCollection puppiP4s;
        std::vector<reco::CandidatePtr> values( pfCandidates->size() );
        //std::vector<int> values(pfCandidates->size());

        for( auto i0 = pfCandidates->begin(),
                i0begin = pfCandidates->begin(),
                i0end = pfCandidates->end(); i0 != i0end; ++i0 ) {
            //for(unsigned int i0 = 0; i0 < lCandidates.size(); i0++) {
            //reco::PFCandidate pCand;
            auto id = dummySinceTranslateIsNotStatic.translatePdgIdToType( i0->pdgId() );
            const reco::PFCandidate *pPF = dynamic_cast<const reco::PFCandidate *>( &( *i0 ) );
            reco::PFCandidate pCand( pPF ? *pPF : reco::PFCandidate( i0->charge(), i0->p4(), id ) );
            LorentzVector pVec = i0->p4();
            int val = i0 - i0begin;

            // Find the Puppi particle matched to the input collection using the "user_index" of the object.
            auto puppiMatched = find_if( lCandidates.begin(), lCandidates.end(), [&val]( fastjet::PseudoJet const & i ) { return i.user_index() == val; } );
            if( puppiMatched != lCandidates.end() ) {
                pVec.SetPxPyPzE( puppiMatched->px(), puppiMatched->py(), puppiMatched->pz(), puppiMatched->E() );
            } else {
                pVec.SetPxPyPzE( 0, 0, 0, 0 );
            }
            pCand.setP4( pVec );
            puppiP4s.push_back( pVec );
            fPuppiCandidates->push_back( pCand );
        }

        //Compute the modified p4s
        edm::ValueMap<LorentzVector>::Filler  p4PupFiller( *p4PupOut );
        p4PupFiller.insert( pfCandidates, puppiP4s.begin(), puppiP4s.end() );
        p4PupFiller.fill();

        evt.put( lPupOut );
        evt.put( p4PupOut );
        edm::OrphanHandle<reco::PFCandidateCollection> oh = evt.put( fPuppiCandidates );
        for( unsigned int ic = 0, nc = oh->size(); ic < nc; ++ic ) {
            reco::CandidatePtr pkref( oh, ic );
            values[ic] = pkref;

        }

        edm::ValueMap<reco::CandidatePtr>::Filler filler( *pfMap_p );
        filler.insert( pfCandidates, values.begin(), values.end() );
        filler.fill();
        if( fPuppiDiagnostics ) {
            //theAlphas    = (fPuppiContainer->puppiAlphas());
            //theAlphasMed = (fPuppiContainer->puppiAlphasMed());
            //theAlphasRms = (fPuppiContainer->puppiAlphasRMS());
            //alphas       = (fPuppiContainer->puppiRawAlphas());
            //nalgos = fPuppiContainer->puppiNAlgos();
        }
    }
    if( debug_ ) { std::cout << setw( 13 ) << " (pfMap_p).size() == " << pfMap_p->size() << std::endl; }

    evt.put( pfMap_p );
    //////////////////////////////////////////////
    if( fPuppiDiagnostics ) {
        // all the different alphas per particle
        // THE alpha per particle
        evt.put( alphas      , "PuppiRawAlphas" );
        evt.put( nalgos      , "PuppiNAlgos" );
        evt.put( theAlphas   , "PuppiAlphas" );
        evt.put( theAlphasMed, "PuppiAlphasMed" );
        evt.put( theAlphasRms, "PuppiAlphasRms" );
    }
    // -----------------------------------
    // save on the event
    // -----------------------------------

    eventNb++;
}






// ------------------------------------------------------------------------------------------
void flashggPuppiProducer::beginJob()
{
}
// ------------------------------------------------------------------------------------------
void flashggPuppiProducer::endJob()
{
}
// ------------------------------------------------------------------------------------------
void flashggPuppiProducer::beginRun( edm::Run &, edm::EventSetup const & )
{
}
// ------------------------------------------------------------------------------------------
void flashggPuppiProducer::endRun( edm::Run &, edm::EventSetup const & )
{
}
// ------------------------------------------------------------------------------------------
void flashggPuppiProducer::beginLuminosityBlock( edm::LuminosityBlock &, edm::EventSetup const & )
{
}
// ------------------------------------------------------------------------------------------
void flashggPuppiProducer::endLuminosityBlock( edm::LuminosityBlock &, edm::EventSetup const & )
{
}
// ------------------------------------------------------------------------------------------
void flashggPuppiProducer::fillDescriptions( edm::ConfigurationDescriptions &descriptions )
{
    //The following says we do not know what parameters are allowed so do no validation
    // Please change this to state exactly what you do use, even if it is no parameters
    edm::ParameterSetDescription desc;

    desc.setUnknown();
    descriptions.addDefault( desc );
}
//define this as a plug-in
DEFINE_FWK_MODULE( flashggPuppiProducer );
//}// end flashgg::namespace

