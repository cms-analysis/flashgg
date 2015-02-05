#ifndef _ParametricSignalModelConfig_h
#define _ParametricSignalModelConfig_h

#include <string>
#include <map>
#include <vector>

#include <boost/shared_ptr.hpp>

// the iostream header before the python header fixes a compilation
// problem on MacOS
#include <iostream>

#include "pythonFwd.h"


/** class for reading / keeping the configuration given by the user */
class ParametricSignalModelConfig
{
public:
  /** the file name of the shared object containing the model-specific code */
  std::string signalModelSharedObjectName;

  /** name of the file with the signal MC events (and the background model) */
  std::string inputFname;

  /** true if this is to be used for a fermiophobic interpretation of the data */
  bool fermiophobic;

  /** sqrts in TeV. This is used to determine which cross sections to use */
  unsigned sqrts;

  /** this can be set to false in case the input file
   *  does NOT distinguish between right and wrong
   *  vertex. Normally left true.
   */
  bool useRightWrongVertex;

  /** can be used to merge signal process input datasets into a combined
   *  dataset for fitting (e.g. keep ggh as it is while merging the rest).
   *
   *  Maps from the suffix used for the RooDataSet (from the background
   *  workspace) to the name used for the merged dataset.
   */
  std::map<std::string, std::string> signalProcessMergingMapping;

  /** minimum mass (used e.g. for interpolating the fitted parameters) */
  double massmin;

  /** maximum mass (used e.g. for interpolating the fitted parameters) */
  double massmax;

  /** distance (in GeV) between masses used for interpolation */
  double massInterpolationWidth;

  /** weight used for scaling when fitting Gaussians */
  double weightscale;

  /** name of the workspace in the input file */
  std::string inputWorkspaceName;

  /** debugging parameter: skip fits to speedup the program
   *  (e.g. for running with valgrind)
   */
  bool skipFits;

  /** for debugging only (for speeding up the program) */
  bool skipInterpolatedParameterPlots;

  /** name of the (output) html report to be written or
      NULL if no such report should be written */
  boost::shared_ptr<std::string> htmlReportFname;

  /** name of the signal workspace to be produced  */
  std::string signalWorkspaceName;

  /** output filename for the signal model file */
  std::string outputWorkspaceFname;

  /** optinal output workspace with additional information */
  boost::shared_ptr<std::string> outputExtraWorkspaceFname;

  /** number of threds to be used while fitting */
  unsigned numCPU;

  //----------------------------------------

  /** the python dict for reading the model specific configuration */
  PyObject *modelConfigDict;

  //----------------------------------------------------------------------

  /** constructor to initialize some default values */
  ParametricSignalModelConfig();

  /** reads the configuration from a Python file */
  static ParametricSignalModelConfig readPython(const std::string &configFname);

protected:

  static void readSignalProcessMergingFromPython(ParametricSignalModelConfig &config, PyObject *configDict);

private:
  void initializeSignalProcessMergingMapping(bool fermiophobic);

};
#endif
