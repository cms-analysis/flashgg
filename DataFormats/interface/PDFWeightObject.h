#ifndef FLASHgg_PDFWeightObject_h
#define FLASHgg_PDFWeightObject_h

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include <map>
#include <vector>

using namespace std;

namespace flashgg {

    class PDFWeightObject : public GenEventInfoProduct
    {
    public:

        PDFWeightObject();
        //PDFWeightObject( const PDFWeightObject& );
        ~PDFWeightObject();

        vector<float> pdf_weight_container;
    };
}

#endif
