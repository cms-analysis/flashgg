#ifndef FLASHgg_ObjectSystematicProducer_h
#define FLASHgg_ObjectSystematicProducer_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TrackingTools/IPTools/interface/IPTools.h"

#include "flashgg/Systematics/interface/BaseSystMethods.h"

using namespace edm;
using namespace std;

namespace flashgg {

    template <typename flashgg_object, typename param_var>
    class ObjectSystematicProducer : public edm::EDProducer
    {
    public:

        ObjectSystematicProducer( const edm::ParameterSet & );

    private:

        void produce( edm::Event &, const edm::EventSetup & ) override;
        void ApplyCorrections( flashgg_object &y, shared_ptr<BaseSystMethods<flashgg_object, param_var> > CorrToShift, param_var syst_shift );

        edm::EDGetTokenT<View<flashgg_object> > ObjectToken_;

        std::vector<shared_ptr<BaseSystMethods<flashgg_object, param_var> > > Corrections_;
        std::vector<std::vector<param_var> > sigmas_;

        std::vector<std::string> collectionLabelsNonCentral_;
    };

    template <typename flashgg_object, typename param_var>
    ObjectSystematicProducer<flashgg_object, param_var>::ObjectSystematicProducer( const ParameterSet &iConfig ) :
        ObjectToken_( consumes<View<flashgg_object> >( iConfig.getUntrackedParameter<InputTag>( "InputTag", InputTag( "flashggPhotons" ) ) ) )
    {
        produces<std::vector<flashgg_object> >(); // Central value
        std::vector<edm::ParameterSet> vpset = iConfig.getParameter<std::vector<edm::ParameterSet> >( "SystMethods" );

        Corrections_.resize( vpset.size() );

        unsigned int ipset = 0;
        for( const auto &pset : vpset ) {
            std::string methodName = pset.getParameter<string>( "MethodName" );

            // If nsigmas ends up being empty for a given corrector,
            // the central operation will still be applied to all collections
            std::vector<param_var> nsigmas = pset.getParameter<vector<param_var> >( "NSigmas" );

            // Remove 0 values, because we always have a central collection
            nsigmas.erase( std::remove( nsigmas.begin(), nsigmas.end(), 0 ), nsigmas.end() );

            sigmas_.push_back( nsigmas );
            Corrections_.at( ipset ).reset( FlashggSystematicMethodsFactory<flashgg_object, param_var>::get()->create( methodName, pset ) );
            for( const auto &sig : sigmas_.at( ipset ) ) {
                std::string collection_label = Corrections_.at( ipset )->shiftLabel( sig );
                produces<vector<flashgg_object> >( collection_label );
                collectionLabelsNonCentral_.push_back( collection_label ); // 2N elements, current code gets labels right only if loops are consistent
            }
            ipset++;
        }
    }

    ///fucntion takes in the current corection one is looping through and compares with its own internal loop, given that this will be within the corr and sys loop it takes care of the 2n+1 collection number////
    template <typename flashgg_object, typename param_var>
    void ObjectSystematicProducer<flashgg_object, param_var>::ApplyCorrections( flashgg_object &y,
            shared_ptr<BaseSystMethods<flashgg_object, param_var> > CorrToShift, param_var syst_shift )
    {
        for( unsigned int shift = 0; shift < Corrections_.size(); shift++ ) {
            if( CorrToShift == Corrections_.at( shift ) ) {
                Corrections_.at( shift )->applyCorrection( y, syst_shift );
            } else {
                Corrections_.at( shift )->applyCorrection( y, 0 );
            }
        }
    }

    template <typename flashgg_object, typename param_var>
    void ObjectSystematicProducer<flashgg_object, param_var>::produce( Event &evt, const EventSetup & )
    {

        Handle<View<flashgg_object> > objects;
        evt.getByToken( ObjectToken_, objects );

        // Build central collection
        auto_ptr<vector<flashgg_object> > centralObjectColl( new vector<flashgg_object> );
        for( unsigned int i = 0; i < objects->size(); i++ ) {
            flashgg_object obj = flashgg_object( *objects->ptrAt( i ) );
            ApplyCorrections( obj, nullptr, 0 );
            centralObjectColl->push_back( obj );
        }
        evt.put( centralObjectColl ); // put central collection in event

        // build 2N shifted collections
        // A dynamically allocated array of auto_ptrs may be a bit "unsafe" to maintain,
        // although I think I have done it correctly - the delete[] statement below is vital
        // Problem: vector<auto_ptr> is not allowed
        // Possible alternate solutions: map, multimap, vector<unique_ptr> + std::move ??
        std::auto_ptr<std::vector<flashgg_object> > *all_shifted_collections;
        all_shifted_collections = new std::auto_ptr<std::vector<flashgg_object> >[collectionLabelsNonCentral_.size()];
        for( unsigned int ncoll = 0 ; ncoll < collectionLabelsNonCentral_.size() ; ncoll++ ) {
            all_shifted_collections[ncoll].reset( new std::vector<flashgg_object> );
        }
        for( unsigned int i = 0; i < objects->size(); i++ ) {
            unsigned int ncoll = 0;
            for( unsigned int ncorr = 0 ; ncorr < Corrections_.size() ; ncorr++ ) {
                for( const auto &sig : sigmas_.at( ncorr ) ) {
                    flashgg_object obj = flashgg_object( *objects->ptrAt( i ) );
                    ApplyCorrections( obj, Corrections_.at( ncorr ), sig );
                    all_shifted_collections[ncoll]->push_back( obj );
                    ncoll++;
                }
            }
        }

        // Put shifted collections in event
        for( unsigned int ncoll = 0 ; ncoll < collectionLabelsNonCentral_.size() ; ncoll++ ) {
            evt.put( all_shifted_collections[ncoll], collectionLabelsNonCentral_[ncoll] );
        }

        // See note above about array of auto_ptr
        delete[] all_shifted_collections;

    } // end of event
}

#endif

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

