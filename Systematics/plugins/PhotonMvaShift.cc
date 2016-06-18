#include "flashgg/Systematics/interface/ObjectSystMethodBinnedByFunctor.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

namespace flashgg {

    class PhotonMvaShift: public ObjectSystMethodBinnedByFunctor<flashgg::Photon, int>
    {

    public:
        typedef StringCutObjectSelector<Photon, true> selector_type;

        PhotonMvaShift( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer *gv );
        void applyCorrection( flashgg::Photon &y, int syst_shift ) override;
        std::string shiftLabel( int ) const override;

    private:
        selector_type overall_range_;
    };

    PhotonMvaShift::PhotonMvaShift( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer *gv ) :
        ObjectSystMethodBinnedByFunctor( conf, std::forward<edm::ConsumesCollector>(iC), gv ),
        overall_range_( conf.getParameter<std::string>( "OverallRange" ) )
    {
    }

    std::string PhotonMvaShift::shiftLabel( int syst_value ) const
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

    void PhotonMvaShift::applyCorrection( flashgg::Photon &y, int syst_shift )
    {
        if( overall_range_( y ) ) {
            auto val_err = binContents( y );
            if( val_err.first.size() == 1 && val_err.second.size() == 1 ) { // otherwise no-op because we don't have an entry
                float shift_val = val_err.first[0];  // e.g. 0 if no central value change
                if (!applyCentralValue()) shift_val = 0.;
                float shift_err = val_err.second[0]; // e.g. 0.1
                float shift = shift_val + syst_shift * shift_err;
                std::vector<edm::Ptr<reco::Vertex> > keys;
                if( debug_ ) {
                    std::cout << "  " << shiftLabel( syst_shift ) << ": Photon has pt= " << y.pt() << " eta=" << y.eta()
                              << " and we apply an mva shift of " << shift << std::endl;
                    std::cout << "     MVA VALUES BEFORE: ";
                    auto beforeMap = y.phoIdMvaD();
                    for(auto keyval = beforeMap.begin(); keyval != beforeMap.end(); ++keyval) {
                        keys.push_back( keyval->first );
                        std::cout << keyval->second << " ";
                    }
                    std::cout << std::endl;
                }
                y.shiftAllMvaValuesBy( shift ); // we shift all because we don't have access to the selected vertex
                                                // the others are no longer used at this stage anyway, so it cannot hurt
                if ( debug_) {
                    auto afterMap = y.phoIdMvaD();
                    std::cout << "     MVA VALUES AFTER: ";
                    for(auto key = keys.begin() ; key != keys.end() ; ++key ) {
                        std::cout << afterMap.at(*key) << " ";
                    }
                    std::cout << std::endl;
                }
            }
        }
    }
}

DEFINE_EDM_PLUGIN( FlashggSystematicPhotonMethodsFactory,
                   flashgg::PhotonMvaShift,
                   "FlashggPhotonMvaShift" );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

