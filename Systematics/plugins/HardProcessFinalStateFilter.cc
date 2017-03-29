#include "flashgg/Systematics/interface/HardProcessFinalStateFilter.h"

HardProcessFinalStateFilter::HardProcessFinalStateFilter(const edm::ParameterSet& iConfig) :
  genParticleToken_( consumes<View<reco::GenParticle> >( iConfig.getParameter<InputTag>( "genParticleTag" ) ) ),
  genInfoToken_( consumes<GenEventInfoProduct>( iConfig.getParameter<InputTag>( "genInfoTag" ) ) )
{
  npass = 0;
  nfail = 0;
  ntot = 0;
  wpass = 0.;
  wfail = 0.;
  wtot = 0.;
}

HardProcessFinalStateFilter::~HardProcessFinalStateFilter()
{
}

void HardProcessFinalStateFilter::endJob()
{
  std::cout << "HardProcessFinalStateFilter endJob npass/nfail/ntot: " << npass << "/" << nfail << "/" << ntot << " wpass/wfail/wtot: " << wpass << "/" << wfail << "/" << wtot << std::endl;
}


// member functions
//

// ------------ method called on each new Event  ------------
bool
HardProcessFinalStateFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
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

  float weight = 1.;
  edm::Handle<GenEventInfoProduct> genInfo;
  iEvent.getByToken(genInfoToken_, genInfo);
  if( genInfo.isValid() ) {
    const auto &weights = genInfo->weights();
    if( ! weights.empty() ) {
      weight *= weights[0];
    }
  }

  // counting
  ntot += 1;
  wtot += weight;

  if (numGoodPhotons==2) {
    npass += 1;
    wpass += weight;
  } else {
    nfail += 1;
    wfail += weight;
  }

  return (numGoodPhotons==2);
}

DEFINE_FWK_MODULE(HardProcessFinalStateFilter);

