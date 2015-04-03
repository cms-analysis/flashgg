#include "flashgg/Systematics/interface/BaseSystMethods.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "flashgg/DataFormats/interface/Photon.h"

namespace flashgg {

    class SimplePhotonSmear: public BaseSystMethods<flashgg::Photon, int>
    {


    public:
        SimplePhotonSmear( const edm::ParameterSet &conf );

        void applyCorrection( flashgg::Photon &y, int syst_shift ) override;
        std::string shiftLabel( int ) override;

    private:
        double sigma_;
    };

    SimplePhotonSmear::SimplePhotonSmear( const edm::ParameterSet &conf ) :
        BaseSystMethods( conf ),
        sigma_( conf.getParameter<double>( "Sigma" ) ) {}

    std::string SimplePhotonSmear::shiftLabel( int syst_value )
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

    void SimplePhotonSmear::applyCorrection( flashgg::Photon &y, int syst_shift )
    {
        if( syst_shift == 0 ) {
            // std::cout << "Nominal correction" << std::endl;
        } else {
            y.updateEnergy( shiftLabel( syst_shift ), y.energy() + sigma_ * syst_shift );
        }
    }
}

DEFINE_EDM_PLUGIN( FlashggSystematicPhotonMethodsFactory,
                   flashgg::SimplePhotonSmear,
                   "FlashggSimplePhotonSmear" );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

