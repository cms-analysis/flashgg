#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"

namespace flashgg {

  class DiPhotonUntaggedCategory: public DiPhotonTagBase {
  public:
    DiPhotonUntaggedCategory();
    ~DiPhotonUntaggedCategory();

    DiPhotonUntaggedCategory(edm::Ptr<DiPhotonCandidate>,DiPhotonMVAResult);
    DiPhotonUntaggedCategory(edm::Ptr<DiPhotonCandidate>,edm::Ptr<DiPhotonMVAResult>);
    virtual DiPhotonUntaggedCategory* clone() const;


	private:



	};
}
