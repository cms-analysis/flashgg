#include "flashgg/DataFormats/interface/DiPhotonMVAResult.h"


namespace flashgg {

    DiPhotonMVAResult::DiPhotonMVAResult() :
        leadptom( -9999. ),
        subleadptom( -9999. ),
        subleadmva( -9999. ),
        leadmva( -9999. ),
        leadeta( -9999. ),
        subleadeta( -9999. ),
        sigmarv( -9999. ),
        decorrSigmarv( -9999. ),
        sigmawv( -9999. ),
        CosPhi( -9999. ),
        vtxprob( -9999. ),
        result( -9999. ) {}
}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

