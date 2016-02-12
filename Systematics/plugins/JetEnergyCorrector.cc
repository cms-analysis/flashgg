#include "flashgg/Systematics/interface/ObjectSystMethodBinnedByFunctor.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"

namespace flashgg {

    class JetEnergyCorrector: public BaseSystMethod<flashgg::Jet, int>
    {

    public:
        typedef StringCutObjectSelector<Jet, true> selector_type;

        JetEnergyCorrector( const edm::ParameterSet &conf );
        void applyCorrection( flashgg::Jet &y, int syst_shift ) override;
        std::string shiftLabel( int ) const override;
        void setJECUncertainty ( const JetCorrectorParameters &) override;
        void setJEC( const JetCorrector*, const edm::Event &, const edm::EventSetup &  ) override;

    private:
        selector_type overall_range_;
        unique_ptr<JetCorrectionUncertainty> jec_unc_;
        const JetCorrector* jec_cor_;
        const edm::Event* evt_;
        const edm::EventSetup* evt_setup_;
        bool jec_set_;
        bool debug_;
    };

    JetEnergyCorrector::JetEnergyCorrector( const edm::ParameterSet &conf ) :
        BaseSystMethod( conf ),
        overall_range_( conf.getParameter<std::string>( "OverallRange" ) ),
        debug_( conf.getUntrackedParameter<bool>( "Debug", false ) )
    {
        jec_set_ = false;
    }

    void JetEnergyCorrector::setJEC( const JetCorrector* theJEC, const edm::Event &iEvent, const edm::EventSetup & iSetup ) {
        jec_set_ = true;
        jec_cor_ = theJEC;
        evt_ = &iEvent;
        evt_setup_ = &iSetup;
    }

    void JetEnergyCorrector::setJECUncertainty( const JetCorrectorParameters & JetCorPar ) {
        jec_unc_.reset( new JetCorrectionUncertainty(JetCorPar) );
    }

    std::string JetEnergyCorrector::shiftLabel( int syst_value ) const
    {
        std::string result;
        if( syst_value == 0 ) {
            result = Form( "%sCentral", label().c_str() );
        } else if( syst_value > 0 ) {
            result = Form( "%sUp%.2dsigma", label().c_str(), syst_value );
        } else {
            result = Form( "%sDown%.2dsigma", label().c_str(), -1 * syst_value );
        }
        return result;
    }

    void JetEnergyCorrector::applyCorrection( flashgg::Jet &y, int syst_shift )
    {
        if( overall_range_( y ) ) {
            double jec_adjust = 1.;
            if (jec_set_ && applyCentralValue()) {
                double jec = jec_cor_->correction( y.correctedJet("Uncorrected") , *evt_, *evt_setup_ );
                double oldjec = (y.energy()/y.correctedJet("Uncorrected").energy());
                if ( debug_ ) {
                    std::cout << " DOING JEC! We get this jec from the corrector: " << jec << std::endl;
                    std::cout << "    ... previous jec was: " << oldjec << std::endl;
                }
                jec_adjust = jec/oldjec;
            }
            jec_unc_->setJetEta(y.eta());
            jec_unc_->setJetPt(jec_adjust*y.pt()); 
            float unc = jec_unc_->getUncertainty(true);
            float scale = jec_adjust + syst_shift*unc;
            if( debug_ ) {
                std::cout << "  " << shiftLabel( syst_shift ) << ": Jet has pt= " << y.pt() << " eta=" << y.eta()
                          << " and we apply a multiplicative correction of " << scale << std::endl;
            }
            y.setP4( scale * y.p4() );
        }
    }
}

DEFINE_EDM_PLUGIN( FlashggSystematicJetMethodsFactory,
                   flashgg::JetEnergyCorrector,
                   "FlashggJetEnergyCorrector" );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

