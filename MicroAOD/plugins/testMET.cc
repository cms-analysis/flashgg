//metFilterTest
// system include files
#include <memory>
#include <cmath>
#include <iostream>
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/Ptr.h"
//#include "DataFormats/Common/interface/PtrVector.h"

#include "DataFormats/Math/interface/deltaR.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include <set>
#include "TFile.h"


class metFilterTest : public edm::EDAnalyzer
{
public:
  explicit metFilterTest( const edm::ParameterSet & );
  ~metFilterTest();
private:
  virtual void analyze( const edm::Event &, const edm::EventSetup & ) override;
  edm::EDGetTokenT<edm::TriggerResults> triggerBits_;
};

metFilterTest::~metFilterTest()
{
}

metFilterTest::metFilterTest( const edm::ParameterSet &iConfig ):
  triggerBits_( consumes<edm::TriggerResults>( iConfig.getParameter<edm::InputTag>( "bits" ) ) )
{

}



void metFilterTest::analyze( const edm::Event &iEvent, const edm::EventSetup &iSetup )
{
  edm::Handle<edm::TriggerResults> triggerBits;
  iEvent.getByToken( triggerBits_, triggerBits );
  if( !triggerBits.isValid() ) {
    LogDebug( "" ) << "TriggerResults product not found - returning result=false!";
    return;
  }
  const edm::TriggerNames &triggerNames = iEvent.triggerNames( *triggerBits );
  for( unsigned int i = 0; i < triggerNames.triggerNames().size(); i++ ) 
    {
      //if(triggerBits->accept(triggerNames.triggerName(i)))
      bool on = triggerBits->accept(i);
      if(on)
	std::cout << triggerNames.triggerName(i) << "  is on++++++++++++++++++" << std::endl;
      else
	std::cout << triggerNames.triggerName(i) << "  is off------------------" << std::endl;
    }
}

DEFINE_FWK_MODULE( metFilterTest );
