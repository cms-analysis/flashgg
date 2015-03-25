
#ifndef FLASHgg_PhotonIdUtils_h
#define FLASHgg_PhotonIdUtils_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Handle.h"
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

  class PhotonIdUtils {

  public:
    
    PhotonIdUtils();
    ~PhotonIdUtils();

    void               initialize( );

    float              pfIsoChgWrtVtx( edm::Ptr<pat::Photon>&, 
				       const edm::Ptr<reco::Vertex>,
				       const flashgg::VertexCandidateMap,
				       float, float, float, float );
    std::map<edm::Ptr<reco::Vertex>,float> pfIsoChgWrtAllVtx( edm::Ptr<pat::Photon>&, 
							      const edm::Handle<edm::View<reco::Vertex> >&,
							      const flashgg::VertexCandidateMap, 
							      float, float, float, float );

    float              pfIsoChgWrtWorstVtx( std::map<edm::Ptr<reco::Vertex>,float>&);

    float              pfCaloIso( edm::Ptr<pat::Photon>&, 
				   const edm::Handle<edm::View<pat::PackedCandidate> >&, 
				  float, float, float, float, float, float, float, reco::PFCandidate::ParticleType, const reco::Vertex * vtx=0);
    
    
    void               setupMVA( const std::string&, const std::string& );
    float              computeMVAWrtVtx( flashgg::Photon&, const edm::Ptr<reco::Vertex>&, const double );

    static flashgg::Photon     pho4MomCorrection(edm::Ptr<flashgg::Photon>&, edm::Ptr<reco::Vertex>);

    static bool vetoPackedCand(const pat::Photon& photon, const edm::Ptr<pat::PackedCandidate> & pfcand);

    std::map<edm::Ptr<reco::Vertex>,float> computeMVAWrtAllVtx( flashgg::Photon&, const edm::Handle< edm::View<reco::Vertex> >&, const double );
    
    std::shared_ptr<TMVA::Reader> phoIdMva;

    void removeOverlappingCandidates(bool x) { removeOverlappingCandidates_ = x; };
    
    
    static void recomputeNonZsClusterShapes(reco::Photon & pho, noZS::EcalClusterLazyTools &tools);
    static void recomputeNonZsClusterShapes(reco::Photon & pho, const EcalRecHitCollection* ebRecHists, const EcalRecHitCollection * eeRecHist, const CaloTopology * topology);
   
    template <class T> static void fillExtraClusterShapes(flashgg::Photon & pho, T & lazyTool) {
      const reco::CaloClusterPtr  seed_clu = pho.superCluster()->seed();
      const reco::SuperClusterRef super_clu= pho.superCluster();
      
      std::vector<float> viCov = lazyTool.localCovariances(*seed_clu);
	    
      pho.setSipip(viCov[2]);
      pho.setSieip(viCov[1]);
      pho.setE2nd(lazyTool.e2nd(*seed_clu));
      pho.setE2x5right(lazyTool.e2x5Right(*seed_clu));
      pho.setE2x5left(lazyTool.e2x5Left(*seed_clu));
      pho.setE2x5top(lazyTool.e2x5Top(*seed_clu));
      pho.setE2x5bottom(lazyTool.e2x5Bottom(*seed_clu));
      pho.setE2x5max(lazyTool.e2x5Max(*seed_clu));
      pho.setEright(lazyTool.e2x5Right(*seed_clu));
      pho.setEleft(lazyTool.e2x5Left(*seed_clu));
      pho.setEtop(lazyTool.e2x5Top(*seed_clu));
      pho.setEbottom(lazyTool.e2x5Bottom(*seed_clu));
      pho.setE1x3(lazyTool.e1x3(*seed_clu));
      pho.setS4(lazyTool.e2x2(*seed_clu)/lazyTool.e5x5(*seed_clu));
      pho.setESEffSigmaRR(lazyTool.eseffsirir(*super_clu));
    }
    
    
  private: 
    
    bool removeOverlappingCandidates_;
    
    // photon MVA variables: move to more sophisticated object?
    
    float phoIdMva_SCRawE_;                  
    float phoIdMva_R9_;                      
    float phoIdMva_covIEtaIEta_;             
    float phoIdMva_PhiWidth_;  
    float phoIdMva_EtaWidth_;             
    float phoIdMva_covIEtaIPhi_;                                                                                                                            
    float phoIdMva_S4_;                     
    float phoIdMva_pfPhoIso03_;             
    float phoIdMva_pfChgIso03_;              
    float phoIdMva_pfChgIso03worst_;         
    float phoIdMva_ScEta_;                  
    float phoIdMva_rho_;                    
    float phoIdMva_ESEffSigmaRR_;           

    std::shared_ptr<TMVA::Reader> phoIdMva_2012_EB_;
    std::shared_ptr<TMVA::Reader> phoIdMva_2012_EE_;

  };


}


#endif
