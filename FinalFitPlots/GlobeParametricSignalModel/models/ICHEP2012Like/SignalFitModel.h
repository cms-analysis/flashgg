#ifndef _ExampleSignalFitModel_h
#define _ExampleSignalFitModel_h

#include <fitting/AbstractSignalFitModel.h>

#include <RooRealVar.h>
#include <TString.h>

#include <vector>
#include <string>
#include <ParametricSignalModelConfig.h>

#include "Config.h"

namespace ICHEP2012LikeModel
{
  /** example signal fit model, based on the original macro by Josh */
  class SignalFitModel : public AbstractSignalFitModel
  {
  public:
    //----------------------------------------------------------------------

    /** category names */
    const std::vector<std::string> catnames;

  protected:

    const ParametricSignalModelConfig &config;

    /** model-speficic configuration */
    const Config &modelConfig;
    //----------------------------------------------------------------------
  public:

    SignalFitModel(const ParametricSignalModelConfig &_config, const ICHEP2012LikeModel::Config &_modelConfig, const std::vector<std::string> &catnames_, RooRealVar *hmass, RooRealVar *mnom);

  public:

  protected:
    /** first index is category name, second index is
	process name, value is smearing uncertainty */
    // std::map<std::string, std::map<std::string, double> > smearingUncertainties;

  public:
    //----------------------------------------
    // new interface
    //----------------------------------------

    /** returns the names of the parameters (to be tracked) of the fitting function
     *  for the given category and right/wrong vertex
     */
    virtual std::vector<std::string> getFitFunctionParamNames(const std::string &catname,
    		bool rightVertex, const std::string &sigProcName);

    /** creates the signal pdf with the given parameters.
     */
    virtual RooAbsPdf* makeFitFunction(RooRealVar *massHypothesis, const std::vector<RooRealVar *>& fitVars, 
				       const std::string &catname, bool rightVertex, const std::string &mergedSigProcName,
				       const std::string &funcName, std::vector<RooAbsReal *> &params,
                                       const std::string &prefixForHiddenObjects);

    /** this is called before fitting and should set initial values
     *  and ranges of the given parameter
     */
    // void prepareParameterForFitting(const std::string &paramName, RooRealVar *param);

    virtual void prepareParametersForFitting(int mhyp, const std::string &catname, bool rightVertex, const std::string &mergedSigProcName,
					   const std::map<std::string, RooRealVar *> &fitParams) ;



    /** called after the fit: in this case, it orders the parameters e.g. according
	to the sigma from the fit (in order to have a reasonable evolution
	of the sigmas as function of the mass hypothesis) */
    virtual void postProcessFittedParameters(const std::string &catname, bool rightVertex, const std::string &mergedSigProcName,
					     const std::map<std::string, RooRealVar *> &fittedParams);

    //----------------------------------------
  protected:
    unsigned getNumGaussians(const std::string &catname, bool rightVertex, const std::string &mergedSigProcName);

    class ValueComparator
    {
    protected:
      const std::vector<double> &values;
    public:
    ValueComparator(const std::vector<double> &_values) : 
      values(_values)
      {
      }

      bool operator()(unsigned index1, unsigned index2)
      {
	return values[index1] < values[index2];
      }

    };

    //----------------------------------------------------------------------
  public:

    /** plot the comparison between the fitted functional form and the
     *  mass points after fitting the mass spectrum
     *
     *  @param plotNamePart is typically "rightvtx" or "wrongvtx"
     *
     *  TODO: this makes strong assumptions on the components of the
     *        overall functional form and probably should go into
     *        the class SimpleGaussMass
     */
    void
    drawFittedMass(const std::string &plotNamePart,
                   int mh,
                   const std::string &mergedSigProcName,
                   int bins,
                   const std::vector<RooRealVar *> &fitVariables,

                   // the corresponding dataset we fitted to
                   RooDataSet *mcsigwdata,
                   FitFunction *rightpdf,
                   const std::string &catname,
                   ReportMaker *reportMaker);


  };
}


#endif
