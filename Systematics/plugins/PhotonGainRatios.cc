#include "flashgg/Systematics/interface/BaseSystMethod.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "flashgg/DataFormats/interface/Photon.h"

#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"

namespace flashgg {

    class PhotonGainRatios: public BaseSystMethod<Photon,int>
    {

    public:
        PhotonGainRatios( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer *gv );
        void applyCorrection( flashgg::Photon &y, int syst_shift ) override;
        std::string shiftLabel( int ) const override;
        void eventInitialize( const edm::Event &, const edm::EventSetup & ) override;

    private:
        bool updateEnergy_;
        edm::EDGetTokenT<EcalRecHitCollection> ecalCalibHitEBToken_;
        edm::EDGetTokenT<EcalRecHitCollection> ecalRecalibHitEBToken_;
        edm::Handle<EcalRecHitCollection> ecalCalibHitEB_;
        edm::Handle<EcalRecHitCollection> ecalRecalibHitEB_;
    };

    PhotonGainRatios::PhotonGainRatios( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer *gv ) :
        BaseSystMethod<Photon,int>(conf,std::forward<edm::ConsumesCollector>(iC)),
        updateEnergy_( conf.getParameter<bool>( "updateEnergy" ) ),
        ecalCalibHitEBToken_( iC.consumes<EcalRecHitCollection>( conf.getParameter<edm::InputTag>( "calibratedEBRechits" ) ) ),
        ecalRecalibHitEBToken_( iC.consumes<EcalRecHitCollection>( conf.getParameter<edm::InputTag>( "reCalibratedEBRechits" ) ) )
    {
        this->setMakesWeight(false);
    }

    void PhotonGainRatios::eventInitialize(const edm::Event & evt, const edm::EventSetup & es) 
    {
        evt.getByToken(ecalCalibHitEBToken_,ecalCalibHitEB_);
        evt.getByToken(ecalRecalibHitEBToken_,ecalRecalibHitEB_);
        // evt.getByToken(ecalHitEEToken_,ecalHitEE_);
    }
    
    std::string PhotonGainRatios::shiftLabel( int syst_value ) const
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

  
    
    double CorrectionFunction1(double x, double chi2){
        if (x<4000) return 1;
        if (x>10000) x=10000;
        double p0   =    0.0567521;
        double p1   =  0.000609019;
        double p2   = -1.35626e-07;
        double p3   =  1.21114e-11;
        double p4   = -3.84392e-16;
        return p0+p1*x+p2*x*x+p3*x*x*x+p4*x*x*x*x;
    }
    double CorrectionFunction2(double x, double chi2){
        if (x<7000) return 1;
        if (x>18000) x=18000;
        double p0   =    0.770166;
        double p1   = 7.22759e-05;
        double p2   = -6.8392e-09;
        double p3   = 1.67209e-13;
        return p0+p1*x+p2*x*x+p3*x*x*x;
    }
    double CorrectionFunction3(double x, double chi2){
        if (x<4000 || x>7000) return 1;
        double p0   =       -2.391;
        double p1   =  0.000997096;
        double p2   = -8.34263e-08;
        return p0+p1*x+p2*x*x;
    }
    float MultiFitParametricCorrection(float amplitude_multifit_intime_uncal, float chi2, bool has_g6, bool has_g1){
        
        float x = amplitude_multifit_intime_uncal;
        
        if (!has_g1 && !has_g6) return 1.; // no gain switch
        else if (has_g6 && !has_g1) {
            if (x>5000 && chi2<250) return 1.;
            else return CorrectionFunction1(x,chi2);
        }
        else if (!has_g6 && has_g1){
            if (chi2>6000) return CorrectionFunction1(x,chi2);
            else if (x<4000 || x>7000) return 1.;
            else return CorrectionFunction3(x,chi2);
        }
        else if (has_g1 && has_g6) return CorrectionFunction2(x,chi2);
        else return 1.;
    }


    void PhotonGainRatios::applyCorrection( flashgg::Photon &y, int syst_shift )
    {
        const auto & hitsAndFractions = y.superCluster()->hitsAndFractions();
        
        float energySum=0., recalibEnergySum=0., recalibCorrEnergySum=0.;
        for(const auto & hnf : hitsAndFractions) {
            /// std::cout << hnf.first.rawId() << " " << hnf.second << std::endl;
            auto crh = ecalCalibHitEB_->find(hnf.first);
            if( crh == ecalCalibHitEB_->end() ) { continue; }
            auto rrh = ecalRecalibHitEB_->find(hnf.first);
            if( rrh == ecalRecalibHitEB_->end() ) { continue; }
            float chi2 = crh->chi2();
            bool g6 = crh->checkFlag( EcalRecHit::kHasSwitchToGain6 );
            bool g1 = crh->checkFlag( EcalRecHit::kHasSwitchToGain1 );           
            energySum += crh->energy() * hnf.second;
            recalibEnergySum += rrh->energy() * hnf.second;
            float gcorr = MultiFitParametricCorrection(rrh->energy(),chi2,g6,g1);
            recalibCorrEnergySum += rrh->energy() * hnf.second / gcorr;
            /// if( g6 || g1 ) { 
            ///     std::cout << "PhotonGainRatios::applyCorrection " << g6 << " " << g1 << " " << gcorr << " " << energySum << " " << recalibEnergySum << " " << recalibCorrEnergySum << std::endl;
            /// }
        }
        
        // corrRawEnergy = rawEnergy - energySum + energySum * recalibCorrEnergySum / recalibEnergySum
        float corr = 1. + energySum * ( recalibEnergySum != 0 ? recalibCorrEnergySum / recalibEnergySum - 1. : 0. ) / y.superCluster()->rawEnergy();
        
        // std::cout << "PhotonGainRatios::applyCorrection " << energySum << " " << recalibEnergySum << " " << recalibCorrEnergySum << std::endl;
        if( updateEnergy_ ) {
            y.updateEnergy( shiftLabel( syst_shift ), corr * y.energy() );
        } else {
            y.addUserFloat( shiftLabel( syst_shift ), corr * y.energy() );
        }
    }
    
}

DEFINE_EDM_PLUGIN( FlashggSystematicPhotonMethodsFactory,
                   flashgg::PhotonGainRatios,
                   "FlashggPhotonGainRatios" );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

