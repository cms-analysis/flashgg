#ifndef FLASHgg_WeightedObject_h
#define FLASHgg_WeightedObject_h

#include <map>
#include <vector>
#include <string>

using namespace std;

namespace flashgg {

    class WeightedObject
    {

    public:
        float weight( string key ) const { return ( hasWeight( key ) ? _weights.at( key ) : -1. ); }
        float centralWeight() const { return weight( central_key ); }
        void setWeight( string key, float val ) { _weights[key] = val; }
        void setCentralWeight( float val ) { setWeight( central_key, val ); }
        bool hasWeight( string key ) const { return ( _weights.find( key ) != _weights.end() ); }

        void includeWeights( const WeightedObject &other )
        {
            // multiplies weights which are present in this and other, imports weights that are only in other
            auto myWeights = weightList();
            auto otherWeights = other.weightList();
            for( auto keyIt = myWeights.begin() ; keyIt != myWeights.end() ; keyIt++ ) {
                if( other.hasWeight( *keyIt ) ) {
                    setWeight( *keyIt, weight( *keyIt ) * other.weight( *keyIt ) );
                }
            }
            for( auto keyIt = otherWeights.begin() ; keyIt != otherWeights.end() ; keyIt++ ) {
                if( !hasWeight( *keyIt ) ) {
                    setWeight( *keyIt, other.weight( *keyIt ) );
                }
            }
        }

        vector<string> weightList() const
        {
            vector<string> result;
            for( auto it = _weights.begin() ; it != _weights.end() ; it++ ) {
                result.push_back( it->first );
            }
            return result;
        }

    private:
        map<string, float> _weights;
        static constexpr const char *central_key = "Central";

    };
}

#endif

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
