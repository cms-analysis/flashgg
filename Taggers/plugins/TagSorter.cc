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

#include "TMVA/Reader.h"
#include "TMath.h"
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
    };

    TagSorter::TagSorter( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) )
    {

        massCutUpper = iConfig.getUntrackedParameter<double>( "massCutUpper", 180. );
        massCutLower = iConfig.getUntrackedParameter<double>( "massCutLower", 100. );

        const auto &vpset = iConfig.getParameterSetVector( "TagPriorityRanges" );

        vector<string> labels;

        for( const auto &pset : vpset ) {
            InputTag tag = pset.getParameter<InputTag>( "TagName" );
            int c1 = pset.getUntrackedParameter<int>( "MinCategory", -999 );
            int c2 = pset.getUntrackedParameter<int>( "MaxCategory", 999 );
            unsigned int i = 0;
            for( ; i < labels.size() ; i++ ) {
                //                std::cout << labels[i] << " " << tag.label() << std::endl;
                if( labels[i] == tag.label() ) { break; }
            }
            if( i == TagList_.size() ) {
                labels.push_back( tag.label() );
                TagList_.push_back( consumes<View<flashgg::DiPhotonTagBase> >( tag ) );
            }
            TagPriorityRanges.emplace_back( tag.label(), c1, c2, i );
        }

        produces<edm::OwnVector<flashgg::DiPhotonTagBase> >();
        produces<edm::OwnVector<flashgg::TagTruthBase> >();
    }

    void TagSorter::produce( Event &evt, const EventSetup & )
    {

        auto_ptr<edm::OwnVector<flashgg::DiPhotonTagBase> > SelectedTag( new edm::OwnVector<flashgg::DiPhotonTagBase> );
        auto_ptr<edm::OwnVector<flashgg::TagTruthBase> > SelectedTagTruth( new edm::OwnVector<flashgg::TagTruthBase> );

        //		int priority = -1; // for debug
        for( auto tpr = TagPriorityRanges.begin() ; tpr != TagPriorityRanges.end() ; tpr++ ) {
            //    	priority += 1; // for debug

            Handle<View<flashgg::DiPhotonTagBase> > TagVectorEntry;
            evt.getByToken( TagList_[tpr->collIndex], TagVectorEntry );

            //            Handle<View<flashgg::TagTruthBase> > TruthVectorEntry;
            //            evt.getByToken( TagList_[tpr->collIndex], TruthVectorEntry );

            //            std::cout << std::cout << " TruthVectorEntry->size() = " << TruthVectorEntry->size() << std::endl;
            //            assert ( TruthVectorEntry->size() == TagVectorEntry->size() );

            edm::RefProd<edm::OwnVector<TagTruthBase> > rTagTruth = evt.getRefBeforePut<edm::OwnVector<TagTruthBase> >();

            //		const PtrVector<flashgg::DiPhotonTagBase>& TagPointers =  TagVectorEntry->ptrVector();
            // get Tags by requesting them as a DiPhotonTagBase, from which they inherit.

            int chosenIndex = -1 ; //this will become the index of the highest priority candidate (operator<)

            // Looking from highest priority to lowest, check if the tag has any entries.
            for( unsigned int  TagPointerLoop = 0; TagPointerLoop < TagVectorEntry->size() ; TagPointerLoop++ )        {

                float mass = TagVectorEntry->ptrAt( TagPointerLoop )->diPhoton()->mass();
                int category = TagVectorEntry->ptrAt( TagPointerLoop )->categoryNumber();

                // std::cout << "[DEBUG]" << tpr->name << " " << tpr->minCat << " " << tpr->maxCat << " "
                //           << mass << " " << category << " " << TagPointerLoop << std::endl;

                // ignore candidate tags with category number outside the present range we're looking at
                if( category < tpr->minCat || category > tpr->maxCat ) { continue ; }

                // ignore candidate tags with diphoton outside of the allowed mass range.
                if( ( mass < massCutLower ) || ( mass > massCutUpper ) ) {continue ;}

                // All the real work for prioritizing inside a tag type is done inside DiPhotonTagBase::operator<
                if( chosenIndex == -1 || ( TagVectorEntry->ptrAt( chosenIndex ).get() < TagVectorEntry->ptrAt( TagPointerLoop ).get() ) );
                chosenIndex = TagPointerLoop;
            }

            if( chosenIndex != -1 ) {
                SelectedTag->push_back( *TagVectorEntry->ptrAt( chosenIndex ) );
                edm::Ptr<TagTruthBase> truth = TagVectorEntry->ptrAt( chosenIndex )->tagTruth();
                if( truth.isNonnull() ) {
                    SelectedTagTruth->push_back( *truth );
                    SelectedTag->back().setTagTruth( edm::refToPtr( edm::Ref<edm::OwnVector<TagTruthBase> >( rTagTruth, 0 ) ) ); // Normally this 0 would be the index number
                }
                //debug message:
                //  std::cout << "[DEBUG] Priority " << priority << " Tag Found! Tag entry "<< chosenIndex  << " with sumPt "
                //    	     << TagVectorEntry->ptrAt(chosenIndex)->sumPt() << ", systLabel " << TagVectorEntry->ptrAt(chosenIndex)->systLabel() << ", systLabelHash " << TagVectorEntry->ptrAt(chosenIndex)->systLabelHash() <<   std::endl;
                break;
            } else {
                //debug message
                // std::cout << "[DEBUG] No Priority " << priority << " Tag ..., looking for Priority " << (priority+1) << " Tag.. " << std::endl;
            }
        }

        assert( SelectedTag->size() == 1 || SelectedTag->size() == 0 );
        evt.put( SelectedTag );
        evt.put( SelectedTagTruth );
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

