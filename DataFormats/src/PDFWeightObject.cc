#include "flashgg/DataFormats/interface/PDFWeightObject.h"

using namespace flashgg;
using namespace std;

PDFWeightObject::PDFWeightObject()
{}

//PDFWeightObject::PDFWeightObject( const PDFWeightObject& )
//{}

PDFWeightObject::~PDFWeightObject()
{}

vector<float> PDFWeightObject::uncompress( vector<uint16_t>& vec ) const {

	vector<float> uncompressed_vector;

	int size = vec.size();	

	for(int i = 0; i<size; i++ ){

	  //		uint16_t compressed_weight = vec[i];

		float uncompressed_weight = MiniFloatConverter::float16to32( vec[i] );	

		uncompressed_vector.push_back( uncompressed_weight );
	}

	return uncompressed_vector;
}

