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
config.Data.splitting = "FileBased"
config.Data.unitsPerJob = UNITSPERJOB
config.Data.publication = True
config.Data.publishDbsUrl = 'phys03'
config.Data.publishDataName = 'FLASHGG_VERSION'
#config.Data.outlfn = "/store/group/phys_higgs/cmshgg/flashgg"
config.Data.outlfn = "/store/user/obondu/flashgg"

config.section_("Site")
#config.Site.storageSite = "T2_CH_CERN"
config.Site.storageSite = "T2_FR_CCIN2P3"
#config.Site.blacklist = ["T2_CH_CERN"]
