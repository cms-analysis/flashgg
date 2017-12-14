flashgg
=======

Before you start, **please take note** of these warnings and comments:
* **N.B.** Make sure you are on lxplus6 or otherwise using an SLC6 machine. Make sure SCRAM_ARCH is slc6_amd64_gcc491 (or later for 80X).
* **N.B.** The setup script will check out many packages and take a while!
* **N.B.** You can ignore "error: addinfo_cache" lines. 
* **N.B.** This is to set up the latest area in a self-consistent way. If you want a particular flashgg version corresponding to other samples, please see https://twiki.cern.ch/twiki/bin/viewauth/CMS/FLASHggFramework#Instructions_for_users

Currently three releases are supported:
* 8_0_28 for Legacy 2016 processing
* 9_4_0_patch1, version for 2017 v2 processing, recipe still being developed
* 9_2_8, latest working version for 2017 processing (will be replaced when 9_4_0 works)

80X: Get everything you need, starting from a clean area:

 ```
 cmsrel CMSSW_8_0_28
 cd CMSSW_8_0_28/src
 cmsenv
 git cms-init
 cd $CMSSW_BASE/src 
 git clone https://github.com/cms-analysis/flashgg flashgg
 source flashgg/setup_8_0_X.sh
 ```

92X: Get everything you need, starting from a clean area:

 ```
 cmsrel CMSSW_9_2_8
 cd CMSSW_9_2_8/src
 cmsenv
 git cms-init
 cd $CMSSW_BASE/src 
 git clone https://github.com/cms-analysis/flashgg flashgg
 source flashgg/setup_9_2_X.sh
 ```

94X: Get everything you need, starting from a clean area:

 ```
 cmsrel CMSSW_9_4_0_patch1
 cd CMSSW_9_4_0_patch1/src
 cmsenv
 git cms-init
 cd $CMSSW_BASE/src
 git clone https://github.com/cms-analysis/flashgg flashgg
 source flashgg/setup_9_4_0.sh
 ```

If everything now looks reasonable, you can build:
 ```
 cd $CMSSW_BASE/src
 scram b -j 3
 ```
And a very basic workflow test:
 ```
 cd $CMSSW_BASE/src/flashgg
 cmsRun MicroAOD/test/microAODstd.py processType=sig datasetName=glugluh # or processType=data depending on input file
 cmsRun Taggers/test/simple_Tag_test.py
 cmsRun Taggers/test/diphotonsDumper_cfg.py
 cmsRun Systematics/test/workspaceStd.py processId=ggh_125
 ```

These are just some test examples; the first makes MicroAOD from a MiniAOD file accessed via xrootd, 
the second produces tag objects and screen output from the new MicroAOD file,
and the other two process the MicroAOD file to test ntuple and workspace output.

The setup code will automatically change the initial remote branch's name to upstream to synchronize with the project's old conventions.  
The code will also automatically create an "origin" repo based on its guess as to where your personal flashgg fork is.
Check that this has worked correctly if you have trouble pushing.  (See setup_*.sh for what it does.)

