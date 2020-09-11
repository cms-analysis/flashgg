#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "flashgg/DataFormats/interface/VBFMVAResult.h"
#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "flashgg/DataFormats/interface/UntaggedTag.h"
#include "flashgg/DataFormats/interface/TagTruthBase.h"
#include "DataFormats/Common/interface/RefToPtr.h"
#include "flashgg/DataFormats/interface/VBFTag.h"
#include "flashgg/DataFormats/interface/NoTag.h"

#include "SimDataFormats/HTXS/interface/HiggsTemplateCrossSections.h"

#include "TMVA/Reader.h"
#include "TMath.h"
#include "TGraph.h"
//#include <typeinfo>

#include <algorithm>


using namespace std;
using namespace edm;

namespace flashgg {

    struct TagPriorityRange {
        string name;
        int minCat;
        int maxCat;
        unsigned int collIndex;

        TagPriorityRange( string s, int c1, int c2, unsigned int i )
        {
            name = s;
            minCat = c1;
            maxCat = c2;
            collIndex = i;
        }
    };

    class TagSorter : public EDProducer
    {

    public:
        TagSorter( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;

        EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
        std::vector<edm::EDGetTokenT<View<flashgg::DiPhotonTagBase> > > TagList_;
        std::vector<TagPriorityRange> TagPriorityRanges;

        double massCutUpper;
        double massCutLower;

        double minObjectWeightException;
        double maxObjectWeightException;
        double minObjectWeightWarning;
        double maxObjectWeightWarning;

        bool isGluonFusion_;
        FileInPath NNLOPSWeightFile_;
        std::vector<std::unique_ptr<TGraph> > NNLOPSWeights_;
        bool applyNNLOPSweight_;

        bool debug_;
        bool storeOtherTagInfo_;
        bool blindedSelectionPrintout_;

        bool createNoTag_;
        bool setHTXSinfo_;
        bool addTruthInfo_;
        EDGetTokenT<HTXS::HiggsClassification> newHTXSToken_;
        EDGetTokenT<View<reco::GenParticle> >      genPartToken_;

        std::vector<std::tuple<DiPhotonTagBase::tag_t,int,int> > otherTags_; // (type,category,diphoton index)

        string tagName(DiPhotonTagBase::tag_t) const;
    };

    TagSorter::TagSorter( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        genPartToken_( consumes<View<reco::GenParticle> >( iConfig.getParameter<InputTag> ( "GenParticleTag" ) ) )
    {

        massCutUpper = iConfig.getParameter<double>( "MassCutUpper" );
        massCutLower = iConfig.getParameter<double>( "MassCutLower" );
        minObjectWeightException = iConfig.getParameter<double>( "MinObjectWeightException" );
        maxObjectWeightException = iConfig.getParameter<double>( "MaxObjectWeightException" );
        minObjectWeightWarning = iConfig.getParameter<double>( "MinObjectWeightWarning" );
        maxObjectWeightWarning = iConfig.getParameter<double>( "MaxObjectWeightWarning" );

        isGluonFusion_ = iConfig.getParameter<bool>("isGluonFusion");
        NNLOPSWeightFile_ = iConfig.getParameter<FileInPath>( "NNLOPSWeightFile" );
        TFile* f = TFile::Open(NNLOPSWeightFile_.fullPath().c_str());
        NNLOPSWeights_.emplace_back((TGraph*)((TGraph*) f->Get("gr_NNLOPSratio_pt_mcatnlo_0jet"))->Clone() );
        NNLOPSWeights_.emplace_back((TGraph*)((TGraph*) f->Get("gr_NNLOPSratio_pt_mcatnlo_1jet"))->Clone() );
        NNLOPSWeights_.emplace_back((TGraph*)((TGraph*) f->Get("gr_NNLOPSratio_pt_mcatnlo_2jet"))->Clone() );
        NNLOPSWeights_.emplace_back((TGraph*)((TGraph*) f->Get("gr_NNLOPSratio_pt_mcatnlo_3jet"))->Clone() );
        f->Close();
        delete f;
        applyNNLOPSweight_ = iConfig.getParameter<bool>("applyNNLOPSweight");
        if( isGluonFusion_ && applyNNLOPSweight_ ) {
            std::cout << "[WARNING]: configuration is set to reweigh from madgraph to NNLOPS, please check this is a ggH sample" << std::endl;
        }

        debug_ = iConfig.getUntrackedParameter<bool>( "Debug", false );
        storeOtherTagInfo_ = iConfig.getParameter<bool>( "StoreOtherTagInfo" );
        blindedSelectionPrintout_ = iConfig.getParameter<bool>("BlindedSelectionPrintout");
        createNoTag_ = iConfig.getParameter<bool>("CreateNoTag");
        setHTXSinfo_ = iConfig.getParameter<bool>("SetHTXSinfo");
        addTruthInfo_ = iConfig.getParameter<bool>("AddTruthInfo");

        const auto &vpset = iConfig.getParameterSetVector( "TagPriorityRanges" );

        vector<string> labels;

        for( const auto &pset : vpset ) {
            InputTag tag = pset.getParameter<InputTag>( "TagName" );
            int c1 = pset.getUntrackedParameter<int>( "MinCategory", -999 );
            int c2 = pset.getUntrackedParameter<int>( "MaxCategory", 999 );
            unsigned int i = 0;
            for( ; i < labels.size() ; i++ ) {
                if( labels[i] == tag.label() ) { break; }
            }
            if( i == TagList_.size() ) {
                // cout << "tag = " << tag << endl;
                labels.push_back( tag.label() );
                TagList_.push_back( consumes<View<flashgg::DiPhotonTagBase> >( tag ) );
            }
            TagPriorityRanges.emplace_back( tag.label(), c1, c2, i );
        }

        ParameterSet HTXSps = iConfig.getParameterSet( "HTXSTags" );
        newHTXSToken_ = consumes<HTXS::HiggsClassification>( HTXSps.getParameter<InputTag>("ClassificationObj") );

        produces<edm::OwnVector<flashgg::DiPhotonTagBase> >();
        produces<edm::OwnVector<flashgg::TagTruthBase> >();
    }

    void TagSorter::produce( Event &evt, const EventSetup & )
    {
        unique_ptr<edm::OwnVector<flashgg::DiPhotonTagBase> > SelectedTag( new edm::OwnVector<flashgg::DiPhotonTagBase> );
        unique_ptr<edm::OwnVector<flashgg::TagTruthBase> > SelectedTagTruth( new edm::OwnVector<flashgg::TagTruthBase> );

        Handle<HTXS::HiggsClassification> htxsClassification;
        evt.getByToken(newHTXSToken_,htxsClassification);

        // Cache other tags for each event; but do not use the old ones next time
        otherTags_.clear(); 

        int priority = -1; // for debug

        bool alreadyChosen = false;

        for( auto tpr = TagPriorityRanges.begin() ; tpr != TagPriorityRanges.end() ; tpr++ ) {
            priority += 1; // for debug
            // cout << "On priority " << priority << endl; // HHWWgg debugging 
            // cout << "TagList_ = " << TagList_ << endl;
            // cout << "tpr->collIndex = " << tpr->collIndex << endl;
            // cout << "TagList_[tpr->collIndex] = " << TagList_[tpr->collIndex] << endl;

            Handle<View<flashgg::DiPhotonTagBase> > TagVectorEntry;
            evt.getByToken( TagList_[tpr->collIndex], TagVectorEntry );

            edm::RefProd<edm::OwnVector<TagTruthBase> > rTagTruth = evt.getRefBeforePut<edm::OwnVector<TagTruthBase> >();

            int chosen_i = -1 ; //this will become the index of the highest priority candidate 
            // cout << "TagVectorEntry->size() = " << TagVectorEntry->size() << endl;
            // Looking from highest priority to lowest, check if the tag has any entries.
            for( unsigned int  tag_i = 0; tag_i < TagVectorEntry->size() ; tag_i++ )        {

                // cout << "tag_i = " << tag_i << endl;

                float mass = TagVectorEntry->ptrAt( tag_i )->diPhoton()->mass();
                float sumPt = TagVectorEntry->ptrAt( tag_i )->diPhoton()->sumPt();
                int category = TagVectorEntry->ptrAt( tag_i )->categoryNumber();

                if (debug_) {
                    std::cout << "[TagSorter DEBUG]" << tpr->name << " " << tpr->minCat << " " << tpr->maxCat << " "
                              << mass << " " << category << " " << tag_i << std::endl;
                }

                // ignore candidate tags with category number outside the present range we're looking at
                if( category < tpr->minCat || category > tpr->maxCat ) { continue ; }

                // ignore candidate tags with diphoton outside of the allowed mass range.
                if( ( mass < massCutLower ) || ( mass > massCutUpper ) ) {continue ;}

                if ( alreadyChosen ) {
                    // This tag would have worked but the selection has already been done for a higher tag
                    // The fact that we are still running means we are storing additional information
                    assert( storeOtherTagInfo_ );
                    SelectedTag->back().addOtherTag( *(TagVectorEntry->ptrAt( tag_i )) );
                } else {
                    // We can still pick a tag in the current priority range
                    // We pick this one if:
                    // i. this is the first, or 
                    // ii. if this is a higher category than existing one, or 
                    // iii. if it is an equal category and has higher sumPt
                    // In the case of (ii) or (iii) we also save the old tag info if storeOtherTagInfo_ is true
                    if ( (chosen_i == -1) ||
                         (TagVectorEntry->ptrAt( chosen_i )->categoryNumber() > category) ||
                         (TagVectorEntry->ptrAt( chosen_i )->categoryNumber() == category && TagVectorEntry->ptrAt( chosen_i )->diPhoton()->sumPt() < sumPt) ) {
                        if ( chosen_i >= 0 && storeOtherTagInfo_ ) {
                            otherTags_.emplace_back( TagVectorEntry->ptrAt( chosen_i )->tagEnum(), 
                                                     TagVectorEntry->ptrAt( chosen_i )->categoryNumber(),
                                                     TagVectorEntry->ptrAt( chosen_i )->diPhotonIndex() 
                                                     );
                        }
                        if ( debug_  ) {
                            std::cout << "[TagSorter DEBUG] Updating chosen_i " << chosen_i << " --> " << tag_i  << std::endl;
                            if ( chosen_i >= 0 ) {
                                std::cout << "    [TagSorter DEBUG] So: updating mass " << TagVectorEntry->ptrAt( chosen_i )->diPhoton()->mass() << " --> " << mass
                                          << " sumPt " << TagVectorEntry->ptrAt( chosen_i )->diPhoton()->sumPt() << " --> "
                                          << TagVectorEntry->ptrAt( tag_i )->diPhoton()->sumPt()
                                          << " cat " << TagVectorEntry->ptrAt( chosen_i )->categoryNumber() << " --> " << category
                                          << std::endl;
                            }
                        }
                        chosen_i = tag_i;
                    }
                }
            }

            if( chosen_i != -1 ) {

                float centralObjectWeight = TagVectorEntry->ptrAt( chosen_i )->centralWeight();
                if (centralObjectWeight < minObjectWeightException || centralObjectWeight > maxObjectWeightException) {
                    throw cms::Exception( "TagObjectWeight" ) << " Tag centralWeight=" << centralObjectWeight << " outside of bound ["
                                                              << minObjectWeightException << "," << maxObjectWeightException
                                                              << "] - " << tpr->name << " chosen_i=" << chosen_i << " - change bounds or debug tag";
                }
                if (centralObjectWeight < minObjectWeightWarning || centralObjectWeight > maxObjectWeightWarning) {
                    std::cout << "WARNING Tag centralWeight=" << centralObjectWeight << " outside of bound ["
                              << minObjectWeightWarning << "," << maxObjectWeightWarning
                              << "] - " << tpr->name << " chosen_i=" << chosen_i << " - consider investigating!" << std::endl;
                }

                SelectedTag->push_back( *TagVectorEntry->ptrAt( chosen_i ) );

                if( ! evt.isRealData() ) {
                    if( addTruthInfo_ ) {
                        TagTruthBase truth;
                        if( TagVectorEntry->ptrAt( chosen_i )->tagTruth().isNonnull() ) { truth = *(TagVectorEntry->ptrAt( chosen_i )->tagTruth()->clone()); }
                        Handle<View<reco::GenParticle> > genParticles;
                        evt.getByToken( genPartToken_, genParticles );
                        TagTruthBase::Point higgsVtx;
                        for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) {
                            int pdgid = genParticles->ptrAt( genLoop )->pdgId();
                            if( pdgid == 25 || pdgid == 22 ) {
                                higgsVtx = genParticles->ptrAt( genLoop )->vertex();
                                break;
                            }
                        }
                        truth.setGenPV( higgsVtx );
                        if( htxsClassification.isValid() && setHTXSinfo_ ) { 
                            truth.setHTXSInfo( int(htxsClassification->stage0_cat),
                                               int(htxsClassification->stage1_cat_pTjet30GeV),
                                               int(htxsClassification->stage1_1_cat_pTjet30GeV),
                                               int(htxsClassification->stage1_1_fine_cat_pTjet30GeV),
                                               int(htxsClassification->stage1_2_cat_pTjet30GeV),
                                               int(htxsClassification->stage1_2_fine_cat_pTjet30GeV),
                                               float(htxsClassification->jets30.size()),
                                               float(htxsClassification->p4decay_higgs.pt()),
                                               float(htxsClassification->p4decay_V.pt()) );
                        }
                        if( isGluonFusion_ ) {
                            int stxsNjets = htxsClassification->jets30.size();
                            float stxsPtH = htxsClassification->p4decay_higgs.pt();
                            float NNLOPSweight = 1;
                            if ( stxsNjets == 0) NNLOPSweight = NNLOPSWeights_[0]->Eval(min(stxsPtH,float(125.0)));
                            else if ( stxsNjets == 1) NNLOPSweight = NNLOPSWeights_[1]->Eval(min(stxsPtH,float(625.0)));
                            else if ( stxsNjets == 2) NNLOPSweight = NNLOPSWeights_[2]->Eval(min(stxsPtH,float(800.0)));
                            else if ( stxsNjets >= 3) NNLOPSweight = NNLOPSWeights_[3]->Eval(min(stxsPtH,float(925.0)));
                            truth.setWeight("NNLOPSweight", NNLOPSweight);
                            if( debug_ ) {
                                std::cout << "[TagSorter DEBUG] computed and stored an NNLOPS weight of " << truth.weight("NNLOPSweight") << std::endl;
                            }
                            if( applyNNLOPSweight_ ) {
                                if( debug_ ) {
                                    std::cout << "[TagSorter DEBUG] reweighing to NNLOPS, central weight being altered by a factor of " << NNLOPSweight << std::endl;
                                    std::cout << "[TagSorter DEBUG]  central weight before: " << SelectedTag->back().centralWeight() << std::endl;
                                }
                                WeightedObject NNLOPSobject;
                                NNLOPSobject.setCentralWeight( NNLOPSweight );
                                SelectedTag->back().includeWeights( NNLOPSobject );
                                if( debug_ ) {
                                    std::cout << "[TagSorter DEBUG]  central weight after: " << SelectedTag->back().centralWeight() << std::endl;
                                }
                            }
                        }
                        SelectedTagTruth->push_back( truth );
                        SelectedTag->back().setTagTruth( edm::refToPtr( edm::Ref<edm::OwnVector<TagTruthBase> >( rTagTruth, 0 ) ) ); // Normally this 0 would be the index number
                    }
                    else {
                        edm::Ptr<TagTruthBase> truth = TagVectorEntry->ptrAt( chosen_i )->tagTruth();
                        if( truth.isNonnull() ) {
                            SelectedTagTruth->push_back( *truth );
                            SelectedTag->back().setTagTruth( edm::refToPtr( edm::Ref<edm::OwnVector<TagTruthBase> >( rTagTruth, 0 ) ) ); // Normally this 0 would be the index number
                        }
                    }
                }

                if ( debug_ ) {
                    std::cout << "[TagSorter DEBUG] Priority " << priority << " Tag Found! Tag entry "<< chosen_i  << " with sumPt "
                              << TagVectorEntry->ptrAt(chosen_i)->sumPt() << ", systLabel " << TagVectorEntry->ptrAt(chosen_i)->systLabel() << std::endl;
                }
                if ( storeOtherTagInfo_ ) {
                    if ( debug_ ) {
                        std::cout << "[TagSorter DEBUG] Saving other interpretations, so we save the ones so far (if any) and then continue looping for new ones" << std::endl;
                    }
                    alreadyChosen = true;
                    SelectedTag->back().addOtherTags( otherTags_ );
                    
                } else {
                    if ( debug_ ) {
                        std::cout << "[TagSorter DEBUG] Not saving other interpretations so we break out of the tag priority loop" << std::endl;
                    }
                    break; 
                }
            } else {
                if ( debug_ ) {
                    std::cout << "[TagSorter DEBUG] No Priority " << priority << " Tag ..., looking for Priority " << (priority+1) << " Tag.. " << std::endl;
                }
            }
        } 

        if ( SelectedTag->size() == 1  && storeOtherTagInfo_ && debug_ ) {
            if ( SelectedTag->back().nOtherTags() > 0 ) {
                std::cout << "[TagSorter DEBUG] List of other tags: (" << SelectedTag->back().nOtherTags() << " total):" << std::endl;
                for ( unsigned i = 0 ; i < SelectedTag->back().nOtherTags() ; i++) {
                    std::cout << "[TagSorter DEBUG]  (tag_t,cat,dipho_i)=(" << SelectedTag->back().otherTagType(i) << "," 
                              << SelectedTag->back().otherTagCategory(i) << ","
                              << SelectedTag->back().otherTagDiPhotonIndex(i) << ")" << std::endl;
                }
            } else {
                std::cout << "[TagSorter DEBUG] No other tag interpretations for this event" << std::endl;
            }
        }

        if ( SelectedTag->size() == 1  && storeOtherTagInfo_ && blindedSelectionPrintout_ ) {
            float mass = SelectedTag->back().diPhoton()->mass();
            if (mass < 115. || mass > 135.) {
                int cat = SelectedTag->back().categoryNumber();
                std::cout << "******************************" << std::endl;
                std::cout << "* BLINDED SELECTION PRINTOUT *" << std::endl;
                std::cout << "******************************" << std::endl;
                std::cout << "* Run " << evt.run() << " LumiSection " << evt.id().luminosityBlock() << " Event " << evt.id().event() << std::endl;
                std::cout << "* Selected tag name: " << TagSorter::tagName(SelectedTag->back().tagEnum()) << std::endl;
                if (cat >= 0) {
                    std::cout << "* Selected tag category: " << cat << std::endl;
                }
                std::cout << "* Selected tag MVA result: " << SelectedTag->back().diPhotonMVA().mvaValue() << std::endl;
                std::cout << "* Selected tag mass: " << mass << std::endl;
                if ( storeOtherTagInfo_ ) {
                    unsigned nother = SelectedTag->back().nOtherTags();
                    int dipho_i = SelectedTag->back().diPhotonIndex();
                    std::cout << "* Number of other tag interpretations: " << nother << std::endl;
                    for (unsigned i = 0 ; i < nother; i++) {
                        std::cout << "*     " << TagSorter::tagName(SelectedTag->back().otherTagType(i)) << " ";
                        int ocat = SelectedTag->back().otherTagCategory(i);
                        if (ocat >= 0) {
                            std::cout << ocat << " ";
                        }
                        if ( SelectedTag->back().otherTagDiPhotonIndex(i) == dipho_i ) {
                            std::cout << "(same diphoton)";
                        } else {
                            std::cout << "(different diphoton)";
                        }
                        std::cout << std::endl;
                    }
                } else {
                    std::cout << "* Other tag interpretations not stored (config)" << std::endl;
                }
                if( SelectedTagTruth->size() != 0 ) {
                    std::cout << "* HTXS Category 0 (1): " << SelectedTagTruth->back().HTXSstage0bin() << " ("
                              << SelectedTagTruth->back().HTXSstage1bin() << ")" << std::endl;
                    std::cout << "* HTXS njets, pTH, pTV: " << SelectedTagTruth->back().HTXSnjets() << ", "
                              << SelectedTagTruth->back().HTXSpTH() << ", "
                              << SelectedTagTruth->back().HTXSpTV() << std::endl;
                }
                std::cout << "******************************" << std::endl;
            }
        }

        assert( SelectedTag->size() == 1 || SelectedTag->size() == 0 );
        if (createNoTag_ && SelectedTag->size() == 0) {
            SelectedTag->push_back(NoTag());
            SelectedTag->back().setStage1recoTag(flashgg::DiPhotonTagBase::NOTAG);
            edm::RefProd<edm::OwnVector<TagTruthBase> > rTagTruth = evt.getRefBeforePut<edm::OwnVector<TagTruthBase> >();
            TagTruthBase truth_obj;
            if ( htxsClassification.isValid() && setHTXSinfo_ ) {
                truth_obj.setHTXSInfo( int(htxsClassification->stage0_cat),
                                       int(htxsClassification->stage1_cat_pTjet30GeV),
                                       int(htxsClassification->stage1_1_cat_pTjet30GeV),
                                       int(htxsClassification->stage1_1_fine_cat_pTjet30GeV),
                                       int(htxsClassification->stage1_2_cat_pTjet30GeV),
                                       int(htxsClassification->stage1_2_fine_cat_pTjet30GeV),
                                       float(htxsClassification->jets30.size()),
                                       float(htxsClassification->p4decay_higgs.pt()),
                                       float(htxsClassification->p4decay_V.pt()) );
            }
            if( isGluonFusion_ ) {
                int stxsNjets = htxsClassification->jets30.size();
                float stxsPtH = htxsClassification->p4decay_higgs.pt();
                float NNLOPSweight = 1;
                if ( stxsNjets == 0) NNLOPSweight = NNLOPSWeights_[0]->Eval(min(stxsPtH,float(125.0)));
                else if ( stxsNjets == 1) NNLOPSweight = NNLOPSWeights_[1]->Eval(min(stxsPtH,float(625.0)));
                else if ( stxsNjets == 2) NNLOPSweight = NNLOPSWeights_[2]->Eval(min(stxsPtH,float(800.0)));
                else if ( stxsNjets >= 3) NNLOPSweight = NNLOPSWeights_[3]->Eval(min(stxsPtH,float(925.0)));
                truth_obj.setWeight("NNLOPSweight", NNLOPSweight);
                if( debug_ ) {
                    std::cout << "[TagSorter DEBUG] computed and stored an NNLOPS weight of " << truth_obj.weight("NNLOPSweight") << std::endl;
                }
                if( applyNNLOPSweight_ ) {
                    if( debug_ ) {
                        std::cout << "[TagSorter DEBUG] reweighing to NNLOPS, central weight being altered by a factor of " << NNLOPSweight << std::endl;
                        std::cout << "[TagSorter DEBUG]  central weight before: " << SelectedTag->back().centralWeight() << std::endl;
                    }
                    WeightedObject NNLOPSobject;
                    NNLOPSobject.setCentralWeight( NNLOPSweight );
                    SelectedTag->back().includeWeights( NNLOPSobject );
                    if( debug_ ) {
                        std::cout << "[TagSorter DEBUG]  central weight after: " << SelectedTag->back().centralWeight() << std::endl;
                    }
                }
            }
            SelectedTagTruth->push_back(truth_obj);
            SelectedTag->back().setTagTruth( edm::refToPtr( edm::Ref<edm::OwnVector<TagTruthBase> >( rTagTruth, 0 ) ) );
            if( SelectedTagTruth->size() != 0 && debug_ ) {
                std::cout << "******************************" << std::endl;
                std::cout << " TRUTH FOR NO TAG..." << std::endl;
                std::cout << "* HTXS Category 0 (1) {1.1} [1.1 fine]: " << SelectedTagTruth->back().HTXSstage0bin() << " ("
                          << SelectedTagTruth->back().HTXSstage1bin() << ")" << " {"
                          << SelectedTagTruth->back().HTXSstage1p1bin() << "}" << " ["
                          << SelectedTagTruth->back().HTXSstage1p1binFine() << "]" << std::endl;
                std::cout << "* HTXS njets, pTH, pTV: " << SelectedTagTruth->back().HTXSnjets() << ", "
                          << SelectedTagTruth->back().HTXSpTH() << ", "
                          << SelectedTagTruth->back().HTXSpTV() << std::endl;
                std::cout << "******************************" << std::endl;
            }
        }
        // if(SelectedTag) cout << "SELECTED TAG EXISTS" << endl;
        // else cout << "SELECTED TAG DOES NOT" << endl;
        evt.put( std::move( SelectedTag ) );
        evt.put( std::move( SelectedTagTruth ) );
    }

    string TagSorter::tagName(DiPhotonTagBase::tag_t tagEnumVal) const {
        switch(tagEnumVal) {
        case DiPhotonTagBase::tag_t::kUndefined:
            return string("UNDEFINED");
        case DiPhotonTagBase::tag_t::kUntagged: 
            return string("Untagged");
        case DiPhotonTagBase::tag_t::kVBF:
            return string("VBF");
        case DiPhotonTagBase::tag_t::kTTHHadronic:
            return string("TTHHadronic");
        case DiPhotonTagBase::tag_t::kTTHLeptonic:
            return string("TTHLeptonic");
        case DiPhotonTagBase::tag_t::kTHQLeptonic:
            return string("THQLeptonic");
        case DiPhotonTagBase::tag_t::kTTHDiLepton:
            return string("TTHDiLepton");
        case DiPhotonTagBase::tag_t::kVHTight:
            return string("VHTight");
        case DiPhotonTagBase::tag_t::kVHLoose:
            return string("VHLoose");
        case DiPhotonTagBase::tag_t::kVHHadronic:
            return string("VHHadronic");
        case DiPhotonTagBase::tag_t::kVHEt:
            return string("VHEt");
        case DiPhotonTagBase::tag_t::kZHLeptonic:
            return string("ZHLeptonic");
        case DiPhotonTagBase::tag_t::kWHLeptonic:
            return string("WHLeptonic");
        case DiPhotonTagBase::tag_t::kVHLeptonicLoose:
            return string("VHLeptonicLoose");
        case DiPhotonTagBase::tag_t::kVHMet:
            return string("VHMet");
        case DiPhotonTagBase::tag_t::kStageOneCombined:
            return string("StageOne");
        }
        return string("TAG NOT ON LIST");
    }

}

typedef flashgg::TagSorter FlashggTagSorter;
DEFINE_FWK_MODULE( FlashggTagSorter );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

