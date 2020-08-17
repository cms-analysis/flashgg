#include "flashgg/Systematics/interface/HardProcessFinalStateFilter.h"

HardProcessFinalStateFilter::HardProcessFinalStateFilter(const edm::ParameterSet& iConfig) :
  genParticleToken_( consumes<View<reco::GenParticle> >( iConfig.getParameter<InputTag>( "genParticleTag" ) ) ),
  packedGenParticleToken_( consumes<vector<pat::PackedGenParticle> >( iConfig.getParameter<InputTag>( "packedGenParticleTag" ) ) ),
  genInfoToken_( consumes<GenEventInfoProduct>( iConfig.getParameter<InputTag>( "genInfoTag" ) ) ),
  usePacked_( iConfig.getParameter<bool>( "usePacked" ) ),
  debug_( iConfig.getParameter<bool>( "debug" ) )
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
  
  if( usePacked_ ) {
    Handle<vector<pat::PackedGenParticle> > packedGenParticles;
    iEvent.getByToken( packedGenParticleToken_, packedGenParticles );

    for( auto gen = packedGenParticles->begin(); gen != packedGenParticles->end(); gen++ ) {
      if (gen->status() >= 60) continue; // in aMC@NLO we get extra bosons above here
      if (debug_) {
	std::cout << " HardProcessFinalStateFilter debug id=" << gen->pdgId() << " status=" << gen->status()
		  << " pt=" << gen->pt() << " eta=" << gen->eta() << " phi=" << gen->phi()
		  << " fromHardProcessFinalState=" << gen->fromHardProcessFinalState() << std::endl;
      }
      if( gen->pdgId()==22 && gen->fromHardProcessFinalState() ) numGoodPhotons+=1;
    }
  }

  else {
    Handle<View<reco::GenParticle> > genParticles;
    iEvent.getByToken( genParticleToken_, genParticles );

    for( unsigned int i = 0 ; i < genParticles->size(); i++ ) {
      Ptr<reco::GenParticle> gen = genParticles->ptrAt(i);
      if (gen->status() >= 60) continue; // in aMC@NLO we get extra bosons above here
      if (debug_) {
	std::cout << " HardProcessFinalStateFilter debug id= " << gen->pdgId() << " status=" << gen->status() 
		  << " pt=" << gen->pt() << " eta=" << gen->eta() << " phi=" << gen->phi()
		  << " fromHardProcessFinalState=" << gen->fromHardProcessFinalState()  << std::endl;
      }
      if( gen->pdgId()==22 && gen->fromHardProcessFinalState() ) numGoodPhotons+=1;
    }
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

  if (debug_) {
    std::cout << "HardProcessFinalStateFilter debug numGoodPhotons=" << numGoodPhotons << " pass=" << (numGoodPhotons==2) << std::endl;
  }

  return (numGoodPhotons==2);
}

DEFINE_FWK_MODULE(HardProcessFinalStateFilter);

