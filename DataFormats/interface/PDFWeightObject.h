#ifndef FLASHgg_PDFWeightObject_h
#define FLASHgg_PDFWeightObject_h

#include "DataFormats/Math/interface/libminifloat.h"
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

	vector<uint16_t> pdfnlo_weight_container;

	vector<uint16_t> alpha_s_container;

	vector<uint16_t> qcd_scale_container;
 
        vector<float> uncompress( vector<uint16_t>& ) const;
        
      };
 }

#endif
