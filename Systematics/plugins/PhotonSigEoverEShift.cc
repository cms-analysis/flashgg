#include "flashgg/Systematics/interface/ObjectSystMethodBinnedByFunctor.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

namespace flashgg {

    class PhotonSigEOverEShift: public ObjectSystMethodBinnedByFunctor<flashgg::Photon, int>
    {

    public:
        typedef StringCutObjectSelector<Photon, true> selector_type;

        PhotonSigEOverEShift( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer *gv );
        void applyCorrection( flashgg::Photon &y, int syst_shift ) override;
        std::string shiftLabel( int ) const override;

    private:
        selector_type overall_range_;
    };

    PhotonSigEOverEShift::PhotonSigEOverEShift( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer *gv ) :
        ObjectSystMethodBinnedByFunctor( conf, std::forward<edm::ConsumesCollector>(iC), gv ),
        overall_range_( conf.getParameter<std::string>( "OverallRange" ) )
    {
    }

    std::string PhotonSigEOverEShift::shiftLabel( int syst_value ) const
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

    void PhotonSigEOverEShift::applyCorrection( flashgg::Photon &y, int syst_shift )
    {
        if( overall_range_( y ) ) {
            auto val_err = binContents( y );
            if( val_err.first.size() == 1 && val_err.second.size() == 1 ) { // otherwise no-op because we don't have an entry
                float shift_val = val_err.first[0];  // e.g. 0 if no central value change
                if (!applyCentralValue()) shift_val = 0.;
                float shift_err = val_err.second[0]; // e.g. 0.1
                float shift = shift_val + syst_shift * shift_err;
                //              std::vector<edm::Ptr<reco::Vertex> > keys;
                if( debug_ ) {
                    std::cout << "  " << shiftLabel( syst_shift ) << ": Photon has pt= " << y.pt() << " eta=" << y.eta()
                              << " and we apply a sigmaEoverE shift of " << shift << "%" << std::endl;
                    std::cout << "     sigE/E VALUE BEFORE: ";
                    auto beforeSigEoE = y.sigEOverE();
                    std::cout << beforeSigEoE << " ";
                    std::cout << std::endl;
                }
                y.shiftSigmaEOverEValueBy( shift, 0.00 ); 
                // the others are no longer used at this stage anyway, so it cannot hurt
                if ( debug_) {
                    auto afterSigEoE = y.sigEOverE();
                    std::cout << "     sigE/E VALUE AFTER: ";
                    std::cout << afterSigEoE << " ";
                    std::cout << std::endl;
                }
            }
        }
    }
    
}
    
DEFINE_EDM_PLUGIN( FlashggSystematicPhotonMethodsFactory,
                   flashgg::PhotonSigEOverEShift,
                   "FlashggPhotonSigEOverEShift" );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

