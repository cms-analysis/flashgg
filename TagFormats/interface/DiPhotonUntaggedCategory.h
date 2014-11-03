#include "flashgg/TagFormats/interface/DiPhotonTagBase.h"

namespace flashgg {

  class DiPhotonUntaggedCategory: public DiPhotonTagBase {
  public:
    DiPhotonUntaggedCategory();
    //    ~DiPhotonUntaggedCategory();
  //  DiPhotonUntaggedCategory(const DiPhotonUntaggedCategory&);
    DiPhotonUntaggedCategory(edm::Ptr<DiPhotonCandidate> dipho,edm::Ptr<DiPhotonMVAResult> mvares);
    //    DiPhotonUntaggedCategory(edm::Ptr<DiPhotonCandidate>,edm::Ptr<DiPhotonMVAResult>, int);

	/* void setVtxProb(float n) { vtxprob_ = n; }
    float getVtxProb() const { return vtxprob_; }
    void setSigmaMrvoM(float n) { sigmaMrvoM_ = n; }
    float getSigmaMrvoM() const { return sigmaMrvoM_; }
    void setSigmaMwvoM(float n) { sigmaMwvoM_ = n; }
    float getSigmaMwvoM() const { return sigmaMwvoM_; }
    void setDiphoMva(float n) { dipho_mva_ = n; }
    float getDiphoMva() const { return dipho_mva_; }*/

	//	DiPhotonMVAResult getDiPhoMVAResult() const {return dipho_mva_res_;}// doesn't work for some reason
	//	void setDiPhoMVAResult(edm::Ptr<DiPhotonMVAResult> x) {dipho_mva_res_ = *x; sigmaMrvoM_ = x->sigmarv;}
	//	DiPhotonMVAResult dipho_mva_res_;
	private:
    //		int category_number_;
//		float vtxprob_; 
//		float dipho_mva_; 
//		float sigmaMrvoM_;
//		float sigmaMwvoM_;

	};
}
