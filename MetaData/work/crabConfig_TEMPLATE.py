# CRAB3 config template for flashgg
# More options available on the twiki :
# https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookCRAB3Tutorial
# To actually prepare the jobs, please execute prepareCrabJobs.py

from WMCore.Configuration import Configuration
config = Configuration()

config.section_("General")
config.General.requestName = "JOBNAME"
config.General.transferLogs = False

config.section_("JobType")
config.JobType.pluginName = "Analysis"
config.JobType.psetName = "PSET"
## config.JobType.maxMemoryMB = 3000 # For memory leaks. NB. will block jobs on many sites
## config.JobType.scriptExe = "cmsWrapper.sh"
config.JobType.pyCfgParams = PYCFG_PARAMS
config.JobType.sendPythonFolder = True

config.section_("Data")
config.Data.inputDataset = "DATASET"
config.Data.inputDBS = 'global'
config.Data.splitting = "SPLITTING"
config.Data.unitsPerJob = UNITSPERJOB
config.Data.publication = True
config.Data.publishDBS = 'phys03'
config.Data.publishDataName = 'FLASHGG_VERSION-PROCESSED_DSET'
config.Data.outLFNDirBase = "OUTLFN"

config.section_("Site")
config.Site.storageSite = "T2_CH_CERN"
#config.Site.blacklist = ["T2_CH_CERN"]
#config.Site.blacklist = ["T2_UK_London_Brunel","T1_US_FNAL","T2_US_MIT"]

