#include "flashgg/DataFormats/interface/Met.h"

using namespace flashgg;

Met::Met() : pat::MET()
{}

Met::~Met()
{}

Met::Met( const pat::MET &amet ) : pat::MET( amet )
{}
/*
Met::setCorPx(float mPx)
{
    
}
Met::setCorPy(float mPx)
{

}
Met::setCorPhi(float mPx)
{

}
*/


// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

