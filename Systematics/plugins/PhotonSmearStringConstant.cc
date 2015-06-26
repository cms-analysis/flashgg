// One-parameter smear, constant term only
// For 2 parameters, see PhotonSmearStringStoachastic.cc

#include "flashgg/Systematics/interface/ObjectSystMethodBinnedByFunctor.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
#include "CLHEP/Random/RandGauss.h"

namespace flashgg {

    class PhotonSmearStringConstant: public ObjectSystMethodBinnedByFunctor<flashgg::Photon, int>
    {

    public:
        typedef StringCutObjectSelector<Photon, true> selector_type;

        PhotonSmearStringConstant( const edm::ParameterSet &conf );
        void applyCorrection( flashgg::Photon &y, int syst_shift ) override;
        std::string shiftLabel( int ) const override;

    private:
        selector_type overall_range_;
    };

    PhotonSmearStringConstant::PhotonSmearStringConstant( const edm::ParameterSet &conf ) :
        ObjectSystMethodBinnedByFunctor( conf ),
        overall_range_( conf.getParameter<std::string>( "OverallRange" ) )
    {
    }

    std::string PhotonSmearStringConstant::shiftLabel( int syst_value ) const
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

    void PhotonSmearStringConstant::applyCorrection( flashgg::Photon &y, int syst_shift )
    {
        if( overall_range_( y ) ) {
            auto val_err = binContents( y );
            if( val_err.first.size() == 1 && val_err.second.size() == 1 ) { // otherwise no-op because we don't have an entry
                float sigma_smearing = val_err.first[0];
                float sigma_smearing_err = val_err.second[0];
                float sigma = sigma_smearing + syst_shift * sigma_smearing_err;
                float newe = CLHEP::RandGauss::shoot( RandomEngine(), y.energy(), sigma * y.energy() );
                if( debug_ ) {
                    std::cout << "  " << shiftLabel( syst_shift ) << ": Photon has energy= " << y.energy() << " eta=" << y.eta()
                              << " and we apply a smearing with sigma " << ( 100 * sigma ) << "% to get new energy=" << newe << std::endl;
                }
                y.updateEnergy( shiftLabel( syst_shift ), newe );
            }
        }
    }
}

DEFINE_EDM_PLUGIN( FlashggSystematicPhotonMethodsFactory,
                   flashgg::PhotonSmearStringConstant,
                   "FlashggPhotonSmearStringConstant" );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

