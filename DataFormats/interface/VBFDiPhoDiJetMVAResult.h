#ifndef FLASHgg_VBFDiPhoDiJetMVAResult_h
#define FLASHgg_VBFDiPhoDiJetMVAResult_h

#include "DataFormats/Common/interface/Ptr.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/VBFMVAResult.h"

namespace flashgg {

  class VBFDiPhoDiJetMVAResult {

  public:
    VBFDiPhoDiJetMVAResult();
 //    VBFDiPhoDiJetMVAResult(VBFMVAResult);
    //    VBFDiPhoDiJetMVAResult(const VBFDiPhoDiJetMVAResult&) = default;  // C++11 only? Should happen automagically anyway

    // Input variables
		float dijet_mva;
		float dipho_mva;
		float dipho_PToM;

	
    // Output
    float vbfDiPhoDiJetMvaResult;
	
		// Also store previous dijet MVA result
		VBFMVAResult vbfMvaResult;
		
		VBFMVAResult getVBFMVAResult() const { return vbfMvaResult;}
		float VBFDiPhoDiJetMVAValue() const { return vbfDiPhoDiJetMvaResult;}

  };

  typedef std::map<edm::Ptr<DiPhotonCandidate>,VBFDiPhoDiJetMVAResult> VBFDiPhoDiJetMVAResultMap;

}

#endif
