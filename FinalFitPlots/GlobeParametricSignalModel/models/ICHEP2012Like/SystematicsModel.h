#ifndef _SystematicsModel_h
#define _SystematicsModel_h

#include <map>
#include <string>
#include <vector>

#include <interpolation/SignalModelParameterInterpolatorMap.h>
#include "Config.h"

#include <RooAddPdf.h>
#include <TString.h>

#include "FitParameterSystematics.h"

#include <AbstractSystematicsModel.h>

namespace ICHEP2012LikeModel
{

  /** Attaches nuisance parameters to
   *   - the normalization functions
   *   - the fitted parameters (which also evolve as function of the mass hypothesis)
   *
   *
   *  TODO: Should return a list of parameters which are introduced as nuisance parameters.
   *        Useful to later cross check whether the datacard actually holds a specification
   *        for each of the introduced nuisance parameters.
   *
   */
  class SystematicsModel : public AbstractSystematicsModel
  {
  protected:
    const Config &config;

    const std::vector<std::string> &catnames;

    RooRealVar *nominalMass;

    const std::vector<std::string> &inputSigProcessNames;

    // these depend on the input process name
    const SignalModelParameterInterpolatorMap &effAccs;

    const SignalModelParameterInterpolatorMap &fitparmfuncs;

    /** the first index is the category number, the second index is the signal process name */

    /** pdfs per cateogry and signal process where the parameters (mus and sigmas) have been equipped
     *  with nuisance parameters.
     *
     *    //////////////////////////////////////////////////////////////////////
     //////////////////////////////////////////////////////////////////////
     //
     // THIS (hggpdfrel_catX_<proc>) IS THE (PER CATEGORY) PDF USED FOR LIMIT SETTING
     // (WHERE THE NORMALIZATION IS TAKEN FROM hggpdfrel_catX_<proc>_norm)
     //////////////////////////////////////////////////////////////////////
     //////////////////////////////////////////////////////////////////////
     *
     */
    // map<unsigned, map<string, RooAbsPdf *> > hggpdfrel;

    /** the first index is the category name, the second is the input signal process name */
    std::map<std::string, std::map<std::string, RooAbsReal *> > effAccFunctions;

  protected:
    //----------
    RooRealVar *hmass;

    FitParameterSystematics *fitParameterSystematics;

  public:
    /** constructor
     *
     * @param _effAccs depend on the input process name
     *
     * */
    SystematicsModel(const Config &_config,
                            const std::vector<std::string> &_catnames,
                            RooRealVar *_mnom, const std::vector<std::string> &inputSigProcessNames,

                            const SignalModelParameterInterpolatorMap &_effAccs,
                            const SignalModelParameterInterpolatorMap &_fitparmfuncs,

                            RooRealVar *_hmass
                            );


  public:
    /** @return the efficiency * acceptance with all nuisance parameters attached */
    virtual RooAbsReal* getNuisancedEffAcc(const std::string &catname, const std::string &inputSigProcName);

    /** @return the nuisanced version of a parameter for the signal shape */
    virtual RooAbsReal* getNuisancedParameter(const std::string &catname, const std::string &inputSigProcName, const std::string &paramName);

    /** @return the list of all nuisance parameters. Shouldn't these
        actually be RooRealVars ? */
    virtual std::vector<RooAbsReal *> getAllNuisanceParameters();

  protected:
    /**
     * builds nuisanced means and sigmas for the Gaussian PDFs
     *
     * @param fitparmfuncs depend on the MERGED process name first parameter is the category number,
     *        the second name is the process name
     */
    void step1();

    /** adds normalization nuisances such as
     *    - r9 fraction efficiency nuisance
     *    - pt fraction efficiency nuisance
     */
    void step2();

    /** these are efficiency * acceptance with some (normalization) nuisance parametes.
     *  The first index is the category name, the second index is the input signal
     *  process name.
     *  */
    std::map<std::string, std::map<std::string, RooAbsReal*> > nuisancedEffAccs;

    /**
     * Adds the pt,R9 fraction nuisance parameters to the normalization
     *
     * creates the objects named hggpdf_<catX>_<proc>_norm
     *
     * @return a function which represents (cross section) * (efficiency * acceptance)
     *   with some nuisance parameters attached
     *
     *  */
#warning this should be renamed, lumi and xsect nuisances are added in fitSignal.cc
    RooAbsReal* step3MakeEffAccWithLumiAndXsectNuisance(unsigned cat, const std::string &inputSigProcName);

    /** called from step1(..).
     *
     * TODO: rather than rebuilding the PDFs from components, this should re-use the original functional
     * form provided by SignalFitModel (i.e. we should ask the custom code then to re-create a
     * pdf based on these nuisanced quantities which are also functions of the mass hypothesis)
     *  */
    RooAddPdf* makeNuisancedSumOfGaussians(const std::string &title,
                                           const std::string &catname,
                                           const std::string &catPrefix,
                                           const std::string &inputSigProcName,
                                           std::map<std::string, std::map<unsigned, RooFormulaVar *> > &meanWithNuisance,
                                           std::map<std::string, std::map<unsigned, RooFormulaVar *> > &sigmaWithNuisance,
                                           SignalModelParameterInterpolator *paramFuncs);



  protected:
    
  };
}

#endif
