#include "flashgg/MicroAOD/interface/GlobalVariablesComputer.h"

using namespace edm;
using namespace reco;

namespace flashgg {


    GlobalVariablesComputer::GlobalVariablesComputer( const edm::ParameterSet &cfg) :
        rhoTag_( cfg.getParameter<edm::InputTag>( "rho" ) ),
        vtxTag_( cfg.getParameter<edm::InputTag>( "vertexes" ) ),
        getPu_( cfg.exists("puInfo") )
    {
        if( getPu_ ) {
            puInfoTag_ = cfg.getParameter<edm::InputTag>("puInfo");
            puReWeight_ = ( cfg.exists("puReWeight") ? cfg.getParameter<bool>("puReWeight") : false );
            if( puReWeight_ ) { 
                puBins_ = cfg.getParameter<std::vector<double> >("puBins");
                puWeight_ = cfg.getParameter<std::vector<double> >("dataPu");
                auto mcpu = cfg.getParameter<std::vector<double> >("mcPu");
                assert( puWeight_.size() == mcpu.size() );
                assert( puWeight_.size() == puBins_.size()-1 );
                auto scl  = std::accumulate(mcpu.begin(),mcpu.end(),0.) / std::accumulate(puWeight_.begin(),puWeight_.end(),0.); // rescale input distribs to unit ara
                for( size_t ib = 0; ib<puWeight_.size(); ++ib ) { puWeight_[ib] *= scl / mcpu[ib]; }
                if( cfg.exists("useTruePu") ) { useTruePu_ = cfg.getParameter<bool>("useTruePu"); }
            }
        }
    }

    GlobalVariablesComputer::GlobalVariablesComputer( const edm::ParameterSet &cfg, edm::ConsumesCollector&& iC ) :
        GlobalVariablesComputer( cfg )
    {
        rhoToken_ = iC.consumes<double>( rhoTag_ );
        vtxToken_ = iC.consumes<VertexCollection>( vtxTag_ );
        puInfoToken_ = iC.consumes<std::vector<PileupSummaryInfo> >( puInfoTag_ );
    }

    float *GlobalVariablesComputer::addressOf( const std::string &varName )
    {
        if( varName == "rho" ) { return &cache_.rho; }
        return 0;
    }

    int GlobalVariablesComputer::indexOf( const std::string &varName )
    {
        if( varName == "rho" ) { return 0; }
        else if( varName == "nvtx" ) { return 1; }
        else if( varName == "run" ) { return 2; }
        else if( varName == "event" ) { return 3; }
        else if( varName == "lumi" ) { return 4; }
        else if( varName == "npu" ) { return 5; }
        else if( varName == "puweight" ) { return 6; }
        else if( varName == "processIndex" ) { return 7; }
        return -1;
    }

    float GlobalVariablesComputer::valueOf( const std::string &varName )
    {
        return valueOf( indexOf( varName ) );
    }

    float GlobalVariablesComputer::valueOf( int varIndex )
    {
        if( varIndex == 0 ) { return cache_.rho; }
        else if( varIndex == 1 ) { return ( float )cache_.nvtx; }
        else if( varIndex == 2 ) { return ( float )cache_.run; }
        else if( varIndex == 3 ) { return ( float )cache_.event; }
        else if( varIndex == 4 ) { return ( float )cache_.lumi; }
        else if( varIndex == 5 ) { return ( float )cache_.npu; }
        else if( varIndex == 6 ) { return ( float )cache_.puweight; }
        else if( varIndex == 7 ) { return ( float )cache_.processIndex; }
        return -1e+6;

    }

    //---FWLite wrapper
    void GlobalVariablesComputer::update( const EventBase &evt )
    {        
        evt.getByLabel( rhoTag_, rhoHandle_ );
        evt.getByLabel( vtxTag_, vertices_ );
        evt.getByLabel( puInfoTag_, puInfo_);

        cache_.event = evt.id().event();
        cache_.lumi = evt.id().luminosityBlock();
        cache_.run  = evt.id().run();
        cache_.rho = *rhoHandle_;
        cache_.nvtx = vertices_->size();

        computer( evt.isRealData() );
    }

    //---FullFramework wrapper
    void GlobalVariablesComputer::update( const Event &evt )
    {
        evt.getByToken( rhoToken_, rhoHandle_ );
        evt.getByToken( vtxToken_, vertices_ );
        evt.getByToken( puInfoToken_, puInfo_);

        cache_.event = evt.id().event();
        cache_.lumi = evt.id().luminosityBlock();
        cache_.run  = evt.id().run();
        cache_.rho = *rhoHandle_;
        cache_.nvtx = vertices_->size();
        
        computer( evt.isRealData() );
    }

    //---real computer
    void GlobalVariablesComputer::computer(bool isRealData)
    {        
        if( !isRealData && getPu_ ) {
            double truePu=0., obsPu=0.;
            for( auto & frame : *puInfo_ ) {
                if( frame.getBunchCrossing() == 0 ) {
                    truePu = frame.getTrueNumInteractions();
                    obsPu = frame.getPU_NumInteractions();
                    break;
                }
            }
            
            cache_.npu = ( useTruePu_ ? truePu : obsPu );
            if( puReWeight_ ) {
                if( cache_.npu <= puBins_.front() || cache_.npu >= puBins_.back() ) {
                    cache_.puweight = 0.;
                } else { 
                    int ibin = (std::lower_bound(puBins_.begin(), puBins_.end(), cache_.npu) - puBins_.begin()) -1;
                    cache_.puweight = puWeight_[ibin];
                }
            }
        }

    }
}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

