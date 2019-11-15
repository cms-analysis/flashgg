#include "flashgg/Systematics/interface/BaseSystMethod.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
#include "CLHEP/Random/RandGauss.h"
#include "JetMETCorrections/Modules/interface/JetResolution.h"
#include "DataFormats/Math/interface/deltaR.h"

namespace flashgg {

    class JetBregSmear: public BaseSystMethod<flashgg::Jet, int>
    {

    public:
        typedef StringCutObjectSelector<Jet, true> selector_type;

        JetBregSmear( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer * gv );
        void applyCorrection( flashgg::Jet &y, int syst_shift ) override;
        std::string shiftLabel( int ) const override;
        void eventInitialize( const edm::Event &iEvent, const edm::EventSetup & iSetup ) override;

    private:
        selector_type overall_range_;
        bool debug_;
        vector<double>BRegressionSFunc_;
    };

    void JetBregSmear::eventInitialize( const edm::Event &iEvent, const edm::EventSetup & iSetup ) {
    }

    JetBregSmear::JetBregSmear( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer * gv ) :
        BaseSystMethod( conf, std::forward<edm::ConsumesCollector>(iC) ), 
        overall_range_( conf.getParameter<std::string>( "OverallRange" ) ),
        debug_( conf.getUntrackedParameter<bool>("Debug", false) ),
        BRegressionSFunc_( conf.getParameter<vector<double > >("BRegressionSFunc") )
    {
        if (!applyCentralValue()) throw cms::Exception("Configuration") << " module not set up to have central value turned off";
    }

    std::string JetBregSmear::shiftLabel( int syst_value ) const
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

    void JetBregSmear::applyCorrection( flashgg::Jet &y, int syst_shift )
    {
        if( overall_range_( y ) ) {
            if ( debug_ ) {
                std::cout << "  " << shiftLabel( syst_shift ) << ": Jet has pt=" << y.pt() << " eta=" << y.eta() << std::endl;
            }
            float scale_factor = 1.;
            if (syst_shift == 0) {
                scale_factor = BRegressionSFunc_[0];
            } else if (syst_shift == 1) {
                scale_factor = BRegressionSFunc_[0]+BRegressionSFunc_[1];
            } else if (syst_shift == -1) {
                scale_factor = BRegressionSFunc_[0]+BRegressionSFunc_[2];
            }
            if ( debug_ ) {
                std::cout << " Using JER SF derived for b-regression = "<<scale_factor << std::endl;
            }
            y.setP4(scale_factor*y.p4());
        }
    }
}

DEFINE_EDM_PLUGIN( FlashggSystematicJetMethodsFactory,
                   flashgg::JetBregSmear,
                   "FlashggJetBregSmear" );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

