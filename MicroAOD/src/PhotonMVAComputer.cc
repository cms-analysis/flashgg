#include "flashgg/MicroAOD/interface/PhotonMVAComputer.h"
#include "FWCore/Framework/interface/Event.h"

using namespace edm;
using namespace std;

namespace flashgg {
    PhotonMVAComputer::PhotonMVAComputer( const ParameterSet &cfg, edm::ConsumesCollector &&cc ) :
        PhotonMVAComputer( cfg )
    {
    }

    PhotonMVAComputer::PhotonMVAComputer( const ParameterSet &cfg ) :
        GlobalVariablesComputer( cfg ),
        classifier_( cfg ),
        selector_( cfg.getParameter<string>( "mvaPreselection" ) )
    {
        auto mvas = cfg.getParameter<vector<edm::ParameterSet> >( "mvas" );
        for( auto &mva : mvas ) {
            toFill_.push_back( std::make_tuple( mva.getParameter<std::string>( "name" ), mva.getParameter<double>( "default" ) ) );
        }
        auto categories = cfg.getParameter<vector<edm::ParameterSet> >( "categories" );

        for( auto &cat : categories ) {
            auto name = cat.getUntrackedParameter<string>( "name", Form( "cat%lu", mvas_.size() ) );
            for( auto &toFill : toFill_ ) {
                mvas_[name].push_back( new mva_type( cat.getParameter<edm::ParameterSet>( std::get<0>( toFill ) ), this ) );
            }
            // cout << name << " " << mvas_[name].size() << endl;
        }
    }

    PhotonMVAComputer::~PhotonMVAComputer()
    {
        for( auto &mva  : mvas_ ) {
            for( auto &func : mva.second ) {
                delete func;
            }
            mva.second.clear();
        }
    }

    void PhotonMVAComputer::fill( Photon &pho )
    {
        bool selected = selector_( pho );
        auto isel = mvas_.end();
        if( selected ) {
            auto cat = classifier_( pho );
            isel = mvas_.find( cat.first.first );

            // cout << cat.first << endl;
            selected = ( isel != mvas_.end() );
        }

        if( selected ) {
            for( size_t imva = 0; imva < toFill_.size(); ++imva ) {
                // cout << isel->second.size() << " " << imva << endl;
                pho.addUserFloat( std::get<0>( toFill_[imva] ), ( *isel->second[imva] )( pho ) );
            }
        } else {
            for( auto &toFill : toFill_ ) {
                pho.addUserFloat( std::get<0>( toFill ), std::get<1>( toFill ) );
            }
        }
    }

}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

