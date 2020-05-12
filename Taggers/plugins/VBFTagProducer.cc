
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/VBFDiPhoDiJetMVAResult.h"
#include "flashgg/DataFormats/interface/VBFMVAResult.h"
#include "flashgg/DataFormats/interface/VBFTag.h"
#include "flashgg/DataFormats/interface/VBFTagTruth.h"

#include "DataFormats/Common/interface/RefToPtr.h"

#include "flashgg/DataFormats/interface/PDFWeightObject.h"

#include <vector>
#include <algorithm>

using namespace std;
using namespace edm;

namespace flashgg {

    class VBFTagProducer : public EDProducer
    {

    public:
        VBFTagProducer( const ParameterSet & );

    private:
        void produce( Event &, const EventSetup & ) override;
        int  chooseCategory( float );

        EDGetTokenT<View<DiPhotonCandidate> >      diPhotonToken_;
        EDGetTokenT<View<VBFDiPhoDiJetMVAResult> > vbfDiPhoDiJetMvaResultToken_;
        EDGetTokenT<View<VBFMVAResult> >           vbfMvaResultToken_;
        EDGetTokenT<View<DiPhotonMVAResult> >      mvaResultToken_;
        EDGetTokenT<View<reco::GenParticle> >      genPartToken_;
        EDGetTokenT<View<reco::GenJet> >           genJetToken_;
        edm::EDGetTokenT<vector<flashgg::PDFWeightObject> > WeightToken_;

        string systLabel_;

        bool dropNonGoldData_;
        bool setArbitraryNonGoldMC_;
        bool requireVBFPreselection_;
        bool getQCDWeights_;

        float vbfPreselLeadPtMin_;
        float vbfPreselSubleadPtMin_;

        float vbfPreselPhoIDMVAMin_;

        vector<double> boundaries;

    };

    VBFTagProducer::VBFTagProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        vbfDiPhoDiJetMvaResultToken_( consumes<View<flashgg::VBFDiPhoDiJetMVAResult> >( iConfig.getParameter<InputTag> ( "VBFDiPhoDiJetMVAResultTag" ) ) ),
        mvaResultToken_( consumes<View<flashgg::DiPhotonMVAResult> >( iConfig.getParameter<InputTag> ( "MVAResultTag" ) ) ),
        genPartToken_( consumes<View<reco::GenParticle> >( iConfig.getParameter<InputTag> ( "GenParticleTag" ) ) ),
        genJetToken_ ( consumes<View<reco::GenJet> >( iConfig.getParameter<InputTag> ( "GenJetTag" ) ) ),
        WeightToken_( consumes<vector<flashgg::PDFWeightObject> >( iConfig.getUntrackedParameter<InputTag>( "WeightTag", InputTag( "flashggPDFWeightObject" ) ) ) ),
        systLabel_   ( iConfig.getParameter<string> ( "SystLabel" ) ),
        dropNonGoldData_   ( iConfig.getParameter<bool> ( "DropNonGoldData" ) ),
        setArbitraryNonGoldMC_   ( iConfig.getParameter<bool> ( "SetArbitraryNonGoldMC" ) ),
        requireVBFPreselection_   ( iConfig.getParameter<bool> ( "RequireVBFPreselection" ) ),
        getQCDWeights_( iConfig.getParameter<bool>( "GetQCDWeights" ) ),
        vbfPreselLeadPtMin_( iConfig.getParameter<double>( "VBFPreselLeadPtMin" ) ),
        vbfPreselSubleadPtMin_( iConfig.getParameter<double>( "VBFPreselSubleadPtMin" ) ),
        vbfPreselPhoIDMVAMin_( iConfig.getParameter<double>( "VBFPreselPhoIDMVAMin") )
    {
        boundaries = iConfig.getParameter<vector<double > >( "Boundaries" );
        assert( is_sorted( boundaries.begin(), boundaries.end() ) ); // we are counting on ascending order - update this to give an error message or exception

        produces<vector<VBFTag> >();
        //produces<vector<VBFTagTruth> >();
    }

    int VBFTagProducer::chooseCategory( float mvavalue )
    {
        // should return 0 if mva above all the numbers, 1 if below the first, ..., boundaries.size()-N if below the Nth, ...
        int n;
        for( n = 0 ; n < ( int )boundaries.size() ; n++ ) {
            if( ( double )mvavalue > boundaries[boundaries.size() - n - 1] ) { return n; }
        }
        return -1; // Does not pass, object will not be produced
    }
    
    void VBFTagProducer::produce( Event &evt, const EventSetup & )
    {
        Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
        evt.getByToken( diPhotonToken_, diPhotons );
        
        Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;
        evt.getByToken( mvaResultToken_, mvaResults );
        
        Handle<View<flashgg::VBFDiPhoDiJetMVAResult> > vbfDiPhoDiJetMvaResults;
        evt.getByToken( vbfDiPhoDiJetMvaResultToken_, vbfDiPhoDiJetMvaResults );

        Handle<View<reco::GenParticle> > genParticles;
        Handle<View<reco::GenJet> > genJets;
        
        Handle<vector<flashgg::PDFWeightObject> > WeightHandle;
        if (getQCDWeights_) {
            evt.getByToken( WeightToken_, WeightHandle );
        }

        std::unique_ptr<vector<VBFTag> >      tags  ( new vector<VBFTag> );
        //std::unique_ptr<vector<VBFTagTruth> > truths( new vector<VBFTagTruth> );

        //unsigned int idx = 0;
        //edm::RefProd<vector<VBFTagTruth> > rTagTruth = evt.getRefBeforePut<vector<VBFTagTruth> >();

        unsigned int index_leadq       = std::numeric_limits<unsigned int>::max();
        unsigned int index_subleadq    = std::numeric_limits<unsigned int>::max();
        unsigned int index_subsubleadq = std::numeric_limits<unsigned int>::max();
        float pt_leadq = 0., pt_subleadq = 0., pt_subsubleadq = 0.;
        
        if( ! evt.isRealData() ) {
            evt.getByToken( genPartToken_, genParticles );
            evt.getByToken( genJetToken_, genJets );
            for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) {
                edm::Ptr<reco::GenParticle> part = genParticles->ptrAt( genLoop );
                if( part->isHardProcess() ) {
                    if( abs( part->pdgId() ) <= 5 ) {
                        if( part->pt() > pt_leadq ) {
                            index_subleadq = index_leadq;
                            pt_subleadq = pt_leadq;
                            index_leadq = genLoop;
                            pt_leadq = part->pt();
                        } else if( part->pt() > pt_subleadq ) {
                            index_subsubleadq  = index_subleadq;
                            pt_subsubleadq     = pt_subleadq;
                            index_subleadq = genLoop;
                            pt_subleadq    = part->pt();
                        }else if( part->pt() > pt_subsubleadq ){
                            index_subsubleadq = genLoop;
                            pt_subleadq       = part->pt();
                        }
                    }
                }
            }
        }
        // We are relying on corresponding sets - update this to give an error/exception
        assert( diPhotons->size() == vbfDiPhoDiJetMvaResults->size() ); 
        assert( diPhotons->size() == mvaResults->size() ); // We are relying on corresponding sets - update this to give an error/exception
        //std::cout << "-----------------------------------------------------" << std::endl;
        for( unsigned int candIndex = 0; candIndex < diPhotons->size() ; candIndex++ ) {
            edm::Ptr<flashgg::VBFDiPhoDiJetMVAResult> vbfdipho_mvares = vbfDiPhoDiJetMvaResults->ptrAt( candIndex );
            edm::Ptr<flashgg::DiPhotonMVAResult>      mvares          = mvaResults->ptrAt( candIndex );
            edm::Ptr<flashgg::DiPhotonCandidate>      dipho           = diPhotons->ptrAt( candIndex );
            
            VBFTag tag_obj( dipho, mvares, vbfdipho_mvares );
            tag_obj.setDiPhotonIndex( candIndex );
            tag_obj.setSystLabel    ( systLabel_ );

            tag_obj.includeWeights( *dipho );
            if ( tag_obj.VBFMVA().dijet_Mjj > 0. ) {

                // We don't want to include all the jet weights because btag weights are not relevant
                // tag_obj.includeWeights( *(tag_obj.VBFMVA().leadJet_ptr) );
                // tag_obj.includeWeights( *(tag_obj.VBFMVA().subleadJet_ptr) );

                // So we do some tricky surgery instead, to get only the UnmatchedPUWeight
                float tagcorig = tag_obj.centralWeight();
                float j1corig = tag_obj.VBFMVA().leadJet_ptr->centralWeight();
                float j2corig = tag_obj.VBFMVA().subleadJet_ptr->centralWeight();
                float j1cadjust = tag_obj.VBFMVA().leadJet_ptr->weight("UnmatchedPUWeightCentral"); // stored without scaling to overall central
                float j2cadjust = tag_obj.VBFMVA().subleadJet_ptr->weight("UnmatchedPUWeightCentral"); // stored without scaling to overall central
                float j1upadjust = 1.;
                float j2upadjust = 1.;
                float j1downadjust = 1.;
                float j2downadjust = 1.;
                if (tag_obj.VBFMVA().leadJet_ptr->hasWeight("UnmatchedPUWeightUp01sigma") ) {
                    j1upadjust = tag_obj.VBFMVA().leadJet_ptr->weight("UnmatchedPUWeightUp01sigma")  / j1corig;
                    j1downadjust = tag_obj.VBFMVA().leadJet_ptr->weight("UnmatchedPUWeightDown01sigma") / j1corig;
                }
                if (tag_obj.VBFMVA().leadJet_ptr->hasWeight("UnmatchedPUWeightUp01sigma") ) {
                    j2upadjust = tag_obj.VBFMVA().subleadJet_ptr->weight("UnmatchedPUWeightUp01sigma") / j2corig;
                    j2downadjust = tag_obj.VBFMVA().subleadJet_ptr->weight("UnmatchedPUWeightDown01sigma") / j2corig;
                }
                if (false && systLabel_ == "") {
                    std::cout << "tagcorig j1cadjust j2cadjust j1upadjust j2upadjust j1downadjust j2downadjust ";
                    std::cout << tagcorig << " " << j1cadjust << " "<< j2cadjust << " "<< j1upadjust << " "<< j2upadjust << " "<< j1downadjust << " "<< j2downadjust << std::endl;
                }
                for (auto it = tag_obj.weightListBegin() ; it != tag_obj.weightListEnd(); it++) {
                    tag_obj.setWeight(*it,tag_obj.weight(*it) * j1cadjust * j2cadjust); 
                }
                if (tag_obj.VBFMVA().leadJet_ptr->hasWeight("UnmatchedPUWeightUp01sigma") ) {
                    tag_obj.setWeight("UnmatchedPUWeightUp01sigma", tag_obj.centralWeight() * j1upadjust * j2upadjust );
                    tag_obj.setWeight("UnmatchedPUWeightDown01sigma", tag_obj.centralWeight() * j1downadjust * j2downadjust );
                }

                if (false && systLabel_ == "") {
                    for (auto it = tag_obj.weightListBegin() ; it != tag_obj.weightListEnd(); it++) {
                        std::cout << "SCZ Weight Debug " << *it << " " << tag_obj.weight(*it) << std::endl;
                        
                    }
                }
            }
            
            if ( getQCDWeights_ ) {
                for( unsigned int weight_index = 0; weight_index < (*WeightHandle).size(); weight_index++ ){
                    vector<uint16_t> compressed_weights = (*WeightHandle)[weight_index].pdf_weight_container;
                    std::vector<float> uncompressed = (*WeightHandle)[weight_index].uncompress( compressed_weights );
                    vector<uint16_t> compressed_alpha = (*WeightHandle)[weight_index].alpha_s_container;
                    std::vector<float> uncompressed_alpha = (*WeightHandle)[weight_index].uncompress( compressed_alpha );
                    vector<uint16_t> compressed_scale = (*WeightHandle)[weight_index].qcd_scale_container;
                    std::vector<float> uncompressed_scale = (*WeightHandle)[weight_index].uncompress( compressed_scale );

                    for( unsigned int j=1; j<(*WeightHandle)[weight_index].pdf_weight_container.size();j++ ) {
                        //                        cout << "compresed weight " << j << " " << " " << (*WeightHandle)[weight_index].pdf_weight_container[j] << endl;
                        //                        cout << "uncompressed weight " << j << " " << uncompressed[j] << endl;
                        tag_obj.setPdf(j-1,uncompressed[j]/uncompressed[0]);
                    }
                    //                    for( unsigned int j=0; j<(*WeightHandle)[weight_index].alpha_s_container.size();j++ ) {
                        //                        cout << "compressed variation " << (*WeightHandle)[weight_index].alpha_s_container[j] << endl;
                        //                        cout << "uncompressed variation " << j << " " << uncompressed_alpha[j] << endl;
                    //                    }
                    tag_obj.setAlphaUp(uncompressed_alpha[0]/uncompressed[0]);
                    tag_obj.setAlphaDown(uncompressed_alpha[1]/uncompressed[0]);
                    tag_obj.setScaleUp(0,uncompressed_scale[1]/uncompressed_scale[0]);
                    tag_obj.setScaleDown(0,uncompressed_scale[2]/uncompressed_scale[0]);
                    tag_obj.setScaleUp(1,uncompressed_scale[3]/uncompressed_scale[0]);
                    tag_obj.setScaleDown(1,uncompressed_scale[6]/uncompressed_scale[0]);
                    tag_obj.setScaleUp(2,uncompressed_scale[4]/uncompressed_scale[0]);
                    tag_obj.setScaleDown(2,uncompressed_scale[8]/uncompressed_scale[0]);

                    //                    for( unsigned int j=0; j<(*WeightHandle)[weight_index].qcd_scale_container.size();j++ ) {
                        //                        cout << "compressed scale " << (*WeightHandle)[weight_index].qcd_scale_container[j] << endl;
                        //                        cout << "uncompressed scale " << j << " " << uncompressed_scale[j] << endl;
                    //                    }

                    //                    std::cout << "Alpha Up: " << tag_obj.alphaUp() << std::endl;
                    //                    std::cout << "Alpha Down: " << tag_obj.alphaDown() <<std::endl;

                    //                    for ( unsigned int j = 0 ; j < 3 ; j++) {
                        //                        std::cout << "Scale Up [" << j << "]: " << tag_obj.scaleUp(j) << std::endl;
                        //                        std::cout << "Scale Down [" << j  << "]: " << tag_obj.scaleDown(j) << std::endl;
                    //                    }
                    
                    

                }

            }


            if ( evt.isRealData() ) {
                tag_obj.setIsGold ( evt.run() );
                //                std::cout << "  VBFTagProducer setting isGold using evt.run()=" << evt.run() << " isGold=" << tag_obj.isGold() << std::endl;
            } else {
                if ( setArbitraryNonGoldMC_ ) {
                    if ( (evt.id().event() % 269) >= 231 ) { // gold 2.31, silver 2.69
                        tag_obj.setIsGoldMC( false );
                        //                        std::cout << "  VBFTagProducer setting isGold using evt.id().event()=" << evt.id().event() << " (evt.id().event() % 269)=" << (evt.id().event() % 269) << " isGold=" << tag_obj.isGold() << std::endl;
                    } else {
                        tag_obj.setIsGoldMC( true );
                    }
                } else { // Never arbitrarily set MC isGold to false
                    tag_obj.setIsGoldMC( true );
                }
            }

            if ( dropNonGoldData_ && !tag_obj.isGold() ) {
                //                std::cout << "  VBFTagProducer has designated this event as not isGold so we drop the tag!" << std::endl;
                continue;
            }

            int catnum = chooseCategory( vbfdipho_mvares->vbfDiPhoDiJetMvaResult );
            tag_obj.setCategoryNumber( catnum );
            unsigned int index_gp_leadjet = std::numeric_limits<unsigned int>::max();
            unsigned int index_gp_subleadjet = std::numeric_limits<unsigned int>::max();
            unsigned int index_gp_leadphoton = std::numeric_limits<unsigned int>::max();
            unsigned int index_gp_subleadphoton = std::numeric_limits<unsigned int>::max();
            unsigned int index_gj_leadjet = std::numeric_limits<unsigned int>::max();
            unsigned int index_gj_subleadjet = std::numeric_limits<unsigned int>::max();
            
            float dr_gp_leadjet = 999.;
            float dr_gp_subleadjet = 999.;
            float dr_gp_leadphoton = 999.;
            float dr_gp_subleadphoton = 999.;
            float dr_gj_leadjet = 999.;
            float dr_gj_subleadjet = 999.;
            VBFTagTruth truth_obj;
            if( ! evt.isRealData() ) {
                for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) {
                    edm::Ptr<reco::GenParticle> part = genParticles->ptrAt( genLoop );
                    if( part->isHardProcess() ) {
                        float dr = deltaR( tag_obj.leadingJet().eta(), tag_obj.leadingJet().phi(), part->eta(), part->phi() );
                        if( dr < dr_gp_leadjet ) {
                            dr_gp_leadjet = dr;
                            index_gp_leadjet = genLoop;
                        }
                        dr = deltaR( tag_obj.subLeadingJet().eta(), tag_obj.subLeadingJet().phi(), part->eta(), part->phi() );
                        if( dr < dr_gp_subleadjet ) {
                            dr_gp_subleadjet = dr;
                            index_gp_subleadjet = genLoop;
                        }
                        dr = deltaR( tag_obj.diPhoton()->leadingPhoton()->eta(), tag_obj.diPhoton()->leadingPhoton()->phi(), part->eta(), part->phi() );
                        if( dr < dr_gp_leadphoton ) {
                            dr_gp_leadphoton = dr;
                            index_gp_leadphoton = genLoop;
                        }
                        dr = deltaR( tag_obj.diPhoton()->subLeadingPhoton()->eta(), tag_obj.diPhoton()->subLeadingPhoton()->phi(), part->eta(), part->phi() );
                        if( dr < dr_gp_subleadphoton ) {
                            dr_gp_subleadphoton = dr;
                            index_gp_subleadphoton = genLoop;
                        }
                    }
                }

                if( index_gp_leadjet < std::numeric_limits<unsigned int>::max() ) { truth_obj.setClosestParticleToLeadingJet( genParticles->ptrAt( index_gp_leadjet ) ); }
                if( index_gp_subleadjet < std::numeric_limits<unsigned int>::max() ) { truth_obj.setClosestParticleToSubLeadingJet( genParticles->ptrAt( index_gp_subleadjet ) ); }
                if( index_gp_leadphoton < std::numeric_limits<unsigned int>::max() ) { truth_obj.setClosestParticleToLeadingPhoton( genParticles->ptrAt( index_gp_leadphoton ) ); }
                if( index_gp_subleadphoton < std::numeric_limits<unsigned int>::max() ) { truth_obj.setClosestParticleToSubLeadingPhoton( genParticles->ptrAt( index_gp_subleadphoton ) ); }
                
                for( unsigned int gjLoop = 0 ; gjLoop < genJets->size() ; gjLoop++ ) {
                    edm::Ptr<reco::GenJet> gj = genJets->ptrAt( gjLoop );
                    float dr = deltaR( tag_obj.leadingJet().eta(), tag_obj.leadingJet().phi(), gj->eta(), gj->phi() );
                    if( dr < dr_gj_leadjet ) {
                        dr_gj_leadjet = dr;
                        index_gj_leadjet = gjLoop;
                    }
                    dr = deltaR( tag_obj.subLeadingJet().eta(), tag_obj.subLeadingJet().phi(), gj->eta(), gj->phi() );
                    if( dr < dr_gj_subleadjet ) {
                        dr_gj_subleadjet = dr;
                        index_gj_subleadjet = gjLoop;
                    }
                }
                if( index_gj_leadjet < std::numeric_limits<unsigned int>::max() ) { truth_obj.setClosestGenJetToLeadingJet( genJets->ptrAt( index_gj_leadjet ) ); }
                if( index_gj_subleadjet < std::numeric_limits<unsigned int>::max() ) { truth_obj.setClosestGenJetToSubLeadingJet( genJets->ptrAt( index_gj_subleadjet ) ); }
                
                if( index_leadq < std::numeric_limits<unsigned int>::max() ) { truth_obj.setLeadingParton( genParticles->ptrAt( index_leadq ) ); }
                if( index_subleadq < std::numeric_limits<unsigned int>::max() ) { truth_obj.setSubLeadingParton( genParticles->ptrAt( index_subleadq ) ); }
                if( index_subsubleadq < std::numeric_limits<unsigned int>::max()) { truth_obj.setSubSubLeadingParton( genParticles->ptrAt( index_subsubleadq ));}

                // Yacine: filling tagTruth Tag with 3 jets matchings
                // the idea is to fill the truth_obj using Jack's 
                // implementation

                // photon overlap removal
                std::vector<edm::Ptr<reco::GenJet>> ptOrderedgenJets;
                for( unsigned int jetLoop( 0 ); jetLoop < genJets->size(); jetLoop++ ) {
                    edm::Ptr<reco::GenJet> gj = genJets->ptrAt(jetLoop );
                    unsigned insertionIndex( 0 );
                    for( unsigned int i( 0 ); i < ptOrderedgenJets.size(); i++ ) {
                        if( gj->pt() <= ptOrderedgenJets[i]->pt() && gj != ptOrderedgenJets[i] ) { insertionIndex = i + 1; }
                    }
                    //Remove photons        
                    float dr_leadPhoton    = deltaR( gj->eta(), gj->phi(),dipho->leadingPhoton()->eta(),dipho->leadingPhoton()->phi() ); 
                    float dr_subLeadPhoton = deltaR( gj->eta(), gj->phi(),dipho->subLeadingPhoton()->eta(),dipho->subLeadingPhoton()->phi() ); 
                    if( dr_leadPhoton > 0.1 && dr_subLeadPhoton > 0.1 ) {
                        ptOrderedgenJets.insert( ptOrderedgenJets.begin() + insertionIndex, gj );
                    }
                }

                //truth_obj.setPtOrderedgenJets(ptOrderedgenJets);
                if (ptOrderedgenJets.size() == 1) {
                    truth_obj.setLeadingGenJet(ptOrderedgenJets[0]);
                }
                if (ptOrderedgenJets.size() == 2) {
                    truth_obj.setLeadingGenJet(ptOrderedgenJets[0]);
                    truth_obj.setSubLeadingGenJet(ptOrderedgenJets[1]);
                }
                if (ptOrderedgenJets.size() == 3) {
                    truth_obj.setLeadingGenJet(ptOrderedgenJets[0]);
                    truth_obj.setSubLeadingGenJet(ptOrderedgenJets[1]);
                    truth_obj.setSubSubLeadingGenJet(ptOrderedgenJets[2]);
                }

                //std::cout << "DEBUG tag_obj.subSubLeadingJet_ptr()->pt() == " << tag_obj.subSubLeadingJet_ptr()->pt() << std::endl;
                truth_obj.setLeadingJet      ( tag_obj.leadingJet_ptr() );
                truth_obj.setSubLeadingJet   ( tag_obj.subLeadingJet_ptr() );
                truth_obj.setSubSubLeadingJet( tag_obj.subSubLeadingJet_ptr() );
                truth_obj.setDiPhoton        ( dipho );           

                //-------------------
                // GenParticles matching
                if ( genParticles->size() > 0 ) {
                    float dr(999.0);
                    unsigned gpIndex(0);
                    for (unsigned partLoop(0);partLoop<genParticles->size();partLoop++) {
                        edm::Ptr<reco::GenParticle> particle = genParticles->ptrAt(partLoop);
                        float deltaR_temp = deltaR(tag_obj.leadingJet().eta(),tag_obj.leadingJet().phi(),particle->eta(),particle->phi());
                        if (deltaR_temp < dr) {dr = deltaR_temp; gpIndex = partLoop;}
                    }
                    truth_obj.setClosestParticleToLeadingJet(genParticles->ptrAt(gpIndex));
                }
                ////Sublead
                if (genParticles->size() > 0) {
                    float dr(999.0);
                    unsigned gpIndex(0);
                    for (unsigned partLoop(0);partLoop<genParticles->size();partLoop++) {
                        edm::Ptr<reco::GenParticle> particle = genParticles->ptrAt(partLoop);
                        float deltaR_temp = deltaR(tag_obj.subLeadingJet().eta(),tag_obj.subLeadingJet().phi(),particle->eta(),particle->phi());
                        if (deltaR_temp < dr) {dr = deltaR_temp; gpIndex = partLoop;}
                    }
                    truth_obj.setClosestParticleToSubLeadingJet(genParticles->ptrAt(gpIndex));
                }
                //////Subsublead
                if (genParticles->size() > 0 &&  tag_obj.VBFMVA().hasValidVBFTriJet ) {
                    float dr(999.0);
                    unsigned gpIndex(0);
                    for (unsigned partLoop(0);partLoop<genParticles->size();partLoop++) {
                        edm::Ptr<reco::GenParticle> particle = genParticles->ptrAt(partLoop);
                        float deltaR_temp = deltaR(tag_obj.subSubLeadingJet().eta(),tag_obj.subSubLeadingJet().phi(),particle->eta(),particle->phi());
                        if (deltaR_temp < dr) {dr = deltaR_temp; gpIndex = partLoop;}
                    }
                    truth_obj.setClosestParticleToSubSubLeadingJet(genParticles->ptrAt(gpIndex));
                } 
                ////----------------
                //// GetJet matching
                if ( ptOrderedgenJets.size() > 0) {
                    float dr(999.0);
                    unsigned gpIndex(0);
                    for (unsigned partLoop(0);partLoop<ptOrderedgenJets.size();partLoop++) {
                        edm::Ptr<reco::GenJet> particle = ptOrderedgenJets[partLoop];
                        float deltaR_temp = deltaR(tag_obj.leadingJet().eta(),tag_obj.leadingJet().phi(),particle->eta(),particle->phi());
                        if (deltaR_temp < dr) {dr = deltaR_temp; gpIndex = partLoop;}
                    }
                    truth_obj.setClosestGenJetToLeadingJet(ptOrderedgenJets[gpIndex]);
                }
                //Subl
                if (ptOrderedgenJets.size() > 0) {
                    float dr(999.0);
                    unsigned gpIndex(0);
                    for (unsigned partLoop(0);partLoop<ptOrderedgenJets.size();partLoop++) {
                        edm::Ptr<reco::GenJet> particle = ptOrderedgenJets[partLoop];
                        float deltaR_temp = deltaR(tag_obj.subLeadingJet().eta(),tag_obj.subLeadingJet().phi(),particle->eta(),particle->phi());
                        if (deltaR_temp < dr) {dr = deltaR_temp; gpIndex = partLoop;}
                    }
                    truth_obj.setClosestGenJetToSubLeadingJet(ptOrderedgenJets[gpIndex]);
                }
                //Subsublead
                if (ptOrderedgenJets.size() > 0 &&  tag_obj.VBFMVA().hasValidVBFTriJet ) {
                    float dr(999.0);
                    unsigned gpIndex(0);
                    for (unsigned partLoop(0);partLoop<ptOrderedgenJets.size();partLoop++) {
                        edm::Ptr<reco::GenJet> particle = ptOrderedgenJets[partLoop];
                        float deltaR_temp = deltaR(tag_obj.subSubLeadingJet().eta(),tag_obj.subSubLeadingJet().phi(),particle->eta(),particle->phi());
                        if (deltaR_temp < dr) {dr = deltaR_temp; gpIndex = partLoop;}
                    }
                    truth_obj.setClosestGenJetToSubSubLeadingJet(ptOrderedgenJets[gpIndex]);
                }
                // --------
                //Partons
                //Lead
                std::vector<edm::Ptr<reco::GenParticle>> ptOrderedPartons;
                for (unsigned int genLoop(0);genLoop < genParticles->size();genLoop++) {
                    edm::Ptr<reco::GenParticle> gp = genParticles->ptrAt(genLoop);
                    bool isGluon = abs( gp->pdgId() ) < 7 && gp->numberOfMothers() == 0;
                    bool isQuark = gp->pdgId() == 21 && gp->numberOfMothers() == 0;
                    if (isGluon || isQuark) {
                        unsigned int insertionIndex(0);
                        for (unsigned int parLoop(0);parLoop<ptOrderedPartons.size();parLoop++) {
                            if (gp->pt() < ptOrderedPartons[parLoop]->pt()) { insertionIndex = parLoop + 1; }
                        }
                        ptOrderedPartons.insert( ptOrderedPartons.begin() + insertionIndex, gp);
                    }
                }
                if ( ptOrderedPartons.size() > 0 ) {
                    float dr(999.0);
                    unsigned pIndex(0);
                    for (unsigned partLoop(0);partLoop<ptOrderedPartons.size();partLoop++) {
                        float deltaR_temp = deltaR(tag_obj.leadingJet().eta(),tag_obj.leadingJet().phi(),
                                                   ptOrderedPartons[partLoop]->eta(),ptOrderedPartons[partLoop]->phi());
                        if (deltaR_temp < dr) {dr = deltaR_temp; pIndex = partLoop;}
                    }
                    truth_obj.setClosestPartonToLeadingJet( ptOrderedPartons[pIndex] );
                }
                //Sublead
                if (ptOrderedPartons.size() > 0) {
                    float dr(999.0);
                    unsigned pIndex(0);
                    for (unsigned partLoop(0);partLoop<ptOrderedPartons.size();partLoop++) {
                        float deltaR_temp = deltaR(tag_obj.subLeadingJet().eta(),tag_obj.subLeadingJet().phi(),
                                                   ptOrderedPartons[partLoop]->eta(),ptOrderedPartons[partLoop]->phi());
                        if (deltaR_temp < dr) {dr = deltaR_temp; pIndex = partLoop;}
                    }
                    truth_obj.setClosestPartonToSubLeadingJet( ptOrderedPartons[pIndex] );
                }
                //SubSublead
                if (ptOrderedPartons.size() > 0 && tag_obj.VBFMVA().hasValidVBFTriJet ) {
                    float dr(999.0);
                    unsigned pIndex(0);
                    for (unsigned partLoop(0);partLoop<ptOrderedPartons.size();partLoop++) {
                        float deltaR_temp = deltaR(tag_obj.subSubLeadingJet().eta(),tag_obj.subSubLeadingJet().phi(),
                                                   ptOrderedPartons[partLoop]->eta(),ptOrderedPartons[partLoop]->phi());
                        if (deltaR_temp < dr) {dr = deltaR_temp; pIndex = partLoop;}
                    }
                    truth_obj.setClosestPartonToSubSubLeadingJet( ptOrderedPartons[pIndex] );
                }
                //------------------
                float dr_leadPhoton(999.),dr_subLeadPhoton(999.);
                unsigned gpIndex1(0),gpIndex2(0);
                for (unsigned partLoop(0);partLoop < genParticles->size();partLoop++) {
                    edm::Ptr<reco::GenParticle> particle = genParticles->ptrAt(partLoop);
                    float deltaR_temp = deltaR(dipho->leadingPhoton()->eta(),dipho->leadingPhoton()->phi(),particle->eta(),particle->phi());
                    if (deltaR_temp < dr_leadPhoton) {dr_leadPhoton = deltaR_temp; gpIndex1 = partLoop;}
                    deltaR_temp = deltaR(dipho->subLeadingPhoton()->eta(),dipho->subLeadingPhoton()->phi(),particle->eta(),particle->phi());
                    if (deltaR_temp < dr_subLeadPhoton) {dr_subLeadPhoton = deltaR_temp; gpIndex2 = partLoop;}
                }
                truth_obj.setClosestParticleToLeadingPhoton(genParticles->ptrAt(gpIndex1));
                truth_obj.setClosestParticleToSubLeadingPhoton(genParticles->ptrAt(gpIndex2));
            }            

            bool VBFpresel = 1;
            if ( requireVBFPreselection_ ) {

                /*
                std::cout << "  Requiring VBF Preselection... dijet_LeadJPt=" << tag_obj.VBFMVA().dijet_LeadJPt
                          << " dijet_SubJPt=" << tag_obj.VBFMVA().dijet_SubJPt
                          << " leadPho_PToM=" << tag_obj.VBFMVA().leadPho_PToM
                          << " sublPho_PToM=" << tag_obj.VBFMVA().sublPho_PToM
                          << " dijet_Mjj=" << tag_obj.VBFMVA().dijet_Mjj << std::endl;
                */

                VBFpresel = ( tag_obj.VBFMVA().dijet_LeadJPt > vbfPreselLeadPtMin_ 
                              && tag_obj.VBFMVA().dijet_SubJPt > vbfPreselSubleadPtMin_ 
                              && tag_obj.diPhoton()->leadPhotonId() > vbfPreselPhoIDMVAMin_
                              && tag_obj.diPhoton()->subLeadPhotonId() > vbfPreselPhoIDMVAMin_
                              && tag_obj.VBFMVA().leadPho_PToM > (1./3) 
                              && tag_obj.VBFMVA().sublPho_PToM > (1./4) 
                              && tag_obj.VBFMVA().dijet_Mjj > 250. );

                //                std::cout << "  VBFpresel=" << VBFpresel << std::endl;
            }

            // saving the collection
            if( VBFpresel && tag_obj.categoryNumber() >= 0 ) {
                tags->push_back( tag_obj );
                //if( ! evt.isRealData() ) {
                //    truths->push_back( truth_obj );
                //    tags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<VBFTagTruth> >( rTagTruth, idx++ ) ) );
                //}
            }
        }

        evt.put( std::move( tags ) );
        //evt.put( std::move( truths ) );
    }
}

typedef flashgg::VBFTagProducer FlashggVBFTagProducer;
DEFINE_FWK_MODULE( FlashggVBFTagProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

