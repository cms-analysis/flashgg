#include "flashgg/DataFormats/interface/PDFWeightObject.h"

using namespace flashgg;
using namespace std;

PDFWeightObject::PDFWeightObject()
{}

PDFWeightObject::PDFWeightObject( const PDFWeightObject& )
{}

PDFWeightObject::~PDFWeightObject()
{}

vector<float> PDFWeightObject::uncompress() const {

	vector<float> uncompressed_vector;

	int size = PDFWeightObject::pdf_weight_container.size();	

	for(int i = 0; i<size; i++ ){

		uint16_t compressed_weight = PDFWeightObject::pdf_weight_container[i];

		float uncompressed_weight = MiniFloatConverter::float16to32( compressed_weight );	

		uncompressed_vector.push_back( uncompressed_weight );
	}

	return uncompressed_vector;
}

