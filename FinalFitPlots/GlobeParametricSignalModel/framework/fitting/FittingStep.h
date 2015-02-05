#ifndef _FittingStep_h
#define _FittingStep_h

#include <TH2.h>

#include "../interpolation/SignalModelParameterInterpolatorMap.h"
#include "../DataSetList.h"

#include "../ParametricSignalModelConfig.h"


#include <string>
#include <vector>

class SignalModelParameterInterpolator;
class ReportMaker;
class FitFunction;
class AbstractSignalFitModel;

/** performs the fitting over all masses, categories, signal processes and right/wrong vertices */
class FittingStep
{
protected:
  ParametricSignalModelConfig &config;

  const std::vector<std::string> &catnames;

  const std::vector<std::string> &mergedSigProcessNames;

  const std::vector<int> &mhs;

  AbstractSignalFitModel *signalFitModel;

  // variable for the higss mass hypothesis
  RooRealVar *mnom;

  std::vector<RooRealVar *> fitVariables;

  SignalModelParameterInterpolatorMap &fitParamInterpolator;

  DataSetList &dataSets;

  ReportMaker *reportMaker;

public:


  //----------------------------------------------------------------------

  /** @param mnom variable for the higss mass hypothesis
   *  @param hmass the reconstructed Higgs mass variable
   * */
  FittingStep(ParametricSignalModelConfig &_config,
      DataSetList &_dataSets,
      const std::vector<std::string> & _catnames,
      const std::vector<std::string> &_mergedSigProcessNames,
      const std::vector<int> &mhs,
      AbstractSignalFitModel *_signalFitModel,
      RooRealVar *mnom,
      const std::vector<RooRealVar *> &_fitVariables,

      SignalModelParameterInterpolatorMap &_fitParamInterpolator,
      ReportMaker *_reportMaker
  );

  /** fits all the events to the functions.
      @param debugWorkspace if non-null, some objects are imported into it
      (e.g. to be used for later investigation by hand)
  */
  void run(RooWorkspace *debugWorkspace);

  /** prints information about failed fits. @return true if there were failed fits. */
  bool printFailedFits(std::ostream &os);

  //----------------------------------------------------------------------


protected:

  void initialize();

  FitFunction* makeFittingFunction(const std::string &catname, bool rightVertex, const std::string &mergedSigProcName);

public:

  /** string describing which fits failed (to be printed at the end) (public for the moment) */
  std::vector<std::string> failedFitsDescriptions;

};


#endif
