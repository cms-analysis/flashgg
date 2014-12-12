#ifndef FLASHgg_VHhadronicTag_h
#define FLASHgg_VHhadronicTag_h

#include "flashgg/TagFormats/interface/DiPhotonTagBase.h"
#include "flashgg/MicroAODFormats/interface/Jet.h"

namespace flashgg {

  class VHhadronicTag: public DiPhotonTagBase {

  public:

    VHhadronicTag();
    VHhadronicTag(edm::Ptr<DiPhotonCandidate>,edm::Ptr<DiPhotonMVAResult>);
    ~VHhadronicTag();
    
    //const std::pair<edm::Ptr<flashgg::Jet>,edm::Ptr<flashgg::Jet> > getJets() const { return JetPair_;}
    const edm::Ptr<flashgg::Jet>      getLeadingJet()    { return leadingJet_; }
    const edm::Ptr<flashgg::Jet>      getSubLeadingJet() { return subleadingJet_; }
    const edm::Ptr<DiPhotonCandidate> getDiPhotonCandidate() const { return theDiPhotonCandidate_;} 
    //const edm::Ptr<DiPhotonMVAResult> getDiPhotonMVAResult() const { return theDiPhotonMVAResult_;} // doesn't work for some reason
    
    //void setJets( std::pair<edm::Ptr<flashgg::Jet>,edm::Ptr<flashgg::Jet> > JetPair ) { JetPair_ = JetPair; }
    void setJets( edm::Ptr<flashgg::Jet>, edm::Ptr<flashgg::Jet> );
  
  private:
    edm::Ptr<DiPhotonCandidate> theDiPhotonCandidate_;
    //edm::Ptr<DiPhotonMVAResult> theDiPhotonMVAResult_;
    //std::pair<edm::Ptr<flashgg::Jet>,edm::Ptr<flashgg::Jet> > JetPair_;
    edm::Ptr<flashgg::Jet> leadingJet_;
    edm::Ptr<flashgg::Jet> subleadingJet_;
    
  };
} 

#endif 

