#!/usr/bin/env cmsRun

import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("Analysis")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.source = cms.Source("PoolSource",
                            fileNames=cms.untracked.vstring(
          "file:./myMicroAODOutputFile.root"
        )
)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1000 )

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("test_mmg.root")
)



#from flashgg.MicroAOD.flashggPreselectedDiPhotons_cfi import flashggPreselectedDiPhotons
#process.kinPreselDiPhotons = flashggPreselectedDiPhotons.clone(
#cut=cms.string(
#        "leadingPhoton.pt > 40 && subLeadingPhoton.pt > 30"
#        " && abs(leadingPhoton.superCluster.eta)<2.5 && abs(subLeadingPhoton.superCluster.eta)<2.5 "
#        " && ( abs(leadingPhoton.superCluster.eta)<1.4442 || abs(leadingPhoton.superCluster.eta)>1.566)"
#        " && ( abs(subLeadingPhoton.superCluster.eta)<1.4442 || abs(subLeadingPhoton.superCluster.eta)>1.566)"
#        )
#                                                              )

process.load("flashgg.Taggers.mumugammaDumper_cfi") ##  import mumugammaDumper 
import flashgg.Taggers.dumperConfigTools as cfgTools


#process.mumugammaDumper.src = "kinPreselDiPhotons"

process.mumugammaDumper.dumpTrees = True
process.mumugammaDumper.dumpWorkspace = False
process.mumugammaDumper.quietRooFit = True


# split tree, histogram and datasets by process
process.mumugammaDumper.nameTemplate ="$PROCESS_$SQRTS_$LABEL_$SUBCAT"
## do not split by process
## process.diphotonDumper.nameTemplate = "minitree_$SQRTS_$LABEL_$SUBCAT"

## define categories and associated objects to dump
cfgTools.addCategory(process.mumugammaDumper,
                     "Reject",
                      " !Is2012FSRZMMG ",
                       -1 ## if nSubcat is -1 do not store anythings
                     )

# interestng categories 
cfgTools.addCategories(process.mumugammaDumper,
                       ## categories definition
                       ## cuts are applied in cascade. Events getting to these categories have already failed the "Reject" selection
                       [("EB","abs(MMG_Photon.superCluster.eta)<1.5",0), ##
                        ("EE","abs(MMG_Photon.superCluster.eta)>1.5",0),##("EE","1",0), ## evereything elese is EB+EE
                        ],
                       ## variables to be dumped in trees/datasets. Same variables for all categories
                       ## if different variables wanted for different categories, can add categorie one by one with cfgTools.addCategory
                       variables=["Mass_mmg            :=mass",
                                  "Mass_mumu           :=DiMuPtr.mass", 
                                  "PT_mumu             :=DiMuPtr.pt", 
                                  "leadPt              :=DiMuPtr.leadingMuon.pt",
                                  "subleadPt           :=DiMuPtr.subleadingMuon.pt",
                                  "photonPT            :=MMG_Photon.pt",
                                  #"photonMVAOutput     :=MMG_Photon.PhoIdMvaDWrtVtx(Vertex)",
                                  "photonSCeta         :=MMG_Photon.superCluster.eta"
                                  #"evtrho              :=rho"
                                  ],
                         ## histograms to be plotted. 
                       ## the variables need to be defined first
                       histograms=["Mass_mmg>>mass(160,10,170)",
                                   "Mass_mumu>>mass_mumu(80,20,100)",
                                   "PT_mumu>>pt_mumu(100,0,100)",
                                   "Mass_mmg:Mass_mumu>>MmmgVsMmm(120,30,150:120,30,150)", 
                                   "subleadPt:leadPt>>ptSubVsLead(200,10,210:200,10,210)",
                                   #"evtrho>>eventRho(50, 0, 50)",
                                   "photonPT>>photonpt(100,10,110)",
                                   #"photonMVAOutput>>IDmva(80,0.2,1.0)",
                                   "photonSCeta>>scEta(50,-2.5,2.5)"
                                   #"maxEta>>maxEta[0.,0.1,0.2,0.3,0.4,0.6,0.8,1.0,1.2,1.4442,1.566,1.7,1.8,2.,2.2,2.3,2.5]"
                                   ]
                       )


process.p1 = cms.Path(
       process.mumugammaDumper
    )



from flashgg.MetaData.JobConfig import customize
customize.setDefault("maxEvents",-1)
customize(process)

