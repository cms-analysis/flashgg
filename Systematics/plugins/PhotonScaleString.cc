#include "flashgg/Systematics/interface/BaseSystMethods.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"


namespace flashgg {

    class PhotonScaleString: public BaseSystMethods<flashgg::Photon, int>
    {

    public:
        PhotonScaleString( const edm::ParameterSet &conf );

        void applyCorrection( flashgg::Photon &y, int syst_shift ) override;
        std::string shiftLabel( int ) override;

    private:
        std::vector<std::pair<StringCutObjectSelector<Photon, true>, double> > bins_;
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
            double scale = pset.getParameter<double>( "Scale" );
            bins_.emplace_back( StringCutObjectSelector<Photon, true>( range ), scale );
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
        if( syst_shift == 0 || !overall_range_( y ) ) {
            // Nominal correction or this corrector does not apply to this object
        } else {
            for( auto binit = bins_.begin() ; binit != bins_.end() ; binit++ ) {
                if( ( binit->first )( y ) ) {
                    if( debug_ ) {
                        std::cout << "  " << label() << ": Photon has pt= " << y.pt() << " eta=" << y.eta()
                                  << " and we a apply a " << syst_shift << " sigma shift with sigma=" << binit->second << std::endl;
                    }
                    y.updateEnergy( shiftLabel( syst_shift ), y.energy() + binit->second * syst_shift );
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

