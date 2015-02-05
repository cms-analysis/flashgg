#ifndef _SignalModelParameterInterpolator_h
#define _SignalModelParameterInterpolator_h

#include "../ParametricSignalModelConfig.h"

#include <TH1.h>
#include <RooRealVar.h>
#include <RooHistFunc.h>
#include <RooDataHist.h>

#include <string>
#include <vector>
#include <list>

class ReportMaker;

//----------------------------------------------------------------------

/** keeps track of the signal model parameters at various mass points
 *  and can produce functions interpolating the values between mass points
 *  and also has some plotting functionality.
 *
 *  Note that this is not necessarily limited to values coming from a fit,
 *  this can also keep track of other quantities e.g. the efficiency
 *  times acceptance.
 *
 */
class SignalModelParameterInterpolator
{
protected:
  ParametricSignalModelConfig &config;

  struct ValueEntry
  {
    double value;
    double error;
  };

  std::vector<std::string> paramNames;

  /** maps from parameter name to histogram */
  std::map<std::string, TH1F *> fitParamHists;

  /** to keep the values stored here (these do not necessarily all go
   *  to the histograms). Later these could be used to do a
   *  linear interpolation.
   *
   *  First index is the parameter name, the second index
   *  is the higgs mass hypothesis.
   */
  std::map<std::string, std::map<unsigned, ValueEntry> > values;

  std::map<std::string, RooDataHist *> fitparmdatas;

  std::map<std::string, RooHistFunc *> fitparmfuncs;

  /** Higgs mass hypothesis variable */
  RooRealVar *mnom;

  /** true if a histogram or function has been requested already
   *  (for at least one parameter) and we can't call addValue(..) anymore
   */
  bool isFrozen;

  const std::string suffix;

public:

  /** initialize with the given names of parameters
   *
   *  @param suffix is something which distinguishes the names of the ROOT objects generated
   *         by this instance of SignalModelParameterInterpolator from others.
   *         This is typically something like the catname + "_" + sigProcName
   *
   *  */
  SignalModelParameterInterpolator(ParametricSignalModelConfig &config_, 
                                   const std::list<std::string> &paramNames_,
                                   RooRealVar *mnom_, const std::string &suffix_);

  /** set the description of the given parameter */
  void setDescription(const std::string &paramName, const std::string &description);

  /** add the values of a single mass point. Note that the order of the fit parameters
   *  must be the same as the names of the parameters given in the constructor.
   */
  void addValue(const std::string &paramName, int mh, double value, double error);

  /** this takes the error from 'value' */
  void addValue(const std::string &paramName, int mh, const RooRealVar &value);

  RooDataHist *getHistogram(const std::string &paramName);

  RooHistFunc *getFunction(const std::string &paramName);

  /** can be used to test the existence of a parameter */
  bool hasParameter(const std::string &paramName);

  /** will check if the given parameter exists. If not, the list of known parameters
   *  is printed on cerr and an assertion will fail.
   */
  void ensureHasParameter(const std::string &paramName);

  /** access by parameter number */
  RooHistFunc *getFunction(unsigned paramIndex);

protected:

  /** creates a histogram (as function of the mass) with the given name.
   *  This is mostly used to create an interpolating function for
   *  fitted shape parameters afterwards. */
  TH1F *
  makeMassFunctionHist(const std::string &name);

  /** used to check whether we fill a value for the same parameter and mass hypothesis more than once */
  bool hasValueEntry(const std::string &paramName, int mh);

public:
  void makePlot(const std::string &paramName, ReportMaker *reportMaker);

  void makeAllPlots(ReportMaker *reportMaker);

};
//----------------------------------------------------------------------

#endif
