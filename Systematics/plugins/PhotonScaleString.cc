#include "flashgg/Systematics/interface/BaseSystMethods.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"


namespace flashgg {

    struct ScaleBin {

        StringCutObjectSelector<Photon, true> selector;
        double shift;
        double shift_err;

        ScaleBin( std::string s, double val, double err ) :
            selector( s ),
            shift( val ),
            shift_err( err ) {}
    };

    class PhotonScaleString: public BaseSystMethods<flashgg::Photon, int>
    {

    public:
        PhotonScaleString( const edm::ParameterSet &conf );

        void applyCorrection( flashgg::Photon &y, int syst_shift ) override;
        std::string shiftLabel( int ) override;

    private:
        std::vector<ScaleBin> bins_;
        StringCutObjectSelector<Photon, true> overall_range_;
        bool debug_;
    };

    PhotonScaleString::PhotonScaleString( const edm::ParameterSet &conf ) :
        BaseSystMethods( conf ),
        overall_range_( conf.getParameter<std::string>( "OverallRange" ) ),
        debug_( conf.getUntrackedParameter<bool>( "Debug", false ) )
    {
        const auto &vpset = conf.getParameterSetVector( "Bins" );
        for( const auto &pset : vpset ) {
            std::string range = pset.getParameter<std::string>( "Range" );
            double shift = pset.getParameter<double>( "Shift" );
            double uncertainty = pset.getParameter<double>( "Uncertainty" );
            bins_.emplace_back( range, shift, uncertainty );
        }
    }

    std::string PhotonScaleString::shiftLabel( int syst_value )
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

    void PhotonScaleString::applyCorrection( flashgg::Photon &y, int syst_shift )
    {
        if( overall_range_( y ) ) {
            for( auto bin = bins_.begin() ; bin != bins_.end() ; bin++ ) {
                if( ( bin->selector )( y ) ) {
                    float scale = 1 + bin->shift + syst_shift * bin->shift_err;
                    if( debug_ ) {
                        std::cout << "  " << shiftLabel( syst_shift ) << ": Photon has pt= " << y.pt() << " eta=" << y.eta()
                                  << " and we apply a multiplicative correction of " << scale << std::endl;
                    }
                    y.updateEnergy( shiftLabel( syst_shift ), scale * y.energy() );
                    break;
                }
            }
        }
    }
}

DEFINE_EDM_PLUGIN( FlashggSystematicPhotonMethodsFactory,
                   flashgg::PhotonScaleString,
                   "FlashggPhotonScaleString" );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

