#include "flashgg/Systematics/interface/PromptFakeFilter.h"
#include "flashgg/DataFormats/interface/Photon.h"

PromptFakeFilter::PromptFakeFilter(const edm::ParameterSet& iConfig) :
  tagToken_( consumes<edm::View<flashgg::DiPhotonTagBase> >( iConfig.getParameter<InputTag>( "TagSorter" ) ) ),
  fakeFake_( iConfig.getParameter<bool>( "doFakeFake" ) ),
  promptFake_( iConfig.getParameter<bool>( "doPromptFake" ) ),
  acceptBoth_( iConfig.getParameter<bool>( "doBoth" ) )
{
  int boolSum = int(fakeFake_) + int(promptFake_) + int(acceptBoth_);
  if (boolSum != 1) {
    throw cms::Exception( "Configuration" ) << "Only one of doFakeFake, doPromptFake, or doBoth should be set";
  }
}

PromptFakeFilter::~PromptFakeFilter()
{
}



// member functions
//

// ------------ method called on each new Event  ------------
bool
PromptFakeFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  bool accepted = false;

  Handle<edm::View<flashgg::DiPhotonTagBase> > TagSorter;
  iEvent.getByToken( tagToken_, TagSorter );
  assert (TagSorter.product()->size() <= 1); // to avoid ambiguity, this must not run on the systematics tag merger
  if (TagSorter.product()->size() == 0) return false;

  flashgg::Photon::mcMatch_t leadMatchType = TagSorter.product()->at(0).diPhoton()->leadingPhoton()->genMatchType();
  flashgg::Photon::mcMatch_t subleadMatchType = TagSorter.product()->at(0).diPhoton()->subLeadingPhoton()->genMatchType();

  if (fakeFake_) {
    accepted = (leadMatchType != 1 && subleadMatchType != 1);
  } else if (promptFake_) {
    accepted = (!(leadMatchType==1 && subleadMatchType==1) && (leadMatchType==1 || subleadMatchType==1));
  } else if (acceptBoth_) {;
    accepted = ( !(leadMatchType==1 && subleadMatchType==1) );
  }

  std::cout << " leadMatchType=" << leadMatchType << " subleadMatchType=" << subleadMatchType << " accepted=" << accepted << std::endl;
  
  return accepted;
}

DEFINE_FWK_MODULE(PromptFakeFilter);

