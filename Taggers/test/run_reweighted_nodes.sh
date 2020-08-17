#fggRunJobs.py --load jsons/HHbbgg_MC_2016_80X_nodesOnly.json -d reweighting_11_10_2018_workspaces_node_$1 Systematics/test/workspaceStd.py maxEvents=-1 doubleHTagsOnly=True doDoubleHTag=True doBJetRegression=True doubleHReweightTarget=$1 doDoubleHGenAnalysis=False doSystematics=False dumpWorkspace=True dumpTrees=False  -H -P -n 500 --no-use-tarball --no-copy-proxy useAAA=1 doPdfWeights=False -q all.q 

#fggRunJobs.py --load jsons/HHbbgg_MC_2016_80X_nodesOnly.json -d reweighting_06_12_2018_node_$1 Systematics/test/workspaceStd.py maxEvents=-1 doubleHTagsOnly=True doDoubleHTag=True doBJetRegression=True doubleHReweightTarget=$1 doDoubleHGenAnalysis=True doSystematics=False dumpWorkspace=False dumpTrees=True year='2016'  -H -P -n 500  --no-copy-proxy useAAA=1 doPdfWeights=False -q all.q 

#for num in `seq 2 13`;
#do
#	echo 'hadd reweighting_06_12_2018_node_'${num}'/output_GluGluToHHTo2B2G_reweighted_node_'${num}'_13TeV-madgraph.root reweighting_06_12_2018_node_'${num}'/*madgraph.root'
#	echo 
#done


for num in `seq 2 13`;
do
	echo 'cp reweighting_06_12_2018_node_'${num}'/output_GluGluToHHTo2B2G_reweighted_node_'${num}'_13TeV-madgraph.root /shome/nchernya/HHbbgg_ETH_devel/root_files/ntuples_2016_20181210/'
	echo 
done
