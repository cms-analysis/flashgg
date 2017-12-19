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
#config.JobType.inputFiles   = [ os.environ['CMSSW_BASE'] + '/src/'+ 'flashgg/MetaData/data/PY8_RunIISpring15DR74_bx50_MC.db' ]
#config.JobType.inputFiles = ['Fall15_25nsV2_MC.db','Fall15_25nsV2_DATA.db']
#config.JobType.inputFiles = ['Spring16_25nsV6_MC.db','Spring16_25nsV6_DATA.db','QGL_80X.db']
config.JobType.inputFiles = ['Summer16_23Sep2016V4_MC.db','Summer16_23Sep2016AllV4_DATA.db','QGL_80X.db']

## incrase jobs time wall, maximum 2750 minutes (~46 hours)
config.JobType.maxJobRuntimeMin = 2750

config.JobType.maxMemoryMB = 2500 # For memory leaks. NB. will block jobs on many sites
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
#config.Site.blacklist = ["T2_CH_CERN"]
#config.Site.blacklist = ["T2_UK_London_Brunel","T1_US_FNAL","T2_US_MIT"]

