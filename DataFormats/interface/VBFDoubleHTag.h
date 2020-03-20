#ifndef flashgg_VBFDoubleHTag
#define flashgg_VBFDoubleHTag

#include "TLorentzVector.h"

#include "flashgg/DataFormats/interface/DoubleHTag.h"
//#include "flashgg/DataFormats/interface/Jet.h"
//#include "DataFormats/Candidate/interface/LeafCandidate.h"
//#include "DataFormats/Math/interface/deltaR.h"

//#include "flashgg/Taggers/interface/FunctionHelpers.h"

namespace flashgg {

    class VBFDoubleHTag: public DoubleHTag 
    {
    public:
        VBFDoubleHTag();
        ~VBFDoubleHTag();

        VBFDoubleHTag( edm::Ptr<DiPhotonCandidate>, edm::Ptr<flashgg::Jet>, edm::Ptr<flashgg::Jet>, edm::Ptr<flashgg::Jet>, edm::Ptr<flashgg::Jet> );
        virtual VBFDoubleHTag *clone() const override;
        /// DiPhotonTagBase::tag_t tagEnum() const override { return DiPhotonTagBase::kDoubleH; }

/*        void setMVA(double x) { mva_ = x; }
        double MVA() const { return mva_; }
      //  void setMVAprob(std::vector<float> &x) const { mva_prob_ = x; }
      //  std::vector<float> MVAprob() const { return mva_prob_; }
        void setMX(double x) { MX_ = x; }
        double MX() const { return MX_; }
        void setGenMhh(double x) { genMhh_ = x; }
        double genMhh() const { return genMhh_; }
        void setGenCosThetaStar_CS(double x) { genCosThetaStar_CS_ = x; }
        double genCosThetaStar_CS() const { return genCosThetaStar_CS_; }
        float ttHScore() const { return ttHScore_; }
        double diphotonPtOverM() const {return diPhoton()->pt()/mass(); }
        double dijetPtOverM() const {return dijet().pt()/mass(); }

        const flashgg::Jet & leadJet() const { return *leadJet_; } 
        const flashgg::Jet & subleadJet() const { return *subleadJet_; } */
        const flashgg::Jet & VBFleadJet() const {return *VBFleadJet_;}
        const flashgg::Jet & VBFsubleadJet() const { return *VBFsubleadJet_;}
        const LorentzVector & diVBFjet() const {return diVBFjet_; }



        
 /*       const LorentzVector & dijet() const { return dijet_; }

        float getCosThetaStar_CS() const;
        float getCosThetaStar_CS_old(float ebeam) const;
        std::vector<float> CosThetaAngles() const;
        float HelicityCosTheta( TLorentzVector Booster, TLorentzVector Boosted) const;
        float getPhoJetMinDr() const;
        float getPhoJetOtherDr() const;
        float getSigmaMDecorr() const;
        float getSigmaMOverMJets() const;
        void  setSigmaMDecorrTransf( DecorrTransform* transfEBEB, DecorrTransform* transfNotEBEB){ transfEBEB_= transfEBEB; transfNotEBEB_=transfNotEBEB;}
        LorentzVector getdiHiggsP4() const {return p4();}
        void setBenchmarkReweight(std::vector<float> x) { benchmark_reweights_ = x; }
        float getBenchmarkReweight(int targetNode) const { return benchmark_reweights_[targetNode]; }
        void setEventNumber(double x) { eventNumber_ = x; }
        double eventNumber() const { return eventNumber_; }
        float getMinDeltaR_VBF_gamma() const;
        float getMinDeltaR_VBF_b() const;
        float ttHScore_;*/
        float sumET_, MET_, phiMET_, dPhi1_, dPhi2_, PhoJetMinDr_,PhoJetOtherDr_, njets_, Xtt0_, Xtt1_, pte1_, pte2_, ptmu1_, ptmu2_, ptdipho_, etae1_, etae2_, etamu1_, etamu2_, etadipho_, phie1_, phie2_, phimu1_, phimu2_, phidipho_, fabs_CosThetaStar_CS_, fabs_CosTheta_bb_, mjj_, ptjet1_, ptjet2_, etajet1_, etajet2_, phijet1_, phijet2_, MinDeltaR_VBF_gamma_, MinDeltaR_VBF_b_; 
      /*  float sumET() const {return sumET_;}
        float MET() const {return MET_;}
        float phiMET() const {return phiMET_;}
        float dPhi1() const {return dPhi1_;}
        float dPhi2() const {return dPhi2_;}
        float PhoJetMinDr() const {return PhoJetMinDr_;}
        float PhoJetOtherDr() const {return PhoJetOtherDr_;}
        float njets() const {return njets_;}
        float mjj() const {return mjj_;}
        float Xtt0() const {return Xtt0_;}
        float Xtt1() const {return Xtt1_;}
        float pte1() const {return pte1_;}
        float pte2() const {return pte2_;}
        float ptmu1() const {return ptmu1_;}
        float ptmu2() const {return ptmu2_;}
        float ptdipho() const {return ptdipho_;}
        float ptjet1() const {return ptjet1_;}
        float ptjet2() const {return ptjet2_;}
        float etajet1() const {return etajet1_;}
        float etajet2() const {return etajet2_;}
        float phijet1() const {return phijet1_;}
        float phijet2() const {return phijet2_;}
        float etae1() const {return etae1_;}
        float etae2() const {return etae2_;}
        float etamu1() const {return etamu1_;}
        float etamu2() const {return etamu2_;}
        float etadipho() const {return etadipho_;}
        float phie1() const {return phie1_;}
        float phie2() const {return phie2_;}
        float phimu1() const {return phimu1_;}
        float phimu2() const {return phimu2_;}
        float phidipho() const {return phidipho_;}
        float fabs_CosThetaStar_CS() const {return fabs_CosThetaStar_CS_;}
        float fabs_CosTheta_bb() const {return fabs_CosTheta_bb_;}*/
        //VBF related variables:
        float getVBFDeltaR_jg() const; 
        float getVBFDeltaR_jb() const; 
        float getVBFJet_mjj() const;
        float getVBFCentrality_jg() const;
        float getVBFCentrality_jb() const;
        float getVBFDelta_phi() const;
        float getVBFProd_eta() const;
        float getVBFJet_Delta_eta() const;
        float getVBFleadJet_pt() const;
        float getVBFsubleadJet_pt() const;
        float getVBFleadJet_eta() const;
        float getVBFsubleadJet_eta() const;
        float getVBFleadJet_phi() const;
        float getVBFsubleadJet_phi() const;
        float getVBFleadJet_pz() const;
        float getVBFsubleadJet_pz() const;
        float getVBFleadJet_QGL() const;
        float getVBFsubleadJet_QGL() const;
        float getVBFleadJet_PUID() const;
        float getVBFsubleadJet_PUID() const;
        float getVBF_angleHH() const;
        float getVBF_dRHH() const;
        float getVBF_etaHH() const;
        float getdiVBFjet_pt() const;
        /*float VBFMjj_, VBFdelta_eta_, VBFeta1_, VBFeta2_, VBFphi1_, VBFphi2_, VBFpt1_, VBFpt2_, VBFpx1_, VBFpx2_, VBFpy1_, VBFpy2_, VBFpz1_, VBFpz2_, Njet_; 
        float VBFMjj() const {return VBFMjj_; }
        float VBFdelta_eta() const {return VBFdelta_eta_;}
        float VBFeta1() {return VBFeta1_;}
        float VBFphi1(){return VBFphi1_;}
        float VBFpt1(){return VBFpt1_;}
        float VBFpx1(){return VBFpx1_;}
        float VBFpy1(){return VBFpy1_;}
        float VBFpz1(){return VBFpz1_;}
        float VBFeta2(){return VBFeta2_;}
        float VBFphi2(){return VBFphi2_;}
        float VBFpt2(){return VBFpt2_;}
        float VBFpx2(){return VBFpx2_;}
        float VBFpy2(){return VBFpy2_;}
        float VBFpz2(){return VBFpz2_;}
        float Njet() {return Njet_;}*/
    private:
 //       double mva_, MX_, genMhh_,genCosThetaStar_CS_;
 //       vector<float> benchmark_reweights_;
 //       std::vector<float> mva_prob_;
 //        long eventNumber_;
        edm::Ptr<flashgg::Jet>  VBFleadJet_, VBFsubleadJet_ ;
        LorentzVector  diVBFjet_ ;
 //       DecorrTransform* transfEBEB_;
 //       DecorrTransform* transfNotEBEB_;
            
    };
}

#endif
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

