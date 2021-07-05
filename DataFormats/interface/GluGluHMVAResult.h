#ifndef FLASHgg_GluGluHMVAResult_h
#define FLASHgg_GluGluHMVAResult_h

#include "DataFormats/Common/interface/Ptr.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/Jet.h"

namespace flashgg {
    
    class GluGluHMVAResult
    {
        
    public:
        GluGluHMVAResult();
        GluGluHMVAResult( edm::Ptr<GluGluHMVAResult> );
        virtual ~GluGluHMVAResult() {}
        // diJet Info
        //flashgg::Jet leadJet;
        //flashgg::Jet subleadJet;
        // 3rd jet
        //flashgg::Jet subsubleadJet;
        reco::Candidate::LorentzVector leadJet;
        reco::Candidate::LorentzVector subleadJet;
        reco::Candidate::LorentzVector subsubleadJet;
        
        edm::Ptr<flashgg::Jet> leadJet_ptr;
        edm::Ptr<flashgg::Jet> subleadJet_ptr;
        // 3rd jet
        edm::Ptr<flashgg::Jet> subsubleadJet_ptr; 
        // 4-vec of the 3 jets
        
        
        // di-photon info 
        // flashgg::DiPhotonCandidate diphoton;
        
        
        // Input variables - jets
        int n_rec_jets;
        int dijet_Mjj;
        float dijet_leadEta;
        float dijet_subleadEta;
        float dijet_subsubleadEta;
        float dijet_leadJPt;
        float dijet_SubleadJPt;
        float dijet_SubsubleadJPt;
        float dijet_leadPUMVA;
        float dijet_subleadPUMVA;
        float dijet_subsubleadPUMVA;
        float dijet_leadDeltaPhi;
        float dijet_subleadDeltaPhi;
        float dijet_subsubleadDeltaPhi;
        float dijet_leadDeltaEta;
        float dijet_subleadDeltaEta;
        float dijet_subsubleadDeltaEta;

        // Input variables - diphoton
        float dipho_leadIDMVA;
        float dipho_subleadIDMVA;
        float dipho_lead_ptoM;
        float dipho_sublead_ptoM;
        float dipho_leadEta;
        float dipho_subleadEta;
        float CosPhi; 
        float vtxprob; 
        float sigmarv; 
        float sigmawv; 
        float diphopt; 

        //getter fns
        float prob_0J_PTH_0_10() const {return ggHMVAResult_prob_0J_PTH_0_10;} 
        float prob_0J_PTH_GT10() const {return ggHMVAResult_prob_0J_PTH_GT10;} 
        float prob_1J_PTH_0_60() const {return ggHMVAResult_prob_1J_PTH_0_60;} 
        float prob_1J_PTH_60_120() const {return ggHMVAResult_prob_1J_PTH_60_120;} 
        float prob_1J_PTH_120_200() const {return ggHMVAResult_prob_1J_PTH_120_200;} 
        float prob_GE2J_MJJ_0_350_PTH_0_60() const {return ggHMVAResult_prob_GE2J_MJJ_0_350_PTH_0_60;} 
        float prob_GE2J_MJJ_0_350_PTH_60_120() const {return ggHMVAResult_prob_GE2J_MJJ_0_350_PTH_60_120;} 
        float prob_GE2J_MJJ_0_350_PTH_120_200() const {return ggHMVAResult_prob_GE2J_MJJ_0_350_PTH_120_200;} 
        float prob_PTH_GT200() const {return ggHMVAResult_prob_PTH_GT200;} 

        float transformedMvaValue(float result) const {return 1. / ( 1. + exp( 0.5*log( 2./(result+1.) - 1 ) ) );} 
        
        // Output
        float ggHMVAResult_prob_0J_PTH_0_10;
        float ggHMVAResult_prob_0J_PTH_GT10;
        float ggHMVAResult_prob_1J_PTH_0_60;
        float ggHMVAResult_prob_1J_PTH_60_120; 
        float ggHMVAResult_prob_1J_PTH_120_200; 
        float ggHMVAResult_prob_GE2J_MJJ_0_350_PTH_0_60;
        float ggHMVAResult_prob_GE2J_MJJ_0_350_PTH_60_120; 
        float ggHMVAResult_prob_GE2J_MJJ_0_350_PTH_120_200;
        float ggHMVAResult_prob_PTH_GT200; 
        
    };
    
    typedef std::map<edm::Ptr<DiPhotonCandidate>, GluGluHMVAResult> GluGluHMVAResultMap;
    
}

#endif
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

