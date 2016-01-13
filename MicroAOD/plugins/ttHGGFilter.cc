#include "flashgg/MicroAOD/interface/ttHGGFilter.h"

ttHGGFilter::ttHGGFilter(const edm::ParameterSet& iConfig) :
  genParticleToken_( consumes<View<reco::GenParticle> >( iConfig.getParameter<InputTag>( "genParticleTag" ) ) )
{

}

ttHGGFilter::~ttHGGFilter()
{
}



// member functions
//

// ------------ method called on each new Event  ------------
bool
ttHGGFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  bool accepted = false;

  Handle<View<reco::GenParticle> > genParticles;
  iEvent.getByToken( genParticleToken_, genParticles );
  
  // -- look for Higgs decaying to two photons                                                                                                                                            
  for( unsigned int i = 0 ; i < genParticles->size(); i++ ) {
    Ptr<reco::GenParticle> gen = genParticles->ptrAt(i);
    if (gen->pdgId() == 25){
      //cout << "Higgs found " <<endl;
      if (gen->numberOfDaughters() !=2 ) continue;
      const reco::Candidate * d1 = gen->daughter( 0 );
      const reco::Candidate * d2 = gen->daughter( 1 );
      //cout << "Higgs with status = " << gen->status() << " has two daughters with pdgId: " << endl;
      //cout << "d1 pdgId = " << d1->pdgId() << "   d2 pdgId = "<< d2->pdgId() <<endl;
      if (d1->pdgId()!=22 || d2->pdgId()!=22) continue;

      accepted = true;
      break;
    }
  }
  
  if (accepted) {
    return true; 
  } else { 
    return false;
  }
  
}

DEFINE_FWK_MODULE(ttHGGFilter);

