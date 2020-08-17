import FWCore.ParameterSet.Config as cms

# FIXME invert electron veto
flashggZeeDiPhotons = cms.EDFilter("DiPhotonCandidateSelector",
                                   src = cms.InputTag("flashggDiPhotons"),
                                   cut = cms.string("""
                             leadingPhoton.pt > 33. && subLeadingPhoton.pt>25 && 
                             ( 
                              (leadingPhoton.r9 <= 0.9 
                                    && ( ((leadingPhoton.isEB && leadingPhoton.hadronicOverEm < 0.075 && leadingPhoton.sigmaIetaIeta < 0.014) 
                                    || (leadingPhoton.isEE && leadingPhoton.hadronicOverEm < 0.075 && leadingPhoton.sigmaIetaIeta < 0.034)) 
                                    && (leadingPhoton.hcalTowerSumEtConeDR03 - 0.005 * leadingPhoton.pt < 4.0) 
                                    && (leadingPhoton.trkSumPtHollowConeDR03 - 0.002 * leadingPhoton.pt < 4.0) 
                                    && (leadingPhoton.pfChgIsoWrtChosenVtx02 < 4.0) 
                                    && (leadingPhoton.passElectronVeto)
                                    && (leadPhotonId>-0.1)
                                    ))
                               || (leadingPhoton.r9 > 0.9 
                                    && ( ((leadingPhoton.isEB && leadingPhoton.hadronicOverEm < 0.082 && leadingPhoton.sigmaIetaIeta < 0.014) 
                                    || (leadingPhoton.isEE && leadingPhoton.hadronicOverEm < 0.075 && leadingPhoton.sigmaIetaIeta < 0.034)) 
                                    && (leadingPhoton.hcalTowerSumEtConeDR03 - 0.005 * leadingPhoton.pt < 50.0) 
                                    && (leadingPhoton.trkSumPtHollowConeDR03 - 0.002 * leadingPhoton.pt < 50.0) 
                                    && (leadingPhoton.pfChgIsoWrtChosenVtx02 < 4.0) 
                                    && (leadingPhoton.passElectronVeto)
                                    && (leadPhotonId>-0.1)
                                    )) 
                             ) && ( 
                              (subLeadingPhoton.r9 <= 0.9 
                                    && ( ((subLeadingPhoton.isEB && subLeadingPhoton.hadronicOverEm < 0.075 && subLeadingPhoton.sigmaIetaIeta < 0.014) 
                                    || (subLeadingPhoton.isEE && subLeadingPhoton.hadronicOverEm < 0.075 && subLeadingPhoton.sigmaIetaIeta < 0.034)) 
                                    && (subLeadingPhoton.hcalTowerSumEtConeDR03 - 0.005 * subLeadingPhoton.pt < 4.0) 
                                    && (subLeadingPhoton.trkSumPtHollowConeDR03 - 0.002 * subLeadingPhoton.pt < 4.0) 
                                    && (subLeadingPhoton.pfChgIsoWrtChosenVtx02 < 4.0) 
                                    && (subLeadingPhoton.passElectronVeto)
                                    && (subLeadPhotonId>-0.1)
                                    )) 
                               || (subLeadingPhoton.r9 > 0.9 
                                    && ( ((subLeadingPhoton.isEB && subLeadingPhoton.hadronicOverEm < 0.082 && subLeadingPhoton.sigmaIetaIeta < 0.014) 
                                    || (subLeadingPhoton.isEE && subLeadingPhoton.hadronicOverEm < 0.075 && subLeadingPhoton.sigmaIetaIeta < 0.034)) 
                                    && (subLeadingPhoton.hcalTowerSumEtConeDR03 - 0.005 * subLeadingPhoton.pt < 50.0) 
                                    && (subLeadingPhoton.trkSumPtHollowConeDR03 - 0.002 * subLeadingPhoton.pt < 50.0) 
                                    && (subLeadingPhoton.pfChgIsoWrtChosenVtx02 < 4.0) 
                                    && (subLeadingPhoton.passElectronVeto)
                                    && (subLeadPhotonId>-0.1)
                                    )) 
                             ) 
                            """)
          )

