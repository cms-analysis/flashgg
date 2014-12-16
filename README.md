flashgg
=======

1. Create a CMSSW_7_2_2_patch2 (or later) project:
 ```
 # make sure you are on lxplus6 or otherwise using an SLC6 machine
 # make sure SCRAM_ARCH is slc6_amd64_gcc481
 cmsrel CMSSW_7_2_2_patch2
 # or later
 cd CMSSW_7_2_2_patch2/src
 cmsenv
 ```

2. Get extra files for Pileup Jet Identification (must be done *before* getting flashgg):
 ```
 git cms-addpkg RecoJets/JetProducers
 git cms-merge-topic -u musella:pileupjetid-for-flashgg-72x
 ```

3. Get weight counter from musella until it is integrated in the release
  ```
  git cms-addpkg CommonTools/UtilAlgos
  git cms-addpkg DataFormats/Common
  git cms-merge-topic musella:topic-weights-count
  ```

4. Fork flashgg repository on the web here: https://github.com/cms-analysis/flashgg

5. In CMSSW_7_2_2_patch2/src, do commands something like: 
 ```
 cd $CMSSW_BASE/src
 git clone git@github.com:yourusername/flashgg.git flashgg
 cd flashgg
 git remote add upstream https://github.com/cms-analysis/flashgg
 # see https://help.github.com/articles/fork-a-repo for more about this 
 ```

6. Now build, a very basic workflow test:
 ```
 cd $CMSSW_BASE/src
 scram b
 cmsRun flashgg/MicroAODProducers/test/simple_Producer_test.py
 cmsRun flashgg/TagProducers/test/simple_Tag_test.py
 ```

For the impatient user (SLC6 right SCRAM_ARCH) just:
```
source ~carrillo/public/for_All/flashgg_master_test.sh
```

For the developer: Fork https://github.com/cms-analysis/flashgg and then execute
```
source ~carrillo/public/for_All/flashgg_me.sh
```
