#include "flashgg/Systematics/interface/BaseSystMethod.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "flashgg/DataFormats/interface/Photon.h"

#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"

namespace flashgg {

    class PhotonHLTMatch: public BaseSystMethod<Photon,int>
    {

    public:
        PhotonHLTMatch( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer *gv );
        void applyCorrection( flashgg::Photon &y, int syst_shift ) override;
        std::string shiftLabel( int ) const override;
        void eventInitialize( const edm::Event &, const edm::EventSetup & ) override;

    private:
        std::vector<string> pathNames_;
        edm::EDGetTokenT<edm::TriggerResults> triggerBitsTok_;
        edm::EDGetTokenT<pat::TriggerObjectStandAloneCollection> triggerObjectsTok_;
        edm::Handle<edm::TriggerResults> triggerBits_;
        edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjects_;
        double deltaRmax_;
        edm::TriggerNames trgNames_;
    };

    PhotonHLTMatch::PhotonHLTMatch( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer *gv ) :
        BaseSystMethod<Photon,int>(conf,std::forward<edm::ConsumesCollector>(iC)),
        pathNames_(conf.getParameter<std::vector<string> >("pathNames")),
        triggerBitsTok_(iC.consumes<edm::TriggerResults>(conf.getParameter<edm::InputTag>("trgBitsSrc"))),        
        triggerObjectsTok_(iC.consumes<pat::TriggerObjectStandAloneCollection>(conf.getParameter<edm::InputTag>("trgObjectsSrc"))),
        deltaRmax_( conf.getParameter<double>("deltaRmax")  )
    {
        this->setMakesWeight(false);
    }

    void PhotonHLTMatch::eventInitialize(const edm::Event & evt, const edm::EventSetup & es) 
    {
        evt.getByToken(triggerBitsTok_, triggerBits_);
        evt.getByToken(triggerObjectsTok_, triggerObjects_);
        trgNames_ = evt.triggerNames(*triggerBits_);
    }
    
    std::string PhotonHLTMatch::shiftLabel( int syst_value ) const
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

    void PhotonHLTMatch::applyCorrection( flashgg::Photon &y, int syst_shift )
    {
        for (pat::TriggerObjectStandAlone obj : *triggerObjects_) {
            
            auto dR = reco::deltaR( obj, y.superCluster()->position() );
            if( dR > deltaRmax_ ) { continue; }
            
            auto pT = obj.pt();
            obj.unpackPathNames(trgNames_);
            auto pathNamesAll = obj.pathNames(false);
            for( auto & opn: pathNamesAll ) {
                if( ! obj.hasPathName( opn, true, false ) ) { continue; }
                for( auto & pn : pathNames_ ) {
                    if( opn.find(pn) != std::string::npos ) {
                        y.addUserInt(pn,1);
                        y.addUserFloat(pn+std::string("CandDR"),dR);
                        y.addUserFloat(pn+std::string("CandPt"),pT);
                    }
                }
            }
        }
        for( auto & pn : pathNames_ ) {
            if( ! y.hasUserInt(pn) ) { y.addUserInt(pn,0); }
        }
    }
}

DEFINE_EDM_PLUGIN( FlashggSystematicPhotonMethodsFactory,
                   flashgg::PhotonHLTMatch,
                   "FlashggPhotonHLTMatch" );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

