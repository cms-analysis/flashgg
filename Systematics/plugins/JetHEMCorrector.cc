#include "flashgg/Systematics/interface/ObjectSystMethodBinnedByFunctor.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
#include "FWCore/Framework/interface/ESHandle.h"


namespace flashgg {

    class JetHEMCorrector: public BaseSystMethod<flashgg::Jet, int>
    {

    public:
        typedef StringCutObjectSelector<Jet, true> selector_type;

        JetHEMCorrector( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer * gv );
        void applyCorrection( flashgg::Jet &y, int syst_shift ) override;
        std::string shiftLabel( int ) const override;
        void eventInitialize( const edm::Event &iEvent, const edm::EventSetup & iSetup ) override;

    private:
        selector_type overall_range_;
        bool debug_;
    };


    JetHEMCorrector::JetHEMCorrector( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer * gv ) :
        BaseSystMethod( conf, std::forward<edm::ConsumesCollector>(iC)  ),
        overall_range_( conf.getParameter<std::string>( "OverallRange" ) ),
        debug_( conf.getUntrackedParameter<bool>( "Debug", false ) )
    {
    }

    void JetHEMCorrector::eventInitialize( const edm::Event &iEvent, const edm::EventSetup & iSetup ) {
    }

    std::string JetHEMCorrector::shiftLabel( int syst_value ) const
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

    void JetHEMCorrector::applyCorrection( flashgg::Jet &y, int syst_shift )
    {
        if( overall_range_( y ) ) {
            float unc = 0.;
            if( y.phi() > -1.57 && y.phi() < -0.87 ) {
                if( y.eta() > -2.5 && y.eta() < -1.3 ) {
                    unc = 0.2;
                }
                else if( y.eta() > -3.0 && y.eta() < -2.5 ) {
                    unc = 0.35;
                }
            }
            float scale = 1. + syst_shift*unc;
            if( debug_ ) {
                std::cout << "  " << shiftLabel( syst_shift ) << ": Jet has pt= " << y.pt() << " eta=" << y.eta()
                          << " and we apply a multiplicative correction of " << scale << std::endl;
            }
            y.setP4( scale * y.p4() );
        }
    }
}

DEFINE_EDM_PLUGIN( FlashggSystematicJetMethodsFactory,
                   flashgg::JetHEMCorrector,
                   "FlashggJetHEMCorrector" );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

