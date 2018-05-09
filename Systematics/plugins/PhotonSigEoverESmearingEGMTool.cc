// written by F Ferri, adapted by S Zenz  

#include "flashgg/Systematics/interface/BaseSystMethod.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
#include "FWCore/Framework/interface/Event.h"
#include "RecoEgamma/EgammaTools/interface/EnergyScaleCorrection.h"

namespace flashgg {
    
    class PhotonSigEoverESmearingEGMTool: public BaseSystMethod<flashgg::Photon, int >
    {
        
    public:
        typedef StringCutObjectSelector<Photon, true> selector_type;
        
        PhotonSigEoverESmearingEGMTool( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer *gv );
        void applyCorrection( flashgg::Photon &y, int syst_shift ) override;
        std::string shiftLabel( int ) const override;
        void eventInitialize( const edm::Event &iEvent, const edm::EventSetup & iSetup ) override;
        
    private:
        selector_type overall_range_;
        EnergyScaleCorrection scaler_;
        bool exaggerateShiftUp_; // debugging
        unsigned run_number_;
        bool debug_;
    };

    void PhotonSigEoverESmearingEGMTool::eventInitialize( const edm::Event &iEvent, const edm::EventSetup & iSetup ) {
        run_number_ = iEvent.run();
    }
    
    PhotonSigEoverESmearingEGMTool::PhotonSigEoverESmearingEGMTool( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer *gv ) :
        BaseSystMethod( conf, std::forward<edm::ConsumesCollector>(iC) ),
        overall_range_( conf.getParameter<std::string>( "OverallRange" ) ),
        scaler_(conf.getParameter<std::string>( "CorrectionFile" )),
        exaggerateShiftUp_( conf.getParameter<bool>( "ExaggerateShiftUp" ) ), // default: false
        debug_( conf.getUntrackedParameter<bool>("Debug", false) )
    {
        if (!applyCentralValue()) throw cms::Exception("SmearingLogic") << "If we do not apply central smearing we cannot scale down the smearing";
    }
    
    std::string PhotonSigEoverESmearingEGMTool::shiftLabel( int syst_value ) const
    {
        std::string result;
        if( syst_value == 0 ) {
            result = Form( "%s", label().c_str() );
        }
        return result;
    }
    
    void PhotonSigEoverESmearingEGMTool::applyCorrection( flashgg::Photon &y, int syst_shift )
    {
        unsigned int gain=12;
        if(y.hasSwitchToGain1()) gain=1;
        if(y.hasSwitchToGain6()) gain=6;

        if( overall_range_( y ) ) {
            // Nothing will happen, with no warning, if the bin count doesn't match expected options
            // TODO for production: make this behavior more robust
            
            // the combination of central value + NSigma * sigma is already
            // computed by getSmearingSigma(...)
            auto sigma = scaler_.smearingSigma(run_number_, y.et(), y.superCluster()->eta(), y.full5x5_r9(), gain, 0, 0);// never apply systematic shift

            if ( sigma < 0. || sigma > 1. ) {
                throw cms::Exception("SmearingLogic") << " sigmaEOverE is going to be smeared by " << sigma << " which sounds implausible (allowed: 0-1)";
            }

            if( debug_ ) { 
                std::cout << "  " << shiftLabel( syst_shift ) << ": Photon has pt= " << y.pt() << " eta=" << y.superCluster()->eta() << " full5x5_r9=" << y.full5x5_r9()
                    << " and we apply a sigmaEoverE smearing of " << sigma << std::endl;
                std::cout << "     sigE/E VALUE BEFORE: ";
                auto beforeSigEoE = y.sigEOverE();
                std::cout << beforeSigEoE << " ";
                std::cout << std::endl;
            }

            y.addUserFloat("unsmearedSigmaEoE", y.sigEOverE() );
            y.smearSigmaEOverEValueBy( sigma );

            if ( debug_) {
                auto afterSigEoE = y.sigEOverE();
                std::cout << "     sigE/E VALUE AFTER: ";
                std::cout << afterSigEoE << " ";
                std::cout << std::endl;
            }

        }
    }
}

DEFINE_EDM_PLUGIN( FlashggSystematicPhotonMethodsFactory,
                   flashgg::PhotonSigEoverESmearingEGMTool,
                   "FlashggPhotonSigEoverESmearingEGMTool" );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
