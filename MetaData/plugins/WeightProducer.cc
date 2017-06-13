// -*- C++ -*-
//
// Package:    WeightProducer
// Class:      WeightProducer
//
/**\class WeightProducer WeightProducer.cc CommonTools/UtilAlgos/plugins/WeightProducer.cc

Description: An event counter that can store the number of events in the lumi block

*/


// system include files
#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"

class WeightProducer : public edm::EDProducer
{
public:
    explicit WeightProducer( const edm::ParameterSet & );
    ~WeightProducer();

private:
    virtual void produce( edm::Event &, const edm::EventSetup & ) override;

    // ----------member data ---------------------------
    /// GenEventInfoProduct_generator__SIM.
    edm::EDGetTokenT<GenEventInfoProduct> genInfoToken_;

    edm::EDGetTokenT<std::vector<PileupSummaryInfo> >   puInfoToken_;

    double lumiWeight_;
    bool doGenInfoWeight_, doPuInfoWeight_;
};



using namespace edm;
using namespace std;



WeightProducer::WeightProducer( const edm::ParameterSet &iConfig ) :
    lumiWeight_( iConfig.getParameter<double>( "lumiWeight" ) ),
    doGenInfoWeight_( false ),
    doPuInfoWeight_( false )
{
    produces<double>();

    if( iConfig.exists( "generator" ) ) {
        doGenInfoWeight_ = true;
        genInfoToken_ = consumes<GenEventInfoProduct>( iConfig.getParameter<InputTag> ( "generator" ) );
    }

    if( iConfig.exists( "pileupInfo" ) ) {
        doPuInfoWeight_ = true;
        puInfoToken_ = consumes<std::vector<PileupSummaryInfo> >( iConfig.getParameter<InputTag> ( "pileupInfo" ) );
    }
}


WeightProducer::~WeightProducer() {}


void
WeightProducer::produce( edm::Event &iEvent, const edm::EventSetup &iSetup )
{
    edm::Handle<GenEventInfoProduct> genInfo;
    edm::Handle<std::vector<PileupSummaryInfo> > puInfo;
    unique_ptr<double> pweight( new double( 1. ) );

    if( ! iEvent.isRealData() ) {
        ( *pweight ) = lumiWeight_;
        if( doGenInfoWeight_ ) {
            iEvent.getByToken( genInfoToken_, genInfo );

            const auto &weights = genInfo->weights();
            ( *pweight ) *= weights[0];
        }
        //// if( doPuInfoWeight_ ) {
        //// 	iEvent.getByToken(puInfoToken_,puInfo);
        //// 	hysto_type::value_type truePu=0., obsPu=0.;
        //// 	for( auto & frame : *puInfo ) {
        //// 	  /// std::cout << frame.getBunchCrossing() << std::endl;
        //// 	  if( frame.getBunchCrossing() == 0 ) {
        //// 	    truePu = frame.getTrueNumInteractions();
        //// 	    obsPu = frame.getPU_NumInteractions();
        //// 	    break;
        //// 	  }
        //// 	}
        //// 	/// cout << truePu << " " << obsPu << endl;
        ////
        //// 	if( doTruePileup_ ) {
        //// 	  size_t bin = 0;
        //// 	  if( truePu >= maxTruePileup_ ) { bin = truePileup_.size() - 1; }
        //// 	  else if( truePu >= minTruePileup_ ) { bin = (size_t)std::floor( (truePu-minTruePileup_) / widthTruePileup_) + 1; }
        //// 	  truePileup_[bin] += weights[0];
        //// 	}
        ////
        //// 	if( doObsPileup_ ) {
        //// 	  size_t bin = 0;
        //// 	  if( obsPu >= maxObsPileup_ ) { bin = obsPileup_.size() - 1; }
        //// 	  else if( obsPu >= minObsPileup_ ) { bin = (size_t)std::floor( (obsPu-minObsPileup_) / widthObsPileup_) + 1; }
        //// 	  /// cout << bin << " " << std::floor( (obsPu-minObsPileup_) / widthObsPileup_) << endl;
        //// 	  obsPileup_[bin] += weights[0];
        //// 	}
        //// }

    }

    iEvent.put( std::move( pweight ) );
    return;
}

//define this as a plug-in
DEFINE_FWK_MODULE( WeightProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

