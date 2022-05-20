#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include "DataFormats/Candidate/interface/Candidate.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"

#include "RecoBTag/FeatureTools/interface/TrackInfoBuilder.h"
#include "RecoBTag/FeatureTools/interface/deep_helpers.h"
#include "RecoBTag/FeatureTools/interface/sorting_modules.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"

#include "DataFormats/Candidate/interface/VertexCompositePtrCandidate.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "DataFormats/BTauReco/interface/DeepBoostedJetTagInfo.h"

using namespace btagbtvdeep;

class SVFlavourTagInfoProducer : public edm::stream::EDProducer<> {
public:
  explicit SVFlavourTagInfoProducer(const edm::ParameterSet &);
  ~SVFlavourTagInfoProducer() override;

  static void fillDescriptions(edm::ConfigurationDescriptions &descriptions);

private:
  typedef std::vector<reco::DeepBoostedJetTagInfo> DeepBoostedJetTagInfoCollection;
  typedef reco::VertexCompositePtrCandidateCollection SVCollection;
  typedef reco::VertexCompositePtrCandidate SV;
  typedef reco::VertexCollection VertexCollection;
  typedef reco::GenParticleCollection GenParticleCollection;
  typedef reco::GenParticleRefVector GenParticleRefVector;
  typedef edm::View<reco::Candidate> CandidateView;
  typedef std::vector<pat::Jet> JetCollection;

  void beginStream(edm::StreamID) override {}
  void produce(edm::Event &, const edm::EventSetup &) override;
  void endStream() override {}

  void fillParticleFeatures(DeepBoostedJetFeatures &fts, const SV &sv, std::vector<reco::CandidatePtr> &matched_cands);
  void matchSVWithPFCands(std::unique_ptr<JetCollection> &svPhantomJets);

  const double deltar_match_sv_pfcand_;

  edm::EDGetTokenT<VertexCollection> vtx_token_;
  edm::EDGetTokenT<SVCollection> sv_token_;
  edm::EDGetTokenT<CandidateView> pfcand_token_;
  edm::EDGetTokenT<GenParticleCollection> genpart_token_;
  edm::EDGetTokenT<GenParticleRefVector> bhadron_token_;
  edm::EDGetTokenT<GenParticleRefVector> chadron_token_;

  edm::Handle<VertexCollection> vtxs_;
  edm::Handle<SVCollection> svs_;
  edm::Handle<CandidateView> pfcands_;
  edm::Handle<GenParticleCollection> genparts_;
  edm::Handle<GenParticleRefVector> bhadrons_;
  edm::Handle<GenParticleRefVector> chadrons_;
  edm::ESHandle<TransientTrackBuilder> track_builder_;

  const static std::vector<std::string> particle_features_;
  const static std::vector<std::string> sv_features_;
  const reco::Vertex *pv_ = nullptr;

  bool debug_ = false;
};

const std::vector<std::string> SVFlavourTagInfoProducer::particle_features_{
    "pfcand_puppiw",        "pfcand_hcalFrac",       "pfcand_VTX_ass",      "pfcand_lostInnerHits",
    "pfcand_quality",       "pfcand_charge",         "pfcand_isEl",         "pfcand_isMu",
    "pfcand_isChargedHad",  "pfcand_isGamma",        "pfcand_isNeutralHad", "pfcand_phirel",
    "pfcand_etarel",        "pfcand_deltaR",         "pfcand_abseta",       "pfcand_ptrel_log",
    "pfcand_erel_log",      "pfcand_pt_log",         
    "pfcand_normchi2",      "pfcand_dz",             "pfcand_dzsig",
    "pfcand_dxy",           "pfcand_dxysig",         "pfcand_dptdpt",       "pfcand_detadeta",
    "pfcand_dphidphi",      "pfcand_dxydxy",         "pfcand_dzdz",         "pfcand_dxydz",
    "pfcand_dphidxy",       "pfcand_dlambdadz",      "pfcand_btagEtaRel",   "pfcand_btagPtRatio",
    "pfcand_btagPParRatio", "pfcand_btagSip2dVal",   "pfcand_btagSip2dSig", "pfcand_btagSip3dVal",
    "pfcand_btagSip3dSig",  "pfcand_btagJetDistVal", "pfcand_mask",         "pfcand_pt_log_nopuppi",
    "pfcand_e_log_nopuppi", "pfcand_ptrel",          "pfcand_erel"};

SVFlavourTagInfoProducer::SVFlavourTagInfoProducer(const edm::ParameterSet &iConfig)
    : deltar_match_sv_pfcand_(iConfig.getParameter<double>("deltar_match_sv_pfcand")),
      vtx_token_(consumes<VertexCollection>(iConfig.getParameter<edm::InputTag>("vertices"))),
      sv_token_(consumes<SVCollection>(iConfig.getParameter<edm::InputTag>("secondary_vertices"))),
      pfcand_token_(consumes<CandidateView>(iConfig.getParameter<edm::InputTag>("pf_candidates"))),
      genpart_token_(consumes<GenParticleCollection>(iConfig.getParameter<edm::InputTag>("genparticles"))),
      bhadron_token_(consumes<GenParticleRefVector>(iConfig.getParameter<edm::InputTag>("bHadrons"))),
      chadron_token_(consumes<GenParticleRefVector>(iConfig.getParameter<edm::InputTag>("cHadrons"))),
      debug_(iConfig.getUntrackedParameter<bool>("debugMode", false)) {

  produces<DeepBoostedJetTagInfoCollection>();
  produces<JetCollection>("svPhantomJets");
}

SVFlavourTagInfoProducer::~SVFlavourTagInfoProducer() {}

void SVFlavourTagInfoProducer::fillDescriptions(edm::ConfigurationDescriptions &descriptions) {
  // pfSVFlavourTagInfos
  edm::ParameterSetDescription desc;
  desc.add<double>("deltar_match_sv_pfcand", 0.4);
  desc.add<edm::InputTag>("vertices", edm::InputTag("offlinePrimaryVertices"));
  desc.add<edm::InputTag>("secondary_vertices", edm::InputTag("inclusiveCandidateSecondaryVertices"));
  desc.add<edm::InputTag>("pf_candidates", edm::InputTag("particleFlow"));
  desc.add<edm::InputTag>("genparticles", edm::InputTag("prunedGenParticles"));
  desc.add<edm::InputTag>("bHadrons", edm::InputTag("patJetPartons", "bHadrons"));
  desc.add<edm::InputTag>("cHadrons", edm::InputTag("patJetPartons", "cHadrons"));
  desc.addOptionalUntracked<bool>("debugMode", false);
  descriptions.add("pfSVFlavourTagInfos", desc);
}

void SVFlavourTagInfoProducer::produce(edm::Event &iEvent, const edm::EventSetup &iSetup) {
  auto output_tag_infos = std::make_unique<DeepBoostedJetTagInfoCollection>();

  iEvent.getByToken(vtx_token_, vtxs_);
  if (vtxs_->empty()) {
    // produce empty TagInfos in case no primary vertex
    iEvent.put(std::move(output_tag_infos));
    return;  // exit event
  }
  // primary vertex
  pv_ = &vtxs_->at(0);

  iEvent.getByToken(sv_token_, svs_);
  iEvent.getByToken(pfcand_token_, pfcands_);

  iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder", track_builder_);

  // create jet collection
  auto svPhantomJets = std::make_unique<JetCollection>();
  for (std::size_t sv_n = 0; sv_n < svs_->size(); sv_n++) {
    pat::Jet ajet;
    svPhantomJets->push_back(ajet);
  }

  // SV truth matching
  if (!iEvent.isRealData()) {
    iEvent.getByToken(genpart_token_, genparts_);
    iEvent.getByToken(bhadron_token_, bhadrons_);
    iEvent.getByToken(chadron_token_, chadrons_);
    matchSVWithPFCands(svPhantomJets);
  }

  for (std::size_t sv_n = 0; sv_n < svs_->size(); sv_n++) {
    const auto &sv = svs_->at(sv_n);
    auto jet_ref = edm::Ref<JetCollection>(svPhantomJets.get(), sv_n);
    edm::RefToBase<reco::Jet> jet_refbase(jet_ref);

    // create jet features
    DeepBoostedJetFeatures features;
    // declare all the feature variables (init as empty vector)
    for (const auto &name : particle_features_) {
      features.add(name);
    }

    // select daughters matched to the SV
    std::vector<reco::CandidatePtr> matched_cands;
    for (std::size_t cand_n = 0; cand_n < pfcands_->size(); cand_n++) {
      const auto &cand = pfcands_->ptrAt(cand_n);
      if (reco::deltaR(*cand, sv) < deltar_match_sv_pfcand_) {
        matched_cands.push_back(cand);
      }
    }
    std::sort(matched_cands.begin(), matched_cands.end(), [](const reco::CandidatePtr a, const reco::CandidatePtr b) {
      return a->pt() > b->pt();
    });

    // fill particle features
    fillParticleFeatures(features, sv, matched_cands);
    if (!matched_cands.empty()) {
      features.check_consistency(particle_features_);
    }
    
    if (debug_) {
      std::cout << "SV # " << sv_n << "  candidates matched: " << matched_cands.size() << std::endl;
      auto printFeatures = [&](const auto &vector) {
        for (const auto &value : vector) std::cout << value << ", ";
      };
      for (const auto &name : particle_features_) {
        std::cout << name << " : "; 
        printFeatures(features.get(name));
        std::cout << std::endl;
      }
    }

    // this should always be done even if features are not filled
    output_tag_infos->emplace_back(features, jet_refbase);
  }

  iEvent.put(std::move(output_tag_infos));
  iEvent.put(std::move(svPhantomJets), "svPhantomJets");
}

void SVFlavourTagInfoProducer::fillParticleFeatures(DeepBoostedJetFeatures &fts, const SV &sv, std::vector<reco::CandidatePtr> &matched_cands) {

  // some jet properties
  math::XYZVector sv_dir = sv.momentum().Unit();
  GlobalVector sv_ref_track_dir(sv.px(),sv.py(),sv.pz());
  const float etasign = sv.eta() > 0 ? 1 : -1;

  // reserve space
  for (const auto &name : particle_features_) {
    fts.reserve(name, matched_cands.size());
  }

  auto useTrackProperties = [&](const reco::PFCandidate *reco_cand) {
    const auto *trk = reco_cand->bestTrack();
    return trk != nullptr;
  };

  for (const auto &cand : matched_cands) {
    const auto *packed_cand = dynamic_cast<const pat::PackedCandidate *>(&(*cand));
    const auto *reco_cand = dynamic_cast<const reco::PFCandidate *>(&(*cand));

    auto candP4 = cand->p4();
    float hcal_fraction = 0.;
    if (packed_cand->pdgId() == 1 || packed_cand->pdgId() == 130) {
    hcal_fraction = packed_cand->hcalFraction();
    } else if (packed_cand->isIsolatedChargedHadron()) {
    hcal_fraction = packed_cand->rawHcalFraction();
    }

    fts.fill("pfcand_hcalFrac", hcal_fraction);
    fts.fill("pfcand_VTX_ass", packed_cand->pvAssociationQuality());
    fts.fill("pfcand_lostInnerHits", packed_cand->lostInnerHits());
    fts.fill("pfcand_quality", packed_cand->bestTrack() ? packed_cand->bestTrack()->qualityMask() : 0);

    fts.fill("pfcand_puppiw", packed_cand->puppiWeight());
    fts.fill("pfcand_charge", packed_cand->charge());
    fts.fill("pfcand_isEl", std::abs(packed_cand->pdgId()) == 11);
    fts.fill("pfcand_isMu", std::abs(packed_cand->pdgId()) == 13);
    fts.fill("pfcand_isChargedHad", std::abs(packed_cand->pdgId()) == 211);
    fts.fill("pfcand_isGamma", std::abs(packed_cand->pdgId()) == 22);
    fts.fill("pfcand_isNeutralHad", std::abs(packed_cand->pdgId()) == 130);

    // impact parameters
    fts.fill("pfcand_dz", packed_cand->dz());
    fts.fill("pfcand_dxy", packed_cand->dxy());
    fts.fill("pfcand_dzsig", packed_cand->bestTrack() ? packed_cand->dz() / packed_cand->dzError() : 0);
    fts.fill("pfcand_dxysig", packed_cand->bestTrack() ? packed_cand->dxy() / packed_cand->dxyError() : 0);

    // basic kinematics
    fts.fill("pfcand_phirel", reco::deltaPhi(candP4, sv));
    fts.fill("pfcand_etarel", etasign * (candP4.eta() - sv.eta()));
    fts.fill("pfcand_deltaR", reco::deltaR(candP4, sv));
    fts.fill("pfcand_abseta", std::abs(candP4.eta()));

    fts.fill("pfcand_ptrel_log", std::log(candP4.pt() / sv.pt()));
    fts.fill("pfcand_ptrel", candP4.pt() / sv.pt());
    fts.fill("pfcand_erel_log", std::log(candP4.energy() / sv.energy()));
    fts.fill("pfcand_erel", candP4.energy() / sv.energy());
    fts.fill("pfcand_pt_log", std::log(candP4.pt()));

    fts.fill("pfcand_mask", 1);
    fts.fill("pfcand_pt_log_nopuppi", std::log(cand->pt()));
    fts.fill("pfcand_e_log_nopuppi", std::log(cand->energy()));


    const reco::Track *trk = nullptr;
    if (packed_cand) {
      trk = packed_cand->bestTrack();
    } else if (reco_cand && useTrackProperties(reco_cand)) {
      trk = reco_cand->bestTrack();
    }
    if (trk) {
      fts.fill("pfcand_normchi2", std::floor(trk->normalizedChi2()));

      // track covariance
      auto cov = [&](unsigned i, unsigned j) { return trk->covariance(i, j); };
      fts.fill("pfcand_dptdpt", cov(0, 0));
      fts.fill("pfcand_detadeta", cov(1, 1));
      fts.fill("pfcand_dphidphi", cov(2, 2));
      fts.fill("pfcand_dxydxy", cov(3, 3));
      fts.fill("pfcand_dzdz", cov(4, 4));
      fts.fill("pfcand_dxydz", cov(3, 4));
      fts.fill("pfcand_dphidxy", cov(2, 3));
      fts.fill("pfcand_dlambdadz", cov(1, 4));

      TrackInfoBuilder trkinfo(track_builder_);
      trkinfo.buildTrackInfo(&(*cand), sv_dir, sv_ref_track_dir, *pv_);
      fts.fill("pfcand_btagEtaRel", trkinfo.getTrackEtaRel());
      fts.fill("pfcand_btagPtRatio", trkinfo.getTrackPtRatio());
      fts.fill("pfcand_btagPParRatio", trkinfo.getTrackPParRatio());
      fts.fill("pfcand_btagSip2dVal", trkinfo.getTrackSip2dVal());
      fts.fill("pfcand_btagSip2dSig", trkinfo.getTrackSip2dSig());
      fts.fill("pfcand_btagSip3dVal", trkinfo.getTrackSip3dVal());
      fts.fill("pfcand_btagSip3dSig", trkinfo.getTrackSip3dSig());
      fts.fill("pfcand_btagJetDistVal", trkinfo.getTrackJetDistVal());
    } else {
      fts.fill("pfcand_normchi2", 999);

      fts.fill("pfcand_dptdpt", 0);
      fts.fill("pfcand_detadeta", 0);
      fts.fill("pfcand_dphidphi", 0);
      fts.fill("pfcand_dxydxy", 0);
      fts.fill("pfcand_dzdz", 0);
      fts.fill("pfcand_dxydz", 0);
      fts.fill("pfcand_dphidxy", 0);
      fts.fill("pfcand_dlambdadz", 0);

      fts.fill("pfcand_btagEtaRel", 0);
      fts.fill("pfcand_btagPtRatio", 0);
      fts.fill("pfcand_btagPParRatio", 0);
      fts.fill("pfcand_btagSip2dVal", 0);
      fts.fill("pfcand_btagSip2dSig", 0);
      fts.fill("pfcand_btagSip3dVal", 0);
      fts.fill("pfcand_btagSip3dSig", 0);
      fts.fill("pfcand_btagJetDistVal", 0);
    }
  }
}

void SVFlavourTagInfoProducer::matchSVWithPFCands(std::unique_ptr<JetCollection> &svPhantomJets) {
  auto &SVs = svs_;
  auto &particles = genparts_;

  int matchedIDs[SVs->size()] = {-1};
  float lightdr;
  auto hadronFlavorID = [](int id) {
    if ((std::abs(id) > 400 && std::abs(id) < 500) || (std::abs(id) > 4000 && std::abs(id) < 5000)) {
      return 4;
    } else if ((std::abs(id) > 500 && std::abs(id) < 600) || (std::abs(id) > 5000 && std::abs(id) < 6000)) {
      return 5;
    } else {
      return -1;
    }
  };
  auto hadronFlavor = [&hadronFlavorID](const reco::GenParticle& gp) { // was GenParticle* gp
    int id = hadronFlavorID(gp.pdgId());
    if (id == 4) { // c -- may be b->c
      reco::GenParticle const* gp_ = &gp;
      //std::cout << "in hadrFlav, had id=4" << std::endl;
      while (gp_->numberOfMothers() != 0) {
        /*if (gp_->numberOfMothers() > 1) {
          std::cout << "    IN HADRONFLAVOR:  Found >1 mothers: " << gp_->numberOfMothers() << std::endl;
          for (unsigned i=0; i<gp_->numberOfMothers(); i++) {
            std::cout << "       mother " << i << " pdgID: " << hadronFlavorID(gp_->mother(i)->pdgId()) << ", nmothers: " << gp_->mother(i)->numberOfMothers() << ", mother ID=" << hadronFlavorID(gp_->mother(i)->mother(0)->pdgId()) << std::endl;
          }
        }*/
        if (hadronFlavorID(gp_->pdgId()) == 5) {
          //std::cout << "  found mother, hadID 5" << std::endl;
          return 10;
        }
        //std::cout << "  mother hadID incorrect, checking next mother" << std::endl;
        gp_ = (reco::GenParticle*)(gp_->mother(0));
      }
    }
    return id;
  };


  // ====================
  // Adopt SV - PF candidates matching algorithm from code
  // https://github.com/p-masterson/DNNTuples/blob/6dcc20fce79f78765ea690d8b130e0f5874d0d5e/Ntupler/src/SVFiller.cc#L109-L251

  // matchedIDs = new int[SVs->size()];  // NOTE:  idxsv should give you the nth entry of matchedIDs

  // //check num of good jets
  // int n_good_jets = 0;
  // for (unsigned j=0; j<jets->size(); j++) {
  //   const auto& jet = jets->at(j);
  //   if ((jet.pt() > 40) 
  //      && (std::abs(jet.eta()) < 2.5) ) {
  //      //&& (jet.hadronFlavour() & 4) ) {
  //     n_good_jets++;
  //   }
  // }
  //std::cout << "Found " << n_good_jets << " good jets" << std::endl;
  //std::cout << "Init SVs: " << SVs->size() << std::endl;

  // if sv close to a jet, assign ID of -999 and ignore it
  /*
  for (unsigned i=0; i<SVs->size(); i++) {
    const auto& sv = SVs->at(i);
    matchedIDs[i] = -1;  // initialize
    for (unsigned j=0; j<jets->size(); j++) {
      const auto& jet = jets->at(j);
      if (  (reco::deltaR(jet, sv) < 0.4)
         && (jet.pt() > 40)
         && (std::abs(jet.eta()) < 2.5)) {
         //&& (jet.hadronFlavour() & 4) ) {  // not 100% sure whether this is correct
         //&& (jet.jetID() & 4) ) {  // don't know how to add this...
        matchedIDs[i] = -999;  // -999 = ignore
        //std::cout << "     SV " << i << " close to jet " << j << std::endl;
      } else if (  (reco::deltaR(jet, sv) < 0.50)
         && (jet.pt() > 35)
         && (std::abs(jet.eta()) < 2.7)) {
        //std::cout << "   close match" << std::endl;
      }
    }
  }
  */

  // - create full list of (sv, hadr pairs); dr-sorted
  //std::cout << "Jet-free SVs: " << jet_free_svs << std::endl;
  std::vector<std::tuple<float, unsigned, unsigned>> pairList; // sv num, had num
  for (unsigned i=0; i<SVs->size(); i++) {
    const auto& sv = SVs->at(i);
    if (matchedIDs[i] == -999) continue; // if sv is near a jet, skip it
    //std::cout << "found sv " << i << std::endl;
    for (unsigned j=0; j<particles->size(); j++) {
      const auto& gp = particles->at(j);
      //if (hadronFlavor(gp) > 0) std::cout << "   hadr flav: " << hadronFlavor(gp) << ", dr=" << reco::deltaR(gp, sv) << std::endl;
      if (!gp.statusFlags().isLastCopy()) continue;
      if ((hadronFlavor(gp) == 4 || hadronFlavor(gp) == 5 || hadronFlavor(gp) == 10)
          && reco::deltaR(gp, sv) < 0.4) {
        //std::cout << "   MATCHED SV " << i << " to particle " << j <<", flav=" << hadronFlavor(gp) << ", dR=" << reco::deltaR(gp, sv) << std::endl;
        pairList.push_back(std::tuple<float, int, int>(deltaR(gp, sv), i, j));
      }
    }
  }
  std::sort(pairList.begin(), pairList.end(),
    [](const std::tuple<float, unsigned, unsigned> & a, const std::tuple<float, unsigned, unsigned> & b) -> bool 
    {return std::get<0>(a) < std::get<0>(b);}  // NOTE:  Flipped direction of <!  May have been wrong before...
    );

  // - take lowest-sep pair and match; remove sv AND hadr from consideration
  //std::cout << "Assigning lowest-sep pairs" << std::endl;
  while (pairList.size() > 0) {
    // pair witih lowest dR:
    std::tuple<float, unsigned, unsigned> sel_tuple = pairList[0];
    const auto& gp = particles->at(std::get<2>(sel_tuple));
    matchedIDs[std::get<1>(sel_tuple)] = hadronFlavor(gp); // matchedIDs[sv#] = gen hadron ID
    //if (hadronFlavor(gp) > 10) {
    //  std::cout << "  ASSIGNED SV " << std::get<1>(sel_tuple) << " to part " << std::get<2>(sel_tuple) << ", hadr type=" << hadronFlavor(gp) << std::endl;
    //}
    // remove all occurences of this sv, hadron
    //for (unsigned i = 0; i < pairList.size(); i++) { // loop through each element of pairList, check for removal
    unsigned ind = 0;
    while (ind < pairList.size()) {
      if (std::get<1>(pairList[ind]) == std::get<1>(sel_tuple) || std::get<2>(pairList[ind]) == std::get<2>(sel_tuple)) {
        // if an element gets erased, do NOT increment ind; the next element will get slotted into ind
        pairList.erase(pairList.begin()+ind);
      } else {
        ind++;
      }
    }
  }
  //std::cout << "Finished removing" << std::endl;

  // - find lights in unmatched SVs (if dR>0.8 for all hadr)
  // note: currently finding too many lights
  int unmatched = 0;
  for (unsigned i=0; i<SVs->size(); i++) {
    if (matchedIDs[i] == -1) unmatched += 1;
  }
  //std::cout << "*Unmatched SVs: " << unmatched << std::endl;

  lightdr = 10;  //unphysically high by default, I think
  std::vector<std::tuple<float, unsigned, unsigned>> pairList_; // sv num, had num
  for (unsigned i=0; i<SVs->size(); i++) {
    const auto& sv = SVs->at(i);
    bool isLight = true;
    if (matchedIDs[i] == -999) continue; // if sv is near a jet, skip it
    for (unsigned j=0; j<particles->size(); j++) {
      const auto& gp = particles->at(j);
      if ((hadronFlavor(gp) == 4 || hadronFlavor(gp) == 5 || hadronFlavor(gp) == 10)
          && reco::deltaR(gp, sv) < 0.8) {
        //pairList_.push_back(std::tuple<float, unsigned, unsigned>(deltaR(gp, sv), i, j));
        isLight = false;
        if (reco::deltaR(gp, sv) < lightdr) lightdr = reco::deltaR(gp, sv);
      }
    }
    if (isLight) {
      //if (matchedIDs[i] != -1) std::cout << "ERROR ERROR ERROR!" << std::endl;
      matchedIDs[i] = 0;
      //std::cout << "  SV " << i << " is light" << std::endl;
    }
    else if (matchedIDs[i] == -1) {
      //std::cout << "  SV " << i << " is unassigned" << std::endl;
    }
  }
  /*
  //std::cout << "Finalizing matching" << std::endl;
  std::cout << "pairList_ size is " << pairList_.size() << std::endl;
  for (unsigned i=0; i<SVs->size(); i++) {
    // if SV not found in pairList, call it a light
    bool isLight = true;
    for (unsigned j=0; j<pairList_.size(); j++) {
      std::tuple<float, unsigned, unsigned> sel_tuple = pairList_[j];
      if (std::get<1>(sel_tuple) == i) isLight = false;
    }
    if (isLight) matchedIDs[i] = 0;
  }
  */


  //finally, move -999s to -1s (now okay to ignore these)
  //for (unsigned i = 0; i<SVs->size(); i++) {
  //  if (matchedIDs[i] == -999) matchedIDs[i] = -1;
  //}

  /*std::cout << "Matching results: [" << std::endl;
  for (unsigned i=0; i<SVs->size(); i++) {
    std::cout << matchedIDs[i] << ", ";
  }
  std::cout << "]\n\n";*/
  //std::cout << "Leaving readevent" << std::endl;

  // End of reference code
  // ====================

  for (std::size_t sv_n = 0; sv_n < svs_->size(); sv_n++) {
    svPhantomJets->at(sv_n).addUserFloat("gen_flavour", matchedIDs[sv_n]);

    auto sv = svs_->at(sv_n);
    int n_b = 0, n_c = 0;
    for (std::size_t had_n = 0; had_n < bhadrons_->size(); had_n++) {
      if (reco::deltaR(*bhadrons_->at(had_n), sv) < deltar_match_sv_pfcand_) n_b++;
    }
    for (std::size_t had_n = 0; had_n < chadrons_->size(); had_n++) {
      if (reco::deltaR(*chadrons_->at(had_n), sv) < deltar_match_sv_pfcand_) n_c++;
    }
    svPhantomJets->at(sv_n).addUserFloat("n_bhadrons", n_b);
    svPhantomJets->at(sv_n).addUserFloat("n_chadrons", n_c);
  }
  
}

// define this as a plug-in
DEFINE_FWK_MODULE(SVFlavourTagInfoProducer);
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
