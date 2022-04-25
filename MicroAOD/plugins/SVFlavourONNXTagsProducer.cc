#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/makeRefToBaseProdFrom.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include "DataFormats/BTauReco/interface/JetTag.h"

#include "DataFormats/BTauReco/interface/DeepBoostedJetTagInfo.h"

#include "PhysicsTools/ONNXRuntime/interface/ONNXRuntime.h"

#include "RecoBTag/FeatureTools/interface/deep_helpers.h"

#include "DataFormats/Candidate/interface/VertexCompositePtrCandidate.h"
#include "flashgg/DataFormats/interface/SecondaryVertex.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <nlohmann/json.hpp>

using namespace cms::Ort;
using namespace btagbtvdeep;

class SVFlavourONNXTagsProducer : public edm::stream::EDProducer<edm::GlobalCache<ONNXRuntime>> {
public:
  explicit SVFlavourONNXTagsProducer(const edm::ParameterSet &, const ONNXRuntime *);
  ~SVFlavourONNXTagsProducer() override;

  static void fillDescriptions(edm::ConfigurationDescriptions &);

  static std::unique_ptr<ONNXRuntime> initializeGlobalCache(const edm::ParameterSet &);
  static void globalEndJob(const ONNXRuntime *);

private:
  typedef std::vector<reco::DeepBoostedJetTagInfo> TagInfoCollection;
  typedef reco::JetTagCollection JetTagCollection;
  typedef std::vector<pat::Jet> JetCollection;
  typedef std::vector<reco::VertexCompositePtrCandidate> SVCollection;

  void produce(edm::Event &, const edm::EventSetup &) override;

  std::vector<float> center_norm_pad(const std::vector<float> &input,
                                     float center,
                                     float scale,
                                     unsigned min_length,
                                     unsigned max_length,
                                     float pad_value = 0,
                                     float replace_inf_value = 0,
                                     float min = 0,
                                     float max = -1);
  void make_inputs(const reco::DeepBoostedJetTagInfo &taginfo);

  const edm::EDGetTokenT<TagInfoCollection> src_;
  const edm::EDGetTokenT<JetCollection> jets_;
  const edm::EDGetTokenT<SVCollection> svs_;
  std::vector<std::string> flav_names_;             // names of the output scores
  std::vector<std::string> input_names_;            // names of each input group - the ordering is important!
  std::vector<std::vector<int64_t>> input_shapes_;  // shapes of each input group (-1 for dynamic axis)
  std::vector<unsigned> input_sizes_;               // total length of each input vector
  std::unordered_map<std::string, PreprocessParams> prep_info_map_;  // preprocessing info for each input group

  FloatArrays data_;

  bool debug_ = false;
};

SVFlavourONNXTagsProducer::SVFlavourONNXTagsProducer(const edm::ParameterSet &iConfig, const ONNXRuntime *cache)
    : src_(consumes<TagInfoCollection>(iConfig.getParameter<edm::InputTag>("src"))),
      jets_(consumes<JetCollection>(iConfig.getParameter<edm::InputTag>("phantom_jets"))),
      svs_(consumes<SVCollection>(iConfig.getParameter<edm::InputTag>("secondary_vertices"))),
      flav_names_(iConfig.getParameter<std::vector<std::string>>("flav_names")),
      debug_(iConfig.getUntrackedParameter<bool>("debugMode", false)) {
  // load preprocessing info
  auto json_path = iConfig.getParameter<std::string>("preprocess_json");
  if (!json_path.empty()) {
    // use preprocessing json file if available
    std::ifstream ifs(edm::FileInPath(json_path).fullPath());
    nlohmann::json js = nlohmann::json::parse(ifs);
    js.at("input_names").get_to(input_names_);
    for (const auto &group_name : input_names_) {
      const auto &group_pset = js.at(group_name);
      auto &prep_params = prep_info_map_[group_name];
      group_pset.at("var_names").get_to(prep_params.var_names);
      if (group_pset.contains("var_length")) {
        prep_params.min_length = group_pset.at("var_length");
        prep_params.max_length = prep_params.min_length;
      } else {
        prep_params.min_length = group_pset.at("min_length");
        prep_params.max_length = group_pset.at("max_length");
        input_shapes_.push_back({1, (int64_t)prep_params.var_names.size(), -1});
      }
      const auto &var_info_pset = group_pset.at("var_infos");
      for (const auto &var_name : prep_params.var_names) {
        const auto &var_pset = var_info_pset.at(var_name);
        double median = var_pset.at("median");
        double norm_factor = var_pset.at("norm_factor");
        double replace_inf_value = var_pset.at("replace_inf_value");
        double lower_bound = var_pset.at("lower_bound");
        double upper_bound = var_pset.at("upper_bound");
        double pad = var_pset.contains("pad") ? double(var_pset.at("pad")) : 0;
        prep_params.var_info_map[var_name] =
            PreprocessParams::VarInfo(median, norm_factor, replace_inf_value, lower_bound, upper_bound, pad);
      }

      // create data storage with a fixed size vector initilized w/ 0
      const auto &len = input_sizes_.emplace_back(prep_params.max_length * prep_params.var_names.size());
      data_.emplace_back(len, 0);
    }
  } else {
    // otherwise use the PSet in the python config file
    const auto &prep_pset = iConfig.getParameterSet("preprocessParams");
    input_names_ = prep_pset.getParameter<std::vector<std::string>>("input_names");
    for (const auto &group_name : input_names_) {
      const auto &group_pset = prep_pset.getParameterSet(group_name);
      auto &prep_params = prep_info_map_[group_name];
      prep_params.var_names = group_pset.getParameter<std::vector<std::string>>("var_names");
      prep_params.min_length = group_pset.getParameter<unsigned>("var_length");
      prep_params.max_length = prep_params.min_length;
      const auto &var_info_pset = group_pset.getParameterSet("var_infos");
      for (const auto &var_name : prep_params.var_names) {
        const auto &var_pset = var_info_pset.getParameterSet(var_name);
        double median = var_pset.getParameter<double>("median");
        double norm_factor = var_pset.getParameter<double>("norm_factor");
        double replace_inf_value = var_pset.getParameter<double>("replace_inf_value");
        double lower_bound = var_pset.getParameter<double>("lower_bound");
        double upper_bound = var_pset.getParameter<double>("upper_bound");
        prep_params.var_info_map[var_name] =
            PreprocessParams::VarInfo(median, norm_factor, replace_inf_value, lower_bound, upper_bound, 0);
      }

      // create data storage with a fixed size vector initiliazed w/ 0
      const auto &len = input_sizes_.emplace_back(prep_params.max_length * prep_params.var_names.size());
      data_.emplace_back(len, 0);
    }
  }

  if (debug_) {
    for (unsigned i = 0; i < input_names_.size(); ++i) {
      const auto &group_name = input_names_.at(i);
      if (!input_shapes_.empty()) {
        std::cout << group_name << "\nshapes: ";
        for (const auto &x : input_shapes_.at(i)) {
          std::cout << x << ", ";
        }
      }
      std::cout << "\nvariables: ";
      for (const auto &x : prep_info_map_.at(group_name).var_names) {
        std::cout << x << ", ";
      }
      std::cout << "\n";
    }
    std::cout << "flav_names: ";
    for (const auto &flav_name : flav_names_) {
      std::cout << flav_name << ", ";
    }
    std::cout << "\n";
  }

  produces<std::vector<flashgg::SecondaryVertex>>();
}

SVFlavourONNXTagsProducer::~SVFlavourONNXTagsProducer() {}

void SVFlavourONNXTagsProducer::fillDescriptions(edm::ConfigurationDescriptions &descriptions) {
  // pfDeepBoostedJetTags
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>("src", edm::InputTag("pfDeepBoostedJetTagInfos"));
  desc.add<edm::InputTag>("phantom_jets", edm::InputTag("pfDeepBoostedJetTagInfos", "jets"));
  desc.add<edm::InputTag>("secondary_vertices", edm::InputTag("slimmedSecondaryVertices"));
  desc.add<std::string>("preprocess_json", "");
  // `preprocessParams` is deprecated -- use the preprocessing json file instead
  edm::ParameterSetDescription preprocessParams;
  preprocessParams.setAllowAnything();
  preprocessParams.setComment("`preprocessParams` is deprecated, please use `preprocess_json` instead.");
  desc.addOptional<edm::ParameterSetDescription>("preprocessParams", preprocessParams);
  desc.add<edm::FileInPath>("model_path",
                            edm::FileInPath("flashgg/MicroAOD/data/ParticleNetSV/V01/model.onnx"));
  desc.add<std::vector<std::string>>("flav_names", std::vector<std::string>{});
  desc.addOptionalUntracked<bool>("debugMode", false);

  descriptions.addWithDefaultLabel(desc);
  descriptions.add("pfSVFlavourONNXTagsProducer", desc);
}

std::unique_ptr<ONNXRuntime> SVFlavourONNXTagsProducer::initializeGlobalCache(const edm::ParameterSet &iConfig) {
  return std::make_unique<ONNXRuntime>(iConfig.getParameter<edm::FileInPath>("model_path").fullPath());
}

void SVFlavourONNXTagsProducer::globalEndJob(const ONNXRuntime *cache) {}

void SVFlavourONNXTagsProducer::produce(edm::Event &iEvent, const edm::EventSetup &iSetup) {
  edm::Handle<TagInfoCollection> tag_infos;
  edm::Handle<JetCollection> jets; // phantom jet collection to catch the gen-matching variable
  edm::Handle<SVCollection> svs;
  iEvent.getByToken(src_, tag_infos);
  iEvent.getByToken(jets_, jets);
  iEvent.getByToken(svs_, svs);

  auto outSVs = std::make_unique<std::vector<flashgg::SecondaryVertex>>();
  for (unsigned sv_n = 0; sv_n < tag_infos->size(); ++sv_n) {
    const auto &taginfo = (*tag_infos)[sv_n];
    outSVs->push_back(svs->at(sv_n)); // do a copy here
    auto &sv = outSVs->back();

    std::vector<float> outputs(flav_names_.size(), 0);  // init as all zeros

    if (!taginfo.features().empty()) {
      // convert inputs
      make_inputs(taginfo);
      // run prediction and get outputs
      outputs = globalCache()->run(input_names_, data_, input_shapes_)[0];
      assert(outputs.size() == flav_names_.size());
    }

    std::vector<float> probs;
    for (std::size_t flav_n = 0; flav_n < flav_names_.size(); flav_n++) {
      probs.push_back(outputs[flav_n]);
    }
    sv.setSvTagProbs(probs);
    sv.setSvGenFlav(jets->at(sv_n).userFloat("gen_flavour"));
  }

  if (debug_) {
    std::cout << "=== " << iEvent.id().run() << ":" << iEvent.id().luminosityBlock() << ":" << iEvent.id().event()
              << " ===" << std::endl;
    for (unsigned sv_n = 0; sv_n < tag_infos->size(); ++sv_n) {
      std::cout << " - SV #" << sv_n << ", gen_flavour=" << outSVs->at(sv_n).svGenFlav() << std::endl;
      for (std::size_t flav_n = 0; flav_n < flav_names_.size(); ++flav_n) {
        std::cout << "    " << flav_names_.at(flav_n) << " = " << outSVs->at(sv_n).svTagProbs()[flav_n] << std::endl;
      }
    }
  }

  // put into the event
  iEvent.put(std::move(outSVs));
}

std::vector<float> SVFlavourONNXTagsProducer::center_norm_pad(const std::vector<float> &input,
                                                                  float center,
                                                                  float norm_factor,
                                                                  unsigned min_length,
                                                                  unsigned max_length,
                                                                  float pad_value,
                                                                  float replace_inf_value,
                                                                  float min,
                                                                  float max) {
  // do variable shifting/scaling/padding/clipping in one go

  assert(min <= pad_value && pad_value <= max);
  assert(min_length <= max_length);

  unsigned target_length = std::clamp((unsigned)input.size(), min_length, max_length);
  std::vector<float> out(target_length, pad_value);
  for (unsigned i = 0; i < input.size() && i < target_length; ++i) {
    out[i] = std::clamp((catch_infs(input[i], replace_inf_value) - center) * norm_factor, min, max);
  }
  return out;
}

void SVFlavourONNXTagsProducer::make_inputs(const reco::DeepBoostedJetTagInfo &taginfo) {
  for (unsigned igroup = 0; igroup < input_names_.size(); ++igroup) {
    const auto &group_name = input_names_[igroup];
    const auto &prep_params = prep_info_map_.at(group_name);
    auto &group_values = data_[igroup];
    group_values.resize(input_sizes_[igroup]);
    // first reset group_values to 0
    std::fill(group_values.begin(), group_values.end(), 0);
    unsigned curr_pos = 0;
    // transform/pad
    for (unsigned i = 0; i < prep_params.var_names.size(); ++i) {
      const auto &varname = prep_params.var_names[i];
      const auto &raw_value = taginfo.features().get(varname);
      const auto &info = prep_params.info(varname);
      auto val = center_norm_pad(raw_value,
                                 info.center,
                                 info.norm_factor,
                                 prep_params.min_length,
                                 prep_params.max_length,
                                 info.pad,
                                 info.replace_inf_value,
                                 info.lower_bound,
                                 info.upper_bound);
      std::copy(val.begin(), val.end(), group_values.begin() + curr_pos);
      curr_pos += val.size();
      if (i == 0 && (!input_shapes_.empty())) {
        input_shapes_[igroup][2] = val.size();
      }

      if (debug_) {
        std::cout << " -- var=" << varname << ", center=" << info.center << ", scale=" << info.norm_factor
                  << ", replace=" << info.replace_inf_value << ", pad=" << info.pad << std::endl;
        for (const auto &v : val) {
          std::cout << v << ",";
        }
        std::cout << std::endl;
      }
    }
    group_values.resize(curr_pos);
  }
}

//define this as a plug-in
DEFINE_FWK_MODULE(SVFlavourONNXTagsProducer);

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

