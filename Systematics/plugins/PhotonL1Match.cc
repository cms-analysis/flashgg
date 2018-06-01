#include "flashgg/Systematics/interface/BaseSystMethod.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "flashgg/DataFormats/interface/Photon.h"

#include "DataFormats/L1Trigger/interface/EGamma.h"
#include "DataFormats/L1Trigger/interface/Jet.h"

namespace flashgg {

    class PhotonL1Match: public BaseSystMethod<Photon,int>
    {

    public:
        PhotonL1Match( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer *gv );
        void applyCorrection( flashgg::Photon &y, int syst_shift ) override;
        std::string shiftLabel( int ) const override;
        void eventInitialize( const edm::Event &, const edm::EventSetup & ) override;

    private:
        bool doEgm_, doJets_;
        edm::EDGetTokenT<l1t::EGammaBxCollection> l1EgmObjectsTok_;
        edm::EDGetTokenT<l1t::JetBxCollection> l1JetObjectsTok_;
        std::vector<l1t::EGamma> l1EgmObjects_;
        std::vector<l1t::Jet> l1JetObjects_;
        double deltaRmax_;
        
        template<class T> struct ComparePt {
            bool operator()( const T& t1, const T& t2 ) const {
                return t1.et() > t2.et();
            }
        };
    };

    PhotonL1Match::PhotonL1Match( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer *gv ) :
        BaseSystMethod<Photon,int>(conf,std::forward<edm::ConsumesCollector>(iC)),
        doEgm_(conf.exists("l1EgmSrc")),
        doJets_(conf.exists("l1JetSrc")),
        deltaRmax_( conf.getParameter<double>("deltaRmax")  )
    {
        this->setMakesWeight(false);
        if( doEgm_ ) {
            l1EgmObjectsTok_ = iC.consumes<l1t::EGammaBxCollection>(conf.getParameter<edm::InputTag>("l1EgmSrc"));
        }
        if( doJets_ ) {
            l1JetObjectsTok_ = iC.consumes<l1t::JetBxCollection>(conf.getParameter<edm::InputTag>("l1JetSrc"));
        }
    }

    void PhotonL1Match::eventInitialize(const edm::Event & evt, const edm::EventSetup & es) 
    {
        edm::Handle<l1t::EGammaBxCollection> l1EgmObjects;
        edm::Handle<l1t::JetBxCollection> l1JetObjects;
        if( doEgm_ ) {
            evt.getByToken(l1EgmObjectsTok_,l1EgmObjects);
            l1EgmObjects_.clear();
            for(auto obj : *l1EgmObjects ) { l1EgmObjects_.push_back(obj); }
            std::sort(l1EgmObjects_.begin(), l1EgmObjects_.end(), ComparePt<l1t::EGamma>());
        }
        if( doJets_ ) {
            evt.getByToken(l1JetObjectsTok_,l1JetObjects);
            l1JetObjects_.clear();
            for(auto obj : *l1JetObjects ) { l1JetObjects_.push_back(obj); }
            std::sort(l1JetObjects_.begin(), l1JetObjects_.end(), ComparePt<l1t::Jet>());
        }
    }
    
    std::string PhotonL1Match::shiftLabel( int syst_value ) const
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

    void PhotonL1Match::applyCorrection( flashgg::Photon &y, int syst_shift )
    {
        if( doEgm_ ) {
            for (auto obj : l1EgmObjects_) { 
                
                auto dR = reco::deltaR( obj, y.superCluster()->position() );
                if( dR > deltaRmax_ ) { continue; }
                
                y.addUserInt("l1EgmMatch",1);
                y.addUserFloat("l1EgmCandDR",dR);
                y.addUserFloat("l1EgmCandPt",obj.et());
                break;
            }
            if( ! y.hasUserInt("l1EgmMatch") ) {
                y.addUserInt("l1EgmMatch",0);
            }
        }
        if( doJets_ ) {
            for (auto obj : l1JetObjects_) { 
                
                auto dR = reco::deltaR( obj, y );
                if( dR > deltaRmax_ ) { continue; }
                
                y.addUserInt("l1JetMatch",1);
                y.addUserFloat("l1JetCandDR",dR);
                y.addUserFloat("l1JetCandPt",obj.et());
                break;
            }
            if( ! y.hasUserInt("l1JetMatch") ) {
                y.addUserInt("l1JetMatch",0);
            }
        }
    }
}

DEFINE_EDM_PLUGIN( FlashggSystematicPhotonMethodsFactory,
                   flashgg::PhotonL1Match,
                   "FlashggPhotonL1Match" );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

