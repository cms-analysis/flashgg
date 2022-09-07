#ifndef FLASHgg_VBFMVAResult_h
#define FLASHgg_VBFMVAResult_h

#include "DataFormats/Common/interface/Ptr.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/Jet.h"

namespace flashgg {
    
    class VBFMVAResult
    {
        
    public:
        VBFMVAResult();
        VBFMVAResult( edm::Ptr<VBFMVAResult> );
        virtual ~VBFMVAResult() {}
        // diJet Info
        //flashgg::Jet leadJet;
        //flashgg::Jet subleadJet;
        // 3rd jet
        //flashgg::Jet subsubleadJet;
        reco::Candidate::LorentzVector leadJet;
        reco::Candidate::LorentzVector subleadJet;
        reco::Candidate::LorentzVector subsubleadJet;
        reco::Candidate::LorentzVector fourthJet;
        
        edm::Ptr<flashgg::Jet> leadJet_ptr;
        edm::Ptr<flashgg::Jet> subleadJet_ptr;
        // 3rd and 4th jet
        edm::Ptr<flashgg::Jet> subsubleadJet_ptr; 
        edm::Ptr<flashgg::Jet> fourthJet_ptr; 
        // 4-vec of the 3 jets
        
        
        bool hasValidVBFTriJet;
        bool hasValidVBFTetraJet;
        
        // di-photon info 
        // flashgg::DiPhotonCandidate diphoton;
        
        // event based variables
        int  n_rec_jets;
        int  n_gen_jets;
        int  n_diphotons;
        
        // Input variables
        float dijet_leadEta ;
        float dijet_subleadEta;
        float dijet_abs_dEta;
        float dijet_LeadJPt ;
        float dijet_pt ;
        float dijet_SubJPt;
        float dijet_Zep;
        float dijet_dphi_trunc;
        float dijet_dipho_dphi;
        float dijet_dphi;
        float dijet_Mjj;
        float dijet_dy;
        float dijet_leady ;
        float dijet_subleady;
        float dijet_dipho_pt;
        float dijet_minDRJetPho;
        float dijet_centrality_gg;
        float dijet_centrality_j3;
        float dijet_centrality_g ;

        
        float dipho_PToM;
        float leadPho_PToM;
        float sublPho_PToM;
        
        // some 3-jet based variables 
        
        float VBFMVAValue() const {return vbfMvaResult_value;}
        float prob_VBF_value() const {return vbfMvaResult_prob_VBF;}
        float prob_ggH_value() const {return vbfMvaResult_prob_ggH;}
        float prob_bkg_value() const {return vbfMvaResult_prob_bkg;}
        // needed to go to native XGBoost score from TMVA version, inverting this https://github.com/jpata/mlglue/blob/master/mlglue/tree.py#L400-L409
        float transformedMvaValue(float result) const {return 1. / ( 1. + exp( 0.5*log( 2./(result+1.) - 1 ) ) );} 

        // DNN for VBF anomalous couplings 
        float dnnprob_bkg_value() const {return vbfDnnResult_prob_bkg;}
        float dnnprob_sm_value() const {return vbfDnnResult_prob_sm;}
        float dnnprob_bsm_value() const {return vbfDnnResult_prob_bsm;}
        // MELA (only D0- used for categorization for now)
        float mela_D0minus_value() const {return D0minus;}

        // Output
        float vbfMvaResult_value;
        float vbfMvaResult_value_bdt;
        float vbfMvaResult_value_bdtg;
        float vbfMvaResult_prob_VBF;
        float vbfMvaResult_prob_ggH;
        float vbfMvaResult_prob_bkg;

        float vbfDnnResult_prob_bkg;
        float vbfDnnResult_prob_sm;
        float vbfDnnResult_prob_bsm;
        float D0minus;
    };
    
    typedef std::map<edm::Ptr<DiPhotonCandidate>, VBFMVAResult> VBFMVAResultMap;
    
}

#endif
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

