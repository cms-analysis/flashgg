#ifndef flashgg_bRegressionDumper_h
#define flashgg_bRegressionDumper_h

#include "flashgg/DataFormats/interface/Jet.h"

#include "flashgg/Taggers/interface/CollectionDumper.h"

namespace flashgg
{
    typedef CollectionDumper<std::vector<Jet> > bRegressionDumper; 
    typedef CollectionDumper<std::vector<Jet>,                     
                             Jet,                                                   
                             CutBasedClassifier<Jet> > CutBasedbRegressionDumper;
}

#endif

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
