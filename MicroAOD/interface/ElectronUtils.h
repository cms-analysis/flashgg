#ifndef FLASHgg_ElectronUtils_h
#define FLASHgg_ElectronUtils_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Ptr.h"
//#include "DataFormats/Common/interface/PtrVector.h"
#include "DataFormats/Common/interface/RefToPtr.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "flashgg/DataFormats/interface/Photon.h"
#include "flashgg/DataFormats/interface/VertexCandidateMap.h"

#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"

#include "FWCore/Common/interface/EventBase.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
/// class EcalRecHitCollection;
class CaloTopology;

#include <TMVA/Reader.h>
#include <TLorentzVector.h>

using namespace edm;

namespace flashgg {
    
    
    class ElectronUtils
    {
        
    public:
        
        ElectronUtils( const edm::ParameterSet &cfg){
            if( cfg.exists( "ElectronPtThreshold" )  ){
                ElectronPtThreshold_ = ( cfg.getParameter<double>( "ElectronPtThreshold" ) );
            }
            if( cfg.exists( "ElectronEtaCuts" )  ){
                ElectronEtaCuts_ = ( cfg.getParameter<vector<double >>( "ElectronEtaCuts" ) );
            }
            if( cfg.exists( "useMVARecipe" )  ){
                useMVARecipe_ = ( cfg.getParameter<bool>( "useMVARecipe" ) );
            }
            if( cfg.exists( "useLooseID" )  ){
                useLooseID_ = ( cfg.getParameter<bool>( "useLooseID" ) );
            }

            if( cfg.exists( "deltaRPhoElectronThreshold" )  ){
                deltaRPhoElectronThreshold_ = ( cfg.getParameter<double>( "deltaRPhoElectronThreshold" ) );
            }

            if( cfg.exists( "deltaRTrkElec" )  ){
                deltaRTrkElec_ = ( cfg.getParameter<double>( "deltaRTrkElec" ) );
            }

            if( cfg.exists( "deltaMassElectronZThreshold" )  ){
                deltaMassElectronZThreshold_ = ( cfg.getParameter<double>( "deltaMassElectronZThreshold" ) );
            }

        };
        ~ElectronUtils(){};
        
        void  initialize( );
        
        bool  passesElectronSelection( const flashgg::Electron *Electron ){
            
            //            std::cout<<"passes electron selection?"<<std::endl;
            pass_=true;
            
            //            std::cout<<"pass starts "<<pass_<<std::endl;
            
            assert(ElectronEtaCuts_.size()==3);

            int idIndex=0;
            if (useLooseID_) idIndex=0;
            else idIndex=1;
            if(useMVARecipe_) idIndex+=2;
        
            float Electron_eta = fabs( Electron->superCluster()->eta() );
            
            if( Electron_eta > ElectronEtaCuts_[2] || ( Electron_eta > ElectronEtaCuts_[0] && Electron_eta < ElectronEtaCuts_[1] ) )  pass_=false;
            //            std::cout<<"after eta accept, pass is "<<pass_<<std::endl;
            if( Electron->pt() < ElectronPtThreshold_ ) pass_=false; 
            //            std::cout<<"after eta accept, pass is "<<pass_<<std::endl;
            
            bool ID;
            if( idIndex==0) ID = Electron->passLooseId();
            else if( idIndex==1) ID = Electron->passMediumId();
            else if( idIndex==2) ID = Electron->passMVAMediumId();
            else if( idIndex==3) ID = Electron->passMVATightId();
                            
            if(!ID) pass_ = false;
            //            std::cout<<"after id sel, pass is "<<pass_<<std::endl;

            if( Electron->hasMatchedConversion() ) pass_= false; 
            //            std::cout<<"after conversion, pass is "<<pass_<<std::endl;
                        
            return pass_;
        }
        

        bool  passesPhoVeto( const flashgg::Electron *electron, const std::vector<edm::Ptr<flashgg::DiPhotonCandidate> > &diphotonPointers  ){
            
            //   std::cout<<"passes pho veto?"<<std::endl;

            passPhoVeto_=true;
            //            std::cout<<"passphoveto starts "<<passPhoVeto_<<std::endl;

            double Zmass_ = 91.9;

            std::vector<const flashgg::Photon *> photons;                
            for(size_t idipho = 0; idipho < diphotonPointers.size(); idipho++){
                edm::Ptr<flashgg::DiPhotonCandidate> diphoton = diphotonPointers[idipho];
                photons.push_back( diphoton->leadingPhoton() );
                photons.push_back( diphoton->subLeadingPhoton() );
        }
        
            TLorentzVector elec_p4;
            elec_p4.SetXYZT( electron->px(), electron->py(), electron->pz(), electron->energy() );
        
            TLorentzVector elec_superClusterVect;
            elec_superClusterVect.SetXYZT(electron->superCluster()->position().x(),electron->superCluster()->position().y(),
                                          electron->superCluster()->position().z(),electron->ecalEnergy());            


        
            for( unsigned int phoIndex = 0; phoIndex <photons.size(); phoIndex++ ) {

                float drPhoElec=deltaR( electron->eta(), electron->phi(), photons.at( phoIndex )->superCluster()->eta(),  photons.at( phoIndex )->superCluster()->phi() ); 
                if( drPhoElec < deltaRPhoElectronThreshold_ ) passPhoVeto_=false;
                //                std::cout<<"adter deltaRPhoElectronThreshold_, passphoveto is "<<passPhoVeto_<<std::endl;
            
                TLorentzVector p;
                p.SetXYZT( photons.at( phoIndex )->px(), photons.at( phoIndex )->py(), photons.at( phoIndex )->pz(), photons.at( phoIndex )->energy() );
            
                if( p.DeltaR( elec_superClusterVect ) < deltaRPhoElectronThreshold_ ) passPhoVeto_=false;
                //                std::cout<<"adter deltaRPhoElectronThreshold_, passphoveto is "<<passPhoVeto_<<std::endl;
            
                if( &( *photons.at( phoIndex )->superCluster() ) == &( *electron->superCluster() ) ) {
                    float TrkElecSCDeltaR = sqrt( electron->deltaEtaSuperClusterTrackAtVtx() * electron->deltaEtaSuperClusterTrackAtVtx() +
                                                  electron->deltaPhiSuperClusterTrackAtVtx() * electron->deltaPhiSuperClusterTrackAtVtx() );
                    if( TrkElecSCDeltaR < deltaRTrkElec_ ) passPhoVeto_=false;               
                }
                //                std::cout<<"adter deltaRTrkElec_, passphoveto is "<<passPhoVeto_<<std::endl;
            
                if( p.DeltaR( elec_p4 ) < deltaRPhoElectronThreshold_ ) passPhoVeto_=false;
                //                std::cout<<"adter deltaRPhoElectronThreshold_, passphoveto is "<<passPhoVeto_<<std::endl;
            
                TLorentzVector elep = elec_p4 + p;
                if( fabs( elep.M() - Zmass_ ) < deltaMassElectronZThreshold_ ) passPhoVeto_=false;   
                //                std::cout<<"adter deltaMassElectronZThreshold_, passphoveto is "<<passPhoVeto_<<std::endl;
            }
            return passPhoVeto_;


        }
        



    private:
        double ElectronPtThreshold_;
        std::vector<double> ElectronEtaCuts_;
        bool useMVARecipe_;
        bool useLooseID_;

        double deltaRPhoElectronThreshold_;
        double deltaRTrkElec_;
        double deltaMassElectronZThreshold_;


        bool pass_;
        bool passPhoVeto_;
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

