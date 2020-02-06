#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "RecoJets/JetProducers/interface/PileupJetIdAlgo.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "flashgg/DataFormats/interface/VertexCandidateMap.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "DataFormats/Math/interface/deltaR.h"


#include <iostream>
#include <string>
#include <vector>

#include "PhysicsTools/TensorFlow/interface/TensorFlow.h"

#include "DataFormats/Math/interface/LorentzVector.h"
#include "TLorentzVector.h"

#define debug 0

using namespace std;
using namespace edm;

namespace flashgg {

    class bRegressionProducer : public EDProducer
    {

    public:
        bRegressionProducer( const ParameterSet & );
        ~bRegressionProducer(){};
        void InitJet();
        void SetNNVectorVar();
        std::vector<float> EvaluateNN();
    private:
        void produce( Event &, const EventSetup & ) override;
        std::vector<edm::InputTag> inputTagJets_;
        std::vector<std::string> inputJetsNames_;
        std::vector<std::string> inputJetsSuffixes_;
        edm::EDGetTokenT<double> rhoToken_;        
        string bRegressionWeightfileName_;
        double y_mean_;
        double y_std_;
        string year_;
        std::vector<edm::EDGetTokenT<edm::View<flashgg::Jet> > > jetTokens_;
        std::vector< std::string > bregtags;

        tensorflow::Session* session;
        std::vector<float> NNvectorVar_; 
        //add vector of mva for each jet

        //mva variables
        float Jet_pt ;
        float Jet_eta ;
        float rho ;
        float Jet_mt ;
        float Jet_leadTrackPt ;
        float Jet_leptonPtRel ;
        float Jet_leptonDeltaR ;
        float Jet_neHEF ;
        float Jet_neEmEF ;
        float Jet_vtxPt ;
        float Jet_vtxMass ;
        float Jet_vtx3dL ;
        float Jet_vtxNtrk ;
        float Jet_vtx3deL ;
        float Jet_numDaughters_pt03 ;
        float Jet_energyRing_dR0_em_Jet_e ;
        float Jet_energyRing_dR1_em_Jet_e ;
        float Jet_energyRing_dR2_em_Jet_e ;
        float Jet_energyRing_dR3_em_Jet_e ;
        float Jet_energyRing_dR4_em_Jet_e ;
        float Jet_energyRing_dR0_neut_Jet_e ;
        float Jet_energyRing_dR1_neut_Jet_e ;
        float Jet_energyRing_dR2_neut_Jet_e ;
        float Jet_energyRing_dR3_neut_Jet_e ;
        float Jet_energyRing_dR4_neut_Jet_e ;
        float Jet_energyRing_dR0_ch_Jet_e ;
        float Jet_energyRing_dR1_ch_Jet_e ;
        float Jet_energyRing_dR2_ch_Jet_e ;
        float Jet_energyRing_dR3_ch_Jet_e ;
        float Jet_energyRing_dR4_ch_Jet_e ;
        float Jet_energyRing_dR0_mu_Jet_e ;
        float Jet_energyRing_dR1_mu_Jet_e ;
        float Jet_energyRing_dR2_mu_Jet_e ;
        float Jet_energyRing_dR3_mu_Jet_e ;
        float Jet_energyRing_dR4_mu_Jet_e ;
        float Jet_chHEF;//implement from here
        float Jet_chEmEF;
        float Jet_leptonPtRelInv;
        int isEle;
        int isMu;
        int isOther;
        float Jet_mass;
        float Jet_withPtd;

        
    };


    bRegressionProducer::bRegressionProducer( const ParameterSet &iConfig ) :
        inputJetsNames_( iConfig.getParameter<std::vector<std::string> > ( "JetNames" )) ,
        inputJetsSuffixes_( iConfig.getParameter<std::vector<std::string> > ( "JetSuffixes" )) ,
        rhoToken_( consumes<double>(iConfig.getParameter<edm::InputTag>( "rhoFixedGridCollection" ) ) ),
        bRegressionWeightfileName_( iConfig.getUntrackedParameter<std::string>("bRegressionWeightfile")),
        y_mean_(iConfig.getUntrackedParameter<double>("y_mean")),
        y_std_(iConfig.getUntrackedParameter<double>("y_std")),
        year_(iConfig.getUntrackedParameter<std::string>("year"))
    {
        for (auto & suffix : inputJetsSuffixes_) {
            for (unsigned int i = 0; i < inputJetsNames_.size() ; i++) {
                  auto name = inputJetsNames_[i];
                  if (!suffix.empty()) inputTagJets_.push_back(edm::InputTag(name,suffix));
                  else  inputTagJets_.push_back(edm::InputTag(name));
                  std::string bregtag = suffix;
                  bregtag.append(std::to_string(i));
                  bregtags.push_back(bregtag);
            }         
        }
        for( auto & tag : inputTagJets_ ) { 
            jetTokens_.push_back( consumes<edm::View<flashgg::Jet> >( tag ) ); 
        }


        tensorflow::GraphDef* graphDef= tensorflow::loadGraphDef(bRegressionWeightfileName_.c_str());
        session = tensorflow::createSession(graphDef);

           

        Jet_pt = 0.;
        Jet_eta = 0.;
        rho = 0.;
        Jet_mt = 0.;
        Jet_leadTrackPt = 0.;
        Jet_leptonPtRel = 0.;
        Jet_leptonDeltaR = 0.;
        Jet_neHEF = 0.;
        Jet_neEmEF = 0.;
        Jet_vtxPt = 0.;
        Jet_vtxMass = 0.;
        Jet_vtx3dL = 0.;
        Jet_vtxNtrk = 0.;
        Jet_vtx3deL = 0.;
        Jet_numDaughters_pt03 = 0;
        Jet_energyRing_dR0_em_Jet_e = 0.;
        Jet_energyRing_dR1_em_Jet_e = 0.;
        Jet_energyRing_dR2_em_Jet_e = 0.;
        Jet_energyRing_dR3_em_Jet_e = 0.;
        Jet_energyRing_dR4_em_Jet_e = 0.;
        Jet_energyRing_dR0_neut_Jet_e = 0.;
        Jet_energyRing_dR1_neut_Jet_e = 0.;
        Jet_energyRing_dR2_neut_Jet_e = 0.;
        Jet_energyRing_dR3_neut_Jet_e = 0.;
        Jet_energyRing_dR4_neut_Jet_e = 0.;
        Jet_energyRing_dR0_ch_Jet_e = 0.;
        Jet_energyRing_dR1_ch_Jet_e = 0.;
        Jet_energyRing_dR2_ch_Jet_e = 0.;
        Jet_energyRing_dR3_ch_Jet_e = 0.;
        Jet_energyRing_dR4_ch_Jet_e = 0.;
        Jet_energyRing_dR0_mu_Jet_e = 0.;
        Jet_energyRing_dR1_mu_Jet_e = 0.;
        Jet_energyRing_dR2_mu_Jet_e = 0.;
        Jet_energyRing_dR3_mu_Jet_e = 0.;
        Jet_energyRing_dR4_mu_Jet_e = 0.;
        Jet_chHEF = 0.;
        Jet_chEmEF = 0.;
        Jet_leptonPtRelInv = 0.;
        isEle = 0.;
        isMu = 0.;
        isOther = 0.;
        Jet_mass = 0.;
        Jet_withPtd = 0.;

        for (auto & bregtag : bregtags) {
            produces<vector<flashgg::Jet> > (bregtag);
        }
    }



    void bRegressionProducer::produce( Event &evt, const EventSetup & )
    {
 
    for (unsigned int jet_col_idx = 0 ;jet_col_idx <jetTokens_.size()  ; jet_col_idx++) { // looping over 12 jet collections (associated to different vertecies) and all systematics
    
        // input jets
       Handle<View<flashgg::Jet> > jets;
       evt.getByToken( jetTokens_[jet_col_idx], jets );//gettting the ith jet collection (out of 12 vertecies)

       unique_ptr<vector<flashgg::Jet> > jetColl( new vector<flashgg::Jet> );
        for( unsigned int i = 0 ; i < jets->size() ; i++ ) {

            InitJet();
            
            Ptr<flashgg::Jet> pjet = jets->ptrAt( i );
            flashgg::Jet fjet = flashgg::Jet( *pjet );


            //variables needed for regression
            //you need to take uncorrected jet for variables
            Jet_pt = fjet.correctedJet("Uncorrected").pt() ;
            Jet_eta = fjet.eta() ;
            Jet_leadTrackPt = fjet.userFloat("leadTrackPt");
            edm::Handle<double> rhoHandle;
            evt.getByToken( rhoToken_, rhoHandle );
            const double rhoFixedGrd = *( rhoHandle.product() );
            rho = rhoFixedGrd;
            Jet_mt = sqrt(fjet.correctedJet("Uncorrected").energy()*fjet.correctedJet("Uncorrected").energy()-fjet.correctedJet("Uncorrected").pz()*fjet.correctedJet("Uncorrected").pz());

            //this max probably not needed, it's just heppy
            Jet_leptonPtRel = std::max(float(0.),fjet.userFloat("softLepPtRel"));
            Jet_leptonDeltaR = std::max(float(0.),fjet.userFloat("softLepDr"));
            Jet_neHEF = fjet.neutralHadronEnergyFraction();
            Jet_neEmEF = fjet.neutralEmEnergyFraction();
            Jet_chHEF = fjet.chargedHadronEnergyFraction();
            Jet_chEmEF = fjet.chargedEmEnergyFraction();
            Jet_leptonPtRelInv = fjet.userFloat("softLepPtRelInv")*Jet_pt/fjet.pt();
            
            int lepPdgID = fjet.userInt("softLepPdgId");
            if (abs(lepPdgID)==13){
                isMu=1; 
            }else if (abs(lepPdgID)==11){
                isEle=1;
            }else{
                isOther=1;
            }
            Jet_mass=fjet.correctedJet("Uncorrected").mass();
            Jet_withPtd=fjet.userFloat("ptD");
            
            if(fjet.userFloat("nSecVertices")>0){
//                float vertexX=fjet.userFloat("vtxPosX")-fjet.userFloat("vtxPx");//check if it's correct
//                float vertexY=fjet.userFloat("vtxPosY")-fjet.userFloat("vtxPy");                
//                Jet_vtxPt = sqrt(vertexX*vertexX+vertexY*vertexY);
                Jet_vtxPt=sqrt(fjet.userFloat("vtxPx")*fjet.userFloat("vtxPx")+fjet.userFloat("vtxPy")*fjet.userFloat("vtxPy"));
                Jet_vtxMass = std::max(float(0.),fjet.userFloat("vtxMass"));
                Jet_vtx3dL = std::max(float(0.),fjet.userFloat("vtx3DVal"));
                Jet_vtxNtrk = std::max(float(0.),fjet.userFloat("vtxNTracks"));
                Jet_vtx3deL = std::max(float(0.),fjet.userFloat("vtx3DSig"));
                if (Jet_vtx3deL!=0.) Jet_vtx3deL = Jet_vtx3dL/Jet_vtx3deL ;
            }
            if (fjet.emEnergies().size()>0){//since in order to save space we save this info only if the candidate has a minimum pt or eta
                Jet_energyRing_dR0_em_Jet_e = fjet.emEnergies()[0]/fjet.correctedJet("Uncorrected").energy();//remember to divide by jet energy
                Jet_energyRing_dR1_em_Jet_e = fjet.emEnergies()[1]/fjet.correctedJet("Uncorrected").energy();
                Jet_energyRing_dR2_em_Jet_e = fjet.emEnergies()[2]/fjet.correctedJet("Uncorrected").energy();
                Jet_energyRing_dR3_em_Jet_e = fjet.emEnergies()[3]/fjet.correctedJet("Uncorrected").energy();
                Jet_energyRing_dR4_em_Jet_e = fjet.emEnergies()[4]/fjet.correctedJet("Uncorrected").energy();
                Jet_energyRing_dR0_neut_Jet_e = fjet.neEnergies()[0]/fjet.correctedJet("Uncorrected").energy();
                Jet_energyRing_dR1_neut_Jet_e = fjet.neEnergies()[1]/fjet.correctedJet("Uncorrected").energy();
                Jet_energyRing_dR2_neut_Jet_e = fjet.neEnergies()[2]/fjet.correctedJet("Uncorrected").energy();
                Jet_energyRing_dR3_neut_Jet_e = fjet.neEnergies()[3]/fjet.correctedJet("Uncorrected").energy();
                Jet_energyRing_dR4_neut_Jet_e = fjet.neEnergies()[4]/fjet.correctedJet("Uncorrected").energy();
                Jet_energyRing_dR0_ch_Jet_e = fjet.chEnergies()[0]/fjet.correctedJet("Uncorrected").energy();
                Jet_energyRing_dR1_ch_Jet_e = fjet.chEnergies()[1]/fjet.correctedJet("Uncorrected").energy();
                Jet_energyRing_dR2_ch_Jet_e = fjet.chEnergies()[2]/fjet.correctedJet("Uncorrected").energy();
                Jet_energyRing_dR3_ch_Jet_e = fjet.chEnergies()[3]/fjet.correctedJet("Uncorrected").energy();
                Jet_energyRing_dR4_ch_Jet_e = fjet.chEnergies()[4]/fjet.correctedJet("Uncorrected").energy();
                Jet_energyRing_dR0_mu_Jet_e = fjet.muEnergies()[0]/fjet.correctedJet("Uncorrected").energy();
                Jet_energyRing_dR1_mu_Jet_e = fjet.muEnergies()[1]/fjet.correctedJet("Uncorrected").energy();
                Jet_energyRing_dR2_mu_Jet_e = fjet.muEnergies()[2]/fjet.correctedJet("Uncorrected").energy();
                Jet_energyRing_dR3_mu_Jet_e = fjet.muEnergies()[3]/fjet.correctedJet("Uncorrected").energy();
                Jet_energyRing_dR4_mu_Jet_e = fjet.muEnergies()[4]/fjet.correctedJet("Uncorrected").energy();
            }
            Jet_numDaughters_pt03 = fjet.userInt("numDaug03");
            
            std::vector<float> bRegNN(3,-999);
            

            if(debug){
                cout<<"Jet_pt :"<<Jet_pt <<endl;
                cout<<"Jet_eta :"<<Jet_eta <<endl;
                cout<<"rho :"<<rho <<endl;
                cout<<"Jet_mt :"<<Jet_mt <<endl;
                cout<<"Jet_leadTrackPt :"<<Jet_leadTrackPt <<endl;
                cout<<"Jet_leptonPtRel :"<<Jet_leptonPtRel <<endl;
                cout<<"Jet_leptonDeltaR :"<<Jet_leptonDeltaR <<endl;
                cout<<"Jet_neHEF :"<<Jet_neHEF <<endl;
                cout<<"Jet_neEmEF :"<<Jet_neEmEF <<endl;
                cout<<"Jet_vtxPt :"<<Jet_vtxPt <<endl;
                cout<<"Jet_vtxMass :"<<Jet_vtxMass <<endl;
                cout<<"Jet_vtx3dL :"<<Jet_vtx3dL <<endl;
                cout<<"Jet_vtxNtrk :"<<Jet_vtxNtrk <<endl;
                cout<<"Jet_vtx3deL :"<<Jet_vtx3deL <<endl;
                cout<<"Jet_numDaughters_pt03 :"<<Jet_numDaughters_pt03 <<endl;
                cout<<"Jet_energyRing_dR0_em_Jet_e :"<<Jet_energyRing_dR0_em_Jet_e <<endl;
                cout<<"Jet_energyRing_dR1_em_Jet_e :"<<Jet_energyRing_dR1_em_Jet_e <<endl;
                cout<<"Jet_energyRing_dR2_em_Jet_e :"<<Jet_energyRing_dR2_em_Jet_e <<endl;
                cout<<"Jet_energyRing_dR3_em_Jet_e :"<<Jet_energyRing_dR3_em_Jet_e <<endl;
                cout<<"Jet_energyRing_dR4_em_Jet_e :"<<Jet_energyRing_dR4_em_Jet_e <<endl;
                cout<<"Jet_energyRing_dR0_neut_Jet_e :"<<Jet_energyRing_dR0_neut_Jet_e <<endl;
                cout<<"Jet_energyRing_dR1_neut_Jet_e :"<<Jet_energyRing_dR1_neut_Jet_e <<endl;
                cout<<"Jet_energyRing_dR2_neut_Jet_e :"<<Jet_energyRing_dR2_neut_Jet_e <<endl;
                cout<<"Jet_energyRing_dR3_neut_Jet_e :"<<Jet_energyRing_dR3_neut_Jet_e <<endl;
                cout<<"Jet_energyRing_dR4_neut_Jet_e :"<<Jet_energyRing_dR4_neut_Jet_e <<endl;
                cout<<"Jet_energyRing_dR0_ch_Jet_e :"<<Jet_energyRing_dR0_ch_Jet_e <<endl;
                cout<<"Jet_energyRing_dR1_ch_Jet_e :"<<Jet_energyRing_dR1_ch_Jet_e <<endl;
                cout<<"Jet_energyRing_dR2_ch_Jet_e :"<<Jet_energyRing_dR2_ch_Jet_e <<endl;
                cout<<"Jet_energyRing_dR3_ch_Jet_e :"<<Jet_energyRing_dR3_ch_Jet_e <<endl;
                cout<<"Jet_energyRing_dR4_ch_Jet_e :"<<Jet_energyRing_dR4_ch_Jet_e <<endl;
                cout<<"Jet_energyRing_dR0_mu_Jet_e :"<<Jet_energyRing_dR0_mu_Jet_e <<endl;
                cout<<"Jet_energyRing_dR1_mu_Jet_e :"<<Jet_energyRing_dR1_mu_Jet_e <<endl;
                cout<<"Jet_energyRing_dR2_mu_Jet_e :"<<Jet_energyRing_dR2_mu_Jet_e <<endl;
                cout<<"Jet_energyRing_dR3_mu_Jet_e :"<<Jet_energyRing_dR3_mu_Jet_e <<endl;
                cout<<"Jet_energyRing_dR4_mu_Jet_e :"<<Jet_energyRing_dR4_mu_Jet_e <<endl;
                cout<<"Jet_chHEF:"<<Jet_chHEF<<endl;
                cout<<"Jet_chEmEF:"<<Jet_chEmEF<<endl;
                cout<<"Jet_leptonPtRelInv:"<<Jet_leptonPtRelInv<<endl;
                cout<<"isEle:"<<isEle<<endl;
                cout<<"isMu:"<<isMu<<endl;
                cout<<"isOther:"<<isOther<<endl;
                cout<<"Jet_mass:"<<Jet_mass<<endl;
                cout<<"Jet_withPtd:"<<Jet_withPtd<<endl;
            }
            SetNNVectorVar();
            bRegNN = EvaluateNN();
            NNvectorVar_.clear();

            float corr=1., res=0.2;
            if (fjet.pt()<20) {//b-jet regression should not be applied to low-pt jets since not trained. just set a correction of 1
                fjet.addUserFloat("bRegNNCorr", corr);
                fjet.addUserFloat("bRegNNResolution",res);
            }else{
                if ( (TMath::Finite(bRegNN[0])) && (TMath::Finite(bRegNN[1])) && (TMath::Finite(bRegNN[2])) )  {
                      corr = bRegNN[0]*y_std_+y_mean_;
                      res = 0.5*(bRegNN[2]-bRegNN[1])*y_std_;
                      if (!( (corr>0.1)&&(corr<2)&&(res>0.005)&&(res<0.9) )) {
                           corr = 1.;
                           res = 0.2;
                      } 
                } 
                fjet.addUserFloat("bRegNNCorr",corr) ;
                fjet.addUserFloat("bRegNNResolution",res);
            }

            TLorentzVector jetCorrected;
            jetCorrected.SetPtEtaPhiE(fjet.pt()*fjet.userFloat("bRegNNCorr"),fjet.eta(),fjet.phi(),fjet.p4().e()*fjet.userFloat("bRegNNCorr"));

            math::XYZTLorentzVector jetCorr;
            jetCorr.SetPxPyPzE(jetCorrected.Px(),jetCorrected.Py(),jetCorrected.Pz(),jetCorrected.E()); 

            fjet.setP4(jetCorr);//set the jet  with the regressed pt

            if (debug){
                cout<<"bRegNNCorr:"<<bRegNN[0]*y_std_+y_mean_<<endl;
                cout<<"bRegNNResolution:"<<0.5*(bRegNN[2]-bRegNN[1])*y_std_<<endl;
                std::cout<<"--------------------------------------------------------------"<<std::endl;
                std::cout<<"--------------------------------------------------------------"<<std::endl;
            }
            

            jetColl->push_back( fjet );
        }
        evt.put( std::move( jetColl ),bregtags[jet_col_idx] );
        }
    }
    
    void bRegressionProducer::InitJet(){
        Jet_pt = 0.;
        Jet_eta = 0.;
        rho = 0.;
        Jet_mt = 0.;
        Jet_leadTrackPt = 0.;
        Jet_leptonPtRel = 0.;
        Jet_leptonDeltaR = 0.;
        Jet_neHEF = 0.;
        Jet_neEmEF = 0.;
        Jet_vtxPt = 0.;
        Jet_vtxMass = 0.;
        Jet_vtx3dL = 0.;
        Jet_vtxNtrk = 0.;
        Jet_vtx3deL = 0.;
        Jet_numDaughters_pt03 = 0;
        Jet_energyRing_dR0_em_Jet_e = 0.;
        Jet_energyRing_dR1_em_Jet_e = 0.;
        Jet_energyRing_dR2_em_Jet_e = 0.;
        Jet_energyRing_dR3_em_Jet_e = 0.;
        Jet_energyRing_dR4_em_Jet_e = 0.;
        Jet_energyRing_dR0_neut_Jet_e = 0.;
        Jet_energyRing_dR1_neut_Jet_e = 0.;
        Jet_energyRing_dR2_neut_Jet_e = 0.;
        Jet_energyRing_dR3_neut_Jet_e = 0.;
        Jet_energyRing_dR4_neut_Jet_e = 0.;
        Jet_energyRing_dR0_ch_Jet_e = 0.;
        Jet_energyRing_dR1_ch_Jet_e = 0.;
        Jet_energyRing_dR2_ch_Jet_e = 0.;
        Jet_energyRing_dR3_ch_Jet_e = 0.;
        Jet_energyRing_dR4_ch_Jet_e = 0.;
        Jet_energyRing_dR0_mu_Jet_e = 0.;
        Jet_energyRing_dR1_mu_Jet_e = 0.;
        Jet_energyRing_dR2_mu_Jet_e = 0.;
        Jet_energyRing_dR3_mu_Jet_e = 0.;
        Jet_energyRing_dR4_mu_Jet_e = 0.;
        Jet_chHEF = 0.;//implement from here
        Jet_chEmEF = 0.;
        Jet_leptonPtRelInv = 0.;
        isEle = 0;
        isMu = 0;
        isOther = 0;
        Jet_mass = 0.;
        Jet_withPtd = 0.;


    }//end InitJet

    void bRegressionProducer::SetNNVectorVar(){

        NNvectorVar_.push_back(Jet_pt) ;//0
        NNvectorVar_.push_back(Jet_eta) ;
        NNvectorVar_.push_back(rho) ;
        NNvectorVar_.push_back(Jet_mt) ;
        NNvectorVar_.push_back(Jet_leadTrackPt) ;
        NNvectorVar_.push_back(Jet_leptonPtRel) ;//5
        NNvectorVar_.push_back(Jet_leptonDeltaR) ;
        NNvectorVar_.push_back(Jet_neHEF) ;
        NNvectorVar_.push_back(Jet_neEmEF) ;
        NNvectorVar_.push_back(Jet_vtxPt) ;
        NNvectorVar_.push_back(Jet_vtxMass) ;//10
        NNvectorVar_.push_back(Jet_vtx3dL) ;
        NNvectorVar_.push_back(Jet_vtxNtrk) ;
        NNvectorVar_.push_back(Jet_vtx3deL) ;
        NNvectorVar_.push_back(Jet_numDaughters_pt03) ;
        NNvectorVar_.push_back(Jet_energyRing_dR0_em_Jet_e) ;//15
        NNvectorVar_.push_back(Jet_energyRing_dR1_em_Jet_e) ;
        NNvectorVar_.push_back(Jet_energyRing_dR2_em_Jet_e) ;
        NNvectorVar_.push_back(Jet_energyRing_dR3_em_Jet_e) ;
        NNvectorVar_.push_back(Jet_energyRing_dR4_em_Jet_e) ;
        NNvectorVar_.push_back(Jet_energyRing_dR0_neut_Jet_e) ;//20
        NNvectorVar_.push_back(Jet_energyRing_dR1_neut_Jet_e) ;
        NNvectorVar_.push_back(Jet_energyRing_dR2_neut_Jet_e) ;
        NNvectorVar_.push_back(Jet_energyRing_dR3_neut_Jet_e) ;
        NNvectorVar_.push_back(Jet_energyRing_dR4_neut_Jet_e) ;
        NNvectorVar_.push_back(Jet_energyRing_dR0_ch_Jet_e) ;//25
        NNvectorVar_.push_back(Jet_energyRing_dR1_ch_Jet_e) ;
        NNvectorVar_.push_back(Jet_energyRing_dR2_ch_Jet_e) ;
        NNvectorVar_.push_back(Jet_energyRing_dR3_ch_Jet_e) ;
        NNvectorVar_.push_back(Jet_energyRing_dR4_ch_Jet_e) ;
        NNvectorVar_.push_back(Jet_energyRing_dR0_mu_Jet_e) ;//30
        NNvectorVar_.push_back(Jet_energyRing_dR1_mu_Jet_e) ;
        NNvectorVar_.push_back(Jet_energyRing_dR2_mu_Jet_e) ;
        NNvectorVar_.push_back(Jet_energyRing_dR3_mu_Jet_e) ;
        NNvectorVar_.push_back(Jet_energyRing_dR4_mu_Jet_e) ;
        NNvectorVar_.push_back(Jet_chHEF);//35
        NNvectorVar_.push_back(Jet_chEmEF);
        NNvectorVar_.push_back(Jet_leptonPtRelInv);
        NNvectorVar_.push_back(isEle);
        NNvectorVar_.push_back(isMu);
        NNvectorVar_.push_back(isOther);//40
        NNvectorVar_.push_back(Jet_mass);
        NNvectorVar_.push_back(Jet_withPtd);

        if(debug){
            for (unsigned int i=0;i<NNvectorVar_.size();i++)
                std::cout<<NNvectorVar_[i]<<" , ";
        }

    }
    
    std::vector<float> bRegressionProducer::EvaluateNN(){
        unsigned int shape=NNvectorVar_.size();
        std::vector<float> correction(3);//3 outputs, first value is mean and then other 2 quantiles
        tensorflow::Tensor input(tensorflow::DT_FLOAT, {1,shape});
        for (unsigned int i = 0; i < NNvectorVar_.size(); i++){
            input.matrix<float>()(0,i) =  float(NNvectorVar_[i]);
        }
        std::vector<tensorflow::Tensor> outputs;
        tensorflow::run(session, { { "ffwd_inp:0",input } }, { "ffwd_out/BiasAdd:0" }, &outputs);
        correction[0] = outputs[0].matrix<float>()(0, 0);
        correction[1] = outputs[0].matrix<float>()(0, 1);
        correction[2] = outputs[0].matrix<float>()(0, 2);
        return correction;
    }//end EvaluateNN
    
}



typedef flashgg::bRegressionProducer flashggbRegressionProducer;
DEFINE_FWK_MODULE( flashggbRegressionProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4


