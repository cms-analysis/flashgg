#ifndef FLASHgg_MCWeightObject_h
#define FLASHgg_MCWeightObject_h

#include <map>
#include <vector>

using namespace std;

namespace flashgg {

    class MCWeightObject
    {
    public:
        MCWeightObject();
        ~MCWeightObject();
        map<string, vector<double> > mc_weight_container;

    };
}

#endif
