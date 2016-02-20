#include "flashgg/DataFormats/interface/Photon.h"
#include "flashgg/Systematics/interface/ObjectSystMethodBinnedByFunctor.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "EgammaAnalysis/ElectronTools/interface/EnergyScaleCorrection_class.hh"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

namespace edm {
    class Event;
}

namespace flashgg {

    class PhotonScale: public ObjectSystMethodBinnedByFunctor<flashgg::Photon, int>
    {

    public:
        typedef StringCutObjectSelector<Photon, true> selector_type;

        PhotonScale( const edm::ParameterSet &conf );
        void applyCorrection( flashgg::Photon &y, int syst_shift, const edm::Event & ev ) override;
        std::string shiftLabel( int ) const override;

    private:
        selector_type overall_range_;
        std::string correctionFile_;
        EnergyScaleCorrection_class scaler_;
    };

    PhotonScale::PhotonScale( const edm::ParameterSet &conf ) :
        ObjectSystMethodBinnedByFunctor( conf ),
        overall_range_( conf.getParameter<std::string>( "OverallRange" ) ),
        correctionFile_( conf.getParameter<std::string>( "CorrectionFile" )),
        scaler_(correctionFile_)
    {
    }

    std::string PhotonScale::shiftLabel( int syst_value ) const
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

    void PhotonScale::applyCorrection( flashgg::Photon &y, int syst_shift, const edm::Event & ev )
    {
        if( overall_range_( y ) ) {
            auto shift_val = scaler_.ScaleCorrection(ev.run(), y.isEB(), y.full5x5_r9(), y.superCluster()->eta(), y.et());
            auto shift_err = scaler_.ScaleCorrectionUncertainty(ev.run(), y.isEB(), y.full5x5_r9(), y.superCluster()->eta(), y.et());
            if (!applyCentralValue()) shift_val = 0.;
            float scale = 1. + shift_val + syst_shift * shift_err;
            if( debug_ ) {
                std::cout << "  " << shiftLabel( syst_shift ) << ": Photon has pt= " << y.pt() << " eta=" << y.eta()
                    << " and we apply a multiplicative correction of " << scale << std::endl;
            }
            //auto val_err = binContents( y );
            //y.updateEnergy( shiftLabel( syst_shift ), scale * y.energy() );
            //if( val_err.first.size() == 1 && val_err.second.size() == 1 ) { // otherwise no-op because we don't have an entry
            //    float shift_val = val_err.first[0];
            //    if (!applyCentralValue()) shift_val = 0.;
            //    float shift_err = val_err.second[0];
            //    float scale = 1 + shift_val + syst_shift * shift_err;
            //    if( debug_ ) {
            //        std::cout << "  " << shiftLabel( syst_shift ) << ": Photon has pt= " << y.pt() << " eta=" << y.eta()
            //                  << " and we apply a multiplicative correction of " << scale << std::endl;
            //    }
            //    y.updateEnergy( shiftLabel( syst_shift ), scale * y.energy() );
            //}
        }
    }
}

DEFINE_EDM_PLUGIN( FlashggSystematicPhotonMethodsFactory,
                   flashgg::PhotonScale,
                   "FlashggPhotonScale" );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

