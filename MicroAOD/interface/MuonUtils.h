#ifndef FLASHgg_MuonUtils_h
#define FLASHgg_MuonUtils_h

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

using namespace edm;

namespace flashgg {
    
    
    class MuonUtils
    {
        
    public:
        
        MuonUtils( const edm::ParameterSet &cfg){
            if( cfg.exists( "MuonFiltersFlags"  )  ){
                muPFIsoSumRelThreshold_ = cfg.getParameter<double>( "MuonFiltersFlags"  );
            }
        };
        ~MuonUtils(){};
        
        void  initialize( );
        
        bool  passesMuonSelection( const flashgg::Muon *muon, const std::vector<edm::Ptr<reco::Vertex> > &vertexPointers){
            
            pass_=false;
            int vtxInd = 0;
            double dzmin = 9999;
            for( size_t ivtx = 0 ; ivtx < vertexPointers.size(); ivtx++ ) {
                Ptr<reco::Vertex> vtx = vertexPointers[ivtx];
                if( !muon->innerTrack() ) continue; 
                if( fabs( muon->innerTrack()->vz() - vtx->position().z() ) < dzmin ) {                    
                    dzmin = fabs( muon->innerTrack()->vz() - vtx->position().z() );
                    vtxInd = ivtx;
                }
            }
            
            Ptr<reco::Vertex> best_vtx = vertexPointers[vtxInd];            

            if( muon::isTightMuon( *muon, *best_vtx ) ) pass_=true; 
            
            double muPFIsoSumRel = ( muon->pfIsolationR04().sumChargedHadronPt 
                                     + max( 0.,muon->pfIsolationR04().sumNeutralHadronEt 
                                            + muon->pfIsolationR04().sumPhotonEt - 0.5 * muon->pfIsolationR04().sumPUPt ) ) / ( muon->pt() );

            if( muPFIsoSumRel <= muPFIsoSumRelThreshold_ ) pass_=true; 
            
            
            return pass_;
        }
        
        

    private:
        double muPFIsoSumRelThreshold_;
        //        edm::EDGetTokenT<edm::TriggerResults> triggerRECO_;
        //        edm::EDGetTokenT<edm::TriggerResults> triggerPAT_;
        
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

