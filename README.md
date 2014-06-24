flashgg
=======
1. Create a CMSSW_7_0_6 project.

2. Fork flashgg repository on the web here: https://github.com/cms-analysis/flashgg

3. In CMSSW_7_0_6/src, do commands something like: 

```
git clone git@github.com:yourusername/flashgg.git flashgg
cd flashgg
git remote add upstream https://github.com/cms-analysis/flashgg
# see https://help.github.com/articles/fork-a-repo for more about this 
```

4. Now build, a very basic workflow test, and an extremely primitive FWLite script:

```
cd $CMSSW_BASE/src
scram b
cd flashgg/MicroAODProducers/test
cmsRun simple_Producer_test.py
root -b -q scan_test_output.C
```

For the impatient user: Just execute 

```
source ~carrillo/public/for_All/flashgg_me.sh
```

after you have forked cms-analysis/flashgg in /afs/cern.ch/user/c/carrillo/public/for_All/flashgg_me.sh
