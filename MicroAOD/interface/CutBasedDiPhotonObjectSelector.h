#ifndef flashgg_CutBasedDiPhotonObjectSelector_h
#define flashgg_CutBasedDiPhotonObjectSelector_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CommonTools/Utils/interface/StringObjectFunction.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

#include "flashgg/MicroAOD/interface/CutBasedClassifier.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"

#include "FWCore/Common/interface/EventBase.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"

#include <tuple>
#include <vector>
#include <map>

namespace flashgg {
    class CutBasedDiPhotonObjectSelector
    {

    public:
        typedef CutBasedClassifier<Photon> classifier_type;
        typedef StringObjectFunction<Photon> functor_type;
        typedef StringCutObjectSelector<DiPhotonCandidate> selector_type;

        CutBasedDiPhotonObjectSelector( const edm::ParameterSet &config, edm::ConsumesCollector &cc );

        bool operator()( const DiPhotonCandidate &cand, const edm::EventBase &ev ) const;

    private:
        bool pass( const Photon &pho ) const;
        edm::EDGetTokenT<double> rhoToken_;
        mutable double rho_;

        classifier_type classifier_;
        std::vector<functor_type> functors_;
        selector_type selector_;
        // category -> vector< <min,max,rhocorr> >
        std::map<std::string, std::vector<std::tuple<functor_type, functor_type, functor_type> > >  selections_;

    };

}

#endif // flashgg_CutBasedDiPhotonObjectSelector_h
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

