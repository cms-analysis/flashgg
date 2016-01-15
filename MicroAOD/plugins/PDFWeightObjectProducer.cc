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
#include "FWCore/Utilities/interface/EDMException.h"
using namespace std;
using namespace edm;

namespace flashgg {
    
	class PDFWeightProducer : public edm::EDProducer
	{
    public:
        PDFWeightProducer( const edm::ParameterSet & );
        void set_generator_type( vector<string> );
    private:
        void produce( edm::Event &, const edm::EventSetup & );
        EDGetTokenT<LHEEventProduct> LHEEventToken_;
        EDGetTokenT<GenEventInfoProduct> srcTokenGen_;
        string tag_;
        string delimiter_1_;
        string delimiter_2_;
        void beginRun( edm::Run const &, edm::EventSetup const &iSetup );
        vector<int> weight_indices;
        vector<int> alpha_indices;
        vector<int> scale_indices;
        string removeSpace( string line );
        PDFWeightsHelper pdfweightshelper_;//tool from HepMCCandAlgos/interface/PDFWeightsHelper
        unsigned int nPdfEigWeights_;
        //		        std::vector<float> pdfeigweights_;
        edm::FileInPath mc2hessianCSV;
        std::vector<double> lhe_weights;
        float gen_weight;
        string pdfid_1;
        string pdfid_2;
	};
    
	PDFWeightProducer::PDFWeightProducer( const edm::ParameterSet &iConfig ):
		LHEEventToken_( consumes<LHEEventProduct>( iConfig.getParameter<InputTag>( "LHEEventTag" ) ) ),
        srcTokenGen_( consumes<GenEventInfoProduct>( iConfig.getParameter<InputTag>("GenTag") ) )
	{
        consumes<LHERunInfoProduct,edm::InRun> (edm::InputTag("externalLHEProducer"));
		tag_ = iConfig.getUntrackedParameter<string>( "tag", "initrwgt" );
		pdfid_1 = iConfig.getUntrackedParameter<string>("pdfid_1","0");
		pdfid_2 = iConfig.getUntrackedParameter<string>("pdfid_2","0");
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

	void PDFWeightProducer::set_generator_type( vector<string> text ){
        
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

        //        throw cms::Exception("UnexpectedInput") << " We were expecting this file generator info to be labelled as MadGraph or powheg";
        //        std::cout << "  WARNING: We get nothing but assume the PDF weight range is 292201-292300 (a la MadGraph)" << std::endl;
        //        pdfid_1 = "292201";
        //        pdfid_2 = "292300";
        // Actually if there is a noop we will process the next round of lines
        
	}
    
    
	void PDFWeightProducer::beginRun( edm::Run const &iRun, edm::EventSetup const &iSetup )
	{
		Handle<LHERunInfoProduct> run;
		typedef vector<LHERunInfoProduct::Header>::const_iterator headers_const_iterator;
        
        iRun.getByLabel( "externalLHEProducer", run );
		LHERunInfoProduct myLHERunInfoProduct = *( run.product() );

		int upper_index = 0;
		vector<string> weight_lines;
        //		string generator;
        
		for( headers_const_iterator iter = myLHERunInfoProduct.headers_begin(); iter != myLHERunInfoProduct.headers_end(); iter++ ) {
            
			vector<string> lines = iter->lines();
            
			if( ( iter->tag() ).compare( tag_ ) == 0 ) {
				//cout << iter->tag() << endl;
				weight_lines = iter->lines();
			}

            PDFWeightProducer::set_generator_type(lines);
            std::cout << " After set_generator_type on " << iter->tag() << " we have pdfids: " << pdfid_1 << " " << pdfid_2 << std::endl;
        }

        for( unsigned int iLine = 0; iLine < weight_lines.size(); iLine++ ) {
            if ( weight_lines[iLine].find("muR") != std::string::npos ) {
                //                std::cout << "Line for scale: " << weight_lines[iLine];
                size_t pos1 = weight_lines[iLine].find("\"");
                size_t pos2 = weight_lines[iLine].find("\"",pos1+1);
                assert (pos1 != std::string::npos && pos2 != std::string::npos);
                string tempstr = weight_lines[iLine].substr(pos1+1,pos2-pos1-1);
                int scaleind = atoi(tempstr.c_str());
                //                std::cout << "    " << pos1 << " " << pos2 << " " << tempstr << " " << scaleind << std::endl;
                scale_indices.push_back( scaleind );
            } else {
                //                std::cout << "Line NOT for scale: " << weight_lines[iLine];
            }
        }
        //        std::cout << std::endl;
        
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
            string jtoken;																					
            size_t jpos;				
			
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

        // See slide 13 here: https://indico.cern.ch/event/459797/contribution/2/attachments/1181555/1710844/mcaod-Nov4-2015.pdf
        int alphas_1 = PDFWeightProducer::weight_indices.back() + 1;
        int alphas_2 = PDFWeightProducer::weight_indices.back() + 2;
        PDFWeightProducer::alpha_indices.push_back(alphas_1);
        PDFWeightProducer::alpha_indices.push_back(alphas_2);

        //        std::cout << " Alpha indices: " << alphas_1 << " " << alphas_2 << std::endl;

        //        std::cout << " PDF weight indices final size: " << weight_indices.size() << std::endl;
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
		float alpha = 1;

		//cout << "weight_container size " << LHEEventHandle->weights().size() << " num_weight " << PDFWeightProducer::weight_indices.size() << endl;
		//int lower_bound = LHEEventHandle->weights().size() - PDFWeightProducer::weight_indices.size();
		int upper_bound = LHEEventHandle->weights().size();
		int size_weight = PDFWeightProducer::weight_indices.size();
		int size_alpha = PDFWeightProducer::alpha_indices.size();
		//cout << "lower_bound " << lower_bound << " upper_bound " << upper_bound << endl;
		for( int i = 0; i < upper_bound; i++ ) {
			int id_i = stoi( LHEEventHandle->weights()[i].id );
			for( int j = 0; j<size_weight; j++ ){
				int id_j = PDFWeightProducer::weight_indices[j];	
                //				cout << "id_i " << id_i << " id_j " << id_j << endl;
                if( id_i == id_j ){
                    //cout << "inner index " << j << " index " << PDFWeightProducer::weight_indices[j] << " id " << LHEEventHandle->weights()[i].id << endl;
					weight = LHEEventHandle->weights()[i].wgt;
					//cout << "weights " << weight << endl;
					lhe_weights.push_back( weight );
				}
			}
			for( int k = 0; k<size_alpha; k++ ){
				int id_k = PDFWeightProducer::alpha_indices[k];
                //                std::cout << " checking alpha_index " << id_k << " against id " << id_i << std::endl;
                if(id_i == id_k ){
                    alpha = LHEEventHandle->weights()[i].wgt;
                    //cout << " alpha " << alpha  << endl;
                    uint16_t alpha_16 = MiniFloatConverter::float32to16( alpha );
                    pdfWeight.alpha_s_container.push_back(alpha_16);	
                }
            }
            for( unsigned k = 0 ; k < PDFWeightProducer::scale_indices.size() ; k++ ) {
                int id_k = PDFWeightProducer::scale_indices[k];
                if ( id_i == id_k ) {
                    float scale = LHEEventHandle->weights()[i].wgt;
                    uint16_t scale_16 = MiniFloatConverter::float32to16( scale );
                    pdfWeight.qcd_scale_container.push_back( scale_16 );
                }
            }
		}

		//cout << "should be 100   " << lhe_weights.size() << endl;

		pdfweightshelper_.Init(size_weight,nPdfEigWeights_,mc2hessianCSV);
        
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

        //		cout << "FINAL pdf_weight_container size " <<pdfWeight.pdf_weight_container.size() << endl;
        //        cout << "FINAL alpha_s_container size " <<pdfWeight.alpha_s_container.size() << endl;
        //        cout << "FINAL qcd_scale_container size " <<pdfWeight.qcd_scale_container.size() << endl;


	}

}

typedef flashgg::PDFWeightProducer FlashggPDFWeightProducer;
DEFINE_FWK_MODULE( FlashggPDFWeightProducer );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
