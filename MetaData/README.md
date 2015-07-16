# flashgg/MetaData README

## Producing microAODs

The so-called microAOD format is a subset of the MINIAOD produced by the flashgg framework, adding specific photon-related informations.

Only high level objects such as photons, leptons, jets and MET are stpored in microAD by default. If additional informations are needed, 
they need to be added explicitely to the output.

The package flashgg/MetaData contains the scripts related to microAOD production.
To start with, go to `flashgg/MetaData/work`.

```
cmsenv
cd ${CMSSW_BASE}/src/flashgg/MetaData/work 
```

The folder contains the script `prepareCrabJobs.py`, which sets up the crab3 tasks needed to generate microAODs. 

First, you need to set up the crab3 and grid environment.

```
source /cvmfs/cms.cern.ch/crab3/crab.sh
voms-proxy-init --voms cms --valid 168:00
```

Then, prepare a json file with the list of datasets to be processed, as in the example below.

```
{
    "data" : ["/DoubleElectron/CMSSW_7_0_6_patch1-GR_70_V2_AN1_RelVal_zEl2012D-v1/MINIAOD"
              ],
    "sig"  : ["/GluGluToHToGG_M-125_13TeV-powheg-pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v2/MINIAODSIM",
              "/VBF_HToGG_M-125_13TeV-powheg-pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM",
              "/WH_ZH_HToGG_M-125_13TeV_pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v2/MINIAODSIM",
              "/TTbarH_HToGG_M-125_13TeV_pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM",
              "/TTbarH_HToGG_M-125_13TeV_amcatnlo-pythia8-tauola/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM",
              "/GluGluToHHTo2B2G_M-125_13TeV-madgraph-pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM"
              ],
    "bkg"  : ["/GJet_Pt20to40_doubleEMEnriched_TuneZ2star_13TeV-pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM",
              "/GJet_Pt40_doubleEMEnriched_TuneZ2star_13TeV-pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v2/MINIAODSIM",
              "/DYJetsToLL_M-50_13TeV-madgraph-pythia8-tauola_v2/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM"
              ]
}
```

To generate the crab3 configuration run (see `./prepareCrabJobs.py --help` for the full list of options).
```
./prepareCrabJobs.py -p <parameterSet> \
		     -C <microAODCampaginName> \
		     -s <jsonFileWithDatasetList> \
		     --mkPilot
```

This will create a folder containing all the crab3 configuration files, the CMSSW parameterSet as well as a file called `config.json` containing the dump
of all the script options.
The latter can be used to reproduce the same configuration (using the `--load` option), eg when adding more dataset to the same campaign.
```
./prepareCrabJobs.py --load <previous_config.json> \
		     -s <jsonFileWithAdditionalDatasets>
```

The `--mkPilot` option will create an additional crab3 configuration that can be used to run on a single file, before submitting the whole list of tasks.

You can now lanch the microAOD production.

```
cd <microAODCampaignName>
echo crabConfig_*.py | xargs -n 1 crab sub
## or using GNU parallel 
## parallel 'crab sub {}' ::: crabConfig_*.py
```

To check status of all jobs:

```
cd <microAODCampaignName>
echo crab_* | xargs -n 1 crab status
```

## Specific production campaigns

Exact instructions used, after setting up the area with the given tag, doing cmsenv, and vomx-proxy-init as above:

### SPRING15

# Further fixes and configuration tweaks for smooth running

```
cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C RunIISpring15-50ns -U 1 -s campaigns/RunIISpring15-50ns.json -V Spring15BetaV2 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py
cd RunIISpring15-50ns
echo crabConfig_*.py | xargs -n 1 crab sub
```

# CMSSW 7_4_6_patch2, no recomputation of cluster shapes

```
cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C RunIISpring15-50ns -U 1 -s campaigns/RunIISpring15-50ns.json -V Spring15BetaV1 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py
cd RunIISpring15-50ns
echo crabConfig_*.py | xargs -n 1 crab sub
```

### SPRING15, preliminary, still recomputing 5x5

```
cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C RunIISpring15-50ns -U 1 -s campaigns/RunIISpring15-50ns.json -V Spring15BetaV0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py
cd RunIISpring15-50ns
echo crabConfig_*.py | xargs -n 1 crab sub
```

### PHYS14

```
cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C HggPhys14 -U 5 -s campaigns/Phys14_samples.json -V Phys14MicroAODV1 -p ../../MicroAODProducers/test/simple_Producer_test.py
cd HggPhys14
echo crabConfig_*.py | xargs -n 1 crab sub
```

### PHYS14 V2, 10 February 2015
```
cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C HggPhys14 -U 5 -s campaigns/Phys14_samples.json -V Phys14MicroAODV2 -p configs/MicroAOD_withIsoScan_10Feb2015.py
cd HggPhys14
echo crabConfig_*.py | xargs -n 1 crab sub
```

### PHYS14 V3, 13 April 2015
```
cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C HggPhys14 -U 5 -s campaigns/Phys14_samples.json -V Phys14MicroAODV3 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py
cd HggPhys14
echo crabConfig_*.py | xargs -n 1 crab sub
```

## Running on microAODs

Once the microAOD production is completed, analysis code can be run on the resulting samples.

While crab can still be used to perfromed such operation, having a quicker system may be desirable. For this reason, a small set of scripts has been set.
The system is able to import datasets from DBS and then handle the job management, including the jobs and output collection.
In addition, automatic weighting of the samples to luminosity and pile-up can be performed.

Full framework as well as (compiled) framework lite analyses are supported. Handling of plain flwite macros is not supported at the moment.

The following scritps can be used to manage samples and jobs:
* `fggManageSamples.py`
* `fggRunJobs.py`
Both scripts are locate in `flashgg/MetaData/scripts` and they are installed in the shell `PATH` upon `scram build`. 
Note: make sure to run `cmsenv` before executing any of these commands. 

### Importing datasets from DBS

In order to be able to run on some samples, the samples need to be imported first. Three steps are needed for this.

1. _`import`_. Import copies the list of files from DBS to a local json file called `catalog`. 
   ```
fggManageSamples.py -C <campagin> -V <flashggversion> import 
# e.g. fggManageSamples.py -C HggPhys14 -V Phys14MicroAODV1 import 
```
   This command will import all datasets matching `/*/*<campaign>*-*<flashggversion>*/USER` and writes the list of file in `flashgg/MetaData/data/<campaign>/dataset.json`. 
A different location for the meta data can be specified with the `--metaDataSrc <pkgName>` option. This will change the destination to `<pkgName>/MetaData/data/<campaign>/datasets.json`. 
An example output for the `import` command is shown below.
   ```
Will use the following datasets catalog:
/afs/cern.ch/work/m/musella/private/Analysis/FGG_7_2_1_patch4/src/flashgg/MetaData/data/HggPhys14/datasets.json
Importing from das /*/*HggPhys14-Phys14MicroAODV1*/USER
Datasets to import
/DYJetsToLL_M-50_13TeV-madgraph-pythia8/sethzenz-HggPhys14-Phys14MicroAODV1-v0-Phys14DR-PU4bx50_PHYS14_25_V1-v1-d7bb6e4a06af43bf30c0514b01defd0b/USER
/DYJetsToLL_M-50_13TeV-madgraph-pythia8/sethzenz-HggPhys14-Phys14MicroAODV1-v1-Phys14DR-PU4bx50_PHYS14_25_V1-v1-d7bb6e4a06af43bf30c0514b01defd0b/USER
/GJet_Pt40_doubleEMEnriched_TuneZ2star_13TeV-pythia6/sethzenz-HggPhys14-Phys14MicroAODV1-v0-Phys14DR-PU20bx25_PHYS14_25_V1-v1-d7bb6e4a06af43bf30c0514b01defd0b/USER

[...]

Importing /DYJetsToLL_M-50_13TeV-madgraph-pythia8/sethzenz-HggPhys14-Phys14MicroAODV1-v0-Phys14DR-PU4bx50_PHYS14_25_V1-v1-d7bb6e4a06af43bf30c0514b01defd0b/USER
Importing /DYJetsToLL_M-50_13TeV-madgraph-pythia8/sethzenz-HggPhys14-Phys14MicroAODV1-v1-Phys14DR-PU4bx50_PHYS14_25_V1-v1-d7bb6e4a06af43bf30c0514b01defd0b/USER
Importing /GJet_Pt40_doubleEMEnriched_TuneZ2star_13TeV-pythia6/sethzenz-HggPhys14-Phys14MicroAODV1-v0-Phys14DR-PU20bx25_PHYS14_25_V1-v1-d7bb6e4a06af43bf30c0514

[...]
```
   At any time, the catalog content can be inspected by the `list` command.

2. `review` . After importing a list of datasets, the catalog content can be reviewed with the `review` command. This allows to remove undesired (eg duplicated) datasets from the catalog.
   This operation is achieved by
   ```fggManageSample -C <campaign> review```
   An example of the output is shown below.
   ```
Will use the following datasets catalog:
/afs/cern.ch/work/m/musella/private/Analysis/FGG_7_2_1_patch4/src/flashgg/MetaData/data/HggPhys14/datasets.json
keep this dataset (yes/no/all)?
 /DYJetsToLL_M-50_13TeV-madgraph-pythia8/sethzenz-HggPhys14-Phys14MicroAODV1-v0-Phys14DR-PU4bx50_PHYS14_25_V1-v1-d7bb6e4a06af43bf30c0514b01defd0b/USER
 [yna] a
More than one sample for GJet_Pt40_doubleEMEnriched_TuneZ2star_13TeV-pythia6:
 /GJet_Pt40_doubleEMEnriched_TuneZ2star_13TeV-pythia6/sethzenz-HggPhys14-Phys14MicroAODV1-v0-Phys14DR-PU20bx25_PHYS14_25_V1-v1-d7bb6e4a06af43bf30c0514b01defd0b/USER
 /GJet_Pt40_doubleEMEnriched_TuneZ2star_13TeV-pythia6/sethzenz-HggPhys14-Phys14MicroAODV1-v1-Phys14DR-PU20bx25_PHYS14_25_V1-v1-d7bb6e4a06af43bf30c0514b01defd0b/USER
Keep all? [yn] n
keep this dataset?
 /GJet_Pt40_doubleEMEnriched_TuneZ2star_13TeV-pythia6/sethzenz-HggPhys14-Phys14MicroAODV1-v0-Phys14DR-PU20bx25_PHYS14_25_V1-v1-d7bb6e4a06af43bf30c0514b01defd0b/USER
 [yn] n
keep this dataset?
 /GJet_Pt40_doubleEMEnriched_TuneZ2star_13TeV-pythia6/sethzenz-HggPhys14-Phys14MicroAODV1-v1-Phys14DR-PU20bx25_PHYS14_25_V1-v1-d7bb6e4a06af43bf30c0514b01defd0b/USER
 [yn] y

[...]
```

3. `check`. Finally, the content of the catalog should be validated using the `check` command. This command will run on all the files in the catalog, check that they can be correctly read in, and also compute sum of weights (for MC samples) which will be later used to reweight the samples. 
   ```
fggManageSamples.py -C <campaign>  check 

Will use the following datasets catalog:
/afs/cern.ch/work/m/musella/private/Analysis/FGG_7_2_1_patch4/src/flashgg/MetaData/data/HggPhys14/datasets.json
Checking all datasets
Checking dataset /TTbarH_HToGG_M-125_13TeV_amcatnlo-pythia8-tauola/sethzenz-HggPhys14-Phys14MicroAODV1-v1-Phys14DR-PU20bx25_PHYS14_25_V1-v1-d7bb6e4a06af43bf30c0514b01defd0b/USER
Checking dataset /WH_ZH_HToGG_M-125_13TeV_pythia6/sethzenz-HggPhys14-Phys14MicroAODV1-v1-Phys14DR-PU20bx25_PHYS14_25_V1-v1-d7bb6e4a06af43bf30c0514b01defd0b/USER
Checking dataset /GJet_Pt40_doubleEMEnriched_TuneZ2star_13TeV-pythia6/sethzenz-HggPhys14-Phys14MicroAODV1-v1-Phys14DR-PU20bx25_PHYS14_25_V1-v1-d7bb6e4a06af43bf30c0514b01defd0b/USER
Checking dataset /DYJetsToLL_M-50_13TeV-madgraph-pythia8/sethzenz-HggPhys14-Phys14MicroAODV1-v1-Phys14DR-PU4bx50_PHYS14_25_V1-v1-d7bb6e4a06af43bf30c0514b01defd0b/USER
Checking dataset /VBF_HToGG_M-125_13TeV-powheg-pythia6/sethzenz-HggPhys14-Phys14MicroAODV1-v1-Phys14DR-PU20bx25_PHYS14_25_V1-v1-d7bb6e4a06af43bf30c0514b01defd0b/USER
Checking dataset /GluGluToHToGG_M-125_13TeV-powheg-pythia6/sethzenz-HggPhys14-Phys14MicroAODV1-v1-Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1-d7bb6e4a06af43bf30c0514b01defd0b/USER
Finished jobs: 0. Total jobs: 26
[...]
Finished jobs: 25. Total jobs: 26
Writing catalog
Done
```
   After such operation, the bad files will be marked as such in the catalog and the total weight for each file will be reported.
   ```
{
    "/DYJetsToLL_M-50_13TeV-madgraph-pythia8/sethzenz-HggPhys14-Phys14MicroAODV1-v1-Phys14DR-PU4bx50_PHYS14_25_V1-v1-d7bb6e4a06af43bf30c0514b01defd0b/USER": {
        "files": [
            {
                "name": "/store/group/phys_higgs/cmshgg/sethzenz/flashgg/HggPhys14/Phys14MicroAODV1/DYJetsToLL_M-50_13TeV-madgraph-pythia8/HggPhys14-Phys14MicroAODV1-v1-Phys
                "nevents": 245736, 
                "weights": 245736.0
            }, 
[...]
}
```
4. The file catalog can now be committed to git.

### Job preparation

At this point, the catalog can be used to drive the job creation and submission process, which is handled by the `fggRunJobs.py` script.


#### Cross section database
Before looking into the actual job configuration, some more meta-data need to be created, namely the cross-section database. The latter is another json file, of the following form:
```
{
    "/GJet_Pt40_doubleEMEnriched_TuneZ2star_13TeV-pythia6" : { "xs" :  },
    "/GJet_Pt20-40_doubleEMEnriched_TuneZ2star_13TeV-pythia6" : { "xs" :  }
} 

```
Its default location is `flashgg/MetaData/data/cross_sections.json`, but several files can be specified.

#### Customization statements
The `fggRunJobs.py` supports jobs which are configured through a `CMSSW` parameter set. These can be `cmsRun` jobs, as well as compiled framework lite executable.

In order to be used by `fggRunJobs.py`, the process configuration, needs to add a specific customization statement, like the one reported below.
The customization class `flashgg.MetaData.JobConfig` uses the `VarParsing` utilities from `CMSSW` to pass command line options and reads information from the file catalog and the cross section database. 
```
# import flashgg customization
from flashgg.MetaData.JobConfig import customize
# set default options if needed
customize.setDefault("maxEvents",100)
customize.setDefault("targetLumi",10e+3)
# call the customization
customize(process)
```

A file catalog or cross-sections database from a different package can also be specified.
```
from flashgg.MetaData.JobConfig import JobConfig

customize = JobConfig(metaDataSrc=<pkgName>,
                      crossSections=["$CMSSW_BASE/src/<pkgName>/MetaData/data/cross_sections.json",
                      "/path/to/my/additional/cross_sections.json"
                      ])

```

#### Jobs driver configuration

Finally, one needs to specify the list of datasets on which `fggRunJobs.py` would need to run.
```
cat >> jobs_gamjets.json << EOF
{
    "processes" : {
	    "GJets_doubleEMEnriched_13TeV-pythia6" : [
	        "/GJet_Pt40_doubleEMEnriched_TuneZ2star_13TeV-pythia6",
		"/GJet_Pt20-40_doubleEMEnriched_TuneZ2star_13TeV-pythia6"
		]
	    },
     "cmdLine" : "campaign=HggPhys14 targetLumi=1e+3"
}
EOF
```
Any of the script options (see `fggJobsRun.py --help` for a full list) can actually be specified in the json file.

### Running jobs

At this point everythin is ready to run the analysis.
```
fggRunJobs.py --load jobs_gamgam_highmass.json -H -D -P -n 5 -d testMe -x cmsRun mypset.py maxEvents=-1
```

An equivalent syntax, provided that `mypset.py` is exectuable and that it specifes its interpreter as `#!/usr/bin/env cmsRun` is.
```
fggRunJobs.py --load jobs_gamgam_highmass.json -H -D -P -n 5 -d testMe ./mypset.py maxEvents=-1
```

Fwlite exectuables can be run in the same way replacing `cmsRun` by the actual executable name.

