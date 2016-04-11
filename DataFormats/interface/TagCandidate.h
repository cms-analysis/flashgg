#ifndef FLASHgg_TagCandidate_h
#define FLASHgg_TagCandidate_h

#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"

namespace flashgg {
    class TagCandidate : public reco::CompositeCandidate
    {
    public:
        TagCandidate();
        TagCandidate( const flashgg::DiPhotonTagBase* tag, int triggerBits[3] );
        ~TagCandidate();

        const edm::Ptr<DiPhotonCandidate> diPhoton() const { return tag_->diPhoton(); }
        const flashgg::DiPhotonMVAResult diPhotonMVA() const { return tag_->diPhotonMVA(); }
        float diPhoMVAValue() const      { return tag_->diPhotonMVA().mvaValue(); }
        int categoryNumber () const      { return tag_->categoryNumber() + cat_; } 
        float vbfMVA() const             { return vbfMVA_;            }
        float combindedMVA() const       { return combindedMVA_;      }
        float dijet_leadEta () const     { return dijet_leadEta_;     }
        float dijet_subleadEta() const   { return dijet_subleadEta_;  }
        float dijet_abs_dEta() const     { return dijet_abs_dEta_;    }
        float dijet_LeadJPt () const     { return dijet_LeadJPt_;     }
        float dijet_SubJPt() const       { return dijet_SubJPt_;      }
        float dijet_Zep() const          { return dijet_Zep_;         }
        float dijet_dphi_trunc() const   { return dijet_dphi_trunc_;  }
        float dijet_dipho_dphi() const   { return dijet_dipho_dphi_;  }
        float dijet_Mjj() const          { return dijet_Mjj_;         }
        float dijet_dy() const           { return dijet_dy_;          }
        float dijet_leady () const       { return dijet_leady_;       }
        float dijet_subleady() const     { return dijet_subleady_;    }
        float dijet_dipho_pt() const     { return dijet_dipho_pt_;    }
        float dijet_minDRJetPho() const  { return dijet_minDRJetPho_; }
        int triggerBit(int i) const      { return triggerBits_[i];    }
        int genMatchLead() const         { return int (tag_->diPhoton()->leadingPhoton()->hasGenMatchType() && 
                                                      (tag_->diPhoton()->leadingPhoton()->genMatchType() == 1)); }
        int genMatchSubLead() const      { return int (tag_->diPhoton()->subLeadingPhoton()->hasGenMatchType() && 
                                                      (tag_->diPhoton()->subLeadingPhoton()->genMatchType() == 1)); }
        
    private:        
        const flashgg::DiPhotonTagBase* tag_;
        int triggerBits_[3];
        int cat_;
        float vbfMVA_;           
        float combindedMVA_;     
        float dijet_leadEta_;   
        float dijet_subleadEta_; 
        float dijet_abs_dEta_;   
        float dijet_LeadJPt_;   
        float dijet_SubJPt_;     
        float dijet_Zep_;        
        float dijet_dphi_trunc_; 
        float dijet_dipho_dphi_; 
        float dijet_Mjj_;        
        float dijet_dy_;         
        float dijet_leady_;     
        float dijet_subleady_;   
        float dijet_dipho_pt_;   
        float dijet_minDRJetPho_;
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

