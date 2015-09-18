#ifndef FLASHgg_PDFWeightObject_h
#define FLASHgg_PDFWeightObject_h

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "DataFormats/PatCandidates/interface/libminifloat.h"
#include <vector>

using namespace std;

namespace flashgg {

    class PDFWeightObject : public GenEventInfoProduct
    {
    public:

        PDFWeightObject();
        PDFWeightObject( const PDFWeightObject& );
        ~PDFWeightObject();
        
        vector<uint16_t> pdf_weight_container;
        
        vector<float> uncompress() const;
        
      };
 }

#endif
