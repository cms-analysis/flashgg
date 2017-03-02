#include "flashgg/Systematics/interface/BaseSystMethod.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

namespace flashgg {

    class PhotonEleMatch: public BaseSystMethod<Photon,int>
    {

    public:
        PhotonEleMatch( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer *gv );
        void applyCorrection( flashgg::Photon &y, int syst_shift ) override;
        std::string shiftLabel( int ) const override;
        void eventInitialize( const edm::Event &, const edm::EventSetup & ) override;

    private:
        edm::EDGetTokenT<edm::View<pat::Electron> > electronToken_;
        edm::Handle<edm::View<pat::Electron> > electronHandle_;

    };

    PhotonEleMatch::PhotonEleMatch( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer *gv ) :
        BaseSystMethod<Photon,int>(conf,std::forward<edm::ConsumesCollector>(iC)),
        electronToken_( iC.consumes<edm::View<pat::Electron> >( conf.getParameter<edm::InputTag>( "electronsSrc") ) )
    {
        this->setMakesWeight(false);
    }

    void PhotonEleMatch::eventInitialize(const edm::Event & evt, const edm::EventSetup & es) 
    {
        evt.getByToken( electronToken_, electronHandle_ );
    }
    
    std::string PhotonEleMatch::shiftLabel( int syst_value ) const
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

    void PhotonEleMatch::applyCorrection( flashgg::Photon &y, int syst_shift )
    {
        for(size_t iele=0; iele<electronHandle_->size(); ++iele) // First try matching by reference
        {
            const auto & ele = electronHandle_->ptrAt(iele);
            if( ele->superCluster() != y.superCluster() ) { continue; }
            y.addUserCand(label(),ele);
            break;
        }
        if( ! y.hasUserCand(label()) ) {
            for(size_t iele=0; iele<electronHandle_->size(); ++iele) // if not match by reference try geometrically
            {
                const auto & ele = electronHandle_->ptrAt(iele);
                if( reco::deltaR(ele->superCluster()->position(),y.superCluster()->position())>0.05 ) { continue; } // FIXME: delta R matching
                y.addUserCand(label(),ele);
                break;
            }
        }
    }
}

DEFINE_EDM_PLUGIN( FlashggSystematicPhotonMethodsFactory,
                   flashgg::PhotonEleMatch,
                   "FlashggPhotonEleMatch" );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

