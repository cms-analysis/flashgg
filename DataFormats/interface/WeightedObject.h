#ifndef FLASHgg_WeightedObject_h
#define FLASHgg_WeightedObject_h

#include <vector>
#include <string>

using namespace std;

namespace flashgg {

    class WeightedObject
    {

    public:
        WeightedObject();
        virtual ~WeightedObject();

        float weight( string key ) const;
        float centralWeight() const { return weight( central_key ); }
        void setWeight( string key, float val );
        void setCentralWeight( float val ) { setWeight( central_key, val ); }
        bool hasWeight( string key ) const;
        void includeWeights( const WeightedObject &other, bool usecentralifnotfound = true );
        void includeWeightsByLabel( const WeightedObject &other, string keyInput, bool usecentralifnotfound = true );
        vector<string>::const_iterator weightListBegin() const { return _labels.begin(); }
        vector<string>::const_iterator weightListEnd() const { return _labels.end(); }

    private:
        vector<string> _labels;
        vector<float> _weights;
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
