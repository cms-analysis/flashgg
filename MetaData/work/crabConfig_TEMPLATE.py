# CRAB3 config template for flashgg
# More options available on the twiki :
# https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookCRAB3Tutorial
# To actually prepare the jobs, please execute prepareCrabJobs.py

from WMCore.Configuration import Configuration
config = Configuration()
import os

config.section_("General")
config.General.requestName = "JOBNAME"
config.General.transferLogs = False

config.section_("JobType")
config.JobType.pluginName = "Analysis"
config.JobType.psetName = "PSET"

## to include local file in the sendbox, this will put the file in the directory where cmsRun runs
config.JobType.inputFiles = ['QGL_AK4chs_94X.db']

## incrase jobs time wall, maximum 2750 minutes (~46 hours)
#config.JobType.maxJobRuntimeMin = 2750

#config.JobType.maxMemoryMB = 2500 # For memory leaks. NB. will block jobs on many sites
## config.JobType.scriptExe = "cmsWrapper.sh"
config.JobType.pyCfgParams = PYCFG_PARAMS
config.JobType.sendPythonFolder = True
config.JobType.sendExternalFolder = True

config.section_("Data")
config.Data.inputDataset = "DATASET"
config.Data.inputDBS = 'global'
config.Data.splitting = "SPLITTING"
config.Data.unitsPerJob = UNITSPERJOB
config.Data.publication = True
config.Data.publishDBS = 'phys03'
config.Data.outputDatasetTag = 'FLASHGG_VERSION-PROCESSED_DSET'
config.Data.outLFNDirBase = "OUTLFN"

config.section_("Site")
config.Site.storageSite = "OUTSITE"
config.Site.blacklist = ["T2_US_Nebraska"]
#config.Site.blacklist = ["T2_UK_London_Brunel","T1_US_FNAL","T2_US_MIT"]

