#ifndef FLASHgg_DiPhotonTagBase_h
#define FLASHgg_DiPhotonTagBase_h

#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"
#include "flashgg/TagFormats/interface/DiPhotonMVAResult.h"

namespace flashgg {

  class DiPhotonTagBase {
  public:
    DiPhotonTagBase();
    virtual ~DiPhotonTagBase() {}
    DiPhotonTagBase(edm::Ptr<DiPhotonCandidate>,edm::Ptr<DiPhotonMVAResult>);
    const DiPhotonCandidate* diPhoton() const;
    const DiPhotonMVAResult diPhotonMVA() const;
    int diPhotonIndex;
    int getDiPhotonIndex() const {return diPhotonIndex;}
    float getSumPt() const { return this->diPhoton()->getSumPt() ;}
    bool operator <(DiPhotonTagBase const& b);
    bool operator >(DiPhotonTagBase const& b);
    void setCategoryNumber(int value) { category_number_ = value; }
    int getCategoryNumber() const { return category_number_; }
    

  private:
    DiPhotonMVAResult mva_result_;
    int category_number_;
    edm::Ptr<DiPhotonCandidate> dipho_;
  };

}

#endif
