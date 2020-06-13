#include "flashgg/MicroAOD/interface/CutBasedJetObjectSelector.h"
#include "FWCore/Framework/interface/Event.h"
#include "flashgg/MicroAOD/interface/JetUtils.h"

using namespace edm;
using namespace std;

namespace flashgg {
    CutBasedJetObjectSelector::CutBasedJetObjectSelector( const ParameterSet &cfg, edm::ConsumesCollector &cc ) :
        //        classifier_( cfg ),
        //        rhoToken_( cc.consumes<double>( cfg.getParameter<InputTag>( "rho" ) ) ),
        selector_( cfg.getParameter<string>( "cut" ) ),
        jetutil_(cfg)
    {
        //        jetutil_ = JetUtils(&cfg);
        
    }

    void CutBasedJetObjectSelector::handle( const EventBase &ev ) const
    {
        //        edm::Handle<double> rho;
        const Event *ee = dynamic_cast<const Event *>( &ev );
        //        if( ee ) { ee->getByToken( rhoToken_, rho ); }
        //        else { assert( 0 );  } // FIXME need to complete for FWlite
        //        rho_ = *rho;
    }

    bool CutBasedJetObjectSelector::operator()( const edm::Ref<edm::View<Jet> > candref, const edm::EventBase &ev ) const {
        return operator()(*candref, ev);
    }

    bool CutBasedJetObjectSelector::operator()( const Jet &cand, const EventBase &ev ) const
    {
        if( ! selector_( cand ) ) { return false; }
        //        handle( ev );
        //   edm::Ptr<flashgg::Jet>  = photons->ptrAt( i );
        bool passSelection = jetutil_.passesPuJetId(&cand);

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

