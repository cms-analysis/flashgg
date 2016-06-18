#ifndef FLASHgg_ObjectSystMethodBinnedByFunctor_h
#define FLASHgg_ObjectSystMethodBinnedByFunctor_h

#include "flashgg/Systematics/interface/BaseSystMethod.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Handle.h"
#include "CommonTools/Utils/interface/StringObjectFunction.h"

#include "flashgg/MicroAOD/interface/GlobalVariablesComputer.h"

namespace flashgg {

    template <class flashgg_object> class ObjectFunctorTrait
    {
    public:
        virtual double eval(const flashgg_object & obj) const = 0;
    };
    
    template<class flashgg_object> class WrappedGlobalVariablesComputer : public ObjectFunctorTrait<flashgg_object>
    {
    public:
        WrappedGlobalVariablesComputer(const flashgg::GlobalVariablesComputer & gv, const std::string & var) : gv_(gv), globalIndex_(gv_.indexOf(var))  { 
        }
        
        double eval(const flashgg_object & obj) const { return gv_.valueOf(globalIndex_); }
        
    private:
        const flashgg::GlobalVariablesComputer & gv_;
        
        int globalIndex_;
        
    };
        

    template<class flashgg_object> class WrappedStringObjectFunctor : public ObjectFunctorTrait<flashgg_object>, StringObjectFunction<flashgg_object>
    {
    public:
        WrappedStringObjectFunctor(const std::string & expr) : StringObjectFunction<flashgg_object>(expr,true) {};
        
        double eval(const flashgg_object & obj) const { return this->operator()(obj); };
    };

        
    template <class flashgg_object, class param_var>
    class ObjectSystMethodBinnedByFunctor : public BaseSystMethod<flashgg_object, param_var>
    {

    public:
        typedef ObjectFunctorTrait<flashgg_object> functor_type;
        
        struct Bin {
            std::vector<double> min; // length: number of variables
            std::vector<double> max; // length: number of variables
            std::vector<double> val; // length: number of parameters (usually 1 or 2)
            std::vector<double> unc; // length: same as val (or 2x, for asymmetric errors)

            Bin( std::vector<double> mi, std::vector<double> ma, std::vector<double> va, std::vector<double> er ) :
                min( mi ), max( ma ), val( va ), unc( er ) {}
        };
        ObjectSystMethodBinnedByFunctor( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer * globalVariables ) :
            BaseSystMethod<flashgg_object, param_var>::BaseSystMethod( conf, std::forward<edm::ConsumesCollector>(iC) ),
            debug_( conf.getUntrackedParameter<bool>( "Debug", false ) )
        {
            const auto &pset = conf.getParameterSet( "BinList" );
            for( const auto variable : pset.getParameter<std::vector<std::string> >( "variables" ) ) {
                auto pos = variable.find( "global." );
                if( pos == 0 ) {
                    if( globalVariables == 0 ) {
                        throw cms::Exception( "Binning" ) << " Global-variables-based binning required but no GlobalVariablesComputer is available.";
                    }
                    functors_.emplace_back( new WrappedGlobalVariablesComputer<flashgg_object>(*globalVariables,variable.substr(7)));
                } else {
                    functors_.emplace_back( new WrappedStringObjectFunctor<flashgg_object>(variable));
                }
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

        std::pair<std::vector<int>, std::vector<Bin> > adjacentBins( const flashgg_object &y )
        {
            std::vector<double> func_vals;//vector storing the structures of type Bin for later access .
            std::vector<int> binIndices;//vector stroing the indices of adjacent bins. Need the indices to know if the been is one the boundary or if it is the last bin.
            std::vector<Bin> bins;
            int num_bins = bins_.size();
            int myLowerBin = 0;
            int myUpperBin = 0;

            for( auto func : functors_ ) { func_vals.push_back( func->eval( y ) ); }
            for( int bin = 0 ; bin < num_bins ; bin++ ) {
                bool found = true;
                for( unsigned int i = 0; i < func_vals.size() ; i++ ) {
                    if( func_vals[i] < bins_[0].min[i] ) {
                        found = false;//if flashgg object is below the lower end of the efficiency .
                        myLowerBin = 0;
                        myUpperBin = 0;
                        break;
                    }
                    if( func_vals[i] > bins_[num_bins - 1].max[i] ) {
                        found = false;//if flashgg object is above the upper end of the efficiency .
                        myLowerBin = num_bins - 1;
                        myUpperBin = num_bins - 1;
                        break;
                    }
                    if( func_vals[i] < bins_[bin].min[i] || func_vals[i] > bins_[bin].max[i] ) {
                        found = false;//finds keeps making found false until you find the correct bin, this would make the if argument false .
                        break;
                    }
                }
                if( found ) {

                    if( bin == num_bins - 1 ) {
                        myLowerBin = bin;//if the found bin is the last bin, this is also at the boundary.
                        myUpperBin = bin;

                    } else {
                        myLowerBin = bin;//indices of adjacent bins .
                        myUpperBin = bin + 1;
                    }
                }

            }

            binIndices.push_back( myLowerBin ); //fill the vectors
            binIndices.push_back( myUpperBin );

            bins.push_back( bins_[myLowerBin] );
            bins.push_back( bins_[myUpperBin] );

            return std::make_pair( binIndices, bins ); //return the pair of vectors, the vectors are ordered from lower bins to upper bins.

        }

        std::pair<std::vector<double>, std::vector<double> > binContents( const flashgg_object &y )
        {
            std::vector<double> func_vals;
            for( const auto &func : functors_ ) { func_vals.push_back( func->eval( y ) ); }
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
            std::stringstream str;
            std::copy(func_vals.begin(),func_vals.end(),std::ostream_iterator<double>(str,","));
            throw cms::Exception( "Binning" ) << " binContents failed for method " << this->name() << ", label " << this->label() << ", shiftLabel " << this->shiftLabel(param_var()) << ", would return a pair of empty vectors " << str.str();
            return std::make_pair( std::vector<double>(), std::vector<double>() ); // this is bad
        }

    protected:
        bool debug_;
        std::vector<std::shared_ptr<functor_type>> functors_; // length: number of variables

    private:
        std::vector<Bin> bins_; // length: number of bins

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

