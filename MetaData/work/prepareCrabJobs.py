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
                    default="../../MicroAOD/test/microAODstd.py", # FIXME should move it to production eventually
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
        make_option("--lumiMask",
                    action="store", dest="lumiMask", type="string",
                    default=None,
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
        make_option("-d","--task-dir",
                    dest="task_dir",action="store",type="string",
                    default=None,
                    help="task folder. default: same as campaign",
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
                    help="default: %default"),
        make_option("--gt","--globalTags",
                    dest="globalTags",
                    action="store",type="string",
                    default="campaigns/globalTagsLookup.json",
                    help="List of global tags to be used for data and MC. Default: %default"),
        
        # include additional parameters for cmsRun, such as the parameters from microAODCustomize
        # the default here is puppi=0 (as an example) see microAODCustomize_cfg.py for more detail
        make_option("-e","--extraPyCfgParam",
                    dest="extraPyCfgParam",
                    action="store",type="string",
                    default=None,
                    help="Extra python config parameters. The arguments must be : -e 'arg1 arg2 ...' ")
        
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

if options.lumiMask: options.lumiMask = os.path.abspath(options.lumiMask)
lumiMasks = {}
datasamples = []
for sample in data:
    if type(sample) == list: 
        if len(sample) != 2: 
            print "ERROR: I think that you tried to specify a sample-specific lumi mask, but the format is incorrect."
            print "       Valid format is [<sample_name>,<lumi_mask>]"
            print "%s" % sample
            sys.exit(-1)
        datasamples.append(sample[0])
        lumiMasks[sample[0]] = os.path.abspath(sample[1])
    else:
        datasamples.append(sample)
data=datasamples


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

    # get the globaltag json
    globalTagPath = options.globalTags
    gtJson = json.load(open(globalTagPath,'r'))
    print "GLOBAL TAGS:",options.globalTags

    if not options.task_dir:
        options.task_dir = options.campaign
    if not os.path.isdir(options.task_dir):
        os.mkdir(options.task_dir)
    os.chdir(options.task_dir)
    print ("Parameter set: %s\nflashggVersion: %s\ncrab template: %s\n" % (options.parameterSet,flashggVersion,options.crabTemplate))
    print ("Copying over parameter set")
    Popen(['cp', '-p', options.parameterSet, './'])
    rel = os.environ.get('CMSSW_BASE')
    print rel
    Popen(['cp', '-p', rel+'/src/flashgg/Systematics/data/JEC/Summer16_23Sep2016V4_MC.db', './'])
    Popen(['cp', '-p', rel+'/src/flashgg/Systematics/data/JEC/Summer16_23Sep2016AllV4_DATA.db', './'])
    Popen(['cp', '-p', rel+'/src/flashgg/MicroAOD/data/QGL_80X.db', './'])
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
        ### if sample in data:
        ###     if ProcessedDataset.count("201"):
        ###         position = ProcessedDataset.find("201")
        ###         PrimaryDataset = PrimaryDataset +"-"+ ProcessedDataset[position:]
            
        jobname = "_".join([flashggVersion, PrimaryDataset, ProcessedDataset])
        orig_jobname = jobname
        while os.path.isdir("crab_" + jobname):
            itry += 1
            jobname = "_".join([flashggVersion, PrimaryDataset, ProcessedDataset, str(itry).zfill(2)])
        if len(jobname) > 97:
            jobname = jobname.replace("TuneCUEP8M1_13TeV-pythia8","13TeV")
        if len(jobname) > 97:
            jobname = jobname.replace("TuneCUETP8M1_13TeV-madgraphMLM-pythia8","13TeV-mg")
        if len(jobname) > 97:
            jobname = jobname.replace("TuneCUETP8M1_13TeV_Pythia8","13TeV-p8")
        if len(jobname) > 97:
            jobname = jobname.replace("RSGravToGG","Grav")
        if len(jobname) > 97:
            jobname = jobname.replace("-PU25nsData2015v1","")
        if len(jobname) > 97:
            jobname = jobname.replace("RunIIFall15MiniAODv2_","")
        if len(jobname) > 97:
            jobname = jobname.replace("RunIIFall15MiniAODv2-magnetOffBS0T_PU25nsData2015v1_0T","0T")
        if len(jobname) > 97:
            jobname = jobname.replace("RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2","Spring16")
        if len(jobname) > 97:
            jobname = jobname.replace("RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2","Spring16")
        if len(jobname) > 97:
            jobname = jobname.replace("RunIISpring16MiniAODv1-PUSpring16RAWAODSIM_80X_mcRun2","Spring16")
        if len(jobname) > 97:
            jobname = jobname.replace("RunIISpring16MiniAODv2-PUSpring16RAWAODSIM","Spring16")
        if len(jobname) > 97:
            jobname = jobname.replace("RunIISpring16MiniAODv1-PUSpring16RAWAODSIM","Spring16")
        if len(jobname) > 97:
            jobname = jobname.replace("plus","p")
        if len(jobname) > 97:
            jobname = jobname.replace("minus","m")
        if len(jobname) > 97:
            jobname = jobname.replace("percentMaterial","Mat")
        if len(jobname) > 97:
            jobname = jobname.replace("TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8","13TeV-amcnlo-p8")
        if len(jobname) > 97:
            jobname = jobname.replace("80X_mcRun2_asymptotic_2016_miniAODv2","asym16")
        if len(jobname) > 97:
            jobname = jobname.replace("pythia8","p8")
        if len(jobname) > 97:
            jobname = jobname.replace("mcRun2_asymptotic_2016","asym16")
        if len(jobname) > 97:
            jobname = jobname.replace("asymptotic_2016","asym16")
        if len(jobname) > 97:
            jobname = jobname.replace("reHLT_80X_mcRun2_asymptotic_v14-v1","reHLTasym16")
        if len(jobname) > 97:
            jobname = jobname.replace("_withHLT_80X_mcRun2_asymptotic_v14_ext1-v1","reHLTasym16")
        if len(jobname) > 97:
            jobname = jobname.replace("RunIISummer16MiniAODv2-PUMoriond17","Summer16")
        if len(jobname) > 97:
            jobname = jobname.replace("TrancheIV","T4")
        if len(jobname) > 97:
                jobname = jobname.replace("PhaseISpring17MiniAOD-FlatPU28to62_90X_upgrade2017_realistic_v20-v1","PhaseISpring17")
        if len(jobname) > 97:
            print orig_jobname
            print "-->", len(jobname), jobname
            raise Exception,"jobname remains too long, additional hacks needed in prepareCrabJobs.py"
        #if len(jobname) > 97:
        #    print "jobname length: %d " % len(jobname)
        #    jobname = jobname[:97]
        jobname0 = jobname.rstrip("-").rstrip("-v")
        
        # Increment flashgg- processing index if job has been launched before (ie if crab dir already exists)
        itry = 0
        jobname = jobname0+"_%s" % ( str(itry).zfill(2) )
        while os.path.isdir("crab_" + jobname):
            itry += 1
            jobname = jobname0+"_%s" % ( str(itry).zfill(2) )
            
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
                        "OUTSITE"         : options.outputSite,
                        "PYCFG_PARAMS"    : [str("datasetName=%s" % sample)]
                       }

        # remove the processing version number from the ProcessedDataset
        PrimaryDataset, ProcessedDataset, DataTier = filter(None, sample.split("/")) 
        position = ProcessedDataset.find("-v")
        processedLabel = ProcessedDataset[:position]
        # print processedLabel
        
        # apprend extra parameters
        if options.extraPyCfgParam:
            replacements["PYCFG_PARAMS"].extend(map( lambda x: '"%s"' % x, options.extraPyCfgParam.split(" ") ))
            
        # associate the processedLabel to the globaltag from the json filex
        if gtJson.get(processedLabel,None):
            globalTag = gtJson[processedLabel]
            # print ProcessedDataset, globalTag
            replacements["PYCFG_PARAMS"].append(str("globalTag=%s" % globalTag[0])) 
        else:
            print 
            print "WARNING: you did not associate any global tag to %s" % processedLabel
            print "         therefore global tag customization will not work for %s" % sample
            print 
            
        # specific replacements for data and MC
        if sample in data:
            replacements["SPLITTING"]   = "LumiBased"
            replacements["UNITSPERJOB"] = str(options.lumisPerJob)
            replacements["PYCFG_PARAMS"].append("processType=data")
            ## FIXME: lumi mask, run ranges, etc.
        if sample in sig:
            ## Extra options for signal samples
            replacements["PYCFG_PARAMS"].append("processType=signal")
        if sample in bkg:
            ## Extra options for background samples
            replacements["PYCFG_PARAMS"].append("processType=background")

        replacements["PYCFG_PARAMS"] = str(replacements["PYCFG_PARAMS"])
        
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
                
        if sample in data:
            lumiMask = lumiMasks.get(sample,options.lumiMask)
            if lumiMask:
                for outfile in outfiles:
                    outfile.write('config.Data.lumiMask = "%s"\n' % (lumiMask))
            else:
                print 
                print "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
                print "WARNING: you are running on data withut specifying a lumi mask"
                print "         please make sure that you know what you are doing"
                print "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
                print 

        if pilotFile:
            pilotFile.write("config.Data.totalUnits = %d\n" % (options.unitsPerJob))
            pilotFile.write("config.Data.publication = False\n")
            pilotFile.write('config.General.requestName = "pilot_%s"\n' % jobname)
        # close output files
        for outfile in outfiles:
            outfile.close()


        
