#include "flashgg/MetaData/interface/IdleWatchdog.h"

#include <iostream>
#include <cassert>

using namespace std;

namespace flashgg {

    IdleWatchdog::IdleWatchdog( const edm::ParameterSet &iConfig ) :
        minIdleFraction_( iConfig.getUntrackedParameter<double>( "minIdleFraction", 0.2 ) ),
        checkEvery_( iConfig.getUntrackedParameter<int>( "checkEvery", 1000 ) ),
        tolerance_( iConfig.getUntrackedParameter<int>( "tolerance", 5 ) )
    {
    }

    void IdleWatchdog::check()
    {
        ++ievent_;
        if( ( ( ievent_ - 1 ) % checkEvery_ ) != 0 ) {
            return;
        }


        // cout << "checking " << endl;
        stopWatch_.Stop();
        float cputime  = stopWatch_.CpuTime();
        float realtime = stopWatch_.RealTime();

        // std::cout << cputime << " " << realtime << std::endl;
        if( cputime / realtime < minIdleFraction_ ) {
            --nFailures_;
        } else {
            nFailures_ = tolerance_;
        }

        if( nFailures_ == 0 ) {
            cerr << "IdleWatchdog job too inefficient. Aborting minIdleFraction: " << minIdleFraction_ << " tolerance: " << tolerance_ << " aborting " << endl;
            exit( 99 );
        }
        stopWatch_.Start();
    }

    void IdleWatchdog::reset()
    {
        ievent_ = 0;
        nFailures_ = tolerance_;
        stopWatch_.Stop();
        stopWatch_.Start();
    }

}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

