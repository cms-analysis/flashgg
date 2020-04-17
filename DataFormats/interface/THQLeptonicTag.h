#ifndef FLASHgg_THQLeptonicTag_h
#define FLASHgg_THQLeptonicTag_h

#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "flashgg/DataFormats/interface/Muon.h"
#include "flashgg/DataFormats/interface/Electron.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "flashgg/DataFormats/interface/Met.h"
#include "flashgg/DataFormats/interface/WeightedObject.h"
//#include "flashgg/DataFormats/interface/THQLeptonicMVAResult.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "TLorentzVector.h"

using namespace edm;

#include <string>
#include <map>

namespace flashgg {

    class THQLeptonicTag: public DiPhotonTagBase
    {
    public:
        THQLeptonicTag();
        THQLeptonicTag( edm::Ptr<DiPhotonCandidate>, edm::Ptr<DiPhotonMVAResult> );
        THQLeptonicTag( edm::Ptr<DiPhotonCandidate>, DiPhotonMVAResult );

        ~THQLeptonicTag();

        THQLeptonicTag *clone() const { return ( new THQLeptonicTag( *this ) ); }

        const std::vector<edm::Ptr<reco::Vertex> > vertices() const { return vertices_;}
        const std::vector<edm::Ptr<Muon> > muons() const { return Muons_;}
        const std::vector<edm::Ptr<flashgg::Electron> > electrons() const {return Electrons_;}
        const std::vector<edm::Ptr<Jet> > jets() const { return Jets_;}
        const std::vector<edm::Ptr<Jet> > Jets_EtaSorted() const { return Jets_EtaSorted_;}
        const std::vector<edm::Ptr<Jet> > bJets() const { return BJets_;}
        const std::vector<edm::Ptr<Jet> > centralJet() const { return CentralJet_ ;}
        const std::vector<edm::Ptr<Jet> > forwardJet() const { return ForwardJet_ ;}

        //DeltaRhis between leptons and MET
        float dPhi_Muon1_MET() const  { 
            return deltaPhi( muons()[0]->phi(),  MET_->phi()); 
        }
        
        float dPhi_Muon2_MET() const  { 
            return deltaPhi( muons()[1]->phi(),  MET_->phi()); 
        }
        float dPhi_Electron1_MET() const  { 
            return deltaPhi( electrons()[0]->phi(),  MET_->phi()); 
        }
        
        float dPhi_Electron2_MET() const  { 
            return deltaPhi( electrons()[1]->phi(),  MET_->phi()); 
        }
        
        
        float getElecAlpha(int eleIndex) const{
            float eleta =  electrons()[eleIndex]->eta();

            //for isolation recalculation        
            float Aeff = 0;
            //cmssw/RecoEgamma/ElectronIdentification/data/Summer16/effAreaElectrons_cone03_pfNeuHadronsAndPhotons_80X.txt
            if( fabs(eleta) <= 1.0000 ){
                Aeff = 0.1703;
            } else if( fabs(eleta) <= 1.4790 ){
                Aeff = 0.1715;
            } else if( fabs(eleta) <= 2.0000 ){
                Aeff = 0.1213;
            } else if( fabs(eleta) <= 2.2000 ){
                Aeff = 0.1230;
            } else if( fabs(eleta) <= 2.3000 ){
                Aeff = 0.1635;
            } else if( fabs(eleta) <= 2.4000 ){
                Aeff = 0.1937;
            } else if( fabs(eleta) <= 5.0000 ){
                Aeff = 0.2393;
            }

            return Aeff;
            
        };
        
        float getrho() const{
            return rho_;
        }

        int getLeptonCharge() const{
            if( LeptonType_ == 1 )
                return electrons()[0]->charge();
            else if(LeptonType_ == 2 )
                return muons()[0]->charge();

            return 0;
        }
        reco::Candidate::LorentzVector getLeptonP4() const{
            if( LeptonType_ == 1 )
                return electrons()[0]->p4();
            else if(LeptonType_ == 2 )
                return muons()[0]->p4();

            reco::Candidate::LorentzVector ret;
            return ret;
        }
        

        int getLeptonType() const{
            return LeptonType_;
        }
//getdR
        float getdRtHchainfwdjet() const{
            return dRtHchainfwdjet_;
        }
        float getdRbjetfwdjet() const{
            return dRbjetfwdjet_;
        }
        
        float getmyfunction() const{
            return dRmyfunval_;
        }
        float getdRsubleadphobjet() const{
            return dRsubleadphobjet_;
        }
        float getdRleadphofwdjet() const{
            return dRleadphofwdjet_;
        }
        float getdRsubleadphofwdjet() const{
            return dRsubleadphofwdjet_;
        }
        float getdRleptonbjet() const{
            return dRleptonbjet_;
        }
        float getdRleptonfwdjet() const{
            return dRleptonfwdjet_;
        }
        float getdEtaleptonfwdjet() const{
            return dEtaleptonfwdjet_;
        }
        float gettop_mt() const{
            return top_mt_;
        }
        float gettop_mass() const{
            return top_mass_;
        }
        float getlepton_ch() const{
            return lepton_ch_;
        }
        float getlepton_leadPt() const{
            return lepton_leadPt_;
        }
        float getlepton_leadEta() const{
            return lepton_leadEta_;
        }
        float getmvaresult() const{
            return mvaresult_;
        }
        float getthq_mvaresult() const{
            return thq_mvaresult_;
        }
        float getthq_mvaresult_ForNonPeakingBkg() const{
            return thq_mvaresult_ForNonPeakingBkg_;
        }
        float getMVAscore_ttHvstHDNN() const{
            return MVAscore_ttHvstHDNN_;
        }
        double getlikelihood() const{
            return likelihood_result_;
        }
        const std::vector<float> bDiscriminatorValue_bjets() const{
            return bDiscriminatorValue_bjets_;
        }
        const std::vector<float> bDiscriminatorValue_jets() const{
            return bDiscriminatorValue_jets_;
        }
        const std::vector<float> bDiscriminatorValue_fwdjets() const{
            return bDiscriminatorValue_fwdjets_;
        }
//

        float thqleptonicMvaRes(string label) const {
            int index = findIndex(label);
            if(index < 0 )
                return -100;
            return thqleptonicMvaRes_[index];
        }
        const Ptr<Jet> getFwdJet(string label) const{
            int index = findIndex(label);
            if(index < 0 ){
                edm::Ptr<Jet> ret;
                return ret;
            }
            return fwdJet[index];
        }
        const Ptr<Jet> getbJet(string label) const{
            int index = findIndex(label);
            if(index < 0 ){
                edm::Ptr<Jet> ret;
                return ret;
            }

            return bJet[index];
        }
        float getTopMass(string label) const{
            int index = findIndex(label);
            if(index < 0 )
                return -100;

            return TopMass[findIndex(label)];
        }

        float getFoxWolframMoment_ONE()const{
            return FoxWolframMoment_ONE;
        }
        float getAplanarity() const{
            return Aplanarity;
        }

        float getHT() const{
            return ht;
        }

        const Ptr<Met> getRECOMET() const{
            return MET_;
        }
        const float getRECOMET_Pt() const{
            return MET_->getCorPt();
        }
        const float getRECOMET_Eta() const{
            return MET_->eta();
        }
        const float getRECOMET_Phi() const{
            return MET_->getCorPhi();
        }

        float getMET_Pt(string label) const{
            int index = findMETIndex(label);
            if(index < 0 )
                return -100;

            return MET_Pt[findMETIndex(label)];
        }
        float getMET_Eta(string label) const{
            int index = findMETIndex(label);
            if(index < 0 )
                return -100;

            return MET_Eta[findMETIndex(label)];
        }
        float getMET_Phi(string label) const{
            int index = findMETIndex(label);
            if(index < 0 )
                return -100;

            return MET_Phi[findMETIndex(label)];
        }
        float getMET_E(string label) const{
            int index = findMETIndex(label);
            if(index < 0 )
                return -100;

            return MET_E[findMETIndex(label)];
        }

        const Ptr<reco::Vertex> getVertex( int vtx_index ) const {
            if(vtx_index < 0 ){
                edm::Ptr<reco::Vertex> ret;
                return ret;
            }
            return vertices_[vtx_index];
        }

        float getLeadingLeptonVertexDxy( std::string lepton) const{
            std::map <std::string, std::vector<float> >::const_iterator it = vtx_dxy_.find(lepton);
            if (it->second.size()>0)
                return it->second[0];
            else
                return -999;
        }

        float getSubLeadingLeptonVertexDxy( std::string lepton) const{
            std::map <std::string, std::vector<float> >::const_iterator it = vtx_dxy_.find(lepton);
            if (it->second.size()>1)
                return it->second[1];
            else
                return -999;
        }
        float getLeadingLeptonVertexDz( std::string lepton) const{
            std::map <std::string, std::vector<float> >::const_iterator it = vtx_dz_.find(lepton);
            if (it->second.size()>0)
                return it->second[0];
            else
                return -999;
        }

        float getSubLeadingLeptonVertexDz( std::string lepton) const{
            std::map <std::string, std::vector<float> >::const_iterator it = vtx_dz_.find(lepton);
            if (it->second.size()>1)
                return it->second[1];
            else
                return -999;
        }
        float getLeadingLeptonDiphoVertexDxy( std::string lepton) const{
            std::map <std::string, std::vector<float> >::const_iterator it = diphovtx_dxy_.find(lepton);
            if (it->second.size()>0)
                return it->second[0];
            else
                return -999;
        }
        float getLeadingLeptonDiphoVertexDz( std::string lepton) const{
            std::map <std::string, std::vector<float> >::const_iterator it = diphovtx_dz_.find(lepton);
            if (it->second.size()>0)
                return it->second[0];
            else
                return -999;
        }
        float getSubLeadingLeptonDiphoVertexDxy( std::string lepton ) const{
            std::map <std::string, std::vector<float> >::const_iterator it = diphovtx_dxy_.find(lepton);
            if (it->second.size()>1)
                return it->second[1];
            else
                return -999;
        }
        float getSubLeadingLeptonDiphoVertexDz( std::string lepton) const{
            std::map <std::string, std::vector<float> >::const_iterator it = diphovtx_dz_.find(lepton);
            if (it->second.size()>1)
                return it->second[1];
            else
                return -999;
        }
        float getLeadingElectronMisHits( ) const{
            if (eleMisHits_.size()>0)
                return eleMisHits_[0];
            else
                return -999;
        }
        float getSubLeadingElectronMisHits( ) const{
            if (eleMisHits_.size()>1)
                return eleMisHits_[1];
            else
                return -999;
        }

        
        float getAlphaUp() const { return alphaUp_; }
        float getAlphaDown() const { return alphaDown_; }
        float getScale(unsigned i) const { return scale_[i]; }
        float getPdf(unsigned i) const { return pdf_[i]; }
        float getPdfNLO() const { return pdfnlo_; }
        float getCtCv(unsigned i) const { return ctcv_[i]; }
        
        void setJets( std::vector<edm::Ptr<Jet> > Jets, std::vector<edm::Ptr<Jet> > Jets_Eta ) {
            Jets_ = Jets; 
            Jets_EtaSorted_ = Jets_Eta;
        }
        void setBJets( std::vector<edm::Ptr<Jet> > BJets )  { BJets_ = BJets;}
//----------------------------------------------------------------------------
        void setcentraljet( std::vector<edm::Ptr<Jet> > CentralJet ) { CentralJet_ = CentralJet ;}
        void setforwardjet( std::vector<edm::Ptr<Jet> > ForwardJet ) { ForwardJet_ = ForwardJet ;}

//----------------------------------------------------------------------------


        void setVertices( std::vector<edm::Ptr<reco::Vertex> > vertices ) {
            vertices_ = vertices;
        }

        void setLeptonVertices( std::string lepton, std::vector<float> a, std::vector<float> b, std::vector<float> c, std::vector<float> d) {

            vtx_dxy_.insert(std::make_pair(lepton,a));
            vtx_dz_.insert(std::make_pair(lepton,b));
            diphovtx_dxy_.insert(std::make_pair(lepton,c));
            diphovtx_dz_.insert(std::make_pair(lepton,d));
            
        }

        int NLooseMu25 , NLooseMu15 , NMediumMu25 , NMediumMu15 , NTightMuo25 , NTightMuo15 ;
        void setMuons( std::vector<edm::Ptr<Muon> > Muons , std::vector<int> passTight , int n_LooseMu25 , int n_LooseMu15 , int n_MediumMu25 , int n_MediumMu15 , int n_TightMuo25 , int n_TightMuo15 ) {
            Muons_ = Muons;
//            assert( Muons.size() == passTight.size() );
            MuPassTight_ = passTight;

            NLooseMu15 = n_LooseMu15 ;
            NLooseMu25 = n_LooseMu25 ;
            NMediumMu15 = n_MediumMu15;
            NMediumMu25 = n_MediumMu25;
            NTightMuo15 = n_TightMuo15;
            NTightMuo25 = n_TightMuo25;
        }

        int n_Ele_Loose , n_Ele_Veto, n_Ele_Medium, n_Ele_Tight;
        void setElectrons( std::vector<edm::Ptr<Electron> > Electrons , std::vector<int> passTight, std::vector<int> passVeto , int n_loose , int n_veto , int n_medium , int n_tight ) {
            Electrons_ = Electrons;
//            assert( passTight.size() == Electrons.size() );
            ElePassTight_ = passTight ;
            assert( ElePassTight_.size() == passVeto.size() );
            ElePassVeto_ = passVeto;

            n_Ele_Veto = n_veto;
            n_Ele_Medium = n_medium ;
            n_Ele_Tight = n_tight;
            n_Ele_Loose = n_loose;
        }
        void setMVAres(string label, float val , float topMass , Ptr<Jet> fwd , Ptr<Jet> bj ) {
            bAssignmentLabels.push_back( label);
            thqleptonicMvaRes_.push_back(val); 
            TopMass.push_back(topMass);
            fwdJet.push_back( fwd ) ;
            bJet.push_back( bj ) ;
        }

        void setRECOMET (edm::Ptr<Met>  MET) {MET_=MET;}
        void setFoxAndAplanarity( float fox , float aplan ){ 
            FoxWolframMoment_ONE = fox ;
            Aplanarity = aplan;

        }
        void setHT(float val){
            ht=val;
        }

        void setMETPtEtaPhiE(  string label, float metPt, float metEta, float metPhi, float metE ){
            metAssignmentLabels.push_back( label);
            MET_Pt.push_back(metPt) ;  MET_Eta.push_back(metEta) ; MET_Phi.push_back(metPhi) ; MET_E.push_back(metE);
        }
        void setMETPxPyPzE(  string label, float metPx, float metPy, float metPz, float metE ){
            metAssignmentLabels.push_back( label);
            MET_Px.push_back(metPx) ;  MET_Py.push_back(metPy) ; MET_Pz.push_back(metPz) ; MET_E.push_back(metE);
        }

        void setLeptonType(int val){ LeptonType_ = val; }

        void setrho(float rho){
            rho_=rho;
        }
        
        void setElectronMisHits(float misHits){
            eleMisHits_.push_back(misHits);
        }
//SET dR
        void setdRtHchainfwdjet(float drtHchainfwdjet ){
             dRtHchainfwdjet_ = drtHchainfwdjet ;
        }
        void setdRbjetfwdjet(float drbjetfwdjet ){
             dRbjetfwdjet_ = drbjetfwdjet ;
        }
        void setdRleadphobjet(float drleadphobjet_){
            dRmyfunval_ = drleadphobjet_;
        }
        void setdRsubleadphobjet(float drsubleadphobjet){
            dRsubleadphobjet_ = drsubleadphobjet;
        }
        void setdRleadphofwdjet(float drleadphofwdjet){
            dRleadphofwdjet_ = drleadphofwdjet;
        }
        void setdRsubleadphofwdjet(float drsubleadphofwdjet){
            dRsubleadphofwdjet_ = drsubleadphofwdjet;
        }
        void setdRleptonbjet( float drleptonbjet ){
            dRleptonbjet_ = drleptonbjet;
        }
        void setdRleptonfwdjet( float drleptonfwdjet ){
            dRleptonfwdjet_ = drleptonfwdjet;
        }
        void setdEtaleptonfwdjet(float dEtaleptonfwdjet){
            dEtaleptonfwdjet_ = dEtaleptonfwdjet;
        }
        void settop_mt(float top_mt){
            top_mt_ = top_mt;
        }
        void settop_mass(float top_mass){
            top_mass_ = top_mass;
        }
        void setlepton_ch(float lepton_ch){
            lepton_ch_ = lepton_ch;
        }
        void setlepton_leadPt(float lepton_leadPt){
            lepton_leadPt_ = lepton_leadPt;
        }
        void setlepton_leadEta(float lepton_leadEta){
            lepton_leadEta_ = lepton_leadEta;
        }
        void setmvaresult( float mvaresult ){
            mvaresult_ = mvaresult;
        }
        void setlikelihood( double likelihood_result ){
            likelihood_result_ = likelihood_result;
        } 
        void setthq_mvaresult( float thq_mvaresult ){
            thq_mvaresult_ = thq_mvaresult;
        }
        void setthq_mvaresult_ForNonPeakingBkg( float thq_mvaresult_ForNonPeakingBkg ){
            thq_mvaresult_ForNonPeakingBkg_ = thq_mvaresult_ForNonPeakingBkg;
        }
        void setMVAscore_ttHvstHDNN( float MVAscore_ttHvstHDNN ){
            MVAscore_ttHvstHDNN_ = MVAscore_ttHvstHDNN;
        }
        void setbDiscriminatorValue( std::vector<float> bDiscriminatorValue_bjets, std::vector<float> bDiscriminatorValue_jets, std::vector<float> bDiscriminatorValue_fwdjets ){
            bDiscriminatorValue_bjets_ = bDiscriminatorValue_bjets;
            bDiscriminatorValue_jets_ = bDiscriminatorValue_jets;
            bDiscriminatorValue_fwdjets_ = bDiscriminatorValue_fwdjets;
        }

      
//
        int nMedium_bJets, nLoose_bJets, nTight_bJets, nCentralJets , nForwardJets;
        double bTagWeight, bTagWeightUp, bTagWeightDown;
        double photonWeights;
        const std::vector<int> ElePassVeto() const{
            return ElePassVeto_;
        }
        const std::vector<int> ElePassTight() const{
            return ElePassTight_;
        }
        const std::vector<int> MuPassTight() const{
            return MuPassTight_;
        }

        void setAlphaUp(float val) { alphaUp_ = val; }
        void setAlphaDown(float val) { alphaDown_ = val; }
        void setScale(unsigned i, float val) { scale_[i] = val; }
        void setPdf(unsigned i, float val) { pdf_[i] = val; }
        void setPdfNLO(float val) { pdfnlo_ = val; }
        void setCtCv(unsigned i, float val) { ctcv_[i] = val; }

    private:

        int findIndex(string label) const{
            ptrdiff_t pos = find(bAssignmentLabels.begin(), bAssignmentLabels.end(), label) - bAssignmentLabels.begin();
            if( pos < int( bAssignmentLabels.size() ) )
                return pos;
            else{
                return -1;
            }
        }
        int findMETIndex(string label) const{
            ptrdiff_t pos = find(metAssignmentLabels.begin(), metAssignmentLabels.end(), label) - metAssignmentLabels.begin();
            if( pos < int( metAssignmentLabels.size() ) )
                return pos;
            else{
                return -1;
            }
        }
        

        std::vector<int> ElePassVeto_;
        std::vector<int> ElePassTight_ ;
        std::vector<int> MuPassTight_ ;

        std::vector<edm::Ptr<Muon> > Muons_;
        std::vector<edm::Ptr<Electron> > Electrons_;
        std::vector<edm::Ptr<Jet> > Jets_;
        std::vector<edm::Ptr<Jet> > Jets_EtaSorted_;
        std::vector<edm::Ptr<Jet> > BJets_;
        std::vector<edm::Ptr<Jet> > CentralJet_;
        std::vector<edm::Ptr<Jet> > ForwardJet_;
        edm::Ptr<flashgg::Met> MET_;
        std::vector<float> thqleptonicMvaRes_;
        std::vector<edm::Ptr<reco::Vertex> > vertices_;
        std::map <std::string, std::vector<float> > vtx_dxy_; std::map <std::string, std::vector<float> > vtx_dz_;
        std::map <std::string, std::vector<float> > diphovtx_dxy_; std::map <std::string, std::vector<float> > diphovtx_dz_;
        std::vector<float> eleMisHits_;
        float rho_;
        float dRtHchainfwdjet_=0;
        float dRbjetfwdjet_;
        float dRleadphobjet_;
        float dRsubleadphobjet_;
        float dRleadphofwdjet_;
        float dRsubleadphofwdjet_;
        float dRleptonbjet_;
        float dRleptonfwdjet_;
        float dEtaleptonfwdjet_;
        float top_mt_;
        float top_mass_;
        float lepton_ch_;
        float lepton_leadPt_;
        float lepton_leadEta_;
        float mvaresult_;
        float thq_mvaresult_;
        float thq_mvaresult_ForNonPeakingBkg_;
        float MVAscore_ttHvstHDNN_;
        double likelihood_result_;
        std::vector<float> bDiscriminatorValue_bjets_;
        std::vector<float> bDiscriminatorValue_jets_;
        std::vector<float> bDiscriminatorValue_fwdjets_;
        float dRmyfunval_;
        int LeptonType_;
        std::vector< std::string > bAssignmentLabels;
        std::vector< Ptr<Jet> > fwdJet ;
        std::vector< Ptr<Jet> > bJet;
        float FoxWolframMoment_ONE;
        float Aplanarity;
        float ht;
        std::vector<float> TopMass;
        std::vector< std::string > metAssignmentLabels;
        std::vector<float> MET_Pt; std::vector<float> MET_Eta; std::vector<float> MET_Phi; 
        std::vector<float> MET_Px; std::vector<float> MET_Py; std::vector<float> MET_Pz;
        std::vector<float> MET_E;

        
        float alphaUp_;
        float alphaDown_;
        float scale_[8];
        float pdf_[100];
        float pdfnlo_;
        float ctcv_[70];
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

