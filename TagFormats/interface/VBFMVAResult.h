#ifndef FLASHgg_VBFMVAResult_h
#define FLASHgg_VBFMVAResult_h

#include "DataFormats/Common/interface/Ptr.h"
#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"
#include "flashgg/MicroAODFormats/interface/Jet.h"


namespace flashgg {

  class VBFMVAResult {

  public:
    VBFMVAResult();
   //  VBFMVAResult(const VBFMVAResult&) = default;  // C++11 only? Should happen automagically anyway
		VBFMVAResult(edm::Ptr<VBFMVAResult>);
		// diJet Info
		
		Jet leadJet;
		Jet subleadJet;
		
		
    // Input variables
   	float dijet_leadEta ;   
    float dijet_subleadEta;
    float dijet_LeadJPt ;
    float dijet_SubJPt;
    float dijet_Zep;
   	float dijet_dPhi_trunc; 
		float dijet_Mjj;
		float dipho_PToM;
    float VBFMVAValue() const {return vbfMvaResult_value;}

    // Output
    float vbfMvaResult_value;
  };

  typedef std::map<edm::Ptr<DiPhotonCandidate>,VBFMVAResult> VBFMVAResultMap;

}

#endif
