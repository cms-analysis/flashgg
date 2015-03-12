#include "flashgg/DataFormats/interface/SingleVertexView.h"

namespace flashgg {

	void SingleVertexView::vertexMaybe() const
	{
		if(vtx_ == 0) { 
			if( dipho_ == 0 ) { dipho_ = &(*edmdipho_); }
			if( dipho_ == 0 ) throw;
			vtx_ = &*(dipho_->vertexPtr(ivtx_)); 
			if( vtx_ == 0 ) throw;
		}
	}
}
