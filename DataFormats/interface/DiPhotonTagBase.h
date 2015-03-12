#ifndef FLASHgg_DiPhotonTagBase_h
#define FLASHgg_DiPhotonTagBase_h

#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/DiPhotonMVAResult.h"

namespace flashgg {

  class DiPhotonTagBase {
  public:
    DiPhotonTagBase();
    virtual ~DiPhotonTagBase();
    DiPhotonTagBase(edm::Ptr<DiPhotonCandidate>,DiPhotonMVAResult);
    DiPhotonTagBase(edm::Ptr<DiPhotonCandidate>,edm::Ptr<DiPhotonMVAResult>);
    const edm::Ptr<DiPhotonCandidate> diPhoton() const { return dipho_; }
    const DiPhotonMVAResult diPhotonMVA() const { return mva_result_; }
    int diPhotonIndex() const {return diPhotonIndex_;}
    void setDiPhotonIndex(int i) { diPhotonIndex_ = i; }
    float sumPt() const { return this->diPhoton()->sumPt() ;}
    bool operator <(const DiPhotonTagBase & b) const;
    operator int() const { return categoryNumber(); }
    virtual DiPhotonTagBase* clone() const;
    void setCategoryNumber(int value) { category_number_ = value; }
    int categoryNumber() const { return category_number_; }

  private:
    DiPhotonMVAResult mva_result_;
    int category_number_;
    int diPhotonIndex_;
    edm::Ptr<DiPhotonCandidate> dipho_;
  };

}

#endif
