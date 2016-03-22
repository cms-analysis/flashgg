#ifndef FLASHgg_DiPhotonTagBase_h
#define FLASHgg_DiPhotonTagBase_h

#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/DiPhotonMVAResult.h"
#include "flashgg/DataFormats/interface/TagTruthBase.h"

namespace flashgg {

    class DiPhotonTagBase : public WeightedObject
    {
    public:
        enum tag_t { kUndefined = 0, kUntagged, kVBF, kTTHHadronic, kTTHLeptonic, kVHTight, kVHLoose, kVHHadronic, kVHEt };

        DiPhotonTagBase();
        virtual ~DiPhotonTagBase(); 
        DiPhotonTagBase( edm::Ptr<DiPhotonCandidate>, DiPhotonMVAResult );
        DiPhotonTagBase( edm::Ptr<DiPhotonCandidate>, edm::Ptr<DiPhotonMVAResult> );
        const edm::Ptr<DiPhotonCandidate> diPhoton() const { return dipho_; }
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
        unsigned nOtherTags() const { return otherTags_.size(); }
        void addOtherTag( const DiPhotonTagBase& other ) { otherTags_.emplace_back( other.tagEnum(), other.diPhotonIndex() ); }
        void addOtherTagTypeAndIndex( DiPhotonTagBase::tag_t tag_i, int dipho_i ) { otherTags_.emplace_back( tag_i, dipho_i ); }
        void addOtherTags( std::vector<std::pair<DiPhotonTagBase::tag_t,int> > others ) { otherTags_ = others; }
        DiPhotonTagBase::tag_t otherTagType( unsigned i ) const { return otherTags_[i].first; };
        int otherTagDiPhotonIndex ( unsigned i ) const { return otherTags_[i].second; };
    private:
        DiPhotonMVAResult mva_result_;
        int category_number_;
        int diPhotonIndex_;
        edm::Ptr<DiPhotonCandidate> dipho_;
        edm::Ptr<TagTruthBase> truth_;
        string systLabel_;
        bool isGold_;
        std::vector<std::pair<DiPhotonTagBase::tag_t,int> > otherTags_;
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

