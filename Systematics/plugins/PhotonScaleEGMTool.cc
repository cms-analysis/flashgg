#include "flashgg/DataFormats/interface/Photon.h"
//#include "flashgg/Systematics/interface/ObjectSystMethodBinnedByFunctor.h"
#include "flashgg/Systematics/interface/BaseSystMethod.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "RecoEgamma/EgammaTools/interface/EnergyScaleCorrection.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

namespace edm {
    class Event;
}

namespace flashgg {

    //class PhotonScaleEGMTool: public ObjectSystMethodBinnedByFunctor<flashgg::Photon, int>
    class PhotonScaleEGMTool: public BaseSystMethod<flashgg::Photon, int>
    {

    public:
        typedef StringCutObjectSelector<Photon, true> selector_type;

        PhotonScaleEGMTool( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer *gv );
        void applyCorrection( flashgg::Photon &y, int syst_shift ) override;
        std::string shiftLabel( int ) const override;
        void eventInitialize( const edm::Event &iEvent, const edm::EventSetup & iSetup ) override;

    private:
        selector_type overall_range_;
        std::string correctionFile_;
        EnergyScaleCorrection scaler_;
        bool exaggerateShiftUp_; // debugging
        std::bitset<EnergyScaleCorrection::kErrNrBits> uncBitMask_; 
        bool debug_;
        unsigned run_number_;
    };

    PhotonScaleEGMTool::PhotonScaleEGMTool( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer *gv ) :
        BaseSystMethod( conf, std::forward<edm::ConsumesCollector>(iC)  ),
        overall_range_( conf.getParameter<std::string>( "OverallRange" ) ),
        correctionFile_( conf.getParameter<std::string>( "CorrectionFile" )),
        scaler_(correctionFile_),
        exaggerateShiftUp_( conf.getParameter<bool>( "ExaggerateShiftUp" ) ),
        uncBitMask_( conf.getParameter<std::string>("UncertaintyBitMask" ) ),
        debug_( conf.getUntrackedParameter<bool>( "Debug", false ) )
    {
    }

    void PhotonScaleEGMTool::eventInitialize( const edm::Event &iEvent, const edm::EventSetup & iSetup ) {
        run_number_ = iEvent.run();
    }
    
    std::string PhotonScaleEGMTool::shiftLabel( int syst_value ) const
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

    void PhotonScaleEGMTool::applyCorrection( flashgg::Photon &y, int syst_shift )
    {
        unsigned int gain=12;
        if(y.hasSwitchToGain1()) gain=1;
        if(y.hasSwitchToGain6()) gain=6;
        if( overall_range_( y ) ) {
            auto shift_val = scaler_.scaleCorr(run_number_, y.et(), y.superCluster()->eta(), y.full5x5_r9(), gain, uncBitMask_);
            auto shift_err = scaler_.scaleCorrUncert(run_number_, y.et(), y.superCluster()->eta(), y.full5x5_r9(), gain, uncBitMask_);
            if (!applyCentralValue()) shift_val = 1.;
            float scale = shift_val + syst_shift * shift_err;
            if( debug_ ) {
                std::cout << "  " << shiftLabel( syst_shift ) << ": Photon has pt= " << y.pt() << " eta=" << y.eta() << " gain=" << gain
                          << " and we apply a multiplicative correction of " << scale << std::endl;
            }
            y.updateEnergy( shiftLabel( syst_shift ), scale * y.energy() );
        }
    }
}

DEFINE_EDM_PLUGIN( FlashggSystematicPhotonMethodsFactory,
                   flashgg::PhotonScaleEGMTool,
                   "FlashggPhotonScaleEGMTool" );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
