#include "flashgg/Validation/plugins/FlashggSampleInfoTree.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include <iostream>

tnp::FlashggSampleInfoTree::FlashggSampleInfoTree(const edm::ParameterSet& iConfig) {
  //// make trees as requested
  //edm::Service<TFileService> fs;
  //addTree_ = fs->make<TTree>("sampleInfo", "sampleInfo");
  //hnPU = fs->make<TH1F>("hnPU", "hnPU", 100, 0., 100.);
  //
  //totalGenWeight_ = 0.0;
  //nEvents_ = 0;
  //min_ = 0;
  //max_ = 0;
  //
  //addTree_->Branch("sumWeight", &totalGenWeight_, "sumWeight/D");
  //addTree_->Branch("nEvents", &nEvents_, "nEvents/D");

  weightToken_ = consumes<edm::MergeableDouble, edm::InLumi>(edm::InputTag("weightsCount","totalWeight"));
  eventToken_  = consumes<edm::MergeableCounter, edm::InLumi>(edm::InputTag("eventCount"));

  sumWeight_ = 0.0;
  nEvents_ = 0.0;

  produces<edm::MergeableDouble, edm::InLumi> ("totalGenWeight"); 
  produces<edm::MergeableDouble, edm::InLumi> ("totalEvent");
}

void tnp::FlashggSampleInfoTree::beginLuminosityBlock(const edm::LuminosityBlock &, const edm::EventSetup&)
{}

void tnp::FlashggSampleInfoTree::endLuminosityBlock(edm::LuminosityBlock const& iLumi, edm::EventSetup const& iSetup) {

  edm::Handle<edm::MergeableDouble> totWeight;
  iLumi.getByToken(weightToken_, totWeight);
  if (totWeight.isValid())
    sumWeight_ += (double)totWeight->value;

  edm::Handle<edm::MergeableCounter> nEventsH;
  iLumi.getByToken(eventToken_, nEventsH);
  if (nEventsH.isValid())
    nEvents_ += (double)nEventsH->value;

  //edm::Handle<edm::MergeableHisto<float> > nPUH;
  //iLumi.getByLabel(edm::InputTag("weightsCount","obsPileup"), nPUH);
  //if (nPUH.isValid()) {
  //  min_ = nPUH->min;
  //  max_ = nPUH->max;
  //  if (values_.size() == 0)
  //    values_.resize(nPUH->values.size(), 0);
  //  
  //  for (unsigned int i=0; i<values_.size(); i++)
  //    values_[i] += nPUH->values[i];
  //}
}

void tnp::FlashggSampleInfoTree::endLuminosityBlockProduce(edm::LuminosityBlock & theLuminosityBlock, const edm::EventSetup & theSetup) {
  //LogTrace("WeightsCounting") << "endLumi: adding " << weightProcessedInLumi_ << " events" << endl;
  
  std::unique_ptr<edm::MergeableDouble> numWeightssPtr(new edm::MergeableDouble);
  numWeightssPtr->value = sumWeight_;
  theLuminosityBlock.put( std::move(numWeightssPtr) , "totalGenWeight");
  
  std::unique_ptr<edm::MergeableDouble> numEventsPtr(new edm::MergeableDouble);
  numEventsPtr->value = nEvents_;
  theLuminosityBlock.put( std::move(numEventsPtr) , "totalEvent");
  //return;
  //addTree_->Fill();
  
}

void tnp::FlashggSampleInfoTree::produce(edm::Event &, const edm::EventSetup&) 
{}


void tnp::FlashggSampleInfoTree::endJob() {

  //hnPU->SetBins(values_.size(), min_, max_);
  //for (int i=0; i<hnPU->GetNbinsX(); i++)
  //  hnPU->SetBinContent(i, values_[i]);
  //addTree_->Fill();
}

DEFINE_FWK_MODULE(tnp::FlashggSampleInfoTree);
