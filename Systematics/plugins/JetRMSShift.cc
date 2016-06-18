#include "flashgg/Systematics/interface/ObjectSystMethodBinnedByFunctor.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

#include "JetMETCorrections/JetCorrector/interface/JetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"


namespace flashgg {

    class JetRMSShift: public ObjectSystMethodBinnedByFunctor<flashgg::Jet, int>
    {

    public:
        typedef StringCutObjectSelector<Jet, true> selector_type;

        JetRMSShift( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer * gv );
        void applyCorrection( flashgg::Jet &y, int syst_shift ) override;
        std::string shiftLabel( int ) const override;

    private:
        selector_type overall_range_;
    };


    JetRMSShift::JetRMSShift( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer * gv ) :
        ObjectSystMethodBinnedByFunctor( conf, std::forward<edm::ConsumesCollector>(iC), gv  ),
        overall_range_( conf.getParameter<std::string>( "OverallRange" ) )
    {
    }

    std::string JetRMSShift::shiftLabel( int syst_value ) const
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

    void JetRMSShift::applyCorrection( flashgg::Jet &y, int syst_shift )
    {
        if( overall_range_( y ) ) {
            auto val_err = binContents( y );
            if( val_err.first.size() == 1 && val_err.second.size() == 1 ) { // otherwise no-op because we don't have an entry
                float shift_val = val_err.first[0];  // e.g. 0 if no central value change
                if (!applyCentralValue()) shift_val = 0.;
                float shift_err = val_err.second[0]; // e.g. 0.002
                float shift = shift_val + syst_shift * shift_err;
                float theRms = y.rms();
                y.setSimpleRMS( theRms + shift );
                if ( this->debug_ ) {
                    std::cout << "JetRMSShift::applyCorrection syst_shift=" << syst_shift << " old rms=" << theRms << " new rms=" << y.rms() << std::endl;
                }
            }
        }
    }
}

DEFINE_EDM_PLUGIN( FlashggSystematicJetMethodsFactory,
                   flashgg::JetRMSShift,
                   "FlashggJetRMSShift" );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

