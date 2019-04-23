flashgg
=======

Before you start, **please take note** of these warnings and comments:
* **N.B.** The setup script will check out many packages and take a while!
* **N.B.** You can ignore "error: addinfo_cache" lines. 

Supported releases:
* 10_5_0
* 10_2_9 ( deprecated, no GRID access)

 ```
 cmsrel CMSSW_10_5_0
 cd CMSSW_10_5_0/src
 cmsenv
 git cms-init
 cd $CMSSW_BASE/src 
 git clone -b dev_legacy_runII https://github.com/cms-analysis/flashgg 
 source flashgg/setup_flashgg.sh
 ```

If everything now looks reasonable, you can build:
 ```
 cd $CMSSW_BASE/src
 scram b -j 4
 ```

Examples to run on RunII legacy test campaign:
 ```
 cd Systematics/test
 voms-proxy-init -voms cms --valid 168:00
 cp /tmp/MYPROXY ~/
 export X509_USER_PROXY=~/MYPROXY
 fggRunJobs.py --load legacy_runII_v1_YEAR.json -d test_legacy_YEAR workspaceStd.py -n 300 -q workday --no-copy-proxy
 ```

**Note: 2018 workflow is just a skepleton, only scales and smearings are known to be correct.**

And a very basic workflow test (for reference, this is not supposed to give paper-grade results):
 ```
 cd $CMSSW_BASE/src/flashgg
 cmsRun MicroAOD/test/microAODstd.py processType=sig year=2017 datasetName=glugluh conditionsJSON=MetaData/data/MetaConditions/Era2016_RR-17Jul2018_v1.json 
 # processType=data depending on input file
 #with year=2017 or year=2016 it will rerun the DeepJet b-tagger for year=2018 it won't rerun
 cmsRun Taggers/test/simple_Tag_test.py
 cmsRun Taggers/test/diphotonsDumper_cfg.py
 cmsRun Systematics/test/workspaceStd.py processId=ggh_125 doHTXS=1
 ```

These are just some test examples; the first makes MicroAOD from a MiniAOD file accessed via xrootd, 
the second produces tag objects and screen output from the new MicroAOD file,
and the other two process the MicroAOD file to test ntuple and workspace output.

The setup code will automatically change the initial remote branch's name to upstream to synchronize with the project's old conventions.  
The code will also automatically create an "origin" repo based on its guess as to where your personal flashgg fork is.
Check that this has worked correctly if you have trouble pushing.  (See setup_*.sh for what it does.)

