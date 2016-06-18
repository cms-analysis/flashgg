#ifndef flashgg_PhotonMVAComputer_h
#define flashgg_PhotonMVAComputer_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CommonTools/Utils/interface/StringObjectFunction.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

#include "flashgg/MicroAOD/interface/CutBasedClassifier.h"
#include "flashgg/DataFormats/interface/Photon.h"

#include "FWCore/Common/interface/EventBase.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"

#include "flashgg/MicroAOD/interface/GlobalVariablesComputer.h"
#include "flashgg/MicroAOD/interface/MVAComputer.h"

#include <tuple>
#include <vector>
#include <map>

namespace flashgg {
    class PhotonMVAComputer : public GlobalVariablesComputer
    {

    public:
        typedef CutBasedClassifier<Photon> classifier_type;
        typedef StringCutObjectSelector<Photon> selector_type;
        typedef MVAComputer<Photon> mva_type;

        PhotonMVAComputer( const edm::ParameterSet &config, edm::ConsumesCollector &&cc );
        PhotonMVAComputer( const edm::ParameterSet &config );
        ~PhotonMVAComputer();

        void fill( Photon & );

    private:
        classifier_type classifier_;
        selector_type selector_;
        // mva_names, default
        std::vector<std::tuple<std::string, float> > toFill_;
        // category -> vector< <min,max,rhocorr> >
        std::map<std::string, std::vector<mva_type *> >  mvas_;

    };

}

#endif // flashgg_PhotonMVAComputer_h
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

