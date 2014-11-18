#ifndef FLASHgg_DiPhotonTagBase_h
#define FLASHgg_DiPhotonTagBase_h

#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"
#include "flashgg/TagFormats/interface/DiPhotonMVAResult.h"

namespace flashgg {

  class DiPhotonTagBase {
  public:
    DiPhotonTagBase();
    virtual ~DiPhotonTagBase() {};
    DiPhotonTagBase(edm::Ptr<DiPhotonCandidate>,DiPhotonMVAResult);
    DiPhotonTagBase(edm::Ptr<DiPhotonCandidate>,edm::Ptr<DiPhotonMVAResult>);
    const edm::Ptr<DiPhotonCandidate> diPhoton() const { return dipho_; }
    const DiPhotonMVAResult diPhotonMVA() const { return mva_result_; }
    int getDiPhotonIndex() const {return diPhotonIndex_;}
    void setDiPhotonIndex(int i) { diPhotonIndex_ = i; }
    float getSumPt() const { return this->diPhoton()->getSumPt() ;}
    bool operator <(const DiPhotonTagBase & b) const;
    virtual DiPhotonTagBase* clone() const;
    void setCategoryNumber(int value) { category_number_ = value; }
    int getCategoryNumber() const { return category_number_; }
    

  private:
    DiPhotonMVAResult mva_result_;
    int category_number_;
    int diPhotonIndex_;
    edm::Ptr<DiPhotonCandidate> dipho_;
  };

}

#endif
