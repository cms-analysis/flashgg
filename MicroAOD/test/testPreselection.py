import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("FLASHggMicroAODPreselectionTest")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = 'PLS170_V7AN1::All'
#process.GlobalTag.globaltag = 'auto:run2_mc'
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
from Configuration.AlCa.GlobalTag import GlobalTag

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( 1000) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1000 )

process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc')

# Fix because auto:run2_mc points to MCRUN2_74_V9::All
current_gt = process.GlobalTag.globaltag.value()
if current_gt.count("::All"):
    new_gt = current_gt.replace("::All","")
    print 'Removing "::All" from GlobalTag by hand for condDBv2: was %s, now %s' % (current_gt,new_gt)
    process.GlobalTag.globaltag = new_gt

process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("file:/afs/cern.ch/user/c/carrillo/flashgg/CMSSW_7_4_6_patch2/src/myMicroAODOutputFile.root"))

process.load("flashgg/MicroAOD/flashggMicroAODSequence_cff")

import FWCore.ParameterSet.Config as cms

phoEffArea=cms.PSet( var=cms.string("abs(superCluster.eta)"), bins=cms.vdouble(0.,0.9,1.5,2,2.2,3), vals=cms.vdouble(0.21,0.2,0.14,0.22,0.31) )
neuEffArea=cms.PSet( var=cms.string("abs(superCluster.eta)"), bins=cms.vdouble(0.,0.9,1.5,2,2.2,3), vals=cms.vdouble(0.04,0.059,0.05,0.05,0.15) )

PreselectionVariables = cms.vstring(
        "egChargedHadronIso", 
        "egPhotonIso", 
        "egNeutralHadronIso",
        "hadTowOverEm",
        "(?r9>0.8||egChargedHadronIso<20||egChargedHadronIso/pt<0.3?full5x5_sigmaIetaIeta:sigmaIetaIeta)",
        "passElectronVeto"
        )

rediscoveryPreselection = cms.VPSet(
        cms.PSet(cut=cms.string("abs(superCluster.eta)<1.5 && r9>0.94"),
                 selection = cms.VPSet(
                cms.PSet(max=cms.string("5.95")),
                cms.PSet(max=cms.string("2.87"), 
                         rhocorr=phoEffArea,
                        ),
                cms.PSet(# no neutra iso cut
                    ),
                cms.PSet(max=cms.string("4.53e-1")),
                cms.PSet(min=cms.string("0.001"), max=cms.string("1.05e-2")),
                cms.PSet(min=cms.string("0.5"))
                 ),
                 ),
        cms.PSet(cut=cms.string("abs(superCluster.eta)<1.5 && r9<=0.94"),
                 selection = cms.VPSet(
                cms.PSet(max=cms.string("7.08")),
                cms.PSet(max=cms.string("5.47"), 
                         rhocorr=phoEffArea
                        ),
                cms.PSet(# no neutra iso cut
                    ),
                cms.PSet(max=cms.string("2.12e-1")),
                cms.PSet(min=cms.string("0.001"), max=cms.string("1.05e-2")),
                cms.PSet(min=cms.string("0.5"))
                ),
                ),
         cms.PSet(cut=cms.string("abs(superCluster.eta)>=1.5 && r9>0.94"),
                  selection = cms.VPSet(
                cms.PSet(max=cms.string("6.10")),
                cms.PSet(max=cms.string("5.98"), 
                         rhocorr=phoEffArea),
                cms.PSet(# no neutra iso cut
                    ),
                cms.PSet(max=cms.string("6.3e-2")),
                cms.PSet(min=cms.string("0.001"), max=cms.string("2.82e-2")),
                cms.PSet(min=cms.string("0.5"))
                 ),
                 ),
        cms.PSet(cut=cms.string("abs(superCluster.eta)>=1.5 && r9<=0.94"),
                 selection = cms.VPSet(
                cms.PSet(max=cms.string("5.07")),
                cms.PSet(max=cms.string("3.44"), 
                         rhocorr=phoEffArea),
                cms.PSet(# no neutra iso cut
                    ),
                cms.PSet(max=cms.string("7.8e-2")),
                cms.PSet(min=cms.string("0.001"), max=cms.string("2.80e-2")),
                cms.PSet(min=cms.string("0.5"))
                ),
                 )
        )

lowMassPreselection = cms.VPSet(
        cms.PSet(cut=cms.string("abs(superCluster.eta)<1.5 && r9>0.94"),
                 selection = cms.VPSet(
                cms.PSet(max=cms.string("5.")),
                cms.PSet(max=cms.string("1.+0.002*pt"), 
                         rhocorr=phoEffArea,
                        ),
                cms.PSet(# no neutral iso cut
                    ),
                cms.PSet(max=cms.string("5.e-2")),
                cms.PSet(min=cms.string("0.001"), max=cms.string("1.05e-2")),
                cms.PSet(min=cms.string("0.5"))
                 ),
                 ),
        cms.PSet(cut=cms.string("abs(superCluster.eta)<1.5 && r9<=0.94"),
                 selection = cms.VPSet(
                cms.PSet(max=cms.string("5.")),
#                cms.PSet(max=cms.string("(?1.+0.002*pt<2?2:1.+0.002*pt)"), 
                cms.PSet(max=cms.string("1.+0.002*pt"), 
                         rhocorr=phoEffArea
                        ),
                cms.PSet(# no neutral iso cut
                    ),
                cms.PSet(max=cms.string("5.e-2")),
                cms.PSet(min=cms.string("0.001"), max=cms.string("1.05e-2")),
                cms.PSet(min=cms.string("0.5"))
                ),
                ),
        cms.PSet(cut=cms.string("abs(superCluster.eta)>=1.5 && r9>0.94"),
                 selection = cms.VPSet(
            cms.PSet(max=cms.string("5.")),
            #               cms.PSet(max=cms.string("(?1.+0.002*pt<2
            cms.PSet(max=cms.string("0.002*pt"), 
                     
                     rhocorr=phoEffArea),
            cms.PSet(# no neutral iso cut
                ),
            cms.PSet(max=cms.string("5.e-2")),
            cms.PSet(min=cms.string("0.001"), max=cms.string("2.80e-2")),
            cms.PSet(min=cms.string("0.5"))
            ),
                 )
        )

selectedPreselection = rediscoveryPreselection

process.flashggPreselectedDiPhotonsTEST = cms.EDFilter(
    "DiPhotonCandidateSelector",
    src = cms.InputTag("flashggDiPhotons"),
    rho = cms.InputTag("fixedGridRhoAll"),
    cut = cms.string(
        "    (leadingPhoton.r9>0.8||leadingPhoton.egChargedHadronIso<20||leadingPhoton.egChargedHadronIso/leadingPhoton.pt<0.3)"
        " && (subLeadingPhoton.r9>0.8||subLeadingPhoton.egChargedHadronIso<20||subLeadingPhoton.egChargedHadronIso/subLeadingPhoton.pt<0.3)" 
        )
    ,
    variables = PreselectionVariables,
    categories = selectedPreselection,
    )

process.out = cms.OutputModule("PoolOutputModule", fileName = cms.untracked.string('myMicroAODOutputFile_afterPreselection.root')
                               )

process.p = cms.Path(process.flashggPreselectedDiPhotonsTEST)
process.e = cms.EndPath(process.out)


