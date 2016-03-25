#ifndef FlashggSampleInfoTree_h
#define FlashggSampleInfoTree_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/MergeableDouble.h"
#include "DataFormats/Common/interface/MergeableCounter.h"

#include <TTree.h>

namespace tnp {
  class FlashggSampleInfoTree : public edm::one::EDProducer<edm::one::WatchLuminosityBlocks, 
    edm::EndLuminosityBlockProducer> {
  public:
    explicit FlashggSampleInfoTree(const edm::ParameterSet& config);
    ~FlashggSampleInfoTree() {};

    virtual void beginLuminosityBlock(const edm::LuminosityBlock &, const edm::EventSetup&) override;
    virtual void endLuminosityBlock(edm::LuminosityBlock const&, const edm::EventSetup&) override;
    virtual void endLuminosityBlockProduce(edm::LuminosityBlock &, const edm::EventSetup&) override;
    
    virtual void produce(edm::Event &, const edm::EventSetup&) override;
    void endJob();

  private:
    //edm::EDGetTokenT<GenEventInfoProduct> weightSrcToken_;
    //edm::EDGetTokenT<reco::VertexCollection> recVtxsToken_;
    
    edm::EDGetTokenT<edm::MergeableDouble> weightToken_;
    edm::EDGetTokenT<edm::MergeableCounter> eventToken_;

    TTree * addTree_;
    double sumWeight_;
    double nEvents_; 

    double totGenWeight_;
    double totNEvents_; 
  };
}

#endif
