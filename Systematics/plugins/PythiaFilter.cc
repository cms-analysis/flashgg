#include "flashgg/Systematics/interface/PythiaFilter.h"

PythiaFilter::PythiaFilter(const edm::ParameterSet& iConfig) :
  genParticleToken_( consumes<View<reco::GenParticle> >( iConfig.getParameter<InputTag>( "genParticleTag" ) ) )
{
}

PythiaFilter::~PythiaFilter()
{
}



// member functions
//

// ------------ method called on each new Event  ------------
bool
PythiaFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  int numGoodPhotons = 0;

  Handle<View<reco::GenParticle> > genParticles;
  iEvent.getByToken( genParticleToken_, genParticles );
  
  for( unsigned int i = 0 ; i < genParticles->size(); i++ ) {
    Ptr<reco::GenParticle> gen = genParticles->ptrAt(i);
    if (gen->status() >= 60) continue; // in aMC@NLO we get extra bosons above here
    //    if (gen->pdgId() == 25){
    //      cout << "Higgs found status=" << " " << gen->status() << endl;
    //    }
    if( gen->pdgId()==22 && gen->fromHardProcessFinalState() ) numGoodPhotons+=1;
  }

  return (numGoodPhotons==2);
}

DEFINE_FWK_MODULE(PythiaFilter);

