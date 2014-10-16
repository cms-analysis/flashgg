#ifndef FLASHgg_VBFTag_h
#define FLASHgg_VBFTag_h


#include "flashgg/TagFormats/interface/DiPhotonTagBase.h"
#include "flashgg/TagFormats/interface/VBFDiPhoDiJetMVAResult.h"
#include "flashgg/MicroAODFormats/interface/Jet.h"

namespace flashgg {

  class VBFTag: public DiPhotonTagBase {
  public:
    VBFTag();
   // ~VBFTag();
    
		
    VBFTag(const VBFTag&);
		VBFTag(edm::Ptr<DiPhotonCandidate>,edm::Ptr<VBFDiPhoDiJetMVAResult>);
  //  VBFTag(edm::Ptr<DiPhotonCandidate>,edm::Ptr<VBFDiPhoDiJetMVAResult>, int);// FIXME

		const VBFDiPhoDiJetMVAResult VBFDiPhoDiJetMVA() const; //doesn't work for some reason
		const VBFMVAResult VBFMVA() const ; //doesn't work for some reason
		const Jet leadingJet() const; //needs to be validated
		const Jet subLeadingJet() const; //needs to be validated

		void setCategoryNumber(int n) { category_number_ = n; }
		int getCategoryNumber() const {return category_number_ ;}

		float dijet_leadEta_; 
		float	dijet_subleadEta_; 
		float	dijet_LeadJPt_; 
		float	dijet_SubJPt_; 
		float	dijet_Zep_; 
		float	dijet_Mjj_; 
		float	dijet_MVA_;
		float	bdt_combined_;     

	private:
		int category_number_;
		VBFDiPhoDiJetMVAResult vbfDiPhoDiJet_mva_result_;
	};

}

#endif
