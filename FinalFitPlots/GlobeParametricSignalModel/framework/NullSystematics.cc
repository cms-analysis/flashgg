#include "NullSystematics.h"

using namespace std;

//----------------------------------------------------------------------
NullSystematics::NullSystematics(const SignalModelParameterInterpolatorMap &_effAccs,
                                 const SignalModelParameterInterpolatorMap &_fitparmfuncs
                                 ) : 
  effAccs(_effAccs),
  fitparmfuncs(_fitparmfuncs)
{
}

//----------------------------------------------------------------------

RooAbsReal* 
NullSystematics::getNuisancedEffAcc(const std::string &catname, const string &inputSigProcName)
{
  return effAccs.get(catname, inputSigProcName)->getFunction(0);
}

//----------------------------------------------------------------------
RooAbsReal* 
NullSystematics::getNuisancedParameter(const std::string &catname,
                                       const std::string &inputSigProcName, const std::string &paramName)
{
  // we also use the input signal process name here
  return fitparmfuncs.get(catname, inputSigProcName)->getFunction(paramName);
}

//----------------------------------------------------------------------
