#ifndef AbstractSystematicsModel_h
#define AbstractSystematicsModel_h

#include <RooAbsReal.h>

class AbstractSystematicsModel
{
public:

  /** must return the efficiency * acceptance with all nuisance parameters attached */
  virtual RooAbsReal* getNuisancedEffAcc(const std::string &catname, const std::string &inputSigProcName) = 0;

  /** must return the nuisanced version of a parameter for the signal shape */
  virtual RooAbsReal* getNuisancedParameter(const std::string &catname,
                                            const std::string &inputSigProcName, const std::string &paramName) = 0;

  /** must return the list of all nuisance parameters.

      (shouldn't these actually be RooRealVars ? */
  virtual std::vector<RooAbsReal *> getAllNuisanceParameters() = 0;

};

#endif
