#include "flashgg/DataFormats/interface/WeightedObject.h"

namespace flashgg {

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
