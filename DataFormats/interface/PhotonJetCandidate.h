#ifndef FLASHgg_PhotonJetCandidate_h
#define FLASHgg_PhotonJetCandidate_h

#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/Candidate/interface/ShallowCloneCandidate.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "flashgg/DataFormats/interface/SinglePhotonView.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "flashgg/DataFormats/interface/WeightedObject.h"

#include "TLorentzVector.h"

namespace flashgg {

    class PhotonJetCandidate : public WeightedObject, public reco::CompositeCandidate
    {
    public:
        PhotonJetCandidate();
        PhotonJetCandidate( edm::Ptr<flashgg::Photon>, edm::Ptr<pat::Jet>, edm::Ptr<reco::Vertex> );
        ~PhotonJetCandidate();

        const edm::Ptr<reco::Vertex> vtx() const { return vertex_; }

        const flashgg::Photon *photon() const ;
        const pat::Jet *jet() const ;

        void setLogSumPt2( float val ) { logsumpt2_ = val; }
        void setPtBal( float val ) { ptbal_ = val; }
        void setPtAsym( float val ) { ptasym_ = val; }
        void setNConv( float val ) { nConv_ = val; }
        void setPullConv( float val ) { pull_conv_ = val; }
        void setNVert( float val ) { nVert_ = val; }
        void setMVA0( float val ) { MVA0_ = val; }
        void setMVA1( float val ) { MVA1_ = val; }
        void setMVA2( float val ) { MVA2_ = val; }
        void setDZ1( float val ) { dZ1_ = val; }
        void setDZ2( float val ) { dZ2_ = val; }
        void setVtxProbMVA( float val ) { vtxprobmva_ = val; }
        void setVertexIndex( int val ) { vertex_index_ = val; }
        void setVertexIndexJet( int val ) { vertex_index_jet_ = val; }
        void setDZfromRecoPV( float val) { dZfromRecoPV_ = val;}
        void setPhotonJetPt( float val ) { photonjetPt_ = val; }

        void setVLogSumPt2( std::vector<float> vval ) { vlogsumpt2_ = vval; }
        void setVPtBal( std::vector<float> vval ) { vptbal_ = vval; }
        void setVPtAsym( std::vector<float> vval ) { vptasym_ = vval; }
        void setVNConv( std::vector<float> vval ) { vnConv_ = vval; }
        void setVPullConv( std::vector<float> vval ) { vpull_conv_ = vval; }
        void setVMVA( std::vector<float> vval ) { vmva_value_ = vval; }
        void setVVtxPtr( std::vector<edm::Ptr<reco::Vertex> >  vval ) { vVtxPtr_ = vval;}
        void setVMVASortedIndex( std::vector<unsigned int>  vval ) { vmva_sortedindex_ = vval;}

        float logSumPt2() const { return logsumpt2_; }
        float ptBal() const { return ptbal_; }
        float ptAsym() const { return ptasym_; }
        float nConv() const { return nConv_; }
        float pullConv() const { return pull_conv_; }
        float nVert() const { return nVert_; }
        float mva0() const { return MVA0_; }
        float mva1() const { return MVA1_; }
        float mva2() const { return MVA2_; }
        float dZ1() const { return dZ1_; }
        float dZ2() const { return dZ2_; }
        float vtxProbMVA() const { return vtxprobmva_; }
        float sumPt() const
        {
            return ( photon()->pt() + jet()->pt() );
        }
        float dZfromRecoPV() const {return dZfromRecoPV_; }
        float photonjetPt() const {return photonjetPt_; }

        int vertexIndex() const { return vertex_index_; }
        int vertexIndexJet() const { return vertex_index_jet_; }

        unsigned int nVtxInfoSize() const { return ( vlogsumpt2_.size() ) ;}
        float logSumPt2( unsigned int iVtx ) const { return ( iVtx < vlogsumpt2_.size() ) ? vlogsumpt2_.at( iVtx ) : -9999. ;}
        float ptBal( unsigned int iVtx ) const  { return iVtx < vptbal_.size() ? vptbal_.at( iVtx ) : -9999. ;}
        float ptAsym( unsigned int iVtx ) const  { return iVtx < vptasym_.size() ? vptasym_.at( iVtx ) : -9999. ; }
        float nConv( unsigned int iVtx ) const  { return iVtx < vnConv_.size() ? vnConv_.at( iVtx ) : -9999.; }
        float pullConv( unsigned int iVtx ) const  { return iVtx < vpull_conv_.size() ? vpull_conv_.at( iVtx ) : -9999.; }
        float mva( unsigned int iVtx ) const  { return iVtx < vmva_value_.size() ? vmva_value_.at( iVtx ) : -9999.; }
        int mvaSortedIndex( unsigned int iVtx ) const  { return iVtx < vmva_sortedindex_.size() ? vmva_sortedindex_.at( iVtx ) : -1; }
        edm::Ptr<reco::Vertex> vertexPtr( unsigned int iVtx ) const  { return iVtx < vVtxPtr_.size() ? vVtxPtr_.at( iVtx ) : edm::Ptr<reco::Vertex>(); }

        float photonId() const { return photon()->phoIdMvaDWrtVtx( vertex_ ); }

        bool operator <( const PhotonJetCandidate &b ) const;
        bool operator >( const PhotonJetCandidate &b ) const;

    private:

    private:
        
        edm::Ptr<reco::Vertex> vertex_;
        int vertex_index_;
        int vertex_index_jet_;

        float logsumpt2_;
        float ptbal_;
        float ptasym_;
        float nConv_;
        float pull_conv_;
        float nVert_;
        float MVA0_;
        float MVA1_;
        float MVA2_;
        float dZ1_;
        float dZ2_;
        float vtxprobmva_;
        float dZfromRecoPV_;
        float photonjetPt_;

        std::vector<float> vlogsumpt2_;
        std::vector<float> vptbal_;
        std::vector<float> vptasym_;
        std::vector<float> vpull_conv_;
        std::vector<float> vnConv_;
        std::vector<float> vmva_value_;
        std::vector<unsigned int> vmva_sortedindex_;
        std::vector<edm::Ptr<reco::Vertex> > vVtxPtr_;

        flashgg::SinglePhotonView viewPho_;

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

