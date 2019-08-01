#ifndef FLASHgg_THQLeptonicTagTruth_h
#define FLASHgg_THQLeptonicTagTruth_h

#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Common/interface/Ptr.h"
#include "flashgg/DataFormats/interface/TagTruthBase.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "flashgg/DataFormats/interface/Muon.h"
#include "flashgg/DataFormats/interface/Electron.h"
#include "DataFormats/Math/interface/deltaR.h"

namespace flashgg {

    class THQLeptonicTagTruth : public TagTruthBase
    {

    public:

        THQLeptonicTagTruth();
        ~THQLeptonicTagTruth();
        //        THQLeptonicTagTruth(const THQLeptonicTagTruth &b);

        // Functions for dumping
        float pt_genJetMatchingToJ1() const { return ( hasClosestGenJetToLeadingJet() ? closestGenJetToLeadingJet()->pt() : -9999. ); }
        float eta_genJetMatchingToJ1() const { return ( hasClosestGenJetToLeadingJet() ? closestGenJetToLeadingJet()->eta() : -9999. );}
        float phi_genJetMatchingToJ1() const { return ( hasClosestGenJetToLeadingJet() ? closestGenJetToLeadingJet()->phi() : -9999. );}
        float pt_genJetMatchingToJ2() const { return ( hasClosestGenJetToSubLeadingJet() ? closestGenJetToSubLeadingJet()->pt() : -9999. );}
        float eta_genJetMatchingToJ2() const { return ( hasClosestGenJetToSubLeadingJet() ? closestGenJetToSubLeadingJet()->eta() : -9999. );}
        float phi_genJetMatchingToJ2() const { return ( hasClosestGenJetToSubLeadingJet() ? closestGenJetToSubLeadingJet()->phi() : -9999. );}
        float pt_genJetMatchingToJ3() const { return ( hasClosestGenJetToSubSubLeadingJet() ? closestGenJetToSubSubLeadingJet()->pt() : -9999. );}
        float eta_genJetMatchingToJ3() const { return ( hasClosestGenJetToSubSubLeadingJet() ? closestGenJetToSubSubLeadingJet()->eta() : -9999. );}
        float phi_genJetMatchingToJ3() const { return ( hasClosestGenJetToSubSubLeadingJet() ? closestGenJetToSubSubLeadingJet()->phi() : -9999. );}
        float pt_genPartMatchingToJ1() const { return ( hasClosestParticleToLeadingJet() ? closestParticleToLeadingJet()->pt() : -9999. );}
        float eta_genPartMatchingToJ1() const { return ( hasClosestParticleToLeadingJet() ? closestParticleToLeadingJet()->eta() : -9999. );}
        float phi_genPartMatchingToJ1() const { return ( hasClosestParticleToLeadingJet() ? closestParticleToLeadingJet()->phi() : -9999. );}
        float pt_genPartMatchingToJ2() const { return ( hasClosestParticleToSubLeadingJet() ? closestParticleToSubLeadingJet()->pt() : -9999. );}
        float eta_genPartMatchingToJ2() const { return ( hasClosestParticleToSubLeadingJet() ? closestParticleToSubLeadingJet()->eta() : -9999. );}
        float phi_genPartMatchingToJ2() const { return ( hasClosestParticleToSubLeadingJet() ? closestParticleToSubLeadingJet()->phi() : -9999. );}
        float pt_genPartMatchingToJ3() const { return ( hasClosestParticleToSubSubLeadingJet() ? closestParticleToSubSubLeadingJet()->pt() : -9999. );}
        float eta_genPartMatchingToJ3() const { return ( hasClosestParticleToSubSubLeadingJet() ? closestParticleToSubSubLeadingJet()->eta() : -9999. );}
        float phi_genPartMatchingToJ3() const { return ( hasClosestParticleToSubSubLeadingJet() ? closestParticleToSubSubLeadingJet()->phi() : -9999. );}
        float pt_genPartMatchingToPho1() const { return ( hasClosestParticleToLeadingPhoton() ? closestParticleToLeadingPhoton()->pt() : -9999. );}
        float eta_genPartMatchingToPho1() const { return ( hasClosestParticleToLeadingPhoton() ? closestParticleToLeadingPhoton()->eta() : -9999. );}
        float phi_genPartMatchingToPho1() const { return ( hasClosestParticleToLeadingPhoton() ? closestParticleToLeadingPhoton()->phi() : -9999. );}
        float pt_genPartMatchingToPho2() const { return ( hasClosestParticleToSubLeadingPhoton() ? closestParticleToSubLeadingPhoton()->pt() : -9999. );}
        float eta_genPartMatchingToPho2() const { return ( hasClosestParticleToSubLeadingPhoton() ? closestParticleToSubLeadingPhoton()->eta() : -9999. );}
        float phi_genPartMatchingToPho2() const { return ( hasClosestParticleToSubLeadingPhoton() ? closestParticleToSubLeadingPhoton()->phi() : -9999. );}

        float pt_genPromptParticleMatchingToLeadingElectron() const {
/*if(hasClosestPromptParticleToLeadingElectron())
{ std::cout<<"confirms a electron"<<std::endl;
std::cout<<"pt of confirmed electron=             "<<closestPromptParticleToLeadingElectron()->pt()<<std::endl;
}*/
 return (hasClosestPromptParticleToLeadingElectron() ? closestPromptParticleToLeadingElectron()->pt() : -9999.);}        

        float pt_P1() const { return ( hasLeadingParton() ? leadingParton()->pt() : -9999. ); }
        float eta_P1() const { return ( hasLeadingParton() ? leadingParton()->eta() : -9999. ); }
        float phi_P1() const { return ( hasLeadingParton() ? leadingParton()->phi() : -9999. ); }
        float pt_P2() const { return ( hasSubLeadingParton() ? subLeadingParton()->pt() : -9999. ); }
        float eta_P2() const { return ( hasSubLeadingParton() ? subLeadingParton()->eta() : -9999. ); }
        float phi_P2() const { return ( hasSubLeadingParton() ? subLeadingParton()->phi() : -9999. ); }
        float pt_P3() const { return ( hasSubSubLeadingParton() ? subSubLeadingParton()->pt() : -9999. ); }
        float eta_P3() const { return ( hasSubSubLeadingParton() ? subSubLeadingParton()->eta() : -9999. ); }
        float phi_P3() const { return ( hasSubSubLeadingParton() ? subSubLeadingParton()->phi() : -9999. ); }
        float pt_J1() const { return ( hasLeadingJet() ? leadingJet()->pt() : -9999. ); }
        float eta_J1() const { return ( hasLeadingJet() ? leadingJet()->eta() : -9999. ); }
        float phi_J1() const { return ( hasLeadingJet() ? leadingJet()->phi() : -9999. ); }
        float pt_J2() const { return ( hasSubLeadingJet() ? subLeadingJet()->pt() : -9999. ); }
        float eta_J2() const { return ( hasSubLeadingJet() ? subLeadingJet()->eta() : -9999. ); }
        float phi_J2() const { return ( hasSubLeadingJet() ? subLeadingJet()->phi() : -9999. ); }
        float pt_J3() const { return ( hasSubSubLeadingJet() ? subSubLeadingJet()->pt() : -9999. ); }
        float eta_J3() const { return ( hasSubSubLeadingJet() ? subSubLeadingJet()->eta() : -9999. ); }
        float phi_J3() const { return ( hasSubSubLeadingJet() ? subSubLeadingJet()->phi() : -9999. ); }

        //DeltaRs between Jet and truth
        float dR_genJetMatchingToJ1() const { return ( hasClosestGenJetToLeadingJet() ? deltaR(closestGenJetToLeadingJet()->eta(),closestGenJetToLeadingJet()->phi(),
                                                                                               eta_J1(),phi_J1()) : -9999. );}
        float dR_genJetMatchingToJ2() const { return ( hasClosestGenJetToSubLeadingJet() ? deltaR(closestGenJetToSubLeadingJet()->eta(),closestGenJetToSubLeadingJet()->phi(),
                                                                                               eta_J2(),phi_J2()) : -9999. );}
        float dR_genJetMatchingToJ3() const { return ( hasClosestGenJetToSubSubLeadingJet() ? deltaR(closestGenJetToSubSubLeadingJet()->eta(),closestGenJetToSubSubLeadingJet()->phi(),
                                                                                               eta_J3(),phi_J3()) : -9999. );}
        float dR_particleMatchingToJ1() const { return ( hasClosestParticleToLeadingJet() ? deltaR(closestParticleToLeadingJet()->eta(),closestParticleToLeadingJet()->phi(),
                                                                                               eta_J1(),phi_J1()) : -9999. );}
        float dR_particleMatchingToJ2() const { return ( hasClosestParticleToSubLeadingJet() ? deltaR(closestParticleToSubLeadingJet()->eta(),closestParticleToSubLeadingJet()->phi(),
                                                                                               eta_J2(),phi_J2()) : -9999. );}
        float dR_particleMatchingToJ3() const { return ( hasClosestParticleToSubSubLeadingJet() ? deltaR(closestParticleToSubSubLeadingJet()->eta(),closestParticleToSubSubLeadingJet()->phi(),
                                                                                               eta_J3(),phi_J3()) : -9999. );}
        float dR_partonMatchingToJ1() const { return ( hasLeadingJet() && hasClosestPartonToLeadingJet() ? deltaR(closestPartonToLeadingJet()->eta(),closestPartonToLeadingJet()->phi(),
                                                                                               eta_J1(),phi_J1()) : -9999. );}
        float dR_partonMatchingToJ2() const { return ( hasSubLeadingJet() && hasClosestPartonToSubLeadingJet() ? deltaR(closestPartonToSubLeadingJet()->eta(),closestPartonToSubLeadingJet()->phi(),
                                                                                               eta_J2(),phi_J2()) : -9999. );}
        float dR_partonMatchingToJ3() const { return ( hasSubSubLeadingJet() && hasClosestPartonToSubSubLeadingJet() ? deltaR(closestPartonToSubSubLeadingJet()->eta(),closestPartonToSubSubLeadingJet()->phi(),
                                                                                               eta_J3(),phi_J3()) : -9999. );}
        float pt_genParticleMatchingToLeadingMuon() const {return (hasClosestParticleToLeadingMuon() ? closestParticleToLeadingMuon()->pt() : -8888);}                 
 


        //Delta Rs between jets
        float dR_J1J2_FggJet() const {if(hasLeadingJet() && hasSubLeadingJet()) {return deltaR(leadingJet()->eta(),leadingJet()->phi(),
                                                                               subLeadingJet()->eta(),subLeadingJet()->phi()); }else{return -9999.;}}
        float dR_J1J3_FggJet() const {if(hasLeadingJet() && hasSubSubLeadingJet()) {return deltaR(leadingJet()->eta(),leadingJet()->phi(),
                                                                               subSubLeadingJet()->eta(),subSubLeadingJet()->phi()); }else{return -9999.;}}
        float dR_J2J3_FggJet() const {if(hasSubLeadingJet() && hasSubSubLeadingJet()) {return deltaR(subLeadingJet()->eta(),subLeadingJet()->phi(),
                                                                               subSubLeadingJet()->eta(),subSubLeadingJet()->phi()); }else{return -9999.;}}
        float dR_min_J13J23_FggJet() const {if (hasLeadingJet() && hasSubLeadingJet() && hasSubSubLeadingJet()) {
                                            return dR_J1J3_FggJet() < dR_J2J3_FggJet() ? dR_J1J3_FggJet() : dR_J2J3_FggJet();
                                           }else{return -9999.;}}
        float dR_J1J2_GenJet() const {if(hasClosestGenJetToLeadingJet() && hasClosestGenJetToSubLeadingJet()) {
                                                                 return deltaR(closestGenJetToLeadingJet()->eta(),closestGenJetToLeadingJet()->phi(),
                                                                               closestGenJetToSubLeadingJet()->eta(),closestGenJetToSubLeadingJet()->phi());
                                     }else{return -9999.;}}
        float dR_J1J3_GenJet() const {if(hasClosestGenJetToLeadingJet() && hasClosestGenJetToSubSubLeadingJet()) {
                                                                 return deltaR(closestGenJetToLeadingJet()->eta(),closestGenJetToLeadingJet()->phi(),
                                                                               closestGenJetToSubSubLeadingJet()->eta(),closestGenJetToSubSubLeadingJet()->phi());
                                     }else{return -9999.;}}
        float dR_J2J3_GenJet() const {if(hasClosestGenJetToSubLeadingJet() && hasClosestGenJetToSubSubLeadingJet()) {
                                                                 return deltaR(closestGenJetToSubLeadingJet()->eta(),closestGenJetToSubLeadingJet()->phi(),
                                                                               closestGenJetToSubSubLeadingJet()->eta(),closestGenJetToSubSubLeadingJet()->phi());
                                     }else{return -9999.;}}
        float dR_J1J2_GenParticle() const {if(hasClosestParticleToLeadingJet() && hasClosestParticleToSubLeadingJet()) {
                                                                 return deltaR(closestParticleToLeadingJet()->eta(),closestParticleToLeadingJet()->phi(),
                                                                               closestParticleToSubLeadingJet()->eta(),closestParticleToSubLeadingJet()->phi());
                                     }else{return -9999.;}}
        float dR_J1J3_GenParticle() const {if(hasClosestParticleToLeadingJet() && hasClosestParticleToSubSubLeadingJet()) {
                                                                 return deltaR(closestParticleToLeadingJet()->eta(),closestParticleToLeadingJet()->phi(),
                                                                               closestParticleToSubSubLeadingJet()->eta(),closestParticleToSubSubLeadingJet()->phi());
                                     }else{return -9999.;}}
        float dR_J2J3_GenParticle() const {if(hasClosestParticleToSubLeadingJet() && hasClosestParticleToSubSubLeadingJet()) {
                                                                 return deltaR(closestParticleToSubLeadingJet()->eta(),closestParticleToSubLeadingJet()->phi(),
                                                                               closestParticleToSubSubLeadingJet()->eta(),closestParticleToSubSubLeadingJet()->phi());
                                     }else{return -9999.;}}
        float dR_P1P2_Partons() const {if(hasLeadingParton() && hasSubLeadingParton()) { return deltaR(leadingParton()->eta(),leadingParton()->phi(),
                                                                                                       subLeadingParton()->eta(),subLeadingParton()->phi());
                                      }else{return -9999.;}}
        float dR_P1P3_Partons() const {if(hasLeadingParton() && hasSubSubLeadingParton()) { return deltaR(leadingParton()->eta(),leadingParton()->phi(),
                                                                                                          subSubLeadingParton()->eta(),subSubLeadingParton()->phi());
                                      }else{return -9999.;}}
        float dR_P2P3_Partons() const {if(hasSubLeadingParton() && hasSubSubLeadingParton()) { return deltaR(subLeadingParton()->eta(),subLeadingParton()->phi(),
                                                                                                             subSubLeadingParton()->eta(),subSubLeadingParton()->phi());
                                      }else{return -9999.;}}

        //DeltaRs between dijets and diphotons
        float dR_DP_12_FggJet() const {if (hasLeadingJet() && hasSubLeadingJet() && hasSubSubLeadingJet()) {
                                       return deltaR(diPhoton()->eta(),diPhoton()->phi(),(leadingJet()->p4()+subLeadingJet()->p4()).eta(),
                                                    (leadingJet()->p4()+subLeadingJet()->p4()).phi());}else{return -9999.;}}
        float dR_DP_13_FggJet() const {if (hasLeadingJet() && hasSubLeadingJet() && hasSubSubLeadingJet()) {
                                       return deltaR(diPhoton()->eta(),diPhoton()->phi(),(leadingJet()->p4()+subSubLeadingJet()->p4()).eta(),
                                                    (leadingJet()->p4()+subSubLeadingJet()->p4()).phi());}else{return -9999.;}}
        float dR_DP_23_FggJet() const {if (hasLeadingJet() && hasSubLeadingJet() && hasSubSubLeadingJet()) {
                                       return deltaR(diPhoton()->eta(),diPhoton()->phi(),(subLeadingJet()->p4()+subSubLeadingJet()->p4()).eta(),
                                                    (subLeadingJet()->p4()+subSubLeadingJet()->p4()).phi());}else{return -9999.;}}
        float dR_DP_12_GenJet() const {if (hasClosestGenJetToLeadingJet() && hasClosestGenJetToSubLeadingJet() && hasClosestGenJetToSubSubLeadingJet()) {
                                       return deltaR(diPhoton()->eta(),diPhoton()->phi(),(closestGenJetToLeadingJet()->p4()+closestGenJetToSubLeadingJet()->p4()).eta(),
                                                    (closestGenJetToLeadingJet()->p4()+closestGenJetToSubLeadingJet()->p4()).phi());}else{return -9999.;}}
        float dR_DP_13_GenJet() const {if (hasClosestGenJetToLeadingJet() && hasClosestGenJetToSubLeadingJet() && hasClosestGenJetToSubSubLeadingJet()) {
                                       return deltaR(diPhoton()->eta(),diPhoton()->phi(),(closestGenJetToLeadingJet()->p4()+closestGenJetToSubSubLeadingJet()->p4()).eta(),
                                                    (closestGenJetToLeadingJet()->p4()+closestGenJetToSubSubLeadingJet()->p4()).phi());}else{return -9999.;}}
        float dR_DP_23_GenJet() const {if (hasClosestGenJetToLeadingJet() && hasClosestGenJetToSubLeadingJet() && hasClosestGenJetToSubSubLeadingJet()) {
                                       return deltaR(diPhoton()->eta(),diPhoton()->phi(),(closestGenJetToSubLeadingJet()->p4()+closestGenJetToSubSubLeadingJet()->p4()).eta(),
                                                    (closestGenJetToSubLeadingJet()->p4()+closestGenJetToSubSubLeadingJet()->p4()).phi());}else{return -9999.;}}
        float dR_DP_12_GenParticle() const {if (hasClosestParticleToLeadingJet() && hasClosestParticleToSubLeadingJet() && hasClosestParticleToSubSubLeadingJet()) {
                                       return deltaR(diPhoton()->eta(),diPhoton()->phi(),(closestParticleToLeadingJet()->p4()+closestParticleToSubLeadingJet()->p4()).eta(),
                                                    (closestParticleToLeadingJet()->p4()+closestParticleToSubLeadingJet()->p4()).phi());}else{return -9999.;}}
        float dR_DP_13_GenParticle() const {if (hasClosestParticleToLeadingJet() && hasClosestParticleToSubLeadingJet() && hasClosestParticleToSubSubLeadingJet()) {
                                       return deltaR(diPhoton()->eta(),diPhoton()->phi(),(closestParticleToLeadingJet()->p4()+closestParticleToSubSubLeadingJet()->p4()).eta(),
                                                    (closestParticleToLeadingJet()->p4()+closestParticleToSubSubLeadingJet()->p4()).phi());}else{return -9999.;}}
        float dR_DP_23_GenParticle() const {if (hasClosestParticleToLeadingJet() && hasClosestParticleToSubLeadingJet() && hasClosestParticleToSubSubLeadingJet()) {
                                       return deltaR(diPhoton()->eta(),diPhoton()->phi(),(closestParticleToSubLeadingJet()->p4()+closestParticleToSubSubLeadingJet()->p4()).eta(),
                                                    (closestParticleToSubLeadingJet()->p4()+closestParticleToSubSubLeadingJet()->p4()).phi());}else{return -9999.;}}
        float dR_DP_12_Partons() const {if (hasLeadingParton() && hasSubLeadingParton() && hasSubSubLeadingParton()) {
                                       return deltaR(diPhoton()->eta(),diPhoton()->phi(),(leadingParton()->p4()+subLeadingParton()->p4()).eta(),
                                                    (leadingParton()->p4()+subLeadingParton()->p4()).phi());}else{return -9999.;}}
        float dR_DP_13_Partons() const {if (hasLeadingParton() && hasSubLeadingParton() && hasSubSubLeadingParton()) {
                                       return deltaR(diPhoton()->eta(),diPhoton()->phi(),(leadingParton()->p4()+subSubLeadingParton()->p4()).eta(),
                                                    (leadingParton()->p4()+subSubLeadingParton()->p4()).phi());}else{return -9999.;}}
        float dR_DP_23_Partons() const {if (hasLeadingParton() && hasSubLeadingParton() && hasSubSubLeadingParton()) {
                                       return deltaR(diPhoton()->eta(),diPhoton()->phi(),(subLeadingParton()->p4()+subSubLeadingParton()->p4()).eta(),
                                                    (subLeadingParton()->p4()+subSubLeadingParton()->p4()).phi());}else{return -9999.;}}

        //DeltaRs between individual jets and photons
        float dR_Ph1_1_FggJet() const {if (hasLeadingJet()) { return deltaR(diPhoton()->leadingPhoton()->eta(),diPhoton()->leadingPhoton()->phi(),
                                                                     leadingJet()->eta(),leadingJet()->phi()); }else{return -9999.;}}
        float dR_Ph1_2_FggJet() const {if (hasSubLeadingJet()) { return deltaR(diPhoton()->leadingPhoton()->eta(),diPhoton()->leadingPhoton()->phi(),
                                                                     subLeadingJet()->eta(),subLeadingJet()->phi()); }else{return -9999.;}}
        float dR_Ph1_3_FggJet() const {if (hasSubSubLeadingJet()) { return deltaR(diPhoton()->leadingPhoton()->eta(),diPhoton()->leadingPhoton()->phi(),
                                                                     subSubLeadingJet()->eta(),subSubLeadingJet()->phi()); }else{return -9999.;}}
        float dR_Ph2_1_FggJet() const {if (hasLeadingJet()) { return deltaR(diPhoton()->subLeadingPhoton()->eta(),diPhoton()->subLeadingPhoton()->phi(),
                                                                     leadingJet()->eta(),leadingJet()->phi()); }else{return -9999.;}}
        float dR_Ph2_2_FggJet() const {if (hasSubLeadingJet()) { return deltaR(diPhoton()->subLeadingPhoton()->eta(),diPhoton()->subLeadingPhoton()->phi(),
                                                                     subLeadingJet()->eta(),subLeadingJet()->phi()); }else{return -9999.;}}
        float dR_Ph2_3_FggJet() const {if (hasSubSubLeadingJet()) { return deltaR(diPhoton()->subLeadingPhoton()->eta(),diPhoton()->subLeadingPhoton()->phi(),
                                                                     subSubLeadingJet()->eta(),subSubLeadingJet()->phi()); }else{return -9999.;}}
        float dR_Ph1_1_GenJet() const {if (hasClosestGenJetToLeadingJet()) { return deltaR(diPhoton()->leadingPhoton()->eta(),diPhoton()->leadingPhoton()->phi(),
                                                                     closestGenJetToLeadingJet()->eta(),closestGenJetToLeadingJet()->phi()); }else{return -9999.;}}
        float dR_Ph1_2_GenJet() const {if (hasClosestGenJetToSubLeadingJet()) { return deltaR(diPhoton()->leadingPhoton()->eta(),diPhoton()->leadingPhoton()->phi(),
                                                                     subLeadingJet()->eta(),subLeadingJet()->phi()); }else{return -9999.;}}
        float dR_Ph1_3_GenJet() const {if (hasClosestGenJetToSubSubLeadingJet()) { return deltaR(diPhoton()->leadingPhoton()->eta(),diPhoton()->leadingPhoton()->phi(),
                                                                     subSubLeadingJet()->eta(),subSubLeadingJet()->phi()); }else{return -9999.;}}
        float dR_Ph2_1_GenJet() const {if (hasClosestGenJetToLeadingJet()) { return deltaR(diPhoton()->subLeadingPhoton()->eta(),diPhoton()->subLeadingPhoton()->phi(),
                                                                     closestGenJetToLeadingJet()->eta(),closestGenJetToLeadingJet()->phi()); }else{return -9999.;}}
        float dR_Ph2_2_GenJet() const {if (hasClosestGenJetToSubLeadingJet()) { return deltaR(diPhoton()->subLeadingPhoton()->eta(),diPhoton()->subLeadingPhoton()->phi(),
                                                                     subLeadingJet()->eta(),subLeadingJet()->phi()); }else{return -9999.;}}
        float dR_Ph2_3_GenJet() const {if (hasClosestGenJetToSubSubLeadingJet()) { return deltaR(diPhoton()->subLeadingPhoton()->eta(),diPhoton()->subLeadingPhoton()->phi(),
                                                                     subSubLeadingJet()->eta(),subSubLeadingJet()->phi()); }else{return -9999.;}}
        float dR_Ph1_1_GenParticle() const {if (hasClosestParticleToLeadingJet()) { return deltaR(diPhoton()->leadingPhoton()->eta(),diPhoton()->leadingPhoton()->phi(),
                                                                     closestParticleToLeadingJet()->eta(),closestParticleToLeadingJet()->phi()); }else{return -9999.;}}
        float dR_Ph1_2_GenParticle() const {if (hasClosestParticleToSubLeadingJet()) { return deltaR(diPhoton()->leadingPhoton()->eta(),diPhoton()->leadingPhoton()->phi(),
                                                                     subLeadingJet()->eta(),subLeadingJet()->phi()); }else{return -9999.;}}
        float dR_Ph1_3_GenParticle() const {if (hasClosestParticleToSubSubLeadingJet()) { return deltaR(diPhoton()->leadingPhoton()->eta(),diPhoton()->leadingPhoton()->phi(),
                                                                     subSubLeadingJet()->eta(),subSubLeadingJet()->phi()); }else{return -9999.;}}
        float dR_Ph2_1_GenParticle() const {if (hasClosestParticleToLeadingJet()) { return deltaR(diPhoton()->subLeadingPhoton()->eta(),diPhoton()->subLeadingPhoton()->phi(),
                                                                     closestParticleToLeadingJet()->eta(),closestParticleToLeadingJet()->phi()); }else{return -9999.;}}
        float dR_Ph2_2_GenParticle() const {if (hasClosestParticleToSubLeadingJet()) { return deltaR(diPhoton()->subLeadingPhoton()->eta(),diPhoton()->subLeadingPhoton()->phi(),
                                                                     subLeadingJet()->eta(),subLeadingJet()->phi()); }else{return -9999.;}}
        float dR_Ph2_3_GenParticle() const {if (hasClosestParticleToSubSubLeadingJet()) { return deltaR(diPhoton()->subLeadingPhoton()->eta(),diPhoton()->subLeadingPhoton()->phi(),
                                                                     subSubLeadingJet()->eta(),subSubLeadingJet()->phi()); }else{return -9999.;}}
        float dR_Ph1_1_Partons() const {if (hasLeadingParton()) { return deltaR(diPhoton()->leadingPhoton()->eta(),diPhoton()->leadingPhoton()->phi(),
                                                                     leadingParton()->eta(),leadingParton()->phi()); }else{return -9999.;}}
        float dR_Ph1_2_Partons() const {if (hasSubLeadingParton()) { return deltaR(diPhoton()->leadingPhoton()->eta(),diPhoton()->leadingPhoton()->phi(),
                                                                     subLeadingParton()->eta(),subLeadingParton()->phi()); }else{return -9999.;}}
        float dR_Ph1_3_Partons() const {if (hasSubSubLeadingParton()) { return deltaR(diPhoton()->leadingPhoton()->eta(),diPhoton()->leadingPhoton()->phi(),
                                                                     subSubLeadingParton()->eta(),subSubLeadingParton()->phi()); }else{return -9999.;}}
        float dR_Ph2_1_Partons() const {if (hasLeadingParton()) { return deltaR(diPhoton()->subLeadingPhoton()->eta(),diPhoton()->subLeadingPhoton()->phi(),
                                                                     leadingParton()->eta(),leadingParton()->phi()); }else{return -9999.;}}
        float dR_Ph2_2_Partons() const {if (hasSubLeadingParton()) { return deltaR(diPhoton()->subLeadingPhoton()->eta(),diPhoton()->subLeadingPhoton()->phi(),
                                                                     subLeadingParton()->eta(),subLeadingParton()->phi()); }else{return -9999.;}}
        float dR_Ph2_3_Partons() const {if (hasSubSubLeadingParton()) { return deltaR(diPhoton()->subLeadingPhoton()->eta(),diPhoton()->subLeadingPhoton()->phi(),
                                                                     subSubLeadingParton()->eta(),subSubLeadingParton()->phi()); }else{return -9999.;}}
        //dR between the trijet and diphoton
        float dR_DP_123_FggJet() const {if (hasLeadingJet() && hasSubLeadingJet() && hasSubSubLeadingJet()) {
                                       return deltaR(diPhoton()->eta(),diPhoton()->phi(),(leadingJet()->p4()+subLeadingJet()->p4()+subSubLeadingJet()->p4()).eta(),
                                                    (leadingJet()->p4()+subLeadingJet()->p4()+subSubLeadingJet()->p4()).phi());}else{return -9999.;}}
        float dR_DP_123_GenJet() const {if (hasClosestGenJetToLeadingJet() && hasClosestGenJetToSubLeadingJet() && hasClosestGenJetToSubSubLeadingJet()) {
                                       return deltaR(diPhoton()->eta(),diPhoton()->phi(),   
                                                    (closestGenJetToLeadingJet()->p4()+closestGenJetToSubLeadingJet()->p4()+closestGenJetToSubSubLeadingJet()->p4()).eta(),
                                                    (closestGenJetToLeadingJet()->p4()+closestGenJetToSubLeadingJet()->p4()+closestGenJetToSubSubLeadingJet()->p4()).phi());}
                                        else{return -9999.;}}
        float dR_DP_123_GenParticle() const {if (hasClosestParticleToLeadingJet() && hasClosestParticleToSubLeadingJet() && hasClosestParticleToSubSubLeadingJet()) {
                                       return deltaR(diPhoton()->eta(),diPhoton()->phi(),   
                                                    (closestParticleToLeadingJet()->p4()+closestParticleToSubLeadingJet()->p4()+closestParticleToSubSubLeadingJet()->p4()).eta(),
                                                    (closestParticleToLeadingJet()->p4()+closestParticleToSubLeadingJet()->p4()+closestParticleToSubSubLeadingJet()->p4()).phi());}
                                        else{return -9999.;}}
        float dR_DP_123_Partons() const {if (hasLeadingParton() && hasSubLeadingParton() && hasSubSubLeadingParton()) {
                                       return deltaR(diPhoton()->eta(),diPhoton()->phi(),(leadingParton()->p4()+subLeadingParton()->p4()+subSubLeadingParton()->p4()).eta(),
                                                    (leadingParton()->p4()+subLeadingParton()->p4()+subSubLeadingParton()->p4()).phi());}else{return -9999.;}}

        //Has (thing) methods
        bool hasClosestGenJetToLeadingJet() const {
//if(closestGenJetToLeadingJet_.isNonnull()) std::cout<<"has closest gen jet to leading jet"<<std::endl;
//else std::cout<<"don't has closest gen jet to leading jet"<<std::endl;
return closestGenJetToLeadingJet_.isNonnull(); }
        bool hasClosestGenJetToSubLeadingJet() const { return closestGenJetToSubLeadingJet_.isNonnull(); }
        bool hasClosestGenJetToSubSubLeadingJet() const { return closestGenJetToSubSubLeadingJet_.isNonnull(); }
        bool hasClosestParticleToLeadingJet() const { return closestParticleToLeadingJet_.isNonnull(); }
        bool hasClosestParticleToSubLeadingJet() const { return closestParticleToSubLeadingJet_.isNonnull(); }
        bool hasClosestParticleToSubSubLeadingJet() const { return closestParticleToSubSubLeadingJet_.isNonnull(); }
        bool hasClosestPartonToLeadingJet() const { return closestPartonToLeadingJet_.isNonnull(); }
        bool hasClosestPartonToSubLeadingJet() const { return closestPartonToSubLeadingJet_.isNonnull(); }
        bool hasClosestPartonToSubSubLeadingJet() const { return closestPartonToSubSubLeadingJet_.isNonnull(); }
        bool hasClosestParticleToLeadingPhoton() const { return closestParticleToLeadingPhoton_.isNonnull(); }
        bool hasClosestParticleToSubLeadingPhoton() const { return closestParticleToSubLeadingPhoton_.isNonnull(); }
        bool hasClosestParticleToLeadingMuon() const { return closestParticleToLeadingMuon_.isNonnull(); }
        bool hasClosestParticleToSubLeadingMuon() const { return closestParticleToSubLeadingMuon_.isNonnull(); }
        bool hasClosestParticleToLeadingElectron() const { return closestParticleToLeadingElectron_.isNonnull(); }
        bool hasClosestParticleToSubLeadingElectron() const { return closestParticleToSubLeadingElectron_.isNonnull(); }
        bool hasClosestPromptParticleToLeadingMuon() const { return closestPromptParticleToLeadingMuon_.isNonnull(); }
        bool hasClosestPromptParticleToSubLeadingMuon() const { return closestPromptParticleToSubLeadingMuon_.isNonnull(); }
        bool hasClosestPromptParticleToLeadingElectron() const {
//std::cout<<"pt of prompt electron"<<closestPromptParticleToLeadingElectron_->pt()<<std::endl;
//if(closestPromptParticleToLeadingElectron_ != NULL) std::cout<<"has closest prompt electron!!!!!"<<std::endl;
//if(closestPromptParticleToLeadingElectron_.isNonnull()){
//std::cout<<"pt of prompt electron"<<closestPromptParticleToLeadingElectron_->pt()<<std::endl;
//std::cout<<"has closest prompt electron"<<std::endl;
//}
//else std::cout<<"don't has closest prompt electron"<<std::endl;

return closestPromptParticleToLeadingElectron_.isNonnull(); }
        bool hasClosestPromptParticleToSubLeadingElectron() const { return closestPromptParticleToSubLeadingElectron_.isNonnull(); }
        bool hasDiPhoton() const { return diPhoton_.isNonnull(); }
        bool hasLeadingJet() const { return leadingJet_.isNonnull(); }
        bool hasSubLeadingJet() const { return subLeadingJet_.isNonnull(); }
        bool hasSubSubLeadingJet() const { return subSubLeadingJet_.isNonnull(); }
        bool hasLeadingParton() const { return leadingParton_.isNonnull(); }
        bool hasSubLeadingParton() const { return subLeadingParton_.isNonnull(); }
        bool hasSubSubLeadingParton() const { return subSubLeadingParton_.isNonnull(); }
        bool hasLeadingGenJet() const { return leadingGenJet_.isNonnull(); }
        bool hasSubLeadingGenJet() const { return subLeadingGenJet_.isNonnull(); }
        bool hasSubSubLeadingGenJet() const { return subSubLeadingGenJet_.isNonnull(); }
        bool hasDijet() const {return numberOfFggJets() > 1;} bool hasTrijet() const {return numberOfFggJets() > 2;}
        bool hasLeadingMuon() const { return leadingMuon_.isNonnull(); }
        bool hasSubLeadingMuon() const { return subLeadingMuon_.isNonnull(); }
        bool hasLeadingElectron() const { return leadingElectron_.isNonnull(); }
        bool hasSubLeadingElectron() const { return subLeadingElectron_.isNonnull(); }
        //bool hasGenMET() const { return genMET_.isNonnull(); }

        //Getter methods
        const edm::Ptr<reco::GenParticle> leadingParton() const { return leadingParton_; }
        const edm::Ptr<reco::GenParticle> subLeadingParton() const { return subLeadingParton_; }
        const edm::Ptr<reco::GenParticle> subSubLeadingParton() const { return subSubLeadingParton_; }
        const edm::Ptr<reco::GenJet> leadingGenJet() const { return leadingGenJet_; }
        const edm::Ptr<reco::GenJet> subLeadingGenJet() const { return subLeadingGenJet_; }
        const edm::Ptr<reco::GenJet> subSubLeadingGenJet() const { return subSubLeadingGenJet_; }
        const edm::Ptr<flashgg::Jet> leadingJet() const {return leadingJet_; }
        const edm::Ptr<flashgg::Jet> subLeadingJet() const {return subLeadingJet_; }
        const edm::Ptr<flashgg::Jet> subSubLeadingJet() const {return subSubLeadingJet_; }
        const edm::Ptr<flashgg::DiPhotonCandidate> diPhoton() const { return diPhoton_; }
        const edm::Ptr<flashgg::Muon> leadingMuon() const {return leadingMuon_; }
        const edm::Ptr<flashgg::Muon> subLeadingMuon() const {return subLeadingMuon_; }
        const edm::Ptr<flashgg::Electron> leadingElectron() const {return leadingElectron_; }
        const edm::Ptr<flashgg::Electron> subLeadingElectron() const {return subLeadingElectron_; } 
        const std::vector<edm::Ptr<reco::GenParticle>> ptOrderedPartons() const {return ptOrderedPartons_;}
        const std::vector<edm::Ptr<reco::GenJet>> ptOrderedGenJets() const {return ptOrderedGenJets_;}
        const std::vector<edm::Ptr<flashgg::Jet>> ptOrderedFggJets() const {return ptOrderedFggJets_;}

        //More Getter methods
        const edm::Ptr<reco::GenJet> closestGenJetToLeadingJet() const { return closestGenJetToLeadingJet_; }
        const edm::Ptr<reco::GenJet> closestGenJetToSubLeadingJet() const { return closestGenJetToSubLeadingJet_; }
        const edm::Ptr<reco::GenJet> closestGenJetToSubSubLeadingJet() const { return closestGenJetToSubSubLeadingJet_; }
        const edm::Ptr<reco::GenParticle> closestParticleToLeadingJet() const { return closestParticleToLeadingJet_; }
        const edm::Ptr<reco::GenParticle> closestParticleToSubLeadingJet() const { return closestParticleToSubLeadingJet_; }
        const edm::Ptr<reco::GenParticle> closestParticleToSubSubLeadingJet() const { return closestParticleToSubSubLeadingJet_; }
        const edm::Ptr<reco::GenParticle> closestPartonToLeadingJet() const { return closestPartonToLeadingJet_; }
        const edm::Ptr<reco::GenParticle> closestPartonToSubLeadingJet() const { return closestPartonToSubLeadingJet_; }
        const edm::Ptr<reco::GenParticle> closestPartonToSubSubLeadingJet() const { return closestPartonToSubSubLeadingJet_; }
        const edm::Ptr<reco::GenParticle> closestParticleToLeadingPhoton() const { return closestParticleToLeadingPhoton_; }
        const edm::Ptr<reco::GenParticle> closestParticleToSubLeadingPhoton() const { return closestParticleToSubLeadingPhoton_; }
        const edm::Ptr<reco::GenParticle> closestParticleToLeadingMuon() const { return closestParticleToLeadingMuon_; }
        const edm::Ptr<reco::GenParticle> closestParticleToSubLeadingMuon() const { return closestParticleToSubLeadingMuon_; }
        const edm::Ptr<reco::GenParticle> closestParticleToLeadingElectron() const { return closestParticleToLeadingElectron_; }
        const edm::Ptr<reco::GenParticle> closestParticleToSubLeadingElectron() const { return closestParticleToSubLeadingElectron_; }
        const edm::Ptr<reco::GenParticle> closestPromptParticleToLeadingMuon() const { return closestPromptParticleToLeadingMuon_; }
        const edm::Ptr<reco::GenParticle> closestPromptParticleToSubLeadingMuon() const { return closestPromptParticleToSubLeadingMuon_; }
        const edm::Ptr<reco::GenParticle> closestPromptParticleToLeadingElectron() const { return closestPromptParticleToLeadingElectron_; }
        const edm::Ptr<reco::GenParticle> closestPromptParticleToSubLeadingElectron() const { return closestPromptParticleToSubLeadingElectron_; }
        /*
        float getMET_Pt(string label) const{
            int index = findMETIndex(label);
            if(index < 0 )
                return -100;

            return genMET_Pt_[findMETIndex(label)];
        }
        float getMET_Eta(string label) const{
            int index = findMETIndex(label);
            if(index < 0 )
                return -100;
            
            return genMET_Eta_[findMETIndex(label)];
        }
        float getMET_Phi(string label) const{
            int index = findMETIndex(label);
            if(index < 0 )
                return -100;
            
            return genMET_Phi_[findMETIndex(label)];
        }
        float getMET_E(string label) const{
            int index = findMETIndex(label);
            if(index < 0 )
                return -100;
            
            return genMET_E_[findMETIndex(label)];
        }
        */
        //Setter methods
        void setClosestGenJetToLeadingJet( const edm::Ptr<reco::GenJet> &val ) { closestGenJetToLeadingJet_ = val; }
        void setClosestGenJetToSubLeadingJet( const edm::Ptr<reco::GenJet> &val ) { closestGenJetToSubLeadingJet_ = val; }
        void setClosestGenJetToSubSubLeadingJet( const edm::Ptr<reco::GenJet> &val ) { closestGenJetToSubSubLeadingJet_ = val; }
        void setClosestParticleToLeadingJet( const edm::Ptr<reco::GenParticle> &val ) { closestParticleToLeadingJet_ = val; }
        void setClosestParticleToSubLeadingJet( const edm::Ptr<reco::GenParticle> &val ) { closestParticleToSubLeadingJet_ = val; }
        void setClosestParticleToSubSubLeadingJet( const edm::Ptr<reco::GenParticle> &val ) { closestParticleToSubSubLeadingJet_ = val; }
        void setClosestPartonToLeadingJet( const edm::Ptr<reco::GenParticle> &val ) { closestPartonToLeadingJet_ = val; }
        void setClosestPartonToSubLeadingJet( const edm::Ptr<reco::GenParticle> &val ) { closestPartonToSubLeadingJet_ = val; }
        void setClosestPartonToSubSubLeadingJet( const edm::Ptr<reco::GenParticle> &val ) { closestPartonToSubSubLeadingJet_ = val; }
        void setClosestParticleToLeadingPhoton( const edm::Ptr<reco::GenParticle> &val ) { closestParticleToLeadingPhoton_ = val; }
        void setClosestParticleToSubLeadingPhoton( const edm::Ptr<reco::GenParticle> &val ) { closestParticleToSubLeadingPhoton_ = val; }
        void setClosestParticleToLeadingMuon( const edm::Ptr<reco::GenParticle> &val ) { closestParticleToLeadingMuon_ = val; }
        void setClosestParticleToSubLeadingMuon( const edm::Ptr<reco::GenParticle> &val ) { closestParticleToSubLeadingMuon_ = val; }
        void setClosestParticleToLeadingElectron( const edm::Ptr<reco::GenParticle> &val ) { closestParticleToLeadingElectron_ = val; }
        void setClosestParticleToSubLeadingElectron( const edm::Ptr<reco::GenParticle> &val ) { closestParticleToSubLeadingElectron_ = val; }
        void setClosestPromptParticleToLeadingMuon( const edm::Ptr<reco::GenParticle> &val ) { closestPromptParticleToLeadingMuon_ = val; }
        void setClosestPromptParticleToSubLeadingMuon( const edm::Ptr<reco::GenParticle> &val ) { closestPromptParticleToSubLeadingMuon_ = val; }
        void setClosestPromptParticleToLeadingElectron( const edm::Ptr<reco::GenParticle> &val ) { closestPromptParticleToLeadingElectron_ = val;
//std::cout<<"Getting input in this electron set function"<<std::endl;
//std::cout<<"Pt of that electron"<<closestPromptParticleToLeadingElectron_->pt()<<std::endl;
 }
        void setClosestPromptParticleToSubLeadingElectron( const edm::Ptr<reco::GenParticle> &val ) { closestPromptParticleToSubLeadingElectron_ = val; }
        void setLeadingParton( const edm::Ptr<reco::GenParticle> &val ) { leadingParton_ = val; }
        void setSubLeadingParton( const edm::Ptr<reco::GenParticle> &val ) { subLeadingParton_ = val; }
        void setSubSubLeadingParton( const edm::Ptr<reco::GenParticle> &val ) { subSubLeadingParton_ = val; }
        void setLeadingJet      ( const edm::Ptr<flashgg::Jet> &val ) { leadingJet_       = val; }
        void setSubLeadingJet   ( const edm::Ptr<flashgg::Jet> &val ) { subLeadingJet_    = val; }
        void setSubSubLeadingJet( const edm::Ptr<flashgg::Jet> &val ) { subSubLeadingJet_ = val; }
        void setLeadingGenJet( const edm::Ptr<reco::GenJet> &val ) { leadingGenJet_ = val; }
        void setSubLeadingGenJet( const edm::Ptr<reco::GenJet> &val ) { subLeadingGenJet_ = val; }
        void setSubSubLeadingGenJet( const edm::Ptr<reco::GenJet> &val ) { subSubLeadingGenJet_ = val; }
        void setPtOrderedPartons( const std::vector<edm::Ptr<reco::GenParticle>> &val ) { ptOrderedPartons_ = val; }
        void setPtOrderedGenJets( const std::vector<edm::Ptr<reco::GenJet>> &val ) { ptOrderedGenJets_ = val; }
        void setPtOrderedFggJets( const std::vector<edm::Ptr<flashgg::Jet>> &val ) { ptOrderedFggJets_ = val; }
        void setDiPhoton( const edm::Ptr<flashgg::DiPhotonCandidate> &val ) {diPhoton_ = val;}
        void setLeadingMuon      ( const edm::Ptr<flashgg::Muon> &val ) { leadingMuon_       = val; }
        void setSubLeadingMuon   ( const edm::Ptr<flashgg::Muon> &val ) { subLeadingMuon_    = val; }
        void setLeadingElectron      ( const edm::Ptr<flashgg::Electron> &val ) { leadingElectron_       = val; }
        void setSubLeadingElectron   ( const edm::Ptr<flashgg::Electron> &val ) { subLeadingElectron_    = val; }

        void testfunction(){ //PS
//        std::cout<<"test value"<<closestPromptParticleToLeadingElectron()->pt()<<std::endl;
}

//bool hasClosestPromptParticleToLeadingElectron() const {
//std::cout<<"pt of prompt electron"<<closestPromptParticleToLeadingElectron_->pt()<<std::endl;
////if(closestPromptParticleToLeadingElectron_ != NULL) std::cout<<"has closest prompt electron!!!!!"<<std::endl;
//if(closestPromptParticleToLeadingElectron_.isNonnull()){
//std::cout<<"pt of prompt electron"<<closestPromptParticleToLeadingElectron_->pt()<<std::endl;
//std::cout<<"has closest prompt electron"<<std::endl;
//}
//else std::cout<<"don't has closest prompt electron"<<std::endl;

// return closestPromptParticleToLeadingElectron_.isNonnull(); }
        /*
        void setMETValues(  string label, float metPt, float metEta, float metPhi, float metE ){
            metAssignmentLabels.push_back( label);
            genMET_Pt_.push_back(metPt) ;  genMET_Eta_.push_back(metEta) ; genMET_Phi_.push_back(metPhi) ; genMET_E_.push_back(metE);
        }
        */
        //Counts
        unsigned int numberOfPartons() const {return ptOrderedPartons_.size();} 
        unsigned int numberOfGenJets() const {return ptOrderedGenJets_.size();} 
        unsigned int numberOfFggJets() const {return ptOrderedFggJets_.size();} 
        unsigned int numberOfDistinctMatchedPartons() const { 
            if (hasClosestPartonToLeadingJet() && hasClosestPartonToSubLeadingJet() && !hasClosestPartonToSubSubLeadingJet()) {
                return ( closestPartonToLeadingJet() != closestPartonToSubLeadingJet() ? 2 : 0 );
            }else if (hasClosestPartonToLeadingJet() && hasClosestPartonToSubLeadingJet() && hasClosestPartonToSubSubLeadingJet()) {
                unsigned numDistinct(0);
                numDistinct += ( closestPartonToLeadingJet() != closestPartonToSubLeadingJet() ? 2 : 0 );
                numDistinct += ( closestPartonToLeadingJet() != closestPartonToSubSubLeadingJet() ? 2 : 0 );
                numDistinct += ( closestPartonToSubLeadingJet() != closestPartonToSubSubLeadingJet() ? 2 : 0 );
                return numDistinct/2;
            }else if (hasClosestPartonToLeadingJet()){return 1;}else{return 0;}
        }
        unsigned int numberOfMatchesAfterDRCut(float dRCut) const {
            unsigned int count(0);
            if (hasClosestPartonToLeadingJet() && hasClosestPartonToSubLeadingJet() && !hasClosestPartonToSubSubLeadingJet()) {
                count += (fabs(dR_partonMatchingToJ1()) < dRCut ? 1 : 0 );
                count += (fabs(dR_partonMatchingToJ2()) < dRCut ? 1 : 0 );
            }else if (hasClosestPartonToLeadingJet() && hasClosestPartonToSubLeadingJet() && hasClosestPartonToSubSubLeadingJet()) {
                count += (fabs(dR_partonMatchingToJ1()) < dRCut ? 1 : 0 );
                count += (fabs(dR_partonMatchingToJ2()) < dRCut ? 1 : 0 );
                count += (fabs(dR_partonMatchingToJ3()) < dRCut ? 1 : 0 );
            }else if (hasClosestPartonToLeadingJet()) {
                count += (fabs(dR_partonMatchingToJ1()) < dRCut ? 1 : 0 );
            }
            return count;
        } 
        unsigned int numberOfLeadingAndSubLeadingMatchesAfterDRCut(float dRCut) const {
            unsigned int count(0);
            if (hasClosestPartonToLeadingJet() && hasClosestPartonToSubLeadingJet() && !hasClosestPartonToSubSubLeadingJet()) {
            }else if (hasClosestPartonToLeadingJet()) {
                count += (fabs(dR_partonMatchingToJ1()) < dRCut ? 1 : 0 );
            }
            return count;
        }
        unsigned int numberOfLeadingMatchesAfterDRCut(float dRCut) const {
            unsigned int count(0);
            if (hasClosestPartonToLeadingJet()) {
                count += (fabs(dR_partonMatchingToJ1()) < dRCut ? 1 : 0 );
            }
            return count;
        }
        //Clone
        THQLeptonicTagTruth *clone() const;

    private:
        /*
        int findMETIndex(string label) const{
            ptrdiff_t pos = find(metAssignmentLabels.begin(), metAssignmentLabels.end(), label) - metAssignmentLabels.begin();
            if( pos < int( metAssignmentLabels.size() ) )
                return pos;
            else{
                return -1;
            }
        }
        */
        edm::Ptr<reco::GenJet> closestGenJetToLeadingJet_;
        edm::Ptr<reco::GenJet> closestGenJetToSubLeadingJet_;
        edm::Ptr<reco::GenJet> closestGenJetToSubSubLeadingJet_;
        edm::Ptr<reco::GenParticle> closestParticleToLeadingJet_;
        edm::Ptr<reco::GenParticle> closestParticleToSubLeadingJet_;
        edm::Ptr<reco::GenParticle> closestParticleToSubSubLeadingJet_;
        edm::Ptr<reco::GenParticle> closestPartonToLeadingJet_;
        edm::Ptr<reco::GenParticle> closestPartonToSubLeadingJet_;
        edm::Ptr<reco::GenParticle> closestPartonToSubSubLeadingJet_;
        edm::Ptr<reco::GenParticle> closestParticleToLeadingPhoton_;
        edm::Ptr<reco::GenParticle> closestParticleToSubLeadingPhoton_;
        edm::Ptr<reco::GenParticle> closestParticleToLeadingMuon_;
        edm::Ptr<reco::GenParticle> closestParticleToSubLeadingMuon_;
        edm::Ptr<reco::GenParticle> closestParticleToLeadingElectron_;
        edm::Ptr<reco::GenParticle> closestParticleToSubLeadingElectron_;
        edm::Ptr<reco::GenParticle> closestPromptParticleToLeadingMuon_;
        edm::Ptr<reco::GenParticle> closestPromptParticleToSubLeadingMuon_;
        edm::Ptr<reco::GenParticle> closestPromptParticleToLeadingElectron_;
        edm::Ptr<reco::GenParticle> closestPromptParticleToSubLeadingElectron_;

        edm::Ptr<flashgg::Jet> leadingJet_;
        edm::Ptr<flashgg::Jet> subLeadingJet_;
        edm::Ptr<flashgg::Jet> subSubLeadingJet_;

        edm::Ptr<reco::GenParticle> leadingParton_;
        edm::Ptr<reco::GenParticle> subLeadingParton_;
        edm::Ptr<reco::GenParticle> subSubLeadingParton_;

        edm::Ptr<reco::GenJet> leadingGenJet_;
        edm::Ptr<reco::GenJet> subLeadingGenJet_;
        edm::Ptr<reco::GenJet> subSubLeadingGenJet_;

        edm::Ptr<flashgg::DiPhotonCandidate> diPhoton_;

        edm::Ptr<flashgg::Muon> leadingMuon_;
        edm::Ptr<flashgg::Muon> subLeadingMuon_;
        edm::Ptr<flashgg::Electron> leadingElectron_;
        edm::Ptr<flashgg::Electron> subLeadingElectron_;
        //std::vector<float> genMET_Pt_; std::vector<float> genMET_Eta_; std::vector<float> genMET_Phi_; std::vector<float> genMET_E_;

        std::vector<edm::Ptr<reco::GenParticle>> ptOrderedPartons_;
        std::vector<edm::Ptr<reco::GenJet>> ptOrderedGenJets_;
        std::vector<edm::Ptr<flashgg::Jet>> ptOrderedFggJets_;
        
    };
}

#endif
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
