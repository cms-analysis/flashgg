#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "flashgg/DataFormats/interface/DiPhotonMVAResult.h"
#include "flashgg/DataFormats/interface/DoubleHTag.h"
#include "flashgg/DataFormats/interface/TagTruthBase.h"
#include "DataFormats/Common/interface/RefToPtr.h"

#include "TLorentzVector.h"


#include <vector>
#include <algorithm>
#include "TH2F.h"
#include "TFile.h"

using namespace std;
using namespace edm;

namespace flashgg {

    class DoubleHReweighter : public EDProducer
    {

    public:
        DoubleHReweighter( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;
        float getWeight( int targetNode,float gen_mHH, float gen_cosTheta);
        float getCosThetaStar_CS(TLorentzVector h1, TLorentzVector h2);
        float functionGF(float kl, float kt, float c2, float cg, float c2g, vector<double> A);
        pair<int,int> find2DBin(TH2* h, float x, float y);

        EDGetTokenT<View<reco::GenParticle> > genParticleToken_;
        int doReweight_;
        edm::FileInPath weightsFile_;  // file with prepared histograms needed for reweighting
        const unsigned int NUM_benchmarks = 15;  // number of becnhmarks for reweighting 12 +1 SM = 13, box=14, fake2017SM=15 
        const unsigned int numberSMbenchmark = 13;  // index of SM benchmark 
        const unsigned int numberBoxbenchmark = 14;  // index of SM benchmark 
        const unsigned int numberFakebenchmark = 15;  // index of SM benchmark 
        std::vector<TH2F*> hists_params_;   // histograms with weights for 15 parameters
        TH2F* hist_SM_;  // histogram for SM point
        TH2F* hist_inputMix_;  // histogram for input mix of nodes
        unsigned int NCOEFFSA_;// number of needed coefficient to describe the phase space 
        vector<double> A_13TeV_SM_;
        TFile* f_weights_ ;
        edm::ParameterSet benchmarks_map_ ;

    };

    DoubleHReweighter::DoubleHReweighter( const ParameterSet &iConfig ) :
        genParticleToken_( consumes<View<reco::GenParticle> >( iConfig.getParameter<InputTag> ( "GenParticleTag" ) ) ),
        doReweight_( iConfig.getParameter<int> ( "doReweight" ) ),
        weightsFile_(iConfig.getUntrackedParameter<edm::FileInPath>("weightsFile")),
        NCOEFFSA_(iConfig.getParameter<unsigned int>( "NCOEFFSA" )),
        A_13TeV_SM_(iConfig.getParameter< vector<double>>("A_13TeV_SM")),
        benchmarks_map_(iConfig.getParameter<edm::ParameterSet>("benchmarks_map"))
    {

            f_weights_ = new TFile((weightsFile_.fullPath()).c_str(), "READ");
            
            for (unsigned int n=0; n<NCOEFFSA_; n++){
                hists_params_.push_back((TH2F*)f_weights_->Get(Form("h_A%i",n)));
                if (!(hists_params_[n])) throw cms::Exception( "Configuration" ) << "The file "<<weightsFile_.fullPath()<<" provided for reweighting full grid does not contain the expected histogram number : "<<n<<std::endl;
            }
            hist_SM_ = (TH2F*)f_weights_->Get("h_SM");
            if (!(hist_SM_)) throw cms::Exception( "Configuration" ) << "The file "<<weightsFile_.fullPath()<<" provided for reweighting benchmarks does not contain the expected SM histogram."<<std::endl;
            hist_inputMix_ = (TH2F*)f_weights_->Get("allHHNodeMap2D");
            if (!(hist_inputMix_)) throw cms::Exception( "Configuration" ) << "The file "<<weightsFile_.fullPath()<<" provided for reweighting benchmarks does not contain the expected input histogram for mix of nodes."<<std::endl;
    
        for (unsigned int num=0;num<NUM_benchmarks;num++)
             if (num==(numberSMbenchmark-1)) produces<float>("benchmarkSM");
             else if (num==(numberBoxbenchmark-1)) produces<float>("benchmarkBox");
             else if (num==(numberFakebenchmark-1)) produces<float>("benchmark2017fake");
             else produces<float>(Form("benchmark%i",num));
    }   
    
// return bin in 2D isto wihtout under/over flow (e.g. if ibin > ibinmax , ibin = ibinmax)
    pair<int,int> DoubleHReweighter::find2DBin(TH2* h, float x, float y)
    {
        int ibinx = h->GetXaxis()->FindBin(x);
        int ibiny = h->GetYaxis()->FindBin(y);
        if (ibinx <= 0) ibinx = 1;
        if (ibinx > h->GetNbinsX()) ibinx = h->GetNbinsX();
        if (ibiny <= 0) ibiny = 1;
        if (ibiny > h->GetNbinsY()) ibiny = h->GetNbinsY();

        return make_pair(ibinx, ibiny);
}

    float DoubleHReweighter::functionGF(float kl, float kt, float c2, float cg, float c2g, vector<double> A)
    {
         return ( A[0]*pow(kt,4) + A[1]*pow(c2,2) + (A[2]*pow(kt,2) + A[3]*pow(cg,2))*pow(kl,2) + A[4]*pow(c2g,2) + ( A[5]*c2 + A[6]*kt*kl )*pow(kt,2) + (A[7]*kt*kl + A[8]*cg*kl )*c2 + A[9]*c2*c2g + (A[10]*cg*kl + A[11]*c2g)*pow(kt,2)+ (A[12]*kl*cg + A[13]*c2g )*kt*kl + A[14]*cg*c2g*kl );
    }


    float DoubleHReweighter::getWeight( int targetNode,float gen_mHH, float gen_cosTheta)
    {
        float w = 0.;
        pair<int,int> bins = find2DBin(hist_inputMix_, gen_mHH, abs(gen_cosTheta));
        float denom = hist_inputMix_->GetBinContent(bins.first, bins.second);
        if (denom == 0) { 
            return 0;
        }
        float nEvSM = hist_SM_->GetBinContent(bins.first, bins.second);
        vector<double> Acoeffs;
        for (unsigned int ic = 0; ic < NCOEFFSA_; ++ic){
            Acoeffs.push_back((hists_params_[ic])->GetBinContent(bins.first, bins.second));
        } 
        double kl = benchmarks_map_.getParameter<vector<double>>("kl")[targetNode] ;  
        double kt = benchmarks_map_.getParameter<vector<double>>("kt")[targetNode];  
        double c2 = benchmarks_map_.getParameter<vector<double>>("c2")[targetNode];  
        double cg = benchmarks_map_.getParameter<vector<double>>("cg")[targetNode] ;  
        double c2g = benchmarks_map_.getParameter<vector<double>>("c2g")[targetNode];  
        double effBSM = nEvSM * functionGF(kl,kt,c2,cg,c2g,Acoeffs)/functionGF(kl,kt,c2,cg,c2g,A_13TeV_SM_);
    
        if (effBSM/denom < 0) {
            return 0;
        } // In case of very small negative weights, which can happen
        w = (effBSM/denom);

       return w;
    }
        
    float DoubleHReweighter::getCosThetaStar_CS(TLorentzVector h1, TLorentzVector h2)
    {
    // cos theta star angle in the Collins Soper frame
        TLorentzVector hh = h1 + h2;
        h1.Boost(-hh.BoostVector());                     
        return h1.CosTheta();
    }


    void DoubleHReweighter::produce( Event &evt, const EventSetup & )
    {
        // prepare output
        std::vector<edm::Ptr<reco::GenParticle> > selHiggses;
        Handle<View<reco::GenParticle> > genParticles;
        evt.getByToken( genParticleToken_, genParticles );
        for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) {
            edm::Ptr<reco::GenParticle> genPar = genParticles->ptrAt(genLoop);
            if (selHiggses.size()>1) break;
            if (genPar->pdgId()==25 && genPar->isHardProcess()){
                selHiggses.push_back(genPar);
            }   
        }
       
        std::vector<float> NRWeights; //we will use this in the future when we would like to save all weights
        if (selHiggses.size()==2){
            TLorentzVector H1,H2;
            H1.SetPtEtaPhiE(selHiggses[0]->p4().pt(),selHiggses[0]->p4().eta(),selHiggses[0]->p4().phi(),selHiggses[0]->p4().energy());
            H2.SetPtEtaPhiE(selHiggses[1]->p4().pt(),selHiggses[1]->p4().eta(),selHiggses[1]->p4().phi(),selHiggses[1]->p4().energy());
            float gen_mHH  = (H1+H2).M();
            float gen_cosTheta = getCosThetaStar_CS(H1,H2);   
            // Now, lets fill in the weigts for the 12 benchmarks.
            for (unsigned int n=0; n<NUM_benchmarks; n++){ 
                NRWeights.push_back(getWeight(n, gen_mHH, gen_cosTheta));
            }
        } 
        for (unsigned int n=0; n<NUM_benchmarks; n++){
            std::string weight_number = "benchmark";
            if (n==(numberSMbenchmark-1)) weight_number.append("SM");
            else if (n==(numberBoxbenchmark-1)) weight_number.append("Box");
            else if (n==(numberFakebenchmark-1)) weight_number.append("2017fake");
            else weight_number.append(std::to_string(n));
            std::unique_ptr<float>  final_weight( new float(NRWeights[n]) );
            evt.put( std::move( final_weight) , weight_number);
        }
         // number is a string. Each collection is specified by 4 string :  type, name of producer, process_name(reco,flashggMicroAOD),last one  - if producer produces more than one object of the same type -> here : number
    }
}




typedef flashgg::DoubleHReweighter FlashggDoubleHReweighter;
DEFINE_FWK_MODULE( FlashggDoubleHReweighter );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
