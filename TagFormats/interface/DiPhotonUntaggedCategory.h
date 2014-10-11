#include "flashgg/TagFormats/interface/DiPhotonTagBase.h"

namespace flashgg {

  class DiPhotonUntaggedCategory: public DiPhotonTagBase {
  public:
    DiPhotonUntaggedCategory();
    //    ~DiPhotonUntaggedCategory();
    DiPhotonUntaggedCategory(const DiPhotonUntaggedCategory&);
    DiPhotonUntaggedCategory(edm::Ptr<DiPhotonCandidate> dipho,edm::Ptr<DiPhotonMVAResult> mvares);
    DiPhotonUntaggedCategory(edm::Ptr<DiPhotonCandidate>,edm::Ptr<DiPhotonMVAResult>, int);
    void setCategoryNumber(int n) { category_number_ = n; }
    int getCategoryNumber() { return category_number_; }
  private:
    int category_number_;
  };
}
