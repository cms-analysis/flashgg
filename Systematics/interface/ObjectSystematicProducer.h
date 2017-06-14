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

#include "flashgg/Systematics/interface/BaseSystMethod.h"

#include "flashgg/MicroAOD/interface/GlobalVariablesComputer.h"

//#include <type_traits>
//#include <typeinfo>
//#include "FWCore/Utilities/interface/EDMException.h"

using namespace edm;
using namespace std;

#define PAIR_ZERO make_pair( param_var( 0 ), param_var( 0 ) )

namespace flashgg {

    template <typename flashgg_object, typename param_var, template <typename...> class output_container>
    class ObjectSystematicProducer : public edm::EDProducer
    {
    public:

        ObjectSystematicProducer( const edm::ParameterSet & );

    protected:
        GlobalVariablesComputer globalVars_;
        std::vector<shared_ptr<BaseSystMethod<flashgg_object, param_var> > > Corrections_;
        std::vector<shared_ptr<BaseSystMethod<flashgg_object, pair<param_var, param_var> > > > Corrections2D_;
        void produce( edm::Event &, const edm::EventSetup & ) override;

    private:

        void ApplyCorrections( flashgg_object &y, shared_ptr<BaseSystMethod<flashgg_object, param_var> > CorrToShift, param_var syst_shift );
        void ApplyCorrections( flashgg_object &y, shared_ptr<BaseSystMethod<flashgg_object, pair<param_var, param_var> > > CorrToShift,
                               pair<param_var, param_var>  syst_shift );
        void ApplyNonCentralWeights( flashgg_object &y );

        edm::EDGetTokenT<View<flashgg_object> > ObjectToken_;

        std::vector<std::vector<param_var> > sigmas_;
        std::vector<std::string> collectionLabelsNonCentral_;

        std::vector<std::vector<pair<param_var, param_var> > > sigmas2D_;
    };

    template <typename flashgg_object, typename param_var, template <typename...> class output_container>
    ObjectSystematicProducer<flashgg_object, param_var, output_container>::ObjectSystematicProducer( const ParameterSet &iConfig ) :
        globalVars_(iConfig),
        ObjectToken_( consumes<View<flashgg_object> >( iConfig.getParameter<InputTag>( "src" ) ) )
    {
        //        edm::Service<edm::RandomNumberGenerator> rng;
        //        if( ! rng.isAvailable() ) {
        //            throw cms::Exception( "Configuration" ) << "ObjectSystematicProducer requires the RandomNumberGeneratorService  - please add to configuration";
        //        }

        produces<output_container<flashgg_object> >(); // Central value
        std::vector<edm::ParameterSet> vpset = iConfig.getParameter<std::vector<edm::ParameterSet> >( "SystMethods" );
        std::vector<edm::ParameterSet> vpset2D = iConfig.getParameter<std::vector<edm::ParameterSet> >( "SystMethods2D" );

        Corrections_.resize( vpset.size() );
        Corrections2D_.resize( vpset2D.size() );

        unsigned int ipset = 0;
        unsigned int ipset2D = 0;
        for( const auto &pset : vpset ) {
            std::string methodName = pset.getParameter<string>( "MethodName" );

            // This block of code supports a list of integer or float type objects

            std::vector<param_var> nsigmas = pset.getParameter<vector<param_var> >( "NSigmas" );

            // Remove 0 values, because we always have a central collection
            // If nsigmas ends up being empty for a given corrector,
            // the central operation will still be applied to all collections
            nsigmas.erase( std::remove( nsigmas.begin(), nsigmas.end(), param_var( 0 ) ), nsigmas.end() );

            sigmas_.push_back( nsigmas );
            //            std::cout << " About to create a 1D factory with methodName = " << methodName << std::endl;
            if( pset.exists( "PhotonMethodName" ) ) {
                string photonMethodName = pset.getParameter<string>( "PhotonMethodName" );
                //                std::cout << "    PhotonMethodName = " << photonMethodName << std::endl;
            }
            Corrections_.at( ipset ).reset( FlashggSystematicMethodsFactory<flashgg_object, param_var>::get()->create( methodName, pset, consumesCollector(), &globalVars_  ) );
            if( !Corrections_.at( ipset )->makesWeight() ) {
                for( const auto &sig : sigmas_.at( ipset ) ) {
                    std::string collection_label = Corrections_.at( ipset )->shiftLabel( sig );
                    produces<output_container<flashgg_object> >( collection_label );
                    collectionLabelsNonCentral_.push_back( collection_label ); // 2N elements, current code gets labels right only if loops are consistent
                }
            } else {
                //                std::cout << " We skipped making a collection for label " << Corrections_.at( ipset )->shiftLabel( param_var( 0 ) ) << " because it's a weight " << std::endl;
            }
            ipset++;
        }

        for( const auto &pset : vpset2D ) {
            // This block of code supports pairs of integer or float type objects, e.g. for smearing

            std::string methodName = pset.getParameter<string>( "MethodName" );

            auto pairset = pset.getParameterSet( "NSigmas" );
            std::vector<param_var> firstVar = pairset.getParameter<vector<param_var> >( "firstVar" );
            std::vector<param_var> secondVar = pairset.getParameter<vector<param_var> >( "secondVar" );
            assert( firstVar.size() == secondVar.size() );

            std::vector<pair<param_var, param_var> > nsigmas;
            for( unsigned int isig = 0 ; isig < firstVar.size() ; isig++ ) {
                nsigmas.emplace_back( firstVar[isig], secondVar[isig] );
            }

            nsigmas.erase( std::remove( nsigmas.begin(), nsigmas.end(), PAIR_ZERO ), nsigmas.end() );

            sigmas2D_.push_back( nsigmas );
            //            std::cout << " About to create a 2D factory with methodName = " << methodName << std::endl;
            if( pset.exists( "PhotonMethodName" ) ) {
                string photonMethodName = pset.getParameter<string>( "PhotonMethodName" );
                //                std::cout << "    PhotonMethodName = " << photonMethodName << std::endl;
            }
            Corrections2D_.at( ipset2D ).reset( FlashggSystematicMethodsFactory<flashgg_object, pair<param_var, param_var> >::get()->create( methodName, pset, consumesCollector(), &globalVars_ ) );
            if( !Corrections_.at( ipset2D )->makesWeight() ) {
                for( const auto &sig : sigmas2D_.at( ipset2D ) ) {
                    std::string collection_label = Corrections2D_.at( ipset2D )->shiftLabel( sig );
                    produces<output_container<flashgg_object> >( collection_label );
                    collectionLabelsNonCentral_.push_back( collection_label );
                }
            } else {
                //                std::cout << " We skipped making a label for " << Corrections2D_.at( ipset2D )->shiftLabel( PAIR_ZERO ) << " because its a weight (2d)" << std::endl;
            }

            ipset2D++;
        }
    }

    ///fucntion takes in the current corection one is looping through and compares with its own internal loop, given that this will be within the corr and sys loop it takes care of the 2n+1 collection number////
    template <typename flashgg_object, typename param_var, template <typename...> class output_container>
    void ObjectSystematicProducer<flashgg_object, param_var, output_container>::ApplyCorrections( flashgg_object &y,
            shared_ptr<BaseSystMethod<flashgg_object, param_var> > CorrToShift,
            param_var syst_shift )
    {
        float theWeight = 1.;
        //        std::cout << " 1d before 1d " << std::endl;
        //        std::cout << " In ObjectSystematicProducer::ApplyCorrections pt m " << y.pt() << " " << y.mass() << std::endl;
        for( unsigned int ncorr = 0; ncorr < Corrections_.size(); ncorr++ ) {
            if( CorrToShift == Corrections_.at( ncorr ) ) {
                Corrections_.at( ncorr )->applyCorrection( y, syst_shift );
            } else if( Corrections_.at( ncorr )->makesWeight() ) {
                //                std::cout << " Setting weight for " << Corrections_.at( ncorr )->shiftLabel( 0 ) <<
                //                    " to " << Corrections_.at( ncorr )->makeWeight( y, param_var( 0 ) ) << std::endl;
                y.setWeight( Corrections_.at( ncorr )->shiftLabel( 0 ), Corrections_.at( ncorr )->makeWeight( y, param_var( 0 ) ) ); // use very carefully, n.b. not scaled
                theWeight *= Corrections_.at( ncorr )->makeWeight( y, param_var( 0 ) );
            } else {
                Corrections_.at( ncorr )->applyCorrection( y, param_var( 0 ) );
            }
        }
        //        std::cout << " 1d before 2d " << std::endl;
        for( unsigned int ncorr = 0; ncorr < Corrections2D_.size(); ncorr++ ) {
            //            std::cout << " 2d ncorr=" << ncorr << "/" << Corrections2D_.size() << std::endl;
            if( Corrections2D_.at( ncorr )->makesWeight() ) {
                y.setWeight( Corrections2D_.at( ncorr )->shiftLabel( PAIR_ZERO ), Corrections2D_.at( ncorr )->makeWeight( y, PAIR_ZERO ) ); // use very carefully, n.b. not scaled
                theWeight *= Corrections2D_.at( ncorr )->makeWeight( y, PAIR_ZERO );
                //                std::cout << " 2d changed the weight to" << theWeight << std::endl;
                //                std::cout << "    " << Corrections2D_.at( ncorr )->shiftLabel( PAIR_ZERO ) << std::endl;
            } else {
                Corrections2D_.at( ncorr )->applyCorrection( y, PAIR_ZERO );
                //                std::cout << " 2d applied correction " << std::endl;
                //                std::cout << "    " << Corrections2D_.at( ncorr )->shiftLabel( PAIR_ZERO ) << std::endl;
            }
        }
        //        std::cout << " Applied a central weight of " << theWeight << " - as part of 1d shift" << std::endl;
        //        if( CorrToShift == nullptr ) {
        //            std::cout << "   Shift was central " << std::endl;
        //        } else {
        //            std::cout << "   Shift was " << CorrToShift->shiftLabel( syst_shift ) << std::endl;
        //        }
        y.setCentralWeight( theWeight );
        //        std::cout << " 1d end " << std::endl;
    }

    template <typename flashgg_object, typename param_var, template <typename...> class output_container>
    void ObjectSystematicProducer<flashgg_object, param_var, output_container>::ApplyCorrections( flashgg_object &y,
            shared_ptr<BaseSystMethod<flashgg_object, pair<param_var, param_var> > > CorrToShift,
            pair<param_var, param_var>  syst_shift )
    {
        float theWeight = 1.;
        //        std::cout << " In ObjectSystematicProducer::ApplyCorrections pt m " << y.pt() << " " << y.mass() << std::endl;
        //        std::cout << "2d before 1d" << std::endl;
        for( unsigned int ncorr = 0; ncorr < Corrections_.size(); ncorr++ ) {
            if( Corrections_.at( ncorr )->makesWeight() ) {
                //                std::cout << " Setting weight for " << Corrections_.at( ncorr )->shiftLabel( 0 ) << 
                //                    " to " << Corrections_.at( ncorr )->makeWeight( y, param_var( 0 ) ) << std::endl;
                y.setWeight( Corrections_.at( ncorr )->shiftLabel( 0 ), Corrections_.at( ncorr )->makeWeight( y, param_var( 0 ) ) ); // use very carefully, n.b. not scaled
                theWeight *= Corrections_.at( ncorr )->makeWeight( y, param_var( 0 ) );
            } else {
                Corrections_.at( ncorr )->applyCorrection( y, param_var( 0 ) );
            }
        }
        //        std::cout << "2d before 2d" << std::endl;
        for( unsigned int ncorr = 0; ncorr < Corrections2D_.size(); ncorr++ ) {
            if( CorrToShift == Corrections2D_.at( ncorr ) ) {
                Corrections2D_.at( ncorr )->applyCorrection( y, syst_shift );
            } else if( Corrections2D_.at( ncorr )->makesWeight() ) {
                y.setWeight( Corrections2D_.at( ncorr )->shiftLabel( PAIR_ZERO ),
                             Corrections2D_.at( ncorr )->makeWeight( y, PAIR_ZERO ) ); // use very carefully, n.b. not scaled        
                theWeight *= Corrections2D_.at( ncorr )->makeWeight( y, PAIR_ZERO );
            } else {
                Corrections2D_.at( ncorr )->applyCorrection( y, PAIR_ZERO );
            }
        }
        y.setCentralWeight( theWeight );
        //        std::cout << " Applied a central weight of " << theWeight << " - as part of 2d shift" << std::endl;
    }

    template <typename flashgg_object, typename param_var, template <typename...> class output_container>
    void ObjectSystematicProducer<flashgg_object, param_var, output_container>::ApplyNonCentralWeights( flashgg_object &y )
    {
        for( unsigned int ncorr = 0; ncorr < Corrections_.size(); ncorr++ ) {
            if( Corrections_.at( ncorr )->makesWeight() ) {
                for( const auto &sig : sigmas_.at( ncorr ) ) {
                    float weightAdjust = ( Corrections_.at( ncorr )->makeWeight( y, sig ) / Corrections_.at( ncorr )->makeWeight( y, param_var( 0 ) ) );
                    std::string label = Corrections_.at( ncorr )->shiftLabel( sig );
                    y.setWeight( label, weightAdjust * y.centralWeight() );
                    //                    std::cout << " Applying 1d non-central weight " << label << " of " << y.weight( label ) << " - pt eta " << y.pt() << " " << y.eta() << std::endl;
                }
            }
        }
        for( unsigned int ncorr = 0; ncorr < Corrections2D_.size(); ncorr++ ) {
            if( Corrections2D_.at( ncorr )->makesWeight() ) {
                for( const auto &sig : sigmas2D_.at( ncorr ) ) {
                    float weightAdjust = ( Corrections2D_.at( ncorr )->makeWeight( y, sig ) / Corrections2D_.at( ncorr )->makeWeight( y, PAIR_ZERO ) );
                    std::string label = Corrections2D_.at( ncorr )->shiftLabel( sig );
                    y.setWeight( label, weightAdjust * y.centralWeight() );
                    //                    std::cout << " Applying 2d non-central weight " << label << " of " << y.weight( label ) << " - pt eta " << y.pt() << " " << y.eta() << std::endl;
                }
            }
        }
    }

    template <typename flashgg_object, typename param_var, template <typename...> class output_container>
    void ObjectSystematicProducer<flashgg_object, param_var, output_container>::produce( Event &evt, const EventSetup & setup )
    {

        Handle<View<flashgg_object> > objects;
        evt.getByToken( ObjectToken_, objects );

        for( unsigned int ncorr = 0 ; ncorr < Corrections_.size() ; ncorr++ ) {
            Corrections_.at( ncorr )->eventInitialize( evt, setup );
        }
        for( unsigned int ncorr = 0 ; ncorr < Corrections2D_.size() ; ncorr++ ) {
            Corrections2D_.at( ncorr )->eventInitialize(evt, setup );
        }

        globalVars_.update(evt);
        
        // Build central collection
        std::vector<float> centralWeights;
        unique_ptr<output_container<flashgg_object> > centralObjectColl( new output_container<flashgg_object> );
        for( unsigned int i = 0; i < objects->size(); i++ ) {
            flashgg_object *p_obj = objects->ptrAt( i )->clone();
            flashgg_object obj = *p_obj;
            delete p_obj;
            ApplyCorrections( obj, nullptr, param_var( 0 ) );
            ApplyNonCentralWeights( obj );
            centralWeights.push_back( obj.centralWeight() );
            centralObjectColl->push_back( obj );
        }
        evt.put( std::move(centralObjectColl) ); // put central collection in event

        //        std::cout << " after producing central" << std::endl;

        // build 2N shifted collections
        // A dynamically allocated array of unique_ptrs may be a bit "unsafe" to maintain,
        // although I think I have done it correctly - the delete[] statement below is vital
        // Problem: vector<unique_ptr> is not allowed
        // Possible alternate solutions: map, multimap, vector<unique_ptr> + std::move ??
        std::unique_ptr<output_container<flashgg_object> > *all_shifted_collections;
        unsigned int total_shifted_collections = collectionLabelsNonCentral_.size();
        all_shifted_collections = new std::unique_ptr<output_container<flashgg_object> >[total_shifted_collections];
        for( unsigned int ncoll = 0 ; ncoll < total_shifted_collections ; ncoll++ ) {
            all_shifted_collections[ncoll].reset( new output_container<flashgg_object> );
        }
        for( unsigned int i = 0; i < objects->size(); i++ ) {
            unsigned int ncoll = 0;
            for( unsigned int ncorr = 0 ; ncorr < Corrections_.size() ; ncorr++ ) {
                for( const auto &sig : sigmas_.at( ncorr ) ) {
                    //                    std::cout << i << " " << ncoll << " " << sig << std::endl;
                    if( !Corrections_.at( ncorr )->makesWeight() ) {
                        flashgg_object *p_obj = objects->ptrAt( i )->clone();
                        flashgg_object obj = *p_obj;
                        delete p_obj;
                        ApplyCorrections( obj, Corrections_.at( ncorr ), sig );
                        obj.setCentralWeight( centralWeights[i] );
                        all_shifted_collections[ncoll]->push_back( obj );
                        ncoll++;
                    }
                }
            }
            for( unsigned int ncorr = 0 ; ncorr < Corrections2D_.size() ; ncorr++ ) {
                for( const auto &sig : sigmas2D_.at( ncorr ) ) {
                    //                    std::cout << i << " " << ncoll << " " << sig.first << " " << sig.second << std::endl;
                    if( !Corrections_.at( ncorr )->makesWeight() ) {
                        flashgg_object *p_obj = objects->ptrAt( i )->clone();
                        flashgg_object obj = *p_obj;
                        delete p_obj;
                        ApplyCorrections( obj, Corrections2D_.at( ncorr ), sig );
                        obj.setCentralWeight( centralWeights[i] );
                        all_shifted_collections[ncoll]->push_back( obj );
                        ncoll++;
                    }
                }
            }
        }

        // Put shifted collections in event
        for( unsigned int ncoll = 0 ; ncoll < total_shifted_collections ; ncoll++ ) {
            evt.put( std::move(all_shifted_collections[ncoll]), collectionLabelsNonCentral_[ncoll] );
        }

        // See note above about array of unique_ptr
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

