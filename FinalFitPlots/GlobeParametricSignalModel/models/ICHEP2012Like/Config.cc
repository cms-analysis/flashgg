#include "Config.h"

extern "C"
{
#include <Python.h>
}


#include <PythonConfigUtils.h>
#include <utils.h>

#include <stdexcept>
using namespace ICHEP2012LikeModel;
using namespace std;

//----------------------------------------------------------------------
Config::Config() :
  numCategories(0),
  numInclusiveCategories(0),
  globalConfig(NULL)
{
}

//----------------------------------------------------------------------

Config
Config::readFromPython(PyObject *configDict)
{
  Config retval;

  retval.catNamePrefix = PythonConfigUtils::getDictStringValue(configDict, "catNamePrefix", "cat");

  retval.numCategories = (unsigned) PythonConfigUtils::getDictIntValue(configDict, "numCategories");
  retval.numInclusiveCategories = PythonConfigUtils::getDictIntValue(configDict, "numInclusiveCategories");

  ASSERTBREAK(retval.numCategories >= retval.numInclusiveCategories);

  // read signal MC smearing values
  readSignalSmearingsFromPython(retval, configDict);

  // read number of Gaussians per category / process from python
  readNumGaussiansFromPython(retval, configDict);

  // fit parameter settings
  retval.fitParameterSettings = PythonConfigUtils::getOptionalDict(configDict, "fitParameterSettings");


  return retval;
}

//----------------------------------------------------------------------

void
Config::readSignalSmearingsFromPython(Config &config, PyObject *configDict)
{
  const std::string dictName = "mcSmearings";
  PyObject *sigProcMappingDict = PythonConfigUtils::getOptionalDict(configDict, dictName);

  if (sigProcMappingDict == NULL)
  {
    cerr << dictName << " not specified as part of the model-specific configuration" << endl;
    exit(1);
  }

  // iterate over the items in this dict
  Py_ssize_t position = 0;
  PyObject *key, *value;

  config.smearingv.clear();

  while (PyDict_Next(sigProcMappingDict, &position, &key, &value))
  {
    // key is the source, value is the destination
    string catname = PyString_AsString(key);
    double val = PyFloat_AsDouble(value);

    // no need to check for duplicate sources with python dicts
    // TODO: should check that the categories actually exist
    cout << "XX sETTING " << catname << " = " << val << endl;
    config.smearingv[catname] = val;

  } // loop over items in the dictionary


}

//----------------------------------------------------------------------

void
Config::readNumGaussiansFromPython(Config &config, PyObject *configDict)
{
  const std::string dictName = "numGaussiansMap";
  PyObject *numGaussiansDict = PythonConfigUtils::getOptionalDict(configDict, dictName);

  if (numGaussiansDict == NULL)
  {
    cerr << dictName << " not specified as part of the model-specific configuration" << endl;
    exit(1);
  }

  // iterate over the items in this dict
  Py_ssize_t position = 0;
  PyObject *key, *value;

  config.numGaussians.clear();

  while (PyDict_Next(numGaussiansDict, &position, &key, &value))
  {
    // key is the category name, value is another dict
    string catname = PythonConfigUtils::castToString(key); //PyString_AsString(key);

    Py_ssize_t position2 = 0;
    PyObject *key2, *value2;
    
    while (PyDict_Next(value, &position2, &key2, &value2))
    {
      string rightWrong = PythonConfigUtils::castToString(key2);
      if (rightWrong != "right" && rightWrong != "wrong")
        throw std::runtime_error("expected 'right' or 'wrong' as dictionary key but found '" + rightWrong + "'");


      Py_ssize_t position3 = 0;
      PyObject *key3, *value3;

      while (PyDict_Next(value2, &position3, &key3, &value3))
      {
        string procName = PythonConfigUtils::castToString(key3);

	// copy the value
	long val = PyInt_AsLong(value3);
	if (val < 1)
	  throw std::runtime_error("number of Gaussians must be at least one, found " + toStr(val));

	config.numGaussians[catname][rightWrong][procName] = (unsigned)val;
      } // loop over right/wrong vertex
    } // loop over signal processes
  } // loop over categories
}

//----------------------------------------------------------------------


