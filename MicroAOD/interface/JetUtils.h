#ifndef FLASHgg_JetUtils_h
#define FLASHgg_JetUtils_h

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

/// class EcalRecHitCollection;
class CaloTopology;

#include <TMVA/Reader.h>


namespace flashgg {
    
    
    class JetUtils
    {
        
    public:
        
        JetUtils( const edm::ParameterSet &cfg){
            if( cfg.exists( "pujidWpPtBin1"  )  ){
                pujidWpPtBin1_ = cfg.getParameter<vector<double> >( "pujidWpPtBin1"  );
            }
            
            if( cfg.exists( "pujidWpPtBin2"  )  ){
                pujidWpPtBin2_ = cfg.getParameter<vector<double> >( "pujidWpPtBin2"  );
            }
            
            if( cfg.exists( "pujidWpPtBin3"  )  ){
                pujidWpPtBin3_ = cfg.getParameter<vector<double> >( "pujidWpPtBin3"  );
            }
            
            if( cfg.exists( "eta_cuts"  )  ){
                eta_cuts_ = cfg.getParameter<vector<double> >( "eta_cuts"  );
            }
            
        };
        ~JetUtils(){};
        
        void  initialize( );
        
        bool  passesPuJetId( const flashgg::Jet *jet){
            
            
            pass_=false;
            if ( (!pujidWpPtBin1_.empty())  &&
                 (!pujidWpPtBin2_.empty())  &&
                 (!pujidWpPtBin3_.empty())  &&
                 (!eta_cuts_.empty())  ){
                for (UInt_t eta_bin=0; eta_bin < pujidWpPtBin1_.size(); eta_bin++ ){
                    //                        if ( fabs( jet->eta() ) >  eta_cuts_[eta_bin].first &&
                    //                             fabs( jet->eta() ) <= eta_cuts_[eta_bin].second){
                    if ( fabs( jet->eta() ) >  eta_cuts_[eta_bin] &&
                         fabs( jet->eta() ) <= eta_cuts_[eta_bin+1]){
                        if ( jet->pt() >  20 &&
                             jet->pt() <= 30 && jet->puJetIdMVA() > pujidWpPtBin1_[eta_bin] )
                            pass_=true;
                        if ( jet->pt() >  30 &&
                             jet->pt() <= 50 && jet->puJetIdMVA() > pujidWpPtBin2_[eta_bin] )
                            pass_=true;
                        if ( jet->pt() >  50 &&
                             jet->pt() <= 100&& jet->puJetIdMVA() > pujidWpPtBin3_[eta_bin] )
                            pass_=true;
                        if (jet->pt() > 100) pass_ = true;
                    }
                }
            }
            else {
                return true;
            }
            //                    std::cout << inputTagJets_[0] << " pt="<< jet->pt() << " :eta: "<< jet->eta() << " :mva: "<< jet->puJetIdMVA() << "  pass_ == " << pass_ << std::endl;
            return pass_;
        }
        
        

    private:
        std::vector<double > pujidWpPtBin1_;
        std::vector<double > pujidWpPtBin2_;
        std::vector<double > pujidWpPtBin3_;
        std::vector<double > eta_cuts_;
        
        bool pass_;
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

