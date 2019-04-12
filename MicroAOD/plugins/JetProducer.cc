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
        PileupJetIdAlgo::AlgoGBRForestsAndConstants* algoGBRForestsAndConstants_;
        bool usePuppi;
        bool computeSimpleRMS, computeRegVars;
        unsigned jetCollectionIndex_;
        EDGetTokenT<double> rhoToken_;
        EDGetTokenT<View<pat::Jet> > miniaodJetToken_;
        bool debug_;
        unsigned pudebug_matched_badrms_, pudebug_matched_;
        bool doPuJetID_;
        float minPtForEneSum_, maxEtaForEneSum_;
        unsigned int nJetsForEneSum_;
    };


    JetProducer::JetProducer( const ParameterSet &iConfig ) :
        jetToken_( consumes<View<pat::Jet> >( iConfig.getParameter<InputTag> ( "JetTag" ) ) ),
        diPhotonToken_( consumes<View<DiPhotonCandidate> >( iConfig.getParameter<InputTag>( "DiPhotonTag" ) ) ),
        vertexToken_  ( consumes<reco::VertexCollection>( iConfig.getParameter<InputTag> ( "VertexTag" ) ) ),
        vertexCandidateMapToken_( consumes<VertexCandidateMap>( iConfig.getParameter<InputTag>( "VertexCandidateMapTag" ) )),
        qgVariablesInputTag( iConfig.getParameter<edm::InputTag>( "qgVariablesInputTag" ) ),
        computeSimpleRMS( iConfig.getParameter<bool>( "ComputeSimpleRMS" ) ),
        computeRegVars( iConfig.getParameter<bool>( "ComputeRegVars" ) ),
        jetCollectionIndex_( iConfig.getParameter<unsigned> ( "JetCollectionIndex" ) ),
        //GluonTagSrc_  (iConfig.getParameter<edm::InputTag>("GluonTagSrc") )
        rhoToken_( consumes<double>(iConfig.getParameter<edm::InputTag>("rho") ) ),
        miniaodJetToken_( consumes<View<pat::Jet> >( iConfig.getParameter<InputTag> ( "MiniAodJetTag" ) ) ),
        debug_( iConfig.getUntrackedParameter<bool>( "Debug",false ) ),
        doPuJetID_( iConfig.getParameter<bool>( "DoPuJetID") ),
        minPtForEneSum_( iConfig.getParameter<double>("MinPtForEneSum") ),
        maxEtaForEneSum_( iConfig.getParameter<double>("MaxEtaForEneSum") ),
        nJetsForEneSum_( iConfig.getParameter<unsigned int>("NJetsForEneSum") )
        //        usePuppi( iConfig.getUntrackedParameter<bool>( "UsePuppi", false ) )
    {
        auto pileupJetIdParameters = iConfig.getParameter<ParameterSet>( "PileupJetIdParameters" );
        algoGBRForestsAndConstants_ = new PileupJetIdAlgo::AlgoGBRForestsAndConstants(pileupJetIdParameters, true);
        pileupJetIdAlgo_.reset( new PileupJetIdAlgo( algoGBRForestsAndConstants_ ) );
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

        Handle<View<pat::Jet> > miniaodJets;
        evt.getByToken( miniaodJetToken_, miniaodJets );
        
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
            for( unsigned int i = 0 ; i < miniaodJets->size() ; i++ ) {
                if (miniaodJets->ptrAt(i)->pt() > 20 ) {
                    std::cout << " DEBUG JET pt eta mva " << miniaodJets->ptrAt(i)->pt() << " " << miniaodJets->ptrAt(i)->eta() << " " << miniaodJets->ptrAt(i)->userFloat("pileupJetId:fullDiscriminant") << std::endl;
                }
            }
        }
        
        for( unsigned int i = 0 ; i < jets->size() ; i++ ) {

            Ptr<pat::Jet> pjet = jets->ptrAt( i );
            flashgg::Jet fjet = flashgg::Jet( *pjet );

            if (fjet.pt() < 15.) {
                if (debug_) std::cout << "   .. skipping jet with pt < 15 to avoid rare bug" << std::endl;
                continue;
            }

            // Copy over userFloats, userInts, and bDiscriminators from MINIAOD jet collection to 0th vertex
            if (jetCollectionIndex_ == 0) {
                bool matched = false;
                for( unsigned int j = 0 ; j < miniaodJets->size() ; j++ ) {
                    if (reco::deltaR(miniaodJets->ptrAt(j)->eta(),miniaodJets->ptrAt(j)->phi(),fjet.eta(),fjet.phi()) < 0.1) {
                        matched = true;
                        if (debug_) {
                            std::cout << " Matched 0th vertex jet " << i << " (ptRaw=" << fjet.correctedP4("Uncorrected").pt() <<  ")  to MINIAOD jet " << j 
                                      << " (ptRaw=" << miniaodJets->ptrAt(j)->correctedP4("Uncorrected").pt() <<  ")" << std::endl;
                        }
                        for (auto x = miniaodJets->ptrAt(j)->userFloatNames().begin() ; x != miniaodJets->ptrAt(j)->userFloatNames().end() ; x++) {
                            //                            std::cout << "    UserFloat " << *x << " has value " << miniaodJets->ptrAt(j)->userFloat(*x) << std::endl;
                            fjet.addUserFloat(string("mini_")+(*x),miniaodJets->ptrAt(j)->userFloat(*x));
                        }
                        for (auto x = miniaodJets->ptrAt(j)->userIntNames().begin() ; x != miniaodJets->ptrAt(j)->userIntNames().end() ; x++) {
                            //                            std::cout << "    UserInt " << *x << " has value " << miniaodJets->ptrAt(j)->userInt(*x) << std::endl;
                            fjet.addUserInt(string("mini_")+(*x),miniaodJets->ptrAt(j)->userInt(*x));
                        }
                        for (auto x = miniaodJets->ptrAt(j)->getPairDiscri().begin() ; x != miniaodJets->ptrAt(j)->getPairDiscri().end() ; x++) {
                            //                            std::cout << "    bDiscriminator " << x->first << " has value " << x->second << std::endl;
                            fjet.addBDiscriminatorPair(std::make_pair(string("mini_")+(x->first),x->second));
                        }
                    }
                }
                if (!matched) {
                    std::cout << " NO MATCH for 0th vertex jet " << i << " ptRaw,eta is " << fjet.correctedP4("Uncorrected").pt() << " " << fjet.eta() << std::endl;
                }
            }
            
            if (debug_) {
                std::cout << " Start of jet " << i << " pt=" << fjet.pt() << " eta=" << fjet.eta() << std::endl;
                for (auto x = fjet.userFloatNames().begin() ; x != fjet.userFloatNames().end() ; x++) {
                    std::cout << "    UserFloat " << *x << " has value " << fjet.userFloat(*x) << std::endl;                                                                         
                }
                for (auto x = fjet.userIntNames().begin() ; x != fjet.userIntNames().end() ; x++) {
                    std::cout << "    UserInt " << *x << " has value " << fjet.userInt(*x) << std::endl;                                                                             
                }
                for (auto x = fjet.getPairDiscri().begin() ; x != fjet.getPairDiscri().end() ; x++) {
                    std::cout << "    bDiscriminator " << x->first << " has value " << x->second << std::endl;                                                                                         
                }
            }


            //store btagging userfloats
            if (computeRegVars) {
                if (debug_) { std::cout << " start of computeRegVars" << std::endl; }

                int nSecVertices = pjet->tagInfoCandSecondaryVertex("pfSecondaryVertex")->nVertices();
                float vtxMass = 0, vtxPx = 0, vtxPy = 0, vtxPz = 0, vtx3DVal = 0, vtx3DSig = 0, vtxPosX = 0, vtxPosY = 0, vtxPosZ = 0;
                int vtxNTracks = 0;
                //float ptD=0.;

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

                float ptD(0.0), totalMult(0.0), axis1(0.0), axis2(0.0);
                float sum_dEta(0.0), sum_dPhi(0.0), sum_dEta2(0.0), sum_dPhi2(0.0), sum_dEta_dPhi(0.0), sum_weight(0.0), sum_pt(0.0);

                for(const auto & d : pjet->getJetConstituentsQuick()){
                    if (d->charge()) { // charged particles
                        auto p = dynamic_cast<const pat::PackedCandidate*>(d);
                        if (!p) std::cout << "ERROR: QGTagging variables cannot be computed for these jets!" << std::endl;
                        if (!(p->fromPV() > 1 && p->trackHighPurity())) continue;
                        ++totalMult;
                    }   else { // neutral particles
                        if (d->pt() < 1.0) continue;
                        ++totalMult;
                    } // charged, neutral particles

                    float dEta   = d->eta() - pjet->eta();
                    float dPhi   = reco::deltaPhi(d->phi(), pjet->phi());
                    float pT = d->pt();
                    float weight = pT*pT;

                    sum_dEta      += dEta      * weight;
                    sum_dPhi      += dPhi      * weight;
                    sum_dEta2     += dEta*dEta * weight;
                    sum_dEta_dPhi += dEta*dPhi * weight;
                    sum_dPhi2     += dPhi*dPhi * weight;
                    sum_weight     += pT*pT; //(d->pt())*(d->pt());
                    sum_pt         += pT; //d->pt();

                }

                // calculate axis2 and ptD
                if (sum_weight > 0) {
                    ptD = sqrt(sum_weight)/sum_pt;
                    float ave_dEta  = sum_dEta  / sum_weight;
                    float ave_dPhi  = sum_dPhi  / sum_weight;
                    float ave_dEta2 = sum_dEta2 / sum_weight;
                    float ave_dPhi2 = sum_dPhi2 / sum_weight;
                    float a = ave_dEta2 - ave_dEta*ave_dEta;
                    float b = ave_dPhi2 - ave_dPhi*ave_dPhi;
                    float c = -(sum_dEta_dPhi/sum_weight - ave_dEta*ave_dPhi);
                    float delta = sqrt(fabs( (a-b)*(a-b) + 4*c*c ));
                    if(a+b-delta > 0) axis2 = sqrt(0.5*(a+b-delta));
                    else              axis2 = 0.0;
                    if(a+b+delta > 0) axis1 = sqrt(0.5*(a+b+delta));
                    else              axis1 = 0.0;
                }
                
                fjet.addUserFloat("ptD", ptD);                    
                fjet.addUserFloat("totalMult", totalMult);                    
                fjet.addUserFloat("axis1", axis1);                    
                fjet.addUserFloat("axis2", axis2);                    

                if (debug_) { std::cout << " end of computeRegVars" << std::endl; }

            }

            if (computeSimpleRMS || computeRegVars) {

                if (debug_) { std::cout << " start of computeSimpleRMS || computeRegVars" << std::endl; }

                float leadTrackPt_ = 0, softLepPt = 0, softLepRatio = 0, softLepDr = 0;
                float sumPtDrSq = 0.;
                float sumPtSq = 0.;
                float softLepPtRel = 0.;
                float softLepPtRelInv=0.;

                float cone_boundaries[] = { 0.05, 0.1, 0.2, 0.3, 0.4 }; // hardcoded boundaries: should be made configurable
                size_t ncone_boundaries = sizeof(cone_boundaries)/sizeof(float);
                std::vector<float> chEnergies(ncone_boundaries+1,0.);
                std::vector<float> emEnergies(ncone_boundaries+1,0.); 
                std::vector<float> neEnergies(ncone_boundaries+1,0.); 
                std::vector<float> muEnergies(ncone_boundaries+1,0.);
                int numDaug03 = 0;

                int softLepPdgId=0;


                for ( unsigned k = 0; k < fjet.numberOfSourceCandidatePtrs(); ++k ) {
                    reco::CandidatePtr pfJetConstituent = fjet.sourceCandidatePtr(k);
                    
                    const reco::Candidate* kcand = pfJetConstituent.get();
                    const pat::PackedCandidate* lPack = dynamic_cast<const pat::PackedCandidate *>( kcand );
                    if ( !lPack ) throw cms::Exception( "NoPackedConstituent" ) << " For jet " << i << " failed to get constituent " << k << std::endl;
                    float candPt = kcand->pt();
                    float candDr   = reco::deltaR(*kcand,fjet);
                    sumPtDrSq += candPt*candPt*candDr*candDr;
                    sumPtSq += candPt*candPt;

                    if( candPt > 0.3 ) { ++numDaug03; }
                    if(lPack->charge() != 0 && candPt > leadTrackPt_) leadTrackPt_ = candPt;

                    if(abs(lPack->pdgId()) == 11 || abs(lPack->pdgId()) == 13) {
                        if(candPt > softLepPt){
                            softLepPt = candPt;
                            softLepRatio = candPt/pjet->pt();
                            softLepDr = candDr;
                            softLepPtRel = ( pjet->px()*lPack->px() + pjet->py()*lPack->py() + pjet->pz()*lPack->pz() ) / pjet->p();
                            softLepPtRel = sqrt( lPack->p()*lPack->p() - softLepPtRel*softLepPtRel );

                            softLepPtRelInv = ( pjet->px()*lPack->px() + pjet->py()*lPack->py() + pjet->pz()*lPack->pz() ) / lPack->p();
                            softLepPtRelInv = sqrt( pjet->p()*pjet->p() - softLepPtRelInv*softLepPtRelInv );

                            softLepPdgId = lPack->pdgId();
                        }
                    }
                    
                    int pdgid = abs(lPack->pdgId());
                    size_t icone = std::lower_bound(&cone_boundaries[0],&cone_boundaries[ncone_boundaries],candDr) - &cone_boundaries[0];
                    float candEnergy = kcand->energy();
                    // std::cout << "pdgId " << pdgid << " candDr " << candDr << " icone " << icone << " " << candEnergy << std::endl; 
                    if( pdgid == 22 || pdgid == 11 ) {
                        // std::cout << " fill EM" << std::endl;
                        emEnergies[icone] += candEnergy;
                    } else if ( pdgid == 13 ) { 
                        // std::cout << " fill mu" << std::endl;
                        muEnergies[icone] += candEnergy;
                    } else if ( lPack-> charge() != 0 ) {
                        // std::cout << " fill ch" << std::endl;
                        chEnergies[icone] += candEnergy;
                    } else {
                        // std::cout << " fill ne" << std::endl;
                        neEnergies[icone] += candEnergy;
                    }
                }
                
                
                if (debug_) { std::cout << " before set in  computeSimpleRMS || computeRegVars" << std::endl; }
                
                if (computeSimpleRMS) {
                    if (sumPtSq == 0.) throw cms::Exception( "NoConstituents" ) << " For jet " << i << " we get sumPtSq of 0!" << std::endl;
                    fjet.setSimpleRMS( sumPtDrSq / sumPtSq );
                }

                if (computeRegVars) {
                    fjet.addUserFloat("leadTrackPt", leadTrackPt_);
                    fjet.addUserFloat("softLepPt", softLepPt);
                    fjet.addUserFloat("softLepRatio", softLepRatio);
                    fjet.addUserFloat("softLepDr", softLepDr);
                    fjet.addUserFloat("softLepPtRel", softLepPtRel); 
                    fjet.addUserFloat("softLepPtRelInv", softLepPtRelInv); 
                    fjet.addUserInt("softLepPdgId", softLepPdgId); 
                    fjet.addUserInt("numDaug03", numDaug03);

                    //                    for(size_t icone = 0; icone < ncone_boundaries+1; ++icone) {
                    //                         std::cout << "icone " << icone << " " << emEnergies[icone] << " " << muEnergies[icone] << " " << chEnergies[icone] << " " << neEnergies[icone] << std::endl;
                    //                    }
                    
                    if( fjet.pt() > minPtForEneSum_ && abs(fjet.eta()) < maxEtaForEneSum_ && ( nJetsForEneSum_ == 0 || i <= nJetsForEneSum_ ) ) {
                        /// std::cout << "saving cones " << std::endl;
                        fjet.setChEnergies(chEnergies);
                        fjet.setEmEnergies(emEnergies);
                        fjet.setNeEnergies(neEnergies);
                        fjet.setMuEnergies(muEnergies);
                    }
                }

                if (debug_) { std::cout << " end of computeSimpleRMS || computeRegVars" << std::endl; }

            }
            
            //--- Retrieve the q/g likelihood
            float qgLikelihood = -99.0;
            if(qgHandle.isValid()) qgLikelihood = ( *qgHandle )[jets->refAt( i )];;
            fjet.setQGL(qgLikelihood);

            if (debug_) { 
                std::cout << " after setQGL" << std::endl;

                if (fjet.pt() > 20.) {
                    std::cout << "Jet["<< i << "] QGL=" << qgLikelihood << " partonFlavour=" << fjet.partonFlavour() << std::endl;
                }
            }

            if ( doPuJetID_ ) {
                if ( jetCollectionIndex_ == 0 && doPuJetID_ ) {
                    if (debug_) std::cout << "before computeIdVariables" << std::endl;
                    PileupJetIdentifier lPUJetId = pileupJetIdAlgo_->computeIdVariables( pjet.get(), 0., &vertexes[0], vertexes, rho, false );
                    if (debug_) std::cout << "after computeIdVariables" << std::endl;
                    //                fjet.setPuJetId( primaryVertices->ptrAt( 0 ), lPUJetId );
                    if (debug_ && fjet.pt() > 20) {
                        std::cout << "[STANDARD] pt=" << fjet.pt() << " eta=" << fjet.eta() 
                                  << " lPUJetId RMS, betaStar, mva: " << lPUJetId.RMS() << " " << lPUJetId.betaStar() << " " << lPUJetId.mva() 
                                  << "; simpleRMS " << fjet.rms() << "; match=" << (bool)(fjet.genJet()) << std::endl;
                        for( unsigned int i = 0 ; i < miniaodJets->size() ; i++ ) {
                            if (fabs(miniaodJets->ptrAt(i)->pt()-fjet.pt()) < 1. && fabs(miniaodJets->ptrAt(i)->eta()-fjet.eta()) < 0.1
                                && fabs(miniaodJets->ptrAt(i)->phi()-fjet.phi()) < 0.1 ) {
                                pudebug_matched_++;
                                if (fabs(miniaodJets->ptrAt(i)->userFloat("pileupJetId:fullDiscriminant") - lPUJetId.mva()) > 0.02 ) {
                                    std::cout << "* ---------------------------------" << std::endl;
                                    std::cout << "* Matching jet, non-matching PU MVA" << std::endl;
                                    std::cout << "* ---------------------------------" << std::endl;
                                    std::cout << "*   mva=" << lPUJetId.mva() << " (" << 
                                        miniaodJets->ptrAt(i)->userFloat("pileupJetId:fullDiscriminant") << ")" << std::endl;
                                    std::cout << "*   pt=" << fjet.pt() << " (" << miniaodJets->ptrAt(i)->pt() << ")" << std::endl;
                                    std::cout << "*   rawPt" << fjet.correctedP4("Uncorrected").pt() << " (" << miniaodJets->ptrAt(i)->correctedP4("Uncorrected").pt() << ")" << std::endl;
                                    std::cout << "*   eta=" << fjet.eta() << " (" << miniaodJets->ptrAt(i)->eta() << ")" << std::endl;
                                    std::cout << "*   jetPhi=" << lPUJetId.jetPhi() << " (" << miniaodJets->ptrAt(i)->phi() << ")" << std::endl;
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
                                        miniaodJets->ptrAt(i)->userFloat("pileupJetId:fullDiscriminant") << ")" << std::endl;
                                    std::cout << "+   pt=" << fjet.pt() << " (" << miniaodJets->ptrAt(i)->pt() << ")" << std::endl;
                                    std::cout << "+   eta=" << fjet.eta() << " (" << miniaodJets->ptrAt(i)->eta() << ")" << std::endl;
                                    std::cout << "+   jetPhi=" << lPUJetId.jetPhi() << " (" << miniaodJets->ptrAt(i)->phi() << ")" << std::endl;
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
                            PileupJetIdentifier lPUJetId = pileupJetIdAlgo_->computeIdVariables( pjet.get(), 0., vtx.get(), vertexes, rho, false );
                            if (debug_ && fjet.pt() > 20) {
                                std::cout << "[NON-STANDARD] pt=" << fjet.pt() << " eta=" << fjet.eta()
                                          << " lPUJetID RMS, betaStar, mva: " << lPUJetId.RMS() << " " << lPUJetId.betaStar() << " " << lPUJetId.mva()
                                          << "; simpleRMS " << fjet.rms() << "; match=" << (bool)(fjet.genJet()) << std::endl;
                                PileupJetIdentifier lPUJetId0 = pileupJetIdAlgo_->computeIdVariables( pjet.get(), 0., &vertexes[0], vertexes, rho, false );
                                std::cout << "[NON-S W VTX0] pt=" << fjet.pt() << " eta=" << fjet.eta()
                                      << " lPUJetID RMS, betaStar, mva: " << lPUJetId0.RMS() << " " << lPUJetId0.betaStar() << " " << lPUJetId0.mva()
                                          << "; simpleRMS " << fjet.rms() << "; match=" << (bool)(fjet.genJet()) << std::endl;
                            }
                            fjet.setSimpleMVA( lPUJetId.mva() );
                            break;
                        }
                    }
                }
            } else {
                fjet.setSimpleMVA ( -999. );
            }

            if (debug_) {
                std::cout << " Before pushing back jet " << i << " pt=" << fjet.pt() << " eta=" << fjet.eta() << std::endl;
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
        
        if (debug_) std::cout << " before put " << std::endl;

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

