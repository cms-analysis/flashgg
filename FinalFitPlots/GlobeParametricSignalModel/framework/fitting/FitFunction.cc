#include "FitFunction.h"
#include "../utils.h"

using namespace std;

//----------------------------------------------------------------------

FitFunction::FitFunction(RooAbsPdf *func_,
              const std::map<std::string, RooRealVar *> &params_,
              const std::vector<RooAbsPdf *> &componentsToDraw_) :
    func(func_),
    params(params_),
    componentsToDraw(componentsToDraw_)
{
}

//----------------------------------------------------------------------

std::list<std::string>
FitFunction::getParamNames()
{
  std::list<std::string> retval;
  for (std::map<std::string, RooRealVar *>::const_iterator it = params.begin();
       it != params.end();
       ++it)
  {
    retval.push_back(it->second->GetName());
  }

  return retval;
}

//----------------------------------------------------------------------

std::vector<RooRealVar *>
FitFunction::getParameters()
{
  std::vector<RooRealVar *> retval;
  for (std::map<std::string, RooRealVar *>::const_iterator it = params.begin();
       it != params.end();
       ++it)
  {
    retval.push_back(it->second);
  }
  return retval;
}

//----------------------------------------------------------------------

std::string
FitFunction::getParameterShortName(RooRealVar *param)
{
  for (std::map<std::string, RooRealVar *>::iterator it = params.begin();
       it != params.end();
       ++it)
  {
    if (it->second == param)
      return it->first;
  }

  cerr << "Parameter " << param << " not found in list, exiting." << endl;
  ASSERTBREAK(0);

  return NULL;
}

//----------------------------------------------------------------------
