#ifndef _DNN_HELPER_H_
#define _DNN_HELPER_H_

#include <algorithm>

#include "TLorentzVector.h"

#include "flashgg/DataFormats/interface/Jet.h"
#include "flashgg/DataFormats/interface/Electron.h"
#include "flashgg/DataFormats/interface/Muon.h"

#include "PhysicsTools/TensorFlow/interface/TensorFlow.h"

namespace flashgg {

const int pad_value = -9;

inline
bool sortByValue(const std::pair<int,double>& pair1, const std::pair<int,double>& pair2 ) {
  return pair1.second > pair2.second;
}

class DNN_Helper
{
  public:
    DNN_Helper(string weight_file, bool debug = false);
    ~DNN_Helper();

    void SetInputShapes(unsigned int length_global, unsigned int length_object, unsigned int length_object_sequence);
    void SetInputs(std::vector<edm::Ptr<flashgg::Jet>> jets, std::vector<double> global_features);
    void SetInputs(std::vector<edm::Ptr<flashgg::Jet>> jets, std::vector<edm::Ptr<flashgg::Muon>> muons, std::vector<edm::Ptr<flashgg::Electron>> electrons, std::vector<double> global_features);
    float EvaluateDNN();

  private:
    string weight_file_;

    tensorflow::GraphDef* graph_;
    tensorflow::Session*  session_;
    
    std::vector<double> 		global_features_; // e.g. Met, N_jets, max b-tag score, etc.
    std::vector<std::vector<double>> 	object_features_; // pT ordered list of jets (and leptons)

    unsigned int length_global_; // number of global features
    unsigned int length_object_; // number of features per object
    unsigned int length_object_sequence_; // number of objects per sequence

    bool inputs_set;
    bool debug_;

};

inline
DNN_Helper::~DNN_Helper() {
  tensorflow::closeSession(session_);
  delete graph_;
}

inline
DNN_Helper::DNN_Helper(string weight_file, bool debug) {
  weight_file_ = weight_file;

  graph_ = tensorflow::loadGraphDef((weight_file_).c_str());
  session_ = tensorflow::createSession(graph_);  

  inputs_set = false;

  debug_ = debug;
}

inline
void DNN_Helper::SetInputShapes(unsigned int length_global, unsigned int length_object, unsigned int length_object_sequence) {
  length_global_ = length_global;
  length_object_ = length_object;
  length_object_sequence_ = length_object_sequence;

  //global_features_.resize(length_global_);
  //object_features_.resize(length_object_sequence_);
  //for (unsigned int i = 0; i < object_features_.size(); i++)
  //  object_features_[i].resize(length_object_);
}

inline
void DNN_Helper::SetInputs(std::vector<edm::Ptr<flashgg::Jet>> jets, std::vector<double> global_features) {
  if (global_features.size() != length_global_)
    throw "Global features incorrectly configured!";
  
  // Global features
  global_features_ = global_features;

  // Object features //

  // First, set all values to the pad value
  object_features_.resize(length_object_sequence_);
  for (unsigned int i = 0; i < object_features_.size(); i++)
    object_features_[i].resize(length_object_);
  for (unsigned int i = 0; i < object_features_.size(); i++) {
    for (unsigned int j = 0; j < object_features_[i].size(); j++)
      object_features_[i][j] = pad_value; 
  }

  std::vector<std::vector<double>> object_features_unordered;
  object_features_unordered.resize(jets.size());
  for (unsigned int i = 0; i < object_features_unordered.size(); i++)
    object_features_unordered[i].resize(length_object_);

   for (unsigned int i = 0; i < object_features_unordered.size(); i++) {
    object_features_unordered[i][0] = log(jets[i]->pt());
    object_features_unordered[i][1] = jets[i]->eta();
    object_features_unordered[i][2] = jets[i]->phi();
    object_features_unordered[i][3] = log(jets[i]->energy());
    object_features_unordered[i][4] = jets[i]->bDiscriminator("pfDeepCSVJetTags:probb");
    object_features_unordered[i][5] = jets[i]->bDiscriminator("pfDeepCSVJetTags:probbb");
    object_features_unordered[i][6] = jets[i]->bDiscriminator("pfDeepCSVJetTags:probc");
    object_features_unordered[i][7] = jets[i]->bDiscriminator("pfDeepCSVJetTags:probudsg");
  }

  // Third, order jets by pT
  std::vector<std::pair<unsigned int, double> > pt_ordering;

  for (unsigned int i = 0; i < jets.size(); i++)
    pt_ordering.push_back(std::pair<unsigned int, double>(i, jets[i]->pt()));

  std::sort(pt_ordering.begin(), pt_ordering.end(), sortByValue);
  for (unsigned int i = 0; i < std::min(object_features_.size(), pt_ordering.size()); i++) {
    object_features_[i] = object_features_unordered[pt_ordering[i].first];
  }

  inputs_set = true;
}

inline
void DNN_Helper::SetInputs(std::vector<edm::Ptr<flashgg::Jet>> jets, std::vector<edm::Ptr<flashgg::Muon>> muons, std::vector<edm::Ptr<flashgg::Electron>> electrons, std::vector<double> global_features) {
  if (global_features.size() != length_global_)
    throw "Global features incorrectly configured!";

  // Global features
  global_features_ = global_features;

  // First, set all values to the pad value
  object_features_.resize(length_object_sequence_);
  for (unsigned int i = 0; i < object_features_.size(); i++)
    object_features_[i].resize(length_object_);
  for (unsigned int i = 0; i < object_features_.size(); i++) {
    for (unsigned int j = 0; j < object_features_[i].size(); j++)
      object_features_[i][j] = pad_value;
  }

  std::vector<std::vector<double>> object_features_unordered;
  object_features_unordered.resize(jets.size() + electrons.size() + muons.size());
  for (unsigned int i = 0; i < object_features_unordered.size(); i++)
    object_features_unordered[i].resize(length_object_);

   for (unsigned int i = 0; i < jets.size(); i++) {
    object_features_unordered[i][0] = log(jets[i]->pt());
    object_features_unordered[i][1] = jets[i]->eta();
    object_features_unordered[i][2] = jets[i]->phi();
    object_features_unordered[i][3] = log(jets[i]->energy());
    object_features_unordered[i][4] = jets[i]->bDiscriminator("pfDeepCSVJetTags:probb");
    object_features_unordered[i][5] = jets[i]->bDiscriminator("pfDeepCSVJetTags:probbb");
    object_features_unordered[i][6] = jets[i]->bDiscriminator("pfDeepCSVJetTags:probc");
    object_features_unordered[i][7] = jets[i]->bDiscriminator("pfDeepCSVJetTags:probudsg");
    object_features_unordered[i][8] = -2;
  }

  for (unsigned int i = jets.size(); i < jets.size() + electrons.size(); i++) {
    unsigned int j = i - jets.size();
    object_features_unordered[i][0] = log(electrons[j]->pt());
    object_features_unordered[i][1] = electrons[j]->eta();
    object_features_unordered[i][2] = electrons[j]->phi();
    object_features_unordered[i][3] = log(electrons[j]->energy());
    object_features_unordered[i][4] = -2; 
    object_features_unordered[i][5] = -2;
    object_features_unordered[i][6] = -2;
    object_features_unordered[i][7] = -2;
    object_features_unordered[i][8] = 1; // 1 for electrons
  }

  for (unsigned int i = jets.size() + electrons.size(); i < jets.size() + electrons.size() + muons.size(); i++) {
    unsigned int j = i - (jets.size() + electrons.size());
    object_features_unordered[i][0] = log(muons[j]->pt());
    object_features_unordered[i][1] = muons[j]->eta();
    object_features_unordered[i][2] = muons[j]->phi();
    object_features_unordered[i][3] = log(muons[j]->energy());
    object_features_unordered[i][4] = -2;
    object_features_unordered[i][5] = -2;
    object_features_unordered[i][6] = -2;
    object_features_unordered[i][7] = -2;
    object_features_unordered[i][8] = 0; // 0 for muons
  }

  // Third, order jets by pT
  std::vector<std::pair<unsigned int, double> > pt_ordering;

  for (unsigned int i = 0; i < object_features_unordered.size(); i++)
    pt_ordering.push_back(std::pair<unsigned int, double>(i, object_features_unordered[i][0]));

  std::sort(pt_ordering.begin(), pt_ordering.end(), sortByValue);
  for (unsigned int i = 0; i < std::min(object_features_.size(), pt_ordering.size()); i++) {
    object_features_[i] = object_features_unordered[pt_ordering[i].first];
  }

  inputs_set = true;
}

inline
float DNN_Helper::EvaluateDNN() {
  if (!inputs_set)
    throw "Error: inputs have not been reset since last evaluation!";

  tensorflow::Tensor global_input(tensorflow::DT_FLOAT, {1, length_global_});
  tensorflow::Tensor object_input(tensorflow::DT_FLOAT, tensorflow::TensorShape({1, length_object_sequence_, length_object_}));

  for (unsigned int i = 0; i < length_global_; i++) {
    global_input.matrix<float>()(0,i) = float(global_features_[i]);
    if (debug_)
      cout << "Global feature " << i << ": " << float(global_features_[i]) << endl;
  }

  for (unsigned int i = 0; i < length_object_sequence_; i++) {
    for (unsigned int j = 0; j < length_object_; j++) {
      object_input.tensor<float,3>()(0,i,j) = float(object_features_[i][j]);
      if (debug_)
	cout << "Object feature " << i << ", " << j << ": " << float(object_features_[i][j]) << endl;
    } 
  }

  std::vector<tensorflow::Tensor> output;
 
  tensorflow::run(session_, {{"input_objects", object_input}, {"input_global", global_input}}, {"output/Sigmoid"}, &output);

  global_features_.clear();
  object_features_.clear(); 
  inputs_set = false;

  return output[0].matrix<float>()(0,0);  
}


// Miscellaneous helper functions for DNN
inline
double convert_tmva_to_prob(double score) {
  // Undo TMVA transformation
  double raw_score = -0.5 * log( (2 / (score + 1)) - 1);

  // Apply logistic (sigmoid) transformation
  double prob = 1 / (1 + exp(-raw_score));
  return prob;
}

inline
double helicity(const TLorentzVector particle_1, const TLorentzVector particle_2) {
  TLorentzVector p1 = particle_1;
  TLorentzVector parent = particle_1 + particle_2;

  TVector3 boost_to_parent = -(parent.BoostVector());
  p1.Boost(boost_to_parent);

  TVector3 v1 = p1.Vect();
  TVector3 vParent = parent.Vect();

  double cos_theta_1 = (v1.Dot(vParent)) / (v1.Mag() * vParent.Mag());

  return abs(cos_theta_1);
}

}
#endif // _DNN_HELPER_H_
