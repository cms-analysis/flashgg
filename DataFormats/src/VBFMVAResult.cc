#include "flashgg/TagFormats/interface/VBFMVAResult.h"


namespace flashgg {

  VBFMVAResult::VBFMVAResult() :
	
		leadJet(),
		subleadJet(),
    dijet_leadEta(-9999.), 
    dijet_subleadEta(-9999.), 
    dijet_LeadJPt(-9999.), 
    dijet_SubJPt(-9999.), 
    dijet_Zep(-9999.), 
   	dijet_dPhi_trunc(-9999.), 
		dijet_Mjj(-9999.), 
		dipho_PToM(-9999.), 
    vbfMvaResult_value(-9999.) {}


		VBFMVAResult::VBFMVAResult(edm::Ptr<VBFMVAResult> x )
		{
		leadJet = x->leadJet;
		subleadJet = x->subleadJet;
    dijet_leadEta   =x-> dijet_leadEta    ;            
    dijet_subleadEta =x->dijet_subleadEta ;
    dijet_LeadJPt    =x->dijet_LeadJPt ;
    dijet_SubJPt     =x->dijet_SubJPt ;
    dijet_Zep        =x->dijet_Zep ;
   	dijet_dPhi_trunc =x->dijet_dPhi_trunc ;
		dijet_Mjj        =x->dijet_Mjj ;
		dipho_PToM       =x->dipho_PToM ;
    vbfMvaResult_value     =x->vbfMvaResult_value; 
		}
}
