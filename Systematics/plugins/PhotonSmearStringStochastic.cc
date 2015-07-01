// General 2 nusianceparameter stochastic oplus constant smearing
// If you want constant smearing only, with only 1 nusiance parameter, use PhotonSmearStringConstant.cc

#include "flashgg/Systematics/interface/ObjectSystMethodBinnedByFunctor.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
#include "CLHEP/Random/RandGauss.h"

namespace flashgg {

    class PhotonSmearStringStochastic: public ObjectSystMethodBinnedByFunctor<flashgg::Photon, std::pair<int, int> >
    {

    public:
        typedef StringCutObjectSelector<Photon, true> selector_type;

        PhotonSmearStringStochastic( const edm::ParameterSet &conf );
        void applyCorrection( flashgg::Photon &y, std::pair<int, int> syst_shift ) override;
        std::string shiftLabel( std::pair<int, int> ) const override;

        const std::string &firstParameterName() const { return label1_; }
        const std::string &secondParameterName() const { return label2_; }

    private:
        selector_type overall_range_;
        const std::string label1_;
        const std::string label2_;
        bool exaggerateShiftUp_; // debugging
    };

    PhotonSmearStringStochastic::PhotonSmearStringStochastic( const edm::ParameterSet &conf ) :
        ObjectSystMethodBinnedByFunctor( conf ),
        overall_range_( conf.getParameter<std::string>( "OverallRange" ) ),
        label1_( conf.getUntrackedParameter<std::string>( "FirstParameterName", "Rho" ) ),
        label2_( conf.getUntrackedParameter<std::string>( "SecondParameterName", "Phi" ) ),
        exaggerateShiftUp_( conf.getUntrackedParameter<bool>( "ExaggerateShiftUp", false ) )
    {
    }

    std::string PhotonSmearStringStochastic::shiftLabel( std::pair<int, int> syst_value ) const
    {
        std::string result = label();
        if( syst_value.first == 0 && syst_value.second == 0 ) {
            result += "Central";
        } else {
            if( syst_value.first > 0 ) { result += Form( "%sUp%.2dsigma", firstParameterName().c_str(), syst_value.first ); }
            if( syst_value.first < 0 ) { result += Form( "%sDown%.2dsigma", firstParameterName().c_str(), -1 * syst_value.first ); }
            if( syst_value.second > 0 ) { result += Form( "%sUp%.2dsigma", secondParameterName().c_str(), syst_value.second ); }
            if( syst_value.second < 0 ) { result += Form( "%sDown%.2dsigma", secondParameterName().c_str(), -1 * syst_value.second ); }
        }
        return result;
    }

    void PhotonSmearStringStochastic::applyCorrection( flashgg::Photon &y, std::pair<int, int> syst_shift )
    {
        if( overall_range_( y ) ) {
            auto val_err = binContents( y );
            float sigma = 0.;

            // Nothing will happen, with no warning, if the bin count doesn't match expected options
            // TODO for production: make this behavior more robust

            // 3 values (rho, phi, ETmean) and 2 errors (rho, phi) are required

            if( val_err.first.size() == 3 && val_err.second.size() == 2 ) {
                // 3 values (rho, phi, ETmean) and 2 errors (rho, phi) are required
                float rho_val = val_err.first[0];
                float rho_err = val_err.second[0];
                if( exaggerateShiftUp_ && syst_shift.first == 1 ) { rho_err  = 9 * rho_val; }
                float phi_val = val_err.first[1];
                float phi_err = val_err.second[1];
                float ETmean = val_err.first[2];

                // Not entirely yet sure about either the output units or their consistency
                float rho = rho_val + syst_shift.first * rho_err;
                float phi = phi_val + syst_shift.second * phi_err;
                float sigma_C = rho * sin( phi );
                float sigma_S = rho * cos( phi ) * sqrt( ETmean / y.et() );
                sigma = std::sqrt( sigma_C * sigma_C + sigma_S * sigma_S );
            }
            if( val_err.first.size() == 1 && val_err.second.size() == 1 ) {
                // Constant only, correlate systematics with rho and ignore second nusiance parameter
                // This should only be used if some bins are constant and others are stochastic
                // Otherwise we'll have an extra needless parameter and extra needless collections

                float rho_val = val_err.first[0];
                float rho_err = val_err.second[0];
                if( exaggerateShiftUp_ && syst_shift.first == 1 ) { rho_err = 9 * rho_val; }
                sigma = rho_val + syst_shift.first * rho_err;
            }

            float newe = CLHEP::RandGauss::shoot( RandomEngine(), y.energy(), sigma * y.energy() );
            if( debug_ ) {
                std::cout << "  " << shiftLabel( syst_shift ) << ": Photon has energy= " << y.energy() << " eta=" << y.eta()
                          << " and we apply a smearing with sigma " << ( 100 * sigma ) << "% to get new energy=" << newe << std::endl;
            }
            y.updateEnergy( shiftLabel( syst_shift ), newe );
        }
    }
}

DEFINE_EDM_PLUGIN( FlashggSystematicPhotonMethodsFactory2D,
                   flashgg::PhotonSmearStringStochastic,
                   "FlashggPhotonSmearStringStochastic" );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

