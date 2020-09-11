#ifndef flashgg_GlobalVariablesDumper_h
#define flashgg_GlobalVariablesDumper_h

#include "FWCore/Common/interface/EventBase.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "flashgg/MicroAOD/interface/GlobalVariablesComputer.h"

class TTree;

namespace flashgg {

    class GlobalVariablesDumper : public GlobalVariablesComputer
    {
    public:
        GlobalVariablesDumper( const edm::ParameterSet &cfg );
        GlobalVariablesDumper( const edm::ParameterSet &cfg, edm::ConsumesCollector && cc );
        ~GlobalVariablesDumper();

        void bookTreeVariables( TTree *target, const std::map<std::string, std::string> &replacements );

        void fill( const edm::EventBase &event );
        
        void dumpLumiFactor(double lumiFactor);

        std::vector<std::string> getExtraFloatNames();
        float getExtraFloat(std::string varname);
        int getExtraFloatNBin(std::string extrafloatname);
        double getExtraFloatVmin(std::string extrafloatname);
        double getExtraFloatVmax(std::string extrafloatname);
        std::vector<double > getExtraFloatBinning(std::string varname);
        void setProcessIndex(int processIndex) {processIndex_= processIndex;}
        void bookNNLOPSweight(bool x);
        void setNNLOPSweight(double NNLOPSweight);


    private:

        void _init( const edm::ParameterSet &cfg );

        edm::InputTag triggerTag_;
        edm::EDGetTokenT<edm::TriggerResults> triggerToken_;
        std::vector<std::pair<std::string, bool>> bits_;
        
        bool dumpLumiFactor_;
        double lumiFactor_;
        int processIndex_;
        // bool dumpNNLOPSweight_;
        // double NNLOPSweight_;

        std::vector<edm::EDGetTokenT<float>> extraFloatTokens_;
        std::vector<edm::EDGetTokenT<std::vector<float>>> extraVectorFloatTokens_;

        std::vector<edm::EDGetTokenT<double>> extraDoubleTokens_;

        std::vector<edm::InputTag> extraFloatTags_;
        std::vector<std::string> extraFloatNames_;
        std::vector<edm::ParameterSet> extraFloatPSets_;
        std::map<std::string, int> extraFloatNBins_;
        std::map<std::string, double> extraFloatVmins_;
        std::map<std::string, double> extraFloatVmaxs_;
        std::map<std::string, std::vector<double > > extraFloatBinnings_;
        std::vector<float> extraFloatVariables_;
        
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

