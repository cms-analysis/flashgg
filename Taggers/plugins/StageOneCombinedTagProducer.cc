#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/StageOneCombinedTag.h"
#include "flashgg/DataFormats/interface/VBFMVAResult.h"
#include "DataFormats/Common/interface/RefToPtr.h"

#include "flashgg/DataFormats/interface/PDFWeightObject.h"

#include <TLorentzVector.h>
#include <vector>
#include <algorithm>

using namespace std;
using namespace edm;

namespace flashgg {

    class StageOneCombinedTagProducer : public EDProducer
    {

    public:
        StageOneCombinedTagProducer( const ParameterSet & );

    private:
        void produce( Event &, const EventSetup & ) override;
        int chooseCategory( float );
        int computeStage1Kinematics( const StageOneCombinedTag );

        std::vector<double> rawDiphoBounds_, rawDijetBounds_, rawGghBounds_;
        std::map<std::string, float> diphoBounds_, dijetBounds_, gghBounds_;
        void constructBounds();

        EDGetTokenT<View<DiPhotonCandidate> >      diPhotonToken_;
        EDGetTokenT<View<VBFMVAResult> >           vbfMvaResultToken_;
        EDGetTokenT<View<DiPhotonMVAResult> >      mvaResultToken_;
        edm::EDGetTokenT<vector<flashgg::PDFWeightObject> > WeightToken_;

        string systLabel_;

        std::vector<edm::EDGetTokenT<View<flashgg::Jet> > > tokenJets_;
        std::vector<edm::InputTag> inputTagJets_;
        typedef std::vector<edm::Handle<edm::View<flashgg::Jet> > > JetCollectionVector;

    };

    StageOneCombinedTagProducer::StageOneCombinedTagProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        vbfMvaResultToken_( consumes<View<flashgg::VBFMVAResult> >( iConfig.getParameter<InputTag> ( "VBFMVAResultTag" ) ) ),
        mvaResultToken_( consumes<View<flashgg::DiPhotonMVAResult> >( iConfig.getParameter<InputTag> ( "MVAResultTag" ) ) ),
        WeightToken_( consumes<vector<flashgg::PDFWeightObject> >( iConfig.getUntrackedParameter<InputTag>( "WeightTag", InputTag( "flashggPDFWeightObject" ) ) ) ),
        systLabel_   ( iConfig.getParameter<string> ( "SystLabel" ) ),
        inputTagJets_ ( iConfig.getParameter<std::vector<edm::InputTag> >( "inputTagJets" ) )
    {
        for (unsigned i = 0 ; i < inputTagJets_.size() ; i++) {
            auto token = consumes<View<flashgg::Jet> >(inputTagJets_[i]);
            tokenJets_.push_back(token);
        }

        rawDiphoBounds_ = iConfig.getParameter<std::vector<double> > ("rawDiphoBounds");
        rawDijetBounds_ = iConfig.getParameter<std::vector<double> > ("rawDijetBounds");
        rawGghBounds_   = iConfig.getParameter<std::vector<double> > ("rawGghBounds");
        constructBounds();

        produces<vector<DiPhotonTagBase> >();
    }

    void StageOneCombinedTagProducer::produce( Event &evt, const EventSetup & )
    {
        Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
        evt.getByToken( diPhotonToken_, diPhotons );
        
        Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;
        evt.getByToken( mvaResultToken_, mvaResults );
        
        Handle<View<flashgg::VBFMVAResult> > vbfMvaResults;
        evt.getByToken( vbfMvaResultToken_, vbfMvaResults );

        JetCollectionVector Jets( inputTagJets_.size() );
        for( size_t j = 0; j < inputTagJets_.size(); ++j ) {
            evt.getByToken( tokenJets_[j], Jets[j] );
        }

        std::unique_ptr<vector<DiPhotonTagBase> > stage1tags( new vector<DiPhotonTagBase> );

        // We are relying on corresponding sets - update this to give an error/exception
        assert( diPhotons->size() == vbfMvaResults->size() ); 
        assert( diPhotons->size() == mvaResults->size() ); // We are relying on corresponding sets - update this to give an error/exception

        for( unsigned int candIndex = 0; candIndex < diPhotons->size() ; candIndex++ ) {
            edm::Ptr<flashgg::VBFMVAResult>           vbf_mvares      = vbfMvaResults->ptrAt( candIndex );
            edm::Ptr<flashgg::DiPhotonMVAResult>      mvares          = mvaResults->ptrAt( candIndex );
            edm::Ptr<flashgg::DiPhotonCandidate>      dipho           = diPhotons->ptrAt( candIndex );
            
            StageOneCombinedTag stage1tag_obj( dipho, mvares, vbf_mvares );
            stage1tag_obj.setDiPhotonIndex( candIndex );
            stage1tag_obj.setSystLabel( systLabel_ );
            stage1tag_obj.includeWeights( *dipho );

            if ( stage1tag_obj.VBFMVA().dijet_Mjj > 0. ) {

                // We don't want to include all the jet weights because btag weights are not relevant
                // So we do some tricky surgery instead, to get only the UnmatchedPUWeight
                float tagcorig = stage1tag_obj.centralWeight();
                float j1corig = stage1tag_obj.VBFMVA().leadJet_ptr->centralWeight();
                float j2corig = stage1tag_obj.VBFMVA().subleadJet_ptr->centralWeight();
                float j1cadjust = stage1tag_obj.VBFMVA().leadJet_ptr->weight("UnmatchedPUWeightCentral"); // stored without scaling to overall central
                float j2cadjust = stage1tag_obj.VBFMVA().subleadJet_ptr->weight("UnmatchedPUWeightCentral"); // stored without scaling to overall central
                float j1upadjust = 1.;
                float j2upadjust = 1.;
                float j1downadjust = 1.;
                float j2downadjust = 1.;
                if (stage1tag_obj.VBFMVA().leadJet_ptr->hasWeight("UnmatchedPUWeightUp01sigma") ) {
                    j1upadjust = stage1tag_obj.VBFMVA().leadJet_ptr->weight("UnmatchedPUWeightUp01sigma")  / j1corig;
                    j1downadjust = stage1tag_obj.VBFMVA().leadJet_ptr->weight("UnmatchedPUWeightDown01sigma") / j1corig;
                }
                if (stage1tag_obj.VBFMVA().leadJet_ptr->hasWeight("UnmatchedPUWeightUp01sigma") ) {
                    j2upadjust = stage1tag_obj.VBFMVA().subleadJet_ptr->weight("UnmatchedPUWeightUp01sigma") / j2corig;
                    j2downadjust = stage1tag_obj.VBFMVA().subleadJet_ptr->weight("UnmatchedPUWeightDown01sigma") / j2corig;
                }
                if (false && systLabel_ == "") {
                    std::cout << "tagcorig j1cadjust j2cadjust j1upadjust j2upadjust j1downadjust j2downadjust ";
                    std::cout << tagcorig << " " << j1cadjust << " "<< j2cadjust << " "<< j1upadjust << " "<< j2upadjust << " "<< j1downadjust << " "<< j2downadjust << std::endl;
                }
                for (auto it = stage1tag_obj.weightListBegin() ; it != stage1tag_obj.weightListEnd(); it++) {
                    stage1tag_obj.setWeight(*it,stage1tag_obj.weight(*it) * j1cadjust * j2cadjust); 
                }
                if (stage1tag_obj.VBFMVA().leadJet_ptr->hasWeight("UnmatchedPUWeightUp01sigma") ) {
                    stage1tag_obj.setWeight("UnmatchedPUWeightUp01sigma", stage1tag_obj.centralWeight() * j1upadjust * j2upadjust );
                    stage1tag_obj.setWeight("UnmatchedPUWeightDown01sigma", stage1tag_obj.centralWeight() * j1downadjust * j2downadjust );
                }
                if (false && systLabel_ == "") {
                    for (auto it = stage1tag_obj.weightListBegin() ; it != stage1tag_obj.weightListEnd(); it++) {
                        std::cout << "SCZ Weight Debug " << *it << " " << stage1tag_obj.weight(*it) << std::endl;
                        
                    }
                }
            }
            
            int chosenTag = computeStage1Kinematics( stage1tag_obj ); // choose category
            stage1tag_obj.setStage1recoTag( chosenTag ); // set the category in the object

            // saving the collection
            stage1tags->push_back( stage1tag_obj );
        }

        evt.put( std::move( stage1tags ) );
    }

    int StageOneCombinedTagProducer::computeStage1Kinematics( const StageOneCombinedTag tag_obj ) 
    {
        int chosenTag_ = stage1recoTag::LOGICERROR;
        float ptH = tag_obj.diPhoton()->pt();
        unsigned int nJ = 0;
        float mjj = 0.;
        float ptHjj = 0.;
        float cosThetaStar = -9999.;
        float mvaScore = tag_obj.diPhotonMVA().transformedMvaValue(); // maps output score from TMVA back to XGBoost original
        //float dijetScore = tag_obj.VBFMVA().VBFMVAValue();
        float dijetScore = tag_obj.VBFMVA().prob_VBF_value();
        float gghScore = tag_obj.VBFMVA().prob_ggH_value();
        float leadMvaScore = tag_obj.diPhotonMVA().leadmva;
        float subleadMvaScore = tag_obj.diPhotonMVA().subleadmva;
        float leadPToM = tag_obj.diPhotonMVA().leadptom;
        float subleadPToM = tag_obj.diPhotonMVA().subleadptom;
        edm::Ptr<flashgg::Jet> j0 = tag_obj.VBFMVA().leadJet_ptr; 
        edm::Ptr<flashgg::Jet> j1 = tag_obj.VBFMVA().subleadJet_ptr;
        
        if ( !j0.isNull() ) {
            if ( j0->pt() > 30. ) { nJ += 1; }
        }
        if ( !j1.isNull() ) {
            if ( j1->pt() > 30. ) { nJ += 1; }
        }

        if ( nJ >= 2 ) {
            mjj = ( j0->p4() + j1->p4() ).mass();
            ptHjj = ( j0->p4() + j1->p4() + tag_obj.diPhoton()->p4() ).pt();

            TLorentzVector jetl, jets, dijet, phol, phos, diphoton, vstar;
            phol.SetPtEtaPhiE( tag_obj.leadingPhoton()->pt(), tag_obj.leadingPhoton()->eta(), tag_obj.leadingPhoton()->phi(), tag_obj.leadingPhoton()->energy() );
            phos.SetPtEtaPhiE( tag_obj.subLeadingPhoton()->pt(), tag_obj.subLeadingPhoton()->eta(), tag_obj.subLeadingPhoton()->phi(), tag_obj.subLeadingPhoton()->energy() );
            jetl.SetPtEtaPhiE( j0->pt(), j0->eta(), j0->phi(), j0->energy() );
            jets.SetPtEtaPhiE( j1->pt(), j1->eta(), j1->phi(), j1->energy() );
            diphoton = phol + phos;
            dijet = jetl + jets;
            vstar = diphoton + dijet;
            diphoton.Boost( -vstar.BoostVector() );
            cosThetaStar = -diphoton.CosTheta();
        }
        // below assign the categories for ggH & VBF based on diphoton MVA score and dijet MVA score - boundaries taken from metaConditions
        if (nJ == 0) {
            if ( ptH > 10 ) {
                if (mvaScore > diphoBounds_["RECO_0J_PTH_GT10_Tag0"]) {
                    chosenTag_ = stage1recoTag::RECO_0J_PTH_GT10_Tag0;
                }
                else if (mvaScore > diphoBounds_["RECO_0J_PTH_GT10_Tag1"]) {
                    chosenTag_ = stage1recoTag::RECO_0J_PTH_GT10_Tag1;
                }
                else { 
                    chosenTag_ = stage1recoTag::NOTAG;
                }
            }
            else {
                if (mvaScore > diphoBounds_["RECO_0J_PTH_0_10_Tag0"]) {
                    chosenTag_ = stage1recoTag::RECO_0J_PTH_0_10_Tag0;
                }
                else if (mvaScore > diphoBounds_["RECO_0J_PTH_0_10_Tag1"]) {
                    chosenTag_ = stage1recoTag::RECO_0J_PTH_0_10_Tag1;
                }
                else { 
                    chosenTag_ = stage1recoTag::NOTAG;
                }
            }
        } else if ( nJ == 1 ) {
            if ( ptH > 200 ) {
                if (mvaScore > diphoBounds_["RECO_PTH_GT200_Tag0"]) {
                    chosenTag_ = stage1recoTag::RECO_PTH_GT200_Tag0;
                }
                else if (mvaScore > diphoBounds_["RECO_PTH_GT200_Tag1"]) {
                    chosenTag_ = stage1recoTag::RECO_PTH_GT200_Tag1;
                }
                else { 
                    chosenTag_ = stage1recoTag::NOTAG;
                }
            } else if ( ptH > 120. ) {
                if (mvaScore > diphoBounds_["RECO_1J_PTH_120_200_Tag0"]) {
                    chosenTag_ = stage1recoTag::RECO_1J_PTH_120_200_Tag0;
                }
                else if (mvaScore > diphoBounds_["RECO_1J_PTH_120_200_Tag1"]) {
                    chosenTag_ = stage1recoTag::RECO_1J_PTH_120_200_Tag1;
                }
                else { 
                    chosenTag_ = stage1recoTag::NOTAG;
                }
            } else if ( ptH > 60. ) {
                if (mvaScore > diphoBounds_["RECO_1J_PTH_60_120_Tag0"]) {
                    chosenTag_ = stage1recoTag::RECO_1J_PTH_60_120_Tag0;
                }
                else if (mvaScore > diphoBounds_["RECO_1J_PTH_60_120_Tag1"]) {
                    chosenTag_ = stage1recoTag::RECO_1J_PTH_60_120_Tag1;
                }
                else { 
                    chosenTag_ = stage1recoTag::NOTAG;
                }
            } else {
                if (mvaScore > diphoBounds_["RECO_1J_PTH_0_60_Tag0"]) {
                    chosenTag_ = stage1recoTag::RECO_1J_PTH_0_60_Tag0;
                }
                else if (mvaScore > diphoBounds_["RECO_1J_PTH_0_60_Tag1"]) {
                    chosenTag_ = stage1recoTag::RECO_1J_PTH_0_60_Tag1;
                }
                else { 
                    chosenTag_ = stage1recoTag::NOTAG;
                }
            }
        } else { // 2 jets
            bool reProcess = false;
            if ( mjj > 350. && j0->p4().pt() > 40. && j1->p4().pt() > 30. && leadMvaScore > -0.2 && subleadMvaScore > -0.2 ) { //cuts optimised using data-driven dijet BDT plus new diphoton BDT
                if ( ptH > 200. ) {
                    if (dijetScore > dijetBounds_["RECO_VBFTOPO_BSM"] && gghScore < gghBounds_["RECO_VBFTOPO_BSM"]  && mvaScore > diphoBounds_["RECO_VBFTOPO_BSM"]) {
                        chosenTag_ = stage1recoTag::RECO_VBFTOPO_BSM;
                    }
                    else { 
                        reProcess = true;
                    }
                }
                else if ( ptHjj > 0. && ptHjj < 25.) {
                    if ( mjj > 700. ) {
                        if (dijetScore > dijetBounds_["RECO_VBFTOPO_JET3VETO_HIGHMJJ"] && gghScore < gghBounds_["RECO_VBFTOPO_JET3VETO_HIGHMJJ"]  && mvaScore > diphoBounds_["RECO_VBFTOPO_JET3VETO_HIGHMJJ"]) {
                            chosenTag_ = stage1recoTag::RECO_VBFTOPO_JET3VETO_HIGHMJJ;
                        }
                        else { 
                            reProcess = true;
                        }
                    }
                    else if ( mjj > 350. ) {
                        if (dijetScore > dijetBounds_["RECO_VBFTOPO_JET3VETO_LOWMJJ"] && gghScore < gghBounds_["RECO_VBFTOPO_JET3VETO_LOWMJJ"]  && mvaScore > diphoBounds_["RECO_VBFTOPO_JET3VETO_LOWMJJ"]) {
                            chosenTag_ = stage1recoTag::RECO_VBFTOPO_JET3VETO_LOWMJJ;
                        }
                        else { 
                            reProcess = true;
                        }
                    }
                    else { 
                        reProcess = true;
                    }
                } else if ( ptHjj > 25. ) {
                    if ( mjj > 700. ) {
                        if (dijetScore > dijetBounds_["RECO_VBFTOPO_JET3_HIGHMJJ"] && gghScore < gghBounds_["RECO_VBFTOPO_JET3_HIGHMJJ"]  && mvaScore > diphoBounds_["RECO_VBFTOPO_JET3_HIGHMJJ"]) {
                            chosenTag_ = stage1recoTag::RECO_VBFTOPO_JET3_HIGHMJJ;
                        }
                        else { 
                            reProcess = true;
                        }
                    }
                    else if ( mjj > 350. ) {
                        if (dijetScore > dijetBounds_["RECO_VBFTOPO_JET3_LOWMJJ"] && gghScore < gghBounds_["RECO_VBFTOPO_JET3_LOWMJJ"]  && mvaScore > diphoBounds_["RECO_VBFTOPO_JET3_LOWMJJ"]) {
                            chosenTag_ = stage1recoTag::RECO_VBFTOPO_JET3_LOWMJJ;
                        }
                        else { 
                            reProcess = true;
                        }
                    }
                    else { 
                        reProcess = true;
                    }
                }
            }
            else if ( mjj > 60. && mjj < 120. && j0->p4().pt() > 30. && j1->p4().pt() > 30. && abs(j0->p4().eta()) < 2.4 && abs(j1->p4().eta()) < 2.4 && abs(cosThetaStar)<0.5 && leadMvaScore > -0.2 && subleadMvaScore > -0.2 ) { //cuts for VH hadronic
                if (mvaScore > diphoBounds_["RECO_VBFTOPO_VHHAD"]) {
                    chosenTag_ = stage1recoTag::RECO_VBFTOPO_VHHAD;
                }
                else { 
                    reProcess = true;
                }
            }
            else {
               reProcess = true;
            }
            if ( reProcess ) {
                if ( mjj > 350. && j0->p4().pt() > 40. && j1->p4().pt() > 30. && leadMvaScore > -0.2 && subleadMvaScore > -0.2 ) { //cuts optimised using data-driven dijet BDT plus new diphoton BDT
                    if ( dijetScore < dijetBounds_["RECO_VBFLIKEGGH"] && gghScore > gghBounds_["RECO_VBFLIKEGGH"]  && mvaScore > diphoBounds_["RECO_VBFLIKEGGH"]) {
                            chosenTag_ = stage1recoTag::RECO_VBFLIKEGGH;
                        }
                    else { 
                        chosenTag_ = stage1recoTag::NOTAG;
                    }
                }
                else if ( ptH > 200 ) {
                    if (mvaScore > diphoBounds_["RECO_PTH_GT200_Tag0"]) {
                        chosenTag_ = stage1recoTag::RECO_PTH_GT200_Tag0;
                    }
                    else if (mvaScore > diphoBounds_["RECO_PTH_GT200_Tag1"]) {
                        chosenTag_ = stage1recoTag::RECO_PTH_GT200_Tag1;
                    }
                    else { 
                        chosenTag_ = stage1recoTag::NOTAG;
                    }
                } else if ( ptH > 120. ) {
                    if (mvaScore > diphoBounds_["RECO_GE2J_PTH_120_200_Tag0"]) {
                        chosenTag_ = stage1recoTag::RECO_GE2J_PTH_120_200_Tag0;
                    }
                    else if (mvaScore > diphoBounds_["RECO_GE2J_PTH_120_200_Tag1"]) {
                        chosenTag_ = stage1recoTag::RECO_GE2J_PTH_120_200_Tag1;
                    }
                    else { 
                        chosenTag_ = stage1recoTag::NOTAG;
                    }
                } else if ( ptH > 60. ) {
                    if (mvaScore > diphoBounds_["RECO_GE2J_PTH_60_120_Tag0"]) {
                        chosenTag_ = stage1recoTag::RECO_GE2J_PTH_60_120_Tag0;
                    }
                    else if (mvaScore > diphoBounds_["RECO_GE2J_PTH_60_120_Tag1"]) {
                        chosenTag_ = stage1recoTag::RECO_GE2J_PTH_60_120_Tag1;
                    }
                    else { 
                        chosenTag_ = stage1recoTag::NOTAG;
                    }
                } else {
                    if (mvaScore > diphoBounds_["RECO_GE2J_PTH_0_60_Tag0"]) {
                        chosenTag_ = stage1recoTag::RECO_GE2J_PTH_0_60_Tag0;
                    }
                    else if (mvaScore > diphoBounds_["RECO_GE2J_PTH_0_60_Tag1"]) {
                        chosenTag_ = stage1recoTag::RECO_GE2J_PTH_0_60_Tag1;
                    }
                    else { 
                        chosenTag_ = stage1recoTag::NOTAG;
                    }
                }
            }
        }
        // reject events not passing the scaled pT cuts
        if ( leadPToM < 1./3. || subleadPToM < 1./4. ) {
            chosenTag_ = stage1recoTag::NOTAG;
        }
        
        //finally pass back the chosen tag
        return chosenTag_;
    }

    void StageOneCombinedTagProducer::constructBounds() 
    {
        diphoBounds_["RECO_0J_PTH_0_10_Tag0"] = rawDiphoBounds_[0];
        diphoBounds_["RECO_0J_PTH_0_10_Tag1"] = rawDiphoBounds_[1];
        diphoBounds_["RECO_0J_PTH_GT10_Tag0"] = rawDiphoBounds_[2];
        diphoBounds_["RECO_0J_PTH_GT10_Tag1"] = rawDiphoBounds_[3];
        diphoBounds_["RECO_1J_PTH_0_60_Tag0"] = rawDiphoBounds_[4];
        diphoBounds_["RECO_1J_PTH_0_60_Tag1"] = rawDiphoBounds_[5];
        diphoBounds_["RECO_1J_PTH_60_120_Tag0"] = rawDiphoBounds_[6];
        diphoBounds_["RECO_1J_PTH_60_120_Tag1"] = rawDiphoBounds_[7];
        diphoBounds_["RECO_1J_PTH_120_200_Tag0"] = rawDiphoBounds_[8];
        diphoBounds_["RECO_1J_PTH_120_200_Tag1"] = rawDiphoBounds_[9];
        diphoBounds_["RECO_GE2J_PTH_0_60_Tag0"] = rawDiphoBounds_[10];
        diphoBounds_["RECO_GE2J_PTH_0_60_Tag1"] = rawDiphoBounds_[11];
        diphoBounds_["RECO_GE2J_PTH_60_120_Tag0"] = rawDiphoBounds_[12];
        diphoBounds_["RECO_GE2J_PTH_60_120_Tag1"] = rawDiphoBounds_[13];
        diphoBounds_["RECO_GE2J_PTH_120_200_Tag0"] = rawDiphoBounds_[14];
        diphoBounds_["RECO_GE2J_PTH_120_200_Tag1"] = rawDiphoBounds_[15];
        diphoBounds_["RECO_PTH_GT200_Tag0"] = rawDiphoBounds_[16];
        diphoBounds_["RECO_PTH_GT200_Tag1"] = rawDiphoBounds_[17];
        diphoBounds_["RECO_VBFTOPO_VHHAD"] = rawDiphoBounds_[18];
        diphoBounds_["RECO_VBFTOPO_JET3VETO_LOWMJJ"] = rawDiphoBounds_[19];
        diphoBounds_["RECO_VBFTOPO_JET3VETO_HIGHMJJ"] = rawDiphoBounds_[20];
        diphoBounds_["RECO_VBFTOPO_JET3_LOWMJJ"] = rawDiphoBounds_[21];
        diphoBounds_["RECO_VBFTOPO_JET3_HIGHMJJ"] = rawDiphoBounds_[22];
        diphoBounds_["RECO_VBFTOPO_BSM"] = rawDiphoBounds_[23];
        diphoBounds_["RECO_VBFLIKEGGH"] = rawDiphoBounds_[24];

        dijetBounds_["RECO_VBFTOPO_JET3VETO_LOWMJJ"] = rawDijetBounds_[0];
        dijetBounds_["RECO_VBFTOPO_JET3VETO_HIGHMJJ"] = rawDijetBounds_[1];
        dijetBounds_["RECO_VBFTOPO_JET3_LOWMJJ"] = rawDijetBounds_[2];
        dijetBounds_["RECO_VBFTOPO_JET3_HIGHMJJ"] = rawDijetBounds_[3];
        dijetBounds_["RECO_VBFTOPO_BSM"] = rawDijetBounds_[4];
        dijetBounds_["RECO_VBFLIKEGGH"] = rawDijetBounds_[5];

        gghBounds_["RECO_VBFTOPO_JET3VETO_LOWMJJ"] = rawGghBounds_[0];
        gghBounds_["RECO_VBFTOPO_JET3VETO_HIGHMJJ"] = rawGghBounds_[1];
        gghBounds_["RECO_VBFTOPO_JET3_LOWMJJ"] = rawGghBounds_[2];
        gghBounds_["RECO_VBFTOPO_JET3_HIGHMJJ"] = rawGghBounds_[3];
        gghBounds_["RECO_VBFTOPO_BSM"] = rawGghBounds_[4];
        gghBounds_["RECO_VBFLIKEGGH"] = rawGghBounds_[5];
    }

}

typedef flashgg::StageOneCombinedTagProducer FlashggStageOneCombinedTagProducer;
DEFINE_FWK_MODULE( FlashggStageOneCombinedTagProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

