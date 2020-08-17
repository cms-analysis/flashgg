// One-parameter smear, constant term only
// For 2 parameters, see PhotonSmearStringStoachastic.cc

#include "flashgg/Systematics/interface/ObjectSystMethodBinnedByFunctor.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

namespace flashgg {

    class PhotonSmearConstant: public ObjectSystMethodBinnedByFunctor<flashgg::Photon, int>
    {

    public:
        typedef StringCutObjectSelector<Photon, true> selector_type;

        PhotonSmearConstant( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer *gv );
        void applyCorrection( flashgg::Photon &y, int syst_shift ) override;
        std::string shiftLabel( int ) const override;

    private:
        selector_type overall_range_;
        std::string random_label_;
        bool exaggerateShiftUp_; // for sanity checks only
    };

    PhotonSmearConstant::PhotonSmearConstant( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer *gv ) :
        ObjectSystMethodBinnedByFunctor( conf, std::forward<edm::ConsumesCollector>(iC), gv ),
        overall_range_( conf.getParameter<std::string>( "OverallRange" ) ),
        random_label_(conf.getParameter<std::string>("RandomLabel")),
        exaggerateShiftUp_( conf.getParameter<bool>( "ExaggerateShiftUp" ) )

    {
        if (!applyCentralValue()) throw cms::Exception("SmearingLogic") << "If we do not apply central smearing we cannot scale down the smearing";
    }

    std::string PhotonSmearConstant::shiftLabel( int syst_value ) const
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

    void PhotonSmearConstant::applyCorrection( flashgg::Photon &y, int syst_shift )
    {
        if( overall_range_( y ) ) {
            auto val_err = binContents( y );
            if( val_err.first.size() == 1 && val_err.second.size() == 1 ) { // otherwise no-op because we don't have an entry
                float sigma_smearing = val_err.first[0];
                float sigma_smearing_err = val_err.second[0];
                if( exaggerateShiftUp_ && syst_shift == 1 ) { sigma_smearing_err  = 9 * sigma_smearing; }
                float sigma = sigma_smearing + syst_shift * sigma_smearing_err;
                if (!y.hasUserFloat(random_label_)) {
                    throw cms::Exception("Missing embedded random number") << "Could not find key " << random_label_ << " for random numbers embedded in the photon object, please make sure to read the appropriate version of MicroAOD and/or access the correct label and/or run the PerPhotonDiPhoton randomizer on-the-fly";
                }
                float rnd = y.userFloat(random_label_);
                float newe = y.energy() * (1. + rnd * sigma);
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
                   flashgg::PhotonSmearConstant,
                   "FlashggPhotonSmearConstant" );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

