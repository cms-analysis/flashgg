#include "flashgg/MicroAOD/interface/CutBasedPhotonViewSelector.h"
#include "FWCore/Framework/interface/Event.h"

using namespace edm;
using namespace std;

namespace flashgg {
    CutBasedPhotonViewSelector::CutBasedPhotonViewSelector( const ParameterSet &cfg, edm::ConsumesCollector &cc )  :
        CutBasedPhotonObjectSelector( cfg, cc )
    {
    }

    bool CutBasedPhotonViewSelector::operator()( const edm::Ref<edm::View<SinglePhotonView> > candref, const edm::EventBase &ev ) const 
    {
        return operator()( *candref, ev );
    }
    
    bool CutBasedPhotonViewSelector::operator()( const SinglePhotonView &cand, const EventBase &ev ) const
    {
        return CutBasedPhotonObjectSelector::operator()( *cand.photon(), ev );
    }

}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

