#ifndef _SignalModel_h
#define _SignalModel_h

#include <AbstractSignalModel.h>
#include <ParametricSignalModelConfig.h>

#include "Config.h"

#include <vector>
#include <string>

class AbstractSignalModel;
namespace ICHEP2012LikeModel
{
  class SignalModel : public AbstractSignalModel
  {
  protected:
    std::vector<std::string> catnames;
    RooRealVar *hmass;
  
    /** the configuration specific to this signal model */
    Config modelConfig;
  
  
  public:
    virtual void setModelSpecificConfigDict(PyObject *configDict);
  
  
    virtual std::vector<std::string> getCategoryNames();
  
    virtual void setSignalFitVariables(const std::vector<RooRealVar *> &signalFitVariables);
  
    virtual std::list<std::string> getSignalFitVarNames();
  
    virtual AbstractSignalFitModel *getSignalFitModel();
  
    AbstractSystematicsModel *getSystematicsModel(
      const SignalModelParameterInterpolatorMap &fitParamInterpolator,
      const SignalModelParameterInterpolatorMap &effaccInterpolator);
  
  
  };
  
}

#endif
