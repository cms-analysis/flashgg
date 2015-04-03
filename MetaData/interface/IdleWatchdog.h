#ifndef _flashgg_IdleWatchdog_h_
#define _flashgg_IdleWatchdog_h_

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TStopwatch.h"

namespace flashgg {

    class IdleWatchdog
    {
    public:
        IdleWatchdog( const edm::ParameterSet & );

        void check();
        void reset();

    protected:
        double minIdleFraction_;
        int checkEvery_, tolerance_;
        int nFailures_, ievent_;

        TStopwatch stopWatch_;

    };
}
#endif // _flashgg_IdleWatchdog_h_
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

