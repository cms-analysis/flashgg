#ifndef FLASHgg_VHhadronicTag_h
#define FLASHgg_VHhadronicTag_h

#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "flashgg/DataFormats/interface/Jet.h"

namespace flashgg {

  class VHhadronicTag: public DiPhotonTagBase {

  public:

    VHhadronicTag();
    VHhadronicTag(edm::Ptr<DiPhotonCandidate>, DiPhotonMVAResult);
    VHhadronicTag(edm::Ptr<DiPhotonCandidate>,edm::Ptr<DiPhotonMVAResult>);
    VHhadronicTag* clone() const;
    ~VHhadronicTag();
    
    const edm::Ptr<flashgg::Jet>      getLeadingJet()  const  { return leadingJet_; }
    const edm::Ptr<flashgg::Jet>      getSubLeadingJet() const { return subleadingJet_; }
    const edm::Ptr<DiPhotonCandidate> getDiPhotonCandidate() const { return theDiPhotonCandidate_;} 
    //const edm::Ptr<DiPhotonMVAResult> getDiPhotonMVAResult() const { return theDiPhotonMVAResult_;} // doesn't work for some reason
    
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

