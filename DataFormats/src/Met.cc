#include "flashgg/DataFormats/interface/Met.h"

using namespace flashgg;

Met::Met() : pat::MET()
{}

Met::~Met()
{}

Met::Met( const pat::MET &amet ) : pat::MET( amet )
{
    corpx= amet.corPx();
    corpy= amet.corPy();
    corphi= amet.corPhi();
    corpt= amet.corPt();
}

void Met::setCorPx(float mPx)
{
    corpx=mPx;
    corphi = std::atan2(corpy,corpx);
    corpt = hypotf(corpx,corpy); 
}
void Met::setCorPy(float mPy)
{
    corpy=mPy;
    corphi = std::atan2(corpy,corpx);
    corpt = hypotf(corpy,corpx); 
}

void Met::setCorPt(float mPt)
{
    corpt=mPt;
}
void Met::setCorPhi(float mPhi)
{
    corpy=mPhi;
}

void Met::setPassEcalBadCalibFilter(bool mPass)
{
    passEcalBadCalibFilter = mPass;
}

float Met::getCorPx() const
{
    return(corpx);
}
float Met::getCorPy() const
{
    return(corpy);
}
float Met::getCorPhi() const
{
    return(corphi);
}
float Met::getCorPt() const
{
    return(corpt);
}

bool Met::getPassEcalBadCalibFilter() const
{
    return(passEcalBadCalibFilter);
}

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

