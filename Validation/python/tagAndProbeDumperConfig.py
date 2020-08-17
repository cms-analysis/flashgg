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


def getConfig(extended=False, dumpShowerShapes=False, dumpClusterShapes=False, dumpPfIsos=False, dumpEgIsos=False, dumpTag=True, trackClIsoCorrections=False):

    minDiEleVars = ["mass := diPhoton.mass",
                    " pt := diPhoton.pt",
                    "rapidity := diPhoton.rapidity",
                    "eta := diPhoton.eta",
                    "vertexZ  := diPhoton.vtx.z"]

    extDiEleVars = ["vertexId := diPhoton.vtx.key",
                    "mass_5x5 := diPhoton.mass*sqrt(getTag.full5x5_e5x5*getProbe.full5x5_e5x5/(getTag.p4.energy*getProbe.p4.energy))",
                    "deltaEta := abs( getTag.eta - getProbe.eta )",
                    "cosDeltaPhi := cos( getTag.phi - getProbe.phi )",
                    "minR9 := min(getTag.full5x5_r9,getProbe.full5x5_r9)",
                    "maxEta := max(abs(getTag.superCluster.eta),abs(getProbe.superCluster.eta))"]

    minSingleEleVars = [("energyAtStep('initial')", "InitialEnergy"),
                        ("p4.energy", "Energy"),
                        ("superCluster.energy", "ScEnergy"),
                        ("pt", "Pt"),
                        ("eta", "Eta"),
                        ("superCluster.eta", "ScEta"),
                        ("phi", "Phi")]

    minSingleEleViewVars = [("phoIdMvaWrtChosenVtx", "PhoIdMVA")]

    extSingleEleVars = [("superCluster.clustersSize", "ScClustersSize"),
                        ("full5x5_e5x5", "5x5_Energy"),
                        ("hadTowOverEm", "HoE"),
                        ("hasPixelSeed", "PixSeed"),
                        ("passElectronVeto", "PassEleVeto"),
                        ("sigEOverE", "sigEOverE"),
                        ("checkStatusFlag('kSaturated')", "IsSat"),
                        ("checkStatusFlag('kWeird')", "IsWeird"),
                        ("esEffSigmaRR", "sigmaRR"),
                        ("hasUserCand('eleMatch')", "EleMatch")]

    singleEleShowerShapes = [("full5x5_sigmaIetaIeta", "SigmaIeIe"),
                             ("sipip", "CovarianceIpIp"),
                             ("sieip", "CovarianceIeIp"),
                             ("superCluster.etaWidth", "etaWidth_Sc"),
                             ("superCluster.phiWidth", "phiWidth_Sc"),
                             ("full5x5_r9", "R9"),
                             "s4"]

    singleEleClusterShapes = ["full5x5_e1x5",
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
                              "e1x3"]

    singleElePfIsos = [("pfPhoIso03", "PhoIso03"),
                       ("pfChgIsoWrtWorstVtx03", "ChIso03worst"),
                       ("pfChgIsoWrtChosenVtx03","ChIso03")]

    singleEleEgIsos = [("egChargedHadronIso", "ChIso"),
                       ("egPhotonIso", "PhoIso"),
                       ("egNeutralHadronIso", "NeutIso")]

    # singleEleViewPfIso = [("pfChIso03WrtChosenVtx", "ChIso03")]

    genVars = ["genMass := diPhoton.genP4.mass",
               "probeGenPt := ?getProbe.hasMatchedGenPhoton?getProbe.matchedGenPhoton.pt:0",
               "probeMatchType := getProbe.genMatchType",
               "probeGenIso := ?getProbe.hasUserFloat('genIso')?getProbe.userFloat('genIso'):0"]

    tagGenVars = ["tagGenPt := ?getTag.hasMatchedGenPhoton?getTag.matchedGenPhoton.pt:0",
                  "tagMatchType := getTag.genMatchType",
                  "tagGenIso := ?getTag.hasUserFloat('genIso')?getTag.userFloat('genIso'):0", ]

    probeUncorrClIsos = ["probeR9_uncorr                := ? getProbe.hasUserFloat('uncorr_r9') ? getProbe.userFloat('uncorr_r9') : -999.",
                         "probeEtaWidth_uncorr          := ? getProbe.hasUserFloat('uncorr_etaWidth') ? getProbe.userFloat('uncorr_etaWidth') : -999.",
                         "probeS4_uncorr                := ? getProbe.hasUserFloat('uncorr_s4') ? getProbe.userFloat('uncorr_s4') : -999.",
                         "probePhiWidth_uncorr          := ? getProbe.hasUserFloat('uncorr_phiWidth') ? getProbe.userFloat('uncorr_phiWidth') : -999.",
                         "probeSigmaIeIe_uncorr         := ? getProbe.hasUserFloat('uncorr_sieie') ? getProbe.userFloat('uncorr_sieie') : -999",
                         "probeCovarianceIeIp_uncorr    := ? getProbe.hasUserFloat('uncorr_sieip') ? getProbe.userFloat('uncorr_sieip') : -999",
                         "probePhoIso_uncorr            := ? getProbe.hasUserFloat('uncorr_pfPhoIso03') ? getProbe.userFloat('uncorr_pfPhoIso03') : -999",
                         "probeChIso03_uncorr           := ? getProbe.hasUserFloat('uncorr_pfChIso03') ? getProbe.userFloat('uncorr_pfChIso03') : -999",
                         "probeChIso03worst_uncorr      := ? getProbe.hasUserFloat('uncorr_pfChIsoWorst03') ? getProbe.userFloat('uncorr_pfChIsoWorst03') : -999",
                         "probePhoIdMVA_uncorr          := ? getProbe.hasUserFloat('original_phoId') ? getProbe.userFloat('original_phoId') : -999" ]

    probeCorrClVars = ["probePhiWidth := ? getProbe.hasUserFloat('phiWidth') ? getProbe.userFloat('phiWidth') : -999",
                       "probeEtaWidth := ? getProbe.hasUserFloat('etaWidth') ? getProbe.userFloat('etaWidth') : -999"]

    singleEleVars = minSingleEleVars
    singleEleViewVars = minSingleEleViewVars
    if extended:
        singleEleVars.extend(extSingleEleVars)
    if dumpShowerShapes:
        singleEleVars.extend(singleEleShowerShapes)
    if dumpClusterShapes:
        singleEleVars.extend(singleEleClusterShapes)
    if dumpPfIsos:
        singleEleVars.extend(singleElePfIsos)
        # singleEleViewVars.extend(singleEleViewPfIso)
    if dumpEgIsos:
        singleEleVars.extend(singleEleEgIsos)

    variables = minDiEleVars
    if extended:
        variables.extend(extDiEleVars)
    variables.extend(makeOneLegInputs("probe", "getProbe", singleEleVars))
    variables.extend(makeOneLegInputs(
        "probe", "getProbeView", singleEleViewVars))
    variables.extend(genVars)
    if dumpTag:
        variables.extend(makeOneLegInputs("tag", "getTag", singleEleVars))
        variables.extend(makeOneLegInputs(
            "tag", "getTagView", singleEleViewVars))
        variables.extend(tagGenVars)

    if trackClIsoCorrections:
        variables.extend(probeUncorrClIsos + probeCorrClVars)

    return variables


def getCustomConfig(key, dumpTag=True):

    if key == 'minimal':
        return getConfig(dumpTag=dumpTag)
    elif key == 'extended':
        return getConfig(extended=True, dumpTag=dumpTag)
    elif key == 'phoIDInputCorrections':
        return getConfig(extended=True, dumpShowerShapes=True, dumpPfIsos=True, dumpTag=dumpTag, trackClIsoCorrections=True)
    elif key == 'complete':
        return getConfig(extended=True, dumpShowerShapes=True, dumpClusterShapes=True, dumpPfIsos=True, dumpEgIsos=True, dumpTag=dumpTag, trackClIsoCorrections=True)
