#ifndef FLASHgg_DiPhotonTagBase_h
#define FLASHgg_DiPhotonTagBase_h

#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/DiPhotonMVAResult.h"
#include "flashgg/DataFormats/interface/TagTruthBase.h"

namespace flashgg {

    class DiPhotonTagBase : public WeightedObject
    {
    public:
        DiPhotonTagBase();
        virtual ~DiPhotonTagBase();
        DiPhotonTagBase( edm::Ptr<DiPhotonCandidate>, DiPhotonMVAResult );
        DiPhotonTagBase( edm::Ptr<DiPhotonCandidate>, edm::Ptr<DiPhotonMVAResult> );
        DiPhotonTagBase( const DiPhotonTagBase & );
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
    private:
        DiPhotonMVAResult mva_result_;
        int category_number_;
        int diPhotonIndex_;
        edm::Ptr<DiPhotonCandidate> dipho_;
        edm::Ptr<TagTruthBase> truth_;
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

