#include "flashgg/Systematics/interface/ObjectSystMethodBinnedByFunctor.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
#include "CLHEP/Random/RandGauss.h"

namespace flashgg {

    class JetSmearConstant: public ObjectSystMethodBinnedByFunctor<flashgg::Jet, int>
    {

    public:
        typedef StringCutObjectSelector<Jet, true> selector_type;

        JetSmearConstant( const edm::ParameterSet &conf );
        void applyCorrection( flashgg::Jet &y, int syst_shift ) override;
        std::string shiftLabel( int ) const override;
        void setJECUncertainty ( const JetCorrectorParameters & ) override {}; // NotImplemented in base class --> harmless noop
        void setJEC( const JetCorrector*, const edm::Event &, const edm::EventSetup & ) override {}; // NotImplemented in base class --> harmless noop

    private:
        selector_type overall_range_;
        std::string random_label_;
    };

    JetSmearConstant::JetSmearConstant( const edm::ParameterSet &conf ) :
        ObjectSystMethodBinnedByFunctor( conf ), 
        overall_range_( conf.getParameter<std::string>( "OverallRange" ) ),
        random_label_(conf.getParameter<std::string>("RandomLabel"))
        //        exaggerateShiftUp_( conf.getUntrackedParameter<bool>( "ExaggerateShiftUp", false ) )
    {
    }

    std::string JetSmearConstant::shiftLabel( int syst_value ) const
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

    void JetSmearConstant::applyCorrection( flashgg::Jet &y, int syst_shift )
    {
        if( overall_range_( y ) ) {
            auto val_err = binContents( y );
            if( val_err.first.size() == 1 && val_err.second.size() == 1 ) { // otherwise no-op because we don't have an entry
                float scale_factor_central = val_err.first[0];
                if(!applyCentralValue()) scale_factor_central = 1.;
                float scale_factor_err = val_err.second[0];
                float scale_factor = scale_factor_central + syst_shift * scale_factor_err;
                float recpt = y.pt();
                auto genjet = y.genJet();
                if (genjet != nullptr) {
                    float genpt = y.genJet()->pt();
                    //            https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetResolution#Smearing_procedures
                    float newpt = max<float>(0.,genpt + scale_factor*(recpt-genpt));
                    if ( debug_ ) {
                        std::cout << "  " << shiftLabel( syst_shift ) << ": Jet has pt=" << y.pt() << " eta=" << y.eta() << " genpt=" << genpt
                                  << " and we apply a scale_factor to (recpt-genpt) of " << scale_factor << " to get new pt=" << newpt << std::endl;
                    }
                    y.setP4((newpt/recpt)*y.p4());
                } else {
                    if (!y.hasUserFloat(random_label_)) {
                        throw cms::Exception("Missing embedded random number") << "Could not find key " << random_label_ << " for random numbers embedded in the jet object, please make sure to read the appropriate version of MicroAOD and/or access the correct label and/or run the randomizer on-the-fly";
                    }
                    float rnd = y.userFloat(random_label_);       
                    
                    if ( debug_ ) {
                        std::cout << " We do not use it yet, but we have a random number for " << random_label_ << ": " << rnd << std::endl;
                    }
                    if ( debug_ ) {
                        std::cout << "  " << shiftLabel( syst_shift ) << ": Jet has pt=" << y.pt() << " eta=" << y.eta() << " AND NO GENJET! "
                                  << " ... so we do nothing" << std::endl;
                    }
                }
            }
        }
    }
}

DEFINE_EDM_PLUGIN( FlashggSystematicJetMethodsFactory,
                   flashgg::JetSmearConstant,
                   "FlashggJetSmearConstant" );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

