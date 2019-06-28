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
        # print expr[14]
        out.append("%s := %s" % (name, expr))
        print out[-1]
    return out


def getDefaultConfig(trackClIsoCorrections=True):

    diEleVars = ["mass := diPhoton.mass",
                 "pt := diPhoton.pt",
                 "rapidity := diPhoton.rapidity",
                 "eta := diPhoton.eta",
                 "vertexZ  := diPhoton.vtx.z",
                 "vertexId := diPhoton.vtx.key",
                 "genMass := diPhoton.genP4.mass",
                 "mass_5x5 := diPhoton.mass*sqrt(getTag.full5x5_e5x5*getProbe.full5x5_e5x5/(getTag.p4.energy*getProbe.p4.energy))",
                 "deltaEta := abs( getTag.eta - getProbe.eta )",
                 "cosDeltaPhi := cos( getTag.phi - getProbe.phi )",
                 "minR9 := min(getTag.full5x5_r9,getProbe.full5x5_r9)",
                 "maxEta := max(abs(getTag.superCluster.eta),abs(getProbe.superCluster.eta))"]

    singleEleVars = [("energyAtStep('initial')", "InitialEnergy"),
                     ("p4.energy", "Energy"),
                     ("superCluster.energy", "ScEnergy"),
                     ("superCluster.clustersSize", "ScClustersSize"),
                     ("full5x5_e5x5", "5x5_Energy"),
                     ("pt", "Pt"),
                     ("eta", "Eta"),
                     ("full5x5_r9", "R9"),
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
                     ("esEffSigmaRR", "sigmaRR"),
                     ("superCluster.etaWidth", "etaWidth"),
                     ("superCluster.phiWidth", "phiWidth"),
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

    singleEleViewVars = [("pfChIso03WrtChosenVtx", "ChIso03")]

    genVars = ["genMass := diPhoton.genP4.mass",
               "tagGenPt := ?getTag.hasMatchedGenPhoton?getTag.matchedGenPhoton.pt:0",
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

    if trackClIsoCorrections:
        variables += ["probeUncorrR9              := ? getProbe.hasUserFloat('uncorr_r9') ? getProbe.userFloat('uncorr_r9') : -999.",
                      "probeUncorrEtaWidth        := ? getProbe.hasUserFloat('uncorr_etaWidth') ? getProbe.userFloat('uncorr_etaWidth') : -999.",
                      "probeUncorrS4              := ? getProbe.hasUserFloat('uncorr_s4') ? getProbe.userFloat('uncorr_s4') : -999.",
                      "probeUncorrPhiWidth        := ? getProbe.hasUserFloat('uncorr_phiWidth') ? getProbe.userFloat('uncorr_phiWidth') : -999.",
                      "probeUncorrSigmaIeIe       := ? getProbe.hasUserFloat('uncorr_sieie') ? getProbe.userFloat('uncorr_sieie') : -999",
                      "probeUncorrCovarianceIeIp  := ? getProbe.hasUserFloat('uncorr_sieip') ? getProbe.userFloat('uncorr_sieip') : -999",
                      "probeUncorrPhoIso          := ? getProbe.hasUserFloat('uncorr_phoIso') ? getProbe.userFloat('uncorr_phoIso') : -999",
                      "probeUncorrChIso           := ? getProbe.hasUserFloat('uncorr_chIso') ? getProbe.userFloat('uncorr_chIso') : -999",
                      "probeUncorrChIsoWorst      := ? getProbe.hasUserFloat('uncorr_chIsoWorst') ? getProbe.userFloat('uncorr_chIsoWorst') : -999"]

    return variables
