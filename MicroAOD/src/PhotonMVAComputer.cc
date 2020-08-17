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
        // list of mvas to fill, with default values
        auto mvas = cfg.getParameter<vector<edm::ParameterSet> >( "mvas" );
        for( auto &mva : mvas ) {
            toFill_.push_back( std::make_tuple( mva.getParameter<std::string>( "name" ), mva.getParameter<double>( "default" ) ) );
        }
        
        // book fillers for each category
        auto categories = cfg.getParameter<vector<edm::ParameterSet> >( "categories" );
        for( auto &cat : categories ) {
            auto name = cat.getUntrackedParameter<string>( "name", Form( "cat%lu", mvas_.size() ) );
            for( auto &toFill : toFill_ ) {                
                if( cat.exists( std::get<0>( toFill ) ) ) { 
                    // if this category has a rule to fill the mva book the filler
                    mvas_[name].push_back( new mva_type( cat.getParameter<edm::ParameterSet>( std::get<0>( toFill ) ), this ) );
                } else { 
                    // otherwise store a NULL pointer
                    mvas_[name].push_back( 0 );
                }
            }
        }
    }

    PhotonMVAComputer::~PhotonMVAComputer()
    {
        for( auto &mva  : mvas_ ) {
            for( auto &func : mva.second ) {
                if( func != 0 ) { delete func; }
            }
            mva.second.clear();
        }
    }

    void PhotonMVAComputer::fill( Photon &pho )
    {
        // check if the photon passes the preselection
        bool selected = selector_( pho );
        auto isel = mvas_.end();
        if( selected ) {
            // if so, find the corresponding category and filler
            auto cat = classifier_( pho );
            isel = mvas_.find( cat.first );
            // if the candidated doesn't fall in any category mark it as not preselected
            selected = ( isel != mvas_.end() );
        }

        if( selected ) {
            // fill mva values if candidate passes preselection
            for( size_t imva = 0; imva < toFill_.size(); ++imva ) {
                if( isel->second[imva] == 0 ) { 
                    // if no filler is specified for some MVA set value to default
                    pho.addUserFloat( std::get<0>( toFill_[imva] ), std::get<1>( toFill_[imva] ) );
                } else {
                    // otherwise evaluate the MVA
                    pho.addUserFloat( std::get<0>( toFill_[imva] ), ( *isel->second[imva] )( pho )[0] );
                }
            }
        } else {
            // if not pre-selected range set all MVAs to the default values
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

