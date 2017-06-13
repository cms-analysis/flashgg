#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "RecoJets/JetProducers/interface/PileupJetIdAlgo.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "flashgg/DataFormats/interface/VertexCandidateMap.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "DataFormats/Math/interface/deltaR.h"


using namespace std;
using namespace edm;

namespace flashgg {

    class JetProducer : public EDProducer
    {

    public:
        JetProducer( const ParameterSet & );
        ~JetProducer();
    private:
        void produce( Event &, const EventSetup & ) override;
        EDGetTokenT<View<pat::Jet> > jetToken_;
        EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
        EDGetTokenT<reco::VertexCollection >  vertexToken_;
        EDGetTokenT< VertexCandidateMap > vertexCandidateMapToken_;
        //EDGetTokenT<edm::ValueMap<float> > qgToken_; 
        //edm::InputTag qgToken_;
        edm::InputTag qgVariablesInputTag;
        edm::EDGetTokenT<edm::ValueMap<float> > qgToken;
        unique_ptr<PileupJetIdAlgo>  pileupJetIdAlgo_;
        ParameterSet pileupJetIdParameters_;
        bool usePuppi;
        bool computeSimpleRMS, computeRegVars;
        unsigned jetCollectionIndex_;
        EDGetTokenT<double> rhoToken_;
        EDGetTokenT<View<pat::Jet> > jetDebugToken_;
        bool debug_;
        unsigned pudebug_matched_badrms_, pudebug_matched_;
    };


    JetProducer::JetProducer( const ParameterSet &iConfig ) :
        jetToken_( consumes<View<pat::Jet> >( iConfig.getParameter<InputTag> ( "JetTag" ) ) ),
        diPhotonToken_( consumes<View<DiPhotonCandidate> >( iConfig.getParameter<InputTag>( "DiPhotonTag" ) ) ),
        vertexToken_  ( consumes<reco::VertexCollection>( iConfig.getParameter<InputTag> ( "VertexTag" ) ) ),
        vertexCandidateMapToken_( consumes<VertexCandidateMap>( iConfig.getParameter<InputTag>( "VertexCandidateMapTag" ) )),
        qgVariablesInputTag( iConfig.getParameter<edm::InputTag>( "qgVariablesInputTag" ) ),
        pileupJetIdParameters_( iConfig.getParameter<ParameterSet>( "PileupJetIdParameters" ) ),
        computeSimpleRMS( iConfig.getParameter<bool>( "ComputeSimpleRMS" ) ),
        computeRegVars( iConfig.getParameter<bool>( "ComputeRegVars" ) ),
        jetCollectionIndex_( iConfig.getParameter<unsigned> ( "JetCollectionIndex" ) ),
        //GluonTagSrc_  (iConfig.getParameter<edm::InputTag>("GluonTagSrc") )
        rhoToken_( consumes<double>(iConfig.getParameter<edm::InputTag>("rho") ) ),
        jetDebugToken_( consumes<View<pat::Jet> >( iConfig.getUntrackedParameter<InputTag> ( "JetDebugTag",edm::InputTag("slimmedJets") ) ) ),
        debug_( iConfig.getUntrackedParameter<bool>( "Debug",false ) )
        //        usePuppi( iConfig.getUntrackedParameter<bool>( "UsePuppi", false ) )
    {
        pileupJetIdAlgo_.reset( new PileupJetIdAlgo( pileupJetIdParameters_, true ) );
        //qgToken_ = consumes<edm::ValueMap<float> >(edm::InputTag("GluonTagSrc", "qgLikelihood"));
        qgToken  = consumes<edm::ValueMap<float>>( edm::InputTag( qgVariablesInputTag.label(), "qgLikelihood" ) );
        
        produces<vector<flashgg::Jet> >();
        pudebug_matched_badrms_ = 0;
        pudebug_matched_ = 0;
    }

    JetProducer::~JetProducer()  {
        if (debug_) {
            std::cout << "* Total matching jets for PU Debug: " << pudebug_matched_ << std::endl;
            std::cout << "*     Number with different MVA is: " << pudebug_matched_badrms_ << std::endl;
        }
    }
    
    void JetProducer::produce( Event &evt, const EventSetup & )
    {
        
        // input jets
        Handle<View<pat::Jet> > jets;
        evt.getByToken( jetToken_, jets );
        // const PtrVector<pat::Jet>& jetPointers = jets->ptrVector();

        Handle<View<pat::Jet> > debugJets;
        if (debug_) {
            evt.getByToken( jetDebugToken_, debugJets );
        }
        
        // input DiPhoton candidates
        Handle<View<DiPhotonCandidate> > diPhotons;
        evt.getByToken( diPhotonToken_, diPhotons );
        // const PtrVector<DiPhotonCandidate>& diPhotonPointers = diPhotons->ptrVector();

        Handle<reco::VertexCollection> primaryVertices;
        evt.getByToken( vertexToken_, primaryVertices );
        const reco::VertexCollection & vertexes = *(primaryVertices.product());
        // const PtrVector<reco::Vertex>& pvPointers = primaryVertices->ptrVector();
        // std::cout << " jet test ==" << primaryVertices->size() <<  std::endl;

        Handle<VertexCandidateMap> vertexCandidateMap;
        evt.getByToken( vertexCandidateMapToken_, vertexCandidateMap );
        // std::cout << " vtx map ==" << vertexCandidateMap->size() <<  std::endl;
        // output jets
        
        //edm::Handle<edm::ValueMap<float> > qgHandle; 
        //evt.getByToken(qgToken_, qgHandle);
        
        // input QGL
        //edm::Handle<edm::ValueMap<float> >  qgHandle; 
        //evt.getByToken(GluonTagSrc_, qgHandle);
        edm::Handle<edm::ValueMap<float>> qgHandle;
        evt.getByToken( qgToken, qgHandle );

        edm::Handle< double > rhoHandle;
        evt.getByToken( rhoToken_, rhoHandle);
        double rho = *rhoHandle;
        
        unique_ptr<vector<flashgg::Jet> > jetColl( new vector<flashgg::Jet> );

        if (debug_) {
            for( unsigned int i = 0 ; i < debugJets->size() ; i++ ) {
                if (debugJets->ptrAt(i)->pt() > 20 ) {
                    std::cout << " DEBUG JET pt eta mva " << debugJets->ptrAt(i)->pt() << " " << debugJets->ptrAt(i)->eta() << " " << debugJets->ptrAt(i)->userFloat("pileupJetId:fullDiscriminant") << std::endl;
                }
            }
        }
        
        for( unsigned int i = 0 ; i < jets->size() ; i++ ) {
            Ptr<pat::Jet> pjet = jets->ptrAt( i );
            flashgg::Jet fjet = flashgg::Jet( *pjet );

            //store btagging userfloats
            if (computeRegVars) {

                int nSecVertices = pjet->tagInfoCandSecondaryVertex("pfSecondaryVertex")->nVertices();
                float vtxMass = 0, vtxPx = 0, vtxPy = 0, vtxPz = 0, vtx3DVal = 0, vtx3DSig = 0, vtxPosX = 0, vtxPosY = 0, vtxPosZ = 0;
                int vtxNTracks = 0;

                if(nSecVertices > 0){
                    vtxNTracks = pjet->tagInfoCandSecondaryVertex("pfSecondaryVertex")->secondaryVertex(0).numberOfSourceCandidatePtrs();
                    vtxMass = pjet->tagInfoCandSecondaryVertex("pfSecondaryVertex")->secondaryVertex(0).p4().mass();
                    vtxPx = pjet->tagInfoCandSecondaryVertex("pfSecondaryVertex")->secondaryVertex(0).p4().px();
                    vtxPy = pjet->tagInfoCandSecondaryVertex("pfSecondaryVertex")->secondaryVertex(0).p4().py();
                    vtxPz = pjet->tagInfoCandSecondaryVertex("pfSecondaryVertex")->secondaryVertex(0).p4().pz();
                    vtxPosX = pjet->tagInfoCandSecondaryVertex("pfSecondaryVertex")->secondaryVertex(0).vertex().x();
                    vtxPosY = pjet->tagInfoCandSecondaryVertex("pfSecondaryVertex")->secondaryVertex(0).vertex().y();
                    vtxPosZ = pjet->tagInfoCandSecondaryVertex("pfSecondaryVertex")->secondaryVertex(0).vertex().z();
                    vtx3DVal = pjet->tagInfoCandSecondaryVertex("pfSecondaryVertex")->flightDistance(0).value();
                    vtx3DSig = pjet->tagInfoCandSecondaryVertex("pfSecondaryVertex")->flightDistance(0).significance();
                }

                fjet.addUserFloat("nSecVertices", nSecVertices);
                fjet.addUserFloat("vtxNTracks", vtxNTracks);
                fjet.addUserFloat("vtxMass", vtxMass);
                fjet.addUserFloat("vtxPx", vtxPx);
                fjet.addUserFloat("vtxPy", vtxPy);
                fjet.addUserFloat("vtxPz", vtxPz);
                fjet.addUserFloat("vtxPosX", vtxPosX);
                fjet.addUserFloat("vtxPosY", vtxPosY);
                fjet.addUserFloat("vtxPosZ", vtxPosZ);
                fjet.addUserFloat("vtx3DVal", vtx3DVal);
                fjet.addUserFloat("vtx3DSig", vtx3DSig);

            }

            if (computeSimpleRMS || computeRegVars) {
                float leadTrackPt_ = 0, softLepPt = 0, softLepRatio = 0, softLepDr = 0;
                float sumPtDrSq = 0.;
                float sumPtSq = 0.;
                for ( unsigned k = 0; k < fjet.numberOfSourceCandidatePtrs(); ++k ) {
                    reco::CandidatePtr pfJetConstituent = fjet.sourceCandidatePtr(k);
                    
                    const reco::Candidate* kcand = pfJetConstituent.get();
                    const pat::PackedCandidate* lPack = dynamic_cast<const pat::PackedCandidate *>( kcand );
                    if ( !lPack ) throw cms::Exception( "NoPackedConstituent" ) << " For jet " << i << " failed to get constituent " << k << std::endl;
                    float candPt = kcand->pt();
                    float candDr   = reco::deltaR(*kcand,fjet);
                    sumPtDrSq += candPt*candPt*candDr*candDr;
                    sumPtSq += candPt*candPt;

                    if(lPack->charge() != 0 && candPt > leadTrackPt_) leadTrackPt_ = candPt;

                    if(abs(lPack->pdgId()) == 11 || abs(lPack->pdgId()) == 13) {
                        if(candPt > softLepPt){
                            softLepPt = candPt;
                            softLepRatio = candPt/pjet->pt();
                            softLepDr = candDr;
                        }
                    }
                }
                
                if (computeSimpleRMS) {
                    if (sumPtSq == 0.) throw cms::Exception( "NoConstituents" ) << " For jet " << i << " we get sumPtSq of 0!" << std::endl;
                    fjet.setSimpleRMS( sumPtDrSq / sumPtSq );
                }

                if (computeRegVars) {
                    fjet.addUserFloat("leadTrackPt", leadTrackPt_);
                    fjet.addUserFloat("softLepPt", softLepPt);
                    fjet.addUserFloat("softLepRatio", softLepRatio);
                    fjet.addUserFloat("softLepDr", softLepDr);
                }
            }
            
            //--- Retrieve the q/g likelihood
            float qgLikelihood = -99.0;
            if(qgHandle.isValid()) qgLikelihood = ( *qgHandle )[jets->refAt( i )];;
            fjet.setQGL(qgLikelihood);

            //            if (fjet.pt() > 20.) {
            //                std::cout << "Jet["<< i << "] QGL=" << qgLikelihood << " partonFlavour=" << fjet.partonFlavour() << std::endl;
            //            }

            if ( jetCollectionIndex_ == 0 ) {
                PileupJetIdentifier lPUJetId = pileupJetIdAlgo_->computeIdVariables( pjet.get(), 0., &vertexes[0], vertexes, rho );
                //                fjet.setPuJetId( primaryVertices->ptrAt( 0 ), lPUJetId );
                if (debug_ && fjet.pt() > 20) {
                    std::cout << "[STANDARD] pt=" << fjet.pt() << " eta=" << fjet.eta() 
                              << " lPUJetId RMS, betaStar, mva: " << lPUJetId.RMS() << " " << lPUJetId.betaStar() << " " << lPUJetId.mva() 
                              << "; simpleRMS " << fjet.rms() << "; match=" << (bool)(fjet.genJet()) << std::endl;
                    for( unsigned int i = 0 ; i < debugJets->size() ; i++ ) {
                        if (fabs(debugJets->ptrAt(i)->pt()-fjet.pt()) < 1. && fabs(debugJets->ptrAt(i)->eta()-fjet.eta()) < 0.1
                            && fabs(debugJets->ptrAt(i)->phi()-fjet.phi()) < 0.1 ) {
                            pudebug_matched_++;
                            if (fabs(debugJets->ptrAt(i)->userFloat("pileupJetId:fullDiscriminant") - lPUJetId.mva()) > 0.02 ) {
                                std::cout << "* ---------------------------------" << std::endl;
                                std::cout << "* Matching jet, non-matching PU MVA" << std::endl;
                                std::cout << "* ---------------------------------" << std::endl;
                                std::cout << "*   mva=" << lPUJetId.mva() << " (" << 
                                    debugJets->ptrAt(i)->userFloat("pileupJetId:fullDiscriminant") << ")" << std::endl;
                                std::cout << "*   pt=" << fjet.pt() << " (" << debugJets->ptrAt(i)->pt() << ")" << std::endl;
                                std::cout << "*   eta=" << fjet.eta() << " (" << debugJets->ptrAt(i)->eta() << ")" << std::endl;
                                std::cout << "*   jetPhi=" << lPUJetId.jetPhi() << " (" << debugJets->ptrAt(i)->phi() << ")" << std::endl;
                                std::cout << "*   jetM=" << lPUJetId.jetM() << std::endl;
                                std::cout << "*   RMS=" << lPUJetId.RMS() << std::endl;
                                std::cout << "*   nvtx=" << lPUJetId.nvtx() << std::endl;
                                std::cout << "*   dR2Mean=" << lPUJetId.dR2Mean()      << std::endl;
                                std::cout << "*   nParticles=" << lPUJetId.nParticles()      << std::endl;
                                std::cout << "*   nCharged=" << lPUJetId.nCharged()  << std::endl;
                                std::cout << "*   majW=" << lPUJetId.majW()  << std::endl;
                                std::cout << "*   minW=" << lPUJetId.minW() << std::endl;
                                std::cout << "*   frac01=" << lPUJetId.frac01()   << std::endl;
                                std::cout << "*   frac02=" << lPUJetId.frac02()       << std::endl;
                                std::cout << "*   frac03=" << lPUJetId.frac03()    << std::endl;
                                std::cout << "*   frac04=" << lPUJetId.frac04()    << std::endl;
                                std::cout << "*   ptD=" << lPUJetId.ptD()    << std::endl;
                                std::cout << "*   beta=" << lPUJetId.beta()    << std::endl;
                                std::cout << "*   pull=" << lPUJetId.pull()    << std::endl;
                                std::cout << "*   jetR=" << lPUJetId.jetR()    << std::endl;
                                std::cout << "*   jetRchg=" << lPUJetId.jetRchg()    << std::endl;
                                std::cout << "* ---------------------------------" << std::endl;
                                pudebug_matched_badrms_++;
                            } else {
                                std::cout << "+ ---------------------------------" << std::endl;
                                std::cout << "+ Matching jet, matching PU MVA" << std::endl;
                                std::cout << "+ ---------------------------------" << std::endl;
                                std::cout << "+   mva=" << lPUJetId.mva() << " (" <<
                                    debugJets->ptrAt(i)->userFloat("pileupJetId:fullDiscriminant") << ")" << std::endl;
                                std::cout << "+   pt=" << fjet.pt() << " (" << debugJets->ptrAt(i)->pt() << ")" << std::endl;
                                std::cout << "+   eta=" << fjet.eta() << " (" << debugJets->ptrAt(i)->eta() << ")" << std::endl;
                                std::cout << "+   jetPhi=" << lPUJetId.jetPhi() << " (" << debugJets->ptrAt(i)->phi() << ")" << std::endl;
                                std::cout << "+   jetM=" << lPUJetId.jetM() << std::endl;
                                std::cout << "+   RMS=" << lPUJetId.RMS() << std::endl;
                                std::cout << "+   nvtx=" << lPUJetId.nvtx() << std::endl;
                                std::cout << "+   dR2Mean=" << lPUJetId.dR2Mean()      << std::endl;
                                std::cout << "+   nParticles=" << lPUJetId.nParticles()      << std::endl;
                                std::cout << "+   nCharged=" << lPUJetId.nCharged()  << std::endl;
                                std::cout << "+   majW=" << lPUJetId.majW()  << std::endl;
                                std::cout << "+   minW=" << lPUJetId.minW() << std::endl;
                                std::cout << "+   frac01=" << lPUJetId.frac01()   << std::endl;
                                std::cout << "+   frac02=" << lPUJetId.frac02()       << std::endl;
                                std::cout << "+   frac03=" << lPUJetId.frac03()    << std::endl;
                                std::cout << "+   frac04=" << lPUJetId.frac04()    << std::endl;
                                std::cout << "+   ptD=" << lPUJetId.ptD()    << std::endl;
                                std::cout << "+   beta=" << lPUJetId.beta()    << std::endl;
                                std::cout << "+   pull=" << lPUJetId.pull()    << std::endl;
                                std::cout << "+   jetR=" << lPUJetId.jetR()    << std::endl;
                                std::cout << "+   jetRchg=" << lPUJetId.jetRchg()    << std::endl;
                                std::cout << "+ ---------------------------------" << std::endl;
                            }
                        }
                    }
                }
                fjet.setSimpleMVA( lPUJetId.mva() );
            } else {
                for( unsigned int j = 0 ; j < diPhotons->size() ; j++ ) {
                    Ptr<DiPhotonCandidate> diPhoton = diPhotons->ptrAt( j );
                    if ( diPhoton->jetCollectionIndex() == jetCollectionIndex_ ) {
                        Ptr<reco::Vertex> vtx = diPhoton->vtx();
                        PileupJetIdentifier lPUJetId = pileupJetIdAlgo_->computeIdVariables( pjet.get(), 0., vtx.get(), vertexes, rho );
                        if (debug_ && fjet.pt() > 20) {
                            std::cout << "[NON-STANDARD] pt=" << fjet.pt() << " eta=" << fjet.eta()
                                      << " lPUJetID RMS, betaStar, mva: " << lPUJetId.RMS() << " " << lPUJetId.betaStar() << " " << lPUJetId.mva()
                                      << "; simpleRMS " << fjet.rms() << "; match=" << (bool)(fjet.genJet()) << std::endl;
                            PileupJetIdentifier lPUJetId0 = pileupJetIdAlgo_->computeIdVariables( pjet.get(), 0., &vertexes[0], vertexes, rho );
                            std::cout << "[NON-S W VTX0] pt=" << fjet.pt() << " eta=" << fjet.eta()
                                      << " lPUJetID RMS, betaStar, mva: " << lPUJetId0.RMS() << " " << lPUJetId0.betaStar() << " " << lPUJetId0.mva()
                                      << "; simpleRMS " << fjet.rms() << "; match=" << (bool)(fjet.genJet()) << std::endl;
                        }
                        fjet.setSimpleMVA( lPUJetId.mva() );
                        break;
                    }
                }
            }


            /*
            for( unsigned int j = 0 ; j < diPhotons->size() ; j++ ) {
                Ptr<DiPhotonCandidate> diPhoton = diPhotons->ptrAt( j );
                Ptr<reco::Vertex> vtx = diPhoton->vtx();
                if( !usePuppi ) {
                    if( !fjet.hasPuJetId( vtx ) ) {
                        PileupJetIdentifier lPUJetId = pileupJetIdAlgo_->computeIdVariables( pjet.get(), vtx, *vertexCandidateMap, true );
                        fjet.setPuJetId( vtx, lPUJetId );
                    }
                } else {
                    if( !fjet.hasPuJetId( vtx ) ) {
                        PileupJetIdentifier lPUJetId;
                        lPUJetId.RMS( 0 );
                        lPUJetId.betaStar( 0 );
                        int idFlag( 0 );
                        idFlag += 1 <<  PileupJetIdentifier::kTight;
                        idFlag += 1 <<  PileupJetIdentifier::kMedium;
                        idFlag += 1 <<  PileupJetIdentifier::kLoose;
                        lPUJetId.idFlag( idFlag );
                        fjet.setPuJetId( vtx, lPUJetId ); //temporarily make puppi jets pass
                    }
                }
            }
            if( !usePuppi ) {
                if( primaryVertices->size() > 0 && !fjet.hasPuJetId( primaryVertices->ptrAt( 0 ) ) ) {
                    PileupJetIdentifier lPUJetId = pileupJetIdAlgo_->computeIdVariables( pjet.get(), primaryVertices->ptrAt( 0 ), *vertexCandidateMap, true );
                    fjet.setPuJetId( primaryVertices->ptrAt( 0 ), lPUJetId );
                }
            } else {
                if( !fjet.hasPuJetId( primaryVertices->ptrAt( 0 ) ) ) {
                    PileupJetIdentifier lPUJetId;
                    lPUJetId.RMS( 0 );
                    lPUJetId.betaStar( 0 );
                    int idFlag( 0 );
                    idFlag += 1 <<  PileupJetIdentifier::kTight;
                    idFlag += 1 <<  PileupJetIdentifier::kMedium;
                    idFlag += 1 <<  PileupJetIdentifier::kLoose;
                    lPUJetId.idFlag( idFlag );
                    fjet.setPuJetId( primaryVertices->ptrAt( 0 ), lPUJetId ); //temporarily make puppi jets pass
                }
            }
            */
            jetColl->push_back( fjet );
        }

        evt.put( std::move( jetColl ) );
    }
}

typedef flashgg::JetProducer FlashggJetProducer;
DEFINE_FWK_MODULE( FlashggJetProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

