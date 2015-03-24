// -*- C++ -*-
//
// Package:    IdleWatchdog
// Class:      IdleWatchdog
// 
/**\class IdleWatchdog IdleWatchdog.cc CommonTools/UtilAlgos/plugins/IdleWatchdog.cc

Description: An event counter that can store the number of events in the lumi block 

*/


// system include files
#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"

#include "TStopwatch.h"

class IdleWatchdog : public edm::EDAnalyzer {
public:
	explicit IdleWatchdog(const edm::ParameterSet&);
	~IdleWatchdog();
	
private:
	virtual void analyze(const edm::Event &, const edm::EventSetup&);
	virtual void respondToOpenInputFile(edm::FileBlock const&);
        void check();
	void reset();
	
	double minIdleFraction_;
	int checkEvery_, tolerance_;
	int nFailures_, ievent_;
	
	TStopwatch stopWatch_;
};



using namespace edm;
using namespace std;



IdleWatchdog::IdleWatchdog(const edm::ParameterSet& iConfig) :
	minIdleFraction_(iConfig.getUntrackedParameter<double>("minIdleFraction",0.2)),
	checkEvery_(iConfig.getUntrackedParameter<int>("checkEvery",1000)),
	tolerance_(iConfig.getUntrackedParameter<int>("tolerance",5))
{
	
}


IdleWatchdog::~IdleWatchdog(){}


void
IdleWatchdog::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) 
{
	/// cout << ievent_ << " " << checkEvery_ << " " << nFailures_ << " " << tolerance_ << " " << minIdleFraction_ << endl;
	if( ievent_ % checkEvery_ == 0 ) {
		check();
	}
	++ievent_;
	return;
}

void IdleWatchdog::respondToOpenInputFile(edm::FileBlock const&)
{
	reset();
}


void IdleWatchdog::check()
{
	/// cout << "checking " << endl;
	stopWatch_.Stop();
	float cputime  = stopWatch_.CpuTime();
	float realtime = stopWatch_.RealTime();
	
	/// std::cout << cputime << " " << realtime << std::endl;
	if( cputime / realtime < minIdleFraction_ ) {
		--nFailures_;
	} else {
		nFailures_ = tolerance_;
	}
	
	if( nFailures_ == 0 ) {
		cerr << "too inefficient: " << minIdleFraction_ << " " << tolerance_ << " aborting " << endl;
		exit(99);
	}
	stopWatch_.Start();
}

void IdleWatchdog::reset()
{
	ievent_ = 0;
	nFailures_ = tolerance_;
	stopWatch_.Stop();
	stopWatch_.Start();
}

//define this as a plug-in
DEFINE_FWK_MODULE(IdleWatchdog);
