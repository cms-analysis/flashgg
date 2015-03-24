#include "flashgg/MicroAOD/interface/CutBasedDiPhotonObjectSelector.h"
#include "FWCore/Framework/interface/Event.h"

using namespace edm;
using namespace std;

namespace flashgg {
    CutBasedDiPhotonObjectSelector::CutBasedDiPhotonObjectSelector( const ParameterSet &cfg, edm::ConsumesCollector &cc ) :
        rhoToken_( cc.consumes<double>( cfg.getParameter<InputTag>( "rho" ) ) ),
        classifier_( cfg ),
        selector_( cfg.getParameter<string>( "cut" ) )
    {

        auto variables = cfg.getParameter<vector<string> >( "variables" );
        for( auto &expr : variables ) {
            //// cout << expr << endl;
            functors_.push_back( functor_type( expr ) );
        }

        auto categories = cfg.getParameter<vector<edm::ParameterSet> >( "categories" );

        if( cfg.exists( "ignore" ) ) {
            ignoreCuts_ = cfg.getParameter<vector<int> >( "ignore" );
        }
        if( cfg.exists( "invert" ) ) {
            invertCuts_ = cfg.getParameter<vector<int> >( "invert" );
            invertNtimes_ = cfg.getParameter<int>( "invertNtimes" );
        }

        for( auto &cat : categories ) {
            auto name = cat.getUntrackedParameter<string>( "name", Form( "cat%lu", selections_.size() ) );

            auto selection = cat.getParameter<vector<edm::ParameterSet> >( "selection" );
            assert( selection.size() == functors_.size() );
            auto &selval = selections_[name];
            for( auto &var : selection )  {
                auto minval = var.getParameter<string>( "min" );
                auto maxval = var.getParameter<string>( "max" );
                auto rhocorr = var.getParameter<string>( "rhocorr" );

                selval.push_back( make_tuple( functor_type( minval ), functor_type( maxval ), functor_type( rhocorr ) ) );
            }
        }
    }

    bool CutBasedDiPhotonObjectSelector::operator()( const DiPhotonCandidate &cand, const EventBase &ev ) const
    {
        if( ! selector_( cand ) ) { return false; }
        edm::Handle<double> rho;
        const Event *ee = dynamic_cast<const Event *>( &ev );
        if( ee ) { ee->getByToken( rhoToken_, rho ); }
        else { assert( 0 );  } // FIXME need to complete for FWlite
        rho_ = *rho;
        auto leadingPhoton = cand.leadingPhoton();
        auto subleadingPhoton = cand.subLeadingPhoton();
        bool passSelection = pass( *leadingPhoton ) && pass( *subleadingPhoton );
        if( ! passSelection ) { return false; }
        if( ! invertCuts_.empty() ) {
            if( invertNtimes_ == 0 ) {
                return passInverted( *leadingPhoton ) || passInverted( *subleadingPhoton );
            } else if( invertNtimes_ == 1 ) {
                return passInverted( *leadingPhoton ) ^ passInverted( *subleadingPhoton );
            } else {
                return passInverted( *leadingPhoton ) && passInverted( *subleadingPhoton );
            }
        }
        return true;
    }


    bool CutBasedDiPhotonObjectSelector::passInverted( const Photon &pho ) const
    {
        auto cat = classifier_( pho );
        auto isel = selections_.find( cat.first );
        if( isel == selections_.end() ) {
            return false;
        }
        const auto &sel = isel->second;
        for( size_t iv = 0; iv < sel.size(); ++iv ) {
            if( find( invertCuts_.begin(), invertCuts_.end(), iv ) == invertCuts_.end() ) { continue; }
            auto val = functors_[iv]( pho ) - rho_ * get<2>( sel[iv] )( pho );
            if( val > get<0>( sel[iv] )( pho ) && val < get<1>( sel[iv] )( pho ) ) { return false; }
        }
        return true;
    }

    bool CutBasedDiPhotonObjectSelector::pass( const Photon &pho ) const
    {
        auto cat = classifier_( pho );
        auto isel = selections_.find( cat.first );
        if( isel == selections_.end() ) {
            return false;
        }
        const auto &sel = isel->second;
        for( size_t iv = 0; iv < sel.size(); ++iv ) {
            if( find( ignoreCuts_.begin(), ignoreCuts_.end(), iv ) != ignoreCuts_.end() ||
                    find( invertCuts_.begin(), invertCuts_.end(), iv ) != invertCuts_.end()
              ) { continue; }
            auto val = functors_[iv]( pho ) - rho_ * get<2>( sel[iv] )( pho );
            if( val <= get<0>( sel[iv] )( pho ) || val >= get<1>( sel[iv] )( pho ) ) { return false; }
        }
        return true;
    }
}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

