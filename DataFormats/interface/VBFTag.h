#ifndef FLASHgg_VBFTag_h
#define FLASHgg_VBFTag_h


#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "flashgg/DataFormats/interface/VBFDiPhoDiJetMVAResult.h"
#include "flashgg/DataFormats/interface/Jet.h"

namespace flashgg {

    class VBFTag: public DiPhotonTagBase
    {
    public:
        VBFTag();
        ~VBFTag();

        VBFTag *clone() const { return ( new VBFTag( *this ) ); }

        VBFTag( edm::Ptr<DiPhotonCandidate>, edm::Ptr<DiPhotonMVAResult>, edm::Ptr<VBFDiPhoDiJetMVAResult> );
        VBFTag( edm::Ptr<DiPhotonCandidate>, DiPhotonMVAResult, VBFDiPhoDiJetMVAResult );

        const VBFDiPhoDiJetMVAResult VBFDiPhoDiJetMVA() const;
        const VBFMVAResult VBFMVA() const ;
        const reco::Candidate::LorentzVector leadingJet() const; //needs to be validated
        const reco::Candidate::LorentzVector subLeadingJet() const; //needs to be validated
        const reco::Candidate::LorentzVector subSubLeadingJet() const; //needs to be validated // 3rd Jet needed for VBF studies
        
        const edm::Ptr<flashgg::Jet> leadingJet_ptr() const; //needs to be validated
        const edm::Ptr<flashgg::Jet> subLeadingJet_ptr() const; //needs to be validated
        const edm::Ptr<flashgg::Jet> subSubLeadingJet_ptr() const; //needs to be validated // 3rd Jet needed for VBF studies
        
        const float leading_rms   () const { return  hasValidVBFDiJet() ? vbfDiPhoDiJet_mva_result_.vbfMvaResult.leadJet_ptr->rms() : -9999.;}
        const float subLeading_rms() const { return  hasValidVBFDiJet() ? vbfDiPhoDiJet_mva_result_.vbfMvaResult.subleadJet_ptr->rms() : -9999.;}
        const float leading_QGL   () const { return  hasValidVBFDiJet() ? vbfDiPhoDiJet_mva_result_.vbfMvaResult.leadJet_ptr->QGL() : -9999.;}
        const float subLeading_QGL() const { return  hasValidVBFDiJet() ? vbfDiPhoDiJet_mva_result_.vbfMvaResult.subleadJet_ptr->QGL() : -9999.;}
        const float leading_rawPt   () const { return  hasValidVBFDiJet() ? vbfDiPhoDiJet_mva_result_.vbfMvaResult.leadJet_ptr->correctedJet("Uncorrected").pt() : -9999.;}
        const float subLeading_rawPt() const { return  hasValidVBFDiJet() ? vbfDiPhoDiJet_mva_result_.vbfMvaResult.subleadJet_ptr->correctedJet("Uncorrected").pt() : -9999.;}
        
        const float leading_HFHadronEnergyFraction() const { return  hasValidVBFDiJet() ? vbfDiPhoDiJet_mva_result_.vbfMvaResult.leadJet_ptr->HFHadronEnergyFraction() : -9999.;}
        const float leading_HFHadronEnergy() const { return  hasValidVBFDiJet() ? vbfDiPhoDiJet_mva_result_.vbfMvaResult.leadJet_ptr->HFHadronEnergy() : -9999.;  }
        const float leading_HFHadronMultiplicity() const { return  hasValidVBFDiJet() ? vbfDiPhoDiJet_mva_result_.vbfMvaResult.leadJet_ptr->HFHadronEnergy() : -9999.;  }
        const float leading_HFEMEnergyFraction() const { return  hasValidVBFDiJet() ? vbfDiPhoDiJet_mva_result_.vbfMvaResult.leadJet_ptr->HFEMEnergyFraction() : -9999.;}
        const float leading_HFEMEnergy() const { return  hasValidVBFDiJet() ? vbfDiPhoDiJet_mva_result_.vbfMvaResult.leadJet_ptr->HFEMEnergy() : -9999.;  }
        const float leading_HFEMMultiplicity() const { return  hasValidVBFDiJet() ? vbfDiPhoDiJet_mva_result_.vbfMvaResult.leadJet_ptr->HFEMEnergy() : -9999.;  }
        const float subleading_HFHadronEnergyFraction() const { return  hasValidVBFDiJet() ? vbfDiPhoDiJet_mva_result_.vbfMvaResult.subleadJet_ptr->HFHadronEnergyFraction() : -9999.;}
        const float subleading_HFHadronEnergy() const { return  hasValidVBFDiJet() ? vbfDiPhoDiJet_mva_result_.vbfMvaResult.subleadJet_ptr->HFHadronEnergy() : -9999.;  }
        const float subleading_HFHadronMultiplicity() const { return  hasValidVBFDiJet() ? vbfDiPhoDiJet_mva_result_.vbfMvaResult.subleadJet_ptr->HFHadronEnergy() : -9999.;  }
        const float subleading_HFEMEnergyFraction() const { return  hasValidVBFDiJet() ? vbfDiPhoDiJet_mva_result_.vbfMvaResult.subleadJet_ptr->HFEMEnergyFraction() : -9999.;}
        const float subleading_HFEMEnergy() const { return  hasValidVBFDiJet() ? vbfDiPhoDiJet_mva_result_.vbfMvaResult.subleadJet_ptr->HFEMEnergy() : -9999.;  }
        const float subleading_HFEMMultiplicity() const { return  hasValidVBFDiJet() ? vbfDiPhoDiJet_mva_result_.vbfMvaResult.subleadJet_ptr->HFEMEnergy() : -9999.;  }

        const bool hasValidVBFDiJet() const {
            return (vbfDiPhoDiJet_mva_result_.vbfMvaResult.leadJet_ptr.isNonnull() && vbfDiPhoDiJet_mva_result_.vbfMvaResult.subleadJet_ptr.isNonnull()); 
        };
        const bool hasValidVBFTriJet() const; 
    private:
        VBFDiPhoDiJetMVAResult vbfDiPhoDiJet_mva_result_;
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

