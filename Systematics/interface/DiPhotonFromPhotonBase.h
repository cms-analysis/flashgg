#ifndef FLASHgg_DiPhotonFromPhotonBase_h
#define FLASHgg_DiPhotonFromPhotonBase_h

#include "flashgg/Systematics/interface/BaseSystMethod.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"
#include <memory>

namespace flashgg {


    template <class param_var>
    class DiPhotonFromPhotonBase : public BaseSystMethod<DiPhotonCandidate, param_var>
    {

    public:
        DiPhotonFromPhotonBase( const edm::ParameterSet &conf );

        void applyCorrection( DiPhotonCandidate &y, param_var syst_shift ) override;
        std::string shiftLabel( param_var ) const override;

        void setRandomEngine( CLHEP::HepRandomEngine &eng ) override
        {
            //            std::cout << " DiPhotonFromPhotonBase::setRandomEngine " << std::endl;
            BaseSystMethod<DiPhotonCandidate, param_var>::setRandomEngine( eng );
            photon_corr_->setRandomEngine( eng );
        }

    protected:
        bool debug_;

    private:
        std::unique_ptr<BaseSystMethod<flashgg::Photon, param_var> > photon_corr_;
    };

    template<class param_var>
    DiPhotonFromPhotonBase<param_var>::DiPhotonFromPhotonBase( const edm::ParameterSet &conf ) :
        BaseSystMethod<DiPhotonCandidate, param_var>::BaseSystMethod( conf ),
        debug_( conf.getUntrackedParameter<bool>( "Debug", false ) )
    {
        std::string photonMethodName = conf.getParameter<std::string>( "PhotonMethodName" );
        photon_corr_.reset( FlashggSystematicMethodsFactory<flashgg::Photon, param_var>::get()->create( photonMethodName, conf ) );
    }

    template<class param_var>
    std::string DiPhotonFromPhotonBase<param_var>::shiftLabel( param_var syst_value ) const
    {
        return photon_corr_->shiftLabel( syst_value );
    }

    template<class param_var>
    void DiPhotonFromPhotonBase<param_var>::applyCorrection( DiPhotonCandidate &y, param_var syst_shift )
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

#endif

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

