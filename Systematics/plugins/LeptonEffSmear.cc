#include "flashgg/Systematics/interface/BaseSystMethods.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "flashgg/DataFormats/interface/Electron.h"
#include "flashgg/DataFormats/interface/Muon.h"

#include "TGraphAsymmErrors.h"
#include "TFile.h"

namespace flashgg {

	template <class lepton_obj>
	class LeptonEffSmear: public BaseSystMethods<lepton_obj,int>
	{

		public:
			LeptonEffSmear( const edm::ParameterSet &conf );	

			void applyCorrection( lepton_obj&, int syst_shift ) override;
			std::string shiftLabel( int ) override;
			TFile * f;
		private:
			double sigma_;

	};

	LeptonEffSmear::LeptonEffSmear( const edm::ParameterSet &conf) :
		BaseSystMethods( conf ),
		sigma_( conf.getParameter<double>( "Sigma" ) ){}

	std::string LeptonEffSmear::shiftLabel( int syst_value ){
		std::string result;
		if( syst_value == 0 ) {
			result = Form( "%sCentral", label().c_str() );
		} else if( syst_value > 0 ) {
			result = Form( "%sUp%.2dsigma", label().c_str(), syst_value );
		} else {
			result = Form( "%sDown%.2dsigma", label().c_str(), -1 * syst_value );
		}
		return result;
	}

	void LeptonEffSmear::applyCorrection(lepton_obj &lep, int syst_shift){

		f = TFile::Open("../data/Efficiency.root");
		TGraphAsymmErrors * graph = ( TGraphAsymmErrors * )f->Get("LepEff");	

		int numPoints = graph->GetN();
		double x; 
		double y;
		int myBin = -1;
		double xPrevious = -1e9;
		double pt = lep.pt();

		for (int bin=0; bin<numPoints; bin++ ){
			graph->GetPoint(bin, x, y);
			assert( xPrevious < x );
			if(pt > x) {
				myBin = bin; }
			else break;
		}

		int binLow; 
		int binHigh;  
		bool atBoundary(false);

		if (myBin == -1) {
			binHigh = 0; binLow=0; atBoundary=true;

		} else if (myBin == (numPoints-1)) {

			binHigh = numPoints-1; binLow=numPoints-1; atBoundary=true;

		} else {

			binLow=myBin; binHigh=myBin+1;

		}

		double xLow; 
		double yLow;    
		double xHigh; 
		double yHigh;

		graph->GetPoint(binLow, xLow, yLow);
		graph->GetPoint(binHigh, xHigh, yHigh);

		double errLowYup    =  graph->GetErrorYhigh(binLow);
		double errLowYdown  =  graph->GetErrorYlow(binLow);
		double errHighYup   =  graph->GetErrorYhigh(binHigh);
		double errHighYdown =  graph->GetErrorYlow(binHigh);
	
		f->Close();

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

			if(myBin == (numPoints-1)) {
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

DEFINE_EDM_PLUGIN( FlashggSystematicLeptonMethodsFactory,
		flashgg::LeptonEffSmear,
		"FlashggLeptonEffSmear" );
