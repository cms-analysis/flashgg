#ifndef flashgg_CutBasedGenericObjectSelector_h
#define flashgg_CutBasedGenericObjectSelector_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CommonTools/Utils/interface/StringObjectFunction.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

#include "flashgg/MicroAOD/interface/StepWiseFunctor.h"
#include "flashgg/MicroAOD/interface/CutBasedClassifier.h"

#include "FWCore/Common/interface/EventBase.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"

#include <tuple>
#include <vector>
#include <map>

namespace flashgg {
    template<class InputCollection>
    class CutBasedGenericObjectSelector
    {

    public:
        typedef typename InputCollection::value_type T;
        typedef T object_type;
        typedef CutBasedClassifier<T> classifier_type;
        typedef StringObjectFunction<T> functor_type;
        typedef StepWiseFunctor<T> stepwise_functor_type;
        typedef StringCutObjectSelector<T, true> selector_type;
        typedef edm::Ref<InputCollection> ref_type;

        CutBasedGenericObjectSelector( const edm::ParameterSet &config, edm::ConsumesCollector &cc );

        bool operator()( const object_type &cand, const edm::EventBase &ev ) const;
        bool operator()( const ref_type candref, const edm::EventBase &ev ) const;

    protected:
        typedef std::pair<bool,double> limit_type;
        typedef std::shared_ptr<functor_type> functor_ptr;
        typedef std::shared_ptr<stepwise_functor_type> stepwise_functor_ptr;

        void handle( const edm::EventBase &ev ) const;
        bool pass( const object_type &pho ) const;
        bool passInverted( const object_type &pho ) const;

        classifier_type classifier_;
        std::vector<functor_type> functors_;
        std::map<std::string, std::vector<std::tuple<functor_ptr , functor_ptr, stepwise_functor_ptr, limit_type > > >  selections_;
        std::vector<int> ignoreCuts_, invertCuts_, applyCuts_;

    private:
        edm::EDGetTokenT<double> rhoToken_;
        mutable double rho_;

        selector_type selector_;

    };

}

#include "../src/CutBasedGenericObjectSelector.icc"

#endif // flashgg_CutBasedGenericObjectSelector_h
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

