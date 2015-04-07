#include "flashgg/DataFormats/interface/SinglePhotonView.h"

namespace flashgg {

    void SinglePhotonView::daughterMaybe() const
    {
        if( pho_ == 0 ) {
            if( dipho_ == 0 ) { dipho_ = &( *edmdipho_ ); }
            if( dipho_ == 0 ) { throw; }
            pho_ = dynamic_cast<const Photon *>( dipho_->daughter( daughter_ ) );
            if( pho_ == 0 ) { throw; }
        }
    }
}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

