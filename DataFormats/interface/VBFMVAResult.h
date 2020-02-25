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
        
        edm::Ptr<flashgg::Jet> leadJet_ptr;
        edm::Ptr<flashgg::Jet> subleadJet_ptr;
        // 3rd jet
        edm::Ptr<flashgg::Jet> subsubleadJet_ptr; 
        // 4-vec of the 3 jets
        
        
        bool hasValidVBFTriJet;
        
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
        
        // Output
        float vbfMvaResult_value;
        float vbfMvaResult_value_bdt;
        float vbfMvaResult_value_bdtg;
        float vbfMvaResult_prob_VBF;
        float vbfMvaResult_prob_ggH;
        float vbfMvaResult_prob_bkg;
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

