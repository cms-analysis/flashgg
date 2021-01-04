Table of Contents
=================

   * [General Info](#general-info)
   * [Summary workflow of this framework](#summary-workflow-of-this-framework)
   * [Setting Up HHWWgg_dev Repository](#setting-up-hhwwgg_dev-repository)
      * [Setting up a voms Proxy](#setting-up-a-voms-proxy)
      * [HHWWgg Tagger](#hhwwgg-tagger)
         * [Running Locally](#running-locally)
         * [Running on Condor](#running-on-condor)
            * [Example: 2017 Data / MC Variables](#example-2017-data--mc-variables)
            * [Resubmit failed jobs](#resubmit-failed-jobs)
      * [nTuple Processing](#ntuple-processing)
         * [Workspaces](#workspaces)
            * [Hadd Signal](#hadd-signal)
            * [Hadd Data](#hadd-data)
         * [Trees](#trees)
            * [Hadd MC Background](#hadd-mc-background)
      * [Rename Workspace](#rename-workspace)
   * [Examples](#examples)
      * [2018 Semi-Leptonic SM NLO](#2018-semi-leptonic-sm-nlo)
   * [Few Important Things To Note Before running the framework](#few-important-things-to-note-before-running-the-framework)

Created by [gh-md-toc](https://github.com/ekalinin/github-markdown-toc)

# General Info

Contacts:
- Petr Mandrik - petr.mandrik@cern.ch 
- Abraham Tishelman-Charny - abraham.tishelman.charny@cern.ch
- Jin Wang - Jin.Wang@cern.ch
- Ram Krishna Sharma - ram.krishna.sharma@cern.ch
- Chu Wang - chuw@cern.ch

Presentations:
- [8 September 2020 Non-Res Plans](https://indico.cern.ch/event/944262/contributions/4000298/attachments/2098804/3528297/8_September_2020_HH_WWgg_NonResUpdate.pdf)
- [29 June 2020 Analysis Update](https://indico.cern.ch/event/922765/contributions/3915475/attachments/2065622/3466480/29_June_2020_HHWWgg_Status.pdf)

Repositories:
- [HHWWgg Flashgg Final Fit](https://github.com/atishelmanch/flashggFinalFit/tree/HHWWgg_Dev_runII_102x)
- [HHWWgg MicroAOD Production](https://github.com/atishelmanch/flashgg/tree/HHWWgg_Crab)
- [HHWWgg Analysis Tools](https://github.com/NEUAnalyses/HHWWgg_Tools/tree/master)

These instructions describe how to run flashgg modules specific to the `HH->WWgg` analysis. The HHWWgg Tagger plugin, HHWWggTagProducer, is designed to work with workspaceStd.

# Summary workflow of this framework

1. You should have the MicroAOD
2. Prepare the campaign
3. Add cross-section value in the cross-section.json
4. Check running interactive
5. Condor job submission
  1. Before moving to next step check if all jobs processed successfully or not?
  2. If any job filed resubmit them.
  2. Wait for all jobs to finish
6. Hadd the outputs using hadd script.
7. Finally use the flashggfinalfit for limit extraction.

# Setting Up HHWWgg_dev Repository

The HHWWgg development branch is obtained in a similar fasion to the `dev_legacy_runII` branch:

```bash
export SCRAM_ARCH=slc7_amd64_gcc700
cmsrel CMSSW_10_6_8
cd CMSSW_10_6_8/src
cmsenv
git cms-init
cd $CMSSW_BASE/src 
git clone -b HHWWgg_dev https://github.com/atishelmanch/flashgg.git
source flashgg/setup_flashgg.sh
```

If everything now looks reasonable, you can build:

```bash
cd $CMSSW_BASE/src
scram b -j 4
```

Depending on the status of the classes compared to the version of the classes_def.xml file, you may receive an error like:

```bash
error: class 'flashgg::HHWWggTag' has a different checksum for ClassVersion 39. Increment ClassVersion to 40 and assign it to checksum 1253491559
```

This can be fixed by updating the class version like so:

```bash
cd flashgg
scram build updateclassversion 
mv DataFormats/src/classes_def.xml.generated 
DataFormats/src/classes_def.xml
scram b -j
```

## Setting up a voms Proxy

To access grid files to run the tagger on, you must run the following commands:

```bash
cmsenv
voms-proxy-init --voms cms --valid 168:00
```

after the voms command, you should receive an output similar to:

    Created proxy in /tmp/x509up_u95168

to set this proxy to your `X509_USER_PROXY` environment variable for the example above, simply use the command:

```bash
cd $CMSSW_BASE/src/flashgg
. MetaData/scripts/proxy.sh x509up_u95168
```

where `x590up_u95168` would be replaced by whatever your proxy name is.

## HHWWgg Tagger

The HHWWgg tagger is developed to tag events as coming from the HH->WWgg process, and is compatible with workspaceStd in order to include the standard systematics workflow,
and if desired to include tagging of other flashgg tags on the same events.

### Running Locally

You can try the HHWWgg Tagger locally on the SM HH->WWgg->qqlnugg **signal (with 2017 metaConditions)** with:

```bash
cmsRun Systematics/test/workspaceStd.py metaConditions=MetaData/data/MetaConditions/Era2017_RR-31Mar2018_v1-HHWWgg.json campaign=HHWWgg_SM2017 dataset=GluGluToHHTo_WWgg_qqlnu_nodeSM doHHWWggTag=1 HHWWggTagsOnly=1 maxEvents=500 doSystematics=0 dumpWorkspace=0 dumpTrees=1 useAAA=1 doHHWWggTagCutFlow=1 saveHHWWggFinalStateVars=1 HHWWggAnalysisChannel=SL HHWWgguseZeroVtx=1
```

```bash
cmsRun Systematics/test/workspaceStd.py metaConditions=MetaData/data/MetaConditions/Era2017_RR-31Mar2018_v1-HHWWgg.json campaign=Era2017_RR-31Mar2018_v2 dataset=GluGluToHHTo2G2Qlnu_node_cHHH1_TuneCP5_PSWeights_13TeV-powheg-pythia8 doHHWWggTag=1 HHWWggTagsOnly=1 maxEvents=500 doSystematics=1 dumpWorkspace=0 dumpTrees=1 useAAA=1 doHHWWggTagCutFlow=1 saveHHWWggFinalStateVars=1 HHWWggAnalysisChannel=SL HHWWgguseZeroVtx=1 doHHWWggDebug=1
```

and on **2016 data**:

```bash
cmsRun Systematics/test/workspaceStd.py metaConditions=MetaData/data/MetaConditions/Era2016_RR-17Jul2018_v1.json campaign=Era2016_RR-17Jul2018_v2 dataset=/DoubleEG/spigazzi-Era2016_RR-17Jul2018_v2-legacyRun2FullV1-v0-Run2016B-17Jul2018_ver2-v1-86023db6be00ee64cd62a3172358fb9f/USER doHHWWggTag=1 HHWWggTagsOnly=1 maxEvents=500 doSystematics=0 dumpWorkspace=0 dumpTrees=1 useAAA=1 processId=Data processType=Data doHHWWggTagCutFlow=1 saveHHWWggFinalStateVars=1 HHWWggAnalysisChannel=SL HHWWgguseZeroVtx=1
```

and on **2017 data**:

```bash
cmsRun Systematics/test/workspaceStd.py metaConditions=MetaData/data/MetaConditions/Era2017_RR-31Mar2018_v1-HHWWgg.json campaign=Era2017_RR-31Mar2018_v2 dataset=/DoubleEG/spigazzi-Era2017_RR-31Mar2018_v2-legacyRun2FullV1-v0-Run2017B-31Mar2018-v1-d9c0c6cde5cc4a64343ae06f842e5085/USER doHHWWggTag=1 HHWWggTagsOnly=1 maxEvents=500 doSystematics=0 dumpWorkspace=0 dumpTrees=1 useAAA=1 processId=Data processType=Data doHHWWggTagCutFlow=1 saveHHWWggFinalStateVars=1 HHWWggAnalysisChannel=SL HHWWgguseZeroVtx=1 
```

and on **2018 data**:

```bash
cmsRun Systematics/test/workspaceStd.py metaConditions=MetaData/data/MetaConditions/Era2018_RR-17Sep2018_v1.json campaign=Era2018_RR-17Sep2018_v2 dataset=/EGamma/spigazzi-Era2018_RR-17Sep2018_v2-legacyRun2FullV2-v0-Run2018A-17Sep2018-v2-dc8e5fb301bfbf2559680ca888829f0c/USER doHHWWggTag=1 HHWWggTagsOnly=1 maxEvents=509 doSystematics=0 dumpWorkspace=0 dumpTrees=1 useAAA=1 processId=Data processType=Data doHHWWggTagCutFlow=1 saveHHWWggFinalStateVars=1 HHWWggAnalysisChannel=SL HHWWgguseZeroVtx=1
```

All flags are either defined in `MetaData/python/JobConfig.py`, or `Systematics/test/workspaceStd.py`.

An explanation of the flags in this example:
- **metaConditions**: A json file of tags and conditions defined for each year. In this example, 2017 conditions is specified, used to run with correct conditions on 2017 Data and MC.
- **campaign**: The flashgg campaign where the files you want to run on are defined.
- **dataset**: The dataset within the specified campaign where the files you want to run on are defined.
- **doHHWWggTag**: Setting this flag to 1 tells the workspaceStd flow to evaluate each event with the HHWWgg Tagger.
- **HHWWggTagsOnly**: This flag removes all taggers other than the HHWWgg Tagger, mainly all of the `Higgs->gg` taggers.
- **doSystematics**: In this example set to 0. If set to 1, the workspaceStd systematics flow is included where systematic labels are defined in workspaceStd. For each systematic, the tagger is rerun on the microAOD where the systematic quantity is either varied up or down one sigma. If you run with this flag on, there should be a tree (if running with dumpTrees) or a RooDataHist (if running with dumpWorkspace) for each systematic variation.
- **maxEvents**: Maximum events to run over in the specified dataset. Set to -1 to run on all events.
- **dumpWorkspace**: Save RooWorkspace in output file. Useful for input into [fggfinalfit](https://github.com/atishelmanch/flashggFinalFit).
    - When you run with `dumpWorkspace` do not add too many variables. If too many variables added then it will give you the error while hadding the workspace.
- **dumpTrees**: Save tree(s) in output file. Useful for running ntuple analysis afterwards.
- **useAAA**: Use prefix: `root://cms-xrd-global.cern.ch/` when looking for files.
- **processId / processType**: Set to "`Data`" when running on data.
- **doHHWWggTagCutFlow**: Categorize all events that pass pre-selection into HHWWgg categories. Without this flag, events that do not pass all analysis selections are cut.
- **saveHHWWggFinalStateVars**: Save many final state variables such as kinematics for leptons and jets before and after analysis level selections. Variables are defined in [Systematics/python/HHWWggCustomize.py](https://github.com/atishelmanch/flashgg/blob/a7da39035c95cfe3f94b8aa6a428c5811e7dbc59/Systematics/python/HHWWggCustomize.py)
- **HHWWggAnalysisChannel**: The HHWWgg channel analysis to run. This determines the possible categories events can fall into. Options are SL (Semi-Leptonic), FL (Fully-Leptonic) and FH (Fully-Hadronic). By default, if no value is specified, SL is used. NOTE that the untagged category will OVERLAP if combined with other analysis channels.
- **HHWWgguseZeroVtx**: When running over diphoton candidates, checking if they pass preselection, only consider diphoton candidates produced with respect to the Zeroeth vertex. Note that if this is set to false, the standard Hgg vertex will be used for diphoton candidate reconstruction.

With the options specified in the example, if this works properly, you should get an output file named: `output_numEvent500.root` containing a tree for each `HHWWggTag`.

The customization for the HHWWggTag class is defined in a few places, starting with [Systematics/python/HHWWggCustomize.py](https://github.com/atishelmanch/flashgg/blob/a7da39035c95cfe3f94b8aa6a428c5811e7dbc59/Systematics/python/HHWWggCustomize.py). In this python module you can specify variables to save, and the number of categories to save HHWWggTag objects in. The selections are located in
[Taggers/plugins/HHWWggTagProducer.cc](https://github.com/atishelmanch/flashgg/blob/a7da39035c95cfe3f94b8aa6a428c5811e7dbc59/Taggers/plugins/HHWWggTagProducer.cc). For the moment, a tag object "`tag_obj`" is created if an event has a **diphoton**, **exactly one good lepton**, corresponding
to the leptonically decaying W boson, **and at least two 'good' jets**, corresponding to the hadronically decaying W boson. For these objects, '`good`' is defined by the selections specified in
[Taggers/python/flashggHHWWggTag_cfi.py](https://github.com/atishelmanch/flashgg/blob/a7da39035c95cfe3f94b8aa6a428c5811e7dbc59/Taggers/python/flashggHHWWggTag_cfi.py). This tag object can be created and placed into one of three categories:

- **HHWWggTag_0**: Semileptonic electron final state (qqlnugg with l = electron)
- **HHWWggTag_1**: Semileptonic muon final state (qqlnugg with l = muon)
- **HHWWggTag_2**: Fully hadronic state (qqqqgg) (jets selected based on min Mass, or 4 highest pT jets if flag doHHWWggFHptOrdered=1 is set)
- **HHWWggTag_3**: Fully Leptonic final state (lnulnugg)
- **HHWWggTag_4**: Untagged (only used if doHHWWggTagCutFlow=1)

Note that the untagged category is only filled if you are running with the flag `doHHWWggTagCutFlow=1`. To add another category, the number of categories
specified in [Systematics/python/HHWWggCustomize.py](https://github.com/atishelmanch/flashgg/blob/a7da39035c95cfe3f94b8aa6a428c5811e7dbc59/Systematics/python/HHWWggCustomize.py) should be changed like so: `self.tagList = [ ["HHWWggTag",4] ] -> self.tagList = [ ["HHWWggTag",5] ]`. Then, when saving a tag object
of the new category, you would do so in [Taggers/plugins/HHWWggTagProducer.cc](https://github.com/atishelmanch/flashgg/blob/a7da39035c95cfe3f94b8aa6a428c5811e7dbc59/Taggers/plugins/HHWWggTagProducer.cc) with `tag_obj.setCategoryNumber( 3 )` rather than `tag_obj.setCategoryNumber( catNum )` where catNum = 0, 1, or 2.

### Running on Condor

When running over entire datasets, it's useful to submit condor jobs instead of running locally. This is done with the script [HHWWgg_Run_Jobs.sh](https://github.com/atishelmanch/flashgg/blob/a7da39035c95cfe3f94b8aa6a428c5811e7dbc59/HHWWgg_Run_Jobs.sh).

**Note** : You must first follow the proxy steps [above](#setting-up-a-voms-proxy) in order to have access to DAS datasets.

**Note** : There are one user specific parameters in the script: `fggDirec` , which are by default set to `${PWD}`. 

```bash
    fggDirec="$PWD/" # flashgg directory
```

- `fggDirec`: Your current working directory where you have flashgg cloned.
<!-- - `ntupleDirec`: The directory where you want your output files to go. Note that this is the directory where a directory will be created for each batch of jobs, so you don't need to change this for every submission. -->

There are two submission types currently in HHWWgg_Run_Jobs.sh:
- Trees with many final state variables
- Workspaces with minimal variables

The many final state variables job is useful for studying the kinematics of all final state objects, including leptons and jets before and after selections, as well as the two photons associated with the diphoton candidate. As an example, to run over all events of signal and save trees with final state variables, one should run:

```bash
. HHWWgg_Run_Jobs.sh --labelName HHWWgg_v2-6_Trees_X600_Test --nEvents all --output /eos/user/r/rasharma/post_doc_ihep/double-higgs/ntuples/HHWWgg_10July/ --json Taggers/test/HHWWgg_v2-6/HHWWgg_v2-6_X600.json --condorQueue longlunch --year 2017 -g -c -v -t
```

An explanation of the flags:
- **labelName**: The name used for the output folder placed in ntupleDirec
- **nEvents**: The max events to run on. To run on all events, specify the flag argument: "all"
- **output**: Add the path where the output file should be placed.
- **json**: The json file to use for fggrunjobs submission. This should contain the datasets to run on, and specify the campaign, and PU target for MC jobs
- **condorQueue**: The [condor flavour](https://twiki.cern.ch/twiki/bin/view/ABPComputing/LxbatchHTCondor#Queue_Flavours) for the condor jobs. Note that this needs to be carefully selected, otherwise jobs may timeout and no output will be produced. You may need to try multiple flavors to find the ideal one for your job type.
- **year**: Specifies the MetaConditions to use. 2016, 2017 or 2018
- **g**: Use workspaceStd as the CMS configuration file
- **c**: Run HHWWgg cut flow. This means all events that pass pre-selection will be saved in output nTuples.
- **v**: Save HHWWgg final state variables. Currently set up to be MANY variables (this should be noted. It may take more computing time than normal)
- **t**: Save trees in output nTuples. Useful for python modules / c++ macros designed for nTuple analysis with TTrees / TBranches.

In this example the `HHWWgg_v2-6` json is specified. This is a campaign with three signal mass points: 260, 600, 1000 GeV Radion decaying semileptonically with all lepton decays, including taus.

Any json file can be specified as long as it is formatted properly. You should be able to find some examples under **Taggers/test/\*HHWWgg\***. These input json files can also be created from text files of dataset names with SampleTools.py.

**Note** : In order for flashgg campaigns to be defined and therefore accessed via the fggrunjobs json specified with the `--json` flag, they must be created with **fggManageSamples.py**. You can find instructions for performing this [here](https://twiki.cern.ch/twiki/bin/viewauth/CMS/AbrahamTishelmanCharnyHomepage#Adding_MicroAOD_s_to_a_flashgg_C) and [here](https://github.com/cms-analysis/flashgg/tree/dev_legacy_runII/MetaData#importing-datasets-from-dbs).

If your campaign exists in `MetaData/data/`, specifying the campaign and datasets in the json should be defined properly for fggrunjobs. Note that `HHWWgg_v2-6` should be defined for this state of the cloned repository.

To produce workspaces with minimal variables to be used by fggfinalfit, you can for example run:

```bash
. HHWWgg_Run_Jobs.sh --labelName HHWWgg_v2-6_Workspaces_X600_Test --nEvents all  --output /eos/user/r/rasharma/post_doc_ihep/double-higgs/ntuples/HHWWgg_10July/ --json Taggers/test/HHWWgg_v2-6/HHWWgg_v2-6_X600.json --condorQueue microcentury --year 2017 -g -s -w
```

Explaining the new flags:
- **s**: Run flashgg systematics workflow. Required to obtain final results in fggfinalfit with systematic uncertainty. **Note** that even if you just want a stat only result, it is useful to add systematics as you can just choose not to include them in fggfinalfit.
- **w**: Save workspaces in output. Used by fggfinalfit.

If this works properly, the output will be files (to be hadded) containing a RooWorkspace with the variables required for fggfinalfit, namely `CMS_hgg_mass` and `dZ` (for signal).

To produce workspaces for 2017 data, you would run a similar command but with the 2017 DoubleEG dataset input for the json file:

```bash
    . HHWWgg_Run_Jobs.sh --labelName HHWWgg_v2-6_2017_Data_Workspaces --nEvents all --output /eos/user/r/rasharma/post_doc_ihep/double-higgs/ntuples/HHWWgg_10July/ --json Taggers/test/HHWWgg_2017_Data_All/HHWWgg_Data_All_2017.json --condorQueue longlunch --year 2017 -g -s -w
```

#### Example: 2017 Data / MC Variables

To produce ntuples for 2017 Data (DoubleEG dataset) and MC, you would run `HHWWgg_Run_Jobs` with the json files specifying 2017 data and MC, and the flags that will save trees with many final state variables for many objects,
including leptons and jets before and after any selections are applied. This is useful for MVA studies in order to input training information with limit selections to increase statistics.

To use the [HHWWgg_Run_Jobs.sh](https://github.com/atishelmanch/flashgg/blob/a7da39035c95cfe3f94b8aa6a428c5811e7dbc59/HHWWgg_Run_Jobs.sh) script, make sure to first edit the fggDirec variable, as described above in the beginning of the ["Running on Condor" section](#running-on-condor).

In order to submit jobs with 2017 Data, you would run the command:

```bash
. HHWWgg_Run_Jobs.sh --labelName HHWWgg_2017_Data_Trees --nEvents all --output /eos/user/r/rasharma/post_doc_ihep/double-higgs/ntuples/HHWWgg_10July/  --json Taggers/test/HHWWgg_2017_Data_All/HHWWgg_Data_All_2017.json --condorQueue longlunch --year 2017 -g -c -v -t
```

**Note** : In the above example the condor job flavour "longlunch" is specified, giving each job a maximum of two hours of running to complete. Depending on how long the job takes, it may be necessary to specify the next flavour "workday". Specifying "workday" sets the max running time of each job
to 8 hours, meaning it may be more likely that the job completes. However this may mean the job will take longer as it may have worse priority (I am not 100% sure of all the details of how condor works, hence the vague language).

At the moment for HHWWgg, there is a json file specifying the backgrounds that are relevant for this analysis: [Taggers/test/DataMC/Flashgg_bkg.json](https://github.com/atishelmanch/flashgg/blob/a7da39035c95cfe3f94b8aa6a428c5811e7dbc59/Taggers/test/DataMC/Flashgg_bkg.json). To run the same tagger on this json, you would
simply submit with the command:

```bash
. HHWWgg_Run_Jobs.sh --labelName HHWWgg_2017_FggBackground_Trees --nEvents all  --output /eos/user/r/rasharma/post_doc_ihep/double-higgs/ntuples/HHWWgg_10July/ --json Taggers/test/DataMC/Flashgg_bkg.json --condorQueue workday --year 2017 -g -c -v -t
```

For this example, workday may be a better choice of work flavour as there are some backgrounds with many events such as GJet, QCD, Drell Yan and DiPhotonJetsBox that may take a long time to run.

#### Resubmit failed jobs

There is a script [resubmit_jobs.py](Systematics/scripts/resubmit_jobs.py) which helps us to resubmit the jobs.

Uses:

```bash
python Systematics/scripts/resubmit_jobs.py --dir HHWWgg_v2-6_PtOrderNoBqrk_RadionWorkspace  -s /eos/user/r/rasharma/post_doc_ihep/double-higgs/ntuples/August12/HHWWgg_v2-6_PtOrderNoBqrk_RadionWorkspace/
```

Explanation of each flag:

- **dir**: Name of local condor directory in which condor logs are present.
- **s**: Full path of the directory in which first condor job send its output.


## nTuple Processing

After your condor jobs are complete, you should have a number of output files for each signal point or data taking era. The first check is to make sure the output number of files equals the number of condor jobs. If there are output files missing, the condor .err .out and .log files may point to the reason why.

After checking you have all of the output files, this section will describe how to hadd the files properly.

### Workspaces

If you ran with workspaces, you need to hadd the workspaces in order to obtain a root file with a single combined root workspace for each signal point to work with `fggfinalfit`. This can be done with the script [HHWWgg_Process_Files.sh](https://github.com/atishelmanch/flashgg/blob/a7da39035c95cfe3f94b8aa6a428c5811e7dbc59/HHWWgg_Process_Files.sh). 
<!-- As with the `HHWWgg_Run_Jobs.sh` script, you need to first set your user specific variables,
namely the nTupleDirec and fggDirec vars.  -->
<!-- After doing this, to hadd the workspaces from the previous job, assuming they're in your ntuple directory with
the name "HHWWgg_v2-6_Workspaces_X600", you would run the command: -->

#### Hadd Signal
```bash
. HHWWgg_Process_Files.sh --nTupleDir /eos/user/r/rasharma/post_doc_ihep/double-higgs/ntuples/HHWWgg_10July/ --inFolder HHWWgg_v2-6_Workspaces_X600 --outFolder HHWWgg_v2-6_Workspaces_X600_Hadded -s --signalType Res -w 
```

Explaining each flag:
- **nTupleDir**: The main directory where one can find input root file directory. The script also creates an output directory here, where it will add the hadded root files. 
- **inFolder**: The directory inside the `nTupleDir` with files to be hadded
- **outFolder**: The directory in `nTupleDir` you want the hadded files to go into
- **s**: Look for file names with the format of signal files
- **signalType**: Look for file names with the name format of resonant signals. Ex: "output_ggF_X600_HHWWgg_qqlnu_6.root". It's important that the file names are of the expected format, as this script and fggfinalfit scripts will use this to obtain quantities like the resonant masses.
- **w**: This arguments specifies the script to hadd workspaces.

This tells the script to hadd files in `/eos/user/r/rasharma/post_doc_ihep/double-higgs/ntuples/HHWWgg_10July/HHWWgg_v2-6_Workspaces_X600` using the flashgg script [Systematics/scripts/hadd_all.py](https://github.com/atishelmanch/flashgg/blob/a7da39035c95cfe3f94b8aa6a428c5811e7dbc59/Systematics/scripts/hadd_all.py), and put the output files in your desired outFolder. Note that this setup works for any number of resonant mass points, NMSSM
mass pairs or EFT benchmarks located in the `--inFolder`.

If this works properly for this example, you should have a single hadded file in `HHWWgg_v2-6_Workspaces_X600_Hadded` for the 600 GeV resonant point. This will be the input signal file for [fggfinalfit](https://github.com/atishelmanch/flashggFinalFit/tree/HHWWgg_Dev_runII_102x).

#### Hadd Data
To do the same for data, after running HHWWgg_Run_Jobs on a data json and directing your ouput files to HHWWgg_v2-6_Data_Workspaces, you would run:

```bash
. HHWWgg_Process_Files.sh --nTupleDir /eos/user/r/rasharma/post_doc_ihep/double-higgs/ntuples/HHWWgg_10July/ --inFolder HHWWgg_v2-6_2017_Data_Workspaces --outFolder HHWWgg_v2-6_2017_Data_Workspaces_Hadded -d -w
```

Explaining the new flag:
- **d**: Don't look for special file name formats.

By default this should hadd by data era. For example for 2017 data, this should result in 5 hadded files in `HHWWgg_v2-6_2017_Data_Workspaces_Hadded`, one for each Era from B to F which should be named Data_0.root, Data_1.root, ... You would then want to hadd these into a single hadded file for all of 2017 data to be used by fggfinalfit. This can be done with the command:

```bash
    . HHWWgg_Process_Files.sh --nTupleDir /eos/user/r/rasharma/post_doc_ihep/double-higgs/ntuples/HHWWgg_10July/  --inFolder HHWWgg_v2-6_2017_Data_Workspaces_Hadded --outFolder HHWWgg_v2-6_2017_Data_Workspaces_Hadded_Combined -d -c -w
```

Where the new flag is :
- **c**: Combine all data eras.

This command will hadd `Data_*.root` into a single file: `HHWWgg_v2-6_2017_Data_Workspaces_Hadded_Combined/allData.root`. This contains a single workspace will all data you ran on, and
is used as the input for flashggfinalfit.


### Trees
If you ran with trees, these are hadded in the usual way with the hadd command. Or you can use the same script as above by replacing the argument `-w` with `-t`. 

```bash
. HHWWgg_Process_Files.sh --nTupleDir /eos/user/r/rasharma/post_doc_ihep/double-higgs/ntuples/HHWWgg_10July/ --inFolder HHWWgg_v2-6_Workspaces_X600 --outFolder HHWWgg_v2-6_Workspaces_X600_Hadded -s --signalType Res -t 
```

Explanation of additional flag:
- **t**: Specifies to hadd the trees.

#### Hadd MC Background

```bash
. HHWWgg_Process_Files.sh --nTupleDir /eos/user/r/rasharma/post_doc_ihep/double-higgs/ntuples/background/ --inFolder Allbkg --outFolder Allbkg_Hadded -b -t
```

Explanation of additional flag:
- **b**: Specifies the script that it need to hadd background files.

## Rename Workspace

- Script [RenameWorkspace_DataTagger.cpp](https://github.com/atishelmanch/flashgg/blob/a7da39035c95cfe3f94b8aa6a428c5811e7dbc59/RenameWorkspace_DataTagger.cpp) helps to rename workspace for data.
- Script [RenameWorkspace_SignalTagger.cpp](https://github.com/atishelmanch/flashgg/blob/a7da39035c95cfe3f94b8aa6a428c5811e7dbc59/RenameWorkspace_SignalTagger.cpp) helps to rename workspaces for signal.

# Examples 

The purpose of this portion of the README is to provide examples of running the HHWWggTagger. 

## 2018 Semi-Leptonic SM NLO 

The following instructions are an example workflow for producing STAT only results with the centrally produced 2018 SM SL 2018 signal sample. You can first check that the analysis runs locally with the command:

```bash
    cmsRun Systematics/test/workspaceStd.py metaConditions=MetaData/data/MetaConditions/Era2017_RR-31Mar2018_v1-HHWWgg.json campaign=SL-EFT-2018 dataset=GluGluToHHTo2G2Qlnu_node_cHHH1_TuneCP5_PSWeights_13TeV-powheg-pythia8 doHHWWggTag=1 HHWWggTagsOnly=1 maxEvents=500 doSystematics=0 dumpWorkspace=0 dumpTrees=1 useAAA=1 doHHWWggTagCutFlow=1 saveHHWWggFinalStateVars=1 HHWWggAnalysisChannel=SL HHWWgguseZeroVtx=1
```

This should produce an output root file with a tree for each tag, and electron / muon events in tags 0 / 1. As long as it is non zero, this should mean the tagger is working properly. 

The next step is to run over all events using the fggRunJobs script. This can be run and customized with the script MetaData/scripts/SubmitJobs.py. It's helpful first check that everything works by submitting a low number of events and producing output trees:

```bash
    python MetaData/scripts/SubmitJobs.py --fggDirec /afs/cern.ch/work/a/atishelm/private/flashgg-10_6_8/CMSSW_10_6_8/src/flashgg --outDirec /eos/user/a/atishelm/ntuples/HHWWgg_flashgg --label HHWWgg-SL-SM-NLO-LowEvents-Trees --nEvents 1000 --saveHHWWggFinalStateVars --HHWWgg --dumpTrees --jsonPath Taggers/test/Examples/HHWWgg-SL-SM-NLO/SL_SM2018.json
```

Where of course the paths for fggDirec and outDirec should be changed to your flashgg location and output directory. You can then monitor your condor job progress with a [Monidor](https://github.com/atishelmanch/Monidor). If the output trees have entries and values of CMS_hgg_mass for example look sensible, you can run on all events simply by changing the --nEvents flag, and switching from trees to workspaces in order to have an output compatible with fggfinalfit:

```bash
    python MetaData/scripts/SubmitJobs.py --fggDirec /afs/cern.ch/work/a/atishelm/private/flashgg-10_6_8/CMSSW_10_6_8/src/flashgg --outDirec /eos/user/a/atishelm/ntuples/HHWWgg_flashgg --label HHWWgg-SL-SM-NLO-2018 --nEvents -1 --HHWWgg --dumpWorkspace --jsonPath Taggers/test/Examples/HHWWgg-SL-SM-NLO/SL_SM2018.json
```

Where again progress can be monitored via a [Monidor](https://github.com/atishelmanch/Monidor). If all works properly, you should have a number of output files equal to the number of condor jobs submitted. 

One can then run on 2018 data, first running on a low number of events to make sure everything is compatible:

```bash 
    python MetaData/scripts/SubmitJobs.py --fggDirec /afs/cern.ch/work/a/atishelm/private/flashgg-10_6_8/CMSSW_10_6_8/src/flashgg --outDirec /eos/user/a/atishelm/ntuples/HHWWgg_flashgg --label 2018-DoubleEG-Data-LowEvents --nEvents 1000 --HHWWgg --dumpTrees --jsonPath Taggers/test/Examples/HHWWgg-SL-SM-NLO/HHWWgg_Data_All_2018.json --saveHHWWggFinalStateVars --condorQueue microCentury
```

If the output trees have some number of events greater than 0, especially in the untagged category (currently HHWWggTag_4), the tagger should be working properly. One can then run on all events with the command:

```bash
    python MetaData/scripts/SubmitJobs.py --fggDirec /afs/cern.ch/work/a/atishelm/private/flashgg-10_6_8/CMSSW_10_6_8/src/flashgg --outDirec /eos/user/a/atishelm/ntuples/HHWWgg_flashgg --label 2018-DoubleEG-Data --nEvents -1 --HHWWgg --dumpWorkspace --jsonPath Taggers/test/Examples/HHWWgg-SL-SM-NLO/HHWWgg_Data_All_2018.json --condorQueue microCentury    
```

Where the condorQueue flag can be adjusted if more time is required to complete jobs on the first attempt. If jobs fail, they can be resubmitted with the script Systematics/scripts/resubmit_jobs.py. 

# Few Important Things To Note Before running the framework

1. Campaign `RunIIFall17-3-2-0` contains `flashggMetsCorr` as **InputTag** not `flashggMets`. 
  1. Whenever we need to run over this campaign then we need to edit three files:
    1. uncomment/comment the MetTag in [Taggers/python/flashggHHWWggTag_cfi.py](https://github.com/atishelmanch/flashgg/blob/a7da39035c95cfe3f94b8aa6a428c5811e7dbc59/Taggers/python/flashggHHWWggTag_cfi.py).
    1. uncomment/comment the MetTag in [Taggers/python/flashggHHWWggCandidate_cfi.py](https://github.com/atishelmanch/flashgg/blob/a7da39035c95cfe3f94b8aa6a428c5811e7dbc59/Taggers/python/flashggHHWWggCandidate_cfi.py).
    1. uncomment/comment the MetTag in [Systematics/python/flashggMetSystematics_cfi.py](https://github.com/atishelmanch/flashgg/blob/a7da39035c95cfe3f94b8aa6a428c5811e7dbc59/Systematics/python/flashggMetSystematics_cfi.py).

  1. **Alternatively, you can apply the patch `flashggMets_To_flashggMetsCorr.patch` using command below**

      ```bash
      git apply flashggMets_To_flashggMetsCorr.patch
      ```

2. Sample name flow:

   ```bash
   $cat Taggers/test/HHWWgg_v2-6/HHWWggFullyHad.json 
    {
     "processes" : {
     "GluGluToHHTo_WWgg_qqqq_node11" : [ "/ggF_node11_HHWWgg_qqqq"  ]
    },
    "cmdLine" : "campaign=rasharma-HHWWgg_v2-2_Test_94X_mc2017 targetLumi=1e+3 useAAA=1 useEOS=0 puTarget=6.245e-06,...,8.814e-12"
   ```

   - The name `ggF_node11_HHWWgg_qqqq`  in above patch should be defined in `MetaData/data/cross_sections.json`. Something like:

   ```json
   "ggF_node11_HHWWgg_qqqq": {
    "xs": 0.001
   },
   ```

   - The output root file name will be `output_ggF_node11_HHWWgg_qqqq_*.root` when running using condor.
   - The workspace name will be `GluGluToHHTo_WWgg_qqqq_node11`.



