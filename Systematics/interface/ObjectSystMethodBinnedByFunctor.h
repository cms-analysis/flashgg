#ifndef FLASHgg_ObjectSystMethodBinnedByFunctor_h
#define FLASHgg_ObjectSystMethodBinnedByFunctor_h

#include "flashgg/Systematics/interface/BaseSystMethod.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Handle.h"
#include "CommonTools/Utils/interface/StringObjectFunction.h"

namespace flashgg {

    template <class flashgg_object, class param_var>
    class ObjectSystMethodBinnedByFunctor : public BaseSystMethod<flashgg_object, param_var>
    {

    public:
        typedef StringObjectFunction<flashgg_object> functor_type;

        struct Bin {
            std::vector<double> min; // length: number of variables
            std::vector<double> max; // length: number of variables
            std::vector<double> val; // length: number of parameters (usually 1 or 2)
            std::vector<double> unc; // length: same as val (or 2x, for asymmetric errors)

            Bin( std::vector<double> mi, std::vector<double> ma, std::vector<double> va, std::vector<double> er ) :
                min( mi ), max( ma ), val( va ), unc( er ) {}
        };

        ObjectSystMethodBinnedByFunctor( const edm::ParameterSet &conf ) :
            BaseSystMethod<flashgg_object, param_var>::BaseSystMethod( conf ),
            debug_( conf.getUntrackedParameter<bool>( "Debug", false ) )
        {
            const auto &pset = conf.getParameterSet( "BinList" );
            for( const auto variable : pset.getParameter<std::vector<std::string> >( "variables" ) ) {
                functors_.emplace_back( variable );
            }

            for( const auto &b : pset.getParameterSetVector( "bins" ) ) {
                bins_.emplace_back( b.getParameter<std::vector<double> >( "lowBounds" ),
                                    b.getParameter<std::vector<double> >( "upBounds" ),
                                    b.getParameter<std::vector<double> >( "values" ),
                                    b.getParameter<std::vector<double> >( "uncertainties" ) );

            }
        }

        ObjectSystMethodBinnedByFunctor() {};
        virtual ~ObjectSystMethodBinnedByFunctor() {};

        std::pair<std::vector<double>, std::vector<double> > binContents( const flashgg_object &y )
        {
            std::vector<double> func_vals;
            for( const auto &func : functors_ ) { func_vals.push_back( func( y ) ); }
            for( const auto &bin : bins_ ) {
                bool found = true;
                for( unsigned int i = 0 ; i < func_vals.size() ; i++ ) {
                    if( func_vals[i] < bin.min[i] || func_vals[i] >= bin.max[i] ) {
                        found = false;
                        break;
                    }
                }
                if( found ) {
                    return std::make_pair( bin.val, bin.unc );
                }
            }
            throw cms::Exception( "Binning" ) << " binContents failed and would return a pair of empty vectors";
            return std::make_pair( std::vector<double>(), std::vector<double>() ); // this is bad
        }

    protected:
        bool debug_;

    private:
        std::vector<Bin> bins_; // length: number of bins
        std::vector<functor_type> functors_; // length: number of variables

    };
}

#endif
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

