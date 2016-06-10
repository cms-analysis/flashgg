import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing
#import flashgg.Validation.parametricTemplates as common
import sys

options = VarParsing('analysis')

options.register(
    "isMC",
    False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Compute MC efficiencies"
    )

options.register(
    "inputFileName",
    "TnP_data_Golden2015.root",
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
    "Perform cut and count efficiency measurement"
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

EfficiencyBins = cms.PSet(
    #probe_Pho_et = cms.vdouble( 30,90,300 ),
    probe_Pho_et = cms.vdouble( 30., 33.3333, 35., 40., 45., 50., 60., 70., 90., 300. ),
    #probe_Pho_full5x5_r9 = cms.vdouble(0. ,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.875,0.9,0.925,0.95,0.975 ,1.),
    #probe_Pho_full5x5x_r9 = cms.vdouble(0.5,1.0),
    
    #probe_sc_abseta = cms.vdouble(0.0, 1.479),    probe_Pho_full5x5x_r9 = cms.vdouble(0.85, 1.0), #cat 0
    probe_sc_abseta = cms.vdouble( 1.566,2.5),  probe_Pho_full5x5x_r9 = cms.vdouble(0.90, 1.0), #cat 1
    #probe_sc_abseta = cms.vdouble(0.0, 1.479),    probe_Pho_full5x5x_r9 = cms.vdouble(0.50, 0.85), #cat 2
    #probe_sc_abseta = cms.vdouble( 1.566,2.5),  probe_Pho_full5x5x_r9 = cms.vdouble(0.80, 0.9), #cat 3
    #probe_sc_abseta = cms.vdouble(0.0,0.2,0.4,0.8,1.0,1.2,1.479,1.7,1.9,2.1,2.3,2.5),
    #probe_sc_abseta = cms.vdouble(1.479),
    #probe_sc_abseta = cms.vdouble(0.0, 1.479,1.566, 2.5),
    #probe_eta = cms.vdouble( -2.5, 2.5 ),
    )

EfficiencyBinningSpecification = cms.PSet(
    UnbinnedVariables = cms.vstring("mass", "totWeight"),
    BinnedVariables = cms.PSet(EfficiencyBins,
                               mcTrue = cms.vstring("false")
                               ),
    BinToPDFmap = cms.vstring("pdfSignalPlusBackground")  
    )

if (not options.isMC):
    EfficiencyBinningSpecification.UnbinnedVariables = cms.vstring("mass", "totWeight")
    EfficiencyBinningSpecification.BinnedVariables = cms.PSet(EfficiencyBins)
        
mcTruthModules = cms.PSet()
if (options.isMC):
    setattr(mcTruthModules, "MCtruth_" + options.idName, cms.PSet(EfficiencyBinningSpecification))
    setattr(getattr(mcTruthModules, "MCtruth_" + options.idName), "EfficiencyCategoryAndState", cms.vstring(options.idName, "pass"))

############################################################################################

process.TnPMeasurement = cms.EDAnalyzer("TagProbeFitTreeAnalyzer",
                                        InputFileNames = cms.vstring(InputFileName),
                                        InputDirectoryName = cms.string(options.dirName),
                                        InputTreeName = cms.string("fitter_tree"), 
                                        OutputFileName = cms.string(OutputFile),
                                        NumCPU = cms.uint32(1),
                                        SaveWorkspace = cms.bool(False), #VERY TIME CONSUMING FOR MC
                                        doCutAndCount = cms.bool(options.doCutAndCount),
                                        floatShapeParameters = cms.bool(True),
                                        binnedFit = cms.bool(True),
                                        binsForFit = cms.uint32(60),
                                        WeightVariable = cms.string("totWeight"),
                                        #WeightVariable = cms.string(""),
                                        #fixVars = cms.vstring("meanP", "meanF", "sigmaP", "sigmaF", "sigmaP_2", "sigmaF_2"),
                                        
                                        # defines all the real variables of the probes available in the input tree and intended for use in the efficiencies
                                        Variables = cms.PSet(mass = cms.vstring("Tag-Probe Mass", "60.0", "120.0", "GeV/c^{2}"),
                                                             probe_Pho_et = cms.vstring("Probe E_{T}", "0", "10000", "GeV/c"),
                                                             probe_sc_abseta = cms.vstring("Probe #eta", "0", "2.5", ""), 
                                                             probe_Pho_full5x5x_r9 = cms.vstring("Probe 5x5r9", "0.5", "1", ""), 
                                                             totWeight = cms.vstring("totWeight", "-1000000", "100000000", ""), 
                                                             ),
                                        
                                        # defines all the discrete variables of the probes available in the input tree and intended for use in the efficiency calculations
                                        Categories = cms.PSet(),
                                        
                                        # defines all the PDFs that will be available for the efficiency calculations; 
                                        # uses RooFit's "factory" syntax;
                                        # each pdf needs to define "signal", "backgroundPass", "backgroundFail" pdfs, "efficiency[0.9,0,1]" 
                                        # and "signalFractionInPassing[0.9]" are used for initial values  
                                        #Cuts = cms.PSet(r9Cut = cms.vstring("probe_Pho_r9","0.5","above")),
                                        
                                        PDFs = cms.PSet(pdfSignalPlusBackground = cms.vstring(
            "RooCBExGaussShape::signalResPass(mass,meanP[-0.0,-5.000,5.000],sigmaP[0.956,0.00,10.000],alphaP[0.999, 0.0,50.0],nP[1.402,0.000,50.000],sigmaP_2[1.000,0.500,15.00])",
            "RooCBExGaussShape::signalResFail(mass,meanF[-0.0,-5.000,5.000],sigmaF[3.331,0.00,15.000],alphaF[1.586, 0.0,50.0],nF[0.464,0.000,20.00], sigmaF_2[1.675,0.500,15.000])",
            "ZGeneratorLineShape::signalPhy(mass)",
            "RooCMSShape::backgroundPass(mass, alphaPass[60.,50.,70.], betaPass[0.01, 0.,0.1], gammaPass[0.1, 0, 1], peakPass[90.0])",
            "RooCMSShape::backgroundFail(mass, alphaFail[60.,50.,70.], betaFail[0.01, 0.,0.1], gammaFail[0.1, 0, 1], peakFail[90.0])",
            "FCONV::signalPass(mass, signalPhy, signalResPass)",
            "FCONV::signalFail(mass, signalPhy, signalResFail)",
            "efficiency[0.5,0,1]",
            "signalFractionInPassing[1.0]"
            )
                                                        ),
                                        
                                        
                                        # defines a set of efficiency calculations, what PDF to use for fitting and how to bin the data;
                                        # there will be a separate output directory for each calculation that includes a simultaneous fit, side band subtraction and counting. 
                                        Efficiencies = cms.PSet(mcTruthModules)
                                        )

setattr(process.TnPMeasurement.Categories, options.idName, cms.vstring(options.idName, "dummy[pass=1,fail=0]"))
setattr(process.TnPMeasurement.Categories, "mcTrue", cms.vstring("MC true", "dummy[true=1,false=0]"))

if (not options.isMC):
    for pdf in process.TnPMeasurement.PDFs.__dict__:
        param =  process.TnPMeasurement.PDFs.getParameter(pdf)
        if (type(param) is not cms.vstring):
            continue
        for i, l in enumerate(getattr(process.TnPMeasurement.PDFs, pdf)):
            if l.find("signalFractionInPassing") != -1:
                getattr(process.TnPMeasurement.PDFs, pdf)[i] = l.replace("[1.0]","[0.5,0.,1.]")
        
    setattr(process.TnPMeasurement.Efficiencies, options.idName, EfficiencyBinningSpecification)    
    setattr(getattr(process.TnPMeasurement.Efficiencies, options.idName) , "EfficiencyCategoryAndState", cms.vstring(options.idName, "pass"))
else:
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

