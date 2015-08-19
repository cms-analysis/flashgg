#include "flashgg/DataFormats/interface/WeightedObject.h"

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
            return -1.;
        }
        return _weights[std::distance( _labels.begin(), found_label )];
    }

    void WeightedObject::includeWeights( const WeightedObject &other )
    {
        // multiplies weights which are present in this and other, imports weights that are only in other
        for( auto keyIt = _labels.begin() ; keyIt != _labels.end() ; keyIt++ ) {
            if( other.hasWeight( *keyIt ) ) {
                setWeight( *keyIt, weight( *keyIt ) * other.weight( *keyIt ) );
            }
        }
        for( auto keyIt = other._labels.begin() ; keyIt != other._labels.end() ; keyIt++ ) {
            if( !hasWeight( *keyIt ) ) {
                setWeight( *keyIt, other.weight( *keyIt ) );
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
