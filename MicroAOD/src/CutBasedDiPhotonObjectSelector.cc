#include "flashgg/MicroAOD/interface/CutBasedDiPhotonObjectSelector.h"
#include "FWCore/Framework/interface/Event.h"

using namespace edm;
using namespace std;

namespace flashgg {
    CutBasedDiPhotonObjectSelector::CutBasedDiPhotonObjectSelector( const ParameterSet &cfg, edm::ConsumesCollector &cc ) :
        CutBasedPhotonObjectSelector( cfg, cc ),
        selector_( cfg.getParameter<string>( "cut" ) )
    {
        if( cfg.exists( "invert" ) ) {
            invertNtimes_ = cfg.getParameter<int>( "invertNtimes" );
        }
    }

    bool CutBasedDiPhotonObjectSelector::operator()( const edm::Ref<edm::View<DiPhotonCandidate> > candref, const edm::EventBase &ev ) const 
    {
        return operator()( *candref,ev );
    }

    bool CutBasedDiPhotonObjectSelector::operator()( const DiPhotonCandidate &cand, const EventBase &ev ) const
    {
        if( ! selector_( cand ) ) { return false; }
        handle( ev );
        auto leadingPhoton = cand.leadingPhoton();
        auto subleadingPhoton = cand.subLeadingPhoton();
        bool passSelection = pass( *leadingPhoton ) && pass( *subleadingPhoton );
        if( ! passSelection ) { return false; }
        if( ! invertCuts_.empty() ) {
            if( invertNtimes_ == 0 ) {
                return passInverted( *leadingPhoton ) || passInverted( *subleadingPhoton );
            } else if( invertNtimes_ == 1 ) {
                return passInverted( *leadingPhoton ) ^ passInverted( *subleadingPhoton );
            } else {
                return passInverted( *leadingPhoton ) && passInverted( *subleadingPhoton );
            }
        }
        return true;
    }

}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

