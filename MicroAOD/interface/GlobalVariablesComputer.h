#ifndef flashgg_GlobalVariablesComputer_h
#define flashgg_GlobalVariablesComputer_h

#include "FWCore/Common/interface/EventBase.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

// #include "PhysicsTools/Utilities/interface/LumiReWeighting.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

namespace flashgg {

    class GlobalVariablesComputer
    {
    public:
        struct cache_t {
            float rho;
            int   nvtx;
            unsigned int  event;
            unsigned char lumi;
            unsigned int  run;
            float npu;
            float puweight;
            int processIndex;
        };

        GlobalVariablesComputer( const edm::ParameterSet &cfg );
        GlobalVariablesComputer( const edm::ParameterSet &cfg, edm::ConsumesCollector&& iC );

        void update( const edm::EventBase &event );
        void update( const edm::Event &event );
        void computer(bool isRealData);

        float *addressOf( const std::string &varName );
        int indexOf( const std::string &varName );
        float valueOf( const std::string &varName );
        float valueOf( int varIndex );

        const cache_t & cache() const { return cache_; }
        
        bool puReWeight() const { return puReWeight_; }
        
    protected:
        edm::EDGetTokenT<double> rhoToken_;
        edm::EDGetTokenT<reco::VertexCollection> vtxToken_;
        edm::EDGetTokenT<std::vector<PileupSummaryInfo> > puInfoToken_;
        edm::InputTag rhoTag_, vtxTag_;
        bool getPu_, puReWeight_, useTruePu_;
        std::vector<double> puBins_;
        std::vector<double> puWeight_;
        edm::InputTag puInfoTag_;

        edm::Handle<double> rhoHandle_;
        edm::Handle<reco::VertexCollection> vertices_;
        edm::Handle<std::vector<PileupSummaryInfo> > puInfo_;
        
        cache_t cache_;
    };
}

#endif // flashgg_GlobalVariablesComputer_h
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

