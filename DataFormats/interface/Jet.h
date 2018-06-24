#ifndef FLASHgg_Jet_h
#define FLASHgg_Jet_h

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/JetReco/interface/PileupJetIdentifier.h"
#include "flashgg/DataFormats/interface/WeightedObject.h"

namespace flashgg {

    struct MinimalPileupJetIdentifier {
        float RMS;
        float betaStar;
        int idFlag;
    };

    enum JetIDLevel {Loose=0, Tight=1, Tight2017=2};
    
    class Jet : public pat::Jet, public WeightedObject
    {
        
    public:
        Jet();
        Jet( const pat::Jet & );
        ~Jet();
        void setPuJetId( const edm::Ptr<reco::Vertex> vtx, const PileupJetIdentifier & );
        bool hasPuJetId( const edm::Ptr<reco::Vertex> vtx ) const;
        bool passesPuJetId( const edm::Ptr<reco::Vertex> vtx, PileupJetIdentifier::Id level = PileupJetIdentifier::kLoose ) const;
        void setSimpleRMS( float theRMS )  { simpleRMS_ = theRMS; }
        void setSimpleMVA( float theMVA )  { simpleMVA_ = theMVA; }
        float rms() const { return simpleRMS_; }
        float rms( const edm::Ptr<reco::Vertex> vtx ) const;
        float betaStar( const edm::Ptr<reco::Vertex> vtx ) const;
        bool passesPuJetId( const edm::Ptr<DiPhotonCandidate> dipho, PileupJetIdentifier::Id level = PileupJetIdentifier::kLoose )const;
        float rms( const edm::Ptr<DiPhotonCandidate> dipho ) const;
        float betaStar( const edm::Ptr<DiPhotonCandidate> dipho ) const;
        float puJetIdMVA() const { return simpleMVA_; }
        Jet *clone() const { return ( new Jet( *this ) ); }
        
        void  setQGL(const float qglikelihood=-99) {qglikelihood_ = qglikelihood;}
        float QGL () const {return qglikelihood_;}
        
        bool passesJetID( JetIDLevel level = Loose ) const; 

        const bool hasGenMatch() const { return (genJet() != 0); }
        
        const std::vector<float>& chEnergies() const { return chEnergies_; }
        const std::vector<float>& emEnergies() const { return emEnergies_; } 
        const std::vector<float>& neEnergies() const { return neEnergies_; } 
        const std::vector<float>& muEnergies() const { return muEnergies_; }

        void setChEnergies(std::vector<float> val) { chEnergies_ = val; }
        void setEmEnergies(std::vector<float> val) { emEnergies_ = val; } 
        void setNeEnergies(std::vector<float> val) { neEnergies_ = val; } 
        void setMuEnergies(std::vector<float> val) { muEnergies_ = val; }

    private:
        std::map<edm::Ptr<reco::Vertex>, MinimalPileupJetIdentifier> puJetId_;
        float qglikelihood_;
        float simpleRMS_; // simpler storage for PFCHS where this is not vertex-dependent
        float simpleMVA_;
        std::vector<float> chEnergies_, emEnergies_, neEnergies_, muEnergies_;
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

