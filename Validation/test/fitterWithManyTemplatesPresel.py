import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing
import flashgg.Validation.commonFitPreselEB as common
#import flashgg.Validation.commonFitPreselEE as common
#import flashgg.Validation.commonFitPreselBkgSystEE as common
#import flashgg.Validation.parametricTemplates as common
#import flashgg.Validation.commonFitPreselBkgSystEB as common

options = VarParsing('analysis')
options.register(
    "isMC",
    True,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Compute efficiency for MC"
    )

options.register(
    "inputFileName",
    "TnP_mc_v2_norm.root",
    VarParsing.multiplicity.singleton,
    VarParsing.varType.string,
    "Input filename"
    )

options.register(
    "outputFileName",
    "",
    VarParsing.multiplicity.singleton,
    VarParsing.varType.string,
    "Output filename"
    )

options.register(
    "idName",
    "passingPresel",
    VarParsing.multiplicity.singleton,
    VarParsing.varType.string,
    "ID variable name as in the fitter_tree"
    )

options.register(
    "dirName",
    "PhotonToRECO",
    VarParsing.multiplicity.singleton,
    VarParsing.varType.string,
    "Folder name containing the fitter_tree"
    )

options.register(
    "doCutAndCount",
    False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Do not compute fitting, just cut and count"
    )
options.parseArguments()

process = cms.Process("TagProbe")
process.source = cms.Source("EmptySource")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

################################################

InputFileName = options.inputFileName
OutputFile = "efficiency-mc-"+options.idName
if (not options.isMC):
    OutputFile = "efficiency-data-"+options.idName

if (options.outputFileName != ""):
    OutputFile = OutputFile+"-"+options.outputFileName+".root"
else:
    OutputFile = OutputFile+".root"

################################################

#specifies the binning of parameters
EfficiencyBins = cms.PSet(
    #probe_Pho_et = cms.vdouble(20. ,40. ,60. ,100.),
    probe_Pho_r9 = cms.vdouble(0. ,.85 ,1.),
    probe_sc_abseta = cms.vdouble(0.0, 1.479),
    #probe_Pho_r9 = cms.vdouble(0. ,.90 ,1.),
    #probe_sc_abseta = cms.vdouble(1.479, 2.5),
    )

DataBinningSpecification = cms.PSet(
    UnbinnedVariables = cms.vstring("mass"),
    BinnedVariables = cms.PSet(EfficiencyBins),
    BinToPDFmap = cms.vstring(
        "tight_20p0To40p0_0p0To1p5", 
        "*r9_bin0*abseta_bin0*","passingPresel_0p0To0p85_0p0To1p479",
        "*r9_bin1*abseta_bin0*","passingPresel_0p85To1p0_0p0To1p479",
        #"*r9_bin0*abseta_bin0*","passingPresel_0p0To0p9_1p479To2p5",
        #"*r9_bin1*abseta_bin0*","passingPresel_0p9To1p0_1p479To2p5",
        )
    )

McBinningSpecification = cms.PSet(
    UnbinnedVariables = cms.vstring("mass", "totWeight"),
    BinnedVariables = cms.PSet(EfficiencyBins, mcTrue = cms.vstring("true")),
    BinToPDFmap = cms.vstring(
        "tight_20p0To40p0_0p0To1p5", 
        "*et_bin0*eta_bin0*","tight_20p0To40p0_0p0To1p5",
        "*et_bin1*eta_bin0*","tight_40p0To60p0_0p0To1p5",
        "*et_bin2*eta_bin0*","tight_60p0To100p0_0p0To1p5",
        "*et_bin0*eta_bin1*","tight_20p0To40p0_1p5To2p5",
        "*et_bin1*eta_bin1*","tight_40p0To60p0_1p5To2p5",
        "*et_bin2*eta_bin1*","tight_60p0To100p0_1p5To2p5",
        )
)

########################

process.TnPMeasurement = cms.EDAnalyzer("TagProbeFitTreeAnalyzer",
                                        InputFileNames = cms.vstring(InputFileName),
                                        InputDirectoryName = cms.string(options.dirName),
                                        InputTreeName = cms.string("fitter_tree"), 
                                        OutputFileName = cms.string(OutputFile),
                                        NumCPU = cms.uint32(1),
                                        SaveWorkspace = cms.bool(False), #VERY TIME CONSUMING FOR MC
                                        doCutAndCount = cms.bool(options.doCutAndCount),
                                        floatShapeParameters = cms.bool(True),
                                        #fixVars = cms.vstring("peakFail", "peakPass"),
                                        binnedFit = cms.bool(True),
                                        binsForFit = cms.uint32(60),
                                        WeightVariable = cms.string("totWeight"),
                                        # defines all the real variables of the probes available in the input tree and intended for use in the efficiencies
                                        Variables = cms.PSet(
        mass = cms.vstring("Tag-Probe Mass", "60.0", "120.0", "GeV/c^{2}"),
        tag_Pho_et = cms.vstring("Probe E_{T}", "30", "1000", "GeV/c"),
        probe_Pho_et = cms.vstring("Probe E_{T}", "20", "1000", "GeV/c"),
        probe_sc_abseta = cms.vstring("Probe #eta", "0", "2.5", ""), 
        probe_Pho_r9 = cms.vstring("Probe #eta", "0", "1", ""), 
        totWeight = cms.vstring("totWeight", "-1000000", "100000000", ""), 
        #passingPresel = cms.vstring("passingPresel", "1", "1", "")
        #Ele_dRTau = cms.vstring("Ele_dRTau", "0.2", "100000", ""),
        #probe_dRTau = cms.vstring("probe_dRTau", "0.2", "100000", ""),
        ),
                                        
                                        # defines all the discrete variables of the probes available in the input tree and intended for use in the efficiency calculation
                                        Categories = cms.PSet(),
                                        PDFs = common.all_pdfs,
                                        Efficiencies = cms.PSet()
                                        )

setattr(process.TnPMeasurement.Categories, options.idName, cms.vstring(options.idName, "dummy[pass=1,fail=0]"))
setattr(process.TnPMeasurement.Categories, "mcTrue", cms.vstring("MC true", "dummy[true=1,false=0]"))

if (not options.isMC):
    delattr(process.TnPMeasurement, "WeightVariable")
    process.TnPMeasurement.Variables = cms.PSet(
        mass = cms.vstring("Tag-Probe Mass", "60.0", "120.0", "GeV/c^{2}"),
        probe_Pho_pt = cms.vstring("Probe E_{T}", "20", "1000", "GeV/c"),
        tag_Pho_pt = cms.vstring("Probe E_{T}", "30", "1000", "GeV/c"),
        probe_sc_abseta = cms.vstring("Probe #eta", "0", "2.5", ""),  
        probe_Pho_r9 = cms.vstring("Probe #eta", "0", "1", ""), 
        #passingPresel = cms.vstring("passingPresel", "1", "1", "")
        )
    for pdf in process.TnPMeasurement.PDFs.__dict__:
        param =  process.TnPMeasurement.PDFs.getParameter(pdf)
        if (type(param) is not cms.vstring):
            continue
        for i, l in enumerate(getattr(process.TnPMeasurement.PDFs, pdf)):
            if l.find("signalFractionInPassing") != -1:
                getattr(process.TnPMeasurement.PDFs, pdf)[i] = l.replace("[1.0]","[0.5,0.,1.]")

    setattr(process.TnPMeasurement.Efficiencies, options.idName, DataBinningSpecification)    
    setattr(getattr(process.TnPMeasurement.Efficiencies, options.idName) , "EfficiencyCategoryAndState", cms.vstring(options.idName, "pass"))
else:   
    setattr(process.TnPMeasurement.Efficiencies, "MCtruth_" + options.idName, McBinningSpecification)    
    setattr(getattr(process.TnPMeasurement.Efficiencies, "MCtruth_" + options.idName), "EfficiencyCategoryAndState", cms.vstring(options.idName, "pass"))

    for pdf in process.TnPMeasurement.PDFs.__dict__:
        param =  process.TnPMeasurement.PDFs.getParameter(pdf)
        if (type(param) is not cms.vstring):
            continue
        for i, l in enumerate(getattr(process.TnPMeasurement.PDFs, pdf)):
            if l.find("backgroundPass") != -1:
                getattr(process.TnPMeasurement.PDFs, pdf)[i] = "RooPolynomial::backgroundPass(mass, a[0.0])"
            if l.find("backgroundFail") != -1:
                getattr(process.TnPMeasurement.PDFs, pdf)[i] = "RooPolynomial::backgroundFail(mass, a[0.0])"

process.fit = cms.Path(
    process.TnPMeasurement  
    )


