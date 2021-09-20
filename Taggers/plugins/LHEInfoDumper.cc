#include "FWCore/Framework/interface/global/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/Utilities/interface/transform.h"
#include "flashgg/DataFormats/interface/LHEInfoObject.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "TLorentzVector.h"

#include <vector>
#include <iostream>

namespace flashgg {

class LHEInfoDumper : public edm::global::EDProducer<> {
public:
  LHEInfoDumper(edm::ParameterSet const& params)
      : lheTag_(edm::vector_transform(params.getParameter<std::vector<edm::InputTag>>("lheInfo"),
                                      [this](const edm::InputTag& tag) { return mayConsume<LHEEventProduct>(tag); }))
  {
    produces<LHEInfoObject>();
  }

  ~LHEInfoDumper() override {}

  void produce(edm::StreamID id, edm::Event& iEvent, const edm::EventSetup& iSetup) const override {

    std::cout << "EDM 999: producing LHEInfoObject " << std::endl;
    std::unique_ptr<LHEInfoObject>  lheInfoObj ( new LHEInfoObject );
    //edm::RefProd<LHEInfoObject> rLheInfoObj = evt.getRefBeforePut<LHEInfoObject>();
    
    edm::Handle<LHEEventProduct> lheInfo;
    for (const auto& lheTag : lheTag_) {
      std::cout << "The LHE input tag is " << lheTag_ << std::endl;
      iEvent.getByToken(lheTag, lheInfo);
      if (lheInfo.isValid()) {
        break;
      }
    }
    std::cout << "putting in the event the lheInfoObj " << std::endl;
    std::cout << "real data = " << iEvent.isRealData() << std::endl;
    std::cout << "lhe info input  = " << lheInfo.isValid() << std::endl;
    if (! iEvent.isRealData() && lheInfo.isValid()) {
      fillLHEObject(*lheInfo, *lheInfoObj);
      iEvent.put(std::move(lheInfoObj));
    } else {
      // boh, da vedere
    }
  }
  
  void fillLHEObject(const LHEEventProduct& lheProd,
                     LHEInfoObject& out) const {

    double lheHT = 0, lheHTIncoming = 0;
    unsigned int lheNj = 0, lheNb = 0, lheNc = 0, lheNuds = 0, lheNglu = 0;
    double alphaS = 0;

    const auto& hepeup = lheProd.hepeup();
    const auto& pup = hepeup.PUP;
    int lep = -1, lepBar = -1, nu = -1, nuBar = -1;
    std::vector<float> vals_pt;
    std::vector<float> vals_eta;
    std::vector<float> vals_phi;
    std::vector<float> vals_mass;
    std::vector<float> vals_pz;
    std::vector<int> vals_pid;
    std::vector<int> vals_status;
    std::vector<int> vals_spin;
    alphaS = hepeup.AQCDUP;
    for (unsigned int i = 0, n = pup.size(); i < n; ++i) {
      int status = hepeup.ISTUP[i];
      int idabs = std::abs(hepeup.IDUP[i]);
      if (status == 1 || status == -1) {
        TLorentzVector p4(pup[i][0], pup[i][1], pup[i][2], pup[i][3]);  // x,y,z,t
        vals_pid.push_back(hepeup.IDUP[i]);
        vals_spin.push_back(hepeup.SPINUP[i]);
        vals_status.push_back(status);
        if (status == -1) {
          vals_pt.push_back(0);
          vals_eta.push_back(0);
          vals_phi.push_back(0);
          vals_mass.push_back(0);
          vals_pz.push_back(p4.Pz());
        } else {
          vals_pt.push_back(p4.Pt());
          vals_eta.push_back(p4.Eta());
          vals_phi.push_back(p4.Phi());
          vals_mass.push_back(p4.M());
          vals_pz.push_back(0);
        }
      }
      if ((status == 1) && ((idabs == 21) || (idabs > 0 && idabs < 7))) {  //# gluons and quarks
        // object counters
        lheNj++;
        if (idabs == 5)
          lheNb++;
        else if (idabs == 4)
          lheNc++;
        else if (idabs <= 3)
          lheNuds++;
        else if (idabs == 21)
          lheNglu++;
        // HT
        double pt = std::hypot(pup[i][0], pup[i][1]);  // first entry is px, second py
        lheHT += pt;
        int mothIdx = std::max(
            hepeup.MOTHUP[i].first - 1,
            0);  //first and last mother as pair; first entry has index 1 in LHE; incoming particles return motherindex 0
        int mothIdxTwo = std::max(hepeup.MOTHUP[i].second - 1, 0);
        int mothStatus = hepeup.ISTUP[mothIdx];
        int mothStatusTwo = hepeup.ISTUP[mothIdxTwo];
        bool hasIncomingAsMother = mothStatus < 0 || mothStatusTwo < 0;
        if (hasIncomingAsMother)
          lheHTIncoming += pt;
      } else if (idabs == 12 || idabs == 14 || idabs == 16) {
        (hepeup.IDUP[i] > 0 ? nu : nuBar) = i;
      } else if (idabs == 11 || idabs == 13 || idabs == 15) {
        (hepeup.IDUP[i] > 0 ? lep : lepBar) = i;
      }
    }
    std::pair<int, int> v(0, 0);
    if (lep != -1 && lepBar != -1)
      v = std::make_pair(lep, lepBar);
    else if (lep != -1 && nuBar != -1)
      v = std::make_pair(lep, nuBar);
    else if (nu != -1 && lepBar != -1)
      v = std::make_pair(nu, lepBar);
    else if (nu != -1 && nuBar != -1)
      v = std::make_pair(nu, nuBar);

    out.setNjets(lheNj);
    out.setAlphaS(alphaS);

//     out.addColumnValue<uint8_t>("Njets", lheNj, "Number of jets (partons) at LHE step");
//     out.addColumnValue<uint8_t>("Nb", lheNb, "Number of b partons at LHE step");
//     out.addColumnValue<uint8_t>("Nc", lheNc, "Number of c partons at LHE step");
//     out.addColumnValue<uint8_t>("Nuds", lheNuds, "Number of u,d,s partons at LHE step");
//     out.addColumnValue<uint8_t>("Nglu", lheNglu, "Number of gluon partons at LHE step");
//     out.addColumnValue<float>("HT", lheHT, "HT, scalar sum of parton pTs at LHE step");
//     out.addColumnValue<float>(
//         "HTIncoming", lheHTIncoming, "HT, scalar sum of parton pTs at LHE step, restricted to partons");
//     out.addColumnValue<float>("Vpt", lheVpt, "pT of the W or Z boson at LHE step");
//     out.addColumnValue<uint8_t>("NpNLO", lheProd.npNLO(), "number of partons at NLO");
//     out.addColumnValue<uint8_t>("NpLO", lheProd.npLO(), "number of partons at LO");
//     out.addColumnValue<float>("AlphaS", alphaS, "Per-event alphaS");

//     auto outPart = std::make_unique<nanoaod::FlatTable>(vals_pt.size(), "LHEPart", false);
//     outPart->addColumn<float>("pt", vals_pt, "Pt of LHE particles", this->precision_);
//     outPart->addColumn<float>("eta", vals_eta, "Pseodorapidity of LHE particles", this->precision_);
//     outPart->addColumn<float>("phi", vals_phi, "Phi of LHE particles", this->precision_);
// v    outPart->addColumn<float>("mass", vals_mass, "Mass of LHE particles", this->precision_);
//     outPart->addColumn<float>("incomingpz", vals_pz, "Pz of incoming LHE particles", this->precision_);
//     outPart->addColumn<int>("pdgId", vals_pid, "PDG ID of LHE particles");
//     outPart->addColumn<int>("status", vals_status, "LHE particle status; -1:incoming, 1:outgoing");
//     outPart->addColumn<int>("spin", vals_spin, "Spin of LHE particles");

//     return outPart;
  }


protected:
  const std::vector<edm::EDGetTokenT<LHEEventProduct>> lheTag_;
};

}

#include "FWCore/Framework/interface/MakerMacros.h"
typedef flashgg::LHEInfoDumper FlashggLHEInfoDumper; 
DEFINE_FWK_MODULE(FlashggLHEInfoDumper);
