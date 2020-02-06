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

class TTH_DNN_Helper
{
    public:
        TTH_DNN_Helper(string weight_file, bool debug = false);
        ~TTH_DNN_Helper();

        void SetInputShapes(unsigned int length_global, unsigned int length_object, unsigned int length_object_sequence);
        void SetInputs(std::vector<edm::Ptr<flashgg::Jet>> jets, std::vector<double> global_features);
        void SetInputs(std::vector<edm::Ptr<flashgg::Jet>> jets, std::vector<edm::Ptr<flashgg::Muon>> muons, std::vector<edm::Ptr<flashgg::Electron>> electrons, std::vector<double> global_features);

        void SetPreprocessingSchemes(std::vector<double> preprocess_scheme_global_features_mean, std::vector<double> preprocess_scheme_global_features_stddev, std::vector<double> preprocess_scheme_object_features_mean, std::vector<double> preprocess_scheme_object_features_stddev);

        float EvaluateDNN();

    private:
        void Preprocess();

        string weight_file_;

        tensorflow::GraphDef* graph_;
        tensorflow::Session*  session_;
        
        std::vector<double> 		        global_features_; // e.g. Met, N_jets, max b-tag score, etc.
        std::vector<std::vector<double>> 	object_features_; // pT ordered list of jets (and leptons)

        std::vector<double>                 preprocess_scheme_global_features_mean_;
        std::vector<double>                 preprocess_scheme_global_features_stddev_;
        std::vector<double>                 preprocess_scheme_object_features_mean_;
        std::vector<double>                 preprocess_scheme_object_features_stddev_;

        unsigned int length_global_; // number of global features
        unsigned int length_object_; // number of features per object
        unsigned int length_object_sequence_; // number of objects per sequence

        bool inputs_set_;
        bool debug_;

        bool preprocess_;
};

inline
TTH_DNN_Helper::~TTH_DNN_Helper() {
    tensorflow::closeSession(session_);
    delete graph_;
}

inline
TTH_DNN_Helper::TTH_DNN_Helper(string weight_file, bool debug) {
    weight_file_ = weight_file;

    graph_ = tensorflow::loadGraphDef((weight_file_).c_str());
    session_ = tensorflow::createSession(graph_);  

    inputs_set_ = false;
    preprocess_ = false;

    debug_ = debug;
}

inline
void TTH_DNN_Helper::SetPreprocessingSchemes(std::vector<double> preprocess_scheme_global_features_mean, std::vector<double> preprocess_scheme_global_features_stddev, std::vector<double> preprocess_scheme_object_features_mean, std::vector<double> preprocess_scheme_object_features_stddev) {

    preprocess_scheme_global_features_mean_ = preprocess_scheme_global_features_mean;
    preprocess_scheme_global_features_stddev_ = preprocess_scheme_global_features_stddev;
    preprocess_scheme_object_features_mean_ = preprocess_scheme_object_features_mean;
    preprocess_scheme_object_features_stddev_ = preprocess_scheme_object_features_stddev;

    if (debug_) {
        cout << "Global feature means: " << endl;
        for (unsigned int i = 0; i < preprocess_scheme_global_features_mean_.size(); i++)
            cout << i << ": " << preprocess_scheme_global_features_mean_[i] << endl;

        cout << "Global feature stddevs: " << endl;
        for (unsigned int i = 0; i < preprocess_scheme_global_features_stddev_.size(); i++)
            cout << i << ": " << preprocess_scheme_global_features_stddev_[i] << endl;

        cout << "Object feature means: " << endl;
        for (unsigned int i = 0; i < preprocess_scheme_object_features_mean_.size(); i++)
            cout << i << ": " << preprocess_scheme_object_features_mean_[i] << endl;

        cout << "Object feature stddevs: " << endl;
        for (unsigned int i = 0; i < preprocess_scheme_object_features_stddev_.size(); i++)
            cout << i << ": " << preprocess_scheme_object_features_stddev_[i] << endl;
    }

    preprocess_ = true;
}

inline
void TTH_DNN_Helper::Preprocess() {
    if (global_features_.size() != preprocess_scheme_global_features_mean_.size() || global_features_.size() != preprocess_scheme_global_features_stddev_.size())
        throw "[DNN Helper] [ERROR]: Size of global features does not match size of preprocessing scheme";

    for (unsigned int i = 0; i < object_features_.size(); i++) {
        if (object_features_[i].size() != preprocess_scheme_object_features_mean_.size() || object_features_[i].size() != preprocess_scheme_object_features_stddev_.size())
            throw "[DNN Helper] [ERROR]: Size of object features does not match size of preprocessing scheme";
    }

    for (unsigned int i = 0; i < global_features_.size(); i++) {
        global_features_[i] += -(preprocess_scheme_global_features_mean_[i]);
        global_features_[i] *= 1./preprocess_scheme_global_features_stddev_[i];
    }
    
    for (unsigned int i = 0; i < object_features_.size(); i++) {
        for (unsigned int j = 0; j < object_features_[i].size(); j++) {
            if (object_features_[i][j] == pad_value)
                continue;
            object_features_[i][j] += -(preprocess_scheme_object_features_mean_[j]);
            object_features_[i][j] *= 1./preprocess_scheme_object_features_stddev_[j];
        }
    } 
}

inline
void TTH_DNN_Helper::SetInputShapes(unsigned int length_global, unsigned int length_object, unsigned int length_object_sequence) {
    length_global_ = length_global;
    length_object_ = length_object;
    length_object_sequence_ = length_object_sequence;
}

inline
void TTH_DNN_Helper::SetInputs(std::vector<edm::Ptr<flashgg::Jet>> jets, std::vector<double> global_features) {
    if (global_features.size() != length_global_)
        throw "[DNN Helper] [ERROR]: Global features incorrectly configured!";
  
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

    Preprocess();

    inputs_set_ = true;
}

inline
void TTH_DNN_Helper::SetInputs(std::vector<edm::Ptr<flashgg::Jet>> jets, std::vector<edm::Ptr<flashgg::Muon>> muons, std::vector<edm::Ptr<flashgg::Electron>> electrons, std::vector<double> global_features) {
    if (global_features.size() != length_global_)
        throw "[DNN Helper] [ERROR]: Global features incorrectly configured!";

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
        if (debug_)
            cout << "Filling jet " << i << endl;
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
         if (debug_)
            cout << "Filling electron " << i << endl;
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
         if (debug_)
            cout << "Filling muon " << i << endl;
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

    if (debug_)
        cout << "Just finished filling objects, should have filled " << jets.size() + electrons.size() + muons.size() << " objects." << endl;

    // Third, order jets by pT
    std::vector<std::pair<unsigned int, double> > pt_ordering;

    for (unsigned int i = 0; i < object_features_unordered.size(); i++)
        pt_ordering.push_back(std::pair<unsigned int, double>(i, object_features_unordered[i][0]));

    std::sort(pt_ordering.begin(), pt_ordering.end(), sortByValue);
    for (unsigned int i = 0; i < std::min(object_features_.size(), pt_ordering.size()); i++) {
        object_features_[i] = object_features_unordered[pt_ordering[i].first];
    }

    Preprocess();

    inputs_set_ = true;
}

inline
float TTH_DNN_Helper::EvaluateDNN() {
    if (!inputs_set_)
        throw "[DNN Helper] [ERROR]: Inputs have not been reset since last evaluation!";

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
    inputs_set_ = false;

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

inline
void calculate_forward_jet_features(double &forward_jet_pt, double &forward_jet_eta, std::vector<edm::Ptr<flashgg::Jet>> jets, std::string btag_selector, double max1_btag) {
    forward_jet_pt = -1;
    forward_jet_eta = 0.0;

    for (unsigned int i = 0; i < jets.size(); i++) {
        if (jets[i]->bDiscriminator(btag_selector) >= max1_btag && jets.size() > 1)
            continue;

        if (abs(jets[i]->eta()) > forward_jet_eta) {
            forward_jet_pt = log(jets[i]->pt());
            forward_jet_eta = abs(jets[i]->eta());            
        }        
    }

    return;
}

inline
void calculate_lepton_charges(double &lep1_charge, double &lep2_charge, std::vector<edm::Ptr<flashgg::Muon>> muons, std::vector<edm::Ptr<flashgg::Electron>> electrons) {
    lep1_charge = 0;
    lep2_charge = 0;

    double lead_pt(0), sublead_pt(0);

    for (unsigned int i = 0; i < muons.size(); i++) {
        if (muons[i]->pt() > lead_pt) {
            lead_pt = muons[i]->pt();
            lep1_charge = muons[i]->charge();
        }
    }

    for (unsigned int i = 0; i < electrons.size(); i++) {
        if (electrons[i]->pt() > lead_pt) {
            lead_pt = electrons[i]->pt();
            lep1_charge = electrons[i]->charge();
        }
    }

    for (unsigned int i = 0; i < muons.size(); i++) {
        if (muons[i]->pt() < lead_pt && muons[i]->pt() > sublead_pt) {
            sublead_pt = muons[i]->pt();
            lep2_charge = muons[i]->charge();
        }
    }

    for (unsigned int i = 0; i < electrons.size(); i++) {
        if (electrons[i]->pt() < lead_pt && electrons[i]->pt() > sublead_pt) {
            sublead_pt = electrons[i]->pt();
            lep2_charge = electrons[i]->charge();
        }
    }
 
    return; 
}

}
#endif // _DNN_HELPER_H_
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
