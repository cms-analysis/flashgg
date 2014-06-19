#ifndef FLASHgg_PhotonPreselectorBase_h
#define FLASHgg_PhotonPreselectorBase_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Handle.h"
#include "flashgg/MicroAODFormats/interface/Photon.h"
#include "DataFormats/Common/interface/PtrVector.h"

// Shamelessly patterned on https://github.com/cms-sw/cmssw/blob/CMSSW_7_2_X/RecoParticleFlow/PFProducer/interface/BlockElementLinkerBase.h

namespace flashgg {

  class PhotonPreselectorBase {

  public:
    PhotonPreselectorBase(const edm::ParameterSet& conf):
      _selectorName( conf.getParameter<std::string>("PhotonPreselectorName") ) { }

    PhotonPreselectorBase(const PhotonPreselectorBase& ) = delete;
    PhotonPreselectorBase& operator=(const PhotonPreselectorBase&) = delete;

    virtual bool ispreselected(const edm::Ptr<pat::Photon>&) const = 0;
    
    const std::string& name() const { return _selectorName; }

  private:
    const std::string _selectorName;

      
  };
}

#include "FWCore/PluginManager/interface/PluginFactory.h"
typedef edmplugin::PluginFactory< flashgg::PhotonPreselectorBase* (const edm::ParameterSet&) > FlashggPhotonPreselectorFactory;

#endif
