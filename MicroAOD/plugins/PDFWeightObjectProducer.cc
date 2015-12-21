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
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHERunInfoProduct.h"
#include "DataFormats/PatCandidates/interface/libminifloat.h"
#include "PhysicsTools/HepMCCandAlgos/interface/PDFWeightsHelper.h"

using namespace std;
using namespace edm;

namespace flashgg {

	class PDFWeightProducer : public edm::EDProducer
	{
		public:
			PDFWeightProducer( const edm::ParameterSet & );
			string generator_type( vector<string> );
		private:
			void produce( edm::Event &, const edm::EventSetup & );
			EDGetTokenT<LHEEventProduct> LHEEventToken_;
			EDGetTokenT<GenEventInfoProduct> srcTokenGen_;
			string tag_;
			string delimiter_1_;
			string delimiter_2_;
			void beginRun( edm::Run const &, edm::EventSetup const &iSetup );
			vector<int> weight_indices;
			string removeSpace( string line );
			PDFWeightsHelper pdfweightshelper_;
		        unsigned int nPdfEigWeights_;
//		        std::vector<float> pdfeigweights_;
	                edm::FileInPath mc2hessianCSV;
			std::vector<double> lhe_weights;
			float gen_weight;
			string pdfid_1;
			string pdfid_2;
	};

	PDFWeightProducer::PDFWeightProducer( const edm::ParameterSet &iConfig ):
		LHEEventToken_( consumes<LHEEventProduct>( iConfig.getUntrackedParameter<InputTag>( "LHEEventTag", InputTag( "LHEEventProduct" ) ) ) ),
	        srcTokenGen_( consumes<GenEventInfoProduct>( iConfig.getUntrackedParameter<InputTag>("GenTag", InputTag("generator") ) ) )
	{

		tag_ = iConfig.getUntrackedParameter<string>( "tag", "initrwgt" );
		pdfid_1 = iConfig.getUntrackedParameter<string>("pdfid_1","292201");
		pdfid_2 = iConfig.getUntrackedParameter<string>("pdfid_1","292302");
		delimiter_1_ = iConfig.getUntrackedParameter<string>( "delimiter_1", "id=\"" );
		delimiter_2_ = iConfig.getUntrackedParameter<string>( "delimiter_2", "\">" );
	        nPdfEigWeights_ = iConfig.getParameter<unsigned int>("nPdfEigWeights");

                mc2hessianCSV = iConfig.getParameter<edm::FileInPath>("mc2hessianCSV");

		produces<vector<flashgg::PDFWeightObject> >();

	}

	string removeSpaces( string str )
	{
		str.erase( remove_if( str.begin(), str.end(), ::isspace ), str.end() );
		return str;
	}

	string PDFWeightProducer::generator_type( vector<string> text ){

		string type;

		for(unsigned int line = 0; line<text.size(); line++ ){

			size_t found_mg = text.at(line).find("MadGraph");
			size_t found_ph = text.at(line).find("powheg");

			if (found_mg != string::npos ){

				pdfid_1 = "292201";
				pdfid_2 = "292300";
				type = "mg";
				break;
			}


			if ( found_ph != string::npos ){
			
				pdfid_1 = "260001";
				pdfid_2 = "260100";

				//alpha_s1 = "265000";
				//alpha_s2 = "266000";
				type = "ph";
				break;
			}

		}

		return type;

	}


	void PDFWeightProducer::beginRun( edm::Run const &iRun, edm::EventSetup const &iSetup )
	{
		Handle<LHERunInfoProduct> run;
		typedef vector<LHERunInfoProduct::Header>::const_iterator headers_const_iterator;

		iRun.getByLabel( "externalLHEProducer", run );
		LHERunInfoProduct myLHERunInfoProduct = *( run.product() );

		int upper_index = 0;
		vector<string> weight_lines;
		string generator;
		

		for( headers_const_iterator iter = myLHERunInfoProduct.headers_begin(); iter != myLHERunInfoProduct.headers_end(); iter++ ) {
			
			vector<string> lines = iter->lines();
	
			if( ( iter->tag() ).compare( tag_ ) == 0 ) {
				//cout << iter->tag() << endl;
				weight_lines = iter->lines();
			
			}


			generator = PDFWeightProducer::generator_type(lines);

			}

			for( unsigned int iLine = 0; iLine < weight_lines.size(); iLine++ ) {

				string line = weight_lines.at( iLine );
				size_t pos = line.find( pdfid_1 );
				string token;
				while( (pos = line.find(pdfid_1)) != std::string::npos ){
					token = line.substr(pos, pdfid_1.length() );
					if( token.compare( pdfid_1 ) == 0 ){
						upper_index = iLine;
						break;
					} else {				
			
						upper_index = 0;

					}


				}

				if( upper_index != 0 ){break;} 
			}

					for( unsigned int nLine = upper_index; nLine < weight_lines.size(); nLine++ ) {

						string nline = weight_lines.at( nLine );

						//cout << nline << endl;	
						string jline = removeSpaces( nline );
//						cout << "jline " << jline << endl;
						string jtoken;																					size_t jpos;				
			
						while( ( jpos = jline.find( pdfid_2 ) ) != std::string::npos ) {
							jtoken = jline.substr( jpos, pdfid_2.length() );
							//std::cout << "jtoken " << jtoken << std::endl;
							break;
						}
			
					
					string ntoken;
					string mtoken;

					size_t mpos_1 = jline.find( delimiter_1_ );
					size_t mpos_3 = jline.find( delimiter_2_ );

					ntoken = jline.erase( mpos_3 );
					mtoken = jline.substr( mpos_1 + delimiter_1_.length() );
					//cout << "mtoken " << mtoken << endl;
					
					int wgt = stoi( mtoken );

					PDFWeightProducer::weight_indices.push_back( wgt );

					if( jtoken.compare( pdfid_2 ) == 0 ) { break; }

					}

					//if (generator == "ph"){

					//	int alphas_1 = PDFWeightProducer::weight_indices.back() + 1;
					//	int alphas_2 = PDFWeightProducer::weight_indices.back() + 2;

					//	PDFWeightProducer::weight_indices.push_back(alphas_1);
					//	PDFWeightProducer::weight_indices.push_back(alphas_2);
			
					//}


			}	

 


	void PDFWeightProducer::produce( Event &evt, const EventSetup & )
	{
		Handle<LHEEventProduct> LHEEventHandle;
		evt.getByToken( LHEEventToken_, LHEEventHandle );

	        Handle<GenEventInfoProduct> genInfo;
                evt.getByToken( srcTokenGen_, genInfo );
    
                gen_weight = genInfo->weight();

		//cout << "gen " << gen_weight << endl;

		std::auto_ptr<vector<flashgg::PDFWeightObject> > PDFWeight( new vector<flashgg::PDFWeightObject> );

		lhe_weights.clear(); 

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
					weight = LHEEventHandle->weights()[i].wgt;

					//cout << "weights " << weight << endl;

					lhe_weights.push_back( weight );
				}

			}
		}

		//cout << "should be 100   " << lhe_weights.size() << endl;

		pdfweightshelper_.Init(size,nPdfEigWeights_,mc2hessianCSV);

	        std::vector<double> outpdfweights(nPdfEigWeights_);

	        double nomlheweight = LHEEventHandle->weights()[0].wgt;
		//cout << "nomlheweight " << nomlheweight << endl;

		pdfweightshelper_.DoMC2Hessian(nomlheweight,lhe_weights.data(),outpdfweights.data());

	    	for (unsigned int iwgt=0; iwgt<nPdfEigWeights_; ++iwgt) {
      			
			 double wgtval = outpdfweights[iwgt];
		
			 //cout << "wgtval  " << wgtval << endl;
	
			 float real_weight = wgtval*gen_weight/nomlheweight;

	   		 weight_16 = MiniFloatConverter::float32to16( real_weight );
			 
			 //cout << "real_weight " << real_weight << endl; 
     			 //cout << "weight_16 " << weight_16 << endl;
    
		         //the is the weight to be used for evaluating uncertainties with hessian weights
     			 pdfWeight.pdf_weight_container.push_back(weight_16);
   
		}    
	
		PDFWeight->push_back( pdfWeight );

		evt.put( PDFWeight );

//		cout << "final size " <<pdfWeight.pdf_weight_container.size() << endl;

	}

}

typedef flashgg::PDFWeightProducer FlashggPDFWeightProducer;
DEFINE_FWK_MODULE( FlashggPDFWeightProducer );
