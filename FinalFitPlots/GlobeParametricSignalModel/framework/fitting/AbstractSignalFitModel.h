#ifndef _AbstractSignalFitModel_h
#define _AbstractSignalFitModel_h

#include <string>
#include <vector>

#include <RooAbsPdf.h>
#include "../ReportMaker.h"
#include "FitFunction.h"


/** abstract base class for specifying a signal fit model */
class AbstractSignalFitModel
{
public:

  /** must return the names of the parameters (to be tracked) of the fitting function
   *  for the given category and right/wrong vertex
   */
  virtual std::vector<std::string> getFitFunctionParamNames(const std::string &catname,
		  bool rightVertex, const std::string &mergedSigProcName) = 0;

  /** this must create a pdf with the given parameters.
   *
   *  This will typically be used for
   *    - fitting
   *    - building the final pdf where the parameters are
   *      functions of the mass hypothesis and have nuisance
   *      parameters attached.
   *
   *  @param params will/must be provided in the same order as the names returned
   *    by getFitFunctionParamNames(..)
   *
   *  @param fitVars is the list of variables which is fitted to the function.
   *   In the simplest case, this is just the reconstructed Higgs candidate
   *   mass but can be several variables (e.g. reconstructed mass, pT).
   *
   *  ----------
   *
   *  Note that in some cases we need to introduce intermediate variables which
   *  should be hidden but still not
   *
   *  An example is the case where we actually want 'delta M' (difference between fitted
   *  mean and the mass hypothesis) as a parameter to the function (i.e. use it as a
   *  function of the mass hypothesis, add nuisances etc.) while the Gaussian function actually
   *  needs the absolute mean (i.e. delta M + mass hypothesis).
   *
   *  Another example is giving names to the pdf components of the overall pdf.
   *
   *  prefixForHiddenObjects should therefore be unique per category and right/wrong vertex.
   *
   *  */
  virtual RooAbsPdf* makeFitFunction(RooRealVar *massHypothesis, const std::vector<RooRealVar *>& fitVars,
				     const std::string &catname, bool rightVertex, const std::string &sigProcName,
				     const std::string &funcName, std::vector<RooAbsReal *> &params,
				     const std::string &prefixForHiddenObjects) = 0;

  /** this is called before fitting and should set initial values
   *  and ranges of the given parameter
   */
  virtual void prepareParametersForFitting(int mhyp, const std::string &catname, bool rightVertex, const std::string &mergedSigProcName,
					   const std::map<std::string, RooRealVar *> &fitParams) = 0;

  /** this is called AFTER the fit and can do some post-processing of the fitted values.
      A typical application is to re-order Gaussians by increasing sigma (to 
      get a more reasonable evolution as function of the mass hypothesis) etc. */
  virtual void postProcessFittedParameters(const std::string &catname, bool rightVertex, const std::string &mergedSigProcName,
					   const std::map<std::string, RooRealVar *> &fittedParams) = 0;

  /** this is used to produce the report, to inspect the quality of the fit by eye. */
  virtual void drawFittedMass(const std::string &plotNamePart,
                 int mh,
                 const std::string &mergedSigProcName,

                 // TODO: this should be determined elsewhere
                 int bins,
                 const std::vector<RooRealVar *> &fitVariables,

                 // the corresponding dataset we fitted to
                 RooDataSet *mcsigwdata,
                 FitFunction *rightpdf,
                 const std::string &catname,
                 ReportMaker *reportMaker) = 0;

};

#endif
