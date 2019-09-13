# configuration file containing tools for jets
import FWCore.ParameterSet.Config as cms


#===================================================
def loadLocalJECDBfile(process,
                       dbfile = '',
                       tag   = 'JetCorrectorParametersCollection_PY8_RunIISpring15DR74_bx50_MC_AK4PUPPI',
                       label = 'AK4PFPuppi'):
    #===================================================
    # local BD reader
    # load and replace the data base by the local one
    #===================================================
    #print ':: dbfile == ', dbfile
    
    # process.load("CondCore.DBCommon.CondDBCommon_cfi")
    # process.load("CondCore.DBCommon.CondDBSetup_cfi")
    process.load("CondCore.CondDB.CondDB_cfi")
    setattr(process, 'jec' + label,
            cms.ESSource("PoolDBESSource",
                         DBParameters = cms.PSet(
                             messageLevel = cms.untracked.int32(0)
                         ),
                         timetype = cms.string('runnumber'),
                         toGet = cms.VPSet(cms.PSet(
                             record = cms.string('JetCorrectionsRecord'),
                             tag    = cms.string(tag),
                             label  = cms.untracked.string(label)
                         )),
                         connect = cms.string('sqlite_file:%s' % dbfile)
                     ))
    
    setattr(process,  'es_prefer_jec' + label,
            cms.ESPrefer('PoolDBESSource','jec'+ label))
    
#===================================================
