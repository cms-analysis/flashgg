#ifndef flashgg_CutAndClassBasedClassifier_h
#define flashgg_CutAndClassBasedClassifier_h

#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

namespace flashgg {
    template <class T>
    class CutAndClassBasedClassifier
    {
    public:
        typedef StringCutObjectSelector<T, true> functor_type;

        CutAndClassBasedClassifier( const edm::ParameterSet &cfg )
        {
            auto categories = cfg.getParameter<std::vector<edm::ParameterSet> >( "categories" );

            for( auto &cat : categories ) {
                auto cut = cat.getParameter<std::string>( "cut" );
                // auto syst = cat.getParameter<std::string>( "systLabel" );
                auto name = cat.getUntrackedParameter<std::string>( "name", Form( "cat%lu", cuts_.size() ) );
                // auto name = cat.getUntrackedParameter<std::string>( "systLabel", Form( "cat%lu", cuts_.size() ) );
                cuts_.push_back( std::make_pair( functor_type( cut ), name ) );
            }

            auto remap = cfg.getUntrackedParameter<std::vector<edm::ParameterSet> >( "remap", std::vector<edm::ParameterSet>() );
            for( auto &rm : remap ) {
                remap_.insert( std::make_pair( rm.getUntrackedParameter<std::string>( "src" ),
                                               rm.getUntrackedParameter<std::string>( "dst" )
                                             ) );
            }
        }

        std::pair< std::pair<std::string, std::string >, int> operator()( const T &obj ) const
        {
            auto cutbasedpair = std::make_pair( "NOTFOUND", 0 );
            auto classbasedpair = std::make_pair( "NOTFOUND", 0 );
            int counter = 0;

            for( auto &cut : cuts_ ) {
                if( cut.first( obj ) ) {
                    cutbasedpair = std::make_pair( cut.second.c_str(), 0 );
                }
                counter++;
            }

            int id = ( int )obj;
            std::type_index idx( typeid( obj ) );
            auto cached = cache_.find( idx );
            if( cached != cache_.end() ) { classbasedpair = std::make_pair( cached->second.c_str(), id ); }
            auto ret = std::make_pair( edm::stripNamespace( edm::TypeID( obj ).friendlyClassName() ).c_str(), id );
            auto rm = remap_.find( ret.first );
            if( rm != remap_.end() ) { ret.first = rm->second.c_str(); }
            cache_.insert( std::make_pair( idx, ret.first ) );
            classbasedpair = ret;

            auto strings = std::make_pair( classbasedpair.first, cutbasedpair.first );
            auto res = std::make_pair( strings, classbasedpair.second );
            return res;

        }

    private:
        std::vector<std::pair<functor_type, std::string> > cuts_;
        std::map<std::string, std::string> remap_;
        mutable std::unordered_map<std::type_index, std::string> cache_;

    };
}

#endif // flashgg_CutAndClassBasedClassifier_h
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

