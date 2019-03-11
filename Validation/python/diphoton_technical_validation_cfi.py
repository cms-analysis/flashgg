import FWCore.ParameterSet.Config as cms

from flashgg.Taggers.diphotonDumper_cfi import diphotonDumper
import flashgg.Taggers.dumperConfigTools as cfgTools

diphotonDumper.dumpTrees = True
diphotonDumper.dumpWorkspace = False
diphotonDumper.quietRooFit = True
diphotonDumper.maxCandPerEvent = 1
diphotonDumper.nameTemplate = "validation"
diphotonDumper.throwOnUnclassified = cms.bool(False)

validation_variables = [
    #---diphoton variables
    "mass",
    "pt",
    "rapidity",
    "eta",
    "logSumPt2                  := logSumPt2()",
    "ptBal                      := ptBal()",
    "ptAsym                     := ptAsym()",
    "nConv                      := nConv()",
    "pullConv                   := pullConv()",
    "nVert                      := nVert()",
    "mva0                       := mva0()",
    "mva1                       := mva1()",
    "mva2                       := mva2()",
    "dZ1                        := dZ1()",
    "dZ2                        := dZ2()",
    "vtxProbMVA                 := vtxProbMVA()",
    "sumPt                      := sumPt()",
    "vertexIndex                := vertexIndex()",
    "nVtxInfoSize               := nVtxInfoSize()",
    "vtx_                       := vtx().z()",
    "vtx_key                    := vtx.key",
    "genMass                    := genP4.mass",
    "mass_5x5                   := mass*sqrt(leadingPhoton.full5x5_e5x5*subLeadingPhoton.full5x5_e5x5/(leadingPhoton.p4.energy*subLeadingPhoton.p4.energy))",
    "deltaEta                   := abs( leadingPhoton.eta - subLeadingPhoton.eta )",
    "cosDeltaPhi                := cos( leadingPhoton.phi - subLeadingPhoton.phi )",
    "minR9                      := min(leadingPhoton.full5x5_r9,subLeadingPhoton.full5x5_r9)",
    "maxEta                     := max(abs(leadingPhoton.superCluster.eta),abs(subLeadingPhoton.superCluster.eta))",

    #---leading photon variables
    "lead_gen_pt                := ?leadingPhoton.hasMatchedGenPhoton?leadingPhoton.matchedGenPhoton.pt:0",
    "lead_kSaturated            := leadingPhoton.checkStatusFlag('kSaturated')",
    "lead_kWeird                := leadingPhoton.checkStatusFlag('kWeird')",    
    "lead_pt                    := leadingPhoton.pt",
    "lead_eta                   := leadingPhoton.eta",
    "lead_full5x5_r9            := leadingPhoton.full5x5_r9",
    "lead_initial_energy        := leadingPhoton.energyAtStep('initial')",    
    "lead_superCluster_eta      := leadingPhoton.superCluster.eta",
    "lead_phi                   := leadingPhoton.phi",
    "lead_energy                := leadingPhoton.p4.energy",
    "lead_5x5_Energy            := leadingPhoton.full5x5_e5x5",
    "lead_pfChIso03WrtChosenVtx := leadingView.pfChIso03WrtChosenVtx", 
    "lead_pfPhoIso03            := leadingPhoton.pfPhoIso03", 
    "lead_pfChgIsoWrtWorstVtx03 := leadingPhoton.pfChgIsoWrtWorstVtx03",    
    "lead_phoIdMvaWrtChosenVtx  := leadingView.phoIdMvaWrtChosenVtx",
    "lead_genMatchType          := leadingPhoton.genMatchType",
    "lead_genIso                := ?leadingPhoton.hasUserFloat('genIso')?leadingPhoton.userFloat('genIso'):0",
    "lead_egChargedHadronIso    := leadingPhoton.egChargedHadronIso", 
    "lead_egPhotonIso           := leadingPhoton.egPhotonIso", 
    "lead_egNeutralHadronIso    := leadingPhoton.egNeutralHadronIso",
    "lead_hadTowOverEm          := leadingPhoton.hadTowOverEm",
    "lead_full5x5_sigmaIetaIeta := leadingPhoton.full5x5_sigmaIetaIeta",
    "lead_hasPixelSeed          := leadingPhoton.hasPixelSeed",
    "lead_passElectronVeto      := leadingPhoton.passElectronVeto",
    "lead_sigEOverE             := leadingPhoton.sigEOverE",
    # "lead_unsmearedSigmaEoE     := leadingPhoton.userFloat('unsmearedSigmaEoE')",
    # "lead_uncorr_r9             := ? leadingPhoton.hasUserFloat('uncorr_r9') ? leadingPhoton.userFloat('uncorr_r9') : -1.",
    # "lead_uncorr_etaWidth       := ? leadingPhoton.hasUserFloat('uncorr_etaWidth') ? leadingPhoton.userFloat('uncorr_etaWidth') : -1.",
    # "lead_uncorr_s4             := ? leadingPhoton.hasUserFloat('uncorr_s4') ? leadingPhoton.userFloat('uncorr_s4') : -1.",
    "lead_PhotonId              := leadPhotonId()",    

    #---subleading photon variables
    "sublead_gen_pt                := ?subLeadingPhoton.hasMatchedGenPhoton?subLeadingPhoton.matchedGenPhoton.pt:0",
    "sublead_kSaturated            := subLeadingPhoton.checkStatusFlag('kSaturated')",
    "sublead_kWeird                := subLeadingPhoton.checkStatusFlag('kWeird')",    
    "sublead_pt                    := subLeadingPhoton.pt",
    "sublead_eta                   := subLeadingPhoton.eta",
    "sublead_full5x5_r9            := subLeadingPhoton.full5x5_r9",
    "sublead_initial_energy        := subLeadingPhoton.energyAtStep('initial')",    
    "sublead_superCluster_eta      := subLeadingPhoton.superCluster.eta",
    "sublead_phi                   := subLeadingPhoton.phi",
    "sublead_energy                := subLeadingPhoton.p4.energy",
    "sublead_5x5_Energy            := subLeadingPhoton.full5x5_e5x5",
    "sublead_pfChIso03WrtChosenVtx := subLeadingView.pfChIso03WrtChosenVtx", 
    "sublead_pfPhoIso03            := subLeadingPhoton.pfPhoIso03", 
    "sublead_pfChgIsoWrtWorstVtx03 := subLeadingPhoton.pfChgIsoWrtWorstVtx03",    
    "sublead_phoIdMvaWrtChosenVtx  := subLeadingView.phoIdMvaWrtChosenVtx",
    "sublead_genMatchType          := subLeadingPhoton.genMatchType",
    "sublead_genIso                := ?subLeadingPhoton.hasUserFloat('genIso')?subLeadingPhoton.userFloat('genIso'):0",
    "sublead_egChargedHadronIso    := subLeadingPhoton.egChargedHadronIso", 
    "sublead_egPhotonIso           := subLeadingPhoton.egPhotonIso", 
    "sublead_egNeutralHadronIso    := subLeadingPhoton.egNeutralHadronIso",
    "sublead_hadTowOverEm          := subLeadingPhoton.hadTowOverEm",
    "sublead_full5x5_sigmaIetaIeta := subLeadingPhoton.full5x5_sigmaIetaIeta",
    "sublead_hasPixelSeed          := subLeadingPhoton.hasPixelSeed",
    "sublead_passElectronVeto      := subLeadingPhoton.passElectronVeto",
    "sublead_sigEOverE             := subLeadingPhoton.sigEOverE",
    # "sublead_unsmearedSigmaEoE     := subLeadingPhoton.userFloat('unsmearedSigmaEoE')",
    # "sublead_uncorr_r9             := ? subLeadingPhoton.hasUserFloat('uncorr_r9') ? subLeadingPhoton.userFloat('uncorr_r9') : -1.",
    # "sublead_uncorr_etaWidth       := ? subLeadingPhoton.hasUserFloat('uncorr_etaWidth') ? subLeadingPhoton.userFloat('uncorr_etaWidth') : -1.",
    # "sublead_uncorr_s4             := ? subLeadingPhoton.hasUserFloat('uncorr_s4') ? subLeadingPhoton.userFloat('uncorr_s4') : -1.",
    "sublead_PhotonId              := subLeadPhotonId()"
]

cfgTools.addCategory(diphotonDumper,
                     "all",
                     "1",
                     variables=validation_variables,
)
