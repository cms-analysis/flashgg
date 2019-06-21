#ifndef FLASHgg_DiPhotonTagBase_h
#define FLASHgg_DiPhotonTagBase_h

#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/DiPhotonMVAResult.h"
#include "flashgg/DataFormats/interface/TagTruthBase.h"

namespace flashgg {

    class DiPhotonTagBase : public WeightedObject
    {
    public:
        enum tag_t { kUndefined = 0, kUntagged, kVBF, kTTHHadronic, kTTHLeptonic, kTHQLeptonic, kTTHDiLepton, kVHTight, kVHLoose, kVHHadronic, kVHEt,  kZHLeptonic, kWHLeptonic, kVHLeptonicLoose, kVHMet };

        DiPhotonTagBase();
        virtual ~DiPhotonTagBase(); 
        DiPhotonTagBase( edm::Ptr<DiPhotonCandidate>, DiPhotonMVAResult );
        DiPhotonTagBase( edm::Ptr<DiPhotonCandidate>, edm::Ptr<DiPhotonMVAResult> );
        const edm::Ptr<DiPhotonCandidate> diPhoton() const { return dipho_; }

        const flashgg::Photon *leadingPhoton() const { return dipho_->leadingPhoton(); }
        const flashgg::Photon *subLeadingPhoton() const { return dipho_->subLeadingPhoton(); }
        const flashgg::SinglePhotonView *leadingView() const { return dipho_->leadingView(); }
        const flashgg::SinglePhotonView *subLeadingView() const { return dipho_->subLeadingView(); }

        const DiPhotonMVAResult diPhotonMVA() const { return mva_result_; }
        int diPhotonIndex() const {return diPhotonIndex_;}
        void setDiPhotonIndex( int i ) { diPhotonIndex_ = i; }
        float sumPt() const { return this->diPhoton()->sumPt() ;}
        bool operator <( const DiPhotonTagBase &b ) const;
        operator int() const { return categoryNumber(); }
        virtual DiPhotonTagBase *clone() const { return ( new DiPhotonTagBase( *this ) ); }
        void setCategoryNumber( int value ) { category_number_ = value; }
        int categoryNumber() const { return category_number_; }
        void setTagTruth( const edm::Ptr<TagTruthBase> value ) { truth_ = value; }
        const edm::Ptr<TagTruthBase> tagTruth() const { return truth_; }
        void setSystLabel( const std::string label ) { systLabel_ = label; }
        std::string systLabel() const { return systLabel_; }
        bool hasSyst( const string &label ) const { return ( systLabel_ == label );}
        void setIsGold ( int runNumber );
        void setIsGoldMC( bool isGold ) { isGold_ = isGold; }
        bool isGold() const { return isGold_; }
        virtual DiPhotonTagBase::tag_t tagEnum() const { return DiPhotonTagBase::kUndefined; }
        unsigned nOtherTags() const { 
            assert(otherTagTypes_.size() == otherTagCategories_.size());
            assert(otherTagTypes_.size() == otherTagIndices_.size());
            return otherTagTypes_.size(); 
        }
        void addOtherTag( const DiPhotonTagBase& other ) { 
            otherTagTypes_.push_back(other.tagEnum());
            otherTagCategories_.push_back(other.categoryNumber());
            otherTagIndices_.push_back(other.diPhotonIndex());
        }
        void addOtherTags( std::vector<std::tuple<DiPhotonTagBase::tag_t,int,int> > others ) { 
            for (unsigned i = 0 ; i < others.size() ; i++) {
                otherTagTypes_.push_back(std::get<0>(others[i]));
                otherTagCategories_.push_back(std::get<1>(others[i]));
                otherTagIndices_.push_back(std::get<2>(others[i]));
            }
        }
        DiPhotonTagBase::tag_t otherTagType( unsigned i ) const { return otherTagTypes_[i]; }
        int otherTagCategory( unsigned i ) const { return otherTagCategories_[i]; }
        int otherTagDiPhotonIndex ( unsigned i ) const { return otherTagIndices_[i]; }
    protected:
        DiPhotonMVAResult mva_result_;
        int category_number_;
        int diPhotonIndex_;
        edm::Ptr<DiPhotonCandidate> dipho_;
        edm::Ptr<TagTruthBase> truth_;
        string systLabel_;
        bool isGold_;
        //        std::vector<std::tuple<DiPhotonTagBase::tag_t,int,int> > otherTags_; // (type,category,diphoton index) 
        std::vector<DiPhotonTagBase::tag_t> otherTagTypes_;
        std::vector<int> otherTagCategories_;
        std::vector<int> otherTagIndices_;
    };

}

#endif
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

