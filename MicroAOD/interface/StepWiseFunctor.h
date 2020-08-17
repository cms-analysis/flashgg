#ifndef flashgg_StepWiseFunctor_h
#define flashgg_StepWiseFunctor_h

#include <tuple>
#include <vector>
#include <string>

#include "CommonTools/Utils/interface/TFileDirectory.h"

#include "flashgg/Taggers/interface/StringHelpers.h"

#include "CommonTools/Utils/interface/StringObjectFunction.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

#include "TMVA/Reader.h"
#include "flashgg/Taggers/interface/GlobalVariablesDumper.h"

namespace flashgg {

    template<class ObjectT, class FunctorT = StringObjectFunction<ObjectT, true> >
    class StepWiseFunctor
    {
    public:
        typedef ObjectT object_type;
        typedef FunctorT functor_type;

        StepWiseFunctor( const edm::ParameterSet &cfg );
        ~StepWiseFunctor();

        float operator()( const object_type &obj ) const;

    private:

        functor_type functor_;
        std::vector<double> bins_;
        std::vector<double> vals_;
        double default_;

    };

    template<class F, class O>
    StepWiseFunctor<F, O>::StepWiseFunctor( const edm::ParameterSet &cfg ) :
        functor_( cfg.getParameter<std::string>( "var" ) ),
        bins_( cfg.getParameter<std::vector<double> >( "bins" ) ),
        vals_( cfg.getParameter<std::vector<double> >( "vals" ) ),
        default_( 0. )
    {
        if( cfg.exists( "default" ) ) { default_ = cfg.getParameter<double>( "default" ); }
    }

    template<class F, class O>
    StepWiseFunctor<F, O>::~StepWiseFunctor()
    {
    }

    template<class F, class O>
    float StepWiseFunctor<F, O>::operator()( const object_type &obj ) const
    {
        float ret = default_;
        float val = functor_( obj );
        if( val >= bins_[0] && val <= bins_.back() ) {
            for( size_t ibin = 1; ibin < bins_.size(); ++ibin ) {
                if( val <= bins_[ibin] ) {
                    ret = vals_[ibin - 1];
                    break;
                }
            }
        }
        return ret;
    }

}


#endif  // flashgg_StepWiseFunctor_h
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

