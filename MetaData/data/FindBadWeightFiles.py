######################################################################################################################
# Abraham Tishelman-Charny                                                                                           #
# 1 December 2020                                                                                                    #
#                                                                                                                    #
# The purpose of this module is to find microAODs with large or small average GEN weights. Run from Metadata/data.   #
###################################################################################################################### 

import os 
import pandas as pd 

##-- Determine suspicious datasets from fggManageSamples.py, checking Avg weight
# 2016
# ex: fggManageSamples.py -C Era2016_RR-17Jul2018_v2 list '*GluGluToHHTo*'
# Avg weight for:
# /GluGluToHHTo2G2Qlnu_node_cHHH2p45_TuneCUETP8M1_PSWeights_13TeV-powheg-pythia8
# /GluGluToHHTo2G2Qlnu_node_cHHH5_TuneCUETP8M1_PSWeights_13TeV-powheg-pythia8
# seems too high / wrong compared to other cHHH processes 
# /GluGluToHHTo2G2Qlnu_node_SM_TuneCUETP8M1_PSWeights_13TeV-madgraph-pythia8 has one bad file 
#
# 2017 
# fggManageSamples.py -C Era2017_RR-31Mar2018_v2 list '*GluGluToHHTo*cHHH1*'
# /GluGluToHHTo2G2Qlnu_node_cHHH1_TuneCP5_PSWeights_13TeV-powheg-pythia8 has one bad file 
#
# 2018 
# fggManageSamples.py -C Era2018_RR-17Sep2018_v2 list "*GluGluToHHTo*cHHH0*"
# bbgg sample actually seems suspicious here:
# /GluGluToHHTo2B2G_node_cHHH0_TuneCP5_PSWeights_13TeV-powheg-pythia8
# avg weight of 0.074 vs. 0.06 for all WWgg samples 

##-- Get full dataset names for suspicious files with fggManageSamples.py -C Era2016_RR-17Jul2018_v2 list raw 
##-- (is it possible to add string search for list raw output?)
# 
# 2016 
# /GluGluToHHTo2G2Qlnu_node_cHHH2p45_TuneCUETP8M1_PSWeights_13TeV-powheg-pythia8/alesauva-2016_1-10_6_4-v0-RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1-c3d8a5638586a0e8df7c55ce908b2878/USER
# /GluGluToHHTo2G2Qlnu_node_cHHH5_TuneCUETP8M1_PSWeights_13TeV-powheg-pythia8/alesauva-2016_1-10_6_4-v0-RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1-c3d8a5638586a0e8df7c55ce908b2878/USER
# /GluGluToHHTo2G2Qlnu_node_SM_TuneCUETP8M1_PSWeights_13TeV-madgraph-pythia8/alesauva-2016_1-10_6_4-v0-RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1-c3d8a5638586a0e8df7c55ce908b2878/USER
#
# 2017 
# /GluGluToHHTo2G2Qlnu_node_cHHH1_TuneCP5_PSWeights_13TeV-powheg-pythia8/alesauva-2017_1-10_6_4-v0-RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1-1c4bfc6d0b8215cc31448570160b99fd/USER
#
# 2018 
# -

##-- Get Json files with your desired process 
def GetJsons(campaign_, dataset_):
  jsons_ = [] 
  os.system("touch GrepOutput.txt")
  for file in os.listdir(campaign_):
    if ".json" in file:
      filePath = "%s/%s"%(campaign_,file)
      os.system("rm GrepOutput.txt")
      os.system("grep %s %s >> GrepOutput.txt"%(dataset_, filePath))
      GrepSize = os.path.getsize("GrepOutput.txt")
      if(GrepSize > 0): 
        print"Found string %s in file %s"%(dataset_, filePath)
        jsons_.append(filePath)
  return jsons_

def GetBadFiles(json_, dataset_):
  BadWeightFiles_, weightEventRatios_ = [], [] 
  df = pd.read_json(json_)
  info = df.loc[:,dataset_]
  for f in info["files"]:
    totEvents, weights = f['totEvents'], f['weights']
    ratio = float(weights) / float(totEvents)
    print "weights / totEvents: ",ratio
    if(ratio > 1):
      print f['name']    
      BadWeightFiles_.append(str(f['name']))
      weightEventRatios_.append(ratio)
    if dataset_ == "/GluGluToHHTo2G2Qlnu_node_cHHH1_TuneCP5_PSWeights_13TeV-powheg-pythia8/alesauva-2017_1-10_6_4-v0-RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1-1c4bfc6d0b8215cc31448570160b99fd/USER":
      if ratio < 0.015:
        BadWeightFiles_.append(str(f['name']))
        weightEventRatios_.append(ratio)
  return BadWeightFiles_, weightEventRatios_

def GetCampaignsAndDatasets(DatasetsToCheck_):
  campaigns_ = [campaign_ for campaign_ in DatasetsToCheck_]
  datasets_ = [DatasetsToCheck_[campaign_] for campaign_ in campaigns_]
  CampDsetPairs_ = []
  for ic,camp in enumerate(campaigns_):
    datasets = datasets_[ic]
    CampDsetPair_ = [camp,datasets]
    CampDsetPairs_.append(CampDsetPair_)
  return CampDsetPairs_ 

DatasetsToCheck = {
  # Campaign : [dataset_1, dataset_2, ..., dataset_N]
  #"Era2016_RR-17Jul2018_v2" : ["/GluGluToHHTo2G2Qlnu_node_cHHH2p45_TuneCUETP8M1_PSWeights_13TeV-powheg-pythia8/alesauva-2016_1-10_6_4-v0-RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1-c3d8a5638586a0e8df7c55ce908b2878/USER",
                               #"/GluGluToHHTo2G2Qlnu_node_cHHH5_TuneCUETP8M1_PSWeights_13TeV-powheg-pythia8/alesauva-2016_1-10_6_4-v0-RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1-c3d8a5638586a0e8df7c55ce908b2878/USER"
                              #  "/GluGluToHHTo2G2Qlnu_node_SM_TuneCUETP8M1_PSWeights_13TeV-madgraph-pythia8/alesauva-2016_1-10_6_4-v0-RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1-c3d8a5638586a0e8df7c55ce908b2878/USER"
#  ],
  "Era2017_RR-31Mar2018_v2" : ["/GluGluToHHTo2G2l2nu_node_cHHH1_TuneCP5_PSWeights_13TeV-powheg-pythia8/alesauva-2017_0-10_6_4-v0-RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1-1c4bfc6d0b8215cc31448570160b99fd/USER"]
}

CampDsetPairs = GetCampaignsAndDatasets(DatasetsToCheck)
BadMicroAODsOutfile = open("BadMicroAODs.txt","w")
BadMicroAODsOutfile.write("MicroAOD's With Strange GEN Weights\n")

for CampDsetPair in CampDsetPairs:
  campaign, datasets = CampDsetPair[0], CampDsetPair[1]
  BadMicroAODsOutfile.write("\n")
  BadMicroAODsOutfile.write("Campaign: %s\n"%(campaign))
  for dataset in datasets:
    BadMicroAODsOutfile.write("Dataset: %s\n"%(dataset))
    jsons = GetJsons(campaign,dataset)
    for json in jsons:
      BadFiles, weightEventRatios = GetBadFiles(json,dataset)
      print "BadFiles:",BadFiles
      for i,BadFile in enumerate(BadFiles): 
        weightEventRatio = weightEventRatios[i]
        BadMicroAODsOutfile.write("File: %s %s\n"%(BadFile,weightEventRatio))

BadMicroAODsOutfile.close()
