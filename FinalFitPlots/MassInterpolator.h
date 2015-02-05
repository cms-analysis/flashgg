#ifndef MassInterpolator_h
#define MassInterpolator_h

#include <cmath>
#include <algorithm>
#include <vector>
#include <map>
#include <string>

#include "TString.h"
#include "TSpline.h"
#include "TGraph.h"
#include "TDirectory.h"
#include "TFile.h"
#include "RooWorkspace.h"
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "TH1F.h"
#include "TF1.h"
#include "TMatrixD.h"
#include "TMVA/TSpline1.h"
#include "TMVA/TSpline2.h"

#include "Macros/Normalization_8TeV.h"

// ------------------------------------------------------------------------------------------------
class AbsInterpolator
{
public:
	virtual float operator() (float x) const = 0;
};

// ------------------------------------------------------------------------------------------------
template<class T, class H=TGraph> 
class  GraphInterpolator : public AbsInterpolator {
public:
	GraphInterpolator( TString name, H * g ) { sp_ = new T(name,g); };
	float operator() (float x) const {
		return sp_->Eval( x );
	};
	double operator() (double *x, double *p) const {
		return sp_->Eval( x[0] ) - p[0];
	};
	T * sp_;
};

typedef GraphInterpolator<TMVA::TSpline1> LinInterpolator;
typedef GraphInterpolator<TMVA::TSpline2> QuadInterpolator;

typedef GraphInterpolator<TMVA::TSpline1,TH1> HistLinInterpolator;
typedef GraphInterpolator<TMVA::TSpline2,TH1> HistQuadInterpolator;


// ------------------------------------------------------------------------------------------------
class ChannelModel
{
public:
	ChannelModel(const char * process="", const char * channel="") :
		process_(process), channel_(channel)
		{};
	
	const TH1F * histo(int imass) const { return templates_[imass]; };
	float mass(int imass) const { return masses_[imass]; }
	const char * process() const { return process_.c_str(); };
	const char * channel() const { return channel_.c_str(); };

	void process(const char * x) { process_ = x; };
	void channel(const char * x) { channel_ = x; };
	
	bool operator == (std::pair<std::string, std::string> & cmp) const 
		{ return cmp.first == process_ && cmp.second == channel_; };
	
	void addInputMass(TH1F * histo, float mass, int imass);
	void addInterpolated(TH1F * histo, float mass);
	void setup(AbsInterpolator * sigma, AbsInterpolator * br);
	
	float normalization(float mass)  { return (*effAcc_)(mass) * (*sigma_)(mass) * (*br_)(mass); }
	
	const AbsInterpolator * effAcc()  { return effAcc_; }; 
	const AbsInterpolator * sigma()   { return sigma_; };
	const AbsInterpolator * br()      { return br_; };

private:
	AbsInterpolator  * effAcc_,  * sigma_, * br_;
	
	std::string process_;
	std::string channel_;
	std::vector<TH1F*> templates_;
	std::vector<float> masses_;

	std::vector<TH1F*> interpolated_;
	std::vector<float> interpolatedMasses_;
};

// ------------------------------------------------------------------------------------------------
class MassInterpolator
{
public:
	MassInterpolator(size_t nmasses, double * outputMasses, bool doSmoothing, const char * th1pfx, 
			 const char * roohistpfx, const char * fmt="_mass_m%1.5g_", int roundTo=2);
	void addInput(float mass, const char * process, const char * channel, TH1F *);

	void runInterpolation(bool is7TeV);
	
	void outputFile(TFile * x) { outfile_ = x; }
	void outputWs(RooWorkspace * w, RooRealVar * x) { outws_ = w; x_ = x; }
  void addToMap(std::string key, double scaleFactor, std::vector<string> vec);
  std::map<std::string,pair<double,std::vector<string> > > specProcNormMap_;

  void printMap();
	
private:
	ChannelModel * findContainer(const char * process, const char * channel);
	void storeOutput(const TH1F * histo, const std::string & fmt, float mass, bool overwrite);

	typedef std::map<std::string, ChannelModel> channelsList_t;
	typedef std::map<std::string, channelsList_t> processList_t;	
	std::vector<float> inputMasses_, outputMasses_;

	std::string th1pfx_, roohistpfx_, fmt_;
	int roundTo_;
	float roundMul, roundDiv;
	
	processList_t channelContainers_;
	ChannelModel * cacheChannelContaier_;
	
	bool doSmoothing_;
	RooWorkspace * outws_;
	RooRealVar * x_;
	TFile * outfile_;

};


#endif // MassInterpolator_h
