#ifndef flashgg_StageOneNameClassifier_h
#define flashgg_StageOneNameClassifier_h

#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

#include "FWCore/Utilities/interface/TypeID.h"
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <iostream>
#include <boost/type_index.hpp>

namespace flashgg {

    template <class T>
    class StageOneNameClassifier
    {
    public:
        StageOneNameClassifier( const edm::ParameterSet &cfg )
        {
            auto remap = cfg.getUntrackedParameter<std::vector<edm::ParameterSet> >( "remap", std::vector<edm::ParameterSet>() );
            for( auto &rm : remap ) {
                remap_.insert( std::make_pair( rm.getUntrackedParameter<std::string>( "src" ),
                                               rm.getUntrackedParameter<std::string>( "dst" )
                                             ) );
            }
        }

        std::pair<std::string, int> operator()( const T &obj ) const
        {
            int id = ( int )obj;
            //std::type_index idx( typeid( obj ) );
            int idx = obj.getStage1recoTag();
            auto cached = cache_.find( idx );
            if( cached != cache_.end() ) { return std::make_pair( cached->second, id ); }
            auto ret = std::make_pair( obj.stage1KinematicLabel(), id );
            auto rm = remap_.find( ret.first );
            if( rm != remap_.end() ) { ret.first = rm->second; }
            cache_.insert( std::make_pair( idx, ret.first ) );
            return ret;
        }


    private:
        std::map<std::string, std::string> remap_;
        //mutable std::unordered_map<std::type_index, std::string> cache_;
        mutable std::unordered_map<int, std::string> cache_;
    };
}

#endif
