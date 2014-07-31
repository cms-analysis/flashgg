import FWCore.ParameterSet.Config as cms

# single photon preselection is mickmicking as much as possible
# what is documented in AN 2013/253 v8 page 41 table 18,
flashggPreselectedDiPhotons = cms.EDFilter("CandViewSelector",
           src = cms.InputTag("flashggDiPhotons"),
           cut = cms.string("""
                             ( 
                              (leadingPhoton.r9 <= 0.9 
                                    && ( ((leadingPhoton.isEB && leadingPhoton.hadronicOverEm < 0.075 && leadingPhoton.sigmaIetaIeta < 0.014) 
                                    || (leadingPhoton.isEE && leadingPhoton.hadronicOverEm < 0.075 && leadingPhoton.sigmaIetaIeta < 0.034)) 
                                    && (leadingPhoton.hcalTowerSumEtConeDR03 - 0.005 * leadingPhoton.pt < 4.0) 
                                    && (leadingPhoton.trkSumPtHollowConeDR03 - 0.002 * leadingPhoton.pt < 4.0) 
                                    && (leadingPhoton.getpfChgIsoWrtChosenVtx02 < 4.0) 
                                    )) 
                               || (leadingPhoton.r9 > 0.9 
                                    && ( ((leadingPhoton.isEB && leadingPhoton.hadronicOverEm < 0.082 && leadingPhoton.sigmaIetaIeta < 0.014) 
                                    || (leadingPhoton.isEE && leadingPhoton.hadronicOverEm < 0.075 && leadingPhoton.sigmaIetaIeta < 0.034)) 
                                    && (leadingPhoton.hcalTowerSumEtConeDR03 - 0.005 * leadingPhoton.pt < 50.0) 
                                    && (leadingPhoton.trkSumPtHollowConeDR03 - 0.002 * leadingPhoton.pt < 50.0) 
                                    && (leadingPhoton.getpfChgIsoWrtChosenVtx02 < 4.0) 
                                    )) 
                             ) && ( 
                              (subLeadingPhoton.r9 <= 0.9 
                                    && ( ((subLeadingPhoton.isEB && subLeadingPhoton.hadronicOverEm < 0.075 && subLeadingPhoton.sigmaIetaIeta < 0.014) 
                                    || (subLeadingPhoton.isEE && subLeadingPhoton.hadronicOverEm < 0.075 && subLeadingPhoton.sigmaIetaIeta < 0.034)) 
                                    && (subLeadingPhoton.hcalTowerSumEtConeDR03 - 0.005 * subLeadingPhoton.pt < 4.0) 
                                    && (subLeadingPhoton.trkSumPtHollowConeDR03 - 0.002 * subLeadingPhoton.pt < 4.0) 
                                    && (subLeadingPhoton.getpfChgIsoWrtChosenVtx02 < 4.0) 
                                    )) 
                               || (subLeadingPhoton.r9 > 0.9 
                                    && ( ((subLeadingPhoton.isEB && subLeadingPhoton.hadronicOverEm < 0.082 && subLeadingPhoton.sigmaIetaIeta < 0.014) 
                                    || (subLeadingPhoton.isEE && subLeadingPhoton.hadronicOverEm < 0.075 && subLeadingPhoton.sigmaIetaIeta < 0.034)) 
                                    && (subLeadingPhoton.hcalTowerSumEtConeDR03 - 0.005 * subLeadingPhoton.pt < 50.0) 
                                    && (subLeadingPhoton.trkSumPtHollowConeDR03 - 0.002 * subLeadingPhoton.pt < 50.0) 
                                    && (subLeadingPhoton.getpfChgIsoWrtChosenVtx02 < 4.0) 
                                    )) 
                             ) 
                            """)
          )

