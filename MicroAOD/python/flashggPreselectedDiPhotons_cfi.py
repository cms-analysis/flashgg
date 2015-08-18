import FWCore.ParameterSet.Config as cms

#changed to match trigger logic
#uses objects which don't quite match HLT objects
#needs to be updated with final preselection
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
                                        &&
                                     (leadingPhoton.pt > 14 && leadingPhoton.hadTowOverEm()<0.15 && 
                                     (leadingPhoton.r9()>0.8 || leadingPhoton.chargedHadronIso()<20 
                                      || leadingPhoton.chargedHadronIso()<0.3*leadingPhoton.pt())) &&
                                     (subLeadingPhoton.pt > 14 && subLeadingPhoton.hadTowOverEm()<0.15 &&
                                     (subLeadingPhoton.r9()>0.8 || subLeadingPhoton.chargedHadronIso()<20 
                                      || subLeadingPhoton.chargedHadronIso()<0.3*subLeadingPhoton.pt()))               

                                     """)
                                   )
