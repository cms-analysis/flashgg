#ifndef FLASHgg_VBFTagTruth_h
#define FLASHgg_VBFTagTruth_h

#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Common/interface/Ptr.h"
#include "flashgg/DataFormats/interface/TagTruthBase.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "DataFormats/Math/interface/deltaR.h"

namespace flashgg {

    class VBFTagTruth : public TagTruthBase
    {

    public:

        VBFTagTruth();
        ~VBFTagTruth();
        //        VBFTagTruth(const VBFTagTruth &b);

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
                  

    //MVA vars
        //Hemispheres
        //Flashgg jets
        int hemisphere_J1() const { if (hasLeadingJet()) { return ( leadingJet()->eta() > 0 ? 1 : -1 ); }else{ return 0;}}
        int hemisphere_J2() const { if (hasSubLeadingJet()) { return ( subLeadingJet()->eta() > 0 ? 1 : -1 ); }else{ return 0;}}
        int hemisphere_J3() const { if (hasSubSubLeadingJet()) { return ( subSubLeadingJet()->eta() > 0 ? 1 : -1 ); }else{ return 0;}}
        //GenJets
        int hemisphere_J1_GenJet() const { if (hasClosestGenJetToLeadingJet()) { return ( closestGenJetToLeadingJet()->eta() > 0 ? 1 : -1 ); }else{ return 0;}}
        int hemisphere_J2_GenJet() const { if (hasClosestGenJetToSubLeadingJet()) { return ( closestGenJetToSubLeadingJet()->eta() > 0 ? 1 : -1 ); }else{ return 0;}}
        int hemisphere_J3_GenJet() const { if (hasClosestGenJetToSubSubLeadingJet()) { return ( closestGenJetToSubSubLeadingJet()->eta() > 0 ? 1 : -1 ); }else{ return 0;}}
        //Particles
        int hemisphere_J1_GenParticle() const { if (hasClosestParticleToLeadingJet()) { return ( closestParticleToLeadingJet()->eta() > 0 ? 1 : -1 ); }else{ return 0;}}
        int hemisphere_J2_GenParticle() const { if (hasClosestParticleToSubLeadingJet()) { return ( closestParticleToSubLeadingJet()->eta() > 0 ? 1 : -1 ); }else{ return 0;}}
        int hemisphere_J3_GenParticle() const { if (hasClosestParticleToSubSubLeadingJet()) { return ( closestParticleToSubSubLeadingJet()->eta() > 0 ? 1 : -1 ); }else{ return 0;}}
        //Truth partons
        int hemisphere_P1() const { if (hasLeadingParton()) {return (leadingParton()->eta() > 0 ? 1 : -1 ); }else{ return 0;}} 
        int hemisphere_P2() const { if (hasSubLeadingParton()) {return (subLeadingParton()->eta() > 0 ? 1 : -1 ); }else{ return 0;}} 
        int hemisphere_P3() const { if (hasSubSubLeadingParton()) {return (subSubLeadingParton()->eta() > 0 ? 1 : -1 ); }else{ return 0;}} 
        //Opposite hemispheres
        int oppHemispheres_J12() const { if (hasLeadingJet() && hasSubLeadingJet()) {return hemisphere_J1()*hemisphere_J2();}else{return 0;}}
        int oppHemispheres_J13() const { if (hasLeadingJet() && hasSubSubLeadingJet()) {return hemisphere_J1()*hemisphere_J3();}else{return 0;}}
        int oppHemispheres_J23() const { if (hasSubLeadingJet() && hasSubSubLeadingJet()) {return hemisphere_J2()*hemisphere_J3();}else{return 0;}}
        int oppHemispheres_J12_GenJet() const { if (hasClosestGenJetToLeadingJet() && hasClosestGenJetToSubLeadingJet()) 
                                              {return hemisphere_J1_GenJet()*hemisphere_J2_GenJet();}else{return 0;}}
        int oppHemispheres_J13_GenJet() const { if (hasClosestGenJetToLeadingJet() && hasClosestGenJetToSubSubLeadingJet()) 
                                              {return hemisphere_J1_GenJet()*hemisphere_J3_GenJet();}else{return 0;}}
        int oppHemispheres_J23_GenJet() const { if (hasClosestGenJetToSubLeadingJet() && hasClosestGenJetToSubSubLeadingJet()) 
                                              {return hemisphere_J2_GenJet()*hemisphere_J3_GenJet();}else{return 0;}}
        int oppHemispheres_J12_GenParticle() const { if (hasClosestParticleToLeadingJet() && hasClosestParticleToSubLeadingJet()) 
                                              {return hemisphere_J1_GenParticle()*hemisphere_J2_GenParticle();}else{return 0;}}
        int oppHemispheres_J13_GenParticle() const { if (hasClosestParticleToLeadingJet() && hasClosestParticleToSubSubLeadingJet()) 
                                              {return hemisphere_J1_GenParticle()*hemisphere_J3_GenParticle();}else{return 0;}}
        int oppHemispheres_J23_GenParticle() const { if (hasClosestParticleToSubLeadingJet() && hasClosestParticleToSubSubLeadingJet()) 
                                              {return hemisphere_J2_GenParticle()*hemisphere_J3_GenParticle();}else{return 0;}}
        int oppHemispheres_P12() const { if (hasLeadingParton() && hasSubLeadingParton()) {return hemisphere_P1()*hemisphere_P2();}else{return 0;}}
        int oppHemispheres_P13() const { if (hasLeadingParton() && hasSubSubLeadingParton()) {return hemisphere_P1()*hemisphere_P3();}else{return 0;}}
        int oppHemispheres_P23() const { if (hasSubLeadingParton() && hasSubSubLeadingParton()) {return hemisphere_P2()*hemisphere_P3();}else{return 0;}}

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
       
        //Invariant Masses 
        //(mjj)
        float mjj_J1J2_FggJet() const {if (hasLeadingJet() && hasSubLeadingJet()) { return (leadingJet()->p4() + subLeadingJet()->p4()).mass(); }else{return -9999.;}}
        float mjj_J1J3_FggJet() const {if (hasLeadingJet() && hasSubSubLeadingJet()) { return (leadingJet()->p4() + subSubLeadingJet()->p4()).mass(); }else{return -9999.;}}
        float mjj_J2J3_FggJet() const {if (hasSubLeadingJet() && hasSubSubLeadingJet()) { return (subSubLeadingJet()->p4() + subSubLeadingJet()->p4()).mass(); }else{return -9999.;}}
        float mjj_J1J2_GenJet() const {if (hasClosestGenJetToLeadingJet() && hasClosestGenJetToSubLeadingJet()) {
                                            return (closestGenJetToLeadingJet()->p4() + closestGenJetToSubLeadingJet()->p4()).mass(); }else{return -9999.;}}
        float mjj_J1J3_GenJet() const {if (hasClosestGenJetToLeadingJet() && hasClosestGenJetToSubSubLeadingJet()) {
                                            return (closestGenJetToLeadingJet()->p4() + closestGenJetToSubSubLeadingJet()->p4()).mass(); }else{return -9999.;}}
        float mjj_J2J3_GenJet() const {if (hasClosestGenJetToSubLeadingJet() && hasClosestGenJetToSubSubLeadingJet()) {
                                            return (closestGenJetToSubLeadingJet()->p4() + closestGenJetToSubSubLeadingJet()->p4()).mass(); }else{return -9999.;}}
        float mjj_J1J2_GenParticle() const {if (hasClosestParticleToLeadingJet() && hasClosestParticleToSubLeadingJet()) {
                                            return (closestParticleToLeadingJet()->p4() + closestParticleToSubLeadingJet()->p4()).mass(); }else{return -9999.;}}
        float mjj_J1J3_GenParticle() const {if (hasClosestParticleToLeadingJet() && hasClosestParticleToSubSubLeadingJet()) {
                                            return (closestParticleToLeadingJet()->p4() + closestParticleToSubSubLeadingJet()->p4()).mass(); }else{return -9999.;}}
        float mjj_J2J3_GenParticle() const {if (hasClosestParticleToSubLeadingJet() && hasClosestParticleToSubSubLeadingJet()) {
                                            return (closestParticleToSubLeadingJet()->p4() + closestParticleToSubSubLeadingJet()->p4()).mass(); }else{return -9999.;}}
        float mjj_P1P2_Partons() const {if (hasLeadingParton() && hasSubLeadingParton()) {
                                            return (leadingParton()->p4() + subLeadingParton()->p4()).mass();}else{return -9999.;}}
        float mjj_P1P3_Partons() const {if (hasLeadingParton() && hasSubSubLeadingParton()) {
                                            return (leadingParton()->p4() + subSubLeadingParton()->p4()).mass();}else{return -9999.;}}
        float mjj_P2P3_Partons() const {if (hasSubLeadingParton() && hasSubSubLeadingParton()) {
                                            return (subLeadingParton()->p4() + subSubLeadingParton()->p4()).mass();}else{return -9999.;}}
        //(mjjj)
        float mjjj_FggJet() const {if (hasLeadingJet() && hasSubLeadingJet() && hasSubSubLeadingJet()) {
                                    return (leadingJet()->p4() + subLeadingJet()->p4() + subSubLeadingJet()->p4()).mass(); 
                                    }else{return -9999.;}}
        float mjjj_GenJet() const {if (hasClosestGenJetToLeadingJet() && hasClosestGenJetToSubLeadingJet() && hasClosestGenJetToSubSubLeadingJet()) {
                                            return (closestGenJetToLeadingJet()->p4() + closestGenJetToSubLeadingJet()->p4() + closestGenJetToSubSubLeadingJet()->p4()).mass(); 
                                    }else{return -9999.;}}
        float mjjj_GenParticle() const {if (hasClosestParticleToLeadingJet() && hasClosestParticleToSubLeadingJet() && hasClosestParticleToSubSubLeadingJet()) {
                                            return (closestParticleToLeadingJet()->p4() + closestParticleToSubLeadingJet()->p4() + closestParticleToSubSubLeadingJet()->p4()).mass();
                                    }else{return -9999.;}}
        float mjjj_Partons() const {if ( hasLeadingParton() && hasSubLeadingParton() && hasSubSubLeadingParton() ) {
                                            return (leadingParton()->p4() + subLeadingParton()->p4() + subSubLeadingParton()->p4()).mass();
                                    }else{return -9999.;}}

        //dEtas
        float dEta_J1J2_FggJet() const {if (hasLeadingJet() && hasSubLeadingJet()) { return fabs(leadingJet()->eta()-subLeadingJet()->eta()); }else{return -9999.;}}
        float dEta_J1J3_FggJet() const {if (hasLeadingJet() && hasSubSubLeadingJet()) { return fabs(leadingJet()->eta()-subSubLeadingJet()->eta()); }else{return -9999.;}}
        float dEta_J2J3_FggJet() const {if (hasSubLeadingJet() && hasSubSubLeadingJet()) { return fabs(subLeadingJet()->eta()-subSubLeadingJet()->eta()); }else{return -9999.;}}
        float dEta_J1J2_GenJet() const {if (hasClosestGenJetToLeadingJet() && hasClosestGenJetToSubLeadingJet()) {
                                            return fabs(closestGenJetToLeadingJet()->eta() - closestGenJetToSubLeadingJet()->eta()); }else{return -9999.;}}
        float dEta_J1J3_GenJet() const {if (hasClosestGenJetToLeadingJet() && hasClosestGenJetToSubSubLeadingJet()) {
                                            return fabs(closestGenJetToLeadingJet()->eta() - closestGenJetToSubSubLeadingJet()->eta()); }else{return -9999.;}}
        float dEta_J2J3_GenJet() const {if (hasClosestGenJetToSubLeadingJet() && hasClosestGenJetToSubSubLeadingJet()) {
                                            return fabs(closestGenJetToSubLeadingJet()->eta() - closestGenJetToSubSubLeadingJet()->eta()); }else{return -9999.;}}
        float dEta_J1J2_GenParticle() const {if (hasClosestParticleToLeadingJet() && hasClosestParticleToSubLeadingJet()) {
                                            return fabs(closestParticleToLeadingJet()->eta() - closestParticleToSubLeadingJet()->eta()); }else{return -9999.;}}
        float dEta_J1J3_GenParticle() const {if (hasClosestParticleToLeadingJet() && hasClosestParticleToSubSubLeadingJet()) {
                                            return fabs(closestParticleToLeadingJet()->eta() - closestParticleToSubSubLeadingJet()->eta()); }else{return -9999.;}}
        float dEta_J2J3_GenParticle() const {if (hasClosestParticleToSubLeadingJet() && hasClosestParticleToSubSubLeadingJet()) {
                                            return fabs(closestParticleToSubLeadingJet()->eta() - closestParticleToSubSubLeadingJet()->eta()); }else{return -9999.;}}
        float dEta_P1P2_Partons() const {if (hasLeadingParton() && hasSubLeadingParton()) {
                                            return fabs(leadingParton()->eta()-subLeadingParton()->eta()); }else{return -9999.;}}
        float dEta_P1P3_Partons() const {if (hasLeadingParton() && hasSubSubLeadingParton()) {
                                            return fabs(leadingParton()->eta() - subSubLeadingParton()->eta()); }else{return -9999.;}}
        float dEta_P2P3_Partons() const {if (hasSubLeadingParton() && hasSubSubLeadingParton()) {
                                            return fabs(subLeadingParton()->eta() - subSubLeadingParton()->eta()); }else{return -9999.;}}
        //Zeppenfelds
        //(Zepjj)
        float zepjj_J1J2_FggJet() const {if (hasLeadingJet() && hasSubLeadingJet()) {
                                            return fabs(diPhoton()->eta() - 0.5*(leadingJet()->eta() + subLeadingJet()->eta())); }else{return -9999.;}}
        float zepjj_J1J3_FggJet() const {if (hasLeadingJet() && hasSubSubLeadingJet()) {
                                            return fabs(diPhoton()->eta() - 0.5*(leadingJet()->eta() + subSubLeadingJet()->eta())); }else{return -9999.;}}
        float zepjj_J2J3_FggJet() const {if (hasSubLeadingJet() && hasSubSubLeadingJet()) {
                                            return fabs(diPhoton()->eta() - 0.5*(subLeadingJet()->eta() + subSubLeadingJet()->eta())); }else{return -9999.;}}
        float zepjj_J1J2_GenJet() const {if (hasClosestGenJetToLeadingJet() && hasClosestGenJetToSubLeadingJet()) {
                                            return fabs(diPhoton()->eta() - 0.5*(closestGenJetToLeadingJet()->eta() + closestGenJetToSubLeadingJet()->eta())); 
                                         }else{return -9999.;}}
        float zepjj_J1J3_GenJet() const {if (hasClosestGenJetToLeadingJet() && hasClosestGenJetToSubSubLeadingJet()) {
                                            return fabs(diPhoton()->eta() - 0.5*(closestGenJetToLeadingJet()->eta() + closestGenJetToSubSubLeadingJet()->eta())); 
                                         }else{return -9999.;}}
        float zepjj_J2J3_GenJet() const {if (hasClosestGenJetToSubLeadingJet() && hasClosestGenJetToSubSubLeadingJet()) {
                                            return fabs(diPhoton()->eta() - 0.5*(closestGenJetToSubLeadingJet()->eta() + closestGenJetToSubSubLeadingJet()->eta()));
                                         }else{return -9999.;}}
        float zepjj_J1J2_GenParticle() const {if (hasClosestParticleToLeadingJet() && hasClosestParticleToSubLeadingJet()) {
                                            return fabs(diPhoton()->eta() - 0.5*(closestParticleToLeadingJet()->eta() + closestParticleToSubLeadingJet()->eta()));
                                           }else{return -9999.;}}
        float zepjj_J1J3_GenParticle() const {if (hasClosestParticleToLeadingJet() && hasClosestParticleToSubSubLeadingJet()) {
                                            return fabs(diPhoton()->eta() - 0.5*(closestParticleToLeadingJet()->eta() + closestParticleToSubSubLeadingJet()->eta())); 
                                           }else{return -9999.;}}
        float zepjj_J2J3_GenParticle() const {if (hasClosestParticleToSubLeadingJet() && hasClosestParticleToSubSubLeadingJet()) {
                                            return fabs(diPhoton()->eta() - 0.5*(closestParticleToSubLeadingJet()->eta() + closestParticleToSubSubLeadingJet()->eta())); 
                                           }else{return -9999.;}}
        float zepjj_P1P2_Partons() const {if (hasLeadingParton() && hasSubLeadingParton()) {
                                            return fabs(diPhoton()->eta() - 0.5*(leadingParton()->eta() + subLeadingParton()->eta())); }else{return -9999.;}}
        float zepjj_P1P3_Partons() const {if (hasLeadingParton() && hasSubSubLeadingParton()) {
                                            return fabs(diPhoton()->eta() - 0.5*(leadingParton()->eta() + subSubLeadingParton()->eta())); }else{return -9999.;}}
        float zepjj_P2P3_Partons() const {if (hasSubLeadingParton() && hasSubSubLeadingParton()) {
                                            return fabs(diPhoton()->eta() - 0.5*(subLeadingParton()->eta() + subSubLeadingParton()->eta())); }else{return -9999.;}}
        //(Zepjjj)
        float zepjjj_FggJet() const {if (hasLeadingJet() && hasSubLeadingJet() && hasSubSubLeadingJet()) {
                                            return fabs(diPhoton()->eta() - (leadingJet()->eta() + subLeadingJet()->eta() + subSubLeadingJet()->eta())/3 ); 
                                    }else{return -9999.;}}
        float zepjjj_GenJet() const {if (hasClosestGenJetToLeadingJet() && hasClosestGenJetToSubLeadingJet() && hasClosestGenJetToSubSubLeadingJet()) {
                                            return fabs(diPhoton()->eta() - (closestGenJetToLeadingJet()->eta() + closestGenJetToSubLeadingJet()->eta() 
                                                                                + closestGenJetToSubSubLeadingJet()->eta())/3); }else{return -9999.;}}
        float zepjjj_GenParticle() const {if (hasClosestParticleToLeadingJet() && hasClosestParticleToSubLeadingJet() && hasClosestParticleToSubSubLeadingJet()) {
                                            return fabs(diPhoton()->eta() - (closestParticleToLeadingJet()->eta() + closestParticleToSubLeadingJet()->eta() 
                                                                                + closestParticleToSubSubLeadingJet()->eta())/3); }else{return -9999.;}}
        float zepjjj_Partons() const {if (hasLeadingParton() && hasSubLeadingParton() && hasSubSubLeadingParton()) {
                                            return fabs(diPhoton()->eta() - (leadingParton()->eta() + subLeadingParton()->eta() + subSubLeadingParton()->eta())); 
                                      }else{return -9999.;}}
        //dPhi
        //(jj)
        float dPhijj_J1J2_FggJet() const {if (hasLeadingJet() && hasSubLeadingJet()) {
                                            return fabs(deltaPhi(diPhoton()->phi(),(leadingJet()->p4()+subLeadingJet()->p4()).phi())); }else{return -9999.;}}       
        float dPhijj_J1J3_FggJet() const {if (hasLeadingJet() && hasSubSubLeadingJet()) {
                                            return fabs(deltaPhi(diPhoton()->phi(),(leadingJet()->p4()+subSubLeadingJet()->p4()).phi())); }else{return -9999.;}}       
        float dPhijj_J2J3_FggJet() const {if (hasSubLeadingJet() && hasSubSubLeadingJet()) {
                                            return fabs(deltaPhi(diPhoton()->phi(),(subLeadingJet()->p4()+subSubLeadingJet()->p4()).phi())); }else{return -9999.;}}       
        float dPhijj_J1J2_GenJet() const {if (hasClosestGenJetToLeadingJet() && hasClosestGenJetToSubLeadingJet()) {
                                            return fabs(deltaPhi(diPhoton()->phi(),(closestGenJetToLeadingJet()->p4() + closestGenJetToSubLeadingJet()->p4()).phi())); 
                                          }else{return -9999.;}}
        float dPhijj_J1J3_GenJet() const {if (hasClosestGenJetToLeadingJet() && hasClosestGenJetToSubSubLeadingJet()) {
                                            return fabs(deltaPhi(diPhoton()->phi(),(closestGenJetToLeadingJet()->p4() + closestGenJetToSubSubLeadingJet()->p4()).phi()));
                                          }else{return -9999.;}}
        float dPhijj_J2J3_GenJet() const {if (hasClosestGenJetToSubLeadingJet() && hasClosestGenJetToSubSubLeadingJet()) {
                                            return fabs(deltaPhi(diPhoton()->phi(),(closestGenJetToSubLeadingJet()->p4() + closestGenJetToSubSubLeadingJet()->p4()).phi()));
                                         }else{return -9999.;}}
        float dPhijj_J1J2_GenParticle() const {if (hasClosestParticleToLeadingJet() && hasClosestParticleToSubLeadingJet()) {
                                            return fabs(deltaPhi(diPhoton()->phi(),(closestParticleToLeadingJet()->p4() + closestParticleToSubLeadingJet()->p4()).phi()));
                                         }else{return -9999.;}}
        float dPhijj_J1J3_GenParticle() const {if (hasClosestParticleToLeadingJet() && hasClosestParticleToSubSubLeadingJet()) {
                                            return fabs(deltaPhi(diPhoton()->phi(),(closestParticleToLeadingJet()->p4() + closestParticleToSubSubLeadingJet()->p4()).phi()));
                                         }else{return -9999.;}}
        float dPhijj_J2J3_GenParticle() const {if (hasClosestParticleToSubLeadingJet() && hasClosestParticleToSubSubLeadingJet()) {
                                            return fabs(deltaPhi(diPhoton()->phi(),(closestParticleToSubLeadingJet()->p4() + closestParticleToSubSubLeadingJet()->p4()).phi()));
                                         }else{return -9999.;}}
        float dPhijj_P1P2_Partons()  const {if (hasLeadingParton() && hasSubLeadingParton()) {
                                            return fabs(deltaPhi(diPhoton()->phi(),(leadingParton()->p4() + subLeadingParton()->p4()).phi())); }else{return -9999.;}}
        float dPhijj_P1P3_Partons()  const {if (hasLeadingParton() && hasSubSubLeadingParton()) {
                                            return fabs(deltaPhi(diPhoton()->phi(),(leadingParton()->p4() + subSubLeadingParton()->p4()).phi())); }else{return -9999.;}}
        float dPhijj_P2P3_Partons()  const {if (hasSubLeadingParton() && hasSubSubLeadingParton()) {
                                            return fabs(deltaPhi(diPhoton()->phi(),(subLeadingParton()->p4() + subSubLeadingParton()->p4()).phi())); }else{return -9999.;}}
        //(jjj)
        float dPhijjj_FggJet() const {if (hasLeadingJet() && hasSubLeadingJet() && hasSubSubLeadingJet()) {
                                            return fabs(deltaPhi(diPhoton()->phi(),(leadingJet()->p4()+subLeadingJet()->p4()+subSubLeadingJet()->p4()).phi()));
                                      }else{return -9999.;}}       
        float dPhijjj_GenJet() const {if (hasClosestGenJetToLeadingJet() && hasClosestGenJetToSubLeadingJet() && hasClosestGenJetToSubSubLeadingJet()) {
                                            return fabs(deltaPhi(diPhoton()->phi(),(closestGenJetToLeadingJet()->p4() + closestGenJetToSubLeadingJet()->p4()
                                                                                +closestGenJetToSubSubLeadingJet()->p4()).phi())); }else{return -9999.;}}
        float dPhijjj_GenParticle() const {if (hasClosestParticleToLeadingJet() && hasClosestParticleToSubLeadingJet() && hasClosestParticleToSubSubLeadingJet()) {
                                            return fabs(deltaPhi(diPhoton()->phi(),(closestParticleToLeadingJet()->p4() + closestParticleToSubLeadingJet()->p4()
                                                                                +closestParticleToSubSubLeadingJet()->p4()).phi())); }else{return -9999.;}}
        float dPhijjj_Partons() const {if (hasLeadingParton() && hasSubLeadingParton() && hasSubSubLeadingParton()) {
                                            return fabs(deltaPhi(diPhoton()->phi(),(leadingParton()->p4()+subLeadingParton()->p4()+subSubLeadingParton()->p4()).phi()));
                                       }else{return -9999.;}}

        //3Jet eta minus diff in eta
        float dEta_J1J2J3_FggJet() const {if (hasLeadingJet() && hasSubLeadingJet() && hasSubSubLeadingJet()) {return fabs(eta_J1()-fabs(eta_J2()-eta_J3()));}else{return -9999.;}}
        float dEta_J2J3J1_FggJet() const {if (hasLeadingJet() && hasSubLeadingJet() && hasSubSubLeadingJet()) {return fabs(eta_J2()-fabs(eta_J3()-eta_J1()));}else{return -9999.;}}
        float dEta_J3J1J2_FggJet() const {if (hasLeadingJet() && hasSubLeadingJet() && hasSubSubLeadingJet()) {return fabs(eta_J3()-fabs(eta_J1()-eta_J2()));}else{return -9999.;}}
        float dEta_J1J2J3_GenJet() const {if (hasClosestGenJetToLeadingJet() && hasClosestGenJetToSubLeadingJet() && hasClosestGenJetToSubSubLeadingJet()) 
                                          {return fabs(eta_genJetMatchingToJ1() - fabs (eta_genJetMatchingToJ2()-eta_genJetMatchingToJ3()));}else{return 999.;}}
        float dEta_J2J3J1_GenJet() const {if (hasClosestGenJetToLeadingJet() && hasClosestGenJetToSubLeadingJet() && hasClosestGenJetToSubSubLeadingJet()) 
                                          {return fabs(eta_genJetMatchingToJ2() - fabs (eta_genJetMatchingToJ3()-eta_genJetMatchingToJ1()));}else{return 999.;}}
        float dEta_J3J1J2_GenJet() const {if (hasClosestGenJetToLeadingJet() && hasClosestGenJetToSubLeadingJet() && hasClosestGenJetToSubSubLeadingJet()) 
                                          {return fabs(eta_genJetMatchingToJ3() - fabs (eta_genJetMatchingToJ1()-eta_genJetMatchingToJ2()));}else{return 999.;}}
        float dEta_J1J2J3_GenParticle() const {if (hasClosestParticleToLeadingJet() && hasClosestParticleToSubLeadingJet() && hasClosestParticleToSubSubLeadingJet()) 
                                          {return fabs(eta_genJetMatchingToJ1() - fabs (eta_genJetMatchingToJ2()-eta_genJetMatchingToJ3()));}else{return 999.;}}
        float dEta_J2J3J1_GenParticle() const {if (hasClosestParticleToLeadingJet() && hasClosestParticleToSubLeadingJet() && hasClosestParticleToSubSubLeadingJet()) 
                                          {return fabs(eta_genJetMatchingToJ2() - fabs (eta_genJetMatchingToJ3()-eta_genJetMatchingToJ1()));}else{return 999.;}}
        float dEta_J3J1J2_GenParticle() const {if (hasClosestParticleToLeadingJet() && hasClosestParticleToSubLeadingJet() && hasClosestParticleToSubSubLeadingJet()) 
                                          {return fabs(eta_genJetMatchingToJ3() - fabs (eta_genJetMatchingToJ1()-eta_genJetMatchingToJ2()));}else{return 999.;}}
        float dEta_P1P2P3_Partons() const {if (hasLeadingParton() && hasSubLeadingParton() && hasSubSubLeadingParton()) 
                                          {return fabs(eta_P1()-fabs(eta_P2()-eta_P3()));}else{return -9999.;}}
        float dEta_P2P3P1_Partons() const {if (hasLeadingParton() && hasSubLeadingParton() && hasSubSubLeadingParton()) 
                                          {return fabs(eta_P2()-fabs(eta_P3()-eta_P1()));}else{return -9999.;}}
        float dEta_P3P1P2_Partons() const {if (hasLeadingParton() && hasSubLeadingParton() && hasSubSubLeadingParton()) 
                                          {return fabs(eta_P3()-fabs(eta_P1()-eta_P2()));}else{return -9999.;}}

        //3Jet eta of jet  minus mean of other two jets
        float dEta_dJ1_J2J3_FggJet() const {if (hasLeadingJet() && hasSubLeadingJet() && hasSubSubLeadingJet()) {return fabs(eta_J1() - 0.5*(eta_J2()+eta_J3()));}else{return -9999.;}}
        float dEta_dJ2_J3J1_FggJet() const {if (hasLeadingJet() && hasSubLeadingJet() && hasSubSubLeadingJet()) {return fabs(eta_J2() - 0.5*(eta_J3()+eta_J1()));}else{return -9999.;}}
        float dEta_dJ3_J1J2_FggJet() const {if (hasLeadingJet() && hasSubLeadingJet() && hasSubSubLeadingJet()) {return fabs(eta_J3() - 0.5*(eta_J1()+eta_J2()));}else{return -9999.;}}
        float dEta_dJ1_J2J3_GenJet() const {if (hasClosestGenJetToLeadingJet() && hasClosestGenJetToSubLeadingJet() && hasClosestGenJetToSubSubLeadingJet()) 
                                           {return fabs(eta_genJetMatchingToJ1() - 0.5*(eta_genJetMatchingToJ2()+eta_genJetMatchingToJ3()));}else{return -9999.;}}
        float dEta_dJ2_J3J1_GenJet() const {if (hasClosestGenJetToLeadingJet() && hasClosestGenJetToSubLeadingJet() && hasClosestGenJetToSubSubLeadingJet()) 
                                           {return fabs(eta_genJetMatchingToJ2() - 0.5*(eta_genJetMatchingToJ3()+eta_genJetMatchingToJ1()));}else{return -9999.;}}
        float dEta_dJ3_J1J2_GenJet() const {if (hasClosestGenJetToLeadingJet() && hasClosestGenJetToSubLeadingJet() && hasClosestGenJetToSubSubLeadingJet()) 
                                           {return fabs(eta_genJetMatchingToJ3() - 0.5*(eta_genJetMatchingToJ1()+eta_genJetMatchingToJ2()));}else{return -9999.;}}
        float dEta_dJ1_J2J3_GenParticle() const {if (hasClosestParticleToLeadingJet() && hasClosestParticleToSubLeadingJet() && hasClosestParticleToSubSubLeadingJet()) 
                                           {return fabs(eta_genJetMatchingToJ1() - 0.5*(eta_genJetMatchingToJ2()+eta_genJetMatchingToJ3()));}else{return -9999.;}}
        float dEta_dJ2_J3J1_GenParticle() const {if (hasClosestParticleToLeadingJet() && hasClosestParticleToSubLeadingJet() && hasClosestParticleToSubSubLeadingJet()) 
                                           {return fabs(eta_genJetMatchingToJ2() - 0.5*(eta_genJetMatchingToJ3()+eta_genJetMatchingToJ1()));}else{return -9999.;}}
        float dEta_dJ3_J1J2_GenParticle() const {if (hasClosestParticleToLeadingJet() && hasClosestParticleToSubLeadingJet() && hasClosestParticleToSubSubLeadingJet()) 
                                           {return fabs(eta_genJetMatchingToJ3() - 0.5*(eta_genJetMatchingToJ1()+eta_genJetMatchingToJ2()));}else{return -9999.;}}
        float dEta_dJ1_J2J3_Parton() const {if (hasLeadingParton() && hasSubLeadingParton() && hasSubSubLeadingParton()) 
                                           {return fabs(eta_J1() - 0.5*(eta_J2()+eta_J3()));}else{return -9999.;}}
        float dEta_dJ2_J3J1_Parton() const {if (hasLeadingParton() && hasSubLeadingParton() && hasSubSubLeadingParton())    
                                           {return fabs(eta_J2() - 0.5*(eta_J3()+eta_J1()));}else{return -9999.;}}
        float dEta_dJ3_J1J2_Parton() const {if (hasLeadingParton() && hasSubLeadingParton() && hasSubSubLeadingParton()) 
                                           {return fabs(eta_J3() - 0.5*(eta_J1()+eta_J2()));}else{return -9999.;}}

        //3Jet mjj differences
        float mjj_d12_13plus23_FggJet() const {if (hasLeadingJet() && hasSubLeadingJet() && hasSubSubLeadingJet()) {
                                              return (mjj_J1J2_FggJet() - mjj_J1J3_FggJet() - mjj_J2J3_FggJet());}else{return -9999.;}}
        float mjj_d12_13_FggJet() const {if (hasLeadingJet() && hasSubLeadingJet() && hasSubSubLeadingJet()) {
                                              return (mjj_J1J2_FggJet() - mjj_J1J3_FggJet());}else{return -9999.;}}
        float mjj_d12_23_FggJet() const {if (hasLeadingJet() && hasSubLeadingJet() && hasSubSubLeadingJet()) {
                                              return (mjj_J1J2_FggJet() - mjj_J2J3_FggJet());}else{return -9999.;}}
        float mjj_d13_23_FggJet() const {if (hasLeadingJet() && hasSubLeadingJet() && hasSubSubLeadingJet()) {
                                              return (mjj_J1J3_FggJet() - mjj_J2J3_FggJet());}else{return -9999.;}}
        float mjj_d12_13plus23_GenJet() const {if (hasClosestGenJetToLeadingJet() && hasClosestGenJetToSubLeadingJet() && hasClosestGenJetToSubSubLeadingJet()) {
                                              return (mjj_J1J2_GenJet() - mjj_J1J3_GenJet() - mjj_J2J3_GenJet());}else{return -9999.;}}
        float mjj_d12_13_GenJet() const {if (hasClosestGenJetToLeadingJet() && hasClosestGenJetToSubLeadingJet() && hasClosestGenJetToSubSubLeadingJet()) {
                                              return (mjj_J1J2_GenJet() - mjj_J1J3_GenJet());}else{return -9999.;}}
        float mjj_d12_23_GenJet() const {if (hasClosestGenJetToLeadingJet() && hasClosestGenJetToSubLeadingJet() && hasClosestGenJetToSubSubLeadingJet()) {
                                              return (mjj_J1J2_GenJet() - mjj_J2J3_GenJet());}else{return -9999.;}}
        float mjj_d13_23_GenJet() const {if (hasClosestGenJetToLeadingJet() && hasClosestGenJetToSubLeadingJet() && hasClosestGenJetToSubSubLeadingJet()) {
                                              return (mjj_J1J3_GenJet() - mjj_J2J3_GenJet());}else{return -9999.;}}
        float mjj_d12_13plus23_GenParticle() const {if (hasClosestParticleToLeadingJet() && hasClosestParticleToSubLeadingJet() && hasClosestParticleToSubSubLeadingJet()) {
                                              return (mjj_J1J2_GenParticle() - mjj_J1J3_GenParticle() - mjj_J2J3_GenParticle());}else{return -9999.;}}
        float mjj_d12_13_GenParticle() const {if (hasClosestParticleToLeadingJet() && hasClosestParticleToSubLeadingJet() && hasClosestParticleToSubSubLeadingJet()) {
                                              return (mjj_J1J2_GenParticle() - mjj_J1J3_GenParticle());}else{return -9999.;}}
        float mjj_d12_23_GenParticle() const {if (hasClosestParticleToLeadingJet() && hasClosestParticleToSubLeadingJet() && hasClosestParticleToSubSubLeadingJet()) {
                                              return (mjj_J1J2_GenParticle() - mjj_J2J3_GenParticle());}else{return -9999.;}}
        float mjj_d13_23_GenParticle() const {if (hasClosestParticleToLeadingJet() && hasClosestParticleToSubLeadingJet() && hasClosestParticleToSubSubLeadingJet()) {
                                              return (mjj_J1J3_GenParticle() - mjj_J2J3_GenParticle());}else{return -9999.;}}
        float mjj_d12_13plus23_Partons() const {if (hasLeadingParton() && hasSubLeadingParton() && hasSubSubLeadingParton()) {
                                              return (mjj_P1P2_Partons() - mjj_P1P3_Partons() - mjj_P2P3_Partons());}else{return -9999.;}}
        float mjj_d12_13_Partons() const {if (hasLeadingParton() && hasSubLeadingParton() && hasSubSubLeadingParton()) {
                                              return (mjj_P1P2_Partons() - mjj_P1P3_Partons());}else{return -9999.;}}
        float mjj_d12_23_Partons() const {if (hasLeadingParton() && hasSubLeadingParton() && hasSubSubLeadingParton()) {
                                              return (mjj_P1P2_Partons() - mjj_P2P3_Partons());}else{return -9999.;}}
        float mjj_d13_23_Partons() const {if (hasLeadingParton() && hasSubLeadingParton() && hasSubSubLeadingParton()) {
                                              return (mjj_P1P3_Partons() - mjj_P2P3_Partons());}else{return -9999.;}}

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
//ADD TO STRUCTS AND SCRIPT 
        //Pt variable methods
        float missingP4_dPhi_jjj_FggJet() const {if (hasLeadingJet() && hasSubLeadingJet() && hasSubSubLeadingJet()) {
                                                 return fabs(deltaPhi((leadingJet()->p4() + subLeadingJet()->p4() + subSubLeadingJet()->p4() + diPhoton()->p4()).phi(), 
                                                                 diPhoton()->phi()));
                                                }else{return -9999.;}}               
        float missingP4_dPhi_jj_FggJet() const {if (hasLeadingJet() && hasSubLeadingJet()) {
                                                return fabs(deltaPhi((leadingJet()->p4() + subLeadingJet()->p4() + diPhoton()->p4()).phi(), 
                                                                 diPhoton()->phi()));
                                                }else{return -9999.;}}               
        float missingP4_Pt_jjj_FggJet() const {if (hasLeadingJet() && hasSubLeadingJet() && hasSubSubLeadingJet()) {
                                               return (leadingJet()->p4() + subLeadingJet()->p4() + subSubLeadingJet()->p4() + diPhoton()->p4()).Pt();
                                               }else{return -9999.;}}
        float missingP4_Pt_jj_FggJet() const {if (hasLeadingJet() && hasSubLeadingJet()) {
                                              return (leadingJet()->p4() + subLeadingJet()->p4() + diPhoton()->p4()).Pt();
                                              }else{return -9999.;}}
        float missingP4_dPhi_jjj_GenJet() const {if (hasClosestGenJetToLeadingJet() && hasClosestGenJetToSubLeadingJet() && hasClosestGenJetToSubSubLeadingJet()) {
                                                 return fabs(deltaPhi((closestGenJetToLeadingJet()->p4() + closestGenJetToSubLeadingJet()->p4()
                                                                 + closestGenJetToSubSubLeadingJet()->p4() + diPhoton()->p4()).phi(), 
                                                                 diPhoton()->phi()));
                                                }else{return -9999.;}}               
        float missingP4_dPhi_jj_GenJet() const {if (hasClosestGenJetToLeadingJet() && hasClosestGenJetToSubLeadingJet()) {
                                                return fabs(deltaPhi((closestGenJetToLeadingJet()->p4() + closestGenJetToSubLeadingJet()->p4() + diPhoton()->p4()).phi(), 
                                                                 diPhoton()->phi()));
                                                }else{return -9999.;}}               
        float missingP4_Pt_jjj_GenJet() const {if (hasClosestGenJetToLeadingJet() && hasClosestGenJetToSubLeadingJet() && hasClosestGenJetToSubSubLeadingJet()) {
                                               return (closestGenJetToLeadingJet()->p4() + closestGenJetToSubLeadingJet()->p4() 
                                                       + closestGenJetToSubSubLeadingJet()->p4() + diPhoton()->p4()).Pt();
                                               }else{return -9999.;}}
        float missingP4_Pt_jj_GenJet() const {if (hasClosestGenJetToLeadingJet() && hasClosestGenJetToSubLeadingJet()) {
                                              return (closestGenJetToLeadingJet()->p4() + closestGenJetToSubLeadingJet()->p4() + diPhoton()->p4()).Pt();
                                              }else{return -9999.;}}
        float missingP4_dPhi_jjj_GenParticle() const {if (hasClosestParticleToLeadingJet() && hasClosestParticleToSubLeadingJet() && hasClosestParticleToSubSubLeadingJet()) {
                                                 return fabs(deltaPhi((closestParticleToLeadingJet()->p4() + closestParticleToSubLeadingJet()->p4()
                                                                 + closestParticleToSubSubLeadingJet()->p4() + diPhoton()->p4()).phi(), 
                                                                 diPhoton()->phi()));
                                                }else{return -9999.;}}               
        float missingP4_dPhi_jj_GenParticle() const {if (hasClosestParticleToLeadingJet() && hasClosestParticleToSubLeadingJet()) {
                                                return fabs(deltaPhi((closestParticleToLeadingJet()->p4() + closestParticleToSubLeadingJet()->p4() + diPhoton()->p4()).phi(), 
                                                                 diPhoton()->phi()));
                                                }else{return -9999.;}}               
        float missingP4_Pt_jjj_GenParticle() const {if (hasClosestParticleToLeadingJet() && hasClosestParticleToSubLeadingJet() && hasClosestParticleToSubSubLeadingJet()) {
                                               return (closestParticleToLeadingJet()->p4() + closestParticleToSubLeadingJet()->p4() 
                                                       + closestParticleToSubSubLeadingJet()->p4() + diPhoton()->p4()).Pt();
                                               }else{return -9999.;}}
        float missingP4_Pt_jj_GenParticle() const {if (hasClosestParticleToLeadingJet() && hasClosestParticleToSubLeadingJet()) {
                                              return (closestParticleToLeadingJet()->p4() + closestParticleToSubLeadingJet()->p4() + diPhoton()->p4()).Pt();
                                              }else{return -9999.;}}
        float missingP4_dPhi_jjj_Partons() const {if (hasLeadingParton() && hasSubLeadingParton() && hasSubSubLeadingParton()) {
                                                 return fabs(deltaPhi((leadingParton()->p4() + subLeadingParton()->p4() + subSubLeadingParton()->p4() + diPhoton()->p4()).phi(), 
                                                                 diPhoton()->phi()));
                                                }else{return -9999.;}}               
        float missingP4_dPhi_jj_Partons() const {if (hasLeadingParton() && hasSubLeadingParton()) {
                                                return fabs(deltaPhi((leadingParton()->p4() + subLeadingParton()->p4() + diPhoton()->p4()).phi(), 
                                                                 diPhoton()->phi()));
                                                }else{return -9999.;}}               
        float missingP4_Pt_jjj_Partons() const {if (hasLeadingParton() && hasSubLeadingParton() && hasSubSubLeadingParton()) {
                                               return (leadingParton()->p4() + subLeadingParton()->p4() + subSubLeadingParton()->p4() + diPhoton()->p4()).Pt();
                                               }else{return -9999.;}}
        float missingP4_Pt_jj_Partons() const {if (hasLeadingParton() && hasSubLeadingParton()) {
                                              return (leadingParton()->p4() + subLeadingParton()->p4() + diPhoton()->p4()).Pt();
                                              }else{return -9999.;}}

        //pt variables, difference between jj and jjj
        float missingP4_dPhi_d3J2J_FggJet() const {if (hasLeadingJet() && hasSubLeadingJet() && hasSubSubLeadingJet()) {
                                                      return missingP4_dPhi_jjj_FggJet() - missingP4_dPhi_jj_FggJet();
                                                 }else{return -9999.;}}      
        float missingP4_Pt_d3J2J_FggJet() const {if (hasLeadingJet() && hasSubLeadingJet() && hasSubSubLeadingJet()) {
                                                      return missingP4_Pt_jjj_FggJet() - missingP4_Pt_jj_FggJet();
                                                 }else{return -9999.;}}      
        float missingP4_dPhi_d3J2J_GenJet() const {if (hasClosestGenJetToLeadingJet() && hasClosestGenJetToSubLeadingJet() && hasClosestGenJetToSubSubLeadingJet()) {
                                                      return missingP4_dPhi_jjj_GenJet() - missingP4_dPhi_jj_GenJet();
                                                 }else{return -9999.;}}      
        float missingP4_Pt_d3J2J_GenJet() const {if (hasClosestGenJetToLeadingJet() && hasClosestGenJetToSubLeadingJet() && hasClosestGenJetToSubSubLeadingJet()) {
                                                      return missingP4_Pt_jjj_GenJet() - missingP4_Pt_jj_GenJet();
                                                 }else{return -9999.;}}      
        float missingP4_dPhi_d3J2J_GenParticle() const {if (hasClosestParticleToLeadingJet() && hasClosestParticleToSubLeadingJet() && hasClosestParticleToSubSubLeadingJet()) {
                                                      return missingP4_dPhi_jjj_GenParticle() - missingP4_dPhi_jj_GenParticle();
                                                 }else{return -9999.;}}      
        float missingP4_Pt_d3J2J_GenParticle() const {if (hasClosestParticleToLeadingJet() && hasClosestParticleToSubLeadingJet() && hasClosestParticleToSubSubLeadingJet()) {
                                                      return missingP4_Pt_jjj_GenParticle() - missingP4_Pt_jj_GenParticle();
                                                 }else{return -9999.;}}      
        float missingP4_dPhi_d3J2J_Partons() const {if (hasLeadingParton() && hasSubLeadingParton() && hasSubSubLeadingParton()) {
                                                      return missingP4_dPhi_jjj_Partons() - missingP4_dPhi_jj_Partons();
                                                 }else{return -9999.;}}      
        float missingP4_Pt_d3J2J_Partons() const {if (hasLeadingParton() && hasSubLeadingParton() && hasSubSubLeadingParton()) {
                                                      return missingP4_Pt_jjj_Partons() - missingP4_Pt_jj_Partons();
                                                 }else{return -9999.;}}      

        //More delta phis 
        float dPhi_12_FggJet() const {if (hasLeadingJet() && hasSubLeadingJet()) {
                                            return fabs(deltaPhi(leadingJet()->phi(),subLeadingJet()->phi()));
                                      }else{return -9999.;}}
        float dPhi_13_FggJet() const {if (hasLeadingJet() && hasSubSubLeadingJet()) {
                                            return fabs(deltaPhi(leadingJet()->phi(),subSubLeadingJet()->phi()));
                                      }else{return -9999.;}}
        float dPhi_23_FggJet() const {if (hasSubLeadingJet() && hasSubSubLeadingJet()) {
                                            return fabs(deltaPhi(subLeadingJet()->phi(),subSubLeadingJet()->phi()));
                                      }else{return -9999.;}}
        float dPhi_max_FggJet() const {if (hasLeadingJet() && hasSubLeadingJet() && hasSubSubLeadingJet()) {
                                            if (dPhi_12_FggJet() > dPhi_13_FggJet() && dPhi_12_FggJet() > dPhi_23_FggJet()) {return dPhi_12_FggJet();}
                                            else if (dPhi_13_FggJet() > dPhi_12_FggJet() && dPhi_13_FggJet() > dPhi_23_FggJet()) {return dPhi_13_FggJet();}
                                            else {return dPhi_23_FggJet();}}
                                        else {return -9999.;}}
        float dPhi_min_FggJet() const {if (hasLeadingJet() && hasSubLeadingJet() && hasSubSubLeadingJet()) {
                                            if (dPhi_12_FggJet() < dPhi_13_FggJet() && dPhi_12_FggJet() < dPhi_23_FggJet()) {return dPhi_12_FggJet();}
                                            else if (dPhi_13_FggJet() < dPhi_12_FggJet() && dPhi_13_FggJet() < dPhi_23_FggJet()) {return dPhi_13_FggJet();}
                                            else {return dPhi_23_FggJet();}}
                                        else {return -9999.;}}
        float dPhi_min_max_FggJet() const {if (hasLeadingJet() && hasSubLeadingJet() && hasSubSubLeadingJet()) {
                                                return dPhi_max_FggJet() - dPhi_min_FggJet();
                                           }else{return -9999.;}}

        float dPhi_12_GenJet() const {if (hasClosestGenJetToLeadingJet() && hasClosestGenJetToSubLeadingJet()) {
                                            return fabs(deltaPhi(closestGenJetToLeadingJet()->phi(),closestGenJetToSubLeadingJet()->phi()));
                                      }else{return -9999.;}}
        float dPhi_13_GenJet() const {if (hasClosestGenJetToLeadingJet() && hasClosestGenJetToSubSubLeadingJet()) {
                                            return fabs(deltaPhi(closestGenJetToLeadingJet()->phi(),closestGenJetToSubSubLeadingJet()->phi()));
                                      }else{return -9999.;}}
        float dPhi_23_GenJet() const {if (hasClosestGenJetToSubLeadingJet() && hasClosestGenJetToSubSubLeadingJet()) {
                                            return fabs(deltaPhi(closestGenJetToSubLeadingJet()->phi(),closestGenJetToSubSubLeadingJet()->phi()));
                                      }else{return -9999.;}}
        float dPhi_max_GenJet() const {if (hasClosestGenJetToLeadingJet() && hasClosestGenJetToSubLeadingJet() && hasClosestGenJetToSubSubLeadingJet()) {
                                            if (dPhi_12_GenJet() > dPhi_13_GenJet() && dPhi_12_GenJet() > dPhi_23_GenJet()) {return dPhi_12_GenJet();}
                                            else if (dPhi_13_GenJet() > dPhi_12_GenJet() && dPhi_13_GenJet() > dPhi_23_GenJet()) {return dPhi_13_GenJet();}
                                            else {return dPhi_23_GenJet();}}
                                        else {return -9999.;}}
        float dPhi_min_GenJet() const {if (hasClosestGenJetToLeadingJet() && hasClosestGenJetToSubLeadingJet() && hasClosestGenJetToSubSubLeadingJet()) {
                                            if (dPhi_12_GenJet() < dPhi_13_GenJet() && dPhi_12_GenJet() < dPhi_23_GenJet()) {return dPhi_12_GenJet();}
                                            else if (dPhi_13_GenJet() < dPhi_12_GenJet() && dPhi_13_GenJet() < dPhi_23_GenJet()) {return dPhi_13_GenJet();}
                                            else {return dPhi_23_GenJet();}}
                                        else {return -9999.;}}
        float dPhi_min_max_GenJet() const {if (hasClosestGenJetToLeadingJet() && hasClosestGenJetToSubLeadingJet() && hasClosestGenJetToSubSubLeadingJet()) {
                                                return dPhi_max_GenJet() - dPhi_min_GenJet();
                                           }else{return -9999.;}}

        float dPhi_12_GenParticle() const {if (hasClosestParticleToLeadingJet() && hasClosestParticleToSubLeadingJet()) {
                                            return fabs(deltaPhi(closestParticleToLeadingJet()->phi(),closestParticleToSubLeadingJet()->phi()));
                                      }else{return -9999.;}}
        float dPhi_13_GenParticle() const {if (hasClosestParticleToLeadingJet() && hasClosestParticleToSubSubLeadingJet()) {
                                            return fabs(deltaPhi(closestParticleToLeadingJet()->phi(),closestParticleToSubSubLeadingJet()->phi()));
                                      }else{return -9999.;}}
        float dPhi_23_GenParticle() const {if (hasClosestParticleToSubLeadingJet() && hasClosestParticleToSubSubLeadingJet()) {
                                            return fabs(deltaPhi(closestParticleToSubLeadingJet()->phi(),closestParticleToSubSubLeadingJet()->phi()));
                                      }else{return -9999.;}}
        float dPhi_max_GenParticle() const {if (hasClosestParticleToLeadingJet() && hasClosestParticleToSubLeadingJet() && hasClosestParticleToSubSubLeadingJet()) {
                                            if (dPhi_12_GenParticle() > dPhi_13_GenParticle() && dPhi_12_GenParticle() > dPhi_23_GenParticle()) {return dPhi_12_GenParticle();}
                                            else if (dPhi_13_GenParticle() > dPhi_12_GenParticle() && dPhi_13_GenParticle() > dPhi_23_GenParticle()) {return dPhi_13_GenParticle();}
                                            else {return dPhi_23_GenParticle();}}
                                        else {return -9999.;}}
        float dPhi_min_GenParticle() const {if (hasClosestParticleToLeadingJet() && hasClosestParticleToSubLeadingJet() && hasClosestParticleToSubSubLeadingJet()) {
                                            if (dPhi_12_GenParticle() < dPhi_13_GenParticle() && dPhi_12_GenParticle() < dPhi_23_GenParticle()) {return dPhi_12_GenParticle();}
                                            else if (dPhi_13_GenParticle() < dPhi_12_GenParticle() && dPhi_13_GenParticle() < dPhi_23_GenParticle()) {return dPhi_13_GenParticle();}
                                            else {return dPhi_23_GenParticle();}}
                                        else {return -9999.;}}
        float dPhi_min_max_GenParticle() const {if (hasClosestParticleToLeadingJet() && hasClosestParticleToSubLeadingJet() && hasClosestParticleToSubSubLeadingJet()) {
                                                return dPhi_max_GenParticle() - dPhi_min_GenParticle();
                                           }else{return -9999.;}}

        float dPhi_12_Partons() const {if (hasLeadingParton() && hasSubLeadingParton()) {
                                            return fabs(deltaPhi(leadingParton()->phi(),subLeadingParton()->phi()));
                                      }else{return -9999.;}}
        float dPhi_13_Partons() const {if (hasLeadingParton() && hasSubSubLeadingParton()) {
                                            return fabs(deltaPhi(leadingParton()->phi(),subSubLeadingParton()->phi()));
                                      }else{return -9999.;}}
        float dPhi_23_Partons() const {if (hasSubLeadingParton() && hasSubSubLeadingParton()) {
                                            return fabs(deltaPhi(subLeadingParton()->phi(),subSubLeadingParton()->phi()));
                                      }else{return -9999.;}}
        float dPhi_max_Partons() const {if (hasLeadingParton() && hasSubLeadingParton() && hasSubSubLeadingParton()) {
                                            if (dPhi_12_Partons() > dPhi_13_Partons() && dPhi_12_Partons() > dPhi_23_Partons()) {return dPhi_12_Partons();}
                                            else if (dPhi_13_Partons() > dPhi_12_Partons() && dPhi_13_Partons() > dPhi_23_Partons()) {return dPhi_13_Partons();}
                                            else {return dPhi_23_Partons();}}
                                        else {return -9999.;}}
        float dPhi_min_Partons() const {if (hasLeadingParton() && hasSubLeadingParton() && hasSubSubLeadingParton()) {
                                            if (dPhi_12_Partons() < dPhi_13_Partons() && dPhi_12_Partons() < dPhi_23_Partons()) {return dPhi_12_Partons();}
                                            else if (dPhi_13_Partons() < dPhi_12_Partons() && dPhi_13_Partons() < dPhi_23_Partons()) {return dPhi_13_Partons();}
                                            else {return dPhi_23_Partons();}}
                                        else {return -9999.;}}
        float dPhi_min_max_Partons() const {if (hasLeadingParton() && hasSubLeadingParton() && hasSubSubLeadingParton()) {
                                                return dPhi_max_Partons() - dPhi_min_Partons();
                                           }else{return -9999.;}}

        //4-Momentum simplex volume
        float simplex_volume_DP_12_FggJet() const {
            if (hasDiPhoton() && hasLeadingJet() && hasSubLeadingJet()) {
                float volume = momentumSimplexVolume( leadingJet()->p4(),subLeadingJet()->p4());
                return volume;
            }else{return -9999.;} 
        }
        float simplex_volume_DP_12_GenJet() const {
            if (hasDiPhoton() && hasClosestGenJetToLeadingJet() && hasClosestGenJetToSubLeadingJet()) {
                float volume = momentumSimplexVolume( closestGenJetToLeadingJet()->p4(),closestGenJetToSubLeadingJet()->p4());
                return volume;
            }else{return -9999.;} 
        }
        float simplex_volume_DP_12_GenParticle() const {
            if (hasDiPhoton() && hasClosestParticleToLeadingJet() && hasClosestParticleToSubLeadingJet()) {
                float volume = momentumSimplexVolume( closestParticleToLeadingJet()->p4(),closestParticleToSubLeadingJet()->p4());
                return volume;
            }else{return -9999.;} 
        }
        float simplex_volume_DP_12_Partons() const {
            if (hasDiPhoton() && hasLeadingParton() && hasSubLeadingParton()) {
                float volume = momentumSimplexVolume( leadingParton()->p4(),subLeadingParton()->p4());
                return volume;
            }else{return -9999.;} 
        }

        float momentumSimplexVolume (ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > jet1,
                                     ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > jet2) const {

            float volume(0.0);
            std::vector<float> p1(4);
            std::vector<float> p2(4);
            std::vector<float> p3(4);
            std::vector<float> p4(4);

            p1[0] = jet1.T();p1[1] = jet1.X();p1[2] = jet1.Y();p1[3] = jet1.Z();
            p2[0] = jet2.T();p2[1] = jet2.X();p2[2] = jet2.Y();p2[3] = jet2.Z();
            p3[0] = diPhoton()->leadingPhoton()->p4().T();p3[1] = diPhoton()->leadingPhoton()->p4().X();
            p3[2] = diPhoton()->leadingPhoton()->p4().Y();p3[3] = diPhoton()->leadingPhoton()->p4().Z();
            p4[0] = diPhoton()->subLeadingPhoton()->p4().T();p4[1] = diPhoton()->subLeadingPhoton()->p4().X();
            p4[2] = diPhoton()->subLeadingPhoton()->p4().Y();p4[3] = diPhoton()->subLeadingPhoton()->p4().Z();

            for (unsigned i(0);i<4;i++) {
                for (unsigned j(0);j<4;j++) {                                      
                    for (unsigned k(0);k<4;k++) {                                      
                        for (unsigned l(0);l<4;l++) {                                      
                            //Levi-Civita
                            std::vector<unsigned> indices(4);
                            indices[0] = i; indices[1] = j; indices[2] = k; indices[3] = l;
                            int element(1);
                            for (unsigned m(0);m<4;m++) {
                                for (unsigned n(0);n<4;n++) {
                                    if (m <= n) continue;     
                                    int value = indices[m]-indices[n];
                                    if (value < 0) {element *= -1;}
                                    else if (value == 0) {element *= 0;} 
                                } 
                            }
                            //Momentum elements
                            float temp(1.0);
                            temp *= p1[i]*p2[j]*p3[k]*p4[l];
                            temp *= (float)element;
                            volume += temp;
                        }
                    }
                }
            }
            return fabs((1.0/24.0)*volume);
        }





        //Has (thing) methods
        bool hasClosestGenJetToLeadingJet() const { return closestGenJetToLeadingJet_.isNonnull(); }
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

        //Getter methods
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

        //Setter methods
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
        const std::vector<edm::Ptr<reco::GenParticle>> ptOrderedPartons() const {return ptOrderedPartons_;}
        const std::vector<edm::Ptr<reco::GenJet>> ptOrderedGenJets() const {return ptOrderedGenJets_;}
        const std::vector<edm::Ptr<flashgg::Jet>> ptOrderedFggJets() const {return ptOrderedFggJets_;}
        
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

        //Clone
        VBFTagTruth *clone() const;

    private:
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
