#ifndef FLASHgg_DiPhotonFromSinglePhotonViewBase_h
#define FLASHgg_DiPhotonFromSinglePhotonViewBase_h

#include "flashgg/Systematics/interface/BaseSystMethod.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"
#include <memory>

namespace flashgg {


    template <class param_var>
    class DiPhotonFromSinglePhotonViewBase : public BaseSystMethod<DiPhotonCandidate, param_var>
    {

    public:
        DiPhotonFromSinglePhotonViewBase( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer * gv );

        void applyCorrection( DiPhotonCandidate &y, param_var syst_shift ) override;
        float makeWeight( const DiPhotonCandidate &y, param_var syst_shift ) override;
        std::string shiftLabel( param_var ) const override;

        void setRandomEngine( CLHEP::HepRandomEngine &eng ) override
        {
            //            std::cout << " DiPhotonFromSinglePhotonViewBase::setRandomEngine " << std::endl;
            BaseSystMethod<DiPhotonCandidate, param_var>::setRandomEngine( eng );
            photon_corr_->setRandomEngine( eng );
        }
        
    protected:
        bool debug_;

    private:
        std::unique_ptr<BaseSystMethod<flashgg::SinglePhotonView, param_var> > photon_corr_;
        std::unique_ptr<BaseSystMethod<flashgg::SinglePhotonView, param_var> > photon_corr2_;
    };

    template<class param_var>
    DiPhotonFromSinglePhotonViewBase<param_var>::DiPhotonFromSinglePhotonViewBase( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer * gv ) :
        BaseSystMethod<DiPhotonCandidate, param_var>::BaseSystMethod( conf, std::forward<edm::ConsumesCollector>(iC) ),
        debug_( conf.getUntrackedParameter<bool>( "Debug", false ) )
    {
        std::string photonMethodName = conf.getParameter<std::string>( "PhotonMethodName" );
        photon_corr_.reset( FlashggSystematicMethodsFactory<flashgg::SinglePhotonView, param_var>::get()->create( photonMethodName, conf, std::forward<edm::ConsumesCollector>(iC), gv ) );
        if(conf.exists("BinList2"))  //if defined, BinList2 gives bins for sublead, lead uses BinList
            {
                edm::ParameterSet conf2;// =  conf.clone();
                
                conf2.copyFrom(conf,"PhotonMethodName");
                conf2.copyFrom(conf,"MethodName");
                conf2.copyFrom(conf,"Label");
                conf2.copyFrom(conf,"NSigmas");
                conf2.copyFrom(conf,"OverallRange");
                conf2.copyFrom(conf,"Debug");
                conf2.copyFrom(conf,"ApplyCentralValue");
                const auto &pset = conf.getParameterSet( "BinList2" );
                conf2.addParameter<edm::ParameterSet>("BinList", pset);
                std::string binListName = "BinList";
                conf2.insertParameterSet(true,binListName, *(conf.retrieveUnknownParameterSet("BinList2")));
                photon_corr2_.reset( FlashggSystematicMethodsFactory<flashgg::SinglePhotonView, param_var>::get()->create( photonMethodName, conf2, std::forward<edm::ConsumesCollector>(iC),  gv ) );
                
            }
        else { //if BinList2 is not defined, use BinList for both lead and sublead photons
            photon_corr2_.reset( FlashggSystematicMethodsFactory<flashgg::SinglePhotonView, param_var>::get()->create( photonMethodName, conf, std::forward<edm::ConsumesCollector>(iC),  gv ) );
        }
        this->setMakesWeight( photon_corr_->makesWeight() );
    }

    template<class param_var>
    std::string DiPhotonFromSinglePhotonViewBase<param_var>::shiftLabel( param_var syst_value ) const
    {
        return photon_corr_->shiftLabel( syst_value );
    }

    template<typename param_var>
    float DiPhotonFromSinglePhotonViewBase<param_var>::makeWeight( const DiPhotonCandidate &y, param_var syst_shift )
    {
        if( debug_ ) {
            std::cout << "START OF DiPhotonFromPhoton::makeWeight M PT E1 E2 ETA1 ETA2 "
                      << y.mass() << " " << y.pt() << " " << y.leadingPhoton()->energy() << " " << y.subLeadingPhoton()->energy() << " "
                      << y.leadingPhoton()->eta() << " " << y.subLeadingPhoton()->eta() << std::endl;
        }
        float weight1 = photon_corr_->makeWeight( *(y.leadingView()), syst_shift );
        float weight2 = photon_corr2_->makeWeight( *(y.subLeadingView()), syst_shift );
        float diphoweight = weight1*weight2;
        if( debug_ ) {
            std::cout << "END OF DiPhotonFromPhoton::makeWeight M PT E1 E2 ETA1 ETA2 "
                      << " weight1=" << weight1 << " weight2=" << weight2 << " diphoweight=" << diphoweight << std::endl;
        }
        return diphoweight;
    }

    template<class param_var>
    void DiPhotonFromSinglePhotonViewBase<param_var>::applyCorrection( DiPhotonCandidate &y, param_var syst_shift )
    {
        if( debug_ ) {
            std::cout << "START OF DiPhotonFromPhoton::applyCorrection M PT E1 E2 ETA1 ETA2 "
                      << y.mass() << " " << y.pt() << " " << y.leadingPhoton()->energy() << " " << y.subLeadingPhoton()->energy() << " "
                      << y.leadingPhoton()->eta() << " " << y.subLeadingPhoton()->eta() << std::endl;
        }
        y.makePhotonsPersistent();
        photon_corr_->applyCorrection( y.getLeadingView(), syst_shift );
        photon_corr_->applyCorrection( y.getSubLeadingView(), syst_shift );
        y.computeP4AndOrder();
        if( debug_ ) {
            std::cout << "END OF DiPhotonFromPhoton::applyCorrection M PT E1 E2 ETA1 ETA2 "
                      << y.mass() << " " << y.pt() << " " << y.leadingPhoton()->energy() << " " << y.subLeadingPhoton()->energy() << " "
                      << y.leadingPhoton()->eta() << " " << y.subLeadingPhoton()->eta() << std::endl;
        }
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

