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
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"

#include "flashgg/MetaData/interface/IdleWatchdog.h"

class IdleWatchdog : public edm::EDAnalyzer, flashgg::IdleWatchdog
{
public:
    explicit IdleWatchdog( const edm::ParameterSet & );
    ~IdleWatchdog();

    virtual void analyze( const edm::Event &, const edm::EventSetup & );
    virtual void respondToOpenInputFile( edm::FileBlock const & );

};



using namespace edm;
using namespace std;



IdleWatchdog::IdleWatchdog( const edm::ParameterSet &iConfig ) : flashgg::IdleWatchdog( iConfig )
{
}


IdleWatchdog::~IdleWatchdog() {}


void
IdleWatchdog::analyze( const edm::Event &iEvent, const edm::EventSetup &iSetup )
{
    check();
    return;
}

void IdleWatchdog::respondToOpenInputFile( edm::FileBlock const & )
{
    reset();
}



//define this as a plug-in
DEFINE_FWK_MODULE( IdleWatchdog );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

