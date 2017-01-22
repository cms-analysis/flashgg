#include "flashgg/DataFormats/interface/WeightedObject.h"
#include <iostream>

namespace flashgg {

    WeightedObject::WeightedObject()
    {}

    WeightedObject::~WeightedObject()
    {}

    void WeightedObject::setWeight( string key, float val )
    {
        auto found_label = std::lower_bound( _labels.begin(), _labels.end(), key );
        if( found_label == _labels.end() || *found_label != key ) {
            _weights.insert( _weights.begin() + std::distance( _labels.begin(), found_label ), val );
            _labels.insert( found_label, key );
        } else {
            _weights[std::distance( _labels.begin(), found_label )] = val;
        }
    }


    bool WeightedObject::hasWeight( string key ) const
    {
        auto found_label = std::lower_bound( _labels.begin(), _labels.end(), key );
        return ( !( found_label == _labels.end() || *found_label != key ) );
    }

    float WeightedObject::weight( string key ) const
    {
        auto found_label = std::lower_bound( _labels.begin(), _labels.end(), key );
        if( found_label == _labels.end() || *found_label != key ) {
            return 1.;
        }
        return _weights[std::distance( _labels.begin(), found_label )];
    }

    void WeightedObject::includeWeights( const WeightedObject &other, bool usecentralifnotfound /* old behavior: false */ )
    {
        // multiplies weights which are present in this and other, imports weights that are only in other

        // The usecentralifnotfound is a subtle point:  supposer we have electron_down < electron_central < electron_up
        // Now we are building a set of weights for a TTHLeptonic Tag with this method.
        // We ask a jet for its electron weight, which of course it does  not have.
        // With usecentralifnotfound = false we would effectively be putting a contribution of 1 in from the jet.
        // But we will always put in the central value of the jet for the central value of the tag (as we should).
        // So if the central weight of the jet is 2 we could now have electron_down_tag < electron_up_tag < central_tag
        // Turning on usecentralifnotfound means that we use the jet weight of 2 as a multiplier for all weights
        // not contained by the jet, so e.g. electron_down_tag/central_tag = electron_down/electron_central as expected.
        // Please see Taggers/plugins/TTHLeptonicTagProducer.cc for an easier-to-read code example
        // used to debug this and illustrate why we need this behavior.  

        float initialcentralweight = centralWeight();

        for( auto keyIt = _labels.begin() ; keyIt != _labels.end() ; keyIt++ ) {
            if( other.hasWeight( *keyIt ) ) {
                setWeight( *keyIt, weight( *keyIt ) * other.weight( *keyIt ) );
            } else {
                if ( usecentralifnotfound ) {
                    setWeight( *keyIt, weight( *keyIt ) * other.centralWeight() );
                }
            }
        }
        for( auto keyIt = other._labels.begin() ; keyIt != other._labels.end() ; keyIt++ ) {
            if( !hasWeight( *keyIt ) ) {
                if ( usecentralifnotfound ) {
                    setWeight( *keyIt, initialcentralweight * other.weight( *keyIt ) );
                } else {
                    setWeight( *keyIt, other.weight( *keyIt ) );
                }
            }
        }
    }

    void WeightedObject::includeWeightsByLabel( const WeightedObject &other, string keyInput, bool usecentralifnotfound /* default behavior: true*/ )
    {
        // multiplies weights which are present in this and other, imports weights that are only in other

        // The usecentralifnotfound is a subtle point:  supposer we have electron_down < electron_central < electron_up
        // Now we are building a set of weights for a TTHLeptonic Tag with this method.
        // We ask a jet for its electron weight, which of course it does  not have.
        // With usecentralifnotfound = false we would effectively be putting a contribution of 1 in from the jet.
        // But we will always put in the central value of the jet for the central value of the tag (as we should).
        // So if the central weight of the jet is 2 we could now have electron_down_tag < electron_up_tag < central_tag
        // Turning on usecentralifnotfound means that we use the jet weight of 2 as a multiplier for all weights
        // not contained by the jet, so e.g. electron_down_tag/central_tag = electron_down/electron_central as expected.
        // Please see Taggers/plugins/TTHLeptonicTagProducer.cc for an easier-to-read code example
        // used to debug this and illustrate why we need this behavior.  

        bool isWeightByLabelFound = false;
        float initialcentralweight = centralWeight();
        float othercentralweightLabel = 1.0;
        for( auto keyIt = other._labels.begin() ; keyIt != other._labels.end() ; keyIt++ ) {
            //std::cout<<"In includeWeightsByLabel central auto keyIt loop : "<< *keyIt <<std::endl;
            if(keyIt->find(keyInput) != std::string::npos && keyIt->find("Central") != std::string::npos ){
                //std::cout<<"In includeWeightsByLabel central auto keyIt loop 1 : "<< *keyIt <<std::endl;
                othercentralweightLabel = other.weight( *keyIt );
                isWeightByLabelFound = true;
            }
        }

        if(!isWeightByLabelFound){
            std::cout<<"NOTE :  includeWeightsByLabel : LABEL : "<< keyInput << " NOT FOUND LABEL WEIGHT FOR OBJECT" << std::endl;
        }        

        // multiplies weights which are present in this and other
        for( auto keyIt = _labels.begin() ; keyIt != _labels.end() ; keyIt++ ) {
            //std::cout<<"In includeWeightsByLabel auto keyIt loop : "<< *keyIt <<std::endl;
            //if(keyIt == keyInput){            
            if( *keyIt == "Central" ) {//multiply central weight with label central weight
                //std::cout<<"In includeWeightsByLabel auto keyIt loop 1 : "<< *keyIt <<std::endl;
                setWeight( *keyIt, weight( *keyIt ) * othercentralweightLabel );
            } else if( other.hasWeight( *keyIt ) && keyIt->find(keyInput) != std::string::npos ) {//multiply other weights that have the same label, for instance for second jet in event
                //std::cout<<"In includeWeightsByLabel auto keyIt loop 2 : "<< *keyIt <<std::endl;
                setWeight( *keyIt, weight( *keyIt ) * other.weight( *keyIt ) );
            } else {// for other weights with different labels
                //std::cout<<"In includeWeightsByLabel auto keyIt loop 3 : "<< *keyIt <<std::endl;
                if ( usecentralifnotfound ) {
                    //std::cout<<"In includeWeightsByLabel auto keyIt loop 4 : "<< *keyIt <<std::endl;
                    setWeight( *keyIt, weight( *keyIt ) * othercentralweightLabel );
                }
            }
        }
        //imports weights that are only in other
        for( auto keyIt = other._labels.begin() ; keyIt != other._labels.end() ; keyIt++ ) {
            //std::cout<<"In includeWeightsByLabel auto keyIt other loop : "<< *keyIt <<std::endl;
            if( !hasWeight( *keyIt ) ) {
                //std::cout<<"In includeWeightsByLabel auto keyIt other loop 1 : "<< *keyIt <<std::endl;
                if(*keyIt == "Central" ) {//multiply central weight with label central weight
                    //std::cout<<"In includeWeightsByLabel auto keyIt other loop 2 : "<< *keyIt <<std::endl;
                    setWeight( *keyIt, othercentralweightLabel );
                } else if(keyIt->find(keyInput) != std::string::npos ){
                    //std::cout<<"In includeWeightsByLabel auto keyIt other loop 3 : "<< *keyIt <<std::endl;
                    if ( usecentralifnotfound ) {
                        setWeight( *keyIt, initialcentralweight * other.weight( *keyIt ) );
                    } else {
                        setWeight( *keyIt, other.weight( *keyIt ) );
                    }
                } 
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
