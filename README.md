flashgg
=======

1. Create a CMSSW_7_0_6_patch1 project:
 ```
 # make sure you are on lxplus6 or otherwise using an SLC6 machine
 # make sure SCRAM_ARCH is slc6_amd64_gcc481 (should be default)
 cmsrel CMSSW_7_0_6_patch1
 cd CMSSW_7_0_6_patch1/src
 cmsenv
 ```
2. Get extra files for Pileup Jet Identification:
 ```
 git cms-addpkg RecoJets/JetProducers
 git cms-merge-topic -u sethzenz:pileupjetid-for-flashgg
 ```

3. Fork flashgg repository on the web here: https://github.com/cms-analysis/flashgg

4. In CMSSW_7_0_6_patch1/src, do commands something like: 
 ```
 git clone git@github.com:yourusername/flashgg.git flashgg
 cd flashgg
 git remote add upstream https://github.com/cms-analysis/flashgg
 # see https://help.github.com/articles/fork-a-repo for more about this 
 ```

5. Now build, a very basic workflow test, and an extremely primitive FWLite script:
 ```
 cd $CMSSW_BASE/src
 scram b
 cd flashgg/MicroAODProducers/test
 cmsRun simple_Producer_test.py
 root -b -q scan_test_output.C
 ```

For the impatient user: Fork https://github.com/cms-analysis/flashgg and then execute
```
# Might need updating for additional jet steps
source ~carrillo/public/for_All/flashgg_me.sh
```
