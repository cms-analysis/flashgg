#ifndef flashgg_GlobalVariablesDumper_h
#define flashgg_GlobalVariablesDumper_h

#include "FWCore/Common/interface/EventBase.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "flashgg/MicroAOD/interface/GlobalVariablesComputer.h"

class TTree;

namespace flashgg {

    class GlobalVariablesDumper : public GlobalVariablesComputer
    {
    public:
        GlobalVariablesDumper( const edm::ParameterSet &cfg );
        ~GlobalVariablesDumper();

        void bookTreeVariables( TTree *target, const std::map<std::string, std::string> &replacements );

        void fill( const edm::EventBase &event );

    private:
        edm::InputTag triggerTag_;
        std::vector<std::pair<std::string, bool>> bits_;

    };

}

#endif  // flashgg_GlobalVariablesDumper_h
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

