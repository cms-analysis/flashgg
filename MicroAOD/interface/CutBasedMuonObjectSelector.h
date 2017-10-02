#ifndef flashgg_CutBasedMuonObjectSelector_h
#define flashgg_CutBasedMuonObjectSelector_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CommonTools/Utils/interface/StringObjectFunction.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

#include "flashgg/MicroAOD/interface/StepWiseFunctor.h"
#include "flashgg/MicroAOD/interface/CutBasedClassifier.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "flashgg/DataFormats/interface/Muon.h"

#include "flashgg/MicroAOD/interface/MuonUtils.h"

#include "FWCore/Common/interface/EventBase.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"

#include <tuple>
#include <vector>
#include <map>

namespace flashgg {
    class CutBasedMuonObjectSelector
    {

    public:
        //        typedef CutBasedClassifier<Muon> classifier_type;
        typedef StringObjectFunction<Muon> functor_type;
        typedef StepWiseFunctor<Muon> stepwise_functor_type;
        typedef StringCutObjectSelector<Muon, true> selector_type;

        CutBasedMuonObjectSelector( const edm::ParameterSet &config, edm::ConsumesCollector &cc );

        bool operator()( const Muon &cand, const edm::Event &ev ) const;
        //bool operator()( const Muon &cand ) const;
        
        bool operator()( const edm::Ref<edm::View<Muon> > candref, const edm::Event &ev ) const;
        
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
        EDGetTokenT<View<reco::Vertex> > vertexToken_;
        mutable flashgg::MuonUtils muonutil_;

    };

}

#endif // flashgg_CutBasedMuonObjectSelector_h
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

