#ifndef flashgg_MVAComputer_h
#define flashgg_MVAComputer_h

#include <tuple>
#include <vector>
#include <string>

#include "CommonTools/Utils/interface/TFileDirectory.h"

#include "flashgg/Taggers/interface/StringHelpers.h"

#include "CommonTools/Utils/interface/StringObjectFunction.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

#include "TMVA/Reader.h"
#include "flashgg/Taggers/interface/GlobalVariablesDumper.h"

#include "XGBoostCMSSW/XGBoostInterface/interface/XGBComputer.h"

namespace flashgg {

    template<class ObjectT, class FunctorT = StringObjectFunction<ObjectT, true>, bool useXGB=false>
    class MVAComputer
    {
    public:
        typedef ObjectT object_type;
        typedef FunctorT functor_type;

        MVAComputer() {};
        MVAComputer( const edm::ParameterSet &cfg, GlobalVariablesComputer *global = 0 );
        ~MVAComputer();

        std::vector<float> operator()( const object_type &obj ) const;
        
    private:

        void bookMVA() const;        

        mutable TMVA::Reader *reader_;
        GlobalVariablesComputer *global_;

        mutable XGBComputer xgbComputer_;
        mutable XGBComputer::mva_variables  xgbVars_;
        

        bool regression_;
        bool multiclass_;
        std::string classifier_, weights_;
        std::vector<std::tuple<std::string, int> > variables_;
        mutable std::vector<float> values_;
        std::vector<functor_type> functors_;
    };

    template<class F, class O, bool useXGB>
    MVAComputer<F, O, useXGB>::MVAComputer( const edm::ParameterSet &cfg, GlobalVariablesComputer *global ) :
        reader_( 0 ),
        global_( global ),
        regression_( cfg.exists("regression") ? cfg.getParameter<bool>("regression") : false ),
        multiclass_( cfg.exists("multiclass") ? cfg.getParameter<bool>("multiclass") : false ),
        classifier_( cfg.exists("classifier") ? cfg.getParameter<std::string>( "classifier" ) : "" )
    {
        using namespace std;

        weights_    = cfg.getParameter<edm::FileInPath>( "weights" ).fullPath();

        auto variables = cfg.getParameter<vector<edm::ParameterSet> >( "variables" );
        for( auto &var : variables ) {
            auto expr = var.getParameter<string>( "expr" );
            auto name = var.getUntrackedParameter<string>( "name", expr );
            auto pos = expr.find( "global." );
            if( pos == 0 ) {
                assert( global != 0 );
                variables_.push_back( std::make_tuple( name + "::" + expr.substr( 7 ), -1 ) );
            } else {
                functors_.push_back( functor_type( expr ) );
                variables_.push_back( std::make_tuple( name, functors_.size() - 1 ) );
            }
        }
    }

    template<class F, class O, bool useXGB>
    MVAComputer<F, O, useXGB>::~MVAComputer()
    {
        // if( reader_ ) {
        //     delete reader_;
        // }
    }

    template<class F, class O, bool useXGB>
    void MVAComputer<F, O, useXGB>::bookMVA()  const
    {
        if( !useXGB )
            {
                if( reader_ ) { return; }
                reader_ = new TMVA::Reader( "!Color:Silent" );
            }
        
        values_.resize( variables_.size(), 0. );
        for( size_t i = 0; i<variables_.size(); ++i ) {
            auto &name = std::get<0>( variables_[i] );
            auto pos = name.find( "::" );
            auto vname = name.substr( 0, pos );
            if( pos == name.npos ) {
                if( useXGB )
                    xgbVars_.push_back(std::make_tuple(name, 0. ));
                else
                    reader_->AddVariable( name, &values_[i] );
            }
            else {
                if( useXGB )
                    xgbVars_.push_back(std::make_tuple(vname, 0. ));
                else
                    reader_->AddVariable( vname, &values_[i] );
            }
        }
        
        if( useXGB )
            xgbComputer_ = XGBComputer(&xgbVars_, weights_);    
        else
            reader_->BookMVA( classifier_, weights_ );
    }

    template<class F, class O, bool useXGB>
    std::vector<float> MVAComputer<F, O, useXGB>::operator()( const object_type &obj ) const
    {
        if( ! reader_ && values_.size()==0 )
            bookMVA(); 
        
        for( size_t ivar = 0; ivar < variables_.size(); ++ivar ) {
            auto fvar = std::get<1>(variables_[ivar]);
            if(fvar >= 0)
                {
                    values_[ivar] = functors_[fvar]( obj );
                    //---check for un-physical events
                    if(std::isnan(values_[ivar]))
                        return {-999.};
                    if( useXGB )
                        std::get<1>(xgbVars_[ivar]) = values_[ivar];
                }
            else
                {
                    auto name = std::get<0>(variables_[ivar]);
                    auto pos = name.find( "::" );
                    auto gname = name.substr( pos + 2 );
                    values_[ivar] = global_->valueOf( gname );
                    if( useXGB )
                        std::get<1>(xgbVars_[ivar]) = global_->valueOf( gname );
                }
        }

        std::vector<float> result;
        
        if( !useXGB )
        {
            if(multiclass_)
                result = reader_->EvaluateMulticlass( classifier_.c_str() );
            else if(regression_)
                result = reader_->EvaluateRegression( classifier_.c_str() );
            else
                result.push_back(reader_->EvaluateMVA( classifier_.c_str() ));
        }
        else
        {
            xgbComputer_.SetVariables(&xgbVars_);
            result = xgbComputer_();
        }
        
        return result;
    }
}


#endif  // flashgg_MVAComputer_h
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

