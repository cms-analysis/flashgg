#ifndef _NullSystematics_h
#define _NullSystematics_h

#include "AbstractSystematicsModel.h"

#include "interpolation/SignalModelParameterInterpolatorMap.h"

/** class implementing NO systematic uncertainties at all.
    Useful for a first test of a new modeling (but keep
    in mind that the sensitivity will only decrease
    when systematic uncertanities are added)
*/
class NullSystematics : public AbstractSystematicsModel
{
protected:
  const SignalModelParameterInterpolatorMap &effAccs;
  const SignalModelParameterInterpolatorMap &fitparmfuncs;
  
public:
  NullSystematics(const SignalModelParameterInterpolatorMap &_effAccs,
                  const SignalModelParameterInterpolatorMap &_fitparmfuncs
                  );

  /** returns the efficiency * acceptance */
  virtual RooAbsReal* getNuisancedEffAcc(const std::string &catname, const std::string &inputSigProcName);

  /** returns the corresponding parameter for the signal shape */
  virtual RooAbsReal* getNuisancedParameter(const std::string &catname,
                                            const std::string &inputSigProcName, const std::string &paramName);

};

#endif
