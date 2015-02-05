# -- do f test --
./bin/fTest -i hgg.inputbkgdata_8TeV_MVA.root -c 9 -d dat/bsConfig_2012.dat -D plots/fTest/2012
./bin/fTest -i CMS-HGG_massfacmva_jan16reload.root -c 5 -d dat/bsConfig_2011.dat -D plots/fTest/2011 --is2011
# -- submit jobs -- 
./scripts/sub_coverage_jobs.py -D dat/coverageConfig2011.dat -t 100 -n 50 -q 8nh --skipPlots
./scripts/sub_coverage_jobs.py -D dat/coverageConfig2012.dat -t 100 -n 50 -q 8nh --skipPlots
# -- check --
./scripts/checkOutputFiles.py ~/eos/cms/store/user/mkenzie/BackgroundProfiling/
# -- merge --
./scripts/sub_merge_coverage.py -D coverage2011/merge_scripts -d dat/histOpts2011.dat -q 8nm --eosWalk=50
./scripts/sub_merge_coverage.py -D coverage2012/merge_scripts -d dat/histOpts2012.dat -q 8nm --eosWalk=50
# -- draw --
./scripts/drawCoverage.py -d dat/fullDrawOpts.dat
