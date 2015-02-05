#ifndef _Fitter_h
#define _Fitter_h

#include "FitFunction.h"
#include "../DataSetList.h"

#include "../ParametricSignalModelConfig.h"
#include "../interpolation/SignalModelParameterInterpolator.h"
#include "../utils.h"

#include "AbstractSignalFitModel.h"

#include <RooAbsPdf.h>
#include <RooGaussian.h>
#include <RooFit.h>
#include <RooDataSet.h>
#include <RooFitResult.h>

#include <TH2.h>

#include <boost/lexical_cast.hpp>

#include <set>
#include <vector>
#include <sstream>

class ReportMaker;

//----------------------------------------------------------------------

/** class for fitting and drawing the fitted functions vs. data
 *
 *  TODO: how to draw in the cases where the function to be fitted
 *        is a function of more than two variables ?
 *  */
class Fitter
{
protected:
  ParametricSignalModelConfig config;

  SignalModelParameterInterpolator *interpolator;

  DataSetList &dataSets;

public:
  //----------------------------------------------------------------------

  /** @param hmass what is this used for ? Just for plotting ? Does not generalize to 2D fits */
  Fitter(ParametricSignalModelConfig &config_,
         DataSetList &_dataSets,
         int mh,
         const std::string& catname,
         const std::string& mergedSigProcName,
         RooRealVar* mnom,
         const std::vector<RooRealVar*> &fitVariables,

         FitFunction* rightpdf,
         FitFunction* wrongpdf,

         RooRealVar& fracright,

         // TODO: should we split this into right and wrong vertex ?
	 SignalModelParameterInterpolator *interpolator_,

	 AbstractSignalFitModel *signalFitModel,

	 ReportMaker *reportMaker,

	 /** mostly for debugging / testing */
	 RooWorkspace *debuggingWorkspace
  );

  void fit();

  /** fills the histograms to be used later for the interpolation with the fitted
   *  parameter values
   */
  void fillParameterHistograms(const std::string &catname, FitFunction *pdf, int& mh);
  //----------------------------------------------------------------------

  // public for the moment, should implement the corresponding getters
  int fitStatusRightVertex, fitStatusWrongVertex;

 protected:
  /** mostly for debugging */
  void printParameters(const std::map<std::string, RooRealVar *> &fitParams) const;


};
#endif
