#ifndef _Config_h
#define _Config_h

#include <iostream>

#include <pythonFwd.h>

#include <ParametricSignalModelConfig.h>

/** class configuration which is specific to this model */
namespace ICHEP2012LikeModel
{
  class Config
  {
  public:
    Config();

    /** first part of category names (e.g. 'cat' or 'mvacat'). Default is 'cat'. */
    std::string catNamePrefix;
    
    /** overall number of categories. Should be set to the number of categories
        available in the input file, NOT the categories used for calculating the limits */
    unsigned numCategories;

    /** number of inclusive categories (used to determine which categories are 'special',
     *  i.e. VBF like)
     */
    unsigned numInclusiveCategories;

    /** constants for smearing the mass resolution of
     *  MC (to adapt to the data). Index is the category name.
     */
    std::map<std::string, double> smearingv;

    /** how many Gaussians to use where.
	
	First index is category name,
	second index is 'right' or 'wrong' for the right or wrong vertex assignment,
	third index is the signal process name.
        The value is the number of Gaussians to use.
    */
    std::map<std::string, std::map<std::string, std::map<std::string, unsigned> > > numGaussians;

    /** pointer to the object specifying the ranges and initial values
        of parameters for fitting (can be NULL if unspecified) */
    PyObject *fitParameterSettings;

    /** pointer back to the global configuration */
    const ParametricSignalModelConfig *globalConfig;

  public:
    /** reads the model-specific part from the given python dict */
    static Config readFromPython(PyObject *configDict);

  protected:
    static void readSignalSmearingsFromPython(Config &config, PyObject *configDict);

    static void readNumGaussiansFromPython(Config &config, PyObject *configDict);
  };
}

#endif
