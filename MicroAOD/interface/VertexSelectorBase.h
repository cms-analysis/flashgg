#ifndef FLASHgg_VertexSelectorBase_h
#define FLASHgg_VertexSelectorBase_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Handle.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "flashgg/DataFormats/interface/VertexCandidateMap.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"

// Shamelessly patterned on https://github.com/cms-sw/cmssw/blob/CMSSW_7_2_X/RecoParticleFlow/PFProducer/interface/BlockElementLinkerBase.h

namespace flashgg {

  class VertexSelectorBase {

  public:
    VertexSelectorBase(const edm::ParameterSet& conf):
      _selectorName( conf.getParameter<std::string>("VertexSelectorName") ) { }

    VertexSelectorBase(const VertexSelectorBase& ) = delete;
    VertexSelectorBase& operator=(const VertexSelectorBase&) = delete;

    typedef std::map<std::string,double> Parameters_Selector_Type;
    virtual edm::Ptr<reco::Vertex> select(const edm::Ptr<flashgg::Photon>&,
					  const edm::Ptr<flashgg::Photon>&,const edm::Handle<edm::View<reco::Vertex> >&,
					  const VertexCandidateMap&,
					  const edm::Handle<edm::View<reco::Conversion> >&,
					  const math::XYZPoint&
					  //					  const Parameters_Selector_Type&,
					  //                                          const float&
					  )  = 0;
    
    const std::string& name() const { return _selectorName; };

    virtual void writeInfoFromLastSelectionTo(flashgg::DiPhotonCandidate&) = 0;

  private:
    const std::string _selectorName;

      
  };
}

#include "FWCore/PluginManager/interface/PluginFactory.h"
typedef edmplugin::PluginFactory< flashgg::VertexSelectorBase* (const edm::ParameterSet&) > FlashggVertexSelectorFactory;

#endif
