#include "flashgg/Systematics/interface/BaseSystMethods.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"
#include <memory>

namespace flashgg {


    class DiPhotonFromPhoton: public BaseSystMethods<DiPhotonCandidate, int>
    {

    public:
        DiPhotonFromPhoton( const edm::ParameterSet &conf );

        void applyCorrection( DiPhotonCandidate &y, int syst_shift ) override;
        std::string shiftLabel( int ) override;

    private:
        std::unique_ptr<BaseSystMethods<flashgg::Photon, int> > photon_corr_;
        bool debug_;
    };

    DiPhotonFromPhoton::DiPhotonFromPhoton( const edm::ParameterSet &conf ) :
        BaseSystMethods( conf ),
        debug_( conf.getUntrackedParameter<bool>( "Debug", false ) )
    {
        std::string photonMethodName = conf.getParameter<std::string>( "PhotonMethodName" );
        photon_corr_.reset( FlashggSystematicMethodsFactory<flashgg::Photon, int>::get()->create( photonMethodName, conf ) );
    }

    std::string DiPhotonFromPhoton::shiftLabel( int syst_value )
    {
        return photon_corr_->shiftLabel( syst_value );
    }

    void DiPhotonFromPhoton::applyCorrection( DiPhotonCandidate &y, int syst_shift )
    {
        if( debug_ ) { std::cout << "START OF DiPhotonFromPhoton::applyCorrection M PT E1 E2 " << y.mass() << " " << y.pt() << " " << y.leadingPhoton()->energy() << " " << y.subLeadingPhoton()->energy() << std::endl; }
        photon_corr_->applyCorrection( y.getLeadingPhoton(), syst_shift );
        photon_corr_->applyCorrection( y.getSubLeadingPhoton(), syst_shift );
        AddFourMomenta addP4;
        addP4.set( y );
        y.setSystLabel( shiftLabel( syst_shift ) );
        if( debug_ ) { std::cout << "END OF DiPhotonFromPhoton::applyCorrection M PT E1 E2 " << y.mass() << " " << y.pt() << " " << y.leadingPhoton()->energy() << " " << y.subLeadingPhoton()->energy() << std::endl; }
    }
}

DEFINE_EDM_PLUGIN( FlashggSystematicDiPhotonMethodsFactory,
                   flashgg::DiPhotonFromPhoton,
                   "FlashggDiPhotonFromPhoton" );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

