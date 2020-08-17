#include "flashgg/Systematics/interface/VHFilter.h"

VHFilter::VHFilter(const edm::ParameterSet& iConfig) :
  genParticleToken_( consumes<View<reco::GenParticle> >( iConfig.getParameter<InputTag>( "genParticleTag" ) ) ),
  chooseW_( iConfig.getParameter<bool>( "chooseW" ) ),
  chooseZ_( iConfig.getParameter<bool>( "chooseZ" ) )
{
  if (chooseW_ == chooseZ_) {
    throw cms::Exception( "Configuration" ) << " please set either chooseW or chooseZ, but not both";
  }
}

VHFilter::~VHFilter()
{
}



// member functions
//

// ------------ method called on each new Event  ------------
bool
VHFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  bool accepted = false;
  bool isWH = false;
  bool isZH = false;

  Handle<View<reco::GenParticle> > genParticles;
  iEvent.getByToken( genParticleToken_, genParticles );
  
  for( unsigned int i = 0 ; i < genParticles->size(); i++ ) {
    Ptr<reco::GenParticle> gen = genParticles->ptrAt(i);
    if (gen->status() >= 60) continue; // in aMC@NLO we get extra bosons above here
    //    if (gen->pdgId() == 25){
    //      cout << "Higgs found status=" << " " << gen->status() << endl;
    //    }
    if (gen->pdgId() == 23 ){ 
      //      cout << "Z found status=" << " " << gen->status() << endl;
      isZH = true;
    }
    if (abs(gen->pdgId()) == 24){
      //      cout << "W found status=" << " " << gen->status() << endl;
      isWH = true;
    }
  }

  if (isWH && isZH) {
    throw cms::Exception( "GenParticleLogic" ) << " We are calling this both a W and Z event, this is bad, please fix";
    //    cout << " WARNING: both W and Z" << endl;
  }

  if ((!isWH) && (!isZH)) {
    cout << " WARNING: neither W nor Z" << endl;
  }

  if (chooseW_) accepted = isWH;
  if (chooseZ_) accepted = isZH;

  if (accepted) {
    return true; 
  } else { 
    return false;
  }
  
}

DEFINE_FWK_MODULE(VHFilter);

