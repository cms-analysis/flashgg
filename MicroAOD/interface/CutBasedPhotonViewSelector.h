#ifndef flashgg_CutBasedPhotonViewSelector_h
#define flashgg_CutBasedPhotonViewSelector_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CommonTools/Utils/interface/StringObjectFunction.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

#include "flashgg/MicroAOD/interface/StepWiseFunctor.h"
#include "flashgg/MicroAOD/interface/CutBasedClassifier.h"
#include "flashgg/DataFormats/interface/SinglePhotonView.h"

#include "FWCore/Common/interface/EventBase.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"

#include "CutBasedPhotonObjectSelector.h"

#include <tuple>
#include <vector>
#include <map>

namespace flashgg {
    class CutBasedPhotonViewSelector : public CutBasedPhotonObjectSelector
    {

    public:
        CutBasedPhotonViewSelector( const edm::ParameterSet &config, edm::ConsumesCollector &cc );

        bool operator()( const SinglePhotonView &cand, const edm::EventBase &ev ) const;
        bool operator()( const edm::Ref<edm::View<SinglePhotonView> > candref, const edm::EventBase &ev ) const;
    };

}

#endif // flashgg_CutBasedPhotonViewSelector_h
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

