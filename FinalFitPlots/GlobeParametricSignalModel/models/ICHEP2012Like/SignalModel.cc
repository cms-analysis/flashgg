#include "SignalModel.h"

#include "SignalFitModel.h"
#include "SystematicsModel.h"

#include <utils.h>

//----------------------------------------------------------------------
#ifndef __CINT__
#include <boost/assign/list_of.hpp>
using namespace boost::assign;
#include <boost/foreach.hpp>

#endif

using namespace std;

//----------------------------------------------------------------------
// instantiator function
//----------------------------------------------------------------------
extern "C"
{
  AbstractSignalModel *makeInstance()
  {
    return new ICHEP2012LikeModel::SignalModel();
  }
}

//----------------------------------------------------------------------

using namespace ICHEP2012LikeModel;

void
SignalModel::setModelSpecificConfigDict(PyObject *configDict)
{
  modelConfig = Config::readFromPython(configDict);

  // note that this is called AFTER setting the global config
  modelConfig.globalConfig = this->globalConfig;

  // check that each category was actually assigned a MC smearing uncertainty value assigned
  getCategoryNames();
  BOOST_FOREACH(string catname, catnames)
  {
    if (modelConfig.smearingv.count(catname) == 0)
    {
      cerr << "could not find an MC smearing for category '" << catname << "'" << endl;
      exit(1);
    }
  }

}

//----------------------------------------------------------------------

std::list<std::string>
SignalModel::getSignalFitVarNames()
{
  return list_of("CMS_hgg_mass");
}

//----------------------------------------------------------------------

void
SignalModel::setSignalFitVariables(const std::vector<RooRealVar *> &signalFitVariables)
{
  ASSERTBREAK(signalFitVariables.size() == 1);
  hmass = signalFitVariables.at(0);

  // TODO: massmax in the global configuration refers to
  //       the mass hypothesis, NOT the maximum reconstructed
  //       mass. We should be careful NOT to restrict
  //       this range below where we can have data events
  //       (otherwise, 'combine' will fail)

  // hmass->setRange(100, globalConfig->massmax);
  // hmass->setBins(2.0 * (globalConfig->massmax - 100.0));
  hmass->SetTitle("m_{#gamma#gamma}");
  hmass->setUnit("GeV");

  // do we need this ?
  // hmass->setRange("higgsrange", 100.0, globalConfig->massmax);

}

//----------------------------------------------------------------------

std::vector<std::string>
SignalModel::getCategoryNames()
{
  if (catnames.empty())
  {
    // initialize the category names
    for (unsigned i = 0; i < modelConfig.numCategories; ++i)
      catnames.push_back(modelConfig.catNamePrefix + toStr(i));
  }

  return catnames;
}

//----------------------------------------------------------------------

AbstractSignalFitModel *
SignalModel::getSignalFitModel()
{
  return new SignalFitModel(*globalConfig, modelConfig, catnames, hmass, mnom);
}

//----------------------------------------------------------------------
AbstractSystematicsModel *
SignalModel::getSystematicsModel(const SignalModelParameterInterpolatorMap &fitParamInterpolator,
	  const SignalModelParameterInterpolatorMap &effaccInterpolator)
{
  return new SystematicsModel(modelConfig, catnames,
	        mnom, *inputSigProcessNames, effaccInterpolator, fitParamInterpolator, hmass);
}

//----------------------------------------------------------------------
