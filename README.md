flashgg
=======

Before you start, *please take note* of these warnings and comments:
* N.B. Make sure you are on lxplus6 or otherwise using an SLC6 machine. Make sure SCRAM_ARCH is slc6_amd64_gcc481.
* N.B. You must substitute in your git username below.
* N.B. The setup script will check out many packages and take a while!
* N.B. You can ignore "error: addinfo_cache" lines. (Probably. TBC.)
* N.B. If you have already followed these instructions with a previous version of the flashgg code, or if you have modified your repository while doing previous development, please see FLASHggFramework#Comments_for_expert_developers below for advice on avoiding some possible pitfalls.

Get everything you need, starting from a clean area:
 ```
 cmsrel CMSSW_7_2_2_patch2
 cd CMSSW_7_2_2_patch2/src
 cmsenv
 git cms-init
 cd $CMSSW_BASE/src 
 git clone git@github.com:yourusername/flashgg.git flashgg
 source flashgg/setup.sh
 ```
Now if everything looks reasonable, you can build:
 ```
 cd $CMSSW_BASE/src
 scram b -j 9
 ```
And a very basic workflow test:
 ```
 cmsRun flashgg/MicroAODProducers/test/simple_Producer_test.py
 cmsRun flashgg/TagProducers/test/simple_Tag_test.py
 ```
