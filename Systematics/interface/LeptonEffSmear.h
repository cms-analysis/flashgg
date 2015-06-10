#ifndef FLASHgg_LeptonEffSmear_h
#define FLASHgg_LeptonEffSmear_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "flashgg/Systematics/interface/BaseSystMethods.h"
#include "flashgg/DataFormats/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
//#include "flashgg/Systematics/interface/ObjectSystMethodBinnedByFunctor.h"

namespace flashgg {

	template <typename lepton,typename param_var>
		//class LeptonEffSmear : public ObjectSystMethodBinnedByFunctor<lepton,param_var> 
 		class LeptonEffSmear : public BaseSystMethods<lepton,param_var> 
	{
		public:
			//typedef StringCutObjectSelector<lepton, true> selector_type;
			struct LeptonBin {

				std::vector<double> bin_min;
				std::vector<double> bin_max;
				std::vector<double> value;
				std::vector<double> errors;

	
				LeptonBin( std::vector<double> min, std::vector<double> max, std::vector<double> val, std::vector<double> err ):
					bin_min(min), bin_max(max), value(val), errors(err){}
			};// bin_info_;

			LeptonEffSmear( const edm::ParameterSet & conf);
			void applyCorrection(lepton &lep, param_var syst_shift) override;
			std::string shiftLabel( param_var syst_shift ) override;
		private:
			//selector_type overall_range_;
			bool debug_;
			std::vector<LeptonBin> bin_info_;
	};
	
        template<typename lepton,typename param_var>
	LeptonEffSmear<lepton,param_var>::LeptonEffSmear( const edm::ParameterSet &conf) :
		BaseSystMethods<lepton,param_var>( conf ),
//		overall_range_(conf.getParameter<std::string>("OverallRange") ),
		debug_( conf.getUntrackedParameter<bool>("Debug",false) )
	{
	    const auto &pset = conf.getParameterSet( "BinList" );

	for ( const auto b : pset.getParameterSetVector("bins") ) {
                	bin_info_.emplace_back(b.getParameter<std::vector<double> >("lowBounds"),
                                   b.getParameter<std::vector<double> >("upBounds"),
                                   b.getParameter<std::vector<double> >("values"),
                                   b.getParameter<std::vector<double> >("uncertainty"));


		}

	}

	template<typename lepton,typename param_var>
	std::string LeptonEffSmear<lepton,param_var>::shiftLabel( param_var syst_value ){
		std::string result;
		if( syst_value == 0 ) {
			result = Form( "%sCentral", this->label().c_str() );
		} else if( syst_value > 0 ) {
			result = Form( "%sUp%.2dsigma", this->label().c_str(), syst_value );
		} else {
			result = Form( "%sDown%.2dsigma", this->label().c_str(), -1 * syst_value );
		}
		return result;
	}


	template<typename lepton,typename param_var>
 	void LeptonEffSmear<lepton, param_var>::applyCorrection(lepton& lep,param_var syst_shift)
	{	
		
		int myBin = -1;
		//double xPrevious = -1e9;
		double pt = lep.pt();
		int num_bins = bin_info_.size();

		for(int bin = 0; bin < num_bins; bin++){
			if ( pt > bin_info_[bin].bin_max[0] ) {
				myBin = bin;
			}
			else break;
		}

		int binLow = 0; 
		int binHigh = 0;  
		bool atBoundary(false);

		if (myBin == -1) {
			binHigh = 0; binLow=0; atBoundary=true;

		} else if (myBin == num_bins) {

			binHigh = bin_info_.size(); 
			binLow=bin_info_.size(); 
			atBoundary=true;

		} else {

			 binLow=myBin; 
			 binHigh=myBin+1;
		}
		

		double xLow=bin_info_[binLow].bin_min[0]; 
		double xHigh=bin_info_[binHigh].bin_max[0];
		double yLow=bin_info_[binLow].value[0];
		double yHigh=bin_info_[binHigh].value[0];

		double errLowYup    = bin_info_[binLow].errors[0];  
		double errLowYdown  = bin_info_[binLow].errors[1];
		double errHighYup   = bin_info_[binHigh].errors[0];
		double errHighYdown = bin_info_[binHigh].errors[1];


		double theErrorLow; 
		double theErrorHigh;

		if(syst_shift>0) {
			theErrorLow = errLowYup;   
			theErrorHigh = errHighYup;
		} else {
			theErrorLow = errLowYdown; 
			theErrorHigh = errHighYdown;
		}

		double theWeight=1;
		double  theError=0;

		if(!atBoundary) {
			theWeight = yLow + (yHigh-yLow) / (xHigh-xLow) * (pt-xLow);
			theError  = theErrorLow + (theErrorHigh-theErrorLow) / (xHigh-xLow) * (pt-xLow);
		} else {

			if(myBin == num_bins) {
				theWeight = yHigh; 	  
				theError  = theErrorHigh;	
			} else if (myBin == -1) {
				theWeight = yLow; 	  
				theError  = theErrorLow; 	
			} else {   

				std::cout << " ** you claim to be at boundaries of TGraphAsymmErrors but your not! This is a problem " << std::endl;

			}
		} 
		double Weight = theWeight + (theError*syst_shift);
		lep.addUserFloat("effweight",Weight);
	}

}
#endif
