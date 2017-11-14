#ifndef flashgg_CutBasedPhotonObjectSelector_h
#define flashgg_CutBasedPhotonObjectSelector_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CommonTools/Utils/interface/StringObjectFunction.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

#include "flashgg/MicroAOD/interface/StepWiseFunctor.h"
#include "flashgg/MicroAOD/interface/CutBasedClassifier.h"
#include "flashgg/DataFormats/interface/Photon.h"

#include "FWCore/Common/interface/EventBase.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"

#include <tuple>
#include <vector>
#include <unordered_map>
#include <map>

namespace flashgg {
    class CutBasedPhotonObjectSelector
    {

    public:
        typedef CutBasedClassifier<Photon> classifier_type;
        typedef StringObjectFunction<Photon> functor_type;
        typedef StepWiseFunctor<Photon> stepwise_functor_type;
        typedef StringCutObjectSelector<Photon, true> selector_type;

        CutBasedPhotonObjectSelector( const edm::ParameterSet &config, edm::ConsumesCollector &cc );

        bool operator()( const Photon &cand, const edm::EventBase &ev ) const;
        bool operator()( const edm::Ref<edm::View<Photon> > candref, const edm::EventBase &ev ) const;
        const std::unordered_map<std::string, bool>& computeSelections( const Photon &pho, const edm::Event &ev );
        
    protected:
        typedef std::shared_ptr<functor_type> functor_ptr;
        typedef std::shared_ptr<stepwise_functor_type> stepwise_functor_ptr;

        void handle( const edm::EventBase &ev ) const;
        bool pass( const Photon &pho ) const;
        bool passInverted( const Photon &pho ) const;

        classifier_type classifier_;
        std::vector<functor_type> functors_;
        // category -> vector< <min,max,rhocorr> >
        // std::map<std::string, std::vector<std::tuple<functor_type, functor_type, functor_type> > >  selections_
        std::map<std::string, std::vector<std::tuple<functor_ptr , functor_ptr, stepwise_functor_ptr > > >  selections_;
        std::vector<int> ignoreCuts_, invertCuts_, applyCuts_;
        std::vector<string> cutsNames_;
        std::unordered_map<std::string, bool> cutsResults_;

    private:
        edm::EDGetTokenT<double> rhoToken_;
        mutable double rho_;

        selector_type selector_;

    };

}

#endif // flashgg_CutBasedPhotonObjectSelector_h
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

