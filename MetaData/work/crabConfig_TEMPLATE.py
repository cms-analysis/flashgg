# CRAB3 config template for flashgg
# More options available on the twiki :
# https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookCRAB3Tutorial
# To actually prepare the jobs, please execute prepareCrabJobs.py

from WMCore.Configuration import Configuration
config = Configuration()

config.section_("General")
config.General.requestName = "JOBNAME"
config.General.saveLogs = False

config.section_("JobType")
config.JobType.pluginName = "Analysis"
config.JobType.psetName = "PSET"

config.section_("Data")
config.Data.inputDataset = "DATASET"
config.Data.dbsUrl = 'global'
config.Data.splitting = "SPLITTING"
config.Data.unitsPerJob = UNITSPERJOB
config.Data.publication = True
config.Data.publishDbsUrl = 'phys03'
config.Data.publishDataName = 'FLASHGG_VERSION-PROCESSED_DATASET'
config.Data.outlfn = "OUTLFN"

config.section_("Site")
config.Site.storageSite = "OUTSITE"
#config.Site.blacklist = ["T2_CH_CERN"]
