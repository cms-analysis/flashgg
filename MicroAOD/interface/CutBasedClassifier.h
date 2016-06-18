#ifndef flashgg_CutBasedClassifier_h
#define flashgg_CutBasedClassifier_h

#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

namespace flashgg {
    template <class T>
    class CutBasedClassifier
    {
    public:
        typedef StringCutObjectSelector<T, true> functor_type;

        CutBasedClassifier( const edm::ParameterSet &cfg )
        {
            auto categories = cfg.getParameter<std::vector<edm::ParameterSet> >( "categories" );

            for( auto &cat : categories ) {
                auto cut = cat.getParameter<std::string>( "cut" );
                auto name = cat.getUntrackedParameter<std::string>( "name", Form( "cat%lu", cuts_.size() ) );

                cuts_.push_back( std::make_pair( functor_type( cut ), name ) );
            }
        }

        std::pair<std::string, int> operator()( const T &obj ) const
        {
            for( auto &cut : cuts_ ) {
                if( cut.first( obj ) ) { return std::make_pair( cut.second, 0 ); }
            }
            return std::make_pair( "", 0 );
        }

    private:
        std::vector<std::pair<functor_type, std::string> > cuts_;

    };
}

#endif // flashgg_CutBasedClassifier_h
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

