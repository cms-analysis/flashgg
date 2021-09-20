#include "flashgg/DataFormats/interface/LHEInfoObject.h"
#include <iostream>

using namespace flashgg;

LHEInfoObject::LHEInfoObject() {}

LHEInfoObject::~LHEInfoObject() {}

LHEInfoObject *LHEInfoObject::clone() const
{
    LHEInfoObject *result = new LHEInfoObject;
    result->setIncomingPartons( incomingPartons() );
    result->setOutgoingPartons( outgoingPartons() );
    result->setHiggsBoson( higgsBoson() );
    return result;

}

