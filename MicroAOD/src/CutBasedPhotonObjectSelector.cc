#include <algorithm>

#include "flashgg/MicroAOD/interface/CutBasedPhotonObjectSelector.h"
#include "FWCore/Framework/interface/Event.h"

using namespace edm;
using namespace std;

namespace flashgg {
    CutBasedPhotonObjectSelector::CutBasedPhotonObjectSelector( const ParameterSet &cfg, edm::ConsumesCollector &cc ) :
        classifier_( cfg ),
        rhoToken_( cc.consumes<double>( cfg.getParameter<InputTag>( "rho" ) ) ),
        selector_( cfg.getParameter<string>( "cut" ) )
    {

        auto variables = cfg.getParameter<vector<string> >( "variables" );
        for( auto &var : variables ) {
            auto cspace = [](unsigned char const c) { return std::isspace(c); };            
            string name, expr;
            if( var.find(":=") != string::npos )
            {
                name = var.substr( 0, var.find_first_of(":=") );
                expr = var.substr( var.find_first_of(":=")+2 );
                name.erase( remove_if(name.begin(), name.end(), cspace), name.end());
                expr.erase( remove_if(expr.begin(), expr.end(), cspace), expr.end());
            }
            else
                name = expr = var;

            cutsResults_[name] = false;
            cutsNames_.push_back( name );
            functors_.push_back( functor_type( expr ) );
        }
        cutsResults_["all"] = false;
        cutsNames_.push_back( "all" );

        auto categories = cfg.getParameter<vector<edm::ParameterSet> >( "categories" );

        if( cfg.exists( "ignore" ) ) {
            ignoreCuts_ = cfg.getParameter<vector<int> >( "ignore" );
        }
        if( cfg.exists( "invert" ) ) {
            invertCuts_ = cfg.getParameter<vector<int> >( "invert" );
        }

        for( size_t icut = 0; icut < functors_.size(); ++icut ) {
            if( find( ignoreCuts_.begin(), ignoreCuts_.end(), icut ) != ignoreCuts_.end() ||
                    find( invertCuts_.begin(), invertCuts_.end(), icut ) != invertCuts_.end()
              ) { continue; }
            applyCuts_.push_back( icut );
        }

        for( auto &cat : categories ) {
            auto name = cat.getUntrackedParameter<string>( "name", Form( "cat%lu", selections_.size() ) );

            auto selection = cat.getParameter<vector<edm::ParameterSet> >( "selection" );
            assert( selection.size() == functors_.size() );
            auto &selval = selections_[name];
            for( auto &var : selection )  {
                functor_type *minf = 0, *maxf = 0;
                stepwise_functor_type *rhof = 0;
                if( var.exists( "min" ) ) {
                    auto minval = var.getParameter<string>( "min" );
                    minf = new functor_type( minval );
                }
                if( var.exists( "max" ) ) {
                    auto maxval = var.getParameter<string>( "max" );
                    maxf = new functor_type( maxval );
                }
                if( var.exists( "rhocorr" ) ) {
                    auto rhocorr = var.getParameter<edm::ParameterSet>( "rhocorr" );
                    rhof = new stepwise_functor_type( rhocorr );
                }

                selval.push_back( make_tuple( functor_ptr( minf ), functor_ptr( maxf ), stepwise_functor_ptr( rhof ) ) );
            }
        }
    }

    void CutBasedPhotonObjectSelector::handle( const EventBase &ev ) const
    {
        edm::Handle<double> rho;
        const Event *ee = dynamic_cast<const Event *>( &ev );
        if( ee ) { ee->getByToken( rhoToken_, rho ); }
        else { assert( 0 );  } // FIXME need to complete for FWlite
        rho_ = *rho;
    }

    bool CutBasedPhotonObjectSelector::operator()( const edm::Ref<edm::View<Photon> > candref, const edm::EventBase &ev ) const {
        return operator()(*candref,ev);
    }

    bool CutBasedPhotonObjectSelector::operator()( const Photon &cand, const EventBase &ev ) const
    {
        if( ! selector_( cand ) ) { return false; }
        handle( ev );
        bool passSelection = pass( cand ) && invertCuts_.empty() ? true : passInverted( cand );

        return passSelection;
    }


    bool CutBasedPhotonObjectSelector::passInverted( const Photon &pho ) const
    {
        auto cat = classifier_( pho );
        auto isel = selections_.find( cat.first );
        if( isel == selections_.end() ) {
            return false;
        }
        const auto &sel = isel->second;
        /// for( size_t iv = 0; iv < sel.size(); ++iv ) {
        ///     if( find( invertCuts_.begin(), invertCuts_.end(), iv ) == invertCuts_.end() ) { continue; }
        for( auto iv : invertCuts_ ) {
            auto val = functors_[iv]( pho ) - rho_ * ( get<2>( sel[iv] ).get() ? ( *get<2>( sel[iv] ) )( pho ) : 0. );
            //// cout << "invert cut " << iv << " " << val
            ////      << " " << ( get<0>(sel[iv]).get() ? (*get<0>(sel[iv]))( pho ) : -999. )
            ////      << " " << ( get<1>(sel[iv]).get() ? (*get<1>(sel[iv]))( pho ) : -999. )
            ////      << endl;
            bool pass = true;
            if( get<0>( sel[iv] ).get() && val <= ( *get<0>( sel[iv] ) )( pho ) ) { pass = false;  }
            if( get<1>( sel[iv] ).get() && val >= ( *get<1>( sel[iv] ) )( pho ) ) { pass = false;  }
            if( pass ) { return false; }
        }
        return true;
    }

    bool CutBasedPhotonObjectSelector::pass( const Photon &pho ) const
    {
        auto cat = classifier_( pho );
        auto isel = selections_.find( cat.first );
        if( isel == selections_.end() ) {
            return false;
        }
        const auto &sel = isel->second;
        for( auto iv : applyCuts_ ) {
            //---compute rho corrected value if correction is specified
            auto val = functors_[iv]( pho ) - rho_ * ( get<2>( sel[iv] ).get() ? ( *get<2>( sel[iv] ) )( pho ) : 0. );
            //---value less then min
            if( get<0>( sel[iv] ).get() && val <= ( *get<0>( sel[iv] ) )( pho ) ) { return false; }
            //---value greater then max
            else if( get<1>( sel[iv] ).get() && val >= ( *get<1>( sel[iv] ) )( pho ) ) { return false; }
        }

        return true;
    }

    const std::unordered_map<std::string, bool>&
    CutBasedPhotonObjectSelector::computeSelections( const Photon &pho, const Event &ev )
    {
        //---reset
        for(auto& sel : cutsResults_)
            sel.second = false;

        //---compute passed/failed selections
        if( selector_( pho ) )
        {
            handle( ev );

            auto cat = classifier_( pho );
            auto isel = selections_.find( cat.first );
            if( isel != selections_.end() )
            {
                const auto &sel = isel->second;
                bool pass_all = true;
                for( auto iv : applyCuts_ )
                {
                    //---compute rho corrected value if correction is specified
                    auto val = functors_[iv]( pho ) - rho_ * ( get<2>( sel[iv] ).get() ? ( *get<2>( sel[iv] ) )( pho ) : 0. );
                    //---value less then min
                    if( get<0>( sel[iv] ).get() && val <= ( *get<0>( sel[iv] ) )( pho ) )
                    {
                        cutsResults_[cutsNames_[iv]] = false;
                        pass_all = false;
                    }
                    //---value greater then max
                    else if( get<1>( sel[iv] ).get() && val >= ( *get<1>( sel[iv] ) )( pho ) )
                    {
                        cutsResults_[cutsNames_[iv]] = false;
                        pass_all = false;
                    }
                    else
                        cutsResults_[cutsNames_[iv]] = true;                
                }
                for( auto iv : invertCuts_ )
                {
                    auto val = functors_[iv]( pho ) - rho_ * ( get<2>( sel[iv] ).get() ? ( *get<2>( sel[iv] ) )( pho ) : 0. );
                    if( get<0>( sel[iv] ).get() && val <= ( *get<0>( sel[iv] ) )( pho ) )
                        cutsResults_[cutsNames_[iv]] = true;
                    if( get<1>( sel[iv] ).get() && val >= ( *get<1>( sel[iv] ) )( pho ) )
                        cutsResults_[cutsNames_[iv]] = true;
                    else
                    {
                        cutsResults_[cutsNames_[iv]] = false;
                        pass_all = false;
                    }
                }
                //---store overall selection result
                cutsResults_["all"] = pass_all;
            }
        }
        
        return cutsResults_;        
    }
}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

