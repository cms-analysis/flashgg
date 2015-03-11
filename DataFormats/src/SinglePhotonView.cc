#include "flashgg/DataFormats/interface/SinglePhotonView.h"

namespace flashgg {

	void SinglePhotonView::getDaughterMaybe() const
	{
		if(pho_ == 0) { 
			if( dipho_ == 0 ) { dipho_ = &(*edmdipho_); }
			if( dipho_ == 0 ) throw;
			pho_ = dynamic_cast<const Photon *>(dipho_->daughter(daughter_)); 
			if( pho_ == 0 ) throw;
		}
	}
}
