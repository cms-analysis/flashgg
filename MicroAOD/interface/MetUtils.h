#ifndef FLASHgg_MetUtils_h
#define FLASHgg_MetUtils_h

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
    
    
    class MetUtils
    {
        
    public:
        
        MetUtils( const edm::ParameterSet &cfg){
            if( cfg.exists( "MetFiltersFlags"  )  ){
                flagList_ = cfg.getParameter<vector<string> >( "MetFiltersFlags"  );
            }
//            if (cfg.exists("RECOFilters")){
//                triggerRECO_ = consumes<edm::TriggerResults>( cfg.getParameter<InputTag>("RECOfilters") ) ;
//            }
//            if (cfg.exists("PATFilters")){
//                triggerPAT_ = consumes<edm::TriggerResults>(iConfig.getParameter<InputTag>("PATfilters") ) ;
//            }                    
        };
        ~MetUtils(){};
        
        void  initialize( );
        
        bool  passesMetFilters( const edm::Event *evt, edm::Handle<edm::TriggerResults> triggerBits){
            
            
            pass_=true;

//            edm::Handle<edm::TriggerResults> triggerBits;
//            if(! evt.isRealData() ){ 
//                evt.getByToken( triggerPAT_, triggerBits );
//            }
//            else{
//                evt.getByToken( triggerRECO_, triggerBits );
//            }
            
            const edm::TriggerNames &triggerNames = evt->triggerNames( *triggerBits );
            
            
            //            std::vector<std::string> flagList {"Flag_HBHENoiseFilter","Flag_HBHENoiseIsoFilter","Flag_EcalDeadCellTriggerPrimitiveFilter","Flag_goodVertices","Flag_eeBadScFilter"};
            for( unsigned int i = 0; i < triggerNames.triggerNames().size(); i++ ){
                if(!triggerBits->accept(i)){
                    for(size_t j=0;j<flagList_.size();j++){
                        if(flagList_[j]==triggerNames.triggerName(i)){
                            pass_=0;
                            break;
                        }
                    }
                }
            }
            
            
            
            return pass_;
        }
        
        

    private:
        std::vector<std::string > flagList_;
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

