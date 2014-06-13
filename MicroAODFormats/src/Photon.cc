#include "flashgg/MicroAODFormats/interface/Photon.h"

using namespace flashgg;

Photon::Photon() : pat::Photon() {
  testVariable_ = 0;
}

Photon::Photon(const pat::Photon& aPhoton ) : pat::Photon(aPhoton) {
  testVariable_ = 0;
  e2nd_         = 0;
  e2x5right_    = 0;
  e2x5left_     = 0;
  e2x5top_      = 0;
  e2x5bottom_   = 0;
  e2x5max_      = 0;
  eright_       = 0;
  eleft_        = 0;
  etop_         = 0;
  ebottom_      = 0;
}

Photon::~Photon() {}

void Photon::setShowerShapeVariable(std::string showerShapeVariableName, float value){
  if(showerShapeVariableName=="e2nd")
    e2nd_=value;
  if(showerShapeVariableName=="e2x5right")
    e2x5right_=value;
  if(showerShapeVariableName=="e2x5left")
    e2x5left_=value;
  if(showerShapeVariableName=="e2x5top")
    e2x5top_=value;
  if(showerShapeVariableName=="e2x5bottom")
    e2x5bottom_=value;
  if(showerShapeVariableName=="e2x5max")
    e2x5max_=value;
  if(showerShapeVariableName=="eright")
    eright_=value;
  if(showerShapeVariableName=="eleft")
    eleft_=value;
  if(showerShapeVariableName=="etop")
    etop_=value;
  if(showerShapeVariableName=="ebottom")
    ebottom_=value;
}
float Photon::getShowerShapeVariable(std::string showerShapeVariableName){
  if(showerShapeVariableName=="e2nd")
    return(e2nd_);
 else if(showerShapeVariableName=="e2x5right")
    return(e2x5right_);
 else if(showerShapeVariableName=="e2x5left")
    return(e2x5left_);
 else if(showerShapeVariableName=="e2x5top")
    return(e2x5top_);
 else if(showerShapeVariableName=="e2x5bottom")
    return(e2x5bottom_);
 else if(showerShapeVariableName=="e2x5max")
    return(e2x5max_);
 else if(showerShapeVariableName=="eright")
    return(eright_);
 else if(showerShapeVariableName=="eleft")
    return(eleft_);
 else if(showerShapeVariableName=="etop")
    return(etop_);
 else if(showerShapeVariableName=="ebottom")
    return(ebottom_);
  else
    return(-9999);
}

