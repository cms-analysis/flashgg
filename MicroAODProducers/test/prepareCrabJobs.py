#!/usr/bin/env python
from os import path
from subprocess import call, Popen, PIPE

# Arguments
# FIXME: Use argparse argument parsing instead, for the day CRAB3 librairies will stop downgrading the python version with respect to CMSSW :|
flashggVersion = "alphaV0"
parameterSet = "simple_Producer_test.py"
checkIfOnT2 = 1
checkNFiles = 1
createCrabConfig = 1
unitsPerJob = 10

# Prepare the list of samples to run over with default crab parameters
samples = []
##### SM Higgs samples
samples.append("/GluGluToHToGG_M-125_13TeV-powheg-pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v2/MINIAODSIM")
samples.append("/TTbarH_HToGG_M-125_13TeV_pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM")
samples.append("/VBF_HToGG_M-125_13TeV-powheg-pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM")
samples.append("/WH_ZH_HToGG_M-125_13TeV_pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v2/MINIAODSIM")
samples.append("/TTbarH_HToGG_M-125_13TeV_amcatnlo-pythia8-tauola/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM")

##### SM diHiggs samples
samples.append("/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM")

##### Diphoton backgrounds
samples.append("/GJet_Pt20to40_doubleEMEnriched_TuneZ2star_13TeV-pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM")
samples.append("/GJet_Pt40_doubleEMEnriched_TuneZ2star_13TeV-pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v2/MINIAODSIM")

##### DY data and MC for validation
samples.append("/DYJetsToLL_M-50_13TeV-madgraph-pythia8-tauola_v2/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM")
#samples.append("/DoubleElectron/CMSSW_7_0_6_patch1-GR_70_V2_AN1_RelVal_zEl2012D-v1/MINIAOD")

if checkIfOnT2:
    print "### WARNING: make sure you have loaded CRAB3 and a valid CMS VO proxy ###"
    print "Check if the samples are available on T2"
    for sample in samples:
        PrimaryDataset, ProcessedDataset, DataTier = filter(None, sample.split("/"))
        print "Checking availability on T2/T3 of dataset ", PrimaryDataset
        call(["das_client.py --query \"site dataset=" + sample + "\" | grep T[2-3]"], shell=True)

if checkNFiles:
    print "### WARNING: make sure you have loaded CRAB3 and a valid CMS VO proxy ###"
    print "Check how many files to be processed there are per sample"
    for sample in samples:
        PrimaryDataset, ProcessedDataset, DataTier = filter(None, sample.split("/"))
        p = Popen(["das_client.py --query \"file dataset=" + sample + "\" --limit=0 | grep store | wc -l"], shell=True, stdout=PIPE)
        out, err = p.communicate()
        print "Number of files for ", PrimaryDataset, out

if createCrabConfig:
    print "Create the crab config files with the default CMSSW python config file"
    for sample in samples:
        PrimaryDataset, ProcessedDataset, DataTier = filter(None, sample.split("/"))
        print "Preparing crab for processing ", PrimaryDataset
        # Increment flashgg- processing index if job has been launched before (ie if crab dir already exists)
        itry = 0
        jobname = "_".join([flashggVersion, PrimaryDataset, str(itry).zfill(2)])
        while path.isdir("crab_" + jobname):
            itry += 1
            jobname = "_".join([flashggVersion, PrimaryDataset, str(itry).zfill(2)])
        # Actually create the config file: copy the template and replace things where appropriate
        crabConfigFile = "crabConfig_" + jobname + ".py"
        replacements = {"JOBNAME":jobname, "PSET":parameterSet, "DATASET":sample, "UNITSPERJOB":str(unitsPerJob), "FLASHGG_VERSION":flashggVersion}
        infile = open("crabConfig_TEMPLATE.py")
        outfile = open(crabConfigFile, 'w')
        for line in infile:
            for src, target in replacements.iteritems():
                line = line.replace(src, target)
            outfile.write(line)
        infile.close()
        outfile.close()

