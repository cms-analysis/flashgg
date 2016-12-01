#include "flashgg/Taggers/interface/GlobalVariablesDumper.h"

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"

#include "TTree.h"

using namespace edm;
using namespace reco;

namespace flashgg {

    void GlobalVariablesDumper::dumpLumiFactor(double lumiFactor) { 
        dumpLumiFactor_ = true; 
        lumiFactor_ = lumiFactor; 
    }

    GlobalVariablesDumper::GlobalVariablesDumper( const ParameterSet &cfg ) :
        GlobalVariablesComputer( cfg )
    {
        if( cfg.exists( "addTriggerBits" ) ) {
            const auto &trg = cfg.getParameter<ParameterSet>( "addTriggerBits" );
            triggerTag_ = trg.getParameter<InputTag>( "tag" );
        }
        if( cfg.exists( "extraFloats" ) ){
            const auto extraFloats = cfg.getParameter<ParameterSet>( "extraFloats" );
            extraFloatNames_ = extraFloats.getParameterNamesForType<InputTag>();
            for( auto & name : extraFloatNames_ ) {
                extraFloatTags_.push_back( extraFloats.getParameter<InputTag>(name) );
            }
        }
        _init(cfg);
    }


    GlobalVariablesDumper::GlobalVariablesDumper( const ParameterSet &cfg, edm::ConsumesCollector && cc ) :
        GlobalVariablesComputer( cfg, std::forward<edm::ConsumesCollector>(cc) )
    {
        if( cfg.exists( "addTriggerBits" ) ) {
            const auto &trg = cfg.getParameter<ParameterSet>( "addTriggerBits" );
            triggerToken_ = cc.consumes<TriggerResults>( trg.getParameter<InputTag>( "tag" ) );
        }
        if( cfg.exists( "extraFloats" ) ){
            const auto extraFloats = cfg.getParameter<ParameterSet>( "extraFloats" );
            extraFloatNames_ = extraFloats.getParameterNamesForType<InputTag>();
            for( auto & name : extraFloatNames_ ) {
                extraFloatTokens_.push_back( cc.consumes<float>(extraFloats.getParameter<InputTag>(name)) );
                extraDoubleTokens_.push_back( cc.consumes<double>(extraFloats.getParameter<InputTag>(name)) );
                extraVectorFloatTokens_.push_back( cc.consumes<std::vector<float>>(extraFloats.getParameter<InputTag>(name)) );
            }
        }
        _init(cfg);
    }


    void GlobalVariablesDumper::_init( const ParameterSet &cfg )
    {
        if( cfg.exists( "addTriggerBits" ) ) {
            const auto &trg = cfg.getParameter<ParameterSet>( "addTriggerBits" );
            auto bitNames   = trg.getParameter<std::vector<std::string> >( "bits" );
            for( auto &bit : bitNames ) {
                bits_.push_back( std::make_pair( bit, false ) );
            }
        }

        dumpLumiFactor_ = false;
        lumiFactor_ = -999.;
        processIndex_ = -999;

        if( cfg.exists( "extraFloats" ) ){
            const auto extraFloats = cfg.getParameter<ParameterSet>( "extraFloats" );
            extraFloatNames_ = extraFloats.getParameterNamesForType<InputTag>();
            extraFloatVariables_.resize(extraFloatNames_.size(),0.);
        }
    }


    GlobalVariablesDumper::~GlobalVariablesDumper()
    {}

    void GlobalVariablesDumper::bookTreeVariables( TTree *tree, const std::map<std::string, std::string> &replacements )
    {
        tree->Branch( "rho", &cache_.rho );
        tree->Branch( "nvtx", &cache_.nvtx );
        tree->Branch( "event", &cache_.event, "event/l" );
        tree->Branch( "lumi", &cache_.lumi, "lumi/i" );
        tree->Branch( "processIndex", &processIndex_, "processIndex/I" );
        tree->Branch( "run", &cache_.run, "run/i" );
        tree->Branch( "nvtx", &cache_.nvtx );
        if( getPu_ ) {
            tree->Branch( "npu", &cache_.npu );
        }
        // create the output branch puweight even for data
        // so we can more easily merge different types
        // of processes into one tree
        tree->Branch( "puweight", &cache_.puweight );

        for( auto &bit : bits_ ) {
            tree->Branch( bit.first.c_str(), &bit.second, ( bit.first + "/O" ).c_str() );
        }
        if( dumpLumiFactor_ ) { tree->Branch( "lumiFactor", &lumiFactor_ ); }
        //        for( size_t iextra = 0; iextra<extraFloatNames_.size(); ++iextra ) {
        //            tree->Branch( extraFloatNames_[iextra].c_str(), &extraFloatVariables_[iextra] );
        //        }
    }


    std::vector<std::string> GlobalVariablesDumper::getExtraFloatNames(){
        return extraFloatNames_;
    }

    float GlobalVariablesDumper::getExtraFloat(std::string varname){
        for( size_t iextra = 0; iextra<extraFloatNames_.size(); ++iextra ) {
            if(extraFloatNames_[iextra] == varname){
                return extraFloatVariables_[iextra];
            } 
        }
        return -9999.;

    }
    

    void GlobalVariablesDumper::fill( const EventBase &evt )
    {
        update( evt );
        const edm::Event * fullEvent = dynamic_cast<const edm::Event *>(&evt);
        if( ! bits_.empty() ) {
            Handle<TriggerResults> trigResults; //our trigger result object
            if (fullEvent != 0) {
                fullEvent->getByToken(triggerToken_, trigResults);
            } else {
                evt.getByLabel( triggerTag_, trigResults );
            }

            for( auto &bit : bits_ ) { bit.second = false; }
            auto &trigNames = evt.triggerNames( *trigResults );
            for( size_t itrg = 0; itrg < trigNames.size(); ++itrg ) {
                if( ! trigResults->accept( itrg ) ) { continue; }
                auto pathName = trigNames.triggerName( itrg );
                for( auto &bit : bits_ ) {
                    if( pathName.find( bit.first ) != std::string::npos ) {
                        bit.second = true;
                        break;
                    }
                }
            }
        }
        /// for( size_t iextra = 0; iextra<extraFloatTags_.size(); ++iextra ) {
        for( size_t iextra = 0; iextra<extraFloatNames_.size(); ++iextra ) {
            try {
                Handle<float> ihandle; 
                if( fullEvent ) { 
                    fullEvent->getByToken( extraFloatTokens_[iextra], ihandle );
                } else {
                    evt.getByLabel( extraFloatTags_[iextra], ihandle );
                }
                extraFloatVariables_[iextra] = *ihandle;
            } catch (...) {
                try{
                    Handle<double> ihandle; 
                    if( fullEvent ) { 
                        fullEvent->getByToken( extraDoubleTokens_[iextra], ihandle );
                    } else {
                        evt.getByLabel( extraFloatTags_[iextra], ihandle );
                    }
                    extraFloatVariables_[iextra] = *ihandle;
                } catch (...) {
                    Handle<std::vector<float> > ihandle; 
                    if( fullEvent ) { 
                        fullEvent->getByToken( extraVectorFloatTokens_[iextra], ihandle );
                    } else {
                        evt.getByLabel( extraFloatTags_[iextra], ihandle );
                    }
                    // assert( ihandle->size() == 1 );
                    if( ihandle->size()  < 1 ) { std::cout << "NO extra float......... " << extraFloatTags_[iextra].label() << std::endl; continue; }
                    extraFloatVariables_[iextra] = (*ihandle)[0];
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

