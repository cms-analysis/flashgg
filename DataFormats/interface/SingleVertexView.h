#ifndef FLASHgg_SingleVertexView_h
#define FLASHgg_SingleVertexView_h

#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "DataFormats/Common/interface/Ptr.h"

#include <string>

namespace flashgg {

    class SingleVertexView
    {

    public:
        typedef edm::Ptr<DiPhotonCandidate> dipho_ptr_type;
        typedef reco::Vertex cand_type;

        virtual ~SingleVertexView() {}

        SingleVertexView() : isClosestToGen_( false ), dZtoGen_( 0. ), vtx_( 0 ), dipho_( 0 ) {}
        SingleVertexView( dipho_ptr_type dipho, int ivtx ) : isClosestToGen_( false ), dZtoGen_( 0. ), edmdipho_( dipho ), ivtx_( ivtx ), vtx_( 0 ), dipho_( 0 ) {}
        SingleVertexView( const DiPhotonCandidate *dipho, int ivtx ) : isClosestToGen_( false ), dZtoGen_( 0. ), ivtx_( ivtx ), vtx_( 0 ), dipho_( dipho ) {}

        const cand_type &pos() const { vertexMaybe(); return *vtx_; }

        const cand_type *operator->() const { vertexMaybe(); return vtx_; }

        float logSumPt2() const { vertexMaybe(); return dipho_->logSumPt2( ivtx_ ); }
        float ptBal() const { vertexMaybe(); return dipho_->ptBal( ivtx_ );     }
        float ptAsym() const { vertexMaybe(); return dipho_->ptAsym( ivtx_ );    }
        float nConv() const { vertexMaybe(); return dipho_->nConv( ivtx_ );     }
        float pullConv() const { vertexMaybe(); return dipho_->pullConv( ivtx_ );  }
        float mva() const { vertexMaybe(); return dipho_->mva( ivtx_ );       }

        bool isClosestToGen() const { return isClosestToGen_; }
        float dZtoGen() const { return dZtoGen_; }

        void setDzToGen( float x ) { dZtoGen_ = x; };
        void setIsClosestToGen( bool x = true ) { isClosestToGen_ = x; };

    private:
        void vertexMaybe() const;
        bool isClosestToGen_;
        float dZtoGen_;

        dipho_ptr_type edmdipho_;
        int ivtx_;
        mutable const cand_type *vtx_;
        mutable const DiPhotonCandidate *dipho_;

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

