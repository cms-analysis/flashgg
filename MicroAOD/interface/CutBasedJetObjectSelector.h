#ifndef flashgg_CutBasedJetObjectSelector_h
#define flashgg_CutBasedJetObjectSelector_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CommonTools/Utils/interface/StringObjectFunction.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

#include "flashgg/MicroAOD/interface/StepWiseFunctor.h"
#include "flashgg/MicroAOD/interface/CutBasedClassifier.h"
#include "flashgg/DataFormats/interface/Jet.h"

#include "flashgg/MicroAOD/interface/JetUtils.h"

#include "FWCore/Common/interface/EventBase.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"

#include <tuple>
#include <vector>
#include <map>

namespace flashgg {
    class CutBasedJetObjectSelector
    {

    public:
        //        typedef CutBasedClassifier<Jet> classifier_type;
        typedef StringObjectFunction<Jet> functor_type;
        typedef StepWiseFunctor<Jet> stepwise_functor_type;
        typedef StringCutObjectSelector<Jet, true> selector_type;

        CutBasedJetObjectSelector( const edm::ParameterSet &config, edm::ConsumesCollector &cc );

        bool operator()( const Jet &cand, const edm::EventBase &ev ) const;
        //bool operator()( const Jet &cand ) const;
        
        bool operator()( const edm::Ref<edm::View<Jet> > candref, const edm::EventBase &ev ) const;
        
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

        mutable flashgg::JetUtils jetutil_;

    };

}

#endif // flashgg_CutBasedJetObjectSelector_h
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

