
cmsLs /store/group/phys_higgs/cmshgg/processed/V13_03_00/mc/Summer12_S7_8TeV | awk '{ print $5}' | grep '/' > samples.txt 

#edit samples.txt to remove uneeded stuff

mkdir -p data
cd data

./pileupCalcWrapper.py --minBiasXsec=68300 --pileupJson=/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/PileUp/pileup_JSON_DCSONLY_190389-194533.txt /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-194479_8TeV_PromptReco_Collisions12_JSON.txt

cd ..


mkdir -p mc
cd mc

parallel --eta --joblog ../parallel_pileupMerger.log --progress "python ../pileupMerger.py {1}" :::: ../samples.txt
../makePuWeights.py ../data/Cert_190456-194479_8TeV_PromptReco_Collisions12_JSON.txt.68300.pileup.root *.pileup.root

root -l weights.root
.ls
