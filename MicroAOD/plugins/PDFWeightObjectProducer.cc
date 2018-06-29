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
#include "FWCore/Framework/interface/Run.h"


#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

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
        void beginRun( edm::Run const &, edm::EventSetup const &iSetup );
        vector<int> pdf_indices;
        vector<int> alpha_indices;
        vector<int> scale_indices;
        vector<int> pdfnlo_indices;
        PDFWeightsHelper pdfweightshelper_;//tool from HepMCCandAlgos/interface/PDFWeightsHelper
        unsigned int nPdfEigWeights_;
        edm::FileInPath mc2hessianCSV;
        std::vector<double> inpdfweights;
        float gen_weight;
        string pdfset_;
        string pdfid_1;
        string pdfid_2;
        string alphas_id_1;
        string alphas_id_2;
        bool isStandardSample_;
        bool isThqSample_;
        bool doAlphasWeights_;
        bool doScaleWeights_;
        string generatorType ;

        string runLabel_;
        bool debug_;
	};
    
	PDFWeightProducer::PDFWeightProducer( const edm::ParameterSet &iConfig ):
		LHEEventToken_( consumes<LHEEventProduct>( iConfig.getParameter<InputTag>( "LHEEventTag" ) ) ),
        srcTokenGen_( consumes<GenEventInfoProduct>( iConfig.getParameter<InputTag>("GenTag") ) ),
        runLabel_( iConfig.getParameter<string>("LHERunLabel") ),
        debug_( iConfig.getParameter<bool>("Debug") )
	{
		tag_ = iConfig.getUntrackedParameter<string>( "tag", "initrwgt" );
        isStandardSample_ = iConfig.getUntrackedParameter<bool>("isStandardSample",true);
        isThqSample_ = iConfig.getUntrackedParameter<bool>("isThqSample",false);
        doAlphasWeights_ = iConfig.getUntrackedParameter<bool>("doAlphasWeights",true);
        doScaleWeights_ = iConfig.getUntrackedParameter<bool>("doScaleWeights",true); 
        pdfset_ = iConfig.getUntrackedParameter<string>("pdfset","NNPDF30_lo_as_0130.LHgrid");
        nPdfEigWeights_ = iConfig.getParameter<unsigned int>("nPdfEigWeights");
        mc2hessianCSV = iConfig.getParameter<edm::FileInPath>("mc2hessianCSV");

        consumes<LHERunInfoProduct,edm::InRun> (edm::InputTag(runLabel_));
        
		produces<vector<flashgg::PDFWeightObject> >();
	}

    
    
	void PDFWeightProducer::beginRun( edm::Run const &iRun, edm::EventSetup const &iSetup )
	{

        pdf_indices.clear();
        scale_indices.clear();
        alpha_indices.clear();
        pdfnlo_indices.clear();

		Handle<LHERunInfoProduct> run;
		typedef vector<LHERunInfoProduct::Header>::const_iterator headers_const_iterator;
        
        iRun.getByLabel( runLabel_, run );
		LHERunInfoProduct myLHERunInfoProduct = *( run.product() );
        
        //--- get info from LHERunInfoProduct
		vector<string> weight_lines;
		for( headers_const_iterator iter = myLHERunInfoProduct.headers_begin(); iter != myLHERunInfoProduct.headers_end(); iter++ ) {
            
            vector<string> lines = iter->lines();
            if (debug_) {
                for (unsigned int iLine = 0; iLine<lines.size(); iLine++) {
                    std::cout << lines.at(iLine);
                }
            }
            
			if( ( iter->tag() ).compare( tag_ ) == 0 ) {
				//cout << iter->tag() << endl;
				weight_lines = iter->lines();
			}

            for (unsigned int iLine = 0; iLine<lines.size(); iLine++) {
                if (lines[iLine].find("powheg") != std::string::npos){
                    generatorType = "powheg";
                    break;
                }
            }
        }


        // --- Get the PDF ids -
        // See Josh's slides 13-15: https://indico.cern.ch/event/459797/contribution/2/attachments/1181555/1800214/mcaod-Feb15-2016.pdf
        int pdfidx = 0;
        pdfidx = run->heprup().PDFSUP.first;
        if (pdfidx == -1 && generatorType == "powheg"){
            pdfidx = 260000;    
        }
        cout <<" This sample was generated with the following PDFs : " << pdfidx <<endl;
        
        // --- Get min and max pdf index for 100 replicas
        if (!isStandardSample_) { pdfidx = 0; } 
        pdfid_1 = boost::lexical_cast<std::string>(pdfidx + 1);
        pdfid_2 = boost::lexical_cast<std::string>(pdfidx + 100);

        cout << "PDFs min and max id for MC replicas: " << pdfid_1 << "   " << pdfid_2 <<endl;
        

        // --- Get alphas id
        if (doAlphasWeights_){
         
            if (pdfidx == 292200){
                alphas_id_1 = "292301";
                alphas_id_2 = "292302";
            }
            
            if (pdfidx == 292000){
                alphas_id_1 = "292101";
                alphas_id_2 = "292102";
            }
            
            if (pdfidx == 260000){
                alphas_id_1 = "265000";
                alphas_id_2 = "266000";
            }
            
            if (pdfidx == 260400){
                alphas_id_1 = "265400";
                alphas_id_2 = "266400";
            }

            if (pdfidx == 306000){
                alphas_id_1 = "306101";
                alphas_id_2 = "306102";
            }
                
            cout << "alpha_S min and max id             : " << alphas_id_1 << "   " << alphas_id_2 << endl;
        }


        // --- for amcatnlo samples, remove last line
        if ( weight_lines.back() == "  <")
            weight_lines.pop_back();
                
        // --- Convert weight_lines in std::stringstream object and populate boost::property_tree
        std::stringstream ss;
        std::copy(weight_lines.begin(), weight_lines.end(),std::ostream_iterator<std::string>(ss,""));
        //cout << ss.str()<<endl;
        boost::property_tree::ptree pt;
        read_xml( ss , pt);
        
        
        // --- Name of the weightgroup
        string scalevar = "scale_variation";
        string pdfvar   = "PDF_variation";
        string alphavar;  //only for thq samples
        string pdfnlovar; //only for thq samples (could be extended to the rest of samples)

        //        if (pdfidx == 306000) { // 2017?? -  not for reminiaod it seems
        //            pdfvar = "PDF_variation NNPDF31_nnlo_hessian_pdfas";
        //        }

        if (!isStandardSample_){
            pdfvar = pdfset_;
            scalevar = "Central scale variation";
            if (isThqSample_){
                pdfvar = "NNPDF30_lo_as_0130.LHgrid";
                alphavar = "NNPDF30_lo_as_0118.LHgrid";
                pdfnlovar = "NNPDF30_nlo_as_0118.LHgrid";
            }
        }

        // --- Loop over elements to get PDF, alpha_s and scale weights
        BOOST_FOREACH( boost::property_tree::ptree::value_type const& v, pt.get_child("") ) {
            
            if (v.first == "weightgroup" || v.first == "uknown"){
                //std::cout << "First data: " << v.first.data() << std::endl;
                
                boost::property_tree::ptree subtree = (boost::property_tree::ptree) v.second ;
                
                boost::optional<std::string> weightgroupname1 = v.second.get_optional<std::string>("<xmlattr>.name");
                boost::optional<std::string> weightgroupname2 = v.second.get_optional<std::string>("<xmlattr>.type");
                
                if (debug_) {
                    std::cout << "weightgroupname1 and weightgroupname2 are:" << std::endl;
                    if (weightgroupname1) cout <<  weightgroupname1.get() <<endl;
                    if (weightgroupname2) cout <<  weightgroupname2.get() <<endl;
                }

                std::cout << " before pdf weights, pdfvar is " << pdfvar << std::endl;

                // -- PDFs + alpha_s weights
                if ( (weightgroupname1 && weightgroupname1.get().substr(0,pdfvar.length()) == pdfvar) || (weightgroupname2 && weightgroupname2.get().substr(0,pdfvar.length()) == pdfvar)) {
                                    
                    BOOST_FOREACH(boost::property_tree::ptree::value_type &vs,subtree)
                        if (vs.first == "weight") {
                            if (debug_) std::cout << "SCZ " << vs.first <<  "   " << vs.second.get<std::string>("<xmlattr>.id")  << "  " << vs.second.data()<< endl;
                            
                            string strwid  = vs.second.get<std::string>("<xmlattr>.id");
                            string strw    = vs.second.data();

                            int id;
                            try {
                                id = boost::lexical_cast<int>(strwid);
                            } catch (boost::bad_lexical_cast) {
                                std::cout << "conversion failed" << std::endl;
                            }
                            vector<string> strs;

                            if (isStandardSample_){
                                boost::split(strs, strw, boost::is_any_of("="));
                            }
                            else{
                                boost::split(strs, strw, boost::is_any_of(" "));
                            }
                            
                            int pdfindex = stoi(strs.back());
                            if (debug_) cout << "pdfindex " << pdfindex <<endl;

                            if (pdfindex >= boost::lexical_cast<int>(pdfid_1) && pdfindex <= boost::lexical_cast<int>(pdfid_2)){
                                PDFWeightProducer::pdf_indices.push_back( id );
                            }
                            
                            
                            if (doAlphasWeights_ && !isThqSample_){
                                if (pdfindex == boost::lexical_cast<int>(alphas_id_1) || pdfindex == boost::lexical_cast<int>(alphas_id_2)){
                                    PDFWeightProducer::alpha_indices.push_back( id );
                                }
                            }
                        }
                }// end loop over PDF weights

                if (debug_) {
                    std::cout << "before Scale weights" << std::endl;
                }

                // -- Scale weights
                if ( (weightgroupname1 && weightgroupname1.get().substr(0,scalevar.length()) == scalevar)  || ( weightgroupname2 && weightgroupname2.get().substr(0,scalevar.length()) == scalevar) ) {               
                    
                    BOOST_FOREACH(boost::property_tree::ptree::value_type &vs,subtree)
                        if (vs.first == "weight") {
                                                
                            string strwid  = vs.second.get<std::string>("<xmlattr>.id");
                            int id = boost::lexical_cast<int>(strwid);
                            
                            scale_indices.push_back( id );
                        }
                }// end loop over scale weights
                
                // -- Alpha_s weights                                                                                                                                                                        
                if ( isThqSample_ && ( (weightgroupname1 && weightgroupname1.get() == alphavar)  || ( weightgroupname2 && weightgroupname2.get() == alphavar) ) ) {

                    BOOST_FOREACH(boost::property_tree::ptree::value_type &vs,subtree)
                        if (vs.first == "weight") {

                            string strwid  = vs.second.get<std::string>("<xmlattr>.id");
                            int id = boost::lexical_cast<int>(strwid);

                            PDFWeightProducer::alpha_indices.push_back( id );
                        }
                }// end loop over alpha_s weights
                
                
                // -- PDF NLO weights                                                                                                                                                                        
                if ( isThqSample_ && ( (weightgroupname1 && weightgroupname1.get() == pdfnlovar)  || ( weightgroupname2 && weightgroupname2.get() == pdfnlovar) ) ) {

                    BOOST_FOREACH(boost::property_tree::ptree::value_type &vs,subtree)
                        if (vs.first == "weight") {

                            string strwid  = vs.second.get<std::string>("<xmlattr>.id");
                            int id = boost::lexical_cast<int>(strwid);

                            PDFWeightProducer::pdfnlo_indices.push_back( id );
                        }
                }// end loop over pdf nlo weights
            }
        }

        if (debug_) {
            std::cout << "pdf_indices:";
            for( unsigned int j = 0; j < PDFWeightProducer::pdf_indices.size(); j++ ){
                int id_j = PDFWeightProducer::pdf_indices[j];
                std::cout << " " << id_j;
            }
            std::cout <<std::endl;

            std::cout << "alpha_indices:";
            for( unsigned int j = 0; j < PDFWeightProducer::alpha_indices.size(); j++ ){
                int id_j = PDFWeightProducer::alpha_indices[j];
                std::cout << " " << id_j;
            }
            std::cout <<std::endl;

            std::cout << "scale_indices:";
            for( unsigned int j = 0; j < PDFWeightProducer::scale_indices.size(); j++ ){
                int id_j = PDFWeightProducer::scale_indices[j];
                std::cout << " " << id_j;
            }
            std::cout <<std::endl;
        }


    }



	void PDFWeightProducer::produce( Event &evt, const EventSetup & )
	{

        //cout << "Entering PDFWeightProducer::produce " <<endl;

		Handle<LHEEventProduct> LHEEventHandle;
		evt.getByToken( LHEEventToken_, LHEEventHandle );

        Handle<GenEventInfoProduct> genInfo;
        evt.getByToken( srcTokenGen_, genInfo );
    
        gen_weight = genInfo->weight();
        //cout << "gen weight = " << gen_weight <<endl;


		std::unique_ptr<vector<flashgg::PDFWeightObject> > PDFWeight( new vector<flashgg::PDFWeightObject> );

		inpdfweights.clear(); 

		flashgg::PDFWeightObject pdfWeight;


        for( unsigned int i = 0; i < LHEEventHandle->weights().size(); i++) {
            if (debug_) {
                std::cout << i << " " << LHEEventHandle->weights()[i].id << std::endl;
            }
            int id_i;
            try {
                id_i = boost::lexical_cast<int>( LHEEventHandle->weights()[i].id ); //.c_str()
            } catch ( boost::bad_lexical_cast ) {
                std::cout << "conversion failed" << std::endl;
                continue;
            }
			
            // --- Get PDF weights
            for( unsigned int j = 0; j < PDFWeightProducer::pdf_indices.size(); j++ ){
				int id_j = PDFWeightProducer::pdf_indices[j];	
                if( id_i == id_j ){
                    float weight = LHEEventHandle->weights()[i].wgt;
                    inpdfweights.push_back( weight );
				}
			}
            
            // --- Get alpha_s weights
            if ( doAlphasWeights_ ){
                for( unsigned int k = 0; k < PDFWeightProducer::alpha_indices.size(); k++ ){
                    int id_k = PDFWeightProducer::alpha_indices[k];
                    if(id_i == id_k ){
                        float alpha = LHEEventHandle->weights()[i].wgt;
                        uint16_t alpha_16 = MiniFloatConverter::float32to16( alpha );
                        pdfWeight.alpha_s_container.push_back(alpha_16);	
                    }
                }
            }
            
            // --- Get qcd scale weights
            if (doScaleWeights_ ){ 
                //                std::cout << "We want to do scale weights! scale_indices size is " << PDFWeightProducer::scale_indices.size() << std::endl;
                for( unsigned int k = 0 ; k < PDFWeightProducer::scale_indices.size() ; k++ ) {
                    int id_k = PDFWeightProducer::scale_indices[k];
                    //                    std::cout << k << " " << id_k << " " << id_i << std::endl;
                    if ( id_i == id_k ) {
                        float scale = LHEEventHandle->weights()[i].wgt;
                        uint16_t scale_16 = MiniFloatConverter::float32to16( scale );
                        //                        std::cout << " Filling scale " << std::endl;
                        pdfWeight.qcd_scale_container.push_back( scale_16 );
                    }
                }
            }
             
            // --- Get PDF NLO scale weights
            if (isThqSample_){ 
                for( unsigned int k = 0 ; k < PDFWeightProducer::pdfnlo_indices.size() ; k++ ) {
                    int id_k = PDFWeightProducer::pdfnlo_indices[k];
                    if ( id_i == id_k ) {
                        float nloweight = LHEEventHandle->weights()[i].wgt;
                        uint16_t nloweight_16 = MiniFloatConverter::float32to16( nloweight );
                        pdfWeight.pdfnlo_weight_container.push_back( nloweight_16 );
                    }
                }
            }
            
		}// end loop over lhe weights

        if (debug_) {
            cout << "Size of pdf weights    : " << inpdfweights.size() << endl;
            cout << "Size of scale weights  : " << PDFWeightProducer::scale_indices.size() << endl;
            cout << "Size of alpha_s weights: " << PDFWeightProducer::alpha_indices.size() << endl;
            if (isThqSample_)
                cout << "Size of pdf nlo weights: " << PDFWeightProducer::pdfnlo_indices.size() << endl;
        }

        if (!isThqSample_){
            assert(inpdfweights.size() > 0);
            assert(PDFWeightProducer::scale_indices.size() == 9);
            assert(PDFWeightProducer::alpha_indices.size() == 2);
        }
        
		
        // --- Get MCtoHessian PDF weights
        pdfweightshelper_.Init(PDFWeightProducer::pdf_indices.size(),nPdfEigWeights_,mc2hessianCSV);
        
        std::vector<double> outpdfweights(nPdfEigWeights_);
        
        double nomlheweight = LHEEventHandle->weights()[0].wgt;
		//cout << "nomlheweight " << nomlheweight << endl;
        
		pdfweightshelper_.DoMC2Hessian(nomlheweight,inpdfweights.data(),outpdfweights.data());
        
        for (unsigned int iwgt=0; iwgt<nPdfEigWeights_; ++iwgt) {
            
            double wgtval = outpdfweights[iwgt];
            
            //cout << "wgtval  " << wgtval << endl;
            
            float real_weight = wgtval*gen_weight/nomlheweight;
            
            uint16_t weight_16 = MiniFloatConverter::float32to16( real_weight );
			
            //cout << "real_weight " << real_weight << endl; 
            //cout << "weight_16 " << weight_16 << endl;
            
            //the is the weight to be used for evaluating uncertainties with hessian weights
            pdfWeight.pdf_weight_container.push_back(weight_16);
            
		}    
        
		PDFWeight->push_back( pdfWeight );

		evt.put( std::move( PDFWeight ) );
        
        /*
        cout << "FINAL pdf_weight_container size " <<pdfWeight.pdf_weight_container.size() << endl;
        for (unsigned int  i = 0 ; i < pdfWeight.pdf_weight_container.size() ; i++){
            cout << i << "  " << pdfWeight.pdf_weight_container.at(i)  << endl;
            //cout << i << "  " << (pdfWeight.uncompress(pdfWeight.pdf_weight_container)).at(i)  << endl;
        }
        
        cout << "FINAL alpha_s_container size " <<pdfWeight.alpha_s_container.size() << endl;
        for (unsigned int  i = 0 ; i < pdfWeight.alpha_s_container.size() ; i++){
            cout << i << "  " << pdfWeight.alpha_s_container.at(i) <<endl;
        }

        cout << "FINAL qcd_scale_container size " <<pdfWeight.qcd_scale_container.size() << endl;
        for (unsigned int  i = 0 ; i < pdfWeight.qcd_scale_container.size() ; i++){
            cout << i << "  " << pdfWeight.qcd_scale_container.at(i) <<endl;
        }
        */
        
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

//  LocalWords:  endl
