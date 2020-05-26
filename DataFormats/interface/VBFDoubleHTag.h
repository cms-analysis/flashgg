#ifndef flashgg_VBFDoubleHTag
#define flashgg_VBFDoubleHTag

#include "TLorentzVector.h"

#include "flashgg/DataFormats/interface/DoubleHTag.h"

namespace flashgg {

    class VBFDoubleHTag: public DoubleHTag 
    {
    public:
        VBFDoubleHTag();
        ~VBFDoubleHTag();

        VBFDoubleHTag( edm::Ptr<DiPhotonCandidate>, edm::Ptr<flashgg::Jet>, edm::Ptr<flashgg::Jet>, edm::Ptr<flashgg::Jet>, edm::Ptr<flashgg::Jet> );
        virtual VBFDoubleHTag *clone() const override;

        //---setters
        // b quark information:   
        void setb1_pt( double x) {b1_pt_ = x;}
        double b1_pt() const {return b1_pt_;}
        void setb2_pt( double x) {b2_pt_ = x;}
        double b2_pt() const {return b2_pt_;}
        void setb1_eta( double x) {b1_eta_ = x;}
        double b1_eta() const {return b1_eta_;}
        void setb2_eta( double x) {b2_eta_ = x;}
        double b2_eta() const {return b2_eta_;}
        void setb1_phi( double x) {b1_phi_ = x;}
        double b1_phi() const {return b1_phi_;}
        void setb2_phi( double x) {b2_phi_ = x;}
        double b2_phi() const {return b2_phi_;}
        void setb1_pz( double x) {b1_pz_ = x;}
        double b1_pz() const {return b1_pz_;}
        void setb2_pz( double x) {b2_pz_ = x;}
        double b2_pz() const {return b2_pz_;}
        void setMbb_gen( double x) {Mbb_gen_ = x;}
        double Mbb_gen() const {return Mbb_gen_;}

        // forward-backward information:
        void setq1_pt( double x) {q1_pt_ = x;}
        double q1_pt() const {return q1_pt_;}
        void setq2_pt( double x) {q2_pt_ = x;}
        double q2_pt() const {return q2_pt_;}
        void setq1_eta( double x) {q1_eta_ = x;}
        double q1_eta() const {return q1_eta_;}
        void setq2_eta( double x) {q2_eta_ = x;}
        double q2_eta() const {return q2_eta_;}
        void setq1_phi( double x) {q1_phi_ = x;}
        double q1_phi() const {return q1_phi_;}
        void setq2_phi( double x) {q2_phi_ = x;}
        double q2_phi() const {return q2_phi_;}
        void setq1_pz( double x) {q1_pz_ = x;}
        double q1_pz() const {return q1_pz_;}
        void setq2_pz( double x) {q2_pz_ = x;}
        double q2_pz() const {return q2_pz_;}
        void setq1_ID( int x) {q1_ID_ = x;}
        int q1_ID() const {return q1_ID_;}
        void setq2_ID( int x) {q2_ID_ = x;}
        int q2_ID() const {return q2_ID_;}
        void setMqq_gen( double x) {Mqq_gen_ = x;}
        double Mqq_gen() const {return Mqq_gen_;}

        //---getters
        const flashgg::Jet & VBFleadJet() const {return *VBFleadJet_;}
        const flashgg::Jet & VBFsubleadJet() const { return *VBFsubleadJet_;}
        const LorentzVector & diVBFjet() const {return diVBFjet_; }

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

    private:

        edm::Ptr<flashgg::Jet>  VBFleadJet_, VBFsubleadJet_ ;
        LorentzVector  diVBFjet_ ;
        double b1_pt_, b2_pt_, b1_eta_, b2_eta_, b1_phi_, b2_phi_, b1_pz_, b2_pz_, Mbb_gen_,
            q1_ID_, q2_ID_, q1_pt_, q2_pt_, q1_eta_, q2_eta_, q1_phi_, q2_phi_, q1_pz_, q2_pz_, Mqq_gen_;            
            
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

