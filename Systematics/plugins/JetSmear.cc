#include "flashgg/Systematics/interface/BaseSystMethod.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
#include "CLHEP/Random/RandGauss.h"
#include "JetMETCorrections/Modules/interface/JetResolution.h"
#include "DataFormats/Math/interface/deltaR.h"

namespace flashgg {

    class JetSmear: public BaseSystMethod<flashgg::Jet, int>
    {

    public:
        typedef StringCutObjectSelector<Jet, true> selector_type;

        JetSmear( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer * gv );
        void applyCorrection( flashgg::Jet &y, int syst_shift ) override;
        std::string shiftLabel( int ) const override;
        void eventInitialize( const edm::Event &iEvent, const edm::EventSetup & iSetup ) override;

    private:
        selector_type overall_range_;
        std::string random_label_;
        edm::EDGetTokenT<double> m_rho_token;
        bool debug_;

        JME::JetResolution resolution;
        JME::JetResolutionScaleFactor res_sf;
        double current_rho_;

        bool useTextFiles_;
        std::string textFileSF_;
        std::string textFileResolution_;

        bool textFilesRead_;
    };

    void JetSmear::eventInitialize( const edm::Event &iEvent, const edm::EventSetup & iSetup ) {

        if (useTextFiles_) {
            if (!textFilesRead_) {
                std::cout << " About to read resolution" << std::endl;
                resolution = JME::JetResolution(textFileResolution_);
                std::cout << " About to read SF" <<std::endl;
                res_sf = JME::JetResolutionScaleFactor(textFileSF_);
                std::cout << " Both resolution and SF now read" << std::endl;
                textFilesRead_ = true;
            }
        } else {
            resolution = JME::JetResolution::get(iSetup, "AK4PFchs_pt");
            res_sf = JME::JetResolutionScaleFactor::get(iSetup, "AK4PFchs");
        }

        edm::Handle<double> rho;
        iEvent.getByToken(m_rho_token, rho);
        current_rho_ = *rho;
    }

    JetSmear::JetSmear( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer * gv ) :
        BaseSystMethod( conf, std::forward<edm::ConsumesCollector>(iC) ), 
        overall_range_( conf.getParameter<std::string>( "OverallRange" ) ),
        random_label_( conf.getParameter<std::string>("RandomLabel")),
        m_rho_token(iC.consumes<double>( conf.getParameter<edm::InputTag>("rho"))),
        debug_( conf.getUntrackedParameter<bool>("Debug", false) ),
        useTextFiles_( conf.getParameter<bool>("UseTextFiles") ),
        textFileSF_( conf.getParameter<string>("TextFileSF") ),
        textFileResolution_( conf.getParameter<string>("TextFileResolution") )
    {
        if (!applyCentralValue()) throw cms::Exception("Configuration") << " module not set up to have central value turned off";
        textFilesRead_ = false;
    }

    std::string JetSmear::shiftLabel( int syst_value ) const
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

    void JetSmear::applyCorrection( flashgg::Jet &y, int syst_shift )
    {
        if( overall_range_( y ) ) {
            if ( debug_ ) {
                std::cout << "  " << shiftLabel( syst_shift ) << ": Jet has pt=" << y.pt() << " eta=" << y.eta() << std::endl;
            }
            JME::JetParameters parameters_1;
            parameters_1.setJetPt(y.pt());
            parameters_1.setJetEta(y.eta());
            parameters_1.setRho(current_rho_);
            float r = resolution.getResolution(parameters_1);
            if ( debug_ ) {
                std::cout << " GOT RESOLUTION " << std::endl;
            }
            float scale_factor;
            if (syst_shift == 0) {
                scale_factor = res_sf.getScaleFactor({{JME::Binning::JetPt, y.pt()}, {JME::Binning::JetEta, y.eta()}});
            } else if (syst_shift == 1) {
                scale_factor = res_sf.getScaleFactor({{JME::Binning::JetPt, y.pt()}, {JME::Binning::JetEta, y.eta()}}, Variation::UP);
            } else if (syst_shift == -1) {
                scale_factor = res_sf.getScaleFactor({{JME::Binning::JetPt, y.pt()}, {JME::Binning::JetEta, y.eta()}}, Variation::DOWN);
            } else {
                throw cms::Exception("UnsupportedJERShift") << " syst_shift=" << syst_shift << " is not supported";
            }
            if (debug_) std::cout << " GOT SCALE FACTOR " << std::endl;
            float recpt = y.pt();
            auto genjet = y.genJet();
            if (genjet != nullptr && deltaR(y,*(y.genJet())) < 0.2 && fabs(y.pt()-y.genJet()->pt()) < 3*r*y.pt()) {
                float genpt = y.genJet()->pt();
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
                float extra_smear_width = std::sqrt(max<float>(scale_factor*scale_factor - 1,0.)) * r;
                float escale = (1. + rnd * extra_smear_width);
                if (debug_) {
                    std::cout << "  " << shiftLabel( syst_shift ) << ": Jet has pt=" << y.pt() << " eta=" << y.eta() << " NO GEN MATCH "
                              << " rnd=" << rnd << " r=" << r << " scale_factor=" << scale_factor << " extra_smear_width=" << extra_smear_width
                              << " --> escale=" << escale << " to get new pt " << (escale*y.pt()) << std::endl;
                }
                y.setP4(escale*y.p4());
            }
        }
    }
}

DEFINE_EDM_PLUGIN( FlashggSystematicJetMethodsFactory,
                   flashgg::JetSmear,
                   "FlashggJetSmear" );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

