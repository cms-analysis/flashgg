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
#include "flashgg/DataFormats/interface/GluGluHMVAResult.h"
#include "flashgg/DataFormats/interface/DiPhotonMVAResult.h"

#include "TMVA/Reader.h"
#include "TMath.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include <string>

#include "TLorentzVector.h"

using namespace std;
using namespace edm;

namespace flashgg {
    
    class GluGluHMVAProducer : public EDProducer
    {
        
    public:
        GluGluHMVAProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;
        
        std::vector<edm::EDGetTokenT<View<flashgg::Jet> > > tokenJets_;
        EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
        //EDGetTokenT<View<flashgg::Jet> > jetTokenDz_;
        std::vector<edm::InputTag> inputTagJets_;
        EDGetTokenT<View<DiPhotonMVAResult> > diPhotonMVAToken_;

        unique_ptr<TMVA::Reader>ggHMva_;
        FileInPath ggHMVAweightfile_;
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
        
        typedef std::vector<edm::Handle<edm::View<flashgg::Jet> > > JetCollectionVector;
        
        float n_rec_jets_;
        float dijet_Mjj_;
        float dijet_leadEta_;
        float dijet_subleadEta_;
        float dijet_subsubleadEta_;
        float dijet_leadJPt_;
        float dijet_subleadJPt_;
        float dijet_subsubleadJPt_;
        float dijet_leadPUMVA_;
        float dijet_subleadPUMVA_;
        float dijet_subsubleadPUMVA_;
        float dijet_leadDeltaPhi_;
        float dijet_subleadDeltaPhi_;
        float dijet_subsubleadDeltaPhi_;
        float dijet_leadDeltaEta_;
        float dijet_subleadDeltaEta_;
        float dijet_subsubleadDeltaEta_;

        // Input variables - diphoton
        float dipho_leadIDMVA_;
        float dipho_subleadIDMVA_;
        float dipho_lead_ptoM_;
        float dipho_sublead_ptoM_;
        float dipho_leadEta_;
        float dipho_subleadEta_;
        float CosPhi_; 
        float vtxprob_; 
        float sigmarv_; 
        float sigmawv_; 
        float diphopt_; 

    };
    
    GluGluHMVAProducer::GluGluHMVAProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        //jetTokenDz_( consumes<View<flashgg::Jet> >( iConfig.getParameter<InputTag>( "JetTag" ) ) ),
        inputTagJets_ ( iConfig.getParameter<std::vector<edm::InputTag> >( "inputTagJets" ) ),
        diPhotonMVAToken_( consumes<View<flashgg::DiPhotonMVAResult> >( iConfig.getParameter<InputTag> ( "DiPhotonMVATag" ) ) ),
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
        ggHMVAweightfile_ = iConfig.getParameter<edm::FileInPath>( "ggHMVAweightfile" );
        
        n_rec_jets_          = -1.;
        dijet_Mjj_          = -999.;
        dijet_leadEta_        = -999.;
        dijet_subleadEta_    = -999.;
        dijet_subsubleadEta_ = -999.;
        dijet_leadJPt_       = -999.;
        dijet_subleadJPt_    = -999.;
        dijet_subsubleadJPt_ = -999.;
        dijet_leadPUMVA_     = -999.;
        dijet_subleadPUMVA_  = -999.;
        dijet_subsubleadPUMVA_    = -999.;
        dijet_leadDeltaPhi_       = -999.;
        dijet_subleadDeltaPhi_    = -999.;
        dijet_subsubleadDeltaPhi_ = -999.;
        dijet_leadDeltaEta_       = -999.;
        dijet_subleadDeltaEta_    = -999.;
        dijet_subsubleadDeltaEta_ = -999.;

        dipho_leadIDMVA_     = -999.;
        dipho_subleadIDMVA_  = -999.;
        dipho_lead_ptoM_     = -999.;
        dipho_sublead_ptoM_  = -999.;
        dipho_leadEta_       = -999.;
        dipho_subleadEta_    = -999.;
        CosPhi_   = -999.; 
        vtxprob_  = -999.; 
        sigmarv_  = -999.; 
        sigmawv_  = -999.; 
        diphopt_  = -999.; 

        //if (_MVAMethod != ""){
        if (_MVAMethod == "Multi"){
            ggHMva_.reset( new TMVA::Reader( "!Color:Silent" ) );

            ggHMva_->AddVariable( "gghMVA_n_rec_jets"                  , &n_rec_jets_          );
            ggHMva_->AddVariable( "gghMVA_Mjj"                 , &dijet_Mjj_          );
            ggHMva_->AddVariable( "gghMVA_leadEta"             , &dijet_leadEta_        );
            ggHMva_->AddVariable( "gghMVA_subleadEta"          , &dijet_subleadEta_     );
            ggHMva_->AddVariable( "gghMVA_subsubleadEta"       , &dijet_subsubleadEta_  );
            ggHMva_->AddVariable( "gghMVA_leadJPt"             , &dijet_leadJPt_        );
            ggHMva_->AddVariable( "gghMVA_SubleadJPt"          , &dijet_subleadJPt_     );
            ggHMva_->AddVariable( "gghMVA_SubsubleadJPt"       , &dijet_subsubleadJPt_  );
            ggHMva_->AddVariable( "gghMVA_leadPUMVA"           , &dijet_leadPUMVA_        );
            ggHMva_->AddVariable( "gghMVA_subleadPUMVA"        , &dijet_subleadPUMVA_     );
            ggHMva_->AddVariable( "gghMVA_subsubleadPUMVA"     , &dijet_subsubleadPUMVA_  );
            ggHMva_->AddVariable( "gghMVA_leadDeltaPhi"        , &dijet_leadDeltaPhi_        );
            ggHMva_->AddVariable( "gghMVA_subleadDeltaPhi"     , &dijet_subleadDeltaPhi_     );
            ggHMva_->AddVariable( "gghMVA_subsubleadDeltaPhi"  , &dijet_subsubleadDeltaPhi_  );
            ggHMva_->AddVariable( "gghMVA_leadDeltaEta"        , &dijet_leadDeltaEta_        );
            ggHMva_->AddVariable( "gghMVA_subleadDeltaEta"     , &dijet_subleadDeltaEta_     );
            ggHMva_->AddVariable( "gghMVA_subsubleadDeltaEta"  , &dijet_subsubleadDeltaEta_     );
            ggHMva_->AddVariable( "dipho_leadIDMVA"     , &dipho_leadIDMVA_        );
            ggHMva_->AddVariable( "dipho_subleadIDMVA"  , &dipho_subleadIDMVA_     );
            ggHMva_->AddVariable( "dipho_lead_ptoM"     , &dipho_lead_ptoM_        );
            ggHMva_->AddVariable( "dipho_sublead_ptoM"  , &dipho_sublead_ptoM_     );
            ggHMva_->AddVariable( "dipho_leadEta"       , &dipho_leadEta_          );
            ggHMva_->AddVariable( "dipho_subleadEta"    , &dipho_subleadEta_       );
            ggHMva_->AddVariable( "CosPhi"              , &CosPhi_       );
            ggHMva_->AddVariable( "vtxprob"             , &vtxprob_       );
            ggHMva_->AddVariable( "sigmarv"             , &sigmarv_       );
            ggHMva_->AddVariable( "sigmawv"             , &sigmawv_       );
            ggHMva_->AddVariable( "diphopt"             , &diphopt_       );

            ggHMva_->BookMVA( _MVAMethod.c_str() , ggHMVAweightfile_.fullPath() );
        }
        for (unsigned i = 0 ; i < inputTagJets_.size() ; i++) {
            auto token = consumes<View<flashgg::Jet> >(inputTagJets_[i]);
            tokenJets_.push_back(token);
        }
        produces<vector<GluGluHMVAResult> >();
        
    }
    
    void GluGluHMVAProducer::produce( Event &evt, const EventSetup & )
    {
        Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
        evt.getByToken( diPhotonToken_, diPhotons );

        Handle<View<flashgg::DiPhotonMVAResult> > diPhotonMVAResult;
        evt.getByToken( diPhotonMVAToken_, diPhotonMVAResult );

        JetCollectionVector Jets( inputTagJets_.size() );
        for( size_t j = 0; j < inputTagJets_.size(); ++j ) {
            evt.getByToken( tokenJets_[j], Jets[j] );
        }
        
        std::unique_ptr<vector<GluGluHMVAResult> > ggH_results( new vector<GluGluHMVAResult> );
        for( unsigned int candIndex = 0; candIndex < diPhotons->size() ; candIndex++ ) {
            
            flashgg::GluGluHMVAResult mvares;
            
            n_rec_jets_          = -1.;
            dijet_Mjj_          = -999.;
            dijet_leadEta_        = -999.;
            dijet_subleadEta_    = -999.;
            dijet_subsubleadEta_ = -999.;
            dijet_leadJPt_       = -999.;
            dijet_subleadJPt_    = -999.;
            dijet_subsubleadJPt_ = -999.;
            dijet_leadPUMVA_     = -999.;
            dijet_subleadPUMVA_  = -999.;
            dijet_subsubleadPUMVA_    = -999.;
            dijet_leadDeltaPhi_       = -999.;
            dijet_subleadDeltaPhi_    = -999.;
            dijet_subsubleadDeltaPhi_ = -999.;
            dijet_leadDeltaEta_       = -999.;
            dijet_subleadDeltaEta_    = -999.;
            dijet_subsubleadDeltaEta_ = -999.;
     
            dipho_leadIDMVA_     = -999.;
            dipho_subleadIDMVA_  = -999.;
            dipho_lead_ptoM_     = -999.;
            dipho_sublead_ptoM_  = -999.;
            dipho_leadEta_       = -999.;
            dipho_subleadEta_    = -999.;
            CosPhi_   = -999.; 
            vtxprob_  = -999.; 
            sigmarv_  = -999.; 
            sigmawv_  = -999.; 
            diphopt_  = -999.; 
 
            // First find dijet by looking for highest-pt jets...
            std::pair <int, int>     dijet_indices( -1, -1 );
            std::pair <float, float> dijet_pts( -1., -1. );
            int jet_3_index = -1;
            int jet_3_pt    = -1;
                        
            float phi1 = diPhotons->ptrAt( candIndex )->leadingPhoton()->phi();
            float eta1 = diPhotons->ptrAt( candIndex )->leadingPhoton()->eta();
            float phi2 = diPhotons->ptrAt( candIndex )->subLeadingPhoton()->phi();
            float eta2 = diPhotons->ptrAt( candIndex )->subLeadingPhoton()->eta();
            
            bool hasValidVBFDiJet    = 0;
            bool hasValidVBFTriJet   = 0;
            
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
                    //std::cout << "VBFTagMVA::DEBUG  making the pujid --> "<< _pujid_wp_pt_bin_1.size() << std::endl;
                    bool pass=false;
                    for (UInt_t eta_bin=0; eta_bin < _pujid_wp_pt_bin_1.size(); eta_bin++ ){
                        //                        std::cout << inputTagJets_[0] 
                        //        << " eta-bin["<< eta_bin<< "] == " << eta_cuts_[eta_bin].first << "  :: "
                        //        << eta_cuts_[eta_bin].second
                        //        << " pt1: " << _pujid_wp_pt_bin_1[eta_bin]
                        //        << " pt2: " << _pujid_wp_pt_bin_2[eta_bin]
                        //        << " pt3: " << _pujid_wp_pt_bin_3[eta_bin]
                        //        << std::endl;
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
               
                // pt above 20 GeV?
                if(  jet->pt()  < 20 ) { continue; }

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
                    // also update jet 3 if current candidate has higher pt than exisiting candidate 
                    jet_3_index = jetLoop;
                    jet_3_pt    = jet->pt();
                }
                if( jet->pt() > 30.0 ) n_jets_count++;
                // if the jet's pt is neither higher than the lead jet or sublead jet, then forget it!
                if( dijet_indices.first != -1 && dijet_indices.second != -1 ) {hasValidVBFDiJet  = 1;}
                if( hasValidVBFDiJet          && jet_3_index != -1          ) {hasValidVBFTriJet = 1;}
            }

            n_rec_jets_ = n_jets_count;

            //Third jet deltaR cut and merge index finding
            int indexToMergeWithJ3(-1);
            //float thirdJetDRCut(1.8);

            //Getting the P4s
            std::vector<reco::Candidate::LorentzVector> diPhotonP4s(2);
            std::vector<reco::Candidate::LorentzVector> jetP4s;

            const flashgg::Photon *leadPhoton = diPhotons->ptrAt( candIndex )->leadingPhoton();
            const flashgg::Photon *subLeadPhoton = diPhotons->ptrAt( candIndex )->subLeadingPhoton();

            diPhotonP4s[0] = leadPhoton->p4(); 
            diPhotonP4s[1] = subLeadPhoton->p4(); 

            // get purely dipho variables from diphoMVAResult object
            dipho_lead_ptoM_        = diPhotonMVAResult->at(candIndex).leadptom;
            dipho_sublead_ptoM_     = diPhotonMVAResult->at(candIndex).subleadptom;
            dipho_leadEta_          = diPhotonMVAResult->at(candIndex).leadeta;
            dipho_subleadEta_       = diPhotonMVAResult->at(candIndex).subleadeta;
            CosPhi_                 = diPhotonMVAResult->at(candIndex).CosPhi;
            sigmawv_                = diPhotonMVAResult->at(candIndex).sigmawv;
            sigmarv_                = diPhotonMVAResult->at(candIndex).sigmarv;
            vtxprob_                = diPhotonMVAResult->at(candIndex).vtxprob;
            dipho_leadIDMVA_        = diPhotonMVAResult->at(candIndex).leadmva;
            dipho_subleadIDMVA_     = diPhotonMVAResult->at(candIndex).subleadmva;

            diphopt_                = (diPhotonP4s[0] + diPhotonP4s[1]).Pt();


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
                
                
                dijet_Mjj_           = (dijetP4s.first + dijetP4s.second).M();

                TLorentzVector diphoDijetSystem;
                diphoDijetSystem.SetPxPyPzE( (diPhotonP4s[0] + diPhotonP4s[1] + dijetP4s.first + dijetP4s.second).Px(), 
                                             (diPhotonP4s[0] + diPhotonP4s[1] + dijetP4s.first + dijetP4s.second).Py(),
                                             (diPhotonP4s[0] + diPhotonP4s[1] + dijetP4s.first + dijetP4s.second).Pz(),
                                             (diPhotonP4s[0] + diPhotonP4s[1] + dijetP4s.first + dijetP4s.second).E() );
                TLorentzVector diphoSystem; 
                diphoSystem.SetPxPyPzE( (diPhotonP4s[0] + diPhotonP4s[1]).Px(),
                                        (diPhotonP4s[0] + diPhotonP4s[1]).Py(),
                                        (diPhotonP4s[0] + diPhotonP4s[1]).Pz(),
                                        (diPhotonP4s[0] + diPhotonP4s[1]).E() );
                //std::cout << "Dipho system pt, eta, phi = " << diphoSystem.Pt() << ", " << diphoSystem.Eta() << ", " << diphoSystem.Phi() << std::endl;
                //std::cout << "DiphoDijet system pt, eta, phi = " << diphoDijetSystem.Pt() << ", " << diphoDijetSystem.Eta() << ", " << diphoDijetSystem.Phi() << std::endl;
                diphoSystem.Boost( -diphoDijetSystem.BoostVector() );

                mvares.leadJet           = dijetP4s.first;
                mvares.subleadJet        = dijetP4s.second;
                dijet_leadEta_           = dijetP4s.first.eta();
                dijet_subleadEta_        = dijetP4s.second.eta();
                dijet_leadJPt_           = dijetP4s.first.pt();
                dijet_subleadJPt_        = dijetP4s.second.pt();
                dijet_leadPUMVA_         = Jets[jetCollectionIndex]->ptrAt( dijet_indices.first )->puJetIdMVA();
                dijet_subleadPUMVA_    = Jets[jetCollectionIndex]->ptrAt( dijet_indices.second )->puJetIdMVA();
                dijet_leadDeltaPhi_      = deltaPhi( Jets[jetCollectionIndex]->ptrAt( dijet_indices.first )->phi(), (diPhotonP4s[0]+diPhotonP4s[1]).phi());
                dijet_subleadDeltaPhi_ = deltaPhi( Jets[jetCollectionIndex]->ptrAt( dijet_indices.second )->phi(), (diPhotonP4s[0]+diPhotonP4s[1]).phi());
                dijet_leadDeltaEta_        = Jets[jetCollectionIndex]->ptrAt( dijet_indices.first )->eta() - (diPhotonP4s[0]+diPhotonP4s[1]).eta();
                dijet_subleadDeltaEta_ = Jets[jetCollectionIndex]->ptrAt( dijet_indices.second )->eta() - (diPhotonP4s[0]+diPhotonP4s[1]).eta();
                
                mvares.leadJet_ptr    = Jets[jetCollectionIndex]->ptrAt( dijet_indices.first );
                mvares.subleadJet_ptr = Jets[jetCollectionIndex]->ptrAt( dijet_indices.second );

                if ( jet_3_index != -1 ) {
                  dijet_subsubleadPUMVA_    = Jets[jetCollectionIndex]->ptrAt( jet_3_index )->puJetIdMVA();
                  dijet_subsubleadDeltaPhi_ = deltaPhi( Jets[jetCollectionIndex]->ptrAt( jet_3_index )->phi(), (diPhotonP4s[0]+diPhotonP4s[1]).phi());
                  dijet_subsubleadDeltaEta_ = Jets[jetCollectionIndex]->ptrAt( jet_3_index )->eta() - (diPhotonP4s[0]+diPhotonP4s[1]).eta();
                  dijet_subsubleadJPt_    = Jets[jetCollectionIndex]->ptrAt( jet_3_index )->pt();
                  dijet_subsubleadEta_    = Jets[jetCollectionIndex]->ptrAt( jet_3_index )->eta();
                }

                //mvares.diphoton       = *diPhotons->ptrAt( candIndex );
            } else if (dijet_indices.first != -1) {
                dijet_leadEta_           = Jets[jetCollectionIndex]->ptrAt( dijet_indices.first )->eta();
                dijet_leadJPt_           = Jets[jetCollectionIndex]->ptrAt( dijet_indices.first )->pt();
                dijet_leadPUMVA_         = Jets[jetCollectionIndex]->ptrAt( dijet_indices.first )->puJetIdMVA();
                dijet_leadDeltaPhi_      = deltaPhi( Jets[jetCollectionIndex]->ptrAt( dijet_indices.first )->phi(), (diPhotonP4s[0]+diPhotonP4s[1]).phi());
                dijet_leadDeltaEta_      = Jets[jetCollectionIndex]->ptrAt( dijet_indices.first )->eta() - (diPhotonP4s[0]+diPhotonP4s[1]).eta();
                mvares.leadJet_ptr       = Jets[jetCollectionIndex]->ptrAt( dijet_indices.first );
                mvares.subleadJet_ptr    = edm::Ptr<flashgg::Jet>();
            } else {
                mvares.leadJet_ptr    = edm::Ptr<flashgg::Jet>();
                mvares.subleadJet_ptr = edm::Ptr<flashgg::Jet>();
            }
            
            if ( hasValidVBFDiJet && hasValidVBFTriJet){
                //mvares.subsubleadJet     = *Jets[jetCollectionIndex]->ptrAt( jet_3_index );
                mvares.subsubleadJet     = Jets[jetCollectionIndex]->ptrAt( jet_3_index )->p4();
                mvares.subsubleadJet_ptr = Jets[jetCollectionIndex]->ptrAt( jet_3_index );
                //mvares.hasValidVBFTriJet = 1;
            }else{
                mvares.subsubleadJet_ptr =  edm::Ptr<flashgg::Jet>();
            }
            
            //Evaluate ggH BDT add store probs
            if (_MVAMethod == "Multi") {
               mvares.ggHMVAResult_prob_0J_PTH_0_10 = ggHMva_->EvaluateMulticlass( 0, _MVAMethod.c_str() ); 
               // std::cout << "Inside GluGluH producer, class1 prob is:" << mvares.ggHMVAResult_prob_0J_PTH_0_10 << endl;
               mvares.ggHMVAResult_prob_0J_PTH_GT10 = ggHMva_->EvaluateMulticlass( 1, _MVAMethod.c_str() ); 
               mvares.ggHMVAResult_prob_1J_PTH_0_60 = ggHMva_->EvaluateMulticlass( 2, _MVAMethod.c_str() ); 
               mvares.ggHMVAResult_prob_1J_PTH_60_120 = ggHMva_->EvaluateMulticlass( 3, _MVAMethod.c_str() ); 
               mvares.ggHMVAResult_prob_1J_PTH_120_200 = ggHMva_->EvaluateMulticlass( 4, _MVAMethod.c_str() ); 
               mvares.ggHMVAResult_prob_GE2J_MJJ_0_350_PTH_0_60 = ggHMva_->EvaluateMulticlass( 5, _MVAMethod.c_str() ); 
               mvares.ggHMVAResult_prob_GE2J_MJJ_0_350_PTH_60_120 = ggHMva_->EvaluateMulticlass( 6, _MVAMethod.c_str() ); 
               mvares.ggHMVAResult_prob_GE2J_MJJ_0_350_PTH_120_200 = ggHMva_->EvaluateMulticlass( 7, _MVAMethod.c_str() ); 
               mvares.ggHMVAResult_prob_PTH_GT200 = ggHMva_->EvaluateMulticlass( 8, _MVAMethod.c_str() ); 
            }
            mvares.n_rec_jets = n_rec_jets_;            
            mvares.dijet_Mjj =  dijet_Mjj_;
            mvares.dijet_leadEta     = dijet_leadEta_;
            mvares.dijet_subleadEta  = dijet_subleadEta_;
            mvares.dijet_subsubleadEta =  dijet_subsubleadEta_;
            mvares.dijet_leadJPt     = dijet_leadJPt_;
            mvares.dijet_SubleadJPt  = dijet_subleadJPt_;
            mvares.dijet_SubsubleadJPt =  dijet_subsubleadJPt_;
            mvares.dijet_leadPUMVA   = dijet_leadPUMVA_;
            mvares.dijet_subleadPUMVA = dijet_subleadPUMVA_;
            mvares.dijet_subsubleadPUMVA =  dijet_subsubleadPUMVA_;
            mvares.dijet_leadDeltaPhi  = dijet_leadDeltaPhi_;
            mvares.dijet_subleadDeltaPhi = dijet_subleadDeltaPhi_;
            mvares.dijet_subsubleadDeltaPhi =  dijet_subsubleadDeltaPhi_;
            mvares.dijet_leadDeltaEta  = dijet_leadDeltaEta_;
            mvares.dijet_subleadDeltaEta =  dijet_subleadDeltaEta_;
            mvares.dijet_subsubleadDeltaEta = dijet_subsubleadDeltaEta_;

            mvares.dipho_lead_ptoM = dipho_lead_ptoM_;
            mvares.dipho_sublead_ptoM = dipho_sublead_ptoM_; 
            mvares.dipho_leadEta =  dipho_leadEta_;
            mvares.dipho_subleadEta =  dipho_subleadEta_;       
            mvares.diphopt =  diphopt_;       

            ggH_results->push_back( mvares );
        }
        evt.put( std::move( ggH_results ) );
    }
}

typedef flashgg::GluGluHMVAProducer FlashggGluGluHMVAProducer;
DEFINE_FWK_MODULE( FlashggGluGluHMVAProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

