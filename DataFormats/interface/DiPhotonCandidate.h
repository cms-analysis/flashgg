#ifndef FLASHgg_DiPhotonCandidate_h
#define FLASHgg_DiPhotonCandidate_h

#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/Candidate/interface/ShallowCloneCandidate.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "flashgg/DataFormats/interface/SinglePhotonView.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "flashgg/DataFormats/interface/WeightedObject.h"
#include "DataFormats/Math/interface/Point3D.h"



namespace flashgg {

    class DiPhotonCandidate : public reco::LeafCandidate , public WeightedObject
    {
    public:
        DiPhotonCandidate();
        DiPhotonCandidate( edm::Ptr<flashgg::Photon>, edm::Ptr<flashgg::Photon>, edm::Ptr<reco::Vertex> );
        ~DiPhotonCandidate();

        const edm::Ptr<reco::Vertex> vtx() const { return vertex_; }

        const flashgg::Photon *leadingPhoton() const { return viewPho1_.photon(); };
        const flashgg::Photon *subLeadingPhoton() const { return viewPho2_.photon(); }

        flashgg::Photon &getLeadingPhoton() { return viewPho1_.getPhoton(); }
        flashgg::Photon &getSubLeadingPhoton() { return viewPho2_.getPhoton(); };

        flashgg::SinglePhotonView &getLeadingView() { return viewPho1_; }
        flashgg::SinglePhotonView &getSubLeadingView() { return viewPho2_; }

        const flashgg::SinglePhotonView *leadingView() const { return &viewPho1_; }
        const flashgg::SinglePhotonView *subLeadingView() const { return &viewPho2_; }

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
            return ( leadingPhoton()->pt() + subLeadingPhoton()->pt() );
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

        bool operator <( const DiPhotonCandidate &b ) const;
        bool operator >( const DiPhotonCandidate &b ) const;

        //        math::XYZTLorentzVector PhoP4Corr( edm::Ptr<flashgg::Photon> ) const;

        void computeP4AndOrder();
        void makePhotonsPersistent()
        {
            viewPho1_.MakePersistent();
            viewPho2_.MakePersistent();
        }

        void setJetCollectionIndex( unsigned int val ) { jetCollectionIndex_ = val; }
        unsigned int jetCollectionIndex() const { return jetCollectionIndex_; }

        const LorentzVector& genP4() const; 

        DiPhotonCandidate *clone() const { return ( new DiPhotonCandidate( *this ) ); }

        Point genPV() const { return genPV_; }
        void setGenPV( const Point genpv ) { genPV_ = genpv; }

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

        flashgg::SinglePhotonView viewPho1_;
        flashgg::SinglePhotonView viewPho2_;

        unsigned int jetCollectionIndex_; // index for which jet collection corresponds to the vertex choice in this diphoton

        Point genPV_;

        mutable std::vector<LorentzVector> genP4_;
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

