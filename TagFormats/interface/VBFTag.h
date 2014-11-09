#ifndef FLASHgg_VBFTag_h
#define FLASHgg_VBFTag_h


#include "flashgg/TagFormats/interface/DiPhotonTagBase.h"
#include "flashgg/TagFormats/interface/VBFDiPhoDiJetMVAResult.h"
#include "flashgg/MicroAODFormats/interface/Jet.h"

namespace flashgg {

  class VBFTag: public DiPhotonTagBase {
  public:
    VBFTag();
    ~VBFTag();
    
		
  //  VBFTag(const VBFTag&);
    VBFTag(edm::Ptr<DiPhotonCandidate>,edm::Ptr<DiPhotonMVAResult>,edm::Ptr<VBFDiPhoDiJetMVAResult>);
  //  VBFTag(edm::Ptr<DiPhotonCandidate>,edm::Ptr<VBFDiPhoDiJetMVAResult>, int);// FIXME
    VBFTag(edm::Ptr<DiPhotonCandidate>,DiPhotonMVAResult,VBFDiPhoDiJetMVAResult);
    VBFTag* clone() const;
		const VBFDiPhoDiJetMVAResult VBFDiPhoDiJetMVA() const;
		const VBFMVAResult VBFMVA() const ;
		const Jet leadingJet() const; //needs to be validated
		const Jet subLeadingJet() const; //needs to be validated

	private:
		VBFDiPhoDiJetMVAResult vbfDiPhoDiJet_mva_result_;
	};

}

#endif
