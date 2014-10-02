#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"
#include "flashgg/TagFormats/interface/DiPhotonMVAResult.h"

#include "TMVA/Reader.h"
#include "TMath.h"

using namespace std;
using namespace edm;

namespace flashgg {

  class DiPhotonMVAProducer : public EDProducer {

  public:
    DiPhotonMVAProducer( const ParameterSet & );
  private:
    void produce( Event &, const EventSetup & ) override;

    EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
    edm::EDGetTokenT<edm::View<reco::BeamSpot> > beamSpotToken_;

    TMVA::Reader * DiphotonMva_;
    bool DiphotonMva_initialized;
    edm::FileInPath diphotonMVAweightfile_;

  };

  DiPhotonMVAProducer::DiPhotonMVAProducer(const ParameterSet & iConfig) :
    diPhotonToken_(consumes<View<flashgg::DiPhotonCandidate> >(iConfig.getUntrackedParameter<InputTag> ("DiPhotonTag", InputTag("flashggDiPhotons")))),
    beamSpotToken_(consumes<View<reco::BeamSpot> >(iConfig.getUntrackedParameter<InputTag>("BeamSpotTag",InputTag("offlineBeamSpot"))))
  {
    std::cout << " In constructor " << std::endl;

    diphotonMVAweightfile_ = iConfig.getParameter<edm::FileInPath>("diphotonMVAweightfile");
    DiphotonMva_initialized = false;

    std::cout << " In constructor after getting filename for  MVA" << std::endl;

    produces<DiPhotonMVAResultMap>();

    std::cout << " In constructor after produces " << std::endl;
  }
  
  void DiPhotonMVAProducer::produce( Event & evt, const EventSetup & ) {

    std::cout << " In produce " << std::endl;

    Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
    evt.getByToken(diPhotonToken_,diPhotons);
    const PtrVector<flashgg::DiPhotonCandidate>& diPhotonPointers = diPhotons->ptrVector();

    Handle<reco::BeamSpot> recoBeamSpotHandle;
    evt.getByToken(beamSpotToken_,recoBeamSpotHandle);
    float beamsig;
    if (recoBeamSpotHandle.isValid()){
      beamsig = recoBeamSpotHandle->sigmaZ();
    } else {
      beamsig = -9999; // I hope this never happens!
    }

    std::cout << " Got beamsig " << beamsig << std::endl;

    std::auto_ptr<DiPhotonMVAResultMap> assoc(new DiPhotonMVAResultMap);

    for (unsigned int candIndex =0; candIndex < diPhotonPointers.size() ; candIndex++) {
      flashgg::DiPhotonMVAResult mvares;

      edm::Ptr<reco::Vertex> vtx = diPhotonPointers[candIndex]->getVertex();
      const flashgg::Photon* g1 = diPhotonPointers[candIndex]->leadingPhoton();
      const flashgg::Photon* g2 = diPhotonPointers[candIndex]->subLeadingPhoton();

      float r1 = g1->superCluster()->position().r();
      float r2 = g2->superCluster()->position().r();
      float cos_term = TMath::Cos(g1->phi()-g2->phi());
      float sech1 = 1.0/TMath::CosH(g1->eta());
      float sech2 = 1.0/TMath::CosH(g2->eta());
      float tanh1 = 1.0/TMath::TanH(g1->phi());
      float tanh2 = 1.0/TMath::TanH(g2->phi());
      float numerator1 = sech1*(sech1*tanh2-tanh1*sech2*cos_term);
      float numerator2 = sech2*(sech2*tanh1-tanh2*sech1*cos_term);
      float denominator = 1. - tanh1*tanh2 - sech1*sech2*cos_term;
      float angleResolution = ((-1.*beamsig*TMath::Sqrt(.2))/denominator)*(numerator1/r1 + numerator2/r2);
      float alpha_sig = 0.5*angleResolution;
      float SigmaM = TMath::Sqrt(g1->getSigEOverE()*g1->getSigEOverE() + g2->getSigEOverE()*g2->getSigEOverE());
      float MassResolutionWrongVtx = TMath::Sqrt((SigmaM*SigmaM)+(alpha_sig*alpha_sig));

      mvares.leadptom       = g1->pt()/(diPhotonPointers[candIndex]->mass());
      mvares.subleadptom    = g2->pt()/(diPhotonPointers[candIndex]->mass());
      mvares.subleadmva     = g2->getPhoIdMvaDWrtVtx(vtx);
      mvares.leadmva        = g1->getPhoIdMvaDWrtVtx(vtx);
      mvares.leadeta        = g2->eta();
      mvares.subleadeta     = g1->eta();
      mvares.sigmarv        = .5*sqrt((g1->getSigEOverE())*(g1->getSigEOverE()) + (g2->getSigEOverE())*(g2->getSigEOverE()));
      mvares.sigmawv        = MassResolutionWrongVtx;
      mvares.CosPhi         = TMath::Cos(g1->phi()-g2->phi());
      mvares.vtxprob        =  1.-.49*(1+diPhotonPointers[candIndex]->getVtxProbMVA());
      

      if (!DiphotonMva_initialized) {
	DiphotonMva_initialized = true;
	DiphotonMva_ = new TMVA::Reader("!Color:Silent");
	DiphotonMva_->AddVariable("masserrsmeared/mass",&mvares.sigmarv);
	DiphotonMva_->AddVariable("masserrsmearedwrongvtx/mass",&mvares.sigmawv);
	DiphotonMva_->AddVariable("vtxprob",&mvares.vtxprob);
	DiphotonMva_->AddVariable("ph1.pt/mass",&mvares.leadptom);
	DiphotonMva_->AddVariable("ph2.pt/mass",&mvares.subleadptom);
	DiphotonMva_->AddVariable("ph1.eta",&mvares.leadeta);
	DiphotonMva_->AddVariable("ph2.eta",&mvares.subleadeta);
	DiphotonMva_->AddVariable("TMath::Cos(ph1.phi-ph2.phi)",&mvares.CosPhi);
	DiphotonMva_->AddVariable("ph1.idmva",&mvares.leadmva);
	DiphotonMva_->AddVariable("ph2.idmva",&mvares.subleadmva);
	DiphotonMva_->BookMVA("BDT",diphotonMVAweightfile_.fullPath());
      }

      mvares.result = DiphotonMva_->EvaluateMVA("BDT");

      std::cout << " Got result " << mvares.result << std::endl;

      assoc->insert(std::make_pair(diPhotonPointers[candIndex],mvares));
    }
    evt.put(assoc);
  }
}

typedef flashgg::DiPhotonMVAProducer FlashggDiPhotonMVAProducer;
DEFINE_FWK_MODULE(FlashggDiPhotonMVAProducer);
