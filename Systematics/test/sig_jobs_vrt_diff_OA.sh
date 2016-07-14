# NB this command is specific to the configuration at IC and is not gaurenteed elsewhere
#fggRunJobs.py --load sig_jobs.json -q 1nd -d signal_120_125_130 -n 500  -D -P  -x cmsRun workspaceStd.py maxEvents=100 
#fggRunJobs.py -q all.q --load sig_jobs_vrt.json  -d signal_120_125_130_differential_integratepT -n 500  -D -P  -x cmsRun workspaceStd_Differential.py maxEvents=100
fggRunJobs.py -q long.q --load sig_jobs_vrt_OA_new.json  -d signal_testws_80_OA/ -n 500  -D -P  -x cmsRun workspaceStd_Differential_GenVars_OA_new.py maxEvents=-1