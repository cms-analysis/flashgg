#include "flashgg/Systematics/interface/CentralHiggsFilter.h"

CentralHiggsFilter::CentralHiggsFilter(const edm::ParameterSet& iConfig) :
  genParticleToken_( consumes<View<reco::GenParticle> >( iConfig.getParameter<InputTag>( "genParticleTag" ) ) )
{
}

CentralHiggsFilter::~CentralHiggsFilter()
{
}



// member functions
//

// ------------ method called on each new Event  ------------
bool
CentralHiggsFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  bool accepted = false;

  Handle<View<reco::GenParticle> > genParticles;
  iEvent.getByToken( genParticleToken_, genParticles );
  
  for( unsigned int i = 0 ; i < genParticles->size(); i++ ) {
    Ptr<reco::GenParticle> gen = genParticles->ptrAt(i);
    if (gen->status() >= 60) continue; // in aMC@NLO we get extra bosons above here
    if (gen->pdgId() == 25){
      //      cout << "Higgs found status=" << " " << gen->status() << endl;
      //      cout << "Higgs rapidity "<< gen->rapidity() << endl;
      accepted = (fabs(gen->rapidity()) < 2.5);
    }
  }

  //  cout << "accepted=" << accepted << endl;


  if (accepted) {
    return true; 
  } else { 
    return false;
  }
  
}

DEFINE_FWK_MODULE(CentralHiggsFilter);

