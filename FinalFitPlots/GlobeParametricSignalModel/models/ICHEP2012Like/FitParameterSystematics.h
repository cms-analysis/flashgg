#ifndef _ExampleFitParameterSystematics_h
#define _ExampleFitParameterSystematics_h

#include <string>
#include <vector>
#include <map>

#include <RooFormulaVar.h>
#include "Config.h"

class SignalModelParameterInterpolator;

namespace ICHEP2012LikeModel
{

  /** keeps stuff for systematic uncertainties
   *  on the parameters for the functional form
   *  of the PDFs
   */
  class FitParameterSystematics
  {
  protected:
    const Config &config;

    /** nuisance parameters for smearing of MC (to match data)
     *  The (first) index is the category name
     * */
    std::map<std::string, RooRealVar *> deltaSmearNuisance;

    /** nuisance parameter for the delta mu (1,2,3 fully correlated).
     *  The (first) index is the category name
     *
     *  This (per category delta mass nuisances) is not
     *  used for the ICHEP2012 model but rather a global scale
     *  (i.e. the uncertainties*/
    //std::map<std::string, RooRealVar *> nuissanceDeltaMs;

    RooRealVar *globalScaleNuisance;

    RooRealVar *nominalMass;

    friend class SystematicsModel;
  public:

    FitParameterSystematics(const Config &_config, const std::vector<std::string> &catnames,
                                   RooRealVar *_nominalMass);

    /** first index is the category, 
	second index is the input signal process name,
	third index is the number of the parameter/Gaussian (e.g. 1 for mean1 etc.) 
    */
    std::map<std::string, std::map<std::string, std::map<unsigned, RooFormulaVar *> > > deltaMwithNuisance;
    std::map<std::string, std::map<std::string, std::map<unsigned, RooFormulaVar *> > > wrongDeltaMwithNuisance;

    /** adds nuisance parameters (energy scale uncertainty) to Gaussian means */
    void makeNuisancedMeans(
                            const std::string &catname,
                            const std::string &catPrefix,
                            const std::string &inputSigProcName,
                            SignalModelParameterInterpolator *paramFuncs);


    /** same indexing as for the deltaM parameters */
    std::map<std::string, std::map<std::string, std::map<unsigned, RooFormulaVar *> > > sigmaWithNuisance;
    std::map<std::string, std::map<std::string, std::map<unsigned, RooFormulaVar *> > > wrongSigmaWithNuisance;

    /** maps from category to RooFormulaVar */
    std::map<unsigned, RooFormulaVar *> smearmods;

    // constant ! (index is the category name)
    std::map<std::string, RooConstVar *> mcToDataMassSmearingVar;

    void makeNuisancedSigmas(unsigned icat,
                             const std::string &catname,
                             const std::string &catPrefix,
                             const std::string &inputSigProcName,
                             SignalModelParameterInterpolator *paramFuncs);

  };
}

#endif
