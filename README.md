flashgg
=======
1. Set up MiniAOD with CMSSW_7_0_4 as described here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/MiniAOD#Recipe  (it takes a while)

2. Fork flashgg repository on the web here: https://github.com/cms-analysis/flashgg

3. In CMSSW_7_0_4/src, do commands something like: 

```
git clone git@github.com:sethzenz/flashgg.git flashgg
git remote add upstream https://github.com/cms-analysis/flashgg
# see https://help.github.com/articles/fork-a-repo for more about this 
```

Now build, a very basic workflow test, and an extremely primitive FWLite script:

```
scram b
cd flashgg/MicroAODProducers/test
cmsRun simple_Producer_test.py
root -b -q scan_test_output.C
```