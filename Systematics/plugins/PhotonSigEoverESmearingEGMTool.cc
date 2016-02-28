// written by F Ferri, adapted by S Zenz  

#include "flashgg/Systematics/interface/BaseSystMethod.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
#include "FWCore/Framework/interface/Event.h"
#include "EgammaAnalysis/ElectronTools/interface/EnergyScaleCorrection_class.hh"

namespace flashgg {
    
    class PhotonSigEoverESmearingEGMTool: public BaseSystMethod<flashgg::Photon, std::pair<int, int> >
    {
        
    public:
        typedef StringCutObjectSelector<Photon, true> selector_type;
        
        PhotonSigEoverESmearingEGMTool( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer *gv );
        void applyCorrection( flashgg::Photon &y, std::pair<int, int> syst_shift ) override;
        std::string shiftLabel( std::pair<int, int> ) const override;
        void eventInitialize( const edm::Event &iEvent, const edm::EventSetup & iSetup ) override;
        
        const std::string &firstParameterName() const { return label1_; }
        const std::string &secondParameterName() const { return label2_; }
        
    private:
        selector_type overall_range_;
        const std::string label1_;
        const std::string label2_;
        EnergyScaleCorrection_class scaler_;
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
        label1_( conf.getParameter<std::string>( "FirstParameterName" ) ), // default: "Rho"
        label2_( conf.getParameter<std::string>( "SecondParameterName" ) ), // default; "Phi"
        scaler_(conf.getParameter<std::string>( "CorrectionFile" )),
        exaggerateShiftUp_( conf.getParameter<bool>( "ExaggerateShiftUp" ) ), // default: false
        debug_( conf.getUntrackedParameter<bool>("Debug", false) )
    {
        if (!applyCentralValue()) throw cms::Exception("SmearingLogic") << "If we do not apply central smearing we cannot scale down the smearing";
        else scaler_.doSmearings = true;
    }
    
    std::string PhotonSigEoverESmearingEGMTool::shiftLabel( std::pair<int, int> syst_value ) const
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
    
    void PhotonSigEoverESmearingEGMTool::applyCorrection( flashgg::Photon &y, std::pair<int, int> syst_shift )
    {
        if( overall_range_( y ) ) {
            // Nothing will happen, with no warning, if the bin count doesn't match expected options
            // TODO for production: make this behavior more robust
            
            // the combination of central value + NSigma * sigma is already
            // computed by getSmearingSigma(...)
            auto sigma = scaler_.getSmearingSigma(run_number_, y.isEB(), y.full5x5_r9(), y.superCluster()->eta(), y.et(), syst_shift.first, syst_shift.second);

            if( debug_ ) { 
                std::cout << "  " << shiftLabel( syst_shift ) << ": Photon has pt= " << y.pt() << " eta=" << y.superCluster()->eta() << " full5x5_r9=" << y.full5x5_r9()
                    << " and we apply a sigmaEoverE smearing of " << sigma << std::endl;
                std::cout << " syst_shift.first: " << syst_shift.first << " syst_shift.second: " << syst_shift.second <<std::endl;
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

DEFINE_EDM_PLUGIN( FlashggSystematicPhotonMethodsFactory2D,
                   flashgg::PhotonSigEoverESmearingEGMTool,
                   "FlashggPhotonSigEoverESmearingEGMTool" );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
