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
#include "flashgg/DataFormats/interface/VBFMVAResult.h"

#include "TMVA/Reader.h"
#include "TMath.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include <string>

using namespace std;
using namespace edm;

namespace flashgg {
    
    class VBFMVAProducer : public EDProducer
    {
        
    public:
        VBFMVAProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;
        
        std::vector<edm::EDGetTokenT<View<flashgg::Jet> > > tokenJets_;
        EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
        //EDGetTokenT<View<flashgg::Jet> > jetTokenDz_;
        std::vector<edm::InputTag> inputTagJets_;

        unique_ptr<TMVA::Reader>VbfMva_;
        FileInPath vbfMVAweightfile_;
        string     _MVAMethod;
        bool       _usePuJetID;
        bool       _useJetID;
        bool       _merge3rdJet;
        double     _thirdJetDRCut;
        double     _rmsforwardCut;
        string     _JetIDLevel;
        double     _minDijetMinv;
        double     _drJetPhoton;
        
        std::vector<double> _pujid_wp_pt_bin_1;
        std::vector<double> _pujid_wp_pt_bin_2;
        std::vector<double> _pujid_wp_pt_bin_3;
        
        typedef std::vector<edm::Handle<edm::View<flashgg::Jet> > > JetCollectionVector;
        
        float dijet_leadEta_   ;
        float dijet_subleadEta_;
        float dijet_abs_dEta_;
        float dijet_LeadJPt_ ;
        float dijet_pt_ ;
        float dijet_SubJPt_  ;
        float dijet_Zep_     ;
        float dijet_dphi_trunc_;
        float dijet_dphi_;
        float dijet_dipho_dphi_;
        float dijet_Mjj_   ;
        float dijet_minDRJetPho_ ;
        float dijet_centrality_gg_;
        float dijet_centrality_j3_;
        float dijet_centrality_g_ ;
        float dijet_dy_    ;
        float dijet_leady_    ;
        float dijet_subleady_ ;
        float dijet_dipho_pt_ ;
        
        float dipho_PToM_  ;
        float leadPho_PToM_;
        float sublPho_PToM_;

    };
    
    VBFMVAProducer::VBFMVAProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        //jetTokenDz_( consumes<View<flashgg::Jet> >( iConfig.getParameter<InputTag>( "JetTag" ) ) ),
        inputTagJets_ ( iConfig.getParameter<std::vector<edm::InputTag> >( "inputTagJets" ) ),
        _MVAMethod    ( iConfig.getParameter<string> ( "MVAMethod"    ) ),
        _usePuJetID   ( iConfig.getParameter<bool>   ( "UsePuJetID"   ) ),
        _useJetID     ( iConfig.getParameter<bool>   ( "UseJetID"     ) ),
        _merge3rdJet  ( iConfig.getParameter<bool>   ( "merge3rdJet"  ) ),
        _thirdJetDRCut( iConfig.getParameter<double> ( "thirdJetDRCut") ),
        _rmsforwardCut( iConfig.getParameter<double> ( "rmsforwardCut") ),
        _JetIDLevel   ( iConfig.getParameter<string> ( "JetIDLevel"   ) ),
        _minDijetMinv ( iConfig.getParameter<double> ( "MinDijetMinv" ) ),
        _drJetPhoton  ( iConfig.getParameter<double> ( "DrJetPhoton"  ) ),
        _pujid_wp_pt_bin_1  ( iConfig.getParameter<std::vector<double> > ( "pujidWpPtBin1" ) ),
        _pujid_wp_pt_bin_2  ( iConfig.getParameter<std::vector<double> > ( "pujidWpPtBin2" ) )
    {
        vbfMVAweightfile_ = iConfig.getParameter<edm::FileInPath>( "vbfMVAweightfile" );
        
        dijet_leadEta_    = -999.;
        dijet_subleadEta_ = -999.;
        dijet_abs_dEta_   = -999.;
        dijet_LeadJPt_    = -999.;
        dijet_pt_         = -999.;
        dijet_SubJPt_     = -999.;
        dijet_Zep_        = -999.;
        dijet_dphi_trunc_ = -999.;
        dijet_dipho_dphi_ = -999.;
        dijet_dphi_       = -999.;
        dijet_Mjj_        = -999.;
        dijet_dy_         = -999.;
        dipho_PToM_       = -999.;
        leadPho_PToM_     = -999.;
        sublPho_PToM_     = -999.;
        dijet_minDRJetPho_= -999.;
        dijet_centrality_gg_ = -999.;
        dijet_centrality_j3_ = -999.;
        dijet_centrality_g_  = -999.;
        dijet_dipho_pt_   = -999.;
        dijet_leady_      = -999.;
        dijet_subleady_   = -999.;
        
        if (_MVAMethod == "BDTG"){
            VbfMva_.reset( new TMVA::Reader( "!Color:Silent" ) );
            // Run 1 legacy variables
            /*
            VbfMva_->AddVariable( "dijet_LeadJPt"     , &dijet_LeadJPt_    );
            VbfMva_->AddVariable( "dijet_SubJPt"      , &dijet_SubJPt_     );
            VbfMva_->AddVariable( "dijet_abs_dEta"    , &dijet_abs_dEta_   );
            VbfMva_->AddVariable( "dijet_Mjj"         , &dijet_Mjj_        );
            VbfMva_->AddVariable( "dijet_Zep"         , &dijet_Zep_        );
            VbfMva_->AddVariable( "dijet_dPhi_trunc"  , &dijet_dphi_trunc_ );
            VbfMva_->AddVariable( "leadPho_PToM"      , &leadPho_PToM_);
            VbfMva_->AddVariable( "sublPho_PToM"      , &sublPho_PToM_);
            */
            
            // Moriond17 variables
            VbfMva_->AddVariable( "dijet_LeadJPt"          , &dijet_LeadJPt_       );
            VbfMva_->AddVariable( "dijet_SubJPt"           , &dijet_SubJPt_        );
            VbfMva_->AddVariable( "dijet_abs_dEta"         , &dijet_abs_dEta_      );
            VbfMva_->AddVariable( "dijet_Mjj"              , &dijet_Mjj_           );
            VbfMva_->AddVariable( "dijet_centrality_gg"    , &dijet_centrality_gg_ );
            VbfMva_->AddVariable( "dijet_dipho_dphi_trunc" , &dijet_dphi_trunc_    );
            VbfMva_->AddVariable( "dijet_dphi"             , &dijet_dphi_          );
            VbfMva_->AddVariable( "dijet_minDRJetPho"      , &dijet_minDRJetPho_   );
            VbfMva_->AddVariable( "leadPho_PToM"           , &leadPho_PToM_        );
            VbfMva_->AddVariable( "sublPho_PToM"           , &sublPho_PToM_        );
            
            VbfMva_->BookMVA( _MVAMethod.c_str() , vbfMVAweightfile_.fullPath() );
        }
        else if (_MVAMethod == "Multi"){
            VbfMva_.reset( new TMVA::Reader( "!Color:Silent" ) );
            VbfMva_->AddVariable( "dipho_lead_ptoM"        , &leadPho_PToM_        );
            VbfMva_->AddVariable( "dipho_sublead_ptoM"     , &sublPho_PToM_        );
            VbfMva_->AddVariable( "dijet_LeadJPt"          , &dijet_LeadJPt_       );
            VbfMva_->AddVariable( "dijet_SubJPt"           , &dijet_SubJPt_        );
            VbfMva_->AddVariable( "dijet_abs_dEta"         , &dijet_abs_dEta_      );
            VbfMva_->AddVariable( "dijet_Mjj"              , &dijet_Mjj_           );
            VbfMva_->AddVariable( "dijet_centrality"       , &dijet_centrality_gg_ );
            VbfMva_->AddVariable( "dijet_dphi"             , &dijet_dphi_          );
            VbfMva_->AddVariable( "dijet_minDRJetPho"      , &dijet_minDRJetPho_   );
            VbfMva_->AddVariable( "dijet_dipho_dphi_trunc" , &dijet_dphi_trunc_    );
            
            VbfMva_->BookMVA( _MVAMethod.c_str() , vbfMVAweightfile_.fullPath() );
        }
        for (unsigned i = 0 ; i < inputTagJets_.size() ; i++) {
            auto token = consumes<View<flashgg::Jet> >(inputTagJets_[i]);
            tokenJets_.push_back(token);
        }
        produces<vector<VBFMVAResult> >();
        
    }
    
    void VBFMVAProducer::produce( Event &evt, const EventSetup & )
    {
        Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
        evt.getByToken( diPhotonToken_, diPhotons );
        
        JetCollectionVector Jets( inputTagJets_.size() );
        for( size_t j = 0; j < inputTagJets_.size(); ++j ) {
            evt.getByToken( tokenJets_[j], Jets[j] );
        }
        
        std::unique_ptr<vector<VBFMVAResult> > vbf_results( new vector<VBFMVAResult> );
        for( unsigned int candIndex = 0; candIndex < diPhotons->size() ; candIndex++ ) {
            
            flashgg::VBFMVAResult mvares;
            
            dijet_leadEta_    = -999.;
            dijet_subleadEta_ = -999.;
            dijet_abs_dEta_   = -999.;
            dijet_LeadJPt_    = -999.;
            dijet_pt_    = -999.;
            dijet_SubJPt_     = -999.;
            dijet_Zep_        = -999.;
            dijet_dphi_trunc_ = -999.;
            dijet_dipho_dphi_ = -999.;
            dijet_dphi_       = -999.;
            dijet_Mjj_        = -999.;
            dijet_dy_         = -999.;
            dijet_minDRJetPho_= -999.;
            dijet_dipho_pt_   = -999.;
            dijet_leady_      = -999.;
            dijet_subleady_   = -999.;
            dijet_centrality_gg_ = -999.;
            dijet_centrality_j3_ = -999.;
            dijet_centrality_g_  = -999.;
            dipho_PToM_       = -999.;
            leadPho_PToM_     = -999.;
            sublPho_PToM_     = -999.;
           
 
            // First find dijet by looking for highest-pt jets...
            std::pair <int, int>     dijet_indices( -1, -1 );
            std::pair <float, float> dijet_pts( -1., -1. );
            int jet_3_index = -1;
            int jet_3_pt    = -1;
                        
            float phi1 = diPhotons->ptrAt( candIndex )->leadingPhoton()->phi();
            float eta1 = diPhotons->ptrAt( candIndex )->leadingPhoton()->eta();
            float phi2 = diPhotons->ptrAt( candIndex )->subLeadingPhoton()->phi();
            float eta2 = diPhotons->ptrAt( candIndex )->subLeadingPhoton()->eta();
            
            bool hasValidVBFDiJet  = 0;
            bool hasValidVBFTriJet = 0;
            
            int  n_jets_count = 0;
            // take the jets corresponding to the diphoton candidate
            unsigned int jetCollectionIndex = diPhotons->ptrAt( candIndex )->jetCollectionIndex();
                        
            for( UInt_t jetLoop = 0; jetLoop < Jets[jetCollectionIndex]->size() ; jetLoop++ ) {
                Ptr<flashgg::Jet> jet  = Jets[jetCollectionIndex]->ptrAt( jetLoop );
                //if (jet->puJetId(diPhotons[candIndex]) <  PuIDCutoff) {continue;}
                if( _usePuJetID && !jet->passesPuJetId(diPhotons->ptrAt( candIndex ))){ continue;}
                if( _useJetID ){
                    if( _JetIDLevel == "Loose" && !jet->passesJetID  ( flashgg::Tight2017 ) ) continue;
                    if( _JetIDLevel == "Tight" && !jet->passesJetID  ( flashgg::Tight ) ) continue;
                    if( _JetIDLevel == "Tight2017" && !jet->passesJetID (flashgg::Tight2017 ) ) continue;
                    if( _JetIDLevel == "Tight2018" && !jet->passesJetID (flashgg::Tight2018 ) ) continue;
                }
                // rms cuts over 2.5 
                if( fabs( jet->eta() ) > 2.5 && jet->rms() > _rmsforwardCut ){ 
                    //std::cout << "("<< jet->eta()<< ")("<< jet->rms() <<").. jet rejected ::" << std::endl;
                    continue; 
                }
                // new PUJID for differents pt bins
                std::vector<std::pair<double,double> > eta_cuts_(4);
                eta_cuts_[0] = std::make_pair (0    ,2.50 );
                eta_cuts_[1] = std::make_pair (2.50 ,2.75 );
                eta_cuts_[2] = std::make_pair (2.75 ,3.00 );
                eta_cuts_[3] = std::make_pair (3.00 ,10);

                
                if ( (!_pujid_wp_pt_bin_1.empty())  &&
                     (!_pujid_wp_pt_bin_2.empty())  ){
                    bool pass=false;
                    for (UInt_t eta_bin=0; eta_bin < _pujid_wp_pt_bin_1.size(); eta_bin++ ){
                        if ( fabs( jet->eta() ) >  eta_cuts_[eta_bin].first &&
                             fabs( jet->eta() ) <= eta_cuts_[eta_bin].second){
                            if ( jet->pt() >  20 &&
                                 jet->pt() <= 30 && jet->puJetIdMVA() > _pujid_wp_pt_bin_1[eta_bin] )
                                pass=true;
                            if ( jet->pt() >  30 &&
                                 jet->pt() <= 50 && jet->puJetIdMVA() > _pujid_wp_pt_bin_2[eta_bin] )
                                pass=true;
                            if (jet->pt() > 50) pass = true;
                        }
                    }
                    //                    std::cout << inputTagJets_[0] << " pt="<< jet->pt() << " :eta: "<< jet->eta() << " :mva: "<< jet->puJetIdMVA() << "  pass == " << pass << std::endl;
                    if (!pass) continue;
                }
                // within eta 4.7?
                if( fabs( jet->eta() ) > 4.7 ) { continue; }

                // close to lead photon?
                float dPhi = deltaPhi( jet->phi(), phi1 );
                float dEta = jet->eta() - eta1;
                if( sqrt( dPhi * dPhi + dEta * dEta ) < _drJetPhoton ) { continue; }
                
                // close to sublead photon?
                dPhi = deltaPhi( jet->phi(), phi2 );
                dEta = jet->eta() - eta2;
                if( sqrt( dPhi * dPhi + dEta * dEta ) < _drJetPhoton ) { continue; }
                
                if( jet->pt() > dijet_pts.first ) {
                    // if pt of this jet is higher than the one currently in lead position
                    // then shift back lead jet into sublead position...
                    dijet_indices.second = dijet_indices.first;
                    dijet_pts.second     = dijet_pts.first;
                    // .. and put the new jet as the lead jet.
                    dijet_indices.first = jetLoop;
                    dijet_pts.first     = jet->pt();
                    
                    // trijet indicies
                    //jet_3_index = dijet_indices.second;
                    //jet_3_pt    = dijet_pts.second;
                } else if( jet->pt() > dijet_pts.second ) { 
                    // for the 3rd jets
                    jet_3_index = dijet_indices.second;
                    jet_3_pt    = dijet_pts.second;
                    
                    // this condition is added here to force to have the leading 
                    // and subleading jets in two different hemispheres 
                    // if the jet's pt isn't as high as the lead jet's but i higher 
                    // than the sublead jet's The replace the sublead jet by this new jet.
                    dijet_indices.second = jetLoop;
                    dijet_pts.second     = jet->pt();
                    
                }else if( jet->pt() > jet_3_pt ){//&& dijet_indices.first != int(jetLoop) && dijet_indices.second != int(jetLoop)){
                    jet_3_index = jetLoop;
                    jet_3_pt    = jet->pt();
                }
                if( jet->pt() > 30.0 ) n_jets_count++;
                // if the jet's pt is neither higher than the lead jet or sublead jet, then forget it!
                if( dijet_indices.first != -1 && dijet_indices.second != -1 ) {hasValidVBFDiJet  = 1;}
                if( hasValidVBFDiJet          && jet_3_index != -1          ) {hasValidVBFTriJet = 1;}
            }

            //Third jet deltaR cut and merge index finding
            int indexToMergeWithJ3(-1);
            //float thirdJetDRCut(1.8);

            //Getting the P4s
            std::vector<reco::Candidate::LorentzVector> diPhotonP4s(2);
            std::vector<reco::Candidate::LorentzVector> jetP4s;

            diPhotonP4s[0] = diPhotons->ptrAt( candIndex )->leadingPhoton()->p4(); 
            diPhotonP4s[1] = diPhotons->ptrAt( candIndex )->subLeadingPhoton()->p4(); 
            if ( hasValidVBFDiJet ) {
                jetP4s.push_back(Jets[jetCollectionIndex]->ptrAt(dijet_indices.first)->p4());
                jetP4s.push_back(Jets[jetCollectionIndex]->ptrAt(dijet_indices.second)->p4());
            }
            if ( hasValidVBFTriJet ) {
                
                jetP4s.push_back(Jets[jetCollectionIndex]->ptrAt(jet_3_index)->p4());

                float dR_13 = deltaR(jetP4s[0].eta(),jetP4s[0].phi(),jetP4s[2].eta(),jetP4s[2].phi());
                float dR_23 = deltaR(jetP4s[1].eta(),jetP4s[1].phi(),jetP4s[2].eta(),jetP4s[2].phi());
                
                if (dR_13 < dR_23) {
                    indexToMergeWithJ3 = dR_13 < _thirdJetDRCut ? 0 : -1;
                }else{
                    indexToMergeWithJ3 = dR_23 < _thirdJetDRCut ? 1 : -1;
                }

                if (dR_13 > _thirdJetDRCut && dR_23 > _thirdJetDRCut) {
                    hasValidVBFTriJet = 0;
                }
                
                //std::cout << "Third jet merge info:" << std::endl;
                //std::cout << setw(12) << dR_13 << setw(12) << dR_23 << setw(12) << indexToMergeWithJ3 << std::endl;
            }
           
            if( hasValidVBFDiJet ) {
                std::pair<reco::Candidate::LorentzVector,reco::Candidate::LorentzVector> dijetP4s;
                
                //std ::cout << "-->before  jet_1 pt:" << jetP4s[0].pt() << std::endl;
                //std ::cout << "-->before  jet_2 pt:" << jetP4s[1].pt() << std::endl;
                if (indexToMergeWithJ3 != -1 && _merge3rdJet ) {
                    //std::cout << "Hey I am merging jets : " << indexToMergeWithJ3+1 << " with jet 3" << std::endl;    
                    dijetP4s.first  = jetP4s[ indexToMergeWithJ3 == 0 ? 1 : 0 ];
                    dijetP4s.second = jetP4s[ indexToMergeWithJ3 ] + jetP4s[2];                 
                    if (dijetP4s.second.pt() > dijetP4s.first.pt()) { std::swap(dijetP4s.first, dijetP4s.second);}
                }else{
                    dijetP4s.first  = jetP4s[0];
                    dijetP4s.second = jetP4s[1];
                }
                
                //std ::cout << "-->after  jet_1 pt:" << dijetP4s.first.pt()  << std::endl;
                //std ::cout << "-->after  jet_2 pt:" << dijetP4s.second.pt() << std::endl;
                
                dijet_leadEta_    = dijetP4s.first.eta();
                dijet_subleadEta_ = dijetP4s.second.eta();
                
                dijet_abs_dEta_   = fabs( dijetP4s.first.eta() - dijetP4s.second.eta());
                
                dijet_LeadJPt_    = dijetP4s.first.pt();
                dijet_pt_    = (dijetP4s.first + dijetP4s.second).pt();
                dijet_SubJPt_     = dijetP4s.second.pt();
                
                dijet_dipho_dphi_ = fabs(reco::deltaPhi((dijetP4s.first + dijetP4s.second).phi(),(diPhotonP4s[0] + diPhotonP4s[1]).phi()));
                dijet_dphi_trunc_ = std::min((float) dijet_dipho_dphi_, (float) 2.9416);
                dijet_dphi_       = fabs(reco::deltaPhi(dijetP4s.first.phi(),dijetP4s.second.phi()));
                
                dijet_dipho_pt_   = (dijetP4s.first + dijetP4s.second + diPhotonP4s[0] + diPhotonP4s[1]).pt(); 
                
                dijet_Zep_           = fabs( (diPhotonP4s[0]+diPhotonP4s[1]).eta() - 0.5*(dijetP4s.first.eta()+dijetP4s.second.eta()) );
                dijet_centrality_gg_ = exp(-4*pow(dijet_Zep_/dijet_abs_dEta_,2));
                dijet_centrality_g_  = exp(-4*pow(fabs( diPhotonP4s[0].eta() - 0.5*(dijetP4s.first.eta()+dijetP4s.second.eta()) )/dijet_leadEta_,2));
                dijet_Mjj_           = (dijetP4s.first + dijetP4s.second).M();

                dipho_PToM_       = (diPhotonP4s[0] + diPhotonP4s[1]).Pt()/(diPhotonP4s[0] + diPhotonP4s[1]).M();
                leadPho_PToM_     = diPhotonP4s[0].pt()/(diPhotonP4s[0] + diPhotonP4s[1]).M();
                sublPho_PToM_     = diPhotonP4s[1].pt()/(diPhotonP4s[0] + diPhotonP4s[1]).M();
                
                dijet_minDRJetPho_ = std::min( std::min(deltaR( dijetP4s.first ,diPhotonP4s[0] ),
                                                        deltaR( dijetP4s.second,diPhotonP4s[0] )),
                                               std::min(deltaR( dijetP4s.first ,diPhotonP4s[1] ),
                                                        deltaR( dijetP4s.second,diPhotonP4s[1] ))        
                                              );
                
                dijet_dy_         = fabs( (dijetP4s.first + dijetP4s.second).Rapidity() - (diPhotonP4s[0] + diPhotonP4s[1]).Rapidity() );
                
                dijet_leady_      = dijetP4s.first.Rapidity();
                
                dijet_subleady_   = dijetP4s.second.Rapidity();
                
                //mvares.leadJet    = *Jets[jetCollectionIndex]->ptrAt( dijet_indices.first );
                //mvares.subleadJet = *Jets[jetCollectionIndex]->ptrAt( dijet_indices.second );
                mvares.leadJet        = dijetP4s.first;
                mvares.subleadJet     = dijetP4s.second;
                
                mvares.leadJet_ptr    = Jets[jetCollectionIndex]->ptrAt( dijet_indices.first );
                mvares.subleadJet_ptr = Jets[jetCollectionIndex]->ptrAt( dijet_indices.second );
                //mvares.diphoton       = *diPhotons->ptrAt( candIndex );
            } else if( dijet_indices.first != -1 ) {
                mvares.leadJet_ptr     = Jets[jetCollectionIndex]->ptrAt( dijet_indices.first );
                mvares.subleadJet_ptr  = edm::Ptr<flashgg::Jet>();
                dijet_leadEta_         = Jets[jetCollectionIndex]->ptrAt( dijet_indices.first )->eta();
                dijet_LeadJPt_         = Jets[jetCollectionIndex]->ptrAt( dijet_indices.first )->pt();
            } else {
                mvares.leadJet_ptr    = edm::Ptr<flashgg::Jet>();
                mvares.subleadJet_ptr = edm::Ptr<flashgg::Jet>();
            }
            
            if ( hasValidVBFDiJet && hasValidVBFTriJet){
                //mvares.subsubleadJet     = *Jets[jetCollectionIndex]->ptrAt( jet_3_index );
                mvares.subsubleadJet     = Jets[jetCollectionIndex]->ptrAt( jet_3_index )->p4();
                mvares.subsubleadJet_ptr = Jets[jetCollectionIndex]->ptrAt( jet_3_index );
                mvares.hasValidVBFTriJet = 1;
            }else{
                mvares.subsubleadJet_ptr =  edm::Ptr<flashgg::Jet>();
            }
            

            if (_MVAMethod == "BDTG") {
                mvares.vbfMvaResult_value = VbfMva_->EvaluateMVA( _MVAMethod.c_str() );
                //mvares.vbfMvaResult_value = VbfMva_->GetProba( _MVAMethod.c_str() );
            }
            else if (_MVAMethod == "Multi") {
                mvares.vbfMvaResult_prob_bkg = VbfMva_->EvaluateMulticlass( 0, _MVAMethod.c_str() );
                mvares.vbfMvaResult_prob_ggH = VbfMva_->EvaluateMulticlass( 1, _MVAMethod.c_str() );
                mvares.vbfMvaResult_prob_VBF = VbfMva_->EvaluateMulticlass( 2, _MVAMethod.c_str() );
            }
            
            mvares.dijet_leadEta     = dijet_leadEta_ ;
            mvares.dijet_subleadEta  = dijet_subleadEta_ ;
            mvares.dijet_abs_dEta    = dijet_abs_dEta_ ;
            mvares.dijet_LeadJPt     = dijet_LeadJPt_ ;
            mvares.dijet_pt          = dijet_pt_ ;
            mvares.dijet_SubJPt      = dijet_SubJPt_ ;
            mvares.dijet_Zep         = dijet_Zep_ ;
            mvares.dijet_centrality_gg = dijet_centrality_gg_ ;
            mvares.dijet_centrality_g  = dijet_centrality_g_ ;
            mvares.dijet_dphi_trunc  = dijet_dphi_trunc_ ;
            mvares.dijet_dphi        = dijet_dphi_ ;
            mvares.dijet_dipho_dphi  = dijet_dipho_dphi_ ;
            mvares.dijet_Mjj         = dijet_Mjj_ ;
            mvares.n_rec_jets        = n_jets_count;
            mvares.dipho_PToM        = dipho_PToM_ ;
            mvares.sublPho_PToM      = sublPho_PToM_ ;
            mvares.leadPho_PToM      = leadPho_PToM_ ;
            mvares.dijet_minDRJetPho = dijet_minDRJetPho_;
            mvares.dijet_dy          = dijet_dy_;
            mvares.dijet_dipho_pt    = dijet_dipho_pt_ ;
            mvares.dijet_leady       = dijet_leady_   ;
            mvares.dijet_subleady    = dijet_subleady_;
            
            vbf_results->push_back( mvares );
        }
        evt.put( std::move( vbf_results ) );
    }
}

typedef flashgg::VBFMVAProducer FlashggVBFMVAProducer;
DEFINE_FWK_MODULE( FlashggVBFMVAProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

