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


def getDefaultConfig():

    diEleVars = ["mass := diPhoton.mass",
                 "pt := diPhoton.pt",
                 "rapidity := diPhoton.rapidity",
                 "eta := diPhoton.eta",
                 "vertexZ  := diPhoton.vtx.z",
                 "vertexId := diPhoton.vtx.key",
                 "genMass := diPhoton.genP4.mass",
                 "mass_5x5 := diPhoton.mass*sqrt(getTag.full5x5_e5x5*getProbe.full5x5_e5x5/(getTag.p4.energy*getProbe.p4.energy))",
                 "deltaEta := abs( getTag.eta - getProbe.eta )",
                 "codDeltaPhi := cos( getTag.phi - getProbe.phi )",
                 "minR9 := min(getTag.full5x5_r9 - getProbe.full5x5_r9)",
                 "maxEta := max(abs(getTag.superCluster.eta),abs(getProbe.superCluster.eta))"]

    singleEleVars = [("energyAtStep('initial')", "InitialEnergy"),
                     ("p4.energy", "Energy"),
                     ("superCluster.energy", "ScEnergy"),
                     ("superCluster.clustersSize", "ScClustersSize"),
                     ("full5x5_e5x5", "5x5_Energy"),
                     ("pt", "Pt"),
                     ("eta", "Eta"),
                     ("full_5x5_r9", "R9"),
                     ("superCluster.eta", "ScEta"),
                     ("phi", "Phi"),
                     ("pfPhoIso03", "PhoIso03"),
                     ("pfChgIsoWrtWorstVtx03", "ChIso03worst"),
                     ("egChargedHadronIso", "ChIso"),
                     ("egPhotonIso", "PhoIso"),
                     ("egNeutralHadronIso", "NeutIso"),
                     ("hadTowOverEm", "HoE"),
                     ("full5x5_sigmaIetaIeta", "SigmaIeIe"),
                     ("sipip", "CovarianceIpIp"),
                     ("sieip", "CovarianceIeIp"),
                     ("hasPixelSeed", "PixSeed"),
                     ("passElectronVeto", "PassEleVeto"),
                     ("sigEOverE", "sigEOverE"),
                     ("checkStatusFlag('kSaturated')", "IsSat"),
                     ("checkStatusFlag('kWeird')", "IsWeird"),
                     "full5x5_e1x5",
                     "full5x5_e2x5",
                     "full5x5_e3x3",
                     "full5x5_e5x5",
                     "full5x5_maxEnergyXtal",
                     "full5x5_sigmaIetaIeta",
                     "full5x5_r1x5",
                     "full5x5_r2x5",
                     "full5x5_r9",
                     "eMax", "e2nd", "eTop", "eBottom", "eLeft", "eRight",
                     "iEta", "iPhi", "cryEta", "cryPhi",
                     "e2x5right",
                     "e2x5left",
                     "e2x5top",
                     "e2x5bottom",
                     "e2x5max",
                     "e1x3",
                     "s4"]

    singleEleViewVars = [("pfChIso03WrtChosenVertex", "ChIso03")]

    genVars = ["tagGenPt := ?getTag.hasMatchedGenPhoton?getTag.matchedGenPhoton.pt:0",
               "tagMatchType := getTag.genMatchType",
               "tagGenIso := ?getTag.hasUserFloat('genIso')?getTag.userFloat('genIso'):0",
               "probeGenPt := ?getProbe.hasMatchedGenPhoton?getProbe.matchedGenPhoton.pt:0",
               "probeMatchType := getProbe.genMatchType",
               "probeGenIso := ?getProbe.hasUserFloat('genIso')?getProbe.userFloat('genIso'):0"]

    variables = diEleVars
    variables.extend(makeOneLegInputs("tag", "getTag", singleEleVars))
    variables.extend(makeOneLegInputs("probe", "getProbe", singleEleVars))
    variables.extend(makeOneLegInputs("tag", "getTagView", singleEleViewVars))
    variables.extend(makeOneLegInputs(
        "probe", "getProbeView", singleEleViewVars))
    variables.extend(genVars)

    return variables

    # if trackAllCorrections:
    #     variables += [
    #         #"leadRecoEreg              := leadingPhoton.userFloat('reco_regr_E')",
    #         #"leadRecoSigEOverE     := leadingPhoton.userFloat('reco_regr_E_err') / leadingPhoton.userFloat('reco_regr_E')",
    #         #"subleadRecoEreg           := subLeadingPhoton.userFloat('reco_regr_E')",
    #         #"subleadRecoSigEOverE  := subLeadingPhoton.userFloat('reco_regr_E_err') / subLeadingPhoton.userFloat('reco_regr_E')",

    #         #"leadAfterSSTrEreg              := leadingPhoton.userFloat('afterShShTransf_regr_E')",
    #         #"leadAfterSSTrSigEOverE     := leadingPhoton.userFloat('afterShShTransf_regr_E_err') / leadingPhoton.userFloat('afterShShTransf_regr_E')",
    #         #"subleadAfterSSTrEreg           := subLeadingPhoton.userFloat('afterShShTransf_regr_E')",
    #         #"subleadAfterSSTrSigEOverE  := subLeadingPhoton.userFloat('afterShShTransf_regr_E_err') / subLeadingPhoton.userFloat('afterShShTransf_regr_E')",

    #         #"leadUnsmearedSigmaEoE      := leadingPhoton.userFloat('unsmearedSigmaEoE')",
    #         #"subleadUnsmearedSigmaEoE      := subLeadingPhoton.userFloat('unsmearedSigmaEoE')",

    #         "leadUncorrR9              := ? leadingPhoton.hasUserFloat('uncorr_r9') ? leadingPhoton.userFloat('uncorr_r9') : -1.",
    #         "leadUncorrEtaWidth        := ? leadingPhoton.hasUserFloat('uncorr_etaWidth') ? leadingPhoton.userFloat('uncorr_etaWidth') : -1.",
    #         "leadUncorrS4              := ? leadingPhoton.hasUserFloat('uncorr_s4') ? leadingPhoton.userFloat('uncorr_s4') : -1.",

    #         "subleadUncorrR9              := ? subLeadingPhoton.hasUserFloat('uncorr_r9') ? subLeadingPhoton.userFloat('uncorr_r9') : -1.",
    #         "subleadUncorrEtaWidth        := ? subLeadingPhoton.hasUserFloat('uncorr_etaWidth') ? subLeadingPhoton.userFloat('uncorr_etaWidth') : -1.",
    #         "subleadUncorrS4              := ? subLeadingPhoton.hasUserFloat('uncorr_s4') ? subLeadingPhoton.userFloat('uncorr_s4') : -1.",

    #         ### "leadPhoIsoEA :=  map( abs(leadingPhoton.superCluster.eta) :: 0.,0.9,1.5,2.0,2.2,3. :: 0.21,0.2,0.14,0.22,0.31 )",
    #         ### "subleadPhoIsoEA :=  map( abs(subLeadingPhoton.superCluster.eta) :: 0.,0.9,1.5,2.0,2.2,3. :: 0.21,0.2,0.14,0.22,0.31 )",
    #     ]
