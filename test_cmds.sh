NOW=`date +'%y%d%m%H%M%S'`
echo "Provisional version of test script relies on piping syntax for zsh, please let me know if you have a different SHELL and have trouble"
echo "microAODstd.py test (output uAOD_test_auto_$NOW.txt)"
cmsRun MicroAOD/test/microAODstd.py datasetName=EGamma processType=data >& uAOD_test_auto_$NOW.txt
echo "simpleTagTest.py test (output simpleTag_test_auto_$NOW.txt)"
cmsRun Taggers/test/simple_Tag_test.py >& simpleTag_test_auto_$NOW.txt
echo "diphotonsDumper test (output diphotons_test_auto_$NOW.txt)"
cmsRun Taggers/test/diphotonsDumper_cfg.py >& diphotons_test_auto_$NOW.txt
echo "workspaceStd test (output workspace_test_auto_$NOW.txt)"
cmsRun Systematics/test/workspaceStd.py processId=Data doHTXS=1 verboseTagDump=1 verboseSystDump=1  >& workspace_test_auto_$NOW.txt
echo "diffing and writing to diff_summary_$NOW.txt"
diff uAOD_test_auto_$NOW.txt uAOD_test_ref.txt > diff_summary_$NOW.txt
diff simpleTag_test_auto_$NOW.txt simpleTag_test_ref.txt >> diff_summary_$NOW.txt
diff diphotons_test_auto_$NOW.txt diphotons_test_ref.txt >> diff_summary_$NOW.txt
diff workspace_test_auto_$NOW.txt workspace_test_ref.txt | grep -v TimeReport >> diff_summary_$NOW.txt
