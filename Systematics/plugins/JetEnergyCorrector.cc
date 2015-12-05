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
        void setEnergyCorrections ( const JetCorrectorParameters & ) override;

    private:
        selector_type overall_range_;
        unique_ptr<JetCorrectionUncertainty> jec_unc_;
        bool debug_;
    };

    JetEnergyCorrector::JetEnergyCorrector( const edm::ParameterSet &conf ) :
        BaseSystMethod( conf ),
        overall_range_( conf.getParameter<std::string>( "OverallRange" ) ),
        debug_( conf.getUntrackedParameter<bool>( "Debug", false ) )
    {
    }

    void JetEnergyCorrector::setEnergyCorrections( const JetCorrectorParameters & JetCorPar ) {
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
            jec_unc_->setJetEta(y.eta());
            jec_unc_->setJetPt(y.pt()); // TODO: confirm this is the corrected pt
            float unc = jec_unc_->getUncertainty(true);
            float scale = 1 + syst_shift*unc;
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

