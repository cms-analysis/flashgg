#ifndef flashgg_GlobalVariablesComputer_h
#define flashgg_GlobalVariablesComputer_h

#include "FWCore/Common/interface/EventBase.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

namespace flashgg {

    class GlobalVariablesComputer
    {
    public:
        struct cache_t {
            float rho;
            int   nvtx;
            unsigned long long event;
            unsigned int lumi;
            unsigned int  run;
            float npu;
            float puweight;
            int processIndex;
        };

        GlobalVariablesComputer( const edm::ParameterSet &cfg );
        GlobalVariablesComputer( const edm::ParameterSet &cfg, edm::ConsumesCollector &cc );
        GlobalVariablesComputer( const edm::ParameterSet &cfg, edm::ConsumesCollector &&cc );

        void update( const edm::EventBase &event );

        float *addressOf( const std::string &varName );
        int indexOf( const std::string &varName ) const ;
        float valueOf( const std::string &varName ) const ;
        float valueOf( int varIndex ) const;

        const cache_t & cache() const { return cache_; }
        
        bool puReWeight() const { return puReWeight_; }
        
    protected:

        void _init( const edm::ParameterSet & cfg );
        
        bool doRho_, doVtx_;
        edm::InputTag rhoTag_, vtxTag_, puInfo_;
        edm::EDGetTokenT<double> rhoToken_;
        edm::EDGetTokenT<reco::VertexCollection> vtxToken_;
        edm::EDGetTokenT<std::vector<PileupSummaryInfo> > puInfoToken_;
        bool getPu_, puReWeight_, useTruePu_;
        std::vector<double> puBins_;
        std::vector<double> puWeight_;

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

