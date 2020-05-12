#ifndef FLASHgg_GenDiPhoton_h
#define FLASHgg_GenDiPhoton_h

#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include "flashgg/DataFormats/interface/GenPhotonExtra.h"
#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/JetReco/interface/GenJet.h"

#include "flashgg/DataFormats/interface/WeightedObject.h"
#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"

#include "TLorentzVector.h"

namespace flashgg {

    class GenDiPhoton : public reco::LeafCandidate, public WeightedObject
    {
    public:
        GenDiPhoton();
        GenDiPhoton( edm::Ptr<flashgg::GenPhotonExtra>, edm::Ptr<flashgg::GenPhotonExtra>);
        GenDiPhoton( edm::Ptr<flashgg::GenPhotonExtra>, edm::Ptr<flashgg::GenPhotonExtra>, edm::Ptr<reco::GenJet>, edm::Ptr<reco::GenJet>);
        ~GenDiPhoton();

        virtual GenDiPhoton *clone() const { return ( new GenDiPhoton( *this ) ); }
        

        const flashgg::GenPhotonExtra::cand_type* leadingPhoton() const { return   &(leadingPhoton_.cand()); };
        const flashgg::GenPhotonExtra::cand_type* subLeadingPhoton() const { return &(subLeadingPhoton_.cand()); }

        
        const reco::GenJet & leadingJet() const { return *leadingJet_; };
        const reco::GenJet & subLeadingJet() const { return *subLeadingJet_; }

        const flashgg::GenPhotonExtra& leadingExtra() const { return leadingPhoton_; };
        const flashgg::GenPhotonExtra& subLeadingExtra() const { return subLeadingPhoton_; }
        
        float sumPt() const { return  leadingPhoton_.cand().pt() + subLeadingPhoton_.cand().pt(); }
        bool operator <( const GenDiPhoton &b ) const { return ( sumPt() < b.sumPt() ); }
        bool operator >( const GenDiPhoton &b ) const { return ( sumPt() > b.sumPt() ); }

        float sumHt() const { return  sumPt() + leadingJet().pt() + subLeadingJet().pt(); }
        
        LorentzVector dijet() const; 
        
        void setTag(const std::string & tag) { tag_ = tag; }
        std::string tag() const { return tag_; }
        bool isTagged(const std::string & cmp) const { return tag_ == cmp; }
        void setTagObj(const edm::Ptr<DiPhotonTagBase> recoTagObj) { recoTagObj_ = recoTagObj; }
        const edm::Ptr<DiPhotonTagBase> recoTagObj() const { return recoTagObj_; }
        
        
        void setCategoryNumber(int cat) { cat_ = cat; }
        int categoryNumber() const { return cat_; }
        operator int() const { return categoryNumber(); }

        void setHHbbggBenchmarkReweight(std::vector<float> x) { HHbbgg_benchmark_reweights_ = x; }
        float getHHbbggBenchmarkReweight(int targetNode) const { return HHbbgg_benchmark_reweights_[targetNode]; }
        float HHbbggCosThetaStar_CS() const;
        
        void setmHHgen(const float &mHHgen)                         { mHHgen_ = mHHgen; }
        void setcosthetaHHgen(const float &costhetaHHgen)           { costhetaHHgen_ = costhetaHHgen; }
        void setptHsgen(const float &ptH1gen, const float &ptH2gen) { ptH1gen_=ptH1gen; ptH2gen_=ptH2gen; }

        float getmHHgen() const         { return mHHgen_; }
        float getcosthetaHHgen() const  { return costhetaHHgen_; }
        float getptH1gen() const        { return ptH1gen_; }
        float getptH2gen() const        { return ptH2gen_; }


    private:
        void computeP4AndOrder();

        flashgg::GenPhotonExtra leadingPhoton_;
        flashgg::GenPhotonExtra subLeadingPhoton_;
        
        edm::Ptr<reco::GenJet> leadingJet_;
        edm::Ptr<reco::GenJet> subLeadingJet_;
        vector<float> HHbbgg_benchmark_reweights_;
        float mHHgen_,costhetaHHgen_,ptH1gen_,ptH2gen_;

        int cat_;
        std::string tag_;
        edm::Ptr<DiPhotonTagBase> recoTagObj_;
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

