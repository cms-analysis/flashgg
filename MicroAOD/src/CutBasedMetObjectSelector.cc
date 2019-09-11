#include "flashgg/MicroAOD/interface/CutBasedMetObjectSelector.h"
#include "FWCore/Framework/interface/Event.h"
#include "flashgg/MicroAOD/interface/MetUtils.h"

using namespace edm;
using namespace std;

namespace flashgg {
    CutBasedMetObjectSelector::CutBasedMetObjectSelector( const ParameterSet &cfg, edm::ConsumesCollector &cc ) :
        //        classifier_( cfg ),
        //        rhoToken_( cc.consumes<double>( cfg.getParameter<InputTag>( "rho" ) ) ),
        selector_( cfg.getParameter<string>( "cut" ) ),
        metutil_(cfg),
        triggerRECO_( cc.consumes<edm::TriggerResults>(cfg.getParameter<InputTag>("RECOfilters") ) ),
        triggerPAT_( cc.consumes<edm::TriggerResults>(cfg.getParameter<InputTag>("PATfilters") ) )
    {
        //        jetutil_ = MetUtils(&cfg);
        
    }

    void CutBasedMetObjectSelector::handle( const EventBase &ev ) const
    {
        //        edm::Handle<double> rho;
        const Event *ee = dynamic_cast<const Event *>( &ev );
        //        if( ee ) { ee->getByToken( rhoToken_, rho ); }
        //        else { assert( 0 );  } // FIXME need to complete for FWlite
        //        rho_ = *rho;
    }

    bool CutBasedMetObjectSelector::operator()( const edm::Ref<edm::View<Met> > candref, const edm::Event &ev ) const {
        return operator()(*candref, ev);
    }

    bool CutBasedMetObjectSelector::operator()( const Met &cand, const edm::Event &evt ) const
    {
        if( ! selector_( cand ) ) { return false; }
        edm::Handle<edm::TriggerResults> triggerBits;
        if(! evt.isRealData() ){ 
            evt.getByToken( triggerPAT_, triggerBits );
        }
        else{
            evt.getByToken( triggerRECO_, triggerBits );
        }
        bool passSelection = metutil_.passesMetFilters(&evt, triggerBits);

        return passSelection;
    }

}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

