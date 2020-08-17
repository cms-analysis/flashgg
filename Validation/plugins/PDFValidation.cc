#include <memory>

#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHERunInfoProduct.h"
#include "flashgg/DataFormats/interface/PDFWeightObject.h"
#include <iostream>
#include <string>

using namespace std;
using namespace edm;

class PDFWeight : public edm::EDAnalyzer
{
	public:
		explicit PDFWeight( const edm::ParameterSet & );
		~PDFWeight();

	private:

		virtual void beginJob() override;
		virtual void analyze( const edm::Event &, const edm::EventSetup & ) override;
		virtual void endJob() override;
		virtual void beginRun( edm::Run const &, edm::EventSetup const &iSetup );

		edm::EDGetTokenT<GenEventInfoProduct> genParticleToken_;
		edm::EDGetTokenT<LHEEventProduct> LHEEventToken_;
		edm::EDGetTokenT<LHERunInfoProduct> LHERunToken_;
		edm::EDGetTokenT<vector<flashgg::PDFWeightObject> > WeightToken_;
};

PDFWeight::PDFWeight( const edm::ParameterSet &iConfig ):
	genParticleToken_( consumes<GenEventInfoProduct>( iConfig.getUntrackedParameter<InputTag> ( "GenEventInfoProductTag", InputTag( "generator" ) ) ) ),
	LHEEventToken_( consumes<LHEEventProduct>( iConfig.getUntrackedParameter<InputTag> ( "LHETag", InputTag( "LHEEventProduct" ) ) ) ),
	LHERunToken_( consumes<LHERunInfoProduct>( iConfig.getUntrackedParameter<InputTag> ( "LHERunTag", InputTag( "LHERunInfoProduct" ) ) ) ),
	WeightToken_( consumes<vector<flashgg::PDFWeightObject> >( iConfig.getUntrackedParameter<InputTag>( "WeightTag", InputTag( "flashggPDFWeightObject" ) ) ) )
{

}

PDFWeight::~PDFWeight()
{
}

	void
PDFWeight::analyze( const edm::Event &evt, const edm::EventSetup &iSetup )
{

	//    Handle<GenEventInfoProduct> genParticles;
	//    evt.getByToken( genParticleToken_, genParticles );
	//    const PtrVector<GenEventInfoProduct>& gens = genParticles->ptrVector();

	//   cout << "the event weight  " << genParticles->weight() << endl;

	//    Handle<LHEEventProduct> LHEHandle;
	//    evt.getByToken( LHEEventToken_, LHEHandle );

	    Handle<vector<flashgg::PDFWeightObject> > WeightHandle;
	    evt.getByToken( WeightToken_, WeightHandle );
	//   const PtrVector<flashgg::PDFWeightObject> mcWeightPointers = WeightHandle->ptrVector();

	//cout << "XS  " << LHEHandle->originalXWGTUP() << endl;

	        for( unsigned int weight_index = 0; weight_index < (*WeightHandle).size(); weight_index++ ){
	            vector<uint16_t> compressed_weights = (*WeightHandle)[weight_index].pdf_weight_container; 
                std::vector<float> uncompressed = (*WeightHandle)[weight_index].uncompress( compressed_weights );
                vector<uint16_t> compressed_alpha = (*WeightHandle)[weight_index].alpha_s_container;
                std::vector<float> uncompressed_alpha = (*WeightHandle)[weight_index].uncompress( compressed_alpha );
                vector<uint16_t> compressed_scale = (*WeightHandle)[weight_index].qcd_scale_container;
                std::vector<float> uncompressed_scale = (*WeightHandle)[weight_index].uncompress( compressed_scale );

	            for( unsigned int j=0; j<(*WeightHandle)[weight_index].pdf_weight_container.size();j++ ) {
	                    cout << "compresed weight " << (*WeightHandle)[weight_index].pdf_weight_container[j] << endl;
	                    cout << "uncompressed weight " << uncompressed[j] << endl;
	       }
                for( unsigned int j=0; j<(*WeightHandle)[weight_index].alpha_s_container.size();j++ ) {
                        cout << "compressed variation " << (*WeightHandle)[weight_index].alpha_s_container[j] << endl;
                        cout << "uncompressed variation " << uncompressed_alpha[j] << endl;
           }
                for( unsigned int j=0; j<(*WeightHandle)[weight_index].qcd_scale_container.size();j++ ) {
                        cout << "compressed scale " << (*WeightHandle)[weight_index].qcd_scale_container[j] << endl;
                        cout << "uncompressed scale " << uncompressed_scale[j] << endl;
           }
	    
    }
}


	void
PDFWeight::beginJob()
{
}

	void
PDFWeight::endJob()
{
}


	void
PDFWeight::beginRun( edm::Run const &iRun, edm::EventSetup const &iSetup )
{
//	Handle<LHERunInfoProduct> run;
//	typedef vector<LHERunInfoProduct::Header>::const_iterator headers_const_iterator;
//
//	iRun.getByLabel( "externalLHEProducer", run );
//	LHERunInfoProduct myLHERunInfoProduct = *( run.product() );
//
//	for( headers_const_iterator iter = myLHERunInfoProduct.headers_begin(); iter != myLHERunInfoProduct.headers_end(); iter++ ) {
//		if( iter->tag() == "init" )
//		{ cout << iter->tag() << endl; }
//		vector<string> lines = iter->lines();
//		for( unsigned int iLine = 0; iLine < lines.size(); iLine++ ) {
//			cout << lines.at( iLine );
//		}
//	}
}


/*
   void
   PDFWeight::endRun(edm::Run const&, edm::EventSetup const&)
   {
   }
   */

/*
   void
   PDFWeight::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
   {
   }
   */

/*
   void
   PDFWeight::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
   {
   }
   */

DEFINE_FWK_MODULE( PDFWeight );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
