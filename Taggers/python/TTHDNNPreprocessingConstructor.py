import FWCore.ParameterSet.Config as cms
import json

def construct(file, channel, ttH_vs_tH = False):
    preprocess_list = {}
    
    with open(file, "r") as f_in:
        metadata = json.load(f_in)
    
    scheme = metadata["preprocess_scheme"]
    
    global_features_order = ["lead_eta_", "sublead_eta_", "lead_phi_", "sublead_phi_", "leadptoM_", "subleadptoM_", "maxIDMVA_", "minIDMVA_", "log_met_", "met_phi_", "leadPSV_", "subleadPSV_", "dipho_rapidity_", "dipho_pt_over_mass_", "dipho_delta_R", "max1_btag_", "max2_btag_", "njets_"]

    if channel == "Leptonic":
        global_features_order += ["n_lep_tight_"]

    if ttH_vs_tH:
        global_features_order += ["lep1_charge_", "lep2_charge_", "forward_jet_eta_", "forward_jet_pt_"]

    global_features_mean = [metadata["preprocess_scheme"][feature]["mean"] for feature in global_features_order]
    global_features_std_dev = [metadata["preprocess_scheme"][feature]["std_dev"] for feature in global_features_order]

    object_features = 8 if channel == "Hadronic" else 9

    object_features_mean = [metadata["preprocess_scheme"]["objects_%d" % i]["mean"] for i in range(object_features)]
    object_features_std_dev = [metadata["preprocess_scheme"]["objects_%d" % i]["std_dev"] for i in range(object_features)]

    preprocess_list["global_mean"] = cms.vdouble(global_features_mean)
    preprocess_list["global_stddev"] = cms.vdouble(global_features_std_dev)
    preprocess_list["object_mean"] = cms.vdouble(object_features_mean)
    preprocess_list["object_stddev"] = cms.vdouble(object_features_std_dev)

    return preprocess_list
