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

Below are the exact instructions used, after setting up the area with the given tag, doing cmsenv, and vomx-proxy-init as above. 
**N.B.** to duplicate the settings of the production below, you **must** setup with the relevant tag:

```
cd $CMSSW_BASE/src/flashgg
git checkout [relevant tag]
scram b -j 9
```

In fact, it is recommended to set up a new area from scratch and checkout the tag before running setup.sh, as described here: 

https://twiki.cern.ch/twiki/bin/viewauth/CMS/FLASHggFramework#Instructions_for_users  

### Rerun on v2 (3_1_0)

./prepareCrabJobs.py -C RunIIFall17-3_1_0 -U 1 -L 25 -s campaigns/RunIIFall17-3_1_0_part0.json -V 3_1_0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/ReReco/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON_v1.txt

./prepareCrabJobs.py -C RunIIFall17-3_1_0 -U 1 -L 25 -s campaigns/RunIIFall17-3_1_0_part1.json -V 3_1_0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/ReReco/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON_v1.txt

./prepareCrabJobs.py -C RunIIFall17-3_1_0 -U 1 -L 25 -s campaigns/RunIIFall17-3_1_0_part2.json -V 3_1_0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/ReReco/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON_v1.txt

./prepareCrabJobs.py -C RunIIFall17-3_1_0 -U 1 -L 25 -s campaigns/RunIIFall17-3_1_0_part3.json -V 3_1_0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py 

./prepareCrabJobs.py -C RunIIFall17-3_1_0 -U 1 -L 25 -s campaigns/RunIIFall17-3_1_0_part4.json -V 3_1_0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/ReReco/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON_v1.txt

./prepareCrabJobs.py -C RunIIFall17-3_1_0 -U 1 -L 25 -s campaigns/RunIIFall17-3_1_0_part5.json -V 3_1_0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py

./prepareCrabJobs.py -C RunIIFall17-3_1_0 -U 1 -L 25 -s campaigns/RunIIFall17-3_1_0_part6.json -V 3_1_0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py

### First real 2017??? (3_0_0)

./prepareCrabJobs.py -C RunIIFall17-3_0_0 -U 1 -L 25 -s campaigns/RunIIFall17-3_0_0_part0.json -V 3_0_0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask jsons/Cert_294927-306462_13TeV_PromptReco_Collisions17_JSON.txt
./prepareCrabJobs.py -C RunIIFall17-3_0_0 -U 1 -L 25 -s campaigns/RunIIFall17-3_0_0_part1.json -V 3_0_0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask jsons/Cert_294927-306462_13TeV_PromptReco_Collisions17_JSON.txt
./prepareCrabJobs.py -C RunIIFall17-3_0_0 -U 1 -L 25 -s campaigns/RunIIFall17-3_0_0_part2.json -V 3_0_0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask jsons/Cert_294927-306462_13TeV_PromptReco_Collisions17_JSON.txt
./prepareCrabJobs.py -C RunIIFall17-3_0_0 -U 1 -L 25 -s campaigns/RunIIFall17-3_0_0_part3.json -V 3_0_0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask jsons/Cert_294927-306462_13TeV_PromptReco_Collisions17_JSON.txt
./prepareCrabJobs.py -C RunIIFall17-3_0_0 -U 1 -L 25 -s campaigns/RunIIFall17-3_0_0_part4.json -V 3_0_0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask jsons/Cert_294927-306462_13TeV_PromptReco_Collisions17_JSON.txt
./prepareCrabJobs.py -C RunIIFall17-3_0_0 -U 1 -L 25 -s campaigns/RunIIFall17-3_0_0_part5.json -V 3_0_0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask jsons/Cert_294927-306462_13TeV_PromptReco_Collisions17_JSON.txt
./prepareCrabJobs.py -C RunIIFall17-3_0_0 -U 1 -L 25 -s campaigns/RunIIFall17-3_0_0_part6.json -V 3_0_0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask jsons/Cert_294927-306462_13TeV_PromptReco_Collisions17_JSON.txt

#### Adding b regression vars for HH and update lumimask, otherwise consistent (3_0_1)

./prepareCrabJobs.py -C RunIIFall17-3_0_1 -U 1 -L 25 -s campaigns/RunIIFall17-3_0_1_part0.json -V 3_0_1 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask jsons/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON.txt
./prepareCrabJobs.py -C RunIIFall17-3_0_1 -U 1 -L 25 -s campaigns/RunIIFall17-3_0_1_part1.json -V 3_0_1 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py 
./prepareCrabJobs.py -C RunIIFall17-3_0_1 -U 1 -L 25 -s campaigns/RunIIFall17-3_0_1_part2.json -V 3_0_1 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask jsons/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON.txt
./prepareCrabJobs.py -C RunIIFall17-3_0_1 -U 1 -L 25 -s campaigns/RunIIFall17-3_0_1_part3.json -V 3_0_1 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py

#### Resubmit missing data (3_0_2, same as 3_0_1)

./prepareCrabJobs.py -C RunIIFall17-3_0_2 -U 1 -L 25 -s campaigns/RunIIFall17-3_0_2_DoubleEG.json -V 3_0_2 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask  jsons/ToRun_DoubleEG_2017_302_24April.json
./prepareCrabJobs.py -C RunIIFall17-3_0_2 -U 1 -L 25 -s campaigns/RunIIFall17-3_0_2_SingleElectron.json -V 3_0_2 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask  jsons/ToRun_SingleElectron_2017_302_24April.json
./prepareCrabJobs.py -C RunIIFall17-3_0_2 -U 1 -L 25 -s campaigns/RunIIFall17-3_0_2_DoubleMuon.json -V 3_0_2 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask  jsons/ToRun_DoubleMuon_2017_302_24April.json

#### Resubmit missing data (3_0_3, same as 3_0_1)

./prepareCrabJobs.py -C RunIIFall17-3_0_3 -U 1 -L 25 -s campaigns/RunIIFall17-3_0_3_DoubleEG.json -V 3_0_3 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask  jsons/ToRun_DoubleEG_2017_303_17May.json
./prepareCrabJobs.py -C RunIIFall17-3_0_3 -U 1 -L 25 -s campaigns/RunIIFall17-3_0_3_SingleElectron.json -V 3_0_3 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask  jsons/ToRun_SingleElectron_2017_303_17May.json
./prepareCrabJobs.py -C RunIIFall17-3_0_3 -U 1 -L 25 -s campaigns/RunIIFall17-3_0_3_DoubleMuon.json -V 3_0_3 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask  jsons/ToRun_DoubleMuon_2017_303_17May.json


### Aug Legacy
 
./prepareCrabJobs.py -C LegacyReReco-07Aug2017-2_6_1 -U 1 -L 25 -s campaigns/LegacyReReco-07Aug2017-2_6_1_list4.json -V 2_6_1 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask jsons/Recovery-LegacyReReco-07Aug2017-2_6_1-DoubleMuon-22Nov.json

./prepareCrabJobs.py -C LegacyReReco-07Aug2017-2_6_1 -U 1 -L 25 -s campaigns/LegacyReReco-07Aug2017-2_6_1.json -V 2_6_1 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask jsons/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt

./prepareCrabJobs.py -C LegacyReReco-07Aug2017-2_6_1 -U 1 -L 25 -s campaigns/LegacyReReco-07Aug2017-2_6_1_list2.json -V 2_6_1 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask jsons/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt

./prepareCrabJobs.py -C LegacyReReco-07Aug2017-2_6_1 -U 1 -L 25 -s campaigns/LegacyReReco-07Aug2017-2_6_1_list3.json -V 2_6_1 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask jsons/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt

./prepareCrabJobs.py -C LegacyReReco-07Aug2017-2_6_1 -U 1 -L 25 -s campaigns/LegacyReReco-07Aug2017-2_6_1_list4.json -V 2_6_1 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask jsons/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt

### 942 tests

./prepareCrabJobs.py -C RunIIFall17-2_7_7 -U 1 -L 25 -s campaigns/RunIIFall17-2_7_7.json -V 2_7_7 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask jsons/Cert_294927-306462_13TeV_PromptReco_Collisions17_JSON.txt

#### signal

./prepareCrabJobs.py -C RunIIFall17-2_8_0 -U 1 -L 25 -s campaigns/RunIIFall17-2_8_0.json -V 2_8_0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py

./prepareCrabJobs.py -C RunIIFall17-2_8_0 -U 1 -L 25 -s campaigns/RunIIFall17-2_8_0_part2.json -V 2_8_0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py

### 928 tests

./prepareCrabJobs.py -C RunIIFall17-2_7_2-test -U 1 -L 25 -s campaigns/RunIIFall17-2_7_2-test.json -V 2_7_2 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py

./prepareCrabJobs.py -C RunIIFall17-2_7_3-test -U 1 -L 25 -s campaigns/RunIIFall17-2_7_3-test.json -V 2_7_3 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py

./prepareCrabJobs.py -C RunIIFall17-2_7_4 -U 1 -L 25 -s campaigns/RunIIFall17-2_7_4.json -V 2_7_4 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py

./prepareCrabJobs.py -C RunIIFall17-2_7_5 -U 1 -L 25 -s campaigns/RunIIFall17-2_7_5.json -V 2_7_5 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py

#### adding extra trigger info

./prepareCrabJobs.py -C RunIIFall17-2_7_6 -U 1 -L 25 -s campaigns/RunIIFall17-2_7_6_DoubleEG.json -V 2_7_6 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask jsons/ToRun_DoubleEG_Nov23.json

./prepareCrabJobs.py -C RunIIFall17-2_7_6 -U 1 -L 25 -s campaigns/RunIIFall17-2_7_6_SingleElectron.json -V 2_7_6 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask jsons/ToRun_SingleElectron_Nov23.json


### 924 tests

RunIIFall16-2_7_1-test signals

./prepareCrabJobs.py -C RunIIFall16-2_7_1-test -U 1 -L 25 -s campaigns/RunIIFall16-2_7_1-test.json -V 2_7_1 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py


### First Phase1 Test (9_2_3_patch2)

./prepareCrabJobs.py -C RunIIPhaseISpring17-GGH-2_7_0-upgrade2017 -U 1 -L 25 -s campaigns/RunIIPhaseISpring17-GGH-2_7_0-upgrade2017.json -V 2_7_0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py

### First Legacy ReReco Test (8_0_28)

./prepareCrabJobs.py -C LegacyReReco-18Apr2017-2_6_0 -U 1 -L 25 -s campaigns/LegacyReReco-18Apr2017-2_6_0.json -V 2_6_0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask jsons/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt

./prepareCrabJobs.py -C LegacyReReco-18Apr2017-2_6_0 -U 1 -L 25 -s campaigns/LegacyReReco-18Apr2017-2_6_0-part2.json -V 2_6_0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask jsons/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt

### REMINIAOD with MET update

./prepareCrabJobs.py -C ReMiniAOD-03Feb2017-2_5_5 -U 1 -L 25 -s campaigns/ReMiniAOD-03Feb2017-2_5_5_list2.json -V 2_5_5 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask ${CMSSW_BASE}/src/flashgg/MetaData/work/jsons/DoubleEG_MetFix_missing13Mar.json

./prepareCrabJobs.py -C ReMiniAOD-03Feb2017-2_5_5 -U 1 -L 25 -s campaigns/ReMiniAOD-03Feb2017-2_5_5.json -V 2_5_5 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask ${CMSSW_BASE}/src/flashgg/MetaData/work/jsons/Cert_ReReco_notPrompt.json

Two mistakes in next command:
a. version is wrong (but ok campaign name is right, no problem)
b. Used PromptReco json, hence extra submission for 2016B above to catch the extra lumisections

./prepareCrabJobs.py -C ReMiniAOD-03Feb2017-2_5_4 -U 1 -L 25 -s campaigns/ReMiniAOD-03Feb2017-2_5_4.json -V 2_5_1 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask ${CMSSW_BASE}/src/flashgg/MetaData/work/jsons/Cert_271036-284044_13TeV_PromptReco_Collisions16_JSON.txt

### REMINIAOD

(example only of first run)

./prepareCrabJobs.py -C ReMiniAOD-03Feb2017-2_5_0-test -U 1 -L 25 -s campaigns/ReMiniAOD-03Feb2017-2_5_0-test.json -V 2_5_0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask /afs/cern.ch/work/s/sethzenz/fromscratch141/CMSSW_8_0_26_patch1/src/flashgg/MetaData/work/jsons/Cert_271036-284044_13TeV_PromptReco_Collisions16_JSON.txt

### SUMMER16 MC

#### 15 Jan regression

cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C RunIISummer16-2_4_2-25ns_Moriond17 -U 1 -L 25 -s campaigns/RunIISummer16-2_4_2-25ns_Moriond17.json -V 2_4_2 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py
cd RunIISummer16-2_4_2-25ns_Moriond17
echo crabConfig_*.py | xargs -n 1 crab sub

#### old regression

cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C RunIISummer16-2_4_1-25ns_Moriond17 -U 1 -L 25 -s campaigns/RunIISummer16-2_4_1-25ns_Moriond17.json -V 2_4_1 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py
cd RunIISummer16-2_4_1-25ns_Moriond17
echo crabConfig_*.py | xargs -n 1 crab sub


#### broken regression

cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C RunIISummer16-2_4_0-25ns_Moriond17 -U 1 -L 25 -s campaigns/RunIISummer16-2_4_0-25ns_Moriond17.json -V 2_4_0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py
cd RunIISummer16-2_4_0-25ns_Moriond17
echo crabConfig_*.py | xargs -n 1 crab sub


### SRING16 (80x)

#### 80X with DY including PDFs

cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C RunIISpring16DR80X-2_2_0-25ns_DYWithPDF -U 5 -L 25 -s campaigns/RunIISpring16DR80X-2_2_0-25ns_DYWithPDF.json -V 2_2_0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py
cd RunIISpring16DR80X-2_2_0-25ns_DYWithPDF 
echo crabConfig_*.py | xargs -n 1 crab sub

#### 80X MiniAODv2

Tags for flashgg: `2_2_0` and `RunIISpring16DR80X-2_2_0-25ns_ICHEP16_MiniAODv2`

```
cd $CMSSW_BASE/src/flashgg/MetaData/work
# split the campaign json file into sig and (data + bkg) to use different options for file splitting
./prepareCrabJobs.py -C RunIISpring16DR80X-2_2_0-25ns_ICHEP16_MiniAODv2 -U 1 -L 25 -s campaigns/RunIISpring16DR80X-2_2_0-25ns_ICHEP16_MiniAODv2_sig.json -V 2_2_0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask ${PWD}/jsons/json_DCSONLY_1467860596_for_ICHEP_MiniAODv2.txt
./prepareCrabJobs.py -C RunIISpring16DR80X-2_2_0-25ns_ICHEP16_MiniAODv2 -U 5 -L 25 -s campaigns/RunIISpring16DR80X-
2_2_0-25ns_ICHEP16_MiniAODv2_data_bkg.json -V 2_2_0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask ${PWD}/jsons/json_DCSONLY_1467860596_for_ICHEP_MiniAODv2.txt
cd RunIISpring16DR80X-2_2_0-25ns_ICHEP16_MiniAODv2
echo crabConfig_*.py | xargs -n 1 crab sub
```

#### 80X QGL update

Tags for flashgg: `2_1_0` and `RunIISpring16DR80X-2_1_0-25ns_ICHEP16`

```
cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C RunIISpring16DR80X-2_1_2-25ns_ICHEP16 -U 5 -L 25 -s campaigns/RunIISpring16DR80X-2_1_2-25ns_ICHEP16.json -V 2_1_2 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask ${PWD}/jsons/json_DCSONLY_1466185760.txt
cd RunIISpring16DR80X-2_1_2-25ns_ICHEP16
echo crabConfig_*.py | xargs -n 1 crab sub
```


#### 80X Pu Jet ID update 

Tags for flashgg: `2_1_0` and `RunIISpring16DR80X-2_1_0-25ns_ICHEP16`

```
cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C RunIISpring16DR80X-2_1_1-25ns_ICHEP16 -U 5 -L 25 -s campaigns/RunIISpring16DR80X-2_1_1-25ns_ICHEP16.json -V 2_1_1 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask ${PWD}/jsons/json_DCSONLY_1466185760.txt
cd RunIISpring16DR80X-2_1_1-25ns_ICHEP16
echo crabConfig_*.py | xargs -n 1 crab sub
```

#### 80X trainings on 80X

Example of top-up submission for data samples. The campaign is called p2
(part2) and the corresponding catalog will be merged to the main one as soon as
finished.
Tags for flashgg: `2_1_0` and `RunIISpring16DR80X-2_1_0-25ns_ICHEP16_p2`
Latest JSON available taken from the central CMS afs area: `json_DCSONLY_1467118709_for_ICHEPp2.txt`

```
cd $CMSSW_BASE/src/flashgg/MetaData/work
# make one campaign file per dataset:
#   campaigns/RunIISpring16DR80X-2_1_0-25ns_ICHEP16_{DoubleEG,SingleElectron,DoubleMuon,SingleMuon}.json
for dset in DoubleEG SingleElectron DoubleMuon SingleMuon
do
        fggManageSamples.py -C RunIISpring16DR80X-2_1_0-25ns_ICHEP16 getlumi /${dset}* output=${dset}_p1_done.json
        compareJSON.py --sub jsons/json_DCSONLY_1467118709_for_ICHEPp2.txt ${dset}_p1_done.json jsons/RunIISpring16DR80X-2_1_0-25ns_ICHEP16_p2_${dset}.json
mkdir orig ; mv crabConfig_*orig*.py orig
        ./prepareCrabJobs.py -C RunIISpring16DR80X-2_1_0-25ns_ICHEP16_p2 -U 5 -L 25 -s campaigns/RunIISpring16DR80X-2_1_0-25ns_ICHEP16_${dset}.json -V 2_1_0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask ${PWD}/jsons/RunIISpring16DR80X-2_1_0-25ns_ICHEP16_p2_${dset}.json
done
cd RunIISpring16DR80X-2_1_0-25ns_ICHEP16_p2
echo crabConfig_*.py | xargs -n 1 crab sub

for dset in DoubleEG SingleElectron DoubleMuon SingleMuon
do
        fggManageSamples.py -C RunIISpring16DR80X-2_1_0-25ns_ICHEP16 catimport RunIISpring16DR80X-2_1_0-25ns_ICHEP16_p2 "/${dset}*"
        fggManageSamples.py -C RunIISpring16DR80X-2_1_0-25ns_ICHEP16 overlap "/${dset}*"
done
```

Tags for flashgg: `2_1_0` and `RunIISpring16DR80X-2_1_0-25ns_ICHEP16`

```
cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C RunIISpring16DR80X-2_1_0-25ns_ICHEP16 -U 5 -L 25 -s campaigns/RunIISpring16DR80X-2_1_0-25ns_ICHEP16.json -V 2_1_0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask ${PWD}/jsons/json_DCSONLY_1466185760.txt
cd RunIISpring16DR80X-2_1_0-25ns_ICHEP16
python $CMSSW_BASE/src/flashgg/MetaData/scripts/stupid_fix.py
mkdir orig ; mv crabConfig_*orig*.py orig
echo crabConfig_*.py | xargs -n 1 crab sub
```

#### 76X trainings on 80X, 2_0_0

```
cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C RunIISpring16DR80X-2_0_0-25ns -U 5 -L 25 -s campaigns/RunIISpring16DR80X-2_0_0-25ns.json -V 2_0_0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask ${PWD}/jsons/json_DCSONLY.txt
cd RunIISpring16DR80X-2_0_0-25ns/
python $CMSSW_BASE/src/flashgg/MetaData/scripts/stupid_fix.py
mkdir orig ; mv crabConfig_*orig*.py orig
echo crabConfig_*.py | xargs -n 1 crab sub
```

### FALL15 (76x)

#### First tests with 1_3_1

```
cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C RunIIFall15DR76-1_3_0-25ns_ext1 -U 5 -L 25 -s campaigns/RunIIFall15DR76-1_3_0-25ns_ext1.json -V 1_3_0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask ${PWD}/jsons/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_Silver_v2.txt
cd RunIIFall15DR76-1_3_0-25ns_ext1/
python $CMSSW_BASE/src/flashgg/MetaData/scripts/stupid_fix.py
mkdir orig ; mv crabConfig_*orig*.py orig
echo crabConfig_*.py | xargs -n 1 crab sub
```

#### First tests with 1_3_0 

```
cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C RunIIFall15DR76-1_3_0-25ns -U 5 -L 25 -s campaigns/RunIIFall15DR76-1_3_0-25ns.json -V 1_3_0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py
cd RunIIFall15DR76-1_3_0-25ns  --lumiMask ${PWD}/jsons/SilverNotGold_246908-260627_JSON_v2.txt
python $CMSSW_BASE/src/flashgg/MetaData/scripts/stupid_fix.py
mkdir orig ; mv crabConfig_*orig*.py orig
echo crabConfig_*.py | xargs -n 1 crab sub
```

### SPRING15

### 1_2_0 Missing ReReco DoubleEG ###

```
cd $CMSSW_BASE/src/flashgg/MetaData/work/jsons
fggManageSamples.py -C RunIISpring15-ReReco74X-All-1_1_0-Or-1_2_0-25ns getlumi "/DoubleEG/*2015*/*" output=DoubleEG_ReRecoAsRun_29Jan.json
compareJSON.py --sub Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_Silver_v2.txt DoubleEG_ReRecoAsRun_29Jan.json > DoubleEG_ToRun_29Jan.json

cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C RunIISpring15-ReReco74XMissingDoubleEG-1_2_0-25ns -s campaigns/RunIISpring15-ReReco74XMissingDoubleEG-1_2_0-25ns.json  -V 1_2_0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask ${PWD}/jsons/DoubleEG_ToRun_29Jan.json
cd RunIISpring15-ReReco74XMissingDoubleEG-1_2_0-25ns
echo crabConfig_*.py | xargs -n 1 crab sub
```


### 1_2_0 Missing ReReco SingleElectron ###

```
cd $CMSSW_BASE/src/flashgg/MetaData/work/jsons
fggManageSamples.py -C RunIISpring15-ReReco74X-All-1_1_0-Or-1_2_0-25ns getlumi "/SingleElectron/*2015*/*" output=SingleElectron_ReRecoAsRun_29Jan.json
compareJSON.py --sub Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_Silver_v2.txt SingleElectron_ReRecoAsRun_29Jan.json > SingleElectron_ToRun_29Jan.json

cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C RunIISpring15-ReReco74XMissingSingleElectron-1_2_0-25ns -s campaigns/RunIISpring15-ReReco74XMissingSingleElectron-1_2_0-25ns.json  -V 1_2_0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask ${PWD}/jsons/SingleElectron_ToRun_29Jan.json
cd RunIISpring15-ReReco74XMissingSingleElectron-1_2_0-25ns
echo crabConfig_*.py | xargs -n 1 crab sub
```

### 1_2_0 ReMiniAOD missing signal samples

```
cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C RunIISpring15-MoreSignal-1_2_0-25ns -U 1 -L 25 -s campaigns/RunIISpring15-MoreSignal-1_2_0-25ns.json -V 1_2_0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py
cd RunIISpring15-MoreSignal-1_2_0-25ns
python $CMSSW_BASE/src/flashgg/MetaData/scripts/stupid_fix.py
mkdir orig ; mv crabConfig_*orig*.py orig
echo crabConfig_*.py | xargs -n 1 crab sub
```


#### 1_2_0 ReMiniAOD TTH Filtered Rerun

Should be 1_1_0 compatible

```
cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C RunIISpring15-TTHFiltered-1_2_0-25ns -U 5 -L 25 -s campaigns/RunIISpring15-TTHFiltered-1_2_0-25ns.json -V 1_2_0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py
cd RunIISpring15-TTHFiltered-1_2_0-25ns
python $CMSSW_BASE/src/flashgg/MetaData/scripts/stupid_fix.py 
mkdir orig ; mv crabConfig_*orig*.py orig
echo crabConfig_*.py | xargs -n 1 crab sub
```

#### 1_1_0 2015D DoubleEG ReReco ###

```
cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C RunIISpring15-ReReco74X-1_1_0-25ns -U 5 -L 25 -s campaigns/RunIISpring15-ReReco74X-1_1_0-25ns.json -V 1_1_0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask ${PWD}/jsons/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_v2.txt
cd RunIISpring15-ReReco74X-1_1_0-25ns
echo crabConfig_*.py | xargs -n 1 crab sub
```

#### 1_1_0 Prompt 25ns MuonEG Rerun ###

For borked jobs:

```
cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C RunIISpring15-Prompt-MuonEGRerun-1_1_0-25ns -U 5 -L 25 -s campaigns/RunIISpring15-Prompt-MuonEGRerun-1_1_0-25ns.json -V 1_1_0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask ${PWD}/jsons/MuonEG_missing.txt
cd RunIISpring15-Prompt-MuonEGRerun-1_1_0-25ns
echo crabConfig_*.py | xargs -n 1 crab sub
```


#### 1_1_0 on Silver-Only Prompt 25ns

Excludes previous processings via JSON subtraction

```
cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C RunIISpring15-Prompt-SilverNotGold-1_1_0-25ns -U 5 -L 25 -s campaigns/RunIISpring15-Prompt-SilverNotGold-1_1_0-25ns.json -V 1_1_0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask ${PWD}/jsons/ToRun_Prompt_SilverNotGold_2015D_246908-260627_JSON_v2.txt
cd RunIISpring15-Prompt-SilverNotGold-1_1_0-25ns
echo crabConfig_*.py | xargs -n 1 crab sub
```

#### 1_1_0 on Silver-Only ReMiniAOD 25ns

Excludes previous processings via JSON subtraction

```
cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C RunIISpring15-ReMiniAOD-SilverNotGold-1_1_0-25ns -U 5 -L 25 -s campaigns/RunIISpring15-ReMiniAOD-SilverNotGold-1_1_0-25ns.json -V 1_1_0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask ${PWD}/jsons/SilverNotGold_246908-260627_JSON_v2.txt
cd RunIISpring15-ReMiniAOD-SilverNotGold-1_1_0-25ns
echo crabConfig_*.py | xargs -n 1 crab sub
```


#### 1_1_0 on Prompt 25ns

Excludes ReMiniAOD 25ns via JSON subtraction

```
cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C RunIISpring15-Prompt-1_1_0-25ns -U 5 -L 25 -s campaigns/RunIISpring15-Prompt-1_1_0-25ns.json -V 1_1_0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask ${PWD}/jsons/ToRun_Prompt_2015D_246908-260627_JSON_v2.txt
cd RunIISpring15-Prompt-1_1_0-25ns
echo crabConfig_*.py | xargs -n 1 crab sub
```


#### 1_1_0 ReMiniAOD Gold

```
cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C RunIISpring15-ReMiniAOD-1_1_0-25ns -U 5 -L 25 -s campaigns/RunIISpring15-ReMiniAOD-1_1_0-25ns.json -V 1_1_0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask jsons/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_v2.txt
cd RunIISpring15-ReMiniAOD-1_1_0-25ns
python $CMSSW_BASE/src/flashgg/MetaData/scripts/stupid_fix.py 
mkdir orig ; mv crabConfig_*orig*.py orig
echo crabConfig_*.py | xargs -n 1 crab sub
```

#### 1_0_0 on Silver-Only Prompt 25ns

Excludes previous processings via JSON subtraction

```
cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C RunIISpring15-Prompt-SilverNotGold-1_0_0-25ns -U 5 -L 25 -s campaigns/RunIISpring15-Prompt-SilverNotGold-1_0_0-25ns.json -V 1_0_0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask ${PWD}/jsons/ToRun_Prompt_SilverNotGold_2015D_246908-260627_JSON_v2.txt
cd RunIISpring15-Prompt-SilverNotGold-1_0_0-25ns
echo crabConfig_*.py | xargs -n 1 crab sub
```

#### 1_0_0 on Silver-Only ReMiniAOD 25ns

Excludes previous processings via JSON subtraction

```
cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C RunIISpring15-ReMiniAOD-SilverNotGold-1_0_0-25ns -U 5 -L 25 -s campaigns/RunIISpring15-ReMiniAOD-SilverNotGold-1_0_0-25ns.json -V 1_0_0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask ${PWD}/jsons/SilverNotGold_246908-260627_JSON_v2.txt
cd RunIISpring15-ReMiniAOD-SilverNotGold-1_0_0-25ns
echo crabConfig_*.py | xargs -n 1 crab sub
```

#### 1_0_0 on Prompt 25ns

Excludes ReMiniAOD 25ns via JSON subtraction

```
cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C RunIISpring15-Prompt-1_0_0-25ns -U 5 -L 25 -s campaigns/RunIISpring15-Prompt-1_0_0-25ns.json -V 1_0_0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask ${PWD}/jsons/ToRun_Prompt_2015D_246908-260627_JSON_v2.txt
cd RunIISpring15-Prompt-1_0_0-25ns
echo crabConfig_*.py | xargs -n 1 crab sub
```


#### 1_0_0 ReMiniAOD Gold

```
cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C RunIISpring15-ReMiniAOD-1_0_0-25ns -U 5 -L 25 -s campaigns/RunIISpring15-ReMiniAOD-1_0_0-25ns.json -V 1_0_0 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask jsons/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_v2.txt
cd RunIISpring15-ReMiniAOD-1_0_0-25ns
python $CMSSW_BASE/src/flashgg/MetaData/scripts/stupid_fix.py # to shorten job names >100 chars (new crab "feature")
mkdir orig ; mv crabConfig_*orig*.py orig
echo crabConfig_*.py | xargs -n 1 crab sub
```

#### ChristmasTest

```
cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C ChristmasTest -U 5 -L 25 -s campaigns/ChristmasTest.json -V ChristmasTestV2 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask jsons/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON.txt
cd ChristmasTest
python $CMSSW_BASE/src/flashgg/MetaData/scripts/stupid_fix.py # to shorten job names >100 chars (new crab "feature")
mkdir orig ; mv crabConfig_*orig*.py orig
echo crabConfig_*.py | xargs -n 1 crab sub
```

#### Spring15BetaV7 extension of Gold prompt to end of run

```
cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C RunIISpring15-FinalPrompt-BetaV7-25ns -U 5 -L 25 -s campaigns/RunIISpring15-FinalPrompt-BetaV7-25ns.json -V Spring15BetaV7 -p ${CMSSW_BAS\
E}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask jsons/ToRun_FinalPrompt_2015D_JSON.txt
cd RunIISpring15-Prompt-FinalPrompt-BetaV7-25ns
echo crabConfig_*.py | xargs -n 1 crab sub
```

#### Spring15BetaV7 on Silver-Only Prompt 25ns

Excludes ReMiniAOD and prompt Gold via JSON subtraction.  Only one run in this campaign!

```
cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C RunIISpring15-Prompt-SilverNotGold-BetaV7-25ns -U 5 -L 25 -s campaigns/RunIISpring15-Prompt-SilverNotGold-BetaV7-25ns.json -V Spring15BetaV7 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask ${PWD}/jsons/ToRun_Prompt_2015D_SilverNotGold_JSON.txt
cd RunIISpring15-Prompt-SilverNotGold-BetaV7-25ns
echo crabConfig_*.py | xargs -n 1 crab sub
```

#### Spring15BetaV7 on Silver-Only ReMiniAOD 25ns

Excludes previous processings via JSON subtraction

```
cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C RunIISpring15-ReMiniAOD-SilverNotGold-BetaV7-25ns -U 5 -L 25 -s campaigns/RunIISpring15-ReMiniAOD-SilverNotGold-BetaV7-25ns.json -V Spring15BetaV7 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask ${PWD}/jsons/Cert_246908-259891_SilverNotGold_JSON.txt
cd RunIISpring15-ReMiniAOD-SilverNotGold-BetaV7-25ns
echo crabConfig_*.py | xargs -n 1 crab sub
```

#### Spring15BetaV7 on Prompt 25ns

Excludes ReMiniAOD 25ns via JSON subtraction

```
cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C RunIISpring15-Prompt-BetaV7-25ns -U 5 -L 25 -s campaigns/RunIISpring15-Prompt-BetaV7-25ns.json -V Spring15BetaV7 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask ${PWD}/jsons/ToRun_Prompt_2015D_258159-259891_JSON.txt
cd RunIISpring15-Prompt-BetaV7-25ns
echo crabConfig_*.py | xargs -n 1 crab sub
```

#### Spring15BetaV7 on ReMiniAOD 25ns

```
cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C RunIISpring15-ReMiniAOD-BetaV7-25ns -U 5 -L 25 -s campaigns/RunIISpring15-ReMiniAOD-BetaV7-25ns.json -V Spring15BetaV7 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask ${PWD}/jsons/Cert_246908-258750_13TeV_PromptReco_Collisions15_25ns_JSON.txt
cd RunIISpring15-ReMiniAOD-BetaV7-25ns
echo crabConfig_*.py | xargs -n 1 crab sub
```

#### Spring15BetaV6 on ReMiniAOD 25ns

```
cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C RunIISpring15-ReMiniAOD-BetaV6-25ns -U 1 -L 5 -s campaigns/RunIISpring15-ReMiniAOD-BetaV6-25ns.json -V Spring15BetaV6 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions15/13TeV/Cert_246908-258159_13TeV_PromptReco_Collisions15_25ns_JSON_v3.txt
cd RunIISpring15-ReMiniAOD-BetaV6-25ns
echo crabConfig_*.py | xargs -n 1 crab sub
```

#### Spring15BetaV5 25ns

```
cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C RunIISpring15-25ns -U 1 -L 5 -s campaigns/RunIISpring15-25ns.json -V Spring15BetaV5 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions15/13TeV/Cert_246908-255031_13TeV_PromptReco_Collisions15_25ns_JSON_v2.txt
cd RunIISpring15-25ns
echo crabConfig_*.py | xargs -n 1 crab sub
```

```
cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C RunIISpring15-25ns -U 1 -L 5 -s campaigns/RunIISpring15-25ns-2015D-DCSonly.json -V Spring15BetaV5 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions15/13TeV/DCSOnly/json_DCSONLY.txt
cd RunIISpring15-25ns
echo crabConfig_*2015D*.py | xargs -n 1 crab sub
```

#### Spring15BetaV5 50ns

```
cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C RunIISpring15-50ns -U 1 -L 5 -s campaigns/RunIISpring15-50ns.json -V Spring15BetaV5 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions15/13TeV/Cert_246908-255031_13TeV_PromptReco_Collisions15_50ns_JSON_v2.txt
cd RunIISpring15-50ns
echo crabConfig_*.py | xargs -n 1 crab sub
```

```
cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C RunIISpring15-50ns -U 1 -L 5 -s campaigns/RunIISpring15-50ns-2015C.json -V Spring15BetaV5 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions15/13TeV/Cert_254833_13TeV_PromptReco_Collisions15_JSON.txt
cd RunIISpring15-50ns
echo crabConfig_*2015C*.py | xargs -n 1 crab sub
```


#### Spring15BetaV4

````
cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C RunIISpring15-50ns -U 1 -L 50 -s campaigns/RunIISpring15-50ns.json -V Spring15BetaV4 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions15/13TeV/Cert_246908-251883_13TeV_PromptReco_Collisions15_JSON_v2.txt
cd RunIISpring15-50ns
echo crabConfig_*.py | xargs -n 1 crab sub
```

#### Spring15BetaV3

```
cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C RunIISpring15-50ns -U 5 -s campaigns/RunIISpring15-50ns.json -V Spring15BetaV3 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py --lumiMask https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions15/13TeV/Cert_246908-251883_13TeV_PromptReco_Collisions15_JSON_v2.txt
cd RunIISpring15-50ns
echo crabConfig_*.py | xargs -n 1 crab sub
```

#### Further fixes and configuration tweaks for smooth running

```
cd $CMSSW_BASE/src/flashgg/MetaData/work
./prepareCrabJobs.py -C RunIISpring15-50ns -U 1 -s campaigns/RunIISpring15-50ns.json -V Spring15BetaV2 -p ${CMSSW_BASE}/src/flashgg/MicroAOD/test/microAODstd.py
cd RunIISpring15-50ns
echo crabConfig_*.py | xargs -n 1 crab sub
```

#### CMSSW 7_4_6_patch2, no recomputation of cluster shapes

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
Note: make sure to run `cmsenv` before executing any of these commands.  If crab was sourced during the setup, make sure to re-run `cmsenv` as you will get errors otherwise. 

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

   *Note*: By default the `import` command imports all datasets matching the pattern `/*/*<campaing>-<fgg_version>*/*`. This can be changed explicitely specifying the patterns to be imported, e.g.:
   `fggManangeSamples.py -C <campaing>_50ns -S /*/*<campaign>*50ns*/*`
   `fggManangeSamples.py -C <campaing>_20ns -S /*/*<campaign>*20ns*/*`
   
   *Note 2*: Datasets that are submitted or finish at a later stage can be imported in arleady existing catalogs.
   If a datataset with the same name already exists, the list of files will be merged (the `review` and `check` steps need to be re-run`).
 
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

   *Note*: For large catalogs, it may be better to run the submit the check command to the batch system using the `-q <queu_name>`  (8nm on lxbatch would suffice) and also to check different datasets in different steps.
   Furthermore, the check for duplicates can be decoupled from the actual file-by-file check. This can be achieved with the `checklite` command.
   `fggManageSamples.py -C <campaing> checklite`
   `fggManageSamples.py -C <campaing> -q 8nm check /DY*`
   `fggManageSamples.py -C <campaing> -q 8nm check /QCD*`
   `fggManageSamples.py -C <campaing> -q 8nm check /DiPhoton*`
   `fggManageSamples.py -C <campaing> -q 8nm check /*/*Prompt*`

   In general, it is best not to deal with more than 1000 files for each `check` run.

   *Note 2*: If the check steps hangs for some reason (and the jobs were run on the batch system), the ouput of the partially completed jobs can be recovered in a subsequent run.
`fggManageSamples.py -C <campaing> -c check /DY*`

   This command will not submit any new jobs but just integrate the output of the successful ones into the catalog.    An additional call to:
   `fggManageSamples.py -C <campaing> -q 8nm check /DY*`
   Will submit the missing jobs.

   *Note 3*: By default files or datasets that were not already checked are not included in subsequent runs of the `check` and `checklite` commands. To change this behaviour one can use the `--force` option.

   *Note 4*: If the weights for a dataset are 0, the job splitting is done only base on the number of events.  If any of the files listed in the dataset are non-0, then only those with non-0 weights will be used.

   *Note 5*: If you received an error during the `check` step: `[Errno 2] No such file or directory: u'.tmpxxxxxx.json'` rerun `cmsenv` and retry the fggManageSamples.py commands.


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

    *Note: If the cross-section is set to 0, the dataset is assumed to be data.

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

##### Per-dataset specific options

Sometimes, it may be useful to change the running options for some specific datasets. This can be done specifying the dataset as follows:

```
"processes":  {
	      "DYJetsToLL_M-50_TuneCUETP8M1-amcatnloFXFX-pythia8" : [
                                        ["/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8",{"args" : ["maxEvents=1000000"],"njobs":10}]
                ]
}
```

With this mechanism, the number of jobs to be used can be specified on a dataset-by-dataset basis. Also, command line arguments can be added or replaced.


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

In case the pset output filename defined in the analysis code is not using TFile service the python variable used to store the filename can be loaded using customization.

customize.tfileOut = ("module_name","python_variable_for_output_filename")

In case of errors, a full dump of the options loaded and full dump of the pset is sent to the -d directory defined.  This is very useful for debugging errors.

### Resuming jobs monitoring

By default the `fggRunJobs.py` script waits for the submitted jobs to finish and then merges the output of all jobs in one single file.

If jobs are submitted to the batch system (with the `-q <queue_id>` option) in asyncronouse mode (on by default) the monitoring of the jobs can be stopped and resumed.
To stop the monitoring one can simply stop the process e.g. with `Control-C`.

After that, the status of the submission can be checked with the `--summary` command:
`fggRunJobs.py --load <task_folder>/config.json --summary`.

To resume the task monitoring with 
`fggRunJobs.py --load <task_folder>/config.json --cont`.
