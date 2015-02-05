extern "C"
{
  #include <Python.h>
}

#include "PythonConfigUtils.h"



#include "ParametricSignalModelConfig.h"
#include <TDOMParser.h>

#include "utils.h"

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include <iostream>
#include <cstdlib>


extern "C"
{
#include <Python.h>
}



using namespace std;

//----------------------------------------------------------------------
ParametricSignalModelConfig::ParametricSignalModelConfig() :
  fermiophobic(false),
  useRightWrongVertex(true),
  massmin(-1),
  massmax(-1),
  massInterpolationWidth(-1),
  weightscale(2e3),
  skipFits(false),
  skipInterpolatedParameterPlots(false),
  modelConfigDict(NULL)
{
  // initialize a default signal process mapping
  initializeSignalProcessMergingMapping(fermiophobic);
}

//----------------------------------------------------------------------

void
ParametricSignalModelConfig::readSignalProcessMergingFromPython(ParametricSignalModelConfig &config, PyObject *configDict)
{
  PyObject *sigProcMappingDict = PythonConfigUtils::getOptionalDict(configDict, "signalProcessMerging");

  if (sigProcMappingDict == NULL)
    // keep the default 1:1 mapping
    return;

  std::map<std::string, std::string> newMapping;

  // iterate over the items in this dict
  Py_ssize_t position = 0;
  PyObject *key, *value;

  while (PyDict_Next(sigProcMappingDict, &position, &key, &value))
  {
    // key is the source, value is the destination
    string src = PyString_AsString(key);
    string dest = PyString_AsString(value);

    // make sure the source was in the default mapping
    if (config.signalProcessMergingMapping.count(src) == 0)
    {
      cerr << "illegal input process name '" << src << "' in signal process mapping." << endl;
      exit(1);
    }

    // no need to check for duplicate sources with python dicts
    newMapping[src] = dest;

  } // loop over items in the dictionary


  // check that exactly the processes which were in the default mapping
  // are specified
  // 
  // TODO: check also the other way round
  for (std::map<std::string, std::string>::const_iterator it = config.signalProcessMergingMapping.begin();
       it != config.signalProcessMergingMapping.end();
       ++it)
  {
    if (newMapping.count(it->first) == 0)
    {
      cerr << "no mapping for input signal process '" << it->first << "' defined." << endl;
      exit(1);
    }
  }

  config.signalProcessMergingMapping = newMapping;

}

//----------------------------------------------------------------------

ParametricSignalModelConfig
ParametricSignalModelConfig::readPython(const std::string &configFname)
{
  FILE *configFile = fopen(configFname.c_str(), "r");
  if (configFile == NULL)
  {
    cerr << "could not open configuration file '" << configFname << "'. Exiting." << endl;
    exit(1);
  }

  Py_Initialize();

  // parse the configuration file
  int res = PyRun_SimpleFile(configFile, configFname.c_str());

  if (res != 0)
  {
    cerr << "error reading configuration file '" << configFname << "'" << endl;
    exit(1);
  }

  // get the main python module
  PyObject* mainModule = PyImport_AddModule("__main__");

  // get objects in main module
  PyObject* mainModuleDict = PyModule_GetDict(mainModule);

  PyObject *configDict = PyDict_GetItemString(mainModuleDict, "globalConfig");
  if (configDict == NULL)
  {
    cerr << "could not find an object 'globalConfig' after reading the configuration file '" << configFname << "'. Exiting." << endl;
    exit(1);
  }

  // make sure that this is a dict
  if (! PyDict_Check(configDict))
  {
    cerr << "object 'globalConfig' must be a dict." << endl;
    exit(1);
  }

  //--------------------

  // the return value
  ParametricSignalModelConfig config;

  // store the model specific config also in this config
  config.modelConfigDict = PythonConfigUtils::getOptionalDict(mainModuleDict, "modelConfig");

  //--------------------
  try
  {
    config.signalModelSharedObjectName = PythonConfigUtils::getDictStringValue(configDict, "signalModelLibrary");

    config.inputFname = PythonConfigUtils::getDictStringValue(configDict, "inputFile");

    config.inputWorkspaceName = PythonConfigUtils::getDictStringValue(configDict, "workspace", "cms_hgg_workspace");

    config.sqrts = PythonConfigUtils::getDictIntValue(configDict, "sqrts");

    config.fermiophobic = PythonConfigUtils::getDictBooleanValue(configDict, "fermiophobic");

    config.initializeSignalProcessMergingMapping(config.fermiophobic);

    config.useRightWrongVertex = PythonConfigUtils::getDictBooleanValue(configDict, "useRightWrongVertex", true);

    config.massmin = PythonConfigUtils::getDictDoubleValue(configDict, "minMassHypothesis", 100);
    config.massmax = PythonConfigUtils::getDictDoubleValue(configDict, "maxMassHypothesis", 160);

    config.massInterpolationWidth = PythonConfigUtils::getDictDoubleValue(configDict, "massInterpolationWidth", 10);

    config.massInterpolationWidth = PythonConfigUtils::getDictDoubleValue(configDict, "massInterpolationWidth", 10);

    {
      string *tmp = PythonConfigUtils::getDictStringValuePointer(configDict, "htmlReportFname");
      config.htmlReportFname.reset(tmp);
    }

    config.signalWorkspaceName = PythonConfigUtils::getDictStringValue(configDict, "signalWorkspaceName", "wsig");

    config.outputWorkspaceFname = PythonConfigUtils::getDictStringValue(configDict, "outputWorkspaceFname", "ubersignalmodel.root");    

    {
      string *tmp = PythonConfigUtils::getDictStringValuePointer(configDict, "outputExtraWorkspaceFname");
      config.outputExtraWorkspaceFname.reset(tmp);
    }

    config.numCPU = PythonConfigUtils::getDictIntValue(configDict, "numCPU", 1);

    //----------
    // read the signal process mapping (which input signal processes should be merged
    // into what output process names for fitting the signal shape)
    readSignalProcessMergingFromPython(config, configDict);

    //----------

  }
  catch (...)
  {
    cerr << "exception caught while reading configuration file " << configFname << endl;
    exit(1);
  }
  //--------------------


  // don't call Py_Finalize(..) as we want to read the model specific config
  // later on
  // Py_Finalize();
  fclose(configFile);
  return config;
}

//----------------------------------------------------------------------


void
ParametricSignalModelConfig::initializeSignalProcessMergingMapping(bool fermiophobic)
{
  signalProcessMergingMapping.clear();

  if (true)
  {
    // default mapping
    if (! fermiophobic)
    {
      signalProcessMergingMapping["ggh"] = "ggh";
      signalProcessMergingMapping["vbf"] = "vbf";
      signalProcessMergingMapping["wzh"] = "wzh";
      signalProcessMergingMapping["tth"] = "tth";
    }
    else
    {
      signalProcessMergingMapping["vbf"] = "vbf";
      signalProcessMergingMapping["wzh"] = "wzh";
    }
  }

//  if (false)
//  {
//    // grouping into ggh and the rest
//    if (! fermiophobic)
//    {
//      signalProcessMergingMapping["ggh"] = "ggh";
//      signalProcessMergingMapping["tth"] = "other";
//      signalProcessMergingMapping["vbf"] = "other";
//      signalProcessMergingMapping["wzh"] = "other";
//    }
//    else
//    {
//      // fermiophobic (add wzh to vbf for fitting)
//      signalProcessMergingMapping["vbf"] = "vbf";
//      signalProcessMergingMapping["wzh"] = "vbf";
//    }
//
//  }

}
