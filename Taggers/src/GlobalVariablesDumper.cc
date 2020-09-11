#include "flashgg/Taggers/interface/GlobalVariablesDumper.h"

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"

#include "TTree.h"
//#include <limits>

using namespace edm;
using namespace reco;

namespace flashgg {

    void GlobalVariablesDumper::dumpLumiFactor(double lumiFactor) { 
        dumpLumiFactor_ = true; 
        lumiFactor_ = lumiFactor; 
    }

    // void GlobalVariablesDumper::bookNNLOPSweight(bool x) { 
    //     dumpNNLOPSweight_ = x; 
    // }

    // void GlobalVariablesDumper::setNNLOPSweight(double NNLOPSweight) { 
    //     NNLOPSweight_ = NNLOPSweight; 
    // }

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
            const auto extraFloatPSetNames_ = extraFloats.getParameterNamesForType<ParameterSet>();
            for(auto& extrafloatPSetName : extraFloatPSetNames_){
            //            for(int ipset=0; ipset< extraFloatPSets_.size(); ipset++){
            //  edm::ParameterSet extrafloatPSet=extraFloatPSets_[ipset];
                edm::ParameterSet extrafloatPSet= extraFloats.getParameter<ParameterSet>(extrafloatPSetName);
                extraFloatNames_.push_back( extrafloatPSetName );;
                const auto nbins  = extrafloatPSet.existsAs<int >("nbins") ? extrafloatPSet.getParameter<int >("nbins") : 0;
                const auto vmin   = extrafloatPSet.existsAs<double >("vmin") ? extrafloatPSet.getParameter<double >("vmin") : std::numeric_limits<double>::lowest();
                const auto vmax   = extrafloatPSet.existsAs<double >("vmax") ? extrafloatPSet.getParameter<double >("vmax") : std::numeric_limits<double>::max();
                //                const auto vmin   = extrafloatPSet.existsAs<double >("vmin") ? extrafloatPSet.getParameter<double >("vmin") : -999999.;
                //                const auto vmax   = extrafloatPSet.existsAs<double >("vmax") ? extrafloatPSet.getParameter<double >("vmax") : 999999.;
                const auto binning =  extrafloatPSet.existsAs<std::vector<double > >("binning") ? extrafloatPSet.getParameter<std::vector<double> >("binning") : std::vector<double >();

                extraFloatNBins_.insert( std::pair<std::string,int>(extrafloatPSetName, nbins) );
                extraFloatVmins_.insert( std::pair<std::string,double>(extrafloatPSetName, vmin ) );
                extraFloatVmaxs_.insert( std::pair<std::string,double>(extrafloatPSetName, vmax ) );
                extraFloatBinnings_.insert( std::pair<std::string, std::vector<double > >(extrafloatPSetName, binning ) );
            //////          }
            //            extraFloatNames_ = extraFloats.getParameterNamesForType<InputTag>();
            //////            for( auto & name : extraFloatNames_ ) {
                extraFloatTokens_.push_back( cc.consumes<float>(extrafloatPSet.getParameter<InputTag>("src")) );
                extraDoubleTokens_.push_back( cc.consumes<double>(extrafloatPSet.getParameter<InputTag>("src")) );
                extraVectorFloatTokens_.push_back( cc.consumes<std::vector<float>>(extrafloatPSet.getParameter<InputTag>("src")) );
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
            //            const auto extraFloats = cfg.getParameter<ParameterSet>( "extraFloats" );
            //            extraFloatNames_ = extraFloats.getParameterNamesForType<InputTag>();
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
        // if( dumpNNLOPSweight_ ) { tree->Branch( "NNLOPSweight", &NNLOPSweight_ ); }
        //        for( size_t iextra = 0; iextra<extraFloatNames_.size(); ++iextra ) {
        //            tree->Branch( extraFloatNames_[iextra].c_str(), &extraFloatVariables_[iextra] );
        //        }
    }


    std::vector<std::string> GlobalVariablesDumper::getExtraFloatNames(){
        return extraFloatNames_;
    }
    
    int GlobalVariablesDumper::getExtraFloatNBin(std::string extrafloatname){
        if (extraFloatNBins_.find(extrafloatname) != extraFloatNBins_.end()){
            return extraFloatNBins_[extrafloatname];
        }
        else{
            return 0;
        }
    }
    
    double GlobalVariablesDumper::getExtraFloatVmin(std::string extrafloatname){
        if (extraFloatVmins_.find(extrafloatname) != extraFloatVmins_.end()){
            return extraFloatVmins_[extrafloatname];
        }
        else{
            //            return std::numeric_limits<double>::lowest();
            return -99.;
        }
    }

    double GlobalVariablesDumper::getExtraFloatVmax(std::string extrafloatname){
        if (extraFloatVmaxs_.find(extrafloatname) != extraFloatVmaxs_.end()){
            return extraFloatVmaxs_[extrafloatname];
        }
        else{
            //            return std::numeric_limits<double>::max();
            return 99.;
        }
    }
    
    float GlobalVariablesDumper::getExtraFloat(std::string varname){
        for( size_t iextra = 0; iextra<extraFloatNames_.size(); ++iextra ) {
            if(extraFloatNames_[iextra] == varname){
                return extraFloatVariables_[iextra];
            } 
        }
        return -9999.;

    }
    
    std::vector<double > GlobalVariablesDumper::getExtraFloatBinning(std::string extrafloatname){
        if (extraFloatBinnings_.find(extrafloatname) != extraFloatBinnings_.end()){
            return extraFloatBinnings_[extrafloatname];
        }
        else{
            return std::vector<double >();
        }
        
    }


    void GlobalVariablesDumper::fill( const EventBase &evt )
    {
        // std::cout << "[in Taggers/src/GlobalVariablesDumper.cc: void GlobalVariablesDumper::fill] " << std::endl;
        // cout << "[in Taggers/src/GlobalVariablesDumper.cc: void GlobalVariablesDumper::fill] " << endl;
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
                Handle<std::vector<float> > ihandle; 

                if( fullEvent ) { 
                    fullEvent->getByToken( extraVectorFloatTokens_[iextra], ihandle );
                } else {
                    evt.getByLabel( extraFloatTags_[iextra], ihandle );
                }
                // assert( ihandle->size() == 1 );
                
                if( ihandle->size() < 1 ) {
                    continue; }
                extraFloatVariables_[iextra] = (*ihandle)[0];
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
                    Handle<float> ihandle; 
                    if( fullEvent ) { 
                        fullEvent->getByToken( extraFloatTokens_[iextra], ihandle );
                    } else {
                        evt.getByLabel( extraFloatTags_[iextra], ihandle );
                    }

                    if(! ihandle.isValid()){
                        std::cout<<"missing extra float "<< extraFloatNames_[iextra] <<std::endl;
                        assert(0);
                    }

                    extraFloatVariables_[iextra] = *ihandle;
                }
            }
        }
        // std::cout << "[in Taggers/src/GlobalVariablesDumper.cc] - VERY END OF void GlobalVariablesDumper::fill " << std::endl;

    }


}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

