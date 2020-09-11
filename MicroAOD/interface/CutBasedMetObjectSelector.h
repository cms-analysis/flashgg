#ifndef flashgg_CutBasedMetObjectSelector_h
#define flashgg_CutBasedMetObjectSelector_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CommonTools/Utils/interface/StringObjectFunction.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

#include "flashgg/MicroAOD/interface/StepWiseFunctor.h"
#include "flashgg/MicroAOD/interface/CutBasedClassifier.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "flashgg/DataFormats/interface/Met.h"

#include "flashgg/MicroAOD/interface/MetUtils.h"

#include "FWCore/Common/interface/EventBase.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"

#include <tuple>
#include <vector>
#include <map>

namespace flashgg {
    class CutBasedMetObjectSelector
    {

    public:
        //        typedef CutBasedClassifier<Met> classifier_type;
        typedef StringObjectFunction<Met> functor_type;
        typedef StepWiseFunctor<Met> stepwise_functor_type;
        typedef StringCutObjectSelector<Met, true> selector_type;

        CutBasedMetObjectSelector( const edm::ParameterSet &config, edm::ConsumesCollector &cc );

        bool operator()( const Met &cand, const edm::Event &ev ) const;
        //bool operator()( const Met &cand ) const;
        
        bool operator()( const edm::Ref<edm::View<Met> > candref, const edm::Event &ev ) const;
        
    protected:
        typedef std::shared_ptr<functor_type> functor_ptr;
        typedef std::shared_ptr<stepwise_functor_type> stepwise_functor_ptr;

        void handle( const edm::EventBase &ev ) const;

        //        classifier_type classifier_;
        std::vector<functor_type> functors_;
        // category -> vector< <min,max,rhocorr> >
        // std::map<std::string, std::vector<std::tuple<functor_type, functor_type, functor_type> > >  selections_
        std::map<std::string, std::vector<std::tuple<functor_ptr , functor_ptr, stepwise_functor_ptr > > >  selections_;
        std::vector<int> ignoreCuts_, invertCuts_, applyCuts_;

    private:
        //        edm::EDGetTokenT<double> rhoToken_;
        //        mutable double rho_;

        selector_type selector_;
        mutable flashgg::MetUtils metutil_;
        edm::EDGetTokenT<edm::TriggerResults> triggerRECO_;
        edm::EDGetTokenT<edm::TriggerResults> triggerPAT_;




    };

}

#endif // flashgg_CutBasedMetObjectSelector_h
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

