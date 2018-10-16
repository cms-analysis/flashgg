// written by F Ferri, adapted by S Zenz  

#include "flashgg/Systematics/interface/BaseSystMethod.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
#include "FWCore/Framework/interface/Event.h"
#include "RecoEgamma/EgammaTools/interface/EnergyScaleCorrection.h"

namespace flashgg {
    
    class PhotonSmearStochasticEGMTool: public BaseSystMethod<flashgg::Photon, std::pair<int, int> >
    {
        
    public:
        typedef StringCutObjectSelector<Photon, true> selector_type;
        
        PhotonSmearStochasticEGMTool( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer *gv );
        void applyCorrection( flashgg::Photon &y, std::pair<int, int> syst_shift ) override;
        std::string shiftLabel( std::pair<int, int> ) const override;
        void eventInitialize( const edm::Event &iEvent, const edm::EventSetup & iSetup ) override;
        
        const std::string &firstParameterName() const { return label1_; }
        const std::string &secondParameterName() const { return label2_; }
        
    private:
        selector_type overall_range_;
        const std::string label1_;
        const std::string label2_;
        std::string random_label_;
        EnergyScaleCorrection scaler_;
        bool exaggerateShiftUp_; // debugging
        unsigned run_number_;
        bool debug_;
    };

    void PhotonSmearStochasticEGMTool::eventInitialize( const edm::Event &iEvent, const edm::EventSetup & iSetup ) {
        run_number_ = iEvent.run();
    }
    
    PhotonSmearStochasticEGMTool::PhotonSmearStochasticEGMTool( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer *gv ) :
        BaseSystMethod( conf, std::forward<edm::ConsumesCollector>(iC) ),
        overall_range_( conf.getParameter<std::string>( "OverallRange" ) ),
        label1_( conf.getParameter<std::string>( "FirstParameterName" ) ), // default: "Rho"
        label2_( conf.getParameter<std::string>( "SecondParameterName" ) ), // default; "Phi"
        random_label_(conf.getParameter<std::string>("RandomLabel")),
        scaler_(conf.getParameter<std::string>( "CorrectionFile" )),
        exaggerateShiftUp_( conf.getParameter<bool>( "ExaggerateShiftUp" ) ), // default: false
        debug_( conf.getUntrackedParameter<bool>("Debug", false) )
    {
        if (!applyCentralValue()) throw cms::Exception("SmearingLogic") << "If we do not apply central smearing we cannot scale down the smearing";
    }
    
    std::string PhotonSmearStochasticEGMTool::shiftLabel( std::pair<int, int> syst_value ) const
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
    
    void PhotonSmearStochasticEGMTool::applyCorrection( flashgg::Photon &y, std::pair<int, int> syst_shift )
    {
        unsigned int gain=12;
        if(y.hasSwitchToGain1()) gain=1;
        if(y.hasSwitchToGain6()) gain=6;

        if( overall_range_( y ) ) {
            // Nothing will happen, with no warning, if the bin count doesn't match expected options
            // TODO for production: make this behavior more robust
            
            // the combination of central value + NSigma * sigma is already
            // computed by getSmearingSigma(...)
            auto sigma = scaler_.smearingSigma(run_number_, y.et(), y.superCluster()->eta(), y.full5x5_r9(), gain, ((float)syst_shift.first), ((float)syst_shift.second));

            if ( sigma < 0. || sigma > 1. ) {
                throw cms::Exception("SmearingLogic") << " sigmaEOverE is going to be smeared by " << sigma << " which sounds implausible (allowed: 0-1)";                                                                 
            }
            
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

DEFINE_EDM_PLUGIN( FlashggSystematicPhotonMethodsFactory2D,
                   flashgg::PhotonSmearStochasticEGMTool,
                   "FlashggPhotonSmearStochasticEGMTool" );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
