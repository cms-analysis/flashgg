//#include "FWCore/Framework/interface/Frameworkfwd.h"
//#include "FWCore/Framework/interface/Event.h"
//#include "FWCore/Framework/interface/MakerMacros.h"
//#include "FWCore/ParameterSet/interface/ParameterSet.h"
//#include "FWCore/Framework/interface/EDProducer.h"

#include "PhysicsTools/TagAndProbe/plugins/MiniAODTriggerCandProducer.h"
//#include "flashgg/DataFormats/interface/SinglePhotonView.h"
#include "flashgg/DataFormats/interface/Photon.h"

#include <DataFormats/Math/interface/deltaR.h>

//#include "FWCore/MessageLogger/interface/MessageLogger.h"

//#include <string>
//#include <vector>


template <>
bool MiniAODTriggerCandProducer<flashgg::Photon>::onlineOfflineMatching(const edm::TriggerNames & triggerNames, 
									edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjects, 
									edm::Ref<std::vector<flashgg::Photon> > ref, std::string filterLabel, float dRmin) {
  
  for (pat::TriggerObjectStandAlone obj : *triggerObjects) { 
    obj.unpackPathNames(triggerNames); 
    if (obj.hasFilterLabel(filterLabel)) {
      float dR = deltaR(ref->superCluster()->position(), obj.p4());
      if (dR < dRmin)
	return true;
    }
  }

  return false;
}


//template <>
//void MiniAODTriggerCandProducer<flashgg::Photon>::produce(edm::Event &iEvent, const edm::EventSetup &eventSetup) {
//  edm::Handle<edm::TriggerResults> triggerBits;
//  edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjects;
//  
//  edm::Handle<TCollection> inputs;
//  //edm::Handle<TRefVector> inputs;
//
//  iEvent.getByToken(triggerBits_, triggerBits);
//  iEvent.getByToken(triggerObjects_, triggerObjects);
//  iEvent.getByToken(inputColl_, inputs);
//
//  // Create the output collection
//  //std::auto_ptr<TRefVector> outColRef(new TRefVector);
//  std::auto_ptr<std::vector<flashgg::Photon> > outCol(new std::vector<flashgg::Photon>);
//  
//  if (!triggerBits.isValid()) {
//    LogDebug("") << "TriggerResults product not found - returning result=false!";
//    return;
//  }
//
//  const edm::TriggerNames & triggerNames = iEvent.triggerNames(*triggerBits);
//  if (triggerNamesID_ != triggerNames.parameterSetID()) {
//    triggerNamesID_ = triggerNames.parameterSetID();
//    init(*triggerBits, triggerNames);
//  } 
//  
//  for (size_t i=0; i<inputs->size(); i++) {
//    bool saveObj = true;
//    //TRef ref = (*inputs)[i];
//    flashgg::Photon view = (*inputs)[i];
//    
//    for (size_t f=0; f<filterNames_.size(); f++) {
//      if (onlineOfflineMatching(triggerNames, triggerObjects, view, filterNames_[f], dRMatch_)) {
//	if (!isAND_) {
//	  outCol->push_back(view);
//	  break;
//	} 
//      } else {
//	if (isAND_) {
//	  saveObj = false;
//	  break;
//	}	
//      }
//    }
//
//    if (saveObj) {
//      //outColRef->push_back(ref);
//      outCol->push_back(view);
//    }
//  }	  
//
//  //iEvent.put(outColRef);
//  iEvent.put(outCol);
//}



typedef MiniAODTriggerCandProducer<flashgg::Photon> FlashggPhotonTriggerCandProducer;
DEFINE_FWK_MODULE(FlashggPhotonTriggerCandProducer);

