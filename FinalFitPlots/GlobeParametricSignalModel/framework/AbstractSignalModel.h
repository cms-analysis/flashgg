#ifndef AbstractSignalModel_h
#define AbstractSignalModel_h

// the iostream header is used to fix a compilation problem
// when including python.h on MacOS
#include <iostream>

#include "pythonFwd.h"


#include "fitting/AbstractSignalFitModel.h"
#include "AbstractSystematicsModel.h"

#include "interpolation/SignalModelParameterInterpolatorMap.h"

#include <vector>
#include <string>
#include <list>

class RooRealVar;

/** base class for all signal models */
class AbstractSignalModel
{
protected:
  /** overall configuration */
  const ParametricSignalModelConfig *globalConfig;

  /** The Higgs mass hypothesis variable */
  RooRealVar *mnom;

  /** the signal processes found in the input workspace.
   *  I.e. this is BEFORE potentially merging them.
   */
  const std::vector<std::string> *inputSigProcessNames;

public:
  AbstractSignalModel();

  /** must return the names of all categories */
  virtual std::vector<std::string> getCategoryNames() = 0;

  /** this will be called by the framework. There is no need to override this. */
  virtual void setGlobalConfig(const ParametricSignalModelConfig &_config);

  /** this is called to pass the model-specific configuration dict read from
   *  the python configuration file.
   *
   * This is called by the framework AFTER setGlobalConfig(..).
   */
  virtual void setModelSpecificConfigDict(PyObject *configDict) = 0;

  /** this will be called by the framework. There is no need to override this. */
  virtual void setHiggsMassHypothesisVar(RooRealVar *mnom);

  /** will be called by the framework, no need to override */
  virtual void setInputProcessNames(const std::vector<std::string> &inputSigProcessNames);

  //----------------------------------------
  // methods to override
  //----------------------------------------

  /** must return the names of the variables used to
   *  fit the signal. In the simplest case, this is the
   *  reconstructed Higgs candidate mass (e.g. diphoton mass)
   *  but this can also include other variables such
   *  as the pT of the Higgs candidate etc.
   */
  virtual std::list<std::string> getSignalFitVarNames() = 0;

  /** this is called by the 'framework' to set the actual
   *  variable objects taken from the input workspace
   *  (the variables are in the same order as the function
   *  getSignalFitVarNames() returns their names.
   */
  virtual void setSignalFitVariables(const std::vector<RooRealVar *> &signalFitVariables) = 0;

  /** must return an instance of an AbstractSignalFitModel (which
   *  determines how the signal events are fitted)
   */
  virtual AbstractSignalFitModel *getSignalFitModel() = 0;

  /** must return the model for the systematics */
  virtual AbstractSystematicsModel *getSystematicsModel(
      const SignalModelParameterInterpolatorMap &fitParamInterpolator,
      const SignalModelParameterInterpolatorMap &effaccInterpolator) = 0;

  //----------------------------------------
  // end of methods to override
  //----------------------------------------

};

#endif
