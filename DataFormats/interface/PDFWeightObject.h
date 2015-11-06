#ifndef FLASHgg_PDFWeightObject_h
#define FLASHgg_PDFWeightObject_h

#include "DataFormats/PatCandidates/interface/libminifloat.h"
#include <vector>

using namespace std;

namespace flashgg {

    class PDFWeightObject
    {
    public:

        PDFWeightObject();
        //PDFWeightObject( const PDFWeightObject& );
        ~PDFWeightObject();
        
        vector<uint16_t> pdf_weight_container;
        
        vector<float> uncompress() const;
        
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
