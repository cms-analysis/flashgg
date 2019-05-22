import re


def makeOneLegInputs(label, obj, inputs):
    out = []
    for inp in inputs:
        if type(inp) == str:
            name, expr = inp, inp
        else:
            expr, name = inp
        if not name.startswith("_"):
            name = name[0].capitalize()+name[1:]
        name = "%s%s" % (label, name)
        expr = "%s.%s" % (obj,   expr)
        ## print expr[14]
        out.append("%s := %s" % (name, expr))
        print out[-1]
    return out


def addRegressionInput(variables):
    regInputs = [
        ("superCluster.clustersSize", "scClustersSize"),
        ("superCluster.energy", "scEnergy"),

        # cluster shapes
        "full5x5_e1x5",             # "e1x5",
        "full5x5_e2x5",             # "e2x5",
        "full5x5_e3x3",             # "e3x3",
        "full5x5_e5x5",             # "e5x5",
        "full5x5_maxEnergyXtal",    # "maxEnergyXtal",
        "full5x5_sigmaIetaIeta",    # "sigmaIetaIeta",
        "full5x5_r1x5",             # "r1x5",
        "full5x5_r2x5",             # "r2x5",
        "full5x5_r9",               # "r9",
        "eMax", "e2nd", "eTop", "eBottom", "eLeft", "eRight",
        "iEta", "iPhi", "cryEta", "cryPhi",

        # more cluster shapes
        "e2x5right",  # ("e2x5right" ,"e2x5Right"  ),
        "e2x5left",  # ("e2x5left"  ,"e2x5Left"   ),
        "e2x5top",  # ("e2x5top"   ,"e2x5Top"    ),
        "e2x5bottom",  # ("e2x5bottom","e2x5Bottom" ),
        "e2x5max",  # ("e2x5max"   ,"e2x5Max"    ),
        "e1x3",  # ("e1x3"      ,"e1x3"       ),
        "s4",  # ("s4"        ,"s4"         ),

        ("esEffSigmaRR", "sigmaRR"),
        ("superCluster.etaWidth", "etaWidth"), ("superCluster.phiWidth", "phiWidth"),

        ("checkStatusFlag('kSaturated')",
         "kSaturated"), ("checkStatusFlag('kWeird')", "kWeird"),
    ]

    variables.extend(makeOneLegInputs("lead", "leadingPhoton", regInputs))
    variables.extend(makeOneLegInputs(
        "sublead", "subLeadingPhoton", regInputs))


def getDefaultConfig(trackAllCorrections=True):

    variables = ["mass", "pt", "rapidity", "eta",
                 "vertexZ  := vtx.z",
                 "vertexId := vtx.key",
                 "genMass := genP4.mass",

                 "leadInitialEnergy := leadingPhoton.energyAtStep('initial')",
                 "subLeadInitialEnergy := subLeadingPhoton.energyAtStep('initial')",
                 "leadEnergy := leadingPhoton.p4.energy",
                 "subLeadEnergy := subLeadingPhoton.p4.energy",

                 "lead_5x5_Energy := leadingPhoton.full5x5_e5x5",
                 "subLead_5x5_Energy := subLeadingPhoton.full5x5_e5x5",
                 "mass_5x5 := mass*sqrt(leadingPhoton.full5x5_e5x5*subLeadingPhoton.full5x5_e5x5/(leadingPhoton.p4.energy*subLeadingPhoton.p4.energy))",
                 "leadIsSat := leadingPhoton.checkStatusFlag('kSaturated')",
                 "subLeadIsSat := subLeadingPhoton.checkStatusFlag('kSaturated')",
                 "leadIsWeird := leadingPhoton.checkStatusFlag('kWeird')",
                 "subLeadIsWeird := subLeadingPhoton.checkStatusFlag('kWeird')",
                 "genLeadPt := ?leadingPhoton.hasMatchedGenPhoton?leadingPhoton.matchedGenPhoton.pt:0",
                 "genSubLeadPt := ?subLeadingPhoton.hasMatchedGenPhoton?subLeadingPhoton.matchedGenPhoton.pt:0",
                 "deltaEta                 := abs( leadingPhoton.eta - subLeadingPhoton.eta )",
                 "cosDeltaPhi              := cos( leadingPhoton.phi - subLeadingPhoton.phi )",
                 "leadPt                   :=leadingPhoton.pt",
                 "subleadPt                :=subLeadingPhoton.pt",
                 "leadEta                  :=leadingPhoton.eta",
                 "subleadEta               :=subLeadingPhoton.eta",
                 "leadR9                   :=leadingPhoton.full5x5_r9",
                 "subleadR9                :=subLeadingPhoton.full5x5_r9",
                 "leadScEta                :=leadingPhoton.superCluster.eta",
                 "subleadScEta             :=subLeadingPhoton.superCluster.eta",
                 "leadPhi                  :=leadingPhoton.phi",
                 "subleadPhi               :=subLeadingPhoton.phi",
                 "minR9                    :=min(leadingPhoton.full5x5_r9,subLeadingPhoton.full5x5_r9)",
                 "maxEta                   :=max(abs(leadingPhoton.superCluster.eta),abs(subLeadingPhoton.superCluster.eta))",

                 "leadChIso03   := leadingView.pfChIso03WrtChosenVtx",
                 "leadPhoIso03  := leadingPhoton.pfPhoIso03",
                 "leadChIso03worst :=  leadingPhoton.pfChgIsoWrtWorstVtx03",

                 "leadPhoIDMVA      := leadingView.phoIdMvaWrtChosenVtx",
                 "subLeadPhoIDMVA   := subLeadingView.phoIdMvaWrtChosenVtx",


                 "leadMatchType            :=leadingPhoton.genMatchType",
                 "leadGenIso               :=?leadingPhoton.hasUserFloat('genIso')?leadingPhoton.userFloat('genIso'):0",
                 "subleadMatchType         :=subLeadingPhoton.genMatchType",
                 "subleadGenIso            :=?subLeadingPhoton.hasUserFloat('genIso')?subLeadingPhoton.userFloat('genIso'):0",

                 "leadChIso   := leadingPhoton.egChargedHadronIso",
                 "leadPhoIso  := leadingPhoton.egPhotonIso",
                 "leadNeutIso := leadingPhoton.egNeutralHadronIso",
                 "leadHoE     := leadingPhoton.hadTowOverEm",
                 "leadSigmaIeIe := leadingPhoton.full5x5_sigmaIetaIeta",
                 "leadCovarianceIpIp := leadingPhoton.sipip",
                 "leadCovarianceIeIp := leadingPhoton.sieip",
                 "leadPixSeed  := leadingPhoton.hasPixelSeed",
                 "leadPassEleVeto := leadingPhoton.passElectronVeto",


                 ## "subleadBlockChIso   := subLeadingView.pfChIso03WrtChosenVtx",
                 ## "subleadBlockPhoIso  := subLeadingPhoton.pfPhoIso03",
                 ## "subleadRndConePhoIso:= subLeadingPhoton.extraPhoIso('rnd03')",

                 "subleadChIso03   := subLeadingView.pfChIso03WrtChosenVtx",
                 "subleadPhoIso03  := subLeadingPhoton.pfPhoIso03",
                 "subleadChIso03worst :=  subLeadingPhoton.pfChgIsoWrtWorstVtx03",

                 "subleadChIso   := subLeadingPhoton.egChargedHadronIso",
                 "subleadPhoIso  := subLeadingPhoton.egPhotonIso",
                 "subleadNeutIso := subLeadingPhoton.egNeutralHadronIso",
                 "subleadHoE     := subLeadingPhoton.hadTowOverEm",
                 "subleadSigmaIeIe := subLeadingPhoton.full5x5_sigmaIetaIeta",
                 "subleadCovarianceIeIp := subLeadingPhoton.sieip",
                 "subleadCovarianceIpIp := subLeadingPhoton.sipip",
                 "subleadPixSeed := subLeadingPhoton.hasPixelSeed",
                 "subleadPassEleVeto := subLeadingPhoton.passElectronVeto",

                 "leadSigEOverE    := leadingPhoton.sigEOverE",
                 "subleadSigEOverE := subLeadingPhoton.sigEOverE"]
    if trackAllCorrections:
        variables += [
            #"leadRecoEreg              := leadingPhoton.userFloat('reco_regr_E')",
            #"leadRecoSigEOverE     := leadingPhoton.userFloat('reco_regr_E_err') / leadingPhoton.userFloat('reco_regr_E')",
            #"subleadRecoEreg           := subLeadingPhoton.userFloat('reco_regr_E')",
            #"subleadRecoSigEOverE  := subLeadingPhoton.userFloat('reco_regr_E_err') / subLeadingPhoton.userFloat('reco_regr_E')",

            #"leadAfterSSTrEreg              := leadingPhoton.userFloat('afterShShTransf_regr_E')",
            #"leadAfterSSTrSigEOverE     := leadingPhoton.userFloat('afterShShTransf_regr_E_err') / leadingPhoton.userFloat('afterShShTransf_regr_E')",
            #"subleadAfterSSTrEreg           := subLeadingPhoton.userFloat('afterShShTransf_regr_E')",
            #"subleadAfterSSTrSigEOverE  := subLeadingPhoton.userFloat('afterShShTransf_regr_E_err') / subLeadingPhoton.userFloat('afterShShTransf_regr_E')",

            #"leadUnsmearedSigmaEoE      := leadingPhoton.userFloat('unsmearedSigmaEoE')",
            #"subleadUnsmearedSigmaEoE      := subLeadingPhoton.userFloat('unsmearedSigmaEoE')",

            "leadUncorrR9              := ? leadingPhoton.hasUserFloat('uncorr_r9') ? leadingPhoton.userFloat('uncorr_r9') : -1.",
            "leadUncorrEtaWidth        := ? leadingPhoton.hasUserFloat('uncorr_etaWidth') ? leadingPhoton.userFloat('uncorr_etaWidth') : -1.",
            "leadUncorrS4              := ? leadingPhoton.hasUserFloat('uncorr_s4') ? leadingPhoton.userFloat('uncorr_s4') : -1.",

            "subleadUncorrR9              := ? subLeadingPhoton.hasUserFloat('uncorr_r9') ? subLeadingPhoton.userFloat('uncorr_r9') : -1.",
            "subleadUncorrEtaWidth        := ? subLeadingPhoton.hasUserFloat('uncorr_etaWidth') ? subLeadingPhoton.userFloat('uncorr_etaWidth') : -1.",
            "subleadUncorrS4              := ? subLeadingPhoton.hasUserFloat('uncorr_s4') ? subLeadingPhoton.userFloat('uncorr_s4') : -1.",

            ### "leadPhoIsoEA :=  map( abs(leadingPhoton.superCluster.eta) :: 0.,0.9,1.5,2.0,2.2,3. :: 0.21,0.2,0.14,0.22,0.31 )",
            ### "subleadPhoIsoEA :=  map( abs(subLeadingPhoton.superCluster.eta) :: 0.,0.9,1.5,2.0,2.2,3. :: 0.21,0.2,0.14,0.22,0.31 )",
        ]

    return variables


def getTnPVariables(variables):
    """
    Converted default variables into TnP adding Id selection flags
    """

    dipho_to_tnp_var = {"mass": "mass := diPhoton.mass",
                        "pt": "pt := diPhoton.pt",
                        "rapidity": "rapidity := diPhoton.rapidity",
                        "eta": "eta := diPhoton.eta",
                        "vertexZ  := vtx.z": "vertexZ  := diPhoton.vtx.z",
                        "vertexId := vtx.key": "vertexId := diPhoton.vtx.key",
                        "genMass := genP4.mass": "genMass := diPhoton.genP4.mass"
                        }

    tnp_variables = []
    for var in variables:
        if var in dipho_to_tnp_var.keys():
            var = dipho_to_tnp_var[var]

        tnp_var = var
        tnp_var = tnp_var.replace("subLeadingPhoton", "getProbe")
        tnp_var = tnp_var.replace("subLeadingView", "getProbeView")
        tnp_var = tnp_var.replace("leadingPhoton", "getTag")
        tnp_var = tnp_var.replace("leadingView", "getTagView")
        tnp_var = re.sub("[Ss]ub[lL]ead", "probe", tnp_var)
        tnp_var = tnp_var.replace("lead", "tag")
        tnp_var = tnp_var.replace("Lead", "tag")
        tnp_variables.append(tnp_var)

    return tnp_variables
