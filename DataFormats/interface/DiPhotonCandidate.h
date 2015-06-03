#ifndef FLASHgg_DiPhotonCandidate_h
#define FLASHgg_DiPhotonCandidate_h

#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

namespace flashgg {
    class SinglePhotonView;

    class DiPhotonCandidate : public reco::CompositeCandidate
    {
    public:
        DiPhotonCandidate();
        DiPhotonCandidate( edm::Ptr<flashgg::Photon>, edm::Ptr<flashgg::Photon>, edm::Ptr<reco::Vertex> );
        DiPhotonCandidate( const flashgg::Photon &, const flashgg::Photon &, edm::Ptr<reco::Vertex> );
        ~DiPhotonCandidate();

        const edm::Ptr<reco::Vertex> vtx() const { return vertex_; }
        const flashgg::Photon *leadingPhoton() const;
        const flashgg::Photon *subLeadingPhoton() const;

        flashgg::Photon &getLeadingPhoton();
        flashgg::Photon &getSubLeadingPhoton();

        flashgg::SinglePhotonView leadingView() const;
        flashgg::SinglePhotonView subLeadingView() const;

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
            return ( this->daughter( 0 )->pt() + this->daughter( 1 )->pt() );
        }
        int vertexIndex() const { return vertex_index_; }

        unsigned int nVtxInfoSize() const { return ( vlogsumpt2_.size() ) ;}
        float logSumPt2( unsigned int iVtx ) const { return ( iVtx < vlogsumpt2_.size() ) ? vlogsumpt2_.at( iVtx ) : -9999. ;}
        float ptBal( unsigned int iVtx ) const  { return iVtx < vptbal_.size() ? vptbal_.at( iVtx ) : -9999. ;}
        float ptAsym( unsigned int iVtx ) const  { return iVtx < vptasym_.size() ? vptasym_.at( iVtx ) : -9999. ; }
        float nConv( unsigned int iVtx ) const  { return iVtx < vnConv_.size() ? vnConv_.at( iVtx ) : -9999.; }
        float pullConv( unsigned int iVtx ) const  { return iVtx < vpull_conv_.size() ? vpull_conv_.at( iVtx ) : -9999.; }
        float mva( unsigned int iVtx ) const  { return iVtx < vmva_value_.size() ? vmva_value_.at( iVtx ) : -9999.; }
        int mvaSortedIndex( unsigned int iVtx ) const  { return iVtx < vmva_sortedindex_.size() ? vmva_sortedindex_.at( iVtx ) : -1; }
        edm::Ptr<reco::Vertex> vertexPtr( unsigned int iVtx ) const  { return iVtx < vVtxPtr_.size() ? vVtxPtr_.at( iVtx ) : edm::Ptr<reco::Vertex>(); }

        float leadPhotonId() const { return leadingPhoton()->phoIdMvaDWrtVtx( vertex_ ); }
        float subLeadPhotonId() const { return subLeadingPhoton()->phoIdMvaDWrtVtx( vertex_ ); }

    private:

        edm::Ptr<reco::Vertex> vertex_;
        int vertex_index_;

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

        std::vector<float> vlogsumpt2_;
        std::vector<float> vptbal_;
        std::vector<float> vptasym_;
        std::vector<float> vpull_conv_;
        std::vector<float> vnConv_;
        std::vector<float> vmva_value_;
        std::vector<unsigned int> vmva_sortedindex_;
        std::vector<edm::Ptr<reco::Vertex> > vVtxPtr_;

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

