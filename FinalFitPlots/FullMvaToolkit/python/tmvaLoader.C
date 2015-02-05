#include "TMVA/Reader.h" 
#include "TString.h"
#include <string>

class tmvaLoader{

	 float _bdtoutput;
	 float _deltaMOverM;
         TMVA::Reader *tmvaReader_ ;
	 std::string xmlweights_;
	 std::string type_;

	public:
	  tmvaLoader(std::string,std::string);
		float tmvaGetVal(float bdt, float dmom);


};

tmvaLoader::tmvaLoader (std::string xml, std::string type){

	xmlweights_ = xml;
	type_=type;
	tmvaReader_ = new TMVA::Reader();
    	tmvaReader_->AddVariable("bdtoutput",&_bdtoutput);
    	tmvaReader_->AddVariable("deltaMOverM", &_deltaMOverM);
	tmvaReader_->BookMVA(Form("BDT_%s_123",type_.c_str()),xmlweights_.c_str());

}
	
float tmvaLoader::tmvaGetVal(float bdt, float dmom){
	
	_bdtoutput=bdt;
	_deltaMOverM=dmom;
	float bdt_grad = tmvaReader_->EvaluateMVA(Form("BDT_%s_123",type_.c_str()));	
	return bdt_grad;

}
