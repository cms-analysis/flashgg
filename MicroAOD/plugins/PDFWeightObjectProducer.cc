#include <memory>
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <cctype>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "flashgg/DataFormats/interface/PDFWeightObject.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHERunInfoProduct.h"
#include "DataFormats/PatCandidates/interface/libminifloat.h"

using namespace std;
using namespace edm;

namespace flashgg {

	class PDFWeightProducer : public edm::EDProducer
	{
		public:
			PDFWeightProducer( const edm::ParameterSet & );
		private:
			void produce( edm::Event &, const edm::EventSetup & );
			EDGetTokenT<LHEEventProduct> LHEEventToken_;
			string tag_;
			string pdfset_;
			string delimiter_1_;
			string delimiter_2_;
			string delimiter_3_;
			void beginRun( edm::Run const &, edm::EventSetup const &iSetup );
			vector<int> weight_indices;
			string removeSpace( string line );
	};

	PDFWeightProducer::PDFWeightProducer( const edm::ParameterSet &iConfig ):
		LHEEventToken_( consumes<LHEEventProduct>( iConfig.getUntrackedParameter<InputTag>( "LHEEventTag", InputTag( "LHEEventProduct" ) ) ) )
	{

		tag_ = iConfig.getUntrackedParameter<string>( "tag", "initrwgt" );
		pdfset_ = iConfig.getUntrackedParameter<string>( "pdfset", "PDF_variation" );
		delimiter_1_ = iConfig.getUntrackedParameter<string>( "delimiter_1", "id=\"" );
		delimiter_2_ = iConfig.getUntrackedParameter<string>( "delimiter_2", "\">" );
		delimiter_3_ = iConfig.getUntrackedParameter<string>( "delimiter_3", "</weightgroup>" );

		produces<vector<flashgg::PDFWeightObject> >();

	}

	string removeSpaces( string str )
	{
		str.erase( remove_if( str.begin(), str.end(), ::isspace ), str.end() );
		return str;
	}

	void PDFWeightProducer::beginRun( edm::Run const &iRun, edm::EventSetup const &iSetup )
	{
		Handle<LHERunInfoProduct> run;
		typedef vector<LHERunInfoProduct::Header>::const_iterator headers_const_iterator;

		iRun.getByLabel( "externalLHEProducer", run );
		LHERunInfoProduct myLHERunInfoProduct = *( run.product() );

		int upper_index = 0;

		for( headers_const_iterator iter = myLHERunInfoProduct.headers_begin(); iter != myLHERunInfoProduct.headers_end(); iter++ ) {
			if( ( iter->tag() ).compare( tag_ ) == 0 ) {
				//cout << iter->tag() << endl;
				vector<string> lines = iter->lines();
			for( unsigned int iLine = 0; iLine < lines.size(); iLine++ ) {
					string line = lines.at( iLine );
					//cout << line << endl;
					size_t pos = line.find( pdfset_ );
					string token;
					while( ( pos = line.find( pdfset_ ) ) != std::string::npos ) {
						token = line.substr( pos, pdfset_.length() );
						//std::cout << token << std::endl;
						if( token.compare( pdfset_ ) == 0 ) {
							upper_index = 1 + iLine;
							break;
						} else {

							upper_index = 0;

						}

					}

					if( upper_index != 0 ) { break; }
				}

				for( unsigned int nLine = upper_index; nLine < lines.size(); nLine++ ) {

					string nline = lines.at( nLine );

					//cout << nline << endl;	
					
					string jline = removeSpaces( nline );

					//cout << nline.length() << endl;

					if( jline.compare( delimiter_3_ ) == 0 ) { break; }

					string ntoken;
					string mtoken;

					size_t mpos_1 = jline.find( delimiter_1_ );
					size_t mpos_3 = jline.find( delimiter_2_ );

					ntoken = jline.erase( mpos_3 );
					mtoken = jline.substr( mpos_1 + delimiter_1_.length() );
					//cout << mtoken << endl;
	

					int wgt = stoi( mtoken );

					PDFWeightProducer::weight_indices.push_back( wgt );

				}

				break;

			}
		}

	}



	void PDFWeightProducer::produce( Event &evt, const EventSetup & )
	{
		Handle<LHEEventProduct> LHEEventHandle;
		evt.getByToken( LHEEventToken_, LHEEventHandle );

		std::auto_ptr<vector<flashgg::PDFWeightObject> > PDFWeight( new vector<flashgg::PDFWeightObject> );

		flashgg::PDFWeightObject pdfWeight;

		float weight = 1;
		uint16_t weight_16 =1;

		//cout << "weight_container size " << LHEEventHandle->weights().size() << " num_weight " << PDFWeightProducer::weight_indices.size() << endl;
		//int lower_bound = LHEEventHandle->weights().size() - PDFWeightProducer::weight_indices.size();
		int upper_bound = LHEEventHandle->weights().size();
		int size = PDFWeightProducer::weight_indices.size();
		//cout << "lower_bound " << lower_bound << " upper_bound " << upper_bound << endl;
		for( int i = 0; i < upper_bound; i++ ) {

			int id_i = stoi( LHEEventHandle->weights()[i].id );

			for( int j = 0; j<size; j++ ){

				int id_j = PDFWeightProducer::weight_indices[j];	
//				cout << "id_i " << id_i << " id_j " << id_j << endl;
			if( id_i == id_j ){

				        //cout << "inner index " << j << " index " << PDFWeightProducer::weight_indices[j] << " id " << LHEEventHandle->weights()[i].id << endl;
					weight = LHEEventHandle->weights()[i].wgt / LHEEventHandle->originalXWGTUP();
					//cout << "weight " << weight << endl;
					weight_16 = MiniFloatConverter::float32to16(weight);

					pdfWeight.pdf_weight_container.push_back( weight_16 );
				}

			}
		}

		PDFWeight->push_back( pdfWeight );

		evt.put( PDFWeight );

		pdfWeight.pdf_weight_container.size();

	}

}

typedef flashgg::PDFWeightProducer FlashggPDFWeightProducer;
DEFINE_FWK_MODULE( FlashggPDFWeightProducer );

