import FWCore.ParameterSet.Config as cms

# FIXME INVERT ELECTRON VETO
flashggPreselectedDiPhotons = cms.EDFilter("DiPhotonCandidateSelector",
                                   src = cms.InputTag("flashggDiPhotons"),
                                   cut = cms.string("""(leadingPhoton.pt > 30. && subLeadingPhoton.pt>18) &&
                                                    (abs(leadingPhoton.superCluster.eta) < 2.5 && abs(subLeadingPhoton.superCluster.eta) < 2.5) &&
                                    (  leadingPhoton.hadronicOverEm < 0.1 && subLeadingPhoton.hadronicOverEm < 0.1) &&
                                    (( leadingPhoton.r9 > 0.5 && leadingPhoton.isEB) || (leadingPhoton.r9 > 0.8 && leadingPhoton.isEE)) &&
                                    (( subLeadingPhoton.r9 > 0.5 && subLeadingPhoton.isEB) || (subLeadingPhoton.r9 > 0.8 && subLeadingPhoton.isEE)) &&
                                    (
                                        ( leadingPhoton.isEB &&
                                        (leadingPhoton.r9>0.85 || 
                                        (leadingPhoton.sigmaIetaIeta < 0.015 && 
                                         leadingPhoton.pfChgIsoWrtChosenVtx02 < 6.0 && 
                                         leadingPhoton.trkSumPtHollowConeDR03 < 6.0 )))  ||
                                        ( leadingPhoton.isEE &&
                                        (leadingPhoton.r9>0.9 || 
                                        (leadingPhoton.sigmaIetaIeta < 0.035 && 
                                         leadingPhoton.pfChgIsoWrtChosenVtx02 < 6.0 && 
                                         leadingPhoton.trkSumPtHollowConeDR03 < 6.0 ))) 
                                     )
                                     &&
                                    (
                                        ( subLeadingPhoton.isEB &&
                                        (subLeadingPhoton.r9>0.85 || 
                                        (subLeadingPhoton.sigmaIetaIeta < 0.015 && 
                                         subLeadingPhoton.pfChgIsoWrtChosenVtx02 < 6.0 && 
                                         subLeadingPhoton.trkSumPtHollowConeDR03 < 6.0 )))  ||
                                        ( subLeadingPhoton.isEE &&
                                        (subLeadingPhoton.r9>0.9 || 
                                        (subLeadingPhoton.sigmaIetaIeta < 0.035 && 
                                         subLeadingPhoton.pfChgIsoWrtChosenVtx02 < 6.0 && 
                                         subLeadingPhoton.trkSumPtHollowConeDR03 < 6.0 ))) 
                                     )
                                        
                                    
                                     """)
                                   )
