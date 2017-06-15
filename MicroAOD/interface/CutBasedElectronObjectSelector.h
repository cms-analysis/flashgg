#ifndef flashgg_CutBasedElectronObjectSelector_h
#define flashgg_CutBasedElectronObjectSelector_h

#include "flashgg/MicroAOD/interface/CutBasedGenericObjectSelector.h"

#include "flashgg/DataFormats/interface/Electron.h"

namespace flashgg {

	typedef CutBasedGenericObjectSelector<edm::View<flashgg::Electron> > CutBasedElectronObjectSelector;

}

#endif // flashgg_CutBasedElectronObjectSelector_h
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
