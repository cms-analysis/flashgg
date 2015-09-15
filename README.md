flashgg
=======

Before you start, **please take note** of these warnings and comments:
* **N.B.** Make sure you are on lxplus6 or otherwise using an SLC6 machine. Make sure SCRAM_ARCH is slc6_amd64_gcc491.
* **N.B.** The setup script will check out many packages and take a while!
* **N.B.** You can ignore "error: addinfo_cache" lines. 
* **N.B.** This is to set up the latest area in a self-consistent way. If you want a particular flashgg version corresponding to other samples, please see https://twiki.cern.ch/twiki/bin/viewauth/CMS/FLASHggFramework#Instructions_for_users

Get everything you need, starting from a clean area:
 ```
 cmsrel CMSSW_7_4_12
 cd CMSSW_7_4_12/src
 cmsenv
 git cms-init
 cd $CMSSW_BASE/src 
 git clone https://github.com/cms-analysis/flashgg flashgg
 source flashgg/setup.sh
 ```
Now if everything looks reasonable, you can build:
 ```
 cd $CMSSW_BASE/src
 scram b -j 9
 ```
And a very basic workflow test:
 ```
 cmsRun flashgg/MicroAOD/test/microAODstd.py
 cmsRun flashgg/Taggers/test/simple_Tag_test.py 
 ```

The setup code will automatically change the initial remote branch's name to upstream to synchronize with the project's old conventions.  
If you need to commit, you'll have to put your username in the following:
 ```
 cd ${CMSSW_BASE}/src/flashgg ; git remote add origin git@github.com:yourusername/flashgg.git
 ```
