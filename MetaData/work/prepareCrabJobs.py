#!/usr/bin/env python
import os
from subprocess import call, Popen, PIPE

# Load command line option utilities
from flashgg.MetaData.optpars_utils import *

def dumpCfg(options):
    cfg = {}
    for key,val in options.__dict__.iteritems():
        if key != "__opt__" and key != "dumpCfg":
            cfg[key] = val

    return json.dumps( cfg,indent=4)

# Command line options
parser = OptionParser(option_list=[
        make_option("-V","--flashggVersion",
                    action="store", dest="flashggVersion", type="string",
                    default=None,
                    help="FLASHgg version to use. default: %default", 
                    ),
        make_option("--label",
                    action="store", dest="label", type="string",
                    default=None,
                    help="Extra label to add to dataset name (if none take input dataset). default: %default", 
                    ),
        make_option("-p","--pset",
                    action="store", dest="parameterSet", type="string",
                    default="../../MicroAODProducers/test/simple_Producer_test.py", # FIXME should move it to production eventually
                    help="CMSSW parameter set. default: %default", 
                    ),
        make_option("-t","--crabTemplate",
                    action="store", dest="crabTemplate", type="string",
                    default="crabConfig_TEMPLATE.py", 
                    help="CRAB configuration template. default: %default", 
                    ),
        make_option("-c","--createCrabConfig",
                    action="store_true", dest="createCrabConfig",
                    default=True,
                    help="Generate crab configuration. default: %default", 
                    ),
        make_option("--skipCreateCrabConfig",
                    action="store_false", dest="createCrabConfig"
                    ),
        make_option("--checkIfOnT2",
                    action="store_true", dest="checkIfOnT2",
                    default=False,
                    help="default: %default", 
                    ),
        make_option("-n","--checkNFiles",
                    action="store_true", dest="checkNFiles",
                    default=False,
                    help="default: %default", 
                    ),
        make_option("-U","--unitsPerJob",
                    action="store", dest="unitsPerJob", type="int",
                    default=1,
                    help="default: %default", 
                    ),
        make_option("-L","--lumisPerJob",
                    action="store", dest="lumisPerJob", type="int",
                    default=10,
                    help="default: %default", 
                    ),
        make_option("-s","--samples",
                    dest="samples",action="callback",callback=Load(),type="string",
                    default={},
                    help="load samples from JSON file",
                    ),
        make_option("-o","--outputPath",
                    dest="outputPath",action="store",type="string",
                    default="/store/group/phys_higgs/cmshgg/%s/flashgg" % os.getlogin(),
                    help="output storage path. default: %default",
                    ),
        make_option("-C","--campaign",
                    dest="campaign",action="store",type="string",
                    default="CSA14",
                    help="production campaign. default: %default",
                    ),
        make_option("-O","--outputSite",
                    dest="outputSite",action="store",type="string",
                    default="T2_CH_CERN",
                    help="output storage path. default: %default",
                    ),
        make_option("--mkPilot",
                    action="store_true",dest="mkPilot",
                    default=False,
                    help="prepare configuration for pilot job. default: %default"),
        make_option("--load",  # special option to load whole configuaration from JSON
                    action="callback",callback=Load(),dest="__opts__",
                    type="string",
                    help="load JSON file with configuration",metavar="CONFIG.json"
                    ),
        make_option("--dumpCfg",
                    action="store_true",
                    default=False,
                    help="dump configuaration and exit. default: %default"),
        make_option("-v","--verbose",
                    action="store_true", dest="verbose",
                    default=False,
                    help="default: %default",)
        ]
                      )
# parse the command line
(options, args) = parser.parse_args()

if options.dumpCfg:
    print ( dumpCfg(cfg) )
    exit(0)

## "flashgg-MicroAOD-alphaV2"
if not options.flashggVersion:
    git=Popen(['git', 'describe', '--tags'], stdout=PIPE)
    options.flashggVersion = git.stdout.read().split("\n")[0]

flashggVersion = options.flashggVersion
# FIXME: check flashgg version format
#   also check for uncommitted/untracked files and complain
options.parameterSet = os.path.abspath(options.parameterSet)
options.crabTemplate = os.path.abspath(options.crabTemplate)

if options.verbose:
    print ( json.dumps( options.__dict__,indent=4) )


# Prepare the list of samples to run over with default crab parameters
# Keep separate lists for signal backgound and data since treatment may change
data = []
bkg  = []
sig  = []
if len(options.samples) == 0:
    ##### SM Higgs samples
    sig.append("/GluGluToHToGG_M-125_13TeV-powheg-pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v2/MINIAODSIM")
    sig.append("/VBF_HToGG_M-125_13TeV-powheg-pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM")
    sig.append("/WH_ZH_HToGG_M-125_13TeV_pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v2/MINIAODSIM")
    sig.append("/TTbarH_HToGG_M-125_13TeV_pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM")
    sig.append("/TTbarH_HToGG_M-125_13TeV_amcatnlo-pythia8-tauola/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM")
    
    ##### SM diHiggs samples
    sig.append("/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM")
    
    ##### Diphoton backgrounds
    bkg.append("/GJet_Pt20to40_doubleEMEnriched_TuneZ2star_13TeV-pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM")
    bkg.append("/GJet_Pt40_doubleEMEnriched_TuneZ2star_13TeV-pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v2/MINIAODSIM")

    ##### DY data and MC for validation
    bkg.append("/DYJetsToLL_M-50_13TeV-madgraph-pythia8-tauola_v2/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM")
    
    #data.append("/DoubleElectron/CMSSW_7_0_6_patch1-GR_70_V2_AN1_RelVal_zEl2012D-v1/MINIAOD")
else:
    sig  = options.samples["sig"]
    bkg  = options.samples["bkg"]
    data = options.samples["data"]

samples = sig+bkg+data

pilotSamples=[]

if options.mkPilot:
    for group in sig,bkg,data:
        if len(group) > 0:
            pilotSamples.append( group[0] )

if options.checkIfOnT2:
    print "### WARNING: make sure you have loaded CRAB3 and a valid CMS VO proxy ###"
    print "Check if the samples are available on T2"
    for sample in samples:
        PrimaryDataset, ProcessedDataset, DataTier = filter(None, sample.split("/"))
        print "Checking availability on T2/T3 of dataset ", PrimaryDataset
        call(["das_client.py --query \"site dataset=" + sample + "\" | grep T[2-3]"], shell=True)

if options.checkNFiles:
    print "### WARNING: make sure you have loaded CRAB3 and a valid CMS VO proxy ###"
    print "Check how many files to be processed there are per sample"
    for sample in samples:
        PrimaryDataset, ProcessedDataset, DataTier = filter(None, sample.split("/"))
        p = Popen(["das_client.py --query \"file dataset=" + sample + "\" --limit=0 | grep store | wc -l"], shell=True, stdout=PIPE)
        out, err = p.communicate()
        print "Number of files for ", PrimaryDataset, out

if options.createCrabConfig:
    print ("\nCreating CRAB configurations in %s" % options.campaign)
    print ("--------------------------------------------------------")
    if not os.path.isdir(options.campaign):
        os.mkdir(options.campaign)
    os.chdir(options.campaign)
    print ("Parameter set: %s\nflashggVersion: %s\ncrab template: %s\n" % (options.parameterSet,flashggVersion,options.crabTemplate))
    print ("Copying over parameter set")
    Popen(['cp', '-p', options.parameterSet, './'])
    print ("Storing options into config.json")
    cfg = open("config.json","w+")
    cfg.write( dumpCfg(options) )
    infile = open(options.crabTemplate)
    template = [ line for line in infile ]
    infile.close()
    for sample in samples:
        PrimaryDataset, ProcessedDataset, DataTier = filter(None, sample.split("/"))
        label = ProcessedDataset
        if options.label:
            label = options.label
        # Increment flashgg- processing index if job has been launched before (ie if crab dir already exists)
        itry = 0
        jobname = "_".join([flashggVersion, PrimaryDataset, str(itry).zfill(2)])
        while os.path.isdir("crab_" + jobname):
            itry += 1
            jobname = "_".join([flashggVersion, PrimaryDataset, str(itry).zfill(2)])
        # Actually create the config file: copy the template and replace things where appropriate
        crabConfigFile = "crabConfig_" + jobname + ".py"
        print "Preparing crab for processing ", PrimaryDataset, "\n      -> ", crabConfigFile
        replacements = {"JOBNAME"         : jobname,
                        "PSET"            : os.path.basename(options.parameterSet),
                        "DATASET"         : sample,
                        "UNITSPERJOB"     : str(options.unitsPerJob),
                        "FLASHGG_VERSION" : "%s-%s-v%d" % (options.campaign,flashggVersion, itry),
                        "PROCESSED_DSET"  : label,
                        "SPLITTING"       : "FileBased",
                        "OUTLFN"          : "%s/%s/%s" % (options.outputPath,options.campaign,flashggVersion),
                        "OUTSITE"         : options.outputSite
                       }
        # specific replacements for data and MC
        if sample in data:
            replacements["SPLITTING"]   = "LumiBased"
            replacements["UNITSPERJOB"] = str(options.lumisPerJob),
            ## FIXME: lumi mask, run ranges, etc.
        if sample in sig:
            ## Extra options for signal samples
            pass
        if sample in bkg:
            ## Extra options for background samples
            pass
        # open output file
        outfiles = [ open(crabConfigFile, 'w') ]
        pilotFile = None
        if sample in pilotSamples: # make pilot job configuration if needed
            print( "Generating pilot job configuration pilot_%s" % crabConfigFile)
            outfiles.append( open("pilot_%s" % crabConfigFile, 'w' ) )
            pilotFile = outfiles[-1]
        for line in template:
            oline = line
            for src, target in replacements.iteritems():
                oline = oline.replace(src, target)
            for outfile in outfiles:
                outfile.write(oline)
        if pilotFile:
            pilotFile.write("config.Data.totalUnits = %d\n" % (options.unitsPerJob))
            pilotFile.write("config.Data.publication = False\n")
        # close output files
        for outfile in outfiles:
            outfile.close()


        
