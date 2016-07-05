# NB this command is specific to the configuration at IC and is not gaurenteed elsewhere
LM=/home/hep/szenz/fromscratch27/CMSSW_8_0_8_patch1/src/flashgg/Systematics/test/Cert_271036-275125_13TeV_PromptReco_Collisions16_JSON.txt # 3.99/fb
#LM=/home/hep/szenz/fromscratch25/CMSSW_8_0_8/src/flashgg/MetaData/work/jsons/Cert_271036-274421_13TeV_PromptReco_Collisions16_JSON.txt # 2.6/fb 

fggRunJobs.py --load data_jobs.json -d /vols/cms/szenz/data_jobs_023 -x cmsRun workspaceStd.py maxEvents=-1 -n 500 -q hepshort.q -D -P useAAA=0 atIC=1 --no-use-tarball lumiMask=${LM}